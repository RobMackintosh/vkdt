#include "modules/api.h"
#include "core/core.h"
#include "core/fs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libavutil/avassert.h>
#include <libavutil/channel_layout.h>
#include <libavutil/opt.h>
#include <libavutil/mathematics.h>
#include <libavutil/timestamp.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>

typedef struct output_stream_t
{ // a wrapper around a single output AVStream
  AVStream *st;
  AVCodecContext *enc;

  /* pts of the next frame that will be generated */
  int64_t next_pts;
  int samples_count; // used to compute time stamp for audio. XXX can this overflow?

  AVFrame *frame;
  AVFrame *tmp_frame;

  AVPacket *tmp_pkt;
  struct SwrContext *swr_ctx;
}
output_stream_t;

typedef struct buf_t
{
  AVFormatContext *oc;
  const AVCodec  *audio_codec, *video_codec;
  output_stream_t audio_stream, video_stream;
  int audio_mod;   // the module on our graph that has the audio
  int have_buf[3]; // flag that we have read the buffers for Y Cb Cr for the given frame
}
buf_t;

static inline void
close_stream(AVFormatContext *oc, output_stream_t *ost)
{
  avcodec_free_context(&ost->enc);
  av_frame_free(&ost->frame);
  av_frame_free(&ost->tmp_frame);
  av_packet_free(&ost->tmp_pkt);
  swr_free(&ost->swr_ctx);
}

static inline void
close_file(buf_t *dat)
{
  if(!dat->oc) return;
  av_write_trailer(dat->oc);

  close_stream(dat->oc, &dat->video_stream);
  close_stream(dat->oc, &dat->audio_stream);

  // if (!(fmt->flags & AVFMT_NOFILE))
  avio_closep(&dat->oc->pb);

  avformat_free_context(dat->oc);
  dat->oc = 0;
}

static inline int
add_stream(
    dt_module_t     *mod,
    output_stream_t *ost,
    AVFormatContext *oc,
    const AVCodec  **codec,
    enum AVCodecID   codec_id)
{
  // buf_t *dat = mod->data;
  AVCodecContext *c;

  // XXX TODO if video
  // avcodec_find_encoder(AV_CODEC_ID_H265);

  *codec = avcodec_find_encoder(codec_id);
  if (!(*codec))
  {
    fprintf(stderr, "[o-vid] could not find encoder for '%s'\n", avcodec_get_name(codec_id));
    return 1;
  }

  ost->tmp_pkt = av_packet_alloc();
  if (!ost->tmp_pkt)
  {
    fprintf(stderr, "[o-vid] could not allocate AVPacket\n");
    return 1;
  }

  ost->st = avformat_new_stream(oc, NULL);
  if (!ost->st)
  {
    fprintf(stderr, "[o-vid] could not allocate stream\n");
    return 1;
  }
  ost->st->id = oc->nb_streams-1;
  c = avcodec_alloc_context3(*codec);
  if (!c)
  {
    fprintf(stderr, "[o-vid] could not alloc an encoding context\n");
    return 1;
  }
  ost->enc = c;

  // const float p_quality = dt_module_param_float(mod, 1)[0];
  // const int   p_codec   = dt_module_param_int  (mod, 2)[0];
  // const int   p_profile = dt_module_param_int  (mod, 3)[0];
  // const int   p_colour  = dt_module_param_int  (mod, 4)[0];
  const float frame_rate = mod->graph->frame_rate > 0.0f ? mod->graph->frame_rate : 24;

  switch ((*codec)->type)
  {
    case AVMEDIA_TYPE_AUDIO:
      c->sample_fmt  = (*codec)->sample_fmts ? (*codec)->sample_fmts[0] : AV_SAMPLE_FMT_FLTP;
      c->bit_rate    = 64000;
      c->sample_rate = 44100;
      if ((*codec)->supported_samplerates)
      {
        c->sample_rate = (*codec)->supported_samplerates[0];
        for (int i = 0; (*codec)->supported_samplerates[i]; i++)
        {
          if ((*codec)->supported_samplerates[i] == 44100) c->sample_rate = 44100;
        }
      }
      av_channel_layout_copy(&c->ch_layout, &(AVChannelLayout)AV_CHANNEL_LAYOUT_STEREO);
      ost->st->time_base = (AVRational){ 1, c->sample_rate };
      break;

    case AVMEDIA_TYPE_VIDEO:
      c->codec_id = codec_id;
      /* timebase: This is the fundamental unit of time (in seconds) in terms
       * of which frame timestamps are represented. For fixed-fps content,
       * timebase should be 1/framerate and timestamp increments should be
       * identical to 1. */
      ost->st->time_base = (AVRational){ 1, frame_rate };
      c->time_base       = ost->st->time_base;

      // XXX TODO get from output parameters/defaults for h264 or something
      // c->bit_rate = 400000;
      /* Resolution must be a multiple of two. */
      c->width    = mod->connector[0].roi.wd & ~1;
      c->height   = mod->connector[0].roi.ht & ~1;
      // c->gop_size = 12; /* emit one intra frame every twelve frames at most */
      c->pix_fmt  = AV_PIX_FMT_YUV420P;
#if 0
      if (c->codec_id == AV_CODEC_ID_MPEG2VIDEO) {
        /* just for testing, we also add B-frames */
        c->max_b_frames = 2;
      }
      if (c->codec_id == AV_CODEC_ID_MPEG1VIDEO) {
        /* Needed to avoid using macroblocks in which some coeffs overflow.
         * This does not happen with normal video, it just happens here as
         * the motion of the chroma plane does not match the luma plane. */
        c->mb_decision = 2;
      }
#endif
      /// Compression efficiency (slower -> better quality + higher cpu%)
      /// [ultrafast, superfast, veryfast, faster, fast, medium, slow, slower, veryslow]
      /// Set this option to "ultrafast" is critical for realtime encoding
      av_opt_set(c->priv_data, "preset", "ultrafast", 0);

      /// Compression rate (lower -> higher compression) compress to lower size, makes decoded image more noisy
      /// Range: [0; 51], sane range: [18; 26]. I used 35 as good compression/quality compromise. This option also critical for realtime encoding
      av_opt_set(c->priv_data, "crf", "35", 0);

      /// Change settings based upon the specifics of input
      /// [psnr, ssim, grain, zerolatency, fastdecode, animation]
      /// This option is most critical for realtime encoding, because it removes delay between 1th input frame and 1th output packet.
      av_opt_set(c->priv_data, "tune", "zerolatency", 0);
      break;

    default:
      break;
  }

  /* Some formats want stream headers to be separate. */
  if (oc->oformat->flags & AVFMT_GLOBALHEADER)
    c->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
  return 0;
}

static inline AVFrame*
alloc_frame(enum AVPixelFormat pix_fmt, int width, int height)
{
  AVFrame *frame = av_frame_alloc();
  if (!frame) return 0;

  frame->format = pix_fmt;
  frame->width  = width;
  frame->height = height;

  /* allocate the buffers for the frame data */
  if(av_frame_get_buffer(frame, 0) < 0)
  {
    fprintf(stderr, "[o-vid] could not allocate frame data.\n");
    return 0;
  }
  return frame;
}

static inline void
open_video(
    AVFormatContext *oc, const AVCodec *codec,
    output_stream_t *ost, AVDictionary *opt_arg)
{
  AVCodecContext *c = ost->enc;
  AVDictionary *opt = NULL;

  av_dict_copy(&opt, opt_arg, 0);

  /* open the codec */
  int ret = avcodec_open2(c, codec, &opt);
  av_dict_free(&opt);
  if (ret < 0)
  {
    fprintf(stderr, "[o-vid] could not open video codec: %s\n", av_err2str(ret));
    return;
  }

  /* allocate and init a re-usable frame */
  ost->frame = alloc_frame(c->pix_fmt, c->width, c->height);
  if (!ost->frame)
  {
    fprintf(stderr, "[o-vid] could not allocate video frame\n");
    return;
  }

#if 0
  /* If the output format is not YUV420P, then a temporary YUV420P
   * picture is needed too. It is then converted to the required
   * output format. */
  ost->tmp_frame = NULL;
  if (c->pix_fmt != AV_PIX_FMT_YUV420P)
  {
    ost->tmp_frame = alloc_frame(AV_PIX_FMT_YUV420P, c->width, c->height);
    if (!ost->tmp_frame)
    {
      fprintf(stderr, "[o-vid] could not allocate temporary video frame\n");
      return;
    }
  }
#endif

  /* copy the stream parameters to the muxer */
  if(avcodec_parameters_from_context(ost->st->codecpar, c) < 0)
  {
    fprintf(stderr, "[o-vid] could not copy the stream parameters\n");
    return;
  }
}

static inline AVFrame*
alloc_audio_frame(
    enum AVSampleFormat sample_fmt,
    const AVChannelLayout *channel_layout,
    int sample_rate, int nb_samples)
{
  AVFrame *frame = av_frame_alloc();
  if (!frame)
  {
    fprintf(stderr, "[o-vid] error allocating an audio frame\n");
    return 0;
  }

  frame->format = sample_fmt;
  av_channel_layout_copy(&frame->ch_layout, channel_layout);
  frame->sample_rate = sample_rate;
  frame->nb_samples = nb_samples;

  if (nb_samples && av_frame_get_buffer(frame, 0) < 0)
  {
    fprintf(stderr, "[o-vid] error allocating an audio buffer\n");
    return 0;
  }
  return frame;
}

static inline void
open_audio(
    AVFormatContext *oc, const AVCodec *codec,
    output_stream_t *ost, AVDictionary *opt_arg)
{
  AVCodecContext *c;
  int nb_samples;
  int ret;
  AVDictionary *opt = NULL;

  c = ost->enc;

  av_dict_copy(&opt, opt_arg, 0);
  ret = avcodec_open2(c, codec, &opt);
  av_dict_free(&opt);
  if (ret < 0)
  {
    fprintf(stderr, "[o-vid] could not open audio codec: %s\n", av_err2str(ret));
    return;
  }

#if 0
  /* init signal generator */
  ost->t     = 0;
  ost->tincr = 2 * M_PI * 110.0 / c->sample_rate;
  /* increment frequency by 110 Hz per second */
  ost->tincr2 = 2 * M_PI * 110.0 / c->sample_rate / c->sample_rate;
#endif

  if (c->codec->capabilities & AV_CODEC_CAP_VARIABLE_FRAME_SIZE)
    nb_samples = 10000;
  else
    nb_samples = c->frame_size;

  ost->frame = alloc_audio_frame(c->sample_fmt, &c->ch_layout,
      c->sample_rate, nb_samples);
  // XXX TODO this is the frame as it comes from our audio module: we need to init it according to audio_mod img_param sound properties!
  ost->tmp_frame = alloc_audio_frame(AV_SAMPLE_FMT_S16, &c->ch_layout,
      c->sample_rate, nb_samples);

  /* copy the stream parameters to the muxer */
  if(avcodec_parameters_from_context(ost->st->codecpar, c) < 0)
  {
    fprintf(stderr, "[o-vid] could not copy the stream parameters\n");
    return;
  }

  /* create resampler context */
  ost->swr_ctx = swr_alloc();
  if (!ost->swr_ctx)
  {
    fprintf(stderr, "[o-vid] could not allocate resampler context\n");
    return;
  }

  /* set options */
  av_opt_set_chlayout  (ost->swr_ctx, "in_chlayout",       &c->ch_layout,      0);
  av_opt_set_int       (ost->swr_ctx, "in_sample_rate",     c->sample_rate,    0);
  av_opt_set_sample_fmt(ost->swr_ctx, "in_sample_fmt",      AV_SAMPLE_FMT_S16, 0);
  av_opt_set_chlayout  (ost->swr_ctx, "out_chlayout",      &c->ch_layout,      0);
  av_opt_set_int       (ost->swr_ctx, "out_sample_rate",    c->sample_rate,    0);
  av_opt_set_sample_fmt(ost->swr_ctx, "out_sample_fmt",     c->sample_fmt,     0);

  /* initialize the resampling context */
  if ((ret = swr_init(ost->swr_ctx)) < 0)
  {
    fprintf(stderr, "[o-vid] failed to initialize the resampling context\n");
    return;
  }
}

static inline int
open_file(dt_module_t *mod)
{
  buf_t *dat = mod->data;
  if(dat->oc) close_file(dat);
  dat->audio_mod = -1;
  // XXX disable for now
  // for(int i=0;i<mod->graph->num_modules;i++)
    // if(mod->graph->module[i].name && mod->graph->module[i].so->audio) { dat->audio_mod = i; break; }
  const char *basename  = dt_module_param_string(mod, 0);
  char filename[512];
  snprintf(filename, sizeof(filename), "%s.mp4", basename);
  fprintf(stderr, "opening file %s\n", filename);

  const int width  = mod->connector[0].roi.wd & ~1;
  const int height = mod->connector[0].roi.ht & ~1;
  fprintf(stderr, "size %d x %d\n", width, height);
  if(width <= 0 || height <= 0) return 1;

  // TODO insert the usual dance around filename caching and resource searchpaths
  /* allocate the output media context */
  // TODO: use mp4 for h264 and mov for prores
  // TODO: append correct extension?
  avformat_alloc_output_context2(&dat->oc, NULL, "mp4", filename);
  if (!dat->oc) return 1;

  const AVOutputFormat *fmt = dat->oc->oformat;

  /* Add the audio and video streams using the default format codecs
   * and initialize the codecs. */
  // if (fmt->video_codec != AV_CODEC_ID_NONE)
    add_stream(mod, &dat->video_stream, dat->oc, &dat->video_codec, fmt->video_codec);
  // if (fmt->audio_codec != AV_CODEC_ID_NONE)
    add_stream(mod, &dat->audio_stream, dat->oc, &dat->audio_codec, fmt->audio_codec);

  AVDictionary *opt = NULL;
  open_video(dat->oc, dat->video_codec, &dat->video_stream, opt);
  open_audio(dat->oc, dat->audio_codec, &dat->audio_stream, opt);

  int ret = avio_open(&dat->oc->pb, filename, AVIO_FLAG_WRITE);
  if (ret < 0)
  {
    fprintf(stderr, "[o-vid] could not open '%s': %s\n", filename, av_err2str(ret));
    return 1;
  }

  /* Write the stream header, if any. */
  ret = avformat_write_header(dat->oc, &opt);
  if (ret < 0)
  {
    fprintf(stderr, "[o-vid] error occurred when opening output file: %s\n", av_err2str(ret));
    return 1;
  }
  return 0;
}

static inline int
write_frame(
    AVFormatContext *fmt_ctx, AVCodecContext *c,
    AVStream *st, AVFrame *frame, AVPacket *pkt)
{
  // send the frame to the encoder
  int ret = avcodec_send_frame(c, frame);
  if (ret < 0)
  {
    fprintf(stderr, "[o-vid] crror sending a frame to the encoder: %s\n", av_err2str(ret));
    return 1;
  }

  while (ret >= 0)
  {
    ret = avcodec_receive_packet(c, pkt);
    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
      break;
    else if (ret < 0)
    {
      fprintf(stderr, "[o-vid] error encoding a frame: %s\n", av_err2str(ret));
      return 1;
    }

    /* rescale output packet timestamp values from codec to stream timebase */
    av_packet_rescale_ts(pkt, c->time_base, st->time_base);
    pkt->stream_index = st->index;

    /* Write the compressed frame to the media file. */
    // output some stuff:
    // log_packet(fmt_ctx, pkt);
    ret = av_interleaved_write_frame(fmt_ctx, pkt);
    /* pkt is now blank (av_interleaved_write_frame() takes ownership of
     * its contents and resets pkt), so that no unreferencing is necessary.
     * This would be different if one used av_write_frame(). */
    if (ret < 0)
    {
      fprintf(stderr, "[o-vid] error while writing output packet: %s\n", av_err2str(ret));
      return 1;
    }
  }
  if(ret == AVERROR_EOF) return 1;
  return 0;
}

// =================================================
//  module api callbacks
// =================================================

int init(dt_module_t *mod)
{
  buf_t *dat = malloc(sizeof(*dat));
  memset(dat, 0, sizeof(*dat));
  mod->data = dat;
  mod->flags = s_module_request_write_sink;
  return 0;
}

void cleanup(dt_module_t *mod)
{
  if(!mod->data) return;
  buf_t *dat = mod->data;
  close_file(dat);
  free(dat);
  mod->data = 0;
}

void create_nodes(
    dt_graph_t  *graph,
    dt_module_t *module)
{ // encode kernel, wire sink modules for Y Cb Cr
  // TODO this is the place where we could padd wd to be linesize for ffmpeg frames
  const int wd = module->connector[0].roi.wd & ~1;
  const int ht = module->connector[0].roi.ht & ~1;
  dt_roi_t roi_Y  = { .wd = wd, .ht = ht };
  dt_roi_t roi_CbCr = roi_Y;
  // TODO: if 422 or 420 roi_CbCr.wd /= 2
  roi_CbCr.wd /= 2;
  // TODO: if 420 roi_CbCr.ht /= 2
  roi_CbCr.ht /= 2;
  // TODO: pass push constants identifying the buffer input and output characteristics (bit depth, colour space, trc, subsampling)
  const int id_enc = dt_node_add(graph, module, "o-vid", "enc", wd, ht, 1, 0, 0, 4,
      "input", "read", "rgba", "f16", dt_no_roi,
      "Y",     "write", "r",   "ui8", &roi_Y, // TODO: if more than 8 bits, use ui16!
      "Cb",    "write", "r",   "ui8", &roi_CbCr, // TODO: if more than 8 bits, use ui16!
      "Cr",    "write", "r",   "ui8", &roi_CbCr);// TODO: if more than 8 bits, use ui16!
  const int id_Y  = dt_node_add(graph, module, "o-vid", "Y",  1, 1, 1, 0, 0, 1,
      "Y",  "sink", "r", "ui8", dt_no_roi);
  const int id_Cb = dt_node_add(graph, module, "o-vid", "Cb", 1, 1, 1, 0, 0, 1,
      "Cb", "sink", "r", "ui8", dt_no_roi);
  const int id_Cr = dt_node_add(graph, module, "o-vid", "Cr", 1, 1, 1, 0, 0, 1,
      "Cr", "sink", "r", "ui8", dt_no_roi);
  CONN(dt_node_connect_named(graph, id_enc, "Y",  id_Y,  "Y"));
  CONN(dt_node_connect_named(graph, id_enc, "Cb", id_Cb, "Cb"));
  CONN(dt_node_connect_named(graph, id_enc, "Cr", id_Cr, "Cr"));
  dt_connector_copy(graph, module, 0, id_enc, 0);
}

// TODO: which data do we need?
// yuv422p10le : y full res, cb and cr half res in x, full res in y
// yuva444p10le : y, cb, cr, alpha all fullres 10 bits padded to 16 bits
// yuv420p for h264, which is the same as in this example, 8bps for all channels
// there is also
// yuv444p16le which we could use as input, but it only scales the data (it is padded to 16 in the case of 10bits)
// TODO: encoding kernel that inputs anything and will output y + chroma texture
// TODO: fill alpha with constant 1023
void write_sink(
    dt_module_t            *mod,
    void                   *buf,
    dt_write_sink_params_t *p)
{
  buf_t *dat = mod->data;
  if(mod->graph->frame < mod->graph->frame_cnt-1 && !dat->oc) open_file(mod);
  if(!dat->oc) return; // avoid crashes in case opening the file went wrong
  output_stream_t *vost = &dat->video_stream;

  const int wd = mod->connector[0].roi.wd & ~1;
  const int ht = mod->connector[0].roi.ht & ~1;
  // fprintf(stderr, "conn size %d node %d\n", wd, p->node->connector[0].roi.wd);

  /* when we pass a frame to the encoder, it may keep a reference to it
   * internally; make sure we do not overwrite it here */
  if (av_frame_make_writable(vost->frame) < 0) return;

  uint8_t *mapped8 = buf; // TODO: or 16 bit
    // is what we do below with the memcpy
    // fill_yuv_image(ost->frame, ost->next_pts, c->width, c->height);
  if(p->node->kernel == dt_token("Y"))
  {
    // if(wd != vost->frame->linesize[0]) fprintf(stderr, "linesize 0 wrong! %d %d\n", wd, vost->frame->linesize[0]);
    // TODO can we pass the mapped memory to ffmpeg as frame directly?
    for(int j=0;j<ht;j++)
      memcpy(&vost->frame->data[0][j * vost->frame->linesize[0]], mapped8 + j*wd, sizeof(uint8_t)*wd);
    // memcpy(vost->frame->data[0], buf, sizeof(uint8_t)*wd*ht);
        // ost->frame->data[0][y * ost->frame->linesize[0] + x] = x + y + i * 3;
    dat->have_buf[0] = 1;
  }
  else if(p->node->kernel == dt_token("Cb"))
  { // ffmpeg expects the colour planes separate, not in a 2-channel texture
    // something memcpy with subsampled size. make sure linesize and gpu width match!
    // if(wd/2 != vost->frame->linesize[1]) fprintf(stderr, "linesize 1 wrong!\n");
    // memcpy(vost->frame->data[1], buf, sizeof(uint8_t)*wd/2*ht/2);
    for(int j=0;j<ht/2;j++)
      memcpy(&vost->frame->data[1][j * vost->frame->linesize[1]], mapped8 + j*(wd/2), sizeof(uint8_t)*wd/2);
    dat->have_buf[1] = 1;
  }
  else if(p->node->kernel == dt_token("Cr"))
  {
    // if(wd/2 != vost->frame->linesize[2]) fprintf(stderr, "linesize 2 wrong!\n");
    // memcpy(vost->frame->data[2], buf, sizeof(uint8_t)*wd/2*ht/2);
    for(int j=0;j<ht/2;j++)
      memcpy(&vost->frame->data[2][j * vost->frame->linesize[2]], mapped8+ j*(wd/2), sizeof(uint8_t)*wd/2);
    dat->have_buf[2] = 1;
  }
  // alpha has no connector and copies no data

  if(dat->have_buf[0] && dat->have_buf[1] && dat->have_buf[2])
  { // if we have all three channels for a certain frame:
    // XXX TODO: compare to our total length (last timestamp? assume fixed frame rate?)
    /* check if we want to generate more frames */
    // if (av_compare_ts(vost->next_pts, vc->time_base, STREAM_DURATION, (AVRational){ 1, 1 }) > 0) return;

    vost->frame->pts = vost->next_pts++;

    { // write video frame
      // TODO: fill ost->frame->data[3] with constant alpha, maybe suffices to do once
      write_frame(dat->oc, vost->enc, vost->st, vost->frame, vost->tmp_pkt);
      // now it says its finished maybe
    }

    // TODO: we will *not* get an audio callback, we need to grab the samples from the audio module

    if(dat->audio_mod >= 0)
    { // write audio frame
      output_stream_t *ost = &dat->audio_stream;
      AVCodecContext *c;
      AVFrame *frame = ost->tmp_frame;
      int ret;
      int dst_nb_samples;

      c = ost->enc;


      /* check if we want to generate more frames */ // XXX we are not buffering the audio otherwise. this better work out exactly!
      // if (av_compare_ts(ost->next_pts, ost->enc->time_base, STREAM_DURATION, (AVRational){ 1, 1 }) <= 0)
      {

        // TODO: make sure sample_cnt <= frame.nb_samples
        uint16_t *samples = 0;
        int sample_cnt = mod->graph->module[dat->audio_mod].so->audio(mod->graph->module+dat->audio_mod, mod->graph->frame, &samples);
        memcpy(frame->data[0], samples, sizeof(uint16_t)*2*sample_cnt);
        // TODO: memcpy samples to frame->data[0], it is both interleaved stereo S16 (or otherwise set to the same when initing the audio frame)
        // TODO: during audio frame init: query img_params for sound properties
        frame->pts = ost->next_pts;
        ost->next_pts += sample_cnt;// frame->nb_samples;

        /* convert samples from native format to destination codec format, using the resampler */
        /* compute destination number of samples */
        dst_nb_samples = av_rescale_rnd(swr_get_delay(ost->swr_ctx, c->sample_rate) + frame->nb_samples,
            c->sample_rate, c->sample_rate, AV_ROUND_UP);
        av_assert0(dst_nb_samples == frame->nb_samples);

        /* when we pass a frame to the encoder, it may keep a reference to it
         * internally;
         * make sure we do not overwrite it here
         */
        ret = av_frame_make_writable(ost->frame);
        if (ret < 0) return;

        /* convert to destination format */
        ret = swr_convert(ost->swr_ctx,
            ost->frame->data, dst_nb_samples,
            (const uint8_t **)frame->data, frame->nb_samples);
        if (ret < 0)
        {
          fprintf(stderr, "[o-vid] error while resampling sound\n");
          return;
        }
        frame = ost->frame;

        frame->pts = av_rescale_q(ost->samples_count, (AVRational){1, c->sample_rate}, c->time_base);
        ost->samples_count += dst_nb_samples;
      }

      write_frame(dat->oc, c, ost->st, frame, ost->tmp_pkt);
      // ret indicating something stream end?
    } // end audio frame

    // prepare for next frame
    dat->have_buf[0] = dat->have_buf[1] = dat->have_buf[2] = 0;

    // finalise the file:
    if(mod->graph->frame == mod->graph->frame_cnt-1)
      close_file(dat);
  }
}
