#include "modules/api.h"

static inline int round16(const int x)
{
  // rounds x to the next multiple of 16
  // return 16 * (x / 16) + (x % 16 != 0 ? 16 : 0);
  return 16 * ((x+15)/16);
}

int read_source(
    dt_module_t             *mod,
    void                    *mapped,
    dt_read_source_params_t *p)
{
  if(p->node->kernel == dt_token("weights"))
  {
    FILE *f = dt_graph_open_resource(mod->graph, 0, "data/jddcnn-weights.dat", "r");
    if(f)
    { // load hardcoded name of weights
      fseek(f, 0, SEED_END);
      size_t sz = ftell(f);
      if(p->node->connector[0].roi.wd * p->node->connector[0].roi.ht * sizeof(uint16_t) == sz)
      { // load only if allocation size is as expected
        fseek(f, 0, SEED_SET);
        fread(mapped, sz, 1, f);
      }
      else
      {
        // TODO gui message
        fprintf(stderr, "weight file has unexpected size! discarding..\n");
      }
      fclose(f);
    }
  }
  return 0;
}

void create_nodes(dt_graph_t *graph, dt_module_t *module)
{
  // TODO node to ingest bayer thing
  // TODO: kernel input to rggb (potentially rescaling to 0,1 etc too as we go?)

  // XXX FIXME: input features is 4 rggb planes + 1 noise estimate
#define layers_cnt 6
  const int feat[] = {32, 43, 57, 76, 101, 101};
  char shader[10];

  int id_encoder[layers_cnt];   // convolution layer nodes
  int index_weights_buffer = 0; // beginning of the weights of the next convolution

  // starting with bayer planes, i.e. 2x2 downsampled
  int wd[layers_cnt+1] = {module->connector[0].roi.wd, module->connector[0].roi.wd/2};
  int ht[layers_cnt+1] = {module->connector[0].roi.ht, module->connector[0].roi.ht/2};

  for(int i=0;i<layers_cnt;i++)
  {
    const int i_cnt = i == 0 ? 5 : feat[i - 1];
    const int o_cnt = feat[i];

    dt_roi_t roi_out = { .wd = wd[i+1] * ht[i+1], .ht = round16(o_cnt) };
    int pc[] = { index_weights_buffer };

    snprintf(shader, sizeof(shader), "enc%d", i);
    id_encoder[i] = dt_node_add(
        graph, module, "jddcnn", shader,
        wd[i+1] / 8 * DT_LOCAL_SIZE_X, ht[i+1] / 8 * DT_LOCAL_SIZE_Y, 1,
        sizeof(pc), pc, 3,
        "weights", "read",  "ssbo", "f16", dt_no_roi,
        "output",  "write", "ssbo", "f16", &roi_out,
        "input",   "read",  "ssbo", "f16", dt_no_roi);

    fprintf(stderr, "encoder conv %d running on %d x %d\n", i, wd[i+1], ht[i+1]);

    index_weights_buffer += 9 * i_cnt * o_cnt + o_cnt;
    // XXX unfortunately currently the kernels run on full input resolution here.
    // XXX the max pooling is performed on *input* of the next layer, so there is a 4x memory traffic on the table here.
    wd[i+2] = (wd[i+1]+1)/2;
    ht[i+2] = (ht[i+1]+1)/2;
  }

  int id_decoder[layers_cnt];
  for(int i=0;i<layers_cnt;i++)
  {
    const int i_cnt = (i == layers_cnt-1) ? feat[layers_cnt - 1 - i] + 5: feat[layers_cnt - 2 - i] + feat[layers_cnt - 1 - i];
    const int o_cnt = (i == layers_cnt-1) ? 4                           : feat[layers_cnt - 2 - i]; // XXX 4 not 3 output channels?
    int pc[] = { index_weights_buffer };

    // layers upsample their inputs first thing when running. so the resolution to run the kernel here is the larger one,
    // i.e. the input resolution of the corresponding encoder layer.
    dt_roi_t roi_out = { .wd = wd[layers_cnt-1-i] * ht[layers_cnt-1-i], .ht = round16(o_cnt) };

    // TODO: can this go faster if we parallelise workgroups over feature channels too?
    snprintf(shader, sizeof(shader), "dec%d", i);
    id_decoder[i] = dt_node_add(
        graph, module, "jddcnn", shader,
        wd[layers_cnt-1-i] / 8 * DT_LOCAL_SIZE_X, ht[layers_cnt-1-i] / 8 * DT_LOCAL_SIZE_Y, 1,
        sizeof(pc), pc, 4,
        "weights", "read",  "ssbo", "f16", dt_no_roi,
        "output",  "write", "ssbo", "f16", &roi_out,
        "input",   "read",  "ssbo", "f16", dt_no_roi,  // low res inputs, to be upsampled first
        "skip",    "read",  "ssbo", "f16", dt_no_roi); // these are the skip connections on high res
    fprintf(stderr, "decoder conv %d running on %d x %d\n", i, wd[layers_cnt-1-i], ht[layers_cnt-1-i]);

    index_weights_buffer += 9 * i_cnt * o_cnt + o_cnt;
  }

  // wire weights from file
  dt_roi_t roi_weights = { .wd = index_weights_buffer, .ht = 1, .full_wd = index_weights_buffer, .full_ht = 1, .scale = 1 };
  const int id_lut = dt_node_add(graph, module, "jddcnn", "weights", 1, 1, 1, 0, 0, 1,
      "weights", "source", "ssbo", "f16", &roi_weights);
  graph->module[id_lut].flags = s_module_request_read_source; // read once
  for(int i=0;i<6;i++)
    dt_node_connect_named(graph, id_lut, "weights", id_encoder[i], "weights");
  for(int i=0;i<6;i++)
    dt_node_connect_named(graph, id_lut, "weights", id_decoder[i], "weights");

  dt_roi_t roi_out = { .wd = wd[1] * ht[1], .ht = round16(5) };
  const int id_input = dt_node_add(graph, module, "jddcnn", "input", wd[0], ht[0], 1, 0, 0, 2,
      "input",  "read",  "rgba", "f16", dt_no_roi,
      "output", "write", "ssbo", "f16", &roi_out);
  const int id_output = dt_node_add(graph, module, "jddcnn", "output", wd[0], ht[0], 1, 0, 0, 2,
      "input",  "read",  "ssbo", "f16", dt_no_roi,
      "output", "write", "rgba", "f16", &module->connector[1].roi);
  dt_connector_copy(graph, module, 0, id_input,  0);
  dt_connector_copy(graph, module, 1, id_output, 1);
  dt_node_connect_named(graph, id_input,                 "output", id_encoder[0], "input");
  dt_node_connect_named(graph, id_decoder[layers_cnt-1], "output", id_output,     "input");

  for(int i=0;i<layers_cnt-1;i++)
  {
    dt_node_connect_named(graph, id_encoder[i], "output", id_encoder[i+1],            "input");
    dt_node_connect_named(graph, id_encoder[i], "output", id_decoder[layers_cnt-1-i], "skip");
    dt_node_connect_named(graph, id_decoder[i], "output", id_decoder[i+1],            "input");
  }
  dt_node_connect_named(graph, id_encoder[layers_cnt-1], "output", id_decoder[0],            "input");
    // XXX in the last layer, we *first* concat, *then* upsample (to match the bayer plane dimensions)
  dt_node_connect_named(graph, id_input,                 "output", id_decoder[layers_cnt-1], "skip"); // XXX this has different upsampling!
}
