#pragma once
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
// #define NK_BUTTON_TRIGGER_ON_RELEASE
// unfortunately the builtin-nk function triggers infinite loops at times:
#define NK_DTOA(S, D) sprintf(S, "%g", D)
#include "nuklear.h"
#include "nuklear_glfw_vulkan.h"

// define groups of widgets that can be switched by pressing "tab"
#define nk_focus_group_property(TYPE, CTX, NAME, m, V, M, I1, I2)\
  do {\
    if(0 == focus_id_next) { nk_property_focus(CTX); focus_id_next = -1; }\
    nk_property_ ## TYPE(CTX, NAME, m, V, M, I1, I2);\
    int adv = nk_property_## TYPE ##_unfocus(CTX, NAME, m, V, M, I1, tab_keypress);\
    if(adv) { tab_keypress = adv = 0; focus_id_next = 0; }\
  } while(0)
#define nk_focus_group_head() \
  static int focus_id_next = -1;\
  static double time_tab; \
  double time_now = glfwGetTime(); \
  static int tab_keypress = 0; \
  if(glfwGetKey(vkdt.win.window, GLFW_KEY_TAB) == GLFW_PRESS && (time_now - time_tab > 0.2)) \
  { \
    time_tab = time_now; \
    tab_keypress = 1; \
  }
