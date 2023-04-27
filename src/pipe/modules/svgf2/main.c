#include "modules/api.h"
#include "config.h"

void modify_roi_out(
    dt_graph_t  *graph,
    dt_module_t *mod)
{
  // XXX DEBUG only do down kernel at this point
  int wd = mod->connector[0].roi.full_wd;
  int ht = mod->connector[0].roi.full_wd;
  wd = (wd + 1) / 2; ht = (ht + 1) / 2;
  mod->connector[1].roi.full_wd = wd;
  mod->connector[1].roi.full_ht = ht;
  wd = (wd + 1) / 2; ht = (ht + 1) / 2;
  mod->connector[2].roi.full_wd = wd;
  mod->connector[2].roi.full_ht = ht;
  wd = (wd + 1) / 2; ht = (ht + 1) / 2;
  mod->connector[3].roi.full_wd = wd;
  mod->connector[3].roi.full_ht = ht;
}

void modify_roi_in(
    dt_graph_t  *graph,
    dt_module_t *mod)
{
  // XXX DEBUG
  mod->connector[0].wd = mod->connector[1].wd * 2;
  mod->connector[0].ht = mod->connector[1].ht * 2;
}

#if 0 // TODO
void
create_nodes(
    dt_graph_t  *graph,
    dt_module_t *module)
{
  // preblend: merge samples from previous frame buffer before denoising
  int id_preblend = dt_node_add(graph, module, "svgf", "preblend",
      module->connector[0].roi.wd, module->connector[0].roi.ht, 1, 0, 0, 6,
      "mv",    "read", "rg",   "*",   module->connector[0].roi,
      "prevl", "read", "rgba", "*",   module->connector[0].roi,
      "light", "read", "rgba", "*",   module->connector[0].roi,
      "gbufp", "read", "rgba", "f32", module->connector[0].roi,
      "gbufc", "read", "rgba", "f32", module->connector[0].roi,
      "output", "write", "rgba", "f32", module->connector[0].roi);

  // blend: final taa and albedo modulation
  int id_blend = dt_node_add(graph, module, "svgf", "blend",
      module->connector[0].roi.wd, module->connector[0].roi.ht, 1, 0, 0, 5,
      "mv",    "read", "rg",   "*", module->connector[0].roi,
      "prevb", "read", "rgba", "*", module->connector[0].roi,
      "light", "read", "rgba", "*", module->connector[0].roi,
      "albedo", "read", "rgba", "*", module->connector[0].roi,
      "output", "write", "rgba", "f32", module->connector[0].roi);

  // TODO: we need different iterations: down kernel with three outputs (and potentially iterate that)
  // TODO: connect up kernel (1:1 or 3:1 ?)
#if 0
  int id_eaw[SVGF_IT];
  for(int i=0;i<SVGF_IT;i++)
  {
    id_eaw[i] = graph->num_nodes++;
    graph->node[id_eaw[i]] = (dt_node_t) {
      .name   = dt_token("svgf"),
      .kernel = dt_token("eaw"),
      .module = module,
      .wd     = module->connector[0].roi.wd,
      .ht     = module->connector[0].roi.ht,
      .dp     = 1,
      .num_connectors = 4,
      .connector = {{ // 0
        .name   = dt_token("input"),
        .type   = dt_token("read"),
        .chan   = dt_token("rgba"),
        .format = dt_token("*"),
        .roi    = module->connector[0].roi,
        .connected_mi = -1,
      },{ // 1
        .name   = dt_token("gbuf"),
        .type   = dt_token("read"),
        .chan   = dt_token("*"),
        .format = dt_token("f32"),
        .roi    = module->connector[0].roi,
        .connected_mi = -1,
      },{ // 2
        .name   = dt_token("output"),
        .type   = dt_token("write"),
        .chan   = dt_token("rgba"),
        .format = dt_token("f16"),
        .roi    = module->connector[0].roi,
      },{ // 3
        .name   = dt_token("gbuf_out"),
        .type   = dt_token("write"),
        .chan   = dt_token("rg"),
        .format = dt_token("f32"),
        .roi    = module->connector[0].roi,
      }},
      .push_constant_size = 1*sizeof(uint32_t),
      .push_constant = { 1<<i },
    };
  }
#endif

  dt_connector_copy(graph, module, 0, id_preblend, 0);  // mv
  dt_connector_copy_feedback(graph, module, 1, id_preblend, 1);  // previous noisy light
  dt_connector_copy(graph, module, 1, id_preblend, 2);  // noisy light
  dt_connector_copy(graph, module, 4, id_preblend, 3);  // previous gbuffer
  dt_connector_copy(graph, module, 5, id_preblend, 4);  // current gbuffer

  CONN(dt_node_connect(graph, id_preblend, 5, id_eaw[0], 0)); // less noisy light
  // dt_connector_copy(graph, module, 1, id_eaw[0], 0);  // noisy light
  dt_connector_copy(graph, module, 5, id_eaw[0], 1);  // gbuffer for normal, depth, L moments
  for(int i=1;i<SVGF_IT;i++)
  {
    CONN(dt_node_connect (graph, id_eaw[i-1], 2, id_eaw[i], 0));
    CONN(dt_node_connect (graph, id_eaw[i-1], 3, id_eaw[i], 1));
  }
  CONN(dt_node_connect (graph, id_eaw[SVGF_IT-1], 2, id_blend, 2)); // denoised light
  CONN(dt_node_feedback(graph, id_blend, 4, id_blend, 1)); // beauty frame, old
#if SVGF_OFF==1
  dt_connector_copy(graph, module, 1, id_blend, 2); // XXX DEBUG light w/o denoising
#endif

  dt_connector_copy(graph, module, 0, id_blend, 0);  // mv
  dt_connector_copy(graph, module, 2, id_blend, 3);  // albedo
  dt_connector_copy(graph, module, 3, id_blend, 4);  // output beauty
}
#endif
