/*
/// # Nuklear
/// ![](https://cloud.githubusercontent.com/assets/8057201/11761525/ae06f0ca-a0c6-11e5-819d-5610b25f6ef4.gif)
///
/// ## Contents
/// 1. About section
/// 2. Highlights section
/// 3. Features section
/// 4. Usage section
///     1. Flags section
///     2. Constants section
///     3. Dependencies section
/// 5. Example section
/// 6. API section
///     1. Context section
///     2. Input section
///     3. Drawing section
///     4. Window section
///     5. Layouting section
///     6. Groups section
///     7. Tree section
///     8. Properties section
/// 7. License section
/// 8. Changelog section
/// 9. Gallery section
/// 10. Credits section
///
/// ## About
/// This is a minimal state immediate mode graphical user interface toolkit
/// written in ANSI C and licensed under public domain. It was designed as a simple
/// embeddable user interface for application and does not have any dependencies,
/// a default renderbackend or OS window and input handling but instead provides a very modular
/// library approach by using simple input state for input and draw
/// commands describing primitive shapes as output. So instead of providing a
/// layered library that tries to abstract over a number of platform and
/// render backends it only focuses on the actual UI.
///
/// ## Highlights
/// - Graphical user interface toolkit
/// - Single header library
/// - Written in C89 (a.k.a. ANSI C or ISO C90)
/// - Small codebase (~18kLOC)
/// - Focus on portability, efficiency and simplicity
/// - No dependencies (not even the standard library if not wanted)
/// - Fully skinnable and customizable
/// - Low memory footprint with total memory control if needed or wanted
/// - UTF-8 support
/// - No global or hidden state
/// - Customizable library modules (you can compile and use only what you need)
/// - Optional font baker and vertex buffer output
/// - [Code available on github](https://github.com/Immediate-Mode-UI/Nuklear/)
///
/// ## Features
/// - Absolutely no platform dependent code
/// - Memory management control ranging from/to
///     - Ease of use by allocating everything from standard library
///     - Control every byte of memory inside the library
/// - Font handling control ranging from/to
///     - Use your own font implementation for everything
///     - Use this libraries internal font baking and handling API
/// - Drawing output control ranging from/to
///     - Simple shapes for more high level APIs which already have drawing capabilities
///     - Hardware accessible anti-aliased vertex buffer output
/// - Customizable colors and properties ranging from/to
///     - Simple changes to color by filling a simple color table
///     - Complete control with ability to use skinning to decorate widgets
/// - Bendable UI library with widget ranging from/to
///     - Basic widgets like buttons, checkboxes, slider, ...
///     - Advanced widget like abstract comboboxes, contextual menus,...
/// - Compile time configuration to only compile what you need
///     - Subset which can be used if you do not want to link or use the standard library
/// - Can be easily modified to only update on user input instead of frame updates
///
/// ## Usage
/// This library is self contained in one single header file and can be used either
/// in header only mode or in implementation mode. The header only mode is used
/// by default when included and allows including this header in other headers
/// and does not contain the actual implementation. <br /><br />
///
/// The implementation mode requires to define  the preprocessor macro
/// NK_IMPLEMENTATION in *one* .c/.cpp file before #including this file, e.g.:
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~C
///     #define NK_IMPLEMENTATION
///     #include "nuklear.h"
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Also optionally define the symbols listed in the section "OPTIONAL DEFINES"
/// below in header and implementation mode if you want to use additional functionality
/// or need more control over the library.
///
/// !!! WARNING
///     Every time nuklear is included define the same compiler flags. This very important not doing so could lead to compiler errors or even worse stack corruptions.
///
/// ### Flags
/// Flag                            | Description
/// --------------------------------|------------------------------------------
/// NK_PRIVATE                      | If defined declares all functions as static, so they can only be accessed inside the file that contains the implementation
/// NK_INCLUDE_FIXED_TYPES          | If defined it will include header `<stdint.h>` for fixed sized types otherwise nuklear tries to select the correct type. If that fails it will throw a compiler error and you have to select the correct types yourself.
/// NK_INCLUDE_DEFAULT_ALLOCATOR    | If defined it will include header `<stdlib.h>` and provide additional functions to use this library without caring for memory allocation control and therefore ease memory management.
/// NK_INCLUDE_STANDARD_IO          | If defined it will include header `<stdio.h>` and provide additional functions depending on file loading.
/// NK_INCLUDE_STANDARD_VARARGS     | If defined it will include header <stdarg.h> and provide additional functions depending on file loading.
/// NK_INCLUDE_STANDARD_BOOL        | If defined it will include header `<stdbool.h>` for nk_bool otherwise nuklear defines nk_bool as int.
/// NK_INCLUDE_VERTEX_BUFFER_OUTPUT | Defining this adds a vertex draw command list backend to this library, which allows you to convert queue commands into vertex draw commands. This is mainly if you need a hardware accessible format for OpenGL, DirectX, Vulkan, Metal,...
/// NK_INCLUDE_FONT_BAKING          | Defining this adds `stb_truetype` and `stb_rect_pack` implementation to this library and provides font baking and rendering. If you already have font handling or do not want to use this font handler you don't have to define it.
/// NK_INCLUDE_DEFAULT_FONT         | Defining this adds the default font: ProggyClean.ttf into this library which can be loaded into a font atlas and allows using this library without having a truetype font
/// NK_INCLUDE_COMMAND_USERDATA     | Defining this adds a userdata pointer into each command. Can be useful for example if you want to provide custom shaders depending on the used widget. Can be combined with the style structures.
/// NK_BUTTON_TRIGGER_ON_RELEASE    | Different platforms require button clicks occurring either on buttons being pressed (up to down) or released (down to up). By default this library will react on buttons being pressed, but if you define this it will only trigger if a button is released.
/// NK_ZERO_COMMAND_MEMORY          | Defining this will zero out memory for each drawing command added to a drawing queue (inside nk_command_buffer_push). Zeroing command memory is very useful for fast checking (using memcmp) if command buffers are equal and avoid drawing frames when nothing on screen has changed since previous frame.
/// NK_UINT_DRAW_INDEX              | Defining this will set the size of vertex index elements when using NK_VERTEX_BUFFER_OUTPUT to 32bit instead of the default of 16bit
/// NK_KEYSTATE_BASED_INPUT         | Define this if your backend uses key state for each frame rather than key press/release events
///
/// !!! WARNING
///     The following flags will pull in the standard C library:
///     - NK_INCLUDE_DEFAULT_ALLOCATOR
///     - NK_INCLUDE_STANDARD_IO
///     - NK_INCLUDE_STANDARD_VARARGS
///
/// !!! WARNING
///     The following flags if defined need to be defined for both header and implementation:
///     - NK_INCLUDE_FIXED_TYPES
///     - NK_INCLUDE_DEFAULT_ALLOCATOR
///     - NK_INCLUDE_STANDARD_VARARGS
///     - NK_INCLUDE_STANDARD_BOOL
///     - NK_INCLUDE_VERTEX_BUFFER_OUTPUT
///     - NK_INCLUDE_FONT_BAKING
///     - NK_INCLUDE_DEFAULT_FONT
///     - NK_INCLUDE_STANDARD_VARARGS
///     - NK_INCLUDE_COMMAND_USERDATA
///     - NK_UINT_DRAW_INDEX
///
/// ### Constants
/// Define                          | Description
/// --------------------------------|---------------------------------------
/// NK_BUFFER_DEFAULT_INITIAL_SIZE  | Initial buffer size allocated by all buffers while using the default allocator functions included by defining NK_INCLUDE_DEFAULT_ALLOCATOR. If you don't want to allocate the default 4k memory then redefine it.
/// NK_MAX_NUMBER_BUFFER            | Maximum buffer size for the conversion buffer between float and string Under normal circumstances this should be more than sufficient.
/// NK_INPUT_MAX                    | Defines the max number of bytes which can be added as text input in one frame. Under normal circumstances this should be more than sufficient.
///
/// !!! WARNING
///     The following constants if defined need to be defined for both header and implementation:
///     - NK_MAX_NUMBER_BUFFER
///     - NK_BUFFER_DEFAULT_INITIAL_SIZE
///     - NK_INPUT_MAX
///
/// ### Dependencies
/// Function    | Description
/// ------------|---------------------------------------------------------------
/// NK_ASSERT   | If you don't define this, nuklear will use <assert.h> with assert().
/// NK_MEMSET   | You can define this to 'memset' or your own memset implementation replacement. If not nuklear will use its own version.
/// NK_MEMCPY   | You can define this to 'memcpy' or your own memcpy implementation replacement. If not nuklear will use its own version.
/// NK_INV_SQRT | You can define this to your own inverse sqrt implementation replacement. If not nuklear will use its own slow and not highly accurate version.
/// NK_SIN      | You can define this to 'sinf' or your own sine implementation replacement. If not nuklear will use its own approximation implementation.
/// NK_COS      | You can define this to 'cosf' or your own cosine implementation replacement. If not nuklear will use its own approximation implementation.
/// NK_STRTOD   | You can define this to `strtod` or your own string to double conversion implementation replacement. If not defined nuklear will use its own imprecise and possibly unsafe version (does not handle nan or infinity!).
/// NK_DTOA     | You can define this to `dtoa` or your own double to string conversion implementation replacement. If not defined nuklear will use its own imprecise and possibly unsafe version (does not handle nan or infinity!).
/// NK_VSNPRINTF| If you define `NK_INCLUDE_STANDARD_VARARGS` as well as `NK_INCLUDE_STANDARD_IO` and want to be safe define this to `vsnprintf` on compilers supporting later versions of C or C++. By default nuklear will check for your stdlib version in C as well as compiler version in C++. if `vsnprintf` is available it will define it to `vsnprintf` directly. If not defined and if you have older versions of C or C++ it will be defined to `vsprintf` which is unsafe.
///
/// !!! WARNING
///     The following dependencies will pull in the standard C library if not redefined:
///     - NK_ASSERT
///
/// !!! WARNING
///     The following dependencies if defined need to be defined for both header and implementation:
///     - NK_ASSERT
///
/// !!! WARNING
///     The following dependencies if defined need to be defined only for the implementation part:
///     - NK_MEMSET
///     - NK_MEMCPY
///     - NK_SQRT
///     - NK_SIN
///     - NK_COS
///     - NK_STRTOD
///     - NK_DTOA
///     - NK_VSNPRINTF
///
/// ## Example
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// // init gui state
/// enum {EASY, HARD};
/// static int op = EASY;
/// static float value = 0.6f;
/// static int i =  20;
/// struct nk_context ctx;
///
/// nk_init_fixed(&ctx, calloc(1, MAX_MEMORY), MAX_MEMORY, &font);
/// if (nk_begin(&ctx, "Show", nk_rect(50, 50, 220, 220),
///     NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_CLOSABLE)) {
///     // fixed widget pixel width
///     nk_layout_row_static(&ctx, 30, 80, 1);
///     if (nk_button_label(&ctx, "button")) {
///         // event handling
///     }
///
///     // fixed widget window ratio width
///     nk_layout_row_dynamic(&ctx, 30, 2);
///     if (nk_option_label(&ctx, "easy", op == EASY)) op = EASY;
///     if (nk_option_label(&ctx, "hard", op == HARD)) op = HARD;
///
///     // custom widget pixel width
///     nk_layout_row_begin(&ctx, NK_STATIC, 30, 2);
///     {
///         nk_layout_row_push(&ctx, 50);
///         nk_label(&ctx, "Volume:", NK_TEXT_LEFT);
///         nk_layout_row_push(&ctx, 110);
///         nk_slider_float(&ctx, 0, &value, 1.0f, 0.1f);
///     }
///     nk_layout_row_end(&ctx);
/// }
/// nk_end(&ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// ![](https://cloud.githubusercontent.com/assets/8057201/10187981/584ecd68-675c-11e5-897c-822ef534a876.png)
///
/// ## API
///
*/
#ifndef NK_SINGLE_FILE
  #define NK_SINGLE_FILE
#endif

#ifndef NK_NUKLEAR_H_
#define NK_NUKLEAR_H_

#ifdef __cplusplus
extern "C" {
#endif
/*
 * ==============================================================
 *
 *                          CONSTANTS
 *
 * ===============================================================
 */
#define NK_UNDEFINED (-1.0f)
#define NK_UTF_INVALID 0xFFFD /* internal invalid utf8 rune */
#define NK_UTF_SIZE 4 /* describes the number of bytes a glyph consists of*/
#ifndef NK_INPUT_MAX
  #define NK_INPUT_MAX 16
#endif
#ifndef NK_MAX_NUMBER_BUFFER
  #define NK_MAX_NUMBER_BUFFER 64
#endif
#ifndef NK_SCROLLBAR_HIDING_TIMEOUT
  #define NK_SCROLLBAR_HIDING_TIMEOUT 4.0f
#endif
/*
 * ==============================================================
 *
 *                          HELPER
 *
 * ===============================================================
 */
#ifndef NK_API
  #ifdef NK_PRIVATE
    #if (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199409L))
      #define NK_API static inline
    #elif defined(__cplusplus)
      #define NK_API static inline
    #else
      #define NK_API static
    #endif
  #else
    #define NK_API extern
  #endif
#endif
#ifndef NK_LIB
  #ifdef NK_SINGLE_FILE
    #define NK_LIB static
  #else
    #define NK_LIB extern
  #endif
#endif

#define NK_INTERN static
#define NK_STORAGE static
#define NK_GLOBAL static

#define NK_FLAG(x) (1 << (x))
#define NK_STRINGIFY(x) #x
#define NK_MACRO_STRINGIFY(x) NK_STRINGIFY(x)
#define NK_STRING_JOIN_IMMEDIATE(arg1, arg2) arg1 ## arg2
#define NK_STRING_JOIN_DELAY(arg1, arg2) NK_STRING_JOIN_IMMEDIATE(arg1, arg2)
#define NK_STRING_JOIN(arg1, arg2) NK_STRING_JOIN_DELAY(arg1, arg2)

#ifdef _MSC_VER
  #define NK_UNIQUE_NAME(name) NK_STRING_JOIN(name,__COUNTER__)
#else
  #define NK_UNIQUE_NAME(name) NK_STRING_JOIN(name,__LINE__)
#endif

#ifndef NK_STATIC_ASSERT
  #define NK_STATIC_ASSERT(exp) typedef char NK_UNIQUE_NAME(_dummy_array)[(exp)?1:-1]
#endif

#ifndef NK_FILE_LINE
#ifdef _MSC_VER
  #define NK_FILE_LINE __FILE__ ":" NK_MACRO_STRINGIFY(__COUNTER__)
#else
  #define NK_FILE_LINE __FILE__ ":" NK_MACRO_STRINGIFY(__LINE__)
#endif
#endif

#define NK_MIN(a,b) ((a) < (b) ? (a) : (b))
#define NK_MAX(a,b) ((a) < (b) ? (b) : (a))
#define NK_CLAMP(i,v,x) (NK_MAX(NK_MIN(v,x), i))

#ifdef NK_INCLUDE_STANDARD_VARARGS
  #include <stdarg.h>
  #if defined(_MSC_VER) && (_MSC_VER >= 1600) /* VS 2010 and above */
    #include <sal.h>
    #define NK_PRINTF_FORMAT_STRING _Printf_format_string_
  #else
    #define NK_PRINTF_FORMAT_STRING
  #endif
  #if defined(__GNUC__)
    #define NK_PRINTF_VARARG_FUNC(fmtargnumber) __attribute__((format(__printf__, fmtargnumber, fmtargnumber+1)))
    #define NK_PRINTF_VALIST_FUNC(fmtargnumber) __attribute__((format(__printf__, fmtargnumber, 0)))
  #else
    #define NK_PRINTF_VARARG_FUNC(fmtargnumber)
    #define NK_PRINTF_VALIST_FUNC(fmtargnumber)
  #endif
#endif

/*
 * ===============================================================
 *
 *                          BASIC
 *
 * ===============================================================
 */
#ifdef NK_INCLUDE_FIXED_TYPES
 #include <stdint.h>
 #define NK_INT8 int8_t
 #define NK_UINT8 uint8_t
 #define NK_INT16 int16_t
 #define NK_UINT16 uint16_t
 #define NK_INT32 int32_t
 #define NK_UINT32 uint32_t
 #define NK_SIZE_TYPE uintptr_t
 #define NK_POINTER_TYPE uintptr_t
#else
  #ifndef NK_INT8
    #define NK_INT8 signed char
  #endif
  #ifndef NK_UINT8
    #define NK_UINT8 unsigned char
  #endif
  #ifndef NK_INT16
    #define NK_INT16 signed short
  #endif
  #ifndef NK_UINT16
    #define NK_UINT16 unsigned short
  #endif
  #ifndef NK_INT32
    #if defined(_MSC_VER)
      #define NK_INT32 __int32
    #else
      #define NK_INT32 signed int
    #endif
  #endif
  #ifndef NK_UINT32
    #if defined(_MSC_VER)
      #define NK_UINT32 unsigned __int32
    #else
      #define NK_UINT32 unsigned int
    #endif
  #endif
  #ifndef NK_SIZE_TYPE
    #if defined(_WIN64) && defined(_MSC_VER)
      #define NK_SIZE_TYPE unsigned __int64
    #elif (defined(_WIN32) || defined(WIN32)) && defined(_MSC_VER)
      #define NK_SIZE_TYPE unsigned __int32
    #elif defined(__GNUC__) || defined(__clang__)
      #if defined(__x86_64__) || defined(__ppc64__) || defined(__PPC64__) || defined(__aarch64__)
        #define NK_SIZE_TYPE unsigned long
      #else
        #define NK_SIZE_TYPE unsigned int
      #endif
    #else
      #define NK_SIZE_TYPE unsigned long
    #endif
  #endif
  #ifndef NK_POINTER_TYPE
    #if defined(_WIN64) && defined(_MSC_VER)
      #define NK_POINTER_TYPE unsigned __int64
    #elif (defined(_WIN32) || defined(WIN32)) && defined(_MSC_VER)
      #define NK_POINTER_TYPE unsigned __int32
    #elif defined(__GNUC__) || defined(__clang__)
      #if defined(__x86_64__) || defined(__ppc64__) || defined(__PPC64__) || defined(__aarch64__)
        #define NK_POINTER_TYPE unsigned long
      #else
        #define NK_POINTER_TYPE unsigned int
      #endif
    #else
      #define NK_POINTER_TYPE unsigned long
    #endif
  #endif
#endif

#ifndef NK_BOOL
  #ifdef NK_INCLUDE_STANDARD_BOOL
    #include <stdbool.h>
    #define NK_BOOL bool
  #else
    #define NK_BOOL int /* could be char, use int for drop-in replacement backwards compatibility */
  #endif
#endif

typedef NK_INT8 nk_char;
typedef NK_UINT8 nk_uchar;
typedef NK_UINT8 nk_byte;
typedef NK_INT16 nk_short;
typedef NK_UINT16 nk_ushort;
typedef NK_INT32 nk_int;
typedef NK_UINT32 nk_uint;
typedef NK_SIZE_TYPE nk_size;
typedef NK_POINTER_TYPE nk_ptr;
typedef NK_BOOL nk_bool;

typedef nk_uint nk_hash;
typedef nk_uint nk_flags;
typedef nk_uint nk_rune;

/* Make sure correct type size:
 * This will fire with a negative subscript error if the type sizes
 * are set incorrectly by the compiler, and compile out if not */
NK_STATIC_ASSERT(sizeof(nk_short) == 2);
NK_STATIC_ASSERT(sizeof(nk_ushort) == 2);
NK_STATIC_ASSERT(sizeof(nk_uint) == 4);
NK_STATIC_ASSERT(sizeof(nk_int) == 4);
NK_STATIC_ASSERT(sizeof(nk_byte) == 1);
NK_STATIC_ASSERT(sizeof(nk_flags) >= 4);
NK_STATIC_ASSERT(sizeof(nk_rune) >= 4);
NK_STATIC_ASSERT(sizeof(nk_size) >= sizeof(void*));
NK_STATIC_ASSERT(sizeof(nk_ptr) >= sizeof(void*));
#ifdef NK_INCLUDE_STANDARD_BOOL
NK_STATIC_ASSERT(sizeof(nk_bool) == sizeof(bool));
#else
NK_STATIC_ASSERT(sizeof(nk_bool) >= 2);
#endif

/* ============================================================================
 *
 *                                  API
 *
 * =========================================================================== */
struct nk_buffer;
struct nk_allocator;
struct nk_command_buffer;
struct nk_draw_command;
struct nk_convert_config;
struct nk_style_item;
struct nk_text_edit;
struct nk_draw_list;
struct nk_user_font;
struct nk_panel;
struct nk_context;
struct nk_draw_vertex_layout_element;
struct nk_style_button;
struct nk_style_toggle;
struct nk_style_selectable;
struct nk_style_slide;
struct nk_style_progress;
struct nk_style_scrollbar;
struct nk_style_edit;
struct nk_style_property;
struct nk_style_chart;
struct nk_style_combo;
struct nk_style_tab;
struct nk_style_window_header;
struct nk_style_window;

enum {nk_false, nk_true};
struct nk_color {nk_byte r,g,b,a;};
struct nk_colorf {float r,g,b,a;};
struct nk_vec2 {float x,y;};
struct nk_vec2i {short x, y;};
struct nk_rect {float x,y,w,h;};
struct nk_recti {short x,y,w,h;};
typedef char nk_glyph[NK_UTF_SIZE];
typedef union {void *ptr; int id;} nk_handle;
struct nk_image {nk_handle handle; nk_ushort w, h; nk_ushort region[4];};
struct nk_nine_slice {struct nk_image img; nk_ushort l, t, r, b;};
struct nk_cursor {struct nk_image img; struct nk_vec2 size, offset;};
struct nk_scroll {nk_uint x, y;};

enum nk_heading         {NK_UP, NK_RIGHT, NK_DOWN, NK_LEFT};
enum nk_button_behavior {NK_BUTTON_DEFAULT, NK_BUTTON_REPEATER};
enum nk_modify          {NK_FIXED = nk_false, NK_MODIFIABLE = nk_true};
enum nk_orientation     {NK_VERTICAL, NK_HORIZONTAL};
enum nk_collapse_states {NK_MINIMIZED = nk_false, NK_MAXIMIZED = nk_true, NK_FORCE_MINIMIZED = 2, NK_FORCE_MAXIMIZED = 3};
enum nk_show_states     {NK_HIDDEN = nk_false, NK_SHOWN = nk_true};
enum nk_chart_type      {NK_CHART_LINES, NK_CHART_COLUMN, NK_CHART_MAX};
enum nk_chart_event     {NK_CHART_HOVERING = 0x01, NK_CHART_CLICKED = 0x02};
enum nk_color_format    {NK_RGB, NK_RGBA};
enum nk_popup_type      {NK_POPUP_STATIC, NK_POPUP_DYNAMIC};
enum nk_layout_format   {NK_DYNAMIC, NK_STATIC};
enum nk_tree_type       {NK_TREE_NODE, NK_TREE_TAB};

typedef void*(*nk_plugin_alloc)(nk_handle, void *old, nk_size);
typedef void (*nk_plugin_free)(nk_handle, void *old);
typedef nk_bool(*nk_plugin_filter)(const struct nk_text_edit*, nk_rune unicode);
typedef void(*nk_plugin_paste)(nk_handle, struct nk_text_edit*);
typedef void(*nk_plugin_copy)(nk_handle, const char*, int len);

struct nk_allocator {
    nk_handle userdata;
    nk_plugin_alloc alloc;
    nk_plugin_free free;
};
enum nk_symbol_type {
    NK_SYMBOL_NONE,
    NK_SYMBOL_X,
    NK_SYMBOL_UNDERSCORE,
    NK_SYMBOL_CIRCLE_SOLID,
    NK_SYMBOL_CIRCLE_OUTLINE,
    NK_SYMBOL_RECT_SOLID,
    NK_SYMBOL_RECT_OUTLINE,
    NK_SYMBOL_TRIANGLE_UP,
    NK_SYMBOL_TRIANGLE_DOWN,
    NK_SYMBOL_TRIANGLE_LEFT,
    NK_SYMBOL_TRIANGLE_RIGHT,
    NK_SYMBOL_PLUS,
    NK_SYMBOL_MINUS,
    NK_SYMBOL_TRIANGLE_UP_OUTLINE,
    NK_SYMBOL_TRIANGLE_DOWN_OUTLINE,
    NK_SYMBOL_TRIANGLE_LEFT_OUTLINE,
    NK_SYMBOL_TRIANGLE_RIGHT_OUTLINE,
    NK_SYMBOL_MAX
};
/* =============================================================================
 *
 *                                  CONTEXT
 *
 * =============================================================================*/
/*/// ### Context
/// Contexts are the main entry point and the majestro of nuklear and contain all required state.
/// They are used for window, memory, input, style, stack, commands and time management and need
/// to be passed into all nuklear GUI specific functions.
///
/// #### Usage
/// To use a context it first has to be initialized which can be achieved by calling
/// one of either `nk_init_default`, `nk_init_fixed`, `nk_init`, `nk_init_custom`.
/// Each takes in a font handle and a specific way of handling memory. Memory control
/// hereby ranges from standard library to just specifying a fixed sized block of memory
/// which nuklear has to manage itself from.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// struct nk_context ctx;
/// nk_init_xxx(&ctx, ...);
/// while (1) {
///     // [...]
///     nk_clear(&ctx);
/// }
/// nk_free(&ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// #### Reference
/// Function            | Description
/// --------------------|-------------------------------------------------------
/// __nk_init_default__ | Initializes context with standard library memory allocation (malloc,free)
/// __nk_init_fixed__   | Initializes context from single fixed size memory block
/// __nk_init__         | Initializes context with memory allocator callbacks for alloc and free
/// __nk_init_custom__  | Initializes context from two buffers. One for draw commands the other for window/panel/table allocations
/// __nk_clear__        | Called at the end of the frame to reset and prepare the context for the next frame
/// __nk_free__         | Shutdown and free all memory allocated inside the context
/// __nk_set_user_data__| Utility function to pass user data to draw command
 */
#ifdef NK_INCLUDE_DEFAULT_ALLOCATOR
/*/// #### nk_init_default
/// Initializes a `nk_context` struct with a default standard library allocator.
/// Should be used if you don't want to be bothered with memory management in nuklear.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_bool nk_init_default(struct nk_context *ctx, const struct nk_user_font *font);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|---------------------------------------------------------------
/// __ctx__     | Must point to an either stack or heap allocated `nk_context` struct
/// __font__    | Must point to a previously initialized font handle for more info look at font documentation
///
/// Returns either `false(0)` on failure or `true(1)` on success.
///
*/
NK_API nk_bool nk_init_default(struct nk_context*, const struct nk_user_font*);
#endif
/*/// #### nk_init_fixed
/// Initializes a `nk_context` struct from single fixed size memory block
/// Should be used if you want complete control over nuklear's memory management.
/// Especially recommended for system with little memory or systems with virtual memory.
/// For the later case you can just allocate for example 16MB of virtual memory
/// and only the required amount of memory will actually be committed.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_bool nk_init_fixed(struct nk_context *ctx, void *memory, nk_size size, const struct nk_user_font *font);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// !!! Warning
///     make sure the passed memory block is aligned correctly for `nk_draw_commands`.
///
/// Parameter   | Description
/// ------------|--------------------------------------------------------------
/// __ctx__     | Must point to an either stack or heap allocated `nk_context` struct
/// __memory__  | Must point to a previously allocated memory block
/// __size__    | Must contain the total size of __memory__
/// __font__    | Must point to a previously initialized font handle for more info look at font documentation
///
/// Returns either `false(0)` on failure or `true(1)` on success.
*/
NK_API nk_bool nk_init_fixed(struct nk_context*, void *memory, nk_size size, const struct nk_user_font*);
/*/// #### nk_init
/// Initializes a `nk_context` struct with memory allocation callbacks for nuklear to allocate
/// memory from. Used internally for `nk_init_default` and provides a kitchen sink allocation
/// interface to nuklear. Can be useful for cases like monitoring memory consumption.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_bool nk_init(struct nk_context *ctx, const struct nk_allocator *alloc, const struct nk_user_font *font);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|---------------------------------------------------------------
/// __ctx__     | Must point to an either stack or heap allocated `nk_context` struct
/// __alloc__   | Must point to a previously allocated memory allocator
/// __font__    | Must point to a previously initialized font handle for more info look at font documentation
///
/// Returns either `false(0)` on failure or `true(1)` on success.
*/
NK_API nk_bool nk_init(struct nk_context*, const struct nk_allocator*, const struct nk_user_font*);
/*/// #### nk_init_custom
/// Initializes a `nk_context` struct from two different either fixed or growing
/// buffers. The first buffer is for allocating draw commands while the second buffer is
/// used for allocating windows, panels and state tables.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_bool nk_init_custom(struct nk_context *ctx, struct nk_buffer *cmds, struct nk_buffer *pool, const struct nk_user_font *font);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|---------------------------------------------------------------
/// __ctx__     | Must point to an either stack or heap allocated `nk_context` struct
/// __cmds__    | Must point to a previously initialized memory buffer either fixed or dynamic to store draw commands into
/// __pool__    | Must point to a previously initialized memory buffer either fixed or dynamic to store windows, panels and tables
/// __font__    | Must point to a previously initialized font handle for more info look at font documentation
///
/// Returns either `false(0)` on failure or `true(1)` on success.
*/
NK_API nk_bool nk_init_custom(struct nk_context*, struct nk_buffer *cmds, struct nk_buffer *pool, const struct nk_user_font*);
/*/// #### nk_clear
/// Resets the context state at the end of the frame. This includes mostly
/// garbage collector tasks like removing windows or table not called and therefore
/// used anymore.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_clear(struct nk_context *ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to a previously initialized `nk_context` struct
*/
NK_API void nk_clear(struct nk_context*);
/*/// #### nk_free
/// Frees all memory allocated by nuklear. Not needed if context was
/// initialized with `nk_init_fixed`.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_free(struct nk_context *ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to a previously initialized `nk_context` struct
*/
NK_API void nk_free(struct nk_context*);
#ifdef NK_INCLUDE_COMMAND_USERDATA
/*/// #### nk_set_user_data
/// Sets the currently passed userdata passed down into each draw command.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_set_user_data(struct nk_context *ctx, nk_handle data);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|--------------------------------------------------------------
/// __ctx__     | Must point to a previously initialized `nk_context` struct
/// __data__    | Handle with either pointer or index to be passed into every draw commands
*/
NK_API void nk_set_user_data(struct nk_context*, nk_handle handle);
#endif
/* =============================================================================
 *
 *                                  INPUT
 *
 * =============================================================================*/
/*/// ### Input
/// The input API is responsible for holding the current input state composed of
/// mouse, key and text input states.
/// It is worth noting that no direct OS or window handling is done in nuklear.
/// Instead all input state has to be provided by platform specific code. This on one hand
/// expects more work from the user and complicates usage but on the other hand
/// provides simple abstraction over a big number of platforms, libraries and other
/// already provided functionality.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_input_begin(&ctx);
/// while (GetEvent(&evt)) {
///     if (evt.type == MOUSE_MOVE)
///         nk_input_motion(&ctx, evt.motion.x, evt.motion.y);
///     else if (evt.type == [...]) {
///         // [...]
///     }
/// } nk_input_end(&ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// #### Usage
/// Input state needs to be provided to nuklear by first calling `nk_input_begin`
/// which resets internal state like delta mouse position and button transitions.
/// After `nk_input_begin` all current input state needs to be provided. This includes
/// mouse motion, button and key pressed and released, text input and scrolling.
/// Both event- or state-based input handling are supported by this API
/// and should work without problems. Finally after all input state has been
/// mirrored `nk_input_end` needs to be called to finish input process.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// struct nk_context ctx;
/// nk_init_xxx(&ctx, ...);
/// while (1) {
///     Event evt;
///     nk_input_begin(&ctx);
///     while (GetEvent(&evt)) {
///         if (evt.type == MOUSE_MOVE)
///             nk_input_motion(&ctx, evt.motion.x, evt.motion.y);
///         else if (evt.type == [...]) {
///             // [...]
///         }
///     }
///     nk_input_end(&ctx);
///     // [...]
///     nk_clear(&ctx);
/// } nk_free(&ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// #### Reference
/// Function            | Description
/// --------------------|-------------------------------------------------------
/// __nk_input_begin__  | Begins the input mirroring process. Needs to be called before all other `nk_input_xxx` calls
/// __nk_input_motion__ | Mirrors mouse cursor position
/// __nk_input_key__    | Mirrors key state with either pressed or released
/// __nk_input_button__ | Mirrors mouse button state with either pressed or released
/// __nk_input_scroll__ | Mirrors mouse scroll values
/// __nk_input_char__   | Adds a single ASCII text character into an internal text buffer
/// __nk_input_glyph__  | Adds a single multi-byte UTF-8 character into an internal text buffer
/// __nk_input_unicode__| Adds a single unicode rune into an internal text buffer
/// __nk_input_end__    | Ends the input mirroring process by calculating state changes. Don't call any `nk_input_xxx` function referenced above after this call
*/
enum nk_keys {
    NK_KEY_NONE,
    NK_KEY_SHIFT,
    NK_KEY_CTRL,
    NK_KEY_DEL,
    NK_KEY_ENTER,
    NK_KEY_TAB,
    NK_KEY_BACKSPACE,
    NK_KEY_COPY,
    NK_KEY_CUT,
    NK_KEY_PASTE,
    NK_KEY_UP,
    NK_KEY_DOWN,
    NK_KEY_LEFT,
    NK_KEY_RIGHT,
    /* Shortcuts: text field */
    NK_KEY_TEXT_INSERT_MODE,
    NK_KEY_TEXT_REPLACE_MODE,
    NK_KEY_TEXT_RESET_MODE,
    NK_KEY_TEXT_LINE_START,
    NK_KEY_TEXT_LINE_END,
    NK_KEY_TEXT_START,
    NK_KEY_TEXT_END,
    NK_KEY_TEXT_UNDO,
    NK_KEY_TEXT_REDO,
    NK_KEY_TEXT_SELECT_ALL,
    NK_KEY_TEXT_WORD_LEFT,
    NK_KEY_TEXT_WORD_RIGHT,
    /* Shortcuts: scrollbar */
    NK_KEY_SCROLL_START,
    NK_KEY_SCROLL_END,
    NK_KEY_SCROLL_DOWN,
    NK_KEY_SCROLL_UP,
    NK_KEY_MAX
};
enum nk_buttons {
    NK_BUTTON_LEFT,
    NK_BUTTON_MIDDLE,
    NK_BUTTON_RIGHT,
    NK_BUTTON_DOUBLE,
    NK_BUTTON_MAX
};
/*/// #### nk_input_begin
/// Begins the input mirroring process by resetting text, scroll
/// mouse, previous mouse position and movement as well as key state transitions,
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_input_begin(struct nk_context*);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to a previously initialized `nk_context` struct
*/
NK_API void nk_input_begin(struct nk_context*);
/*/// #### nk_input_motion
/// Mirrors current mouse position to nuklear
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_input_motion(struct nk_context *ctx, int x, int y);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to a previously initialized `nk_context` struct
/// __x__       | Must hold an integer describing the current mouse cursor x-position
/// __y__       | Must hold an integer describing the current mouse cursor y-position
*/
NK_API void nk_input_motion(struct nk_context*, float x, float y);
/*/// #### nk_input_key
/// Mirrors the state of a specific key to nuklear
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_input_key(struct nk_context*, enum nk_keys key, nk_bool down);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to a previously initialized `nk_context` struct
/// __key__     | Must be any value specified in enum `nk_keys` that needs to be mirrored
/// __down__    | Must be 0 for key is up and 1 for key is down
*/
NK_API void nk_input_key(struct nk_context*, enum nk_keys, nk_bool down);
/*/// #### nk_input_button
/// Mirrors the state of a specific mouse button to nuklear
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_input_button(struct nk_context *ctx, enum nk_buttons btn, int x, int y, nk_bool down);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to a previously initialized `nk_context` struct
/// __btn__     | Must be any value specified in enum `nk_buttons` that needs to be mirrored
/// __x__       | Must contain an integer describing mouse cursor x-position on click up/down
/// __y__       | Must contain an integer describing mouse cursor y-position on click up/down
/// __down__    | Must be 0 for key is up and 1 for key is down
*/
NK_API void nk_input_button(struct nk_context*, enum nk_buttons, int x, int y, nk_bool down);
/*/// #### nk_input_scroll
/// Copies the last mouse scroll value to nuklear. Is generally
/// a scroll value. So does not have to come from mouse and could also originate
/// TODO finish this sentence
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_input_scroll(struct nk_context *ctx, struct nk_vec2 val);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to a previously initialized `nk_context` struct
/// __val__     | vector with both X- as well as Y-scroll value
*/
NK_API void nk_input_scroll(struct nk_context*, struct nk_vec2 val);
/*/// #### nk_input_char
/// Copies a single ASCII character into an internal text buffer
/// This is basically a helper function to quickly push ASCII characters into
/// nuklear.
///
/// !!! Note
///     Stores up to NK_INPUT_MAX bytes between `nk_input_begin` and `nk_input_end`.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_input_char(struct nk_context *ctx, char c);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to a previously initialized `nk_context` struct
/// __c__       | Must be a single ASCII character preferable one that can be printed
*/
NK_API void nk_input_char(struct nk_context*, char);
/*/// #### nk_input_glyph
/// Converts an encoded unicode rune into UTF-8 and copies the result into an
/// internal text buffer.
///
/// !!! Note
///     Stores up to NK_INPUT_MAX bytes between `nk_input_begin` and `nk_input_end`.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_input_glyph(struct nk_context *ctx, const nk_glyph g);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to a previously initialized `nk_context` struct
/// __g__       | UTF-32 unicode codepoint
*/
NK_API void nk_input_glyph(struct nk_context*, const nk_glyph);
/*/// #### nk_input_unicode
/// Converts a unicode rune into UTF-8 and copies the result
/// into an internal text buffer.
/// !!! Note
///     Stores up to NK_INPUT_MAX bytes between `nk_input_begin` and `nk_input_end`.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_input_unicode(struct nk_context*, nk_rune rune);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to a previously initialized `nk_context` struct
/// __rune__    | UTF-32 unicode codepoint
*/
NK_API void nk_input_unicode(struct nk_context*, nk_rune);
/*/// #### nk_input_end
/// End the input mirroring process by resetting mouse grabbing
/// state to ensure the mouse cursor is not grabbed indefinitely.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_input_end(struct nk_context *ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to a previously initialized `nk_context` struct
*/
NK_API void nk_input_end(struct nk_context*);
/* =============================================================================
 *
 *                                  DRAWING
 *
 * =============================================================================*/
/*/// ### Drawing
/// This library was designed to be render backend agnostic so it does
/// not draw anything to screen directly. Instead all drawn shapes, widgets
/// are made of, are buffered into memory and make up a command queue.
/// Each frame therefore fills the command buffer with draw commands
/// that then need to be executed by the user and his own render backend.
/// After that the command buffer needs to be cleared and a new frame can be
/// started. It is probably important to note that the command buffer is the main
/// drawing API and the optional vertex buffer API only takes this format and
/// converts it into a hardware accessible format.
///
/// #### Usage
/// To draw all draw commands accumulated over a frame you need your own render
/// backend able to draw a number of 2D primitives. This includes at least
/// filled and stroked rectangles, circles, text, lines, triangles and scissors.
/// As soon as this criterion is met you can iterate over each draw command
/// and execute each draw command in a interpreter like fashion:
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// const struct nk_command *cmd = 0;
/// nk_foreach(cmd, &ctx) {
///     switch (cmd->type) {
///     case NK_COMMAND_LINE:
///         your_draw_line_function(...)
///         break;
///     case NK_COMMAND_RECT
///         your_draw_rect_function(...)
///         break;
///     case //...:
///         //[...]
///     }
/// }
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// In program flow context draw commands need to be executed after input has been
/// gathered and the complete UI with windows and their contained widgets have
/// been executed and before calling `nk_clear` which frees all previously
/// allocated draw commands.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// struct nk_context ctx;
/// nk_init_xxx(&ctx, ...);
/// while (1) {
///     Event evt;
///     nk_input_begin(&ctx);
///     while (GetEvent(&evt)) {
///         if (evt.type == MOUSE_MOVE)
///             nk_input_motion(&ctx, evt.motion.x, evt.motion.y);
///         else if (evt.type == [...]) {
///             [...]
///         }
///     }
///     nk_input_end(&ctx);
///     //
///     // [...]
///     //
///     const struct nk_command *cmd = 0;
///     nk_foreach(cmd, &ctx) {
///     switch (cmd->type) {
///     case NK_COMMAND_LINE:
///         your_draw_line_function(...)
///         break;
///     case NK_COMMAND_RECT
///         your_draw_rect_function(...)
///         break;
///     case ...:
///         // [...]
///     }
///     nk_clear(&ctx);
/// }
/// nk_free(&ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// You probably noticed that you have to draw all of the UI each frame which is
/// quite wasteful. While the actual UI updating loop is quite fast rendering
/// without actually needing it is not. So there are multiple things you could do.
///
/// First is only update on input. This of course is only an option if your
/// application only depends on the UI and does not require any outside calculations.
/// If you actually only update on input make sure to update the UI two times each
/// frame and call `nk_clear` directly after the first pass and only draw in
/// the second pass. In addition it is recommended to also add additional timers
/// to make sure the UI is not drawn more than a fixed number of frames per second.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// struct nk_context ctx;
/// nk_init_xxx(&ctx, ...);
/// while (1) {
///     // [...wait for input ]
///     // [...do two UI passes ...]
///     do_ui(...)
///     nk_clear(&ctx);
///     do_ui(...)
///     //
///     // draw
///     const struct nk_command *cmd = 0;
///     nk_foreach(cmd, &ctx) {
///     switch (cmd->type) {
///     case NK_COMMAND_LINE:
///         your_draw_line_function(...)
///         break;
///     case NK_COMMAND_RECT
///         your_draw_rect_function(...)
///         break;
///     case ...:
///         //[...]
///     }
///     nk_clear(&ctx);
/// }
/// nk_free(&ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// The second probably more applicable trick is to only draw if anything changed.
/// It is not really useful for applications with continuous draw loop but
/// quite useful for desktop applications. To actually get nuklear to only
/// draw on changes you first have to define `NK_ZERO_COMMAND_MEMORY` and
/// allocate a memory buffer that will store each unique drawing output.
/// After each frame you compare the draw command memory inside the library
/// with your allocated buffer by memcmp. If memcmp detects differences
/// you have to copy the command buffer into the allocated buffer
/// and then draw like usual (this example uses fixed memory but you could
/// use dynamically allocated memory).
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// //[... other defines ...]
/// #define NK_ZERO_COMMAND_MEMORY
/// #include "nuklear.h"
/// //
/// // setup context
/// struct nk_context ctx;
/// void *last = calloc(1,64*1024);
/// void *buf = calloc(1,64*1024);
/// nk_init_fixed(&ctx, buf, 64*1024);
/// //
/// // loop
/// while (1) {
///     // [...input...]
///     // [...ui...]
///     void *cmds = nk_buffer_memory(&ctx.memory);
///     if (memcmp(cmds, last, ctx.memory.allocated)) {
///         memcpy(last,cmds,ctx.memory.allocated);
///         const struct nk_command *cmd = 0;
///         nk_foreach(cmd, &ctx) {
///             switch (cmd->type) {
///             case NK_COMMAND_LINE:
///                 your_draw_line_function(...)
///                 break;
///             case NK_COMMAND_RECT
///                 your_draw_rect_function(...)
///                 break;
///             case ...:
///                 // [...]
///             }
///         }
///     }
///     nk_clear(&ctx);
/// }
/// nk_free(&ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Finally while using draw commands makes sense for higher abstracted platforms like
/// X11 and Win32 or drawing libraries it is often desirable to use graphics
/// hardware directly. Therefore it is possible to just define
/// `NK_INCLUDE_VERTEX_BUFFER_OUTPUT` which includes optional vertex output.
/// To access the vertex output you first have to convert all draw commands into
/// vertexes by calling `nk_convert` which takes in your preferred vertex format.
/// After successfully converting all draw commands just iterate over and execute all
/// vertex draw commands:
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// // fill configuration
/// struct your_vertex
/// {
///     float pos[2]; // important to keep it to 2 floats
///     float uv[2];
///     unsigned char col[4];
/// };
/// struct nk_convert_config cfg = {};
/// static const struct nk_draw_vertex_layout_element vertex_layout[] = {
///     {NK_VERTEX_POSITION, NK_FORMAT_FLOAT, NK_OFFSETOF(struct your_vertex, pos)},
///     {NK_VERTEX_TEXCOORD, NK_FORMAT_FLOAT, NK_OFFSETOF(struct your_vertex, uv)},
///     {NK_VERTEX_COLOR, NK_FORMAT_R8G8B8A8, NK_OFFSETOF(struct your_vertex, col)},
///     {NK_VERTEX_LAYOUT_END}
/// };
/// cfg.shape_AA = NK_ANTI_ALIASING_ON;
/// cfg.line_AA = NK_ANTI_ALIASING_ON;
/// cfg.vertex_layout = vertex_layout;
/// cfg.vertex_size = sizeof(struct your_vertex);
/// cfg.vertex_alignment = NK_ALIGNOF(struct your_vertex);
/// cfg.circle_segment_count = 22;
/// cfg.curve_segment_count = 22;
/// cfg.arc_segment_count = 22;
/// cfg.global_alpha = 1.0f;
/// cfg.tex_null = dev->tex_null;
/// //
/// // setup buffers and convert
/// struct nk_buffer cmds, verts, idx;
/// nk_buffer_init_default(&cmds);
/// nk_buffer_init_default(&verts);
/// nk_buffer_init_default(&idx);
/// nk_convert(&ctx, &cmds, &verts, &idx, &cfg);
/// //
/// // draw
/// nk_draw_foreach(cmd, &ctx, &cmds) {
/// if (!cmd->elem_count) continue;
///     //[...]
/// }
/// nk_buffer_free(&cms);
/// nk_buffer_free(&verts);
/// nk_buffer_free(&idx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// #### Reference
/// Function            | Description
/// --------------------|-------------------------------------------------------
/// __nk__begin__       | Returns the first draw command in the context draw command list to be drawn
/// __nk__next__        | Increments the draw command iterator to the next command inside the context draw command list
/// __nk_foreach__      | Iterates over each draw command inside the context draw command list
/// __nk_convert__      | Converts from the abstract draw commands list into a hardware accessible vertex format
/// __nk_draw_begin__   | Returns the first vertex command in the context vertex draw list to be executed
/// __nk__draw_next__   | Increments the vertex command iterator to the next command inside the context vertex command list
/// __nk__draw_end__    | Returns the end of the vertex draw list
/// __nk_draw_foreach__ | Iterates over each vertex draw command inside the vertex draw list
*/
enum nk_anti_aliasing {NK_ANTI_ALIASING_OFF, NK_ANTI_ALIASING_ON};
enum nk_convert_result {
    NK_CONVERT_SUCCESS = 0,
    NK_CONVERT_INVALID_PARAM = 1,
    NK_CONVERT_COMMAND_BUFFER_FULL = NK_FLAG(1),
    NK_CONVERT_VERTEX_BUFFER_FULL = NK_FLAG(2),
    NK_CONVERT_ELEMENT_BUFFER_FULL = NK_FLAG(3)
};
struct nk_draw_null_texture {
    nk_handle texture; /* texture handle to a texture with a white pixel */
    struct nk_vec2 uv; /* coordinates to a white pixel in the texture  */
};
struct nk_convert_config {
    float global_alpha; /* global alpha value */
    enum nk_anti_aliasing line_AA; /* line anti-aliasing flag can be turned off if you are tight on memory */
    enum nk_anti_aliasing shape_AA; /* shape anti-aliasing flag can be turned off if you are tight on memory */
    unsigned circle_segment_count; /* number of segments used for circles: default to 22 */
    unsigned arc_segment_count; /* number of segments used for arcs: default to 22 */
    unsigned curve_segment_count; /* number of segments used for curves: default to 22 */
    struct nk_draw_null_texture tex_null; /* handle to texture with a white pixel for shape drawing */
    const struct nk_draw_vertex_layout_element *vertex_layout; /* describes the vertex output format and packing */
    nk_size vertex_size; /* sizeof one vertex for vertex packing */
    nk_size vertex_alignment; /* vertex alignment: Can be obtained by NK_ALIGNOF */
};
/*/// #### nk__begin
/// Returns a draw command list iterator to iterate all draw
/// commands accumulated over one frame.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// const struct nk_command* nk__begin(struct nk_context*);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | must point to an previously initialized `nk_context` struct at the end of a frame
///
/// Returns draw command pointer pointing to the first command inside the draw command list
*/
NK_API const struct nk_command* nk__begin(struct nk_context*);
/*/// #### nk__next
/// Returns draw command pointer pointing to the next command inside the draw command list
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// const struct nk_command* nk__next(struct nk_context*, const struct nk_command*);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct at the end of a frame
/// __cmd__     | Must point to an previously a draw command either returned by `nk__begin` or `nk__next`
///
/// Returns draw command pointer pointing to the next command inside the draw command list
*/
NK_API const struct nk_command* nk__next(struct nk_context*, const struct nk_command*);
/*/// #### nk_foreach
/// Iterates over each draw command inside the context draw command list
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// #define nk_foreach(c, ctx)
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct at the end of a frame
/// __cmd__     | Command pointer initialized to NULL
///
/// Iterates over each draw command inside the context draw command list
*/
#define nk_foreach(c, ctx) for((c) = nk__begin(ctx); (c) != 0; (c) = nk__next(ctx,c))
#ifdef NK_INCLUDE_VERTEX_BUFFER_OUTPUT
/*/// #### nk_convert
/// Converts all internal draw commands into vertex draw commands and fills
/// three buffers with vertexes, vertex draw commands and vertex indices. The vertex format
/// as well as some other configuration values have to be configured by filling out a
/// `nk_convert_config` struct.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_flags nk_convert(struct nk_context *ctx, struct nk_buffer *cmds,
///     struct nk_buffer *vertices, struct nk_buffer *elements, const struct nk_convert_config*);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct at the end of a frame
/// __cmds__    | Must point to a previously initialized buffer to hold converted vertex draw commands
/// __vertices__| Must point to a previously initialized buffer to hold all produced vertices
/// __elements__| Must point to a previously initialized buffer to hold all produced vertex indices
/// __config__  | Must point to a filled out `nk_config` struct to configure the conversion process
///
/// Returns one of enum nk_convert_result error codes
///
/// Parameter                       | Description
/// --------------------------------|-----------------------------------------------------------
/// NK_CONVERT_SUCCESS              | Signals a successful draw command to vertex buffer conversion
/// NK_CONVERT_INVALID_PARAM        | An invalid argument was passed in the function call
/// NK_CONVERT_COMMAND_BUFFER_FULL  | The provided buffer for storing draw commands is full or failed to allocate more memory
/// NK_CONVERT_VERTEX_BUFFER_FULL   | The provided buffer for storing vertices is full or failed to allocate more memory
/// NK_CONVERT_ELEMENT_BUFFER_FULL  | The provided buffer for storing indices is full or failed to allocate more memory
*/
NK_API nk_flags nk_convert(struct nk_context*, struct nk_buffer *cmds, struct nk_buffer *vertices, struct nk_buffer *elements, const struct nk_convert_config*);
/*/// #### nk__draw_begin
/// Returns a draw vertex command buffer iterator to iterate over the vertex draw command buffer
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// const struct nk_draw_command* nk__draw_begin(const struct nk_context*, const struct nk_buffer*);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct at the end of a frame
/// __buf__     | Must point to an previously by `nk_convert` filled out vertex draw command buffer
///
/// Returns vertex draw command pointer pointing to the first command inside the vertex draw command buffer
*/
NK_API const struct nk_draw_command* nk__draw_begin(const struct nk_context*, const struct nk_buffer*);
/*/// #### nk__draw_end
/// Returns the vertex draw command at the end of the vertex draw command buffer
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// const struct nk_draw_command* nk__draw_end(const struct nk_context *ctx, const struct nk_buffer *buf);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct at the end of a frame
/// __buf__     | Must point to an previously by `nk_convert` filled out vertex draw command buffer
///
/// Returns vertex draw command pointer pointing to the end of the last vertex draw command inside the vertex draw command buffer
*/
NK_API const struct nk_draw_command* nk__draw_end(const struct nk_context*, const struct nk_buffer*);
/*/// #### nk__draw_next
/// Increments the vertex draw command buffer iterator
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// const struct nk_draw_command* nk__draw_next(const struct nk_draw_command*, const struct nk_buffer*, const struct nk_context*);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __cmd__     | Must point to an previously either by `nk__draw_begin` or `nk__draw_next` returned vertex draw command
/// __buf__     | Must point to an previously by `nk_convert` filled out vertex draw command buffer
/// __ctx__     | Must point to an previously initialized `nk_context` struct at the end of a frame
///
/// Returns vertex draw command pointer pointing to the end of the last vertex draw command inside the vertex draw command buffer
*/
NK_API const struct nk_draw_command* nk__draw_next(const struct nk_draw_command*, const struct nk_buffer*, const struct nk_context*);
/*/// #### nk_draw_foreach
/// Iterates over each vertex draw command inside a vertex draw command buffer
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// #define nk_draw_foreach(cmd,ctx, b)
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __cmd__     | `nk_draw_command`iterator set to NULL
/// __buf__     | Must point to an previously by `nk_convert` filled out vertex draw command buffer
/// __ctx__     | Must point to an previously initialized `nk_context` struct at the end of a frame
*/
#define nk_draw_foreach(cmd,ctx, b) for((cmd)=nk__draw_begin(ctx, b); (cmd)!=0; (cmd)=nk__draw_next(cmd, b, ctx))
#endif
/* =============================================================================
 *
 *                                  WINDOW
 *
 * =============================================================================
/// ### Window
/// Windows are the main persistent state used inside nuklear and are life time
/// controlled by simply "retouching" (i.e. calling) each window each frame.
/// All widgets inside nuklear can only be added inside the function pair `nk_begin_xxx`
/// and `nk_end`. Calling any widgets outside these two functions will result in an
/// assert in debug or no state change in release mode.<br /><br />
///
/// Each window holds frame persistent state like position, size, flags, state tables,
/// and some garbage collected internal persistent widget state. Each window
/// is linked into a window stack list which determines the drawing and overlapping
/// order. The topmost window thereby is the currently active window.<br /><br />
///
/// To change window position inside the stack occurs either automatically by
/// user input by being clicked on or programmatically by calling `nk_window_focus`.
/// Windows by default are visible unless explicitly being defined with flag
/// `NK_WINDOW_HIDDEN`, the user clicked the close button on windows with flag
/// `NK_WINDOW_CLOSABLE` or if a window was explicitly hidden by calling
/// `nk_window_show`. To explicitly close and destroy a window call `nk_window_close`.<br /><br />
///
/// #### Usage
/// To create and keep a window you have to call one of the two `nk_begin_xxx`
/// functions to start window declarations and `nk_end` at the end. Furthermore it
/// is recommended to check the return value of `nk_begin_xxx` and only process
/// widgets inside the window if the value is not 0. Either way you have to call
/// `nk_end` at the end of window declarations. Furthermore, do not attempt to
/// nest `nk_begin_xxx` calls which will hopefully result in an assert or if not
/// in a segmentation fault.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// if (nk_begin_xxx(...) {
///     // [... widgets ...]
/// }
/// nk_end(ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// In the grand concept window and widget declarations need to occur after input
/// handling and before drawing to screen. Not doing so can result in higher
/// latency or at worst invalid behavior. Furthermore make sure that `nk_clear`
/// is called at the end of the frame. While nuklear's default platform backends
/// already call `nk_clear` for you if you write your own backend not calling
/// `nk_clear` can cause asserts or even worse undefined behavior.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// struct nk_context ctx;
/// nk_init_xxx(&ctx, ...);
/// while (1) {
///     Event evt;
///     nk_input_begin(&ctx);
///     while (GetEvent(&evt)) {
///         if (evt.type == MOUSE_MOVE)
///             nk_input_motion(&ctx, evt.motion.x, evt.motion.y);
///         else if (evt.type == [...]) {
///             nk_input_xxx(...);
///         }
///     }
///     nk_input_end(&ctx);
///
///     if (nk_begin_xxx(...) {
///         //[...]
///     }
///     nk_end(ctx);
///
///     const struct nk_command *cmd = 0;
///     nk_foreach(cmd, &ctx) {
///     case NK_COMMAND_LINE:
///         your_draw_line_function(...)
///         break;
///     case NK_COMMAND_RECT
///         your_draw_rect_function(...)
///         break;
///     case //...:
///         //[...]
///     }
///     nk_clear(&ctx);
/// }
/// nk_free(&ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// #### Reference
/// Function                            | Description
/// ------------------------------------|----------------------------------------
/// nk_begin                            | Starts a new window; needs to be called every frame for every window (unless hidden) or otherwise the window gets removed
/// nk_begin_titled                     | Extended window start with separated title and identifier to allow multiple windows with same name but not title
/// nk_end                              | Needs to be called at the end of the window building process to process scaling, scrollbars and general cleanup
//
/// nk_window_find                      | Finds and returns the window with give name
/// nk_window_get_bounds                | Returns a rectangle with screen position and size of the currently processed window.
/// nk_window_get_position              | Returns the position of the currently processed window
/// nk_window_get_size                  | Returns the size with width and height of the currently processed window
/// nk_window_get_width                 | Returns the width of the currently processed window
/// nk_window_get_height                | Returns the height of the currently processed window
/// nk_window_get_panel                 | Returns the underlying panel which contains all processing state of the current window
/// nk_window_get_content_region        | Returns the position and size of the currently visible and non-clipped space inside the currently processed window
/// nk_window_get_content_region_min    | Returns the upper rectangle position of the currently visible and non-clipped space inside the currently processed window
/// nk_window_get_content_region_max    | Returns the upper rectangle position of the currently visible and non-clipped space inside the currently processed window
/// nk_window_get_content_region_size   | Returns the size of the currently visible and non-clipped space inside the currently processed window
/// nk_window_get_canvas                | Returns the draw command buffer. Can be used to draw custom widgets
/// nk_window_get_scroll                | Gets the scroll offset of the current window
/// nk_window_has_focus                 | Returns if the currently processed window is currently active
/// nk_window_is_collapsed              | Returns if the window with given name is currently minimized/collapsed
/// nk_window_is_closed                 | Returns if the currently processed window was closed
/// nk_window_is_hidden                 | Returns if the currently processed window was hidden
/// nk_window_is_active                 | Same as nk_window_has_focus for some reason
/// nk_window_is_hovered                | Returns if the currently processed window is currently being hovered by mouse
/// nk_window_is_any_hovered            | Return if any window currently hovered
/// nk_item_is_any_active               | Returns if any window or widgets is currently hovered or active
//
/// nk_window_set_bounds                | Updates position and size of the currently processed window
/// nk_window_set_position              | Updates position of the currently process window
/// nk_window_set_size                  | Updates the size of the currently processed window
/// nk_window_set_focus                 | Set the currently processed window as active window
/// nk_window_set_scroll                | Sets the scroll offset of the current window
//
/// nk_window_close                     | Closes the window with given window name which deletes the window at the end of the frame
/// nk_window_collapse                  | Collapses the window with given window name
/// nk_window_collapse_if               | Collapses the window with given window name if the given condition was met
/// nk_window_show                      | Hides a visible or reshows a hidden window
/// nk_window_show_if                   | Hides/shows a window depending on condition
*/
/*
/// #### nk_panel_flags
/// Flag                        | Description
/// ----------------------------|----------------------------------------
/// NK_WINDOW_BORDER            | Draws a border around the window to visually separate window from the background
/// NK_WINDOW_MOVABLE           | The movable flag indicates that a window can be moved by user input or by dragging the window header
/// NK_WINDOW_SCALABLE          | The scalable flag indicates that a window can be scaled by user input by dragging a scaler icon at the button of the window
/// NK_WINDOW_CLOSABLE          | Adds a closable icon into the header
/// NK_WINDOW_MINIMIZABLE       | Adds a minimize icon into the header
/// NK_WINDOW_NO_SCROLLBAR      | Removes the scrollbar from the window
/// NK_WINDOW_TITLE             | Forces a header at the top at the window showing the title
/// NK_WINDOW_SCROLL_AUTO_HIDE  | Automatically hides the window scrollbar if no user interaction: also requires delta time in `nk_context` to be set each frame
/// NK_WINDOW_BACKGROUND        | Always keep window in the background
/// NK_WINDOW_SCALE_LEFT        | Puts window scaler in the left-bottom corner instead right-bottom
/// NK_WINDOW_NO_INPUT          | Prevents window of scaling, moving or getting focus
///
/// #### nk_collapse_states
/// State           | Description
/// ----------------|-----------------------------------------------------------
/// __NK_MINIMIZED__| UI section is collapsed and not visible until maximized
/// __NK_MAXIMIZED__| UI section is extended and visible until minimized
/// <br /><br />
*/
enum nk_panel_flags {
    NK_WINDOW_BORDER            = NK_FLAG(0),
    NK_WINDOW_MOVABLE           = NK_FLAG(1),
    NK_WINDOW_SCALABLE          = NK_FLAG(2),
    NK_WINDOW_CLOSABLE          = NK_FLAG(3),
    NK_WINDOW_MINIMIZABLE       = NK_FLAG(4),
    NK_WINDOW_NO_SCROLLBAR      = NK_FLAG(5),
    NK_WINDOW_TITLE             = NK_FLAG(6),
    NK_WINDOW_SCROLL_AUTO_HIDE  = NK_FLAG(7),
    NK_WINDOW_BACKGROUND        = NK_FLAG(8),
    NK_WINDOW_SCALE_LEFT        = NK_FLAG(9),
    NK_WINDOW_NO_INPUT          = NK_FLAG(10)
};
/*/// #### nk_begin
/// Starts a new window; needs to be called every frame for every
/// window (unless hidden) or otherwise the window gets removed
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_bool nk_begin(struct nk_context *ctx, const char *title, struct nk_rect bounds, nk_flags flags);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __title__   | Window title and identifier. Needs to be persistent over frames to identify the window
/// __bounds__  | Initial position and window size. However if you do not define `NK_WINDOW_SCALABLE` or `NK_WINDOW_MOVABLE` you can set window position and size every frame
/// __flags__   | Window flags defined in the nk_panel_flags section with a number of different window behaviors
///
/// Returns `true(1)` if the window can be filled up with widgets from this point
/// until `nk_end` or `false(0)` otherwise for example if minimized
*/
NK_API nk_bool nk_begin(struct nk_context *ctx, const char *title, struct nk_rect bounds, nk_flags flags);
/*/// #### nk_begin_titled
/// Extended window start with separated title and identifier to allow multiple
/// windows with same title but not name
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_bool nk_begin_titled(struct nk_context *ctx, const char *name, const char *title, struct nk_rect bounds, nk_flags flags);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __name__    | Window identifier. Needs to be persistent over frames to identify the window
/// __title__   | Window title displayed inside header if flag `NK_WINDOW_TITLE` or either `NK_WINDOW_CLOSABLE` or `NK_WINDOW_MINIMIZED` was set
/// __bounds__  | Initial position and window size. However if you do not define `NK_WINDOW_SCALABLE` or `NK_WINDOW_MOVABLE` you can set window position and size every frame
/// __flags__   | Window flags defined in the nk_panel_flags section with a number of different window behaviors
///
/// Returns `true(1)` if the window can be filled up with widgets from this point
/// until `nk_end` or `false(0)` otherwise for example if minimized
*/
NK_API nk_bool nk_begin_titled(struct nk_context *ctx, const char *name, const char *title, struct nk_rect bounds, nk_flags flags);
/*/// #### nk_end
/// Needs to be called at the end of the window building process to process scaling, scrollbars and general cleanup.
/// All widget calls after this functions will result in asserts or no state changes
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_end(struct nk_context *ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
*/
NK_API void nk_end(struct nk_context *ctx);
/*/// #### nk_window_find
/// Finds and returns a window from passed name
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// struct nk_window *nk_window_find(struct nk_context *ctx, const char *name);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __name__    | Window identifier
///
/// Returns a `nk_window` struct pointing to the identified window or NULL if
/// no window with the given name was found
*/
NK_API struct nk_window *nk_window_find(struct nk_context *ctx, const char *name);
/*/// #### nk_window_get_bounds
/// Returns a rectangle with screen position and size of the currently processed window
///
/// !!! WARNING
///     Only call this function between calls `nk_begin_xxx` and `nk_end`
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// struct nk_rect nk_window_get_bounds(const struct nk_context *ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
///
/// Returns a `nk_rect` struct with window upper left window position and size
*/
NK_API struct nk_rect nk_window_get_bounds(const struct nk_context *ctx);
/*/// #### nk_window_get_position
/// Returns the position of the currently processed window.
///
/// !!! WARNING
///     Only call this function between calls `nk_begin_xxx` and `nk_end`
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// struct nk_vec2 nk_window_get_position(const struct nk_context *ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
///
/// Returns a `nk_vec2` struct with window upper left position
*/
NK_API struct nk_vec2 nk_window_get_position(const struct nk_context *ctx);
/*/// #### nk_window_get_size
/// Returns the size with width and height of the currently processed window.
///
/// !!! WARNING
///     Only call this function between calls `nk_begin_xxx` and `nk_end`
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// struct nk_vec2 nk_window_get_size(const struct nk_context *ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
///
/// Returns a `nk_vec2` struct with window width and height
*/
NK_API struct nk_vec2 nk_window_get_size(const struct nk_context*);
/*/// #### nk_window_get_width
/// Returns the width of the currently processed window.
///
/// !!! WARNING
///     Only call this function between calls `nk_begin_xxx` and `nk_end`
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// float nk_window_get_width(const struct nk_context *ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
///
/// Returns the current window width
*/
NK_API float nk_window_get_width(const struct nk_context*);
/*/// #### nk_window_get_height
/// Returns the height of the currently processed window.
///
/// !!! WARNING
///     Only call this function between calls `nk_begin_xxx` and `nk_end`
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// float nk_window_get_height(const struct nk_context *ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
///
/// Returns the current window height
*/
NK_API float nk_window_get_height(const struct nk_context*);
/*/// #### nk_window_get_panel
/// Returns the underlying panel which contains all processing state of the current window.
///
/// !!! WARNING
///     Only call this function between calls `nk_begin_xxx` and `nk_end`
/// !!! WARNING
///     Do not keep the returned panel pointer around, it is only valid until `nk_end`
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// struct nk_panel* nk_window_get_panel(struct nk_context *ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
///
/// Returns a pointer to window internal `nk_panel` state.
*/
NK_API struct nk_panel* nk_window_get_panel(struct nk_context*);
/*/// #### nk_window_get_content_region
/// Returns the position and size of the currently visible and non-clipped space
/// inside the currently processed window.
///
/// !!! WARNING
///     Only call this function between calls `nk_begin_xxx` and `nk_end`
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// struct nk_rect nk_window_get_content_region(struct nk_context *ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
///
/// Returns `nk_rect` struct with screen position and size (no scrollbar offset)
/// of the visible space inside the current window
*/
NK_API struct nk_rect nk_window_get_content_region(struct nk_context*);
/*/// #### nk_window_get_content_region_min
/// Returns the upper left position of the currently visible and non-clipped
/// space inside the currently processed window.
///
/// !!! WARNING
///     Only call this function between calls `nk_begin_xxx` and `nk_end`
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// struct nk_vec2 nk_window_get_content_region_min(struct nk_context *ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
///
/// returns `nk_vec2` struct with  upper left screen position (no scrollbar offset)
/// of the visible space inside the current window
*/
NK_API struct nk_vec2 nk_window_get_content_region_min(struct nk_context*);
/*/// #### nk_window_get_content_region_max
/// Returns the lower right screen position of the currently visible and
/// non-clipped space inside the currently processed window.
///
/// !!! WARNING
///     Only call this function between calls `nk_begin_xxx` and `nk_end`
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// struct nk_vec2 nk_window_get_content_region_max(struct nk_context *ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
///
/// Returns `nk_vec2` struct with lower right screen position (no scrollbar offset)
/// of the visible space inside the current window
*/
NK_API struct nk_vec2 nk_window_get_content_region_max(struct nk_context*);
/*/// #### nk_window_get_content_region_size
/// Returns the size of the currently visible and non-clipped space inside the
/// currently processed window
///
/// !!! WARNING
///     Only call this function between calls `nk_begin_xxx` and `nk_end`
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// struct nk_vec2 nk_window_get_content_region_size(struct nk_context *ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
///
/// Returns `nk_vec2` struct with size the visible space inside the current window
*/
NK_API struct nk_vec2 nk_window_get_content_region_size(struct nk_context*);
/*/// #### nk_window_get_canvas
/// Returns the draw command buffer. Can be used to draw custom widgets
/// !!! WARNING
///     Only call this function between calls `nk_begin_xxx` and `nk_end`
/// !!! WARNING
///     Do not keep the returned command buffer pointer around it is only valid until `nk_end`
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// struct nk_command_buffer* nk_window_get_canvas(struct nk_context *ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
///
/// Returns a pointer to window internal `nk_command_buffer` struct used as
/// drawing canvas. Can be used to do custom drawing.
*/
NK_API struct nk_command_buffer* nk_window_get_canvas(struct nk_context*);
/*/// #### nk_window_get_scroll
/// Gets the scroll offset for the current window
/// !!! WARNING
///     Only call this function between calls `nk_begin_xxx` and `nk_end`
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_window_get_scroll(struct nk_context *ctx, nk_uint *offset_x, nk_uint *offset_y);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter    | Description
/// -------------|-----------------------------------------------------------
/// __ctx__      | Must point to an previously initialized `nk_context` struct
/// __offset_x__ | A pointer to the x offset output (or NULL to ignore)
/// __offset_y__ | A pointer to the y offset output (or NULL to ignore)
*/
NK_API void nk_window_get_scroll(struct nk_context*, nk_uint *offset_x, nk_uint *offset_y);
/*/// #### nk_window_has_focus
/// Returns if the currently processed window is currently active
/// !!! WARNING
///     Only call this function between calls `nk_begin_xxx` and `nk_end`
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_bool nk_window_has_focus(const struct nk_context *ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
///
/// Returns `false(0)` if current window is not active or `true(1)` if it is
*/
NK_API nk_bool nk_window_has_focus(const struct nk_context*);
/*/// #### nk_window_is_hovered
/// Return if the current window is being hovered
/// !!! WARNING
///     Only call this function between calls `nk_begin_xxx` and `nk_end`
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_bool nk_window_is_hovered(struct nk_context *ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
///
/// Returns `true(1)` if current window is hovered or `false(0)` otherwise
*/
NK_API nk_bool nk_window_is_hovered(struct nk_context*);
/*/// #### nk_window_is_collapsed
/// Returns if the window with given name is currently minimized/collapsed
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_bool nk_window_is_collapsed(struct nk_context *ctx, const char *name);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __name__    | Identifier of window you want to check if it is collapsed
///
/// Returns `true(1)` if current window is minimized and `false(0)` if window not
/// found or is not minimized
*/
NK_API nk_bool nk_window_is_collapsed(struct nk_context *ctx, const char *name);
/*/// #### nk_window_is_closed
/// Returns if the window with given name was closed by calling `nk_close`
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_bool nk_window_is_closed(struct nk_context *ctx, const char *name);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __name__    | Identifier of window you want to check if it is closed
///
/// Returns `true(1)` if current window was closed or `false(0)` window not found or not closed
*/
NK_API nk_bool nk_window_is_closed(struct nk_context*, const char*);
/*/// #### nk_window_is_hidden
/// Returns if the window with given name is hidden
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_bool nk_window_is_hidden(struct nk_context *ctx, const char *name);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __name__    | Identifier of window you want to check if it is hidden
///
/// Returns `true(1)` if current window is hidden or `false(0)` window not found or visible
*/
NK_API nk_bool nk_window_is_hidden(struct nk_context*, const char*);
/*/// #### nk_window_is_active
/// Same as nk_window_has_focus for some reason
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_bool nk_window_is_active(struct nk_context *ctx, const char *name);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __name__    | Identifier of window you want to check if it is active
///
/// Returns `true(1)` if current window is active or `false(0)` window not found or not active
*/
NK_API nk_bool nk_window_is_active(struct nk_context*, const char*);
/*/// #### nk_window_is_any_hovered
/// Returns if the any window is being hovered
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_bool nk_window_is_any_hovered(struct nk_context*);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
///
/// Returns `true(1)` if any window is hovered or `false(0)` otherwise
*/
NK_API nk_bool nk_window_is_any_hovered(struct nk_context*);
/*/// #### nk_item_is_any_active
/// Returns if the any window is being hovered or any widget is currently active.
/// Can be used to decide if input should be processed by UI or your specific input handling.
/// Example could be UI and 3D camera to move inside a 3D space.
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_bool nk_item_is_any_active(struct nk_context*);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
///
/// Returns `true(1)` if any window is hovered or any item is active or `false(0)` otherwise
*/
NK_API nk_bool nk_item_is_any_active(struct nk_context*);
/*/// #### nk_window_set_bounds
/// Updates position and size of window with passed in name
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_window_set_bounds(struct nk_context*, const char *name, struct nk_rect bounds);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __name__    | Identifier of the window to modify both position and size
/// __bounds__  | Must point to a `nk_rect` struct with the new position and size
*/
NK_API void nk_window_set_bounds(struct nk_context*, const char *name, struct nk_rect bounds);
/*/// #### nk_window_set_position
/// Updates position of window with passed name
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_window_set_position(struct nk_context*, const char *name, struct nk_vec2 pos);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __name__    | Identifier of the window to modify both position
/// __pos__     | Must point to a `nk_vec2` struct with the new position
*/
NK_API void nk_window_set_position(struct nk_context*, const char *name, struct nk_vec2 pos);
/*/// #### nk_window_set_size
/// Updates size of window with passed in name
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_window_set_size(struct nk_context*, const char *name, struct nk_vec2);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __name__    | Identifier of the window to modify both window size
/// __size__    | Must point to a `nk_vec2` struct with new window size
*/
NK_API void nk_window_set_size(struct nk_context*, const char *name, struct nk_vec2);
/*/// #### nk_window_set_focus
/// Sets the window with given name as active
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_window_set_focus(struct nk_context*, const char *name);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __name__    | Identifier of the window to set focus on
*/
NK_API void nk_window_set_focus(struct nk_context*, const char *name);
/*/// #### nk_window_set_scroll
/// Sets the scroll offset for the current window
/// !!! WARNING
///     Only call this function between calls `nk_begin_xxx` and `nk_end`
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_window_set_scroll(struct nk_context *ctx, nk_uint offset_x, nk_uint offset_y);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter    | Description
/// -------------|-----------------------------------------------------------
/// __ctx__      | Must point to an previously initialized `nk_context` struct
/// __offset_x__ | The x offset to scroll to
/// __offset_y__ | The y offset to scroll to
*/
NK_API void nk_window_set_scroll(struct nk_context*, nk_uint offset_x, nk_uint offset_y);
/*/// #### nk_window_close
/// Closes a window and marks it for being freed at the end of the frame
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_window_close(struct nk_context *ctx, const char *name);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __name__    | Identifier of the window to close
*/
NK_API void nk_window_close(struct nk_context *ctx, const char *name);
/*/// #### nk_window_collapse
/// Updates collapse state of a window with given name
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_window_collapse(struct nk_context*, const char *name, enum nk_collapse_states state);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __name__    | Identifier of the window to close
/// __state__   | value out of nk_collapse_states section
*/
NK_API void nk_window_collapse(struct nk_context*, const char *name, enum nk_collapse_states state);
/*/// #### nk_window_collapse_if
/// Updates collapse state of a window with given name if given condition is met
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_window_collapse_if(struct nk_context*, const char *name, enum nk_collapse_states, int cond);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __name__    | Identifier of the window to either collapse or maximize
/// __state__   | value out of nk_collapse_states section the window should be put into
/// __cond__    | condition that has to be met to actually commit the collapse state change
*/
NK_API void nk_window_collapse_if(struct nk_context*, const char *name, enum nk_collapse_states, int cond);
/*/// #### nk_window_show
/// updates visibility state of a window with given name
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_window_show(struct nk_context*, const char *name, enum nk_show_states);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __name__    | Identifier of the window to either collapse or maximize
/// __state__   | state with either visible or hidden to modify the window with
*/
NK_API void nk_window_show(struct nk_context*, const char *name, enum nk_show_states);
/*/// #### nk_window_show_if
/// Updates visibility state of a window with given name if a given condition is met
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_window_show_if(struct nk_context*, const char *name, enum nk_show_states, int cond);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __name__    | Identifier of the window to either hide or show
/// __state__   | state with either visible or hidden to modify the window with
/// __cond__    | condition that has to be met to actually commit the visibility state change
*/
NK_API void nk_window_show_if(struct nk_context*, const char *name, enum nk_show_states, int cond);
/*/// #### nk_window_show_if
/// Line for visual separation. Draws a line with thickness determined by the current row height.
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_rule_horizontal(struct nk_context *ctx, struct nk_color color, NK_BOOL rounding)
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter       | Description
/// ----------------|-------------------------------------------------------
/// __ctx__         | Must point to an previously initialized `nk_context` struct
/// __color__       | Color of the horizontal line
/// __rounding__    | Whether or not to make the line round
*/
NK_API void nk_rule_horizontal(struct nk_context *ctx, struct nk_color color, nk_bool rounding);
/* =============================================================================
 *
 *                                  LAYOUT
 *
 * =============================================================================
/// ### Layouting
/// Layouting in general describes placing widget inside a window with position and size.
/// While in this particular implementation there are five different APIs for layouting
/// each with different trade offs between control and ease of use. <br /><br />
///
/// All layouting methods in this library are based around the concept of a row.
/// A row has a height the window content grows by and a number of columns and each
/// layouting method specifies how each widget is placed inside the row.
/// After a row has been allocated by calling a layouting functions and then
/// filled with widgets will advance an internal pointer over the allocated row. <br /><br />
///
/// To actually define a layout you just call the appropriate layouting function
/// and each subsequent widget call will place the widget as specified. Important
/// here is that if you define more widgets then columns defined inside the layout
/// functions it will allocate the next row without you having to make another layouting <br /><br />
/// call.
///
/// Biggest limitation with using all these APIs outside the `nk_layout_space_xxx` API
/// is that you have to define the row height for each. However the row height
/// often depends on the height of the font. <br /><br />
///
/// To fix that internally nuklear uses a minimum row height that is set to the
/// height plus padding of currently active font and overwrites the row height
/// value if zero. <br /><br />
///
/// If you manually want to change the minimum row height then
/// use nk_layout_set_min_row_height, and use nk_layout_reset_min_row_height to
/// reset it back to be derived from font height. <br /><br />
///
/// Also if you change the font in nuklear it will automatically change the minimum
/// row height for you and. This means if you change the font but still want
/// a minimum row height smaller than the font you have to repush your value. <br /><br />
///
/// For actually more advanced UI I would even recommend using the `nk_layout_space_xxx`
/// layouting method in combination with a cassowary constraint solver (there are
/// some versions on github with permissive license model) to take over all control over widget
/// layouting yourself. However for quick and dirty layouting using all the other layouting
/// functions should be fine.
///
/// #### Usage
/// 1.  __nk_layout_row_dynamic__<br /><br />
///     The easiest layouting function is `nk_layout_row_dynamic`. It provides each
///     widgets with same horizontal space inside the row and dynamically grows
///     if the owning window grows in width. So the number of columns dictates
///     the size of each widget dynamically by formula:
///
///     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
///     widget_width = (window_width - padding - spacing) * (1/column_count)
///     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
///     Just like all other layouting APIs if you define more widget than columns this
///     library will allocate a new row and keep all layouting parameters previously
///     defined.
///
///     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
///     if (nk_begin_xxx(...) {
///         // first row with height: 30 composed of two widgets
///         nk_layout_row_dynamic(&ctx, 30, 2);
///         nk_widget(...);
///         nk_widget(...);
///         //
///         // second row with same parameter as defined above
///         nk_widget(...);
///         nk_widget(...);
///         //
///         // third row uses 0 for height which will use auto layouting
///         nk_layout_row_dynamic(&ctx, 0, 2);
///         nk_widget(...);
///         nk_widget(...);
///     }
///     nk_end(...);
///     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// 2.  __nk_layout_row_static__<br /><br />
///     Another easy layouting function is `nk_layout_row_static`. It provides each
///     widget with same horizontal pixel width inside the row and does not grow
///     if the owning window scales smaller or bigger.
///
///     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
///     if (nk_begin_xxx(...) {
///         // first row with height: 30 composed of two widgets with width: 80
///         nk_layout_row_static(&ctx, 30, 80, 2);
///         nk_widget(...);
///         nk_widget(...);
///         //
///         // second row with same parameter as defined above
///         nk_widget(...);
///         nk_widget(...);
///         //
///         // third row uses 0 for height which will use auto layouting
///         nk_layout_row_static(&ctx, 0, 80, 2);
///         nk_widget(...);
///         nk_widget(...);
///     }
///     nk_end(...);
///     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// 3.  __nk_layout_row_xxx__<br /><br />
///     A little bit more advanced layouting API are functions `nk_layout_row_begin`,
///     `nk_layout_row_push` and `nk_layout_row_end`. They allow to directly
///     specify each column pixel or window ratio in a row. It supports either
///     directly setting per column pixel width or widget window ratio but not
///     both. Furthermore it is a immediate mode API so each value is directly
///     pushed before calling a widget. Therefore the layout is not automatically
///     repeating like the last two layouting functions.
///
///     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
///     if (nk_begin_xxx(...) {
///         // first row with height: 25 composed of two widgets with width 60 and 40
///         nk_layout_row_begin(ctx, NK_STATIC, 25, 2);
///         nk_layout_row_push(ctx, 60);
///         nk_widget(...);
///         nk_layout_row_push(ctx, 40);
///         nk_widget(...);
///         nk_layout_row_end(ctx);
///         //
///         // second row with height: 25 composed of two widgets with window ratio 0.25 and 0.75
///         nk_layout_row_begin(ctx, NK_DYNAMIC, 25, 2);
///         nk_layout_row_push(ctx, 0.25f);
///         nk_widget(...);
///         nk_layout_row_push(ctx, 0.75f);
///         nk_widget(...);
///         nk_layout_row_end(ctx);
///         //
///         // third row with auto generated height: composed of two widgets with window ratio 0.25 and 0.75
///         nk_layout_row_begin(ctx, NK_DYNAMIC, 0, 2);
///         nk_layout_row_push(ctx, 0.25f);
///         nk_widget(...);
///         nk_layout_row_push(ctx, 0.75f);
///         nk_widget(...);
///         nk_layout_row_end(ctx);
///     }
///     nk_end(...);
///     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// 4.  __nk_layout_row__<br /><br />
///     The array counterpart to API nk_layout_row_xxx is the single nk_layout_row
///     functions. Instead of pushing either pixel or window ratio for every widget
///     it allows to define it by array. The trade of for less control is that
///     `nk_layout_row` is automatically repeating. Otherwise the behavior is the
///     same.
///
///     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
///     if (nk_begin_xxx(...) {
///         // two rows with height: 30 composed of two widgets with width 60 and 40
///         const float ratio[] = {60,40};
///         nk_layout_row(ctx, NK_STATIC, 30, 2, ratio);
///         nk_widget(...);
///         nk_widget(...);
///         nk_widget(...);
///         nk_widget(...);
///         //
///         // two rows with height: 30 composed of two widgets with window ratio 0.25 and 0.75
///         const float ratio[] = {0.25, 0.75};
///         nk_layout_row(ctx, NK_DYNAMIC, 30, 2, ratio);
///         nk_widget(...);
///         nk_widget(...);
///         nk_widget(...);
///         nk_widget(...);
///         //
///         // two rows with auto generated height composed of two widgets with window ratio 0.25 and 0.75
///         const float ratio[] = {0.25, 0.75};
///         nk_layout_row(ctx, NK_DYNAMIC, 30, 2, ratio);
///         nk_widget(...);
///         nk_widget(...);
///         nk_widget(...);
///         nk_widget(...);
///     }
///     nk_end(...);
///     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// 5.  __nk_layout_row_template_xxx__<br /><br />
///     The most complex and second most flexible API is a simplified flexbox version without
///     line wrapping and weights for dynamic widgets. It is an immediate mode API but
///     unlike `nk_layout_row_xxx` it has auto repeat behavior and needs to be called
///     before calling the templated widgets.
///     The row template layout has three different per widget size specifier. The first
///     one is the `nk_layout_row_template_push_static`  with fixed widget pixel width.
///     They do not grow if the row grows and will always stay the same.
///     The second size specifier is `nk_layout_row_template_push_variable`
///     which defines a minimum widget size but it also can grow if more space is available
///     not taken by other widgets.
///     Finally there are dynamic widgets with `nk_layout_row_template_push_dynamic`
///     which are completely flexible and unlike variable widgets can even shrink
///     to zero if not enough space is provided.
///
///     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
///     if (nk_begin_xxx(...) {
///         // two rows with height: 30 composed of three widgets
///         nk_layout_row_template_begin(ctx, 30);
///         nk_layout_row_template_push_dynamic(ctx);
///         nk_layout_row_template_push_variable(ctx, 80);
///         nk_layout_row_template_push_static(ctx, 80);
///         nk_layout_row_template_end(ctx);
///         //
///         // first row
///         nk_widget(...); // dynamic widget can go to zero if not enough space
///         nk_widget(...); // variable widget with min 80 pixel but can grow bigger if enough space
///         nk_widget(...); // static widget with fixed 80 pixel width
///         //
///         // second row same layout
///         nk_widget(...);
///         nk_widget(...);
///         nk_widget(...);
///     }
///     nk_end(...);
///     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// 6.  __nk_layout_space_xxx__<br /><br />
///     Finally the most flexible API directly allows you to place widgets inside the
///     window. The space layout API is an immediate mode API which does not support
///     row auto repeat and directly sets position and size of a widget. Position
///     and size hereby can be either specified as ratio of allocated space or
///     allocated space local position and pixel size. Since this API is quite
///     powerful there are a number of utility functions to get the available space
///     and convert between local allocated space and screen space.
///
///     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
///     if (nk_begin_xxx(...) {
///         // static row with height: 500 (you can set column count to INT_MAX if you don't want to be bothered)
///         nk_layout_space_begin(ctx, NK_STATIC, 500, INT_MAX);
///         nk_layout_space_push(ctx, nk_rect(0,0,150,200));
///         nk_widget(...);
///         nk_layout_space_push(ctx, nk_rect(200,200,100,200));
///         nk_widget(...);
///         nk_layout_space_end(ctx);
///         //
///         // dynamic row with height: 500 (you can set column count to INT_MAX if you don't want to be bothered)
///         nk_layout_space_begin(ctx, NK_DYNAMIC, 500, INT_MAX);
///         nk_layout_space_push(ctx, nk_rect(0.5,0.5,0.1,0.1));
///         nk_widget(...);
///         nk_layout_space_push(ctx, nk_rect(0.7,0.6,0.1,0.1));
///         nk_widget(...);
///     }
///     nk_end(...);
///     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// #### Reference
/// Function                                | Description
/// ----------------------------------------|------------------------------------
/// nk_layout_set_min_row_height            | Set the currently used minimum row height to a specified value
/// nk_layout_reset_min_row_height          | Resets the currently used minimum row height to font height
/// nk_layout_widget_bounds                 | Calculates current width a static layout row can fit inside a window
/// nk_layout_ratio_from_pixel              | Utility functions to calculate window ratio from pixel size
//
/// nk_layout_row_dynamic                   | Current layout is divided into n same sized growing columns
/// nk_layout_row_static                    | Current layout is divided into n same fixed sized columns
/// nk_layout_row_begin                     | Starts a new row with given height and number of columns
/// nk_layout_row_push                      | Pushes another column with given size or window ratio
/// nk_layout_row_end                       | Finished previously started row
/// nk_layout_row                           | Specifies row columns in array as either window ratio or size
//
/// nk_layout_row_template_begin            | Begins the row template declaration
/// nk_layout_row_template_push_dynamic     | Adds a dynamic column that dynamically grows and can go to zero if not enough space
/// nk_layout_row_template_push_variable    | Adds a variable column that dynamically grows but does not shrink below specified pixel width
/// nk_layout_row_template_push_static      | Adds a static column that does not grow and will always have the same size
/// nk_layout_row_template_end              | Marks the end of the row template
//
/// nk_layout_space_begin                   | Begins a new layouting space that allows to specify each widgets position and size
/// nk_layout_space_push                    | Pushes position and size of the next widget in own coordinate space either as pixel or ratio
/// nk_layout_space_end                     | Marks the end of the layouting space
//
/// nk_layout_space_bounds                  | Callable after nk_layout_space_begin and returns total space allocated
/// nk_layout_space_to_screen               | Converts vector from nk_layout_space coordinate space into screen space
/// nk_layout_space_to_local                | Converts vector from screen space into nk_layout_space coordinates
/// nk_layout_space_rect_to_screen          | Converts rectangle from nk_layout_space coordinate space into screen space
/// nk_layout_space_rect_to_local           | Converts rectangle from screen space into nk_layout_space coordinates
*/

enum nk_widget_align {
    NK_WIDGET_ALIGN_LEFT        = 0x01,
    NK_WIDGET_ALIGN_CENTERED    = 0x02,
    NK_WIDGET_ALIGN_RIGHT       = 0x04,
    NK_WIDGET_ALIGN_TOP         = 0x08,
    NK_WIDGET_ALIGN_MIDDLE      = 0x10,
    NK_WIDGET_ALIGN_BOTTOM      = 0x20
};
enum nk_widget_alignment {
    NK_WIDGET_LEFT        = NK_WIDGET_ALIGN_MIDDLE|NK_WIDGET_ALIGN_LEFT,
    NK_WIDGET_CENTERED    = NK_WIDGET_ALIGN_MIDDLE|NK_WIDGET_ALIGN_CENTERED,
    NK_WIDGET_RIGHT       = NK_WIDGET_ALIGN_MIDDLE|NK_WIDGET_ALIGN_RIGHT
};

/*/// #### nk_layout_set_min_row_height
/// Sets the currently used minimum row height.
/// !!! WARNING
///     The passed height needs to include both your preferred row height
///     as well as padding. No internal padding is added.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_layout_set_min_row_height(struct nk_context*, float height);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_begin_xxx`
/// __height__  | New minimum row height to be used for auto generating the row height
*/
NK_API void nk_layout_set_min_row_height(struct nk_context*, float height);
/*/// #### nk_layout_reset_min_row_height
/// Reset the currently used minimum row height back to `font_height + text_padding + padding`
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_layout_reset_min_row_height(struct nk_context*);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_begin_xxx`
*/
NK_API void nk_layout_reset_min_row_height(struct nk_context*);
/*/// #### nk_layout_widget_bounds
/// Returns the width of the next row allocate by one of the layouting functions
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// struct nk_rect nk_layout_widget_bounds(struct nk_context*);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_begin_xxx`
///
/// Return `nk_rect` with both position and size of the next row
*/
NK_API struct nk_rect nk_layout_widget_bounds(struct nk_context*);
/*/// #### nk_layout_ratio_from_pixel
/// Utility functions to calculate window ratio from pixel size
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// float nk_layout_ratio_from_pixel(struct nk_context*, float pixel_width);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_begin_xxx`
/// __pixel__   | Pixel_width to convert to window ratio
///
/// Returns `nk_rect` with both position and size of the next row
*/
NK_API float nk_layout_ratio_from_pixel(struct nk_context*, float pixel_width);
/*/// #### nk_layout_row_dynamic
/// Sets current row layout to share horizontal space
/// between @cols number of widgets evenly. Once called all subsequent widget
/// calls greater than @cols will allocate a new row with same layout.
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_layout_row_dynamic(struct nk_context *ctx, float height, int cols);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_begin_xxx`
/// __height__  | Holds height of each widget in row or zero for auto layouting
/// __columns__ | Number of widget inside row
*/
NK_API void nk_layout_row_dynamic(struct nk_context *ctx, float height, int cols);
/*/// #### nk_layout_row_static
/// Sets current row layout to fill @cols number of widgets
/// in row with same @item_width horizontal size. Once called all subsequent widget
/// calls greater than @cols will allocate a new row with same layout.
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_layout_row_static(struct nk_context *ctx, float height, int item_width, int cols);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_begin_xxx`
/// __height__  | Holds height of each widget in row or zero for auto layouting
/// __width__   | Holds pixel width of each widget in the row
/// __columns__ | Number of widget inside row
*/
NK_API void nk_layout_row_static(struct nk_context *ctx, float height, int item_width, int cols);
/*/// #### nk_layout_row_begin
/// Starts a new dynamic or fixed row with given height and columns.
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_layout_row_begin(struct nk_context *ctx, enum nk_layout_format fmt, float row_height, int cols);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_begin_xxx`
/// __fmt__     | either `NK_DYNAMIC` for window ratio or `NK_STATIC` for fixed size columns
/// __height__  | holds height of each widget in row or zero for auto layouting
/// __columns__ | Number of widget inside row
*/
NK_API void nk_layout_row_begin(struct nk_context *ctx, enum nk_layout_format fmt, float row_height, int cols);
/*/// #### nk_layout_row_push
/// Specifies either window ratio or width of a single column
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_layout_row_push(struct nk_context*, float value);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_begin_xxx`
/// __value__   | either a window ratio or fixed width depending on @fmt in previous `nk_layout_row_begin` call
*/
NK_API void nk_layout_row_push(struct nk_context*, float value);
/*/// #### nk_layout_row_end
/// Finished previously started row
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_layout_row_end(struct nk_context*);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_begin_xxx`
*/
NK_API void nk_layout_row_end(struct nk_context*);
/*/// #### nk_layout_row
/// Specifies row columns in array as either window ratio or size
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_layout_row(struct nk_context*, enum nk_layout_format, float height, int cols, const float *ratio);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_begin_xxx`
/// __fmt__     | Either `NK_DYNAMIC` for window ratio or `NK_STATIC` for fixed size columns
/// __height__  | Holds height of each widget in row or zero for auto layouting
/// __columns__ | Number of widget inside row
*/
NK_API void nk_layout_row(struct nk_context*, enum nk_layout_format, float height, int cols, const float *ratio);
/*/// #### nk_layout_row_template_begin
/// Begins the row template declaration
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_layout_row_template_begin(struct nk_context*, float row_height);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_begin_xxx`
/// __height__  | Holds height of each widget in row or zero for auto layouting
*/
NK_API void nk_layout_row_template_begin(struct nk_context*, float row_height);
/*/// #### nk_layout_row_template_push_dynamic
/// Adds a dynamic column that dynamically grows and can go to zero if not enough space
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_layout_row_template_push_dynamic(struct nk_context*);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_begin_xxx`
/// __height__  | Holds height of each widget in row or zero for auto layouting
*/
NK_API void nk_layout_row_template_push_dynamic(struct nk_context*);
/*/// #### nk_layout_row_template_push_variable
/// Adds a variable column that dynamically grows but does not shrink below specified pixel width
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_layout_row_template_push_variable(struct nk_context*, float min_width);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_begin_xxx`
/// __width__   | Holds the minimum pixel width the next column must always be
*/
NK_API void nk_layout_row_template_push_variable(struct nk_context*, float min_width);
/*/// #### nk_layout_row_template_push_static
/// Adds a static column that does not grow and will always have the same size
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_layout_row_template_push_static(struct nk_context*, float width);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_begin_xxx`
/// __width__   | Holds the absolute pixel width value the next column must be
*/
NK_API void nk_layout_row_template_push_static(struct nk_context*, float width);
/*/// #### nk_layout_row_template_end
/// Marks the end of the row template
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_layout_row_template_end(struct nk_context*);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_begin_xxx`
*/
NK_API void nk_layout_row_template_end(struct nk_context*);
/*/// #### nk_layout_space_begin
/// Begins a new layouting space that allows to specify each widgets position and size.
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_layout_space_begin(struct nk_context*, enum nk_layout_format, float height, int widget_count);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_begin_xxx`
/// __fmt__     | Either `NK_DYNAMIC` for window ratio or `NK_STATIC` for fixed size columns
/// __height__  | Holds height of each widget in row or zero for auto layouting
/// __columns__ | Number of widgets inside row
*/
NK_API void nk_layout_space_begin(struct nk_context*, enum nk_layout_format, float height, int widget_count);
/*/// #### nk_layout_space_push
/// Pushes position and size of the next widget in own coordinate space either as pixel or ratio
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_layout_space_push(struct nk_context *ctx, struct nk_rect bounds);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_layout_space_begin`
/// __bounds__  | Position and size in laoyut space local coordinates
*/
NK_API void nk_layout_space_push(struct nk_context*, struct nk_rect bounds);
/*/// #### nk_layout_space_end
/// Marks the end of the layout space
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_layout_space_end(struct nk_context*);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_layout_space_begin`
*/
NK_API void nk_layout_space_end(struct nk_context*);
/*/// #### nk_layout_space_bounds
/// Utility function to calculate total space allocated for `nk_layout_space`
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// struct nk_rect nk_layout_space_bounds(struct nk_context*);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_layout_space_begin`
///
/// Returns `nk_rect` holding the total space allocated
*/
NK_API struct nk_rect nk_layout_space_bounds(struct nk_context*);
/*/// #### nk_layout_space_to_screen
/// Converts vector from nk_layout_space coordinate space into screen space
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// struct nk_vec2 nk_layout_space_to_screen(struct nk_context*, struct nk_vec2);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_layout_space_begin`
/// __vec__     | Position to convert from layout space into screen coordinate space
///
/// Returns transformed `nk_vec2` in screen space coordinates
*/
NK_API struct nk_vec2 nk_layout_space_to_screen(struct nk_context*, struct nk_vec2);
/*/// #### nk_layout_space_to_local
/// Converts vector from layout space into screen space
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// struct nk_vec2 nk_layout_space_to_local(struct nk_context*, struct nk_vec2);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_layout_space_begin`
/// __vec__     | Position to convert from screen space into layout coordinate space
///
/// Returns transformed `nk_vec2` in layout space coordinates
*/
NK_API struct nk_vec2 nk_layout_space_to_local(struct nk_context*, struct nk_vec2);
/*/// #### nk_layout_space_rect_to_screen
/// Converts rectangle from screen space into layout space
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// struct nk_rect nk_layout_space_rect_to_screen(struct nk_context*, struct nk_rect);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_layout_space_begin`
/// __bounds__  | Rectangle to convert from layout space into screen space
///
/// Returns transformed `nk_rect` in screen space coordinates
*/
NK_API struct nk_rect nk_layout_space_rect_to_screen(struct nk_context*, struct nk_rect);
/*/// #### nk_layout_space_rect_to_local
/// Converts rectangle from layout space into screen space
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// struct nk_rect nk_layout_space_rect_to_local(struct nk_context*, struct nk_rect);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_layout_space_begin`
/// __bounds__  | Rectangle to convert from layout space into screen space
///
/// Returns transformed `nk_rect` in layout space coordinates
*/
NK_API struct nk_rect nk_layout_space_rect_to_local(struct nk_context*, struct nk_rect);

/*/// #### nk_spacer
/// Spacer is a dummy widget that consumes space as usual but doesn't draw anything
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_spacer(struct nk_context* );
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after call `nk_layout_space_begin`
///
*/
NK_API void nk_spacer(struct nk_context* );


/* =============================================================================
 *
 *                                  GROUP
 *
 * =============================================================================
/// ### Groups
/// Groups are basically windows inside windows. They allow to subdivide space
/// in a window to layout widgets as a group. Almost all more complex widget
/// layouting requirements can be solved using groups and basic layouting
/// fuctionality. Groups just like windows are identified by an unique name and
/// internally keep track of scrollbar offsets by default. However additional
/// versions are provided to directly manage the scrollbar.
///
/// #### Usage
/// To create a group you have to call one of the three `nk_group_begin_xxx`
/// functions to start group declarations and `nk_group_end` at the end. Furthermore it
/// is required to check the return value of `nk_group_begin_xxx` and only process
/// widgets inside the window if the value is not 0.
/// Nesting groups is possible and even encouraged since many layouting schemes
/// can only be achieved by nesting. Groups, unlike windows, need `nk_group_end`
/// to be only called if the corresponding `nk_group_begin_xxx` call does not return 0:
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// if (nk_group_begin_xxx(ctx, ...) {
///     // [... widgets ...]
///     nk_group_end(ctx);
/// }
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// In the grand concept groups can be called after starting a window
/// with `nk_begin_xxx` and before calling `nk_end`:
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// struct nk_context ctx;
/// nk_init_xxx(&ctx, ...);
/// while (1) {
///     // Input
///     Event evt;
///     nk_input_begin(&ctx);
///     while (GetEvent(&evt)) {
///         if (evt.type == MOUSE_MOVE)
///             nk_input_motion(&ctx, evt.motion.x, evt.motion.y);
///         else if (evt.type == [...]) {
///             nk_input_xxx(...);
///         }
///     }
///     nk_input_end(&ctx);
///     //
///     // Window
///     if (nk_begin_xxx(...) {
///         // [...widgets...]
///         nk_layout_row_dynamic(...);
///         if (nk_group_begin_xxx(ctx, ...) {
///             //[... widgets ...]
///             nk_group_end(ctx);
///         }
///     }
///     nk_end(ctx);
///     //
///     // Draw
///     const struct nk_command *cmd = 0;
///     nk_foreach(cmd, &ctx) {
///     switch (cmd->type) {
///     case NK_COMMAND_LINE:
///         your_draw_line_function(...)
///         break;
///     case NK_COMMAND_RECT
///         your_draw_rect_function(...)
///         break;
///     case ...:
///         // [...]
///     }
///     nk_clear(&ctx);
/// }
/// nk_free(&ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/// #### Reference
/// Function                        | Description
/// --------------------------------|-------------------------------------------
/// nk_group_begin                  | Start a new group with internal scrollbar handling
/// nk_group_begin_titled           | Start a new group with separated name and title and internal scrollbar handling
/// nk_group_end                    | Ends a group. Should only be called if nk_group_begin returned non-zero
/// nk_group_scrolled_offset_begin  | Start a new group with manual separated handling of scrollbar x- and y-offset
/// nk_group_scrolled_begin         | Start a new group with manual scrollbar handling
/// nk_group_scrolled_end           | Ends a group with manual scrollbar handling. Should only be called if nk_group_begin returned non-zero
/// nk_group_get_scroll             | Gets the scroll offset for the given group
/// nk_group_set_scroll             | Sets the scroll offset for the given group
*/
/*/// #### nk_group_begin
/// Starts a new widget group. Requires a previous layouting function to specify a pos/size.
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_bool nk_group_begin(struct nk_context*, const char *title, nk_flags);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __title__   | Must be an unique identifier for this group that is also used for the group header
/// __flags__   | Window flags defined in the nk_panel_flags section with a number of different group behaviors
///
/// Returns `true(1)` if visible and fillable with widgets or `false(0)` otherwise
*/
NK_API nk_bool nk_group_begin(struct nk_context*, const char *title, nk_flags);
/*/// #### nk_group_begin_titled
/// Starts a new widget group. Requires a previous layouting function to specify a pos/size.
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_bool nk_group_begin_titled(struct nk_context*, const char *name, const char *title, nk_flags);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __id__      | Must be an unique identifier for this group
/// __title__   | Group header title
/// __flags__   | Window flags defined in the nk_panel_flags section with a number of different group behaviors
///
/// Returns `true(1)` if visible and fillable with widgets or `false(0)` otherwise
*/
NK_API nk_bool nk_group_begin_titled(struct nk_context*, const char *name, const char *title, nk_flags);
/*/// #### nk_group_end
/// Ends a widget group
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_group_end(struct nk_context*);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
*/
NK_API void nk_group_end(struct nk_context*);
/*/// #### nk_group_scrolled_offset_begin
/// starts a new widget group. requires a previous layouting function to specify
/// a size. Does not keep track of scrollbar.
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_bool nk_group_scrolled_offset_begin(struct nk_context*, nk_uint *x_offset, nk_uint *y_offset, const char *title, nk_flags flags);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __x_offset__| Scrollbar x-offset to offset all widgets inside the group horizontally.
/// __y_offset__| Scrollbar y-offset to offset all widgets inside the group vertically
/// __title__   | Window unique group title used to both identify and display in the group header
/// __flags__   | Window flags from the nk_panel_flags section
///
/// Returns `true(1)` if visible and fillable with widgets or `false(0)` otherwise
*/
NK_API nk_bool nk_group_scrolled_offset_begin(struct nk_context*, nk_uint *x_offset, nk_uint *y_offset, const char *title, nk_flags flags);
/*/// #### nk_group_scrolled_begin
/// Starts a new widget group. requires a previous
/// layouting function to specify a size. Does not keep track of scrollbar.
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_bool nk_group_scrolled_begin(struct nk_context*, struct nk_scroll *off, const char *title, nk_flags);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __off__     | Both x- and y- scroll offset. Allows for manual scrollbar control
/// __title__   | Window unique group title used to both identify and display in the group header
/// __flags__   | Window flags from nk_panel_flags section
///
/// Returns `true(1)` if visible and fillable with widgets or `false(0)` otherwise
*/
NK_API nk_bool nk_group_scrolled_begin(struct nk_context*, struct nk_scroll *off, const char *title, nk_flags);
/*/// #### nk_group_scrolled_end
/// Ends a widget group after calling nk_group_scrolled_offset_begin or nk_group_scrolled_begin.
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_group_scrolled_end(struct nk_context*);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
*/
NK_API void nk_group_scrolled_end(struct nk_context*);
/*/// #### nk_group_get_scroll
/// Gets the scroll position of the given group.
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_group_get_scroll(struct nk_context*, const char *id, nk_uint *x_offset, nk_uint *y_offset);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter    | Description
/// -------------|-----------------------------------------------------------
/// __ctx__      | Must point to an previously initialized `nk_context` struct
/// __id__       | The id of the group to get the scroll position of
/// __x_offset__ | A pointer to the x offset output (or NULL to ignore)
/// __y_offset__ | A pointer to the y offset output (or NULL to ignore)
*/
NK_API void nk_group_get_scroll(struct nk_context*, const char *id, nk_uint *x_offset, nk_uint *y_offset);
/*/// #### nk_group_set_scroll
/// Sets the scroll position of the given group.
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_group_set_scroll(struct nk_context*, const char *id, nk_uint x_offset, nk_uint y_offset);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter    | Description
/// -------------|-----------------------------------------------------------
/// __ctx__      | Must point to an previously initialized `nk_context` struct
/// __id__       | The id of the group to scroll
/// __x_offset__ | The x offset to scroll to
/// __y_offset__ | The y offset to scroll to
*/
NK_API void nk_group_set_scroll(struct nk_context*, const char *id, nk_uint x_offset, nk_uint y_offset);
/* =============================================================================
 *
 *                                  TREE
 *
 * =============================================================================
/// ### Tree
/// Trees represent two different concept. First the concept of a collapsible
/// UI section that can be either in a hidden or visible state. They allow the UI
/// user to selectively minimize the current set of visible UI to comprehend.
/// The second concept are tree widgets for visual UI representation of trees.<br /><br />
///
/// Trees thereby can be nested for tree representations and multiple nested
/// collapsible UI sections. All trees are started by calling of the
/// `nk_tree_xxx_push_tree` functions and ended by calling one of the
/// `nk_tree_xxx_pop_xxx()` functions. Each starting functions takes a title label
/// and optionally an image to be displayed and the initial collapse state from
/// the nk_collapse_states section.<br /><br />
///
/// The runtime state of the tree is either stored outside the library by the caller
/// or inside which requires a unique ID. The unique ID can either be generated
/// automatically from `__FILE__` and `__LINE__` with function `nk_tree_push`,
/// by `__FILE__` and a user provided ID generated for example by loop index with
/// function `nk_tree_push_id` or completely provided from outside by user with
/// function `nk_tree_push_hashed`.
///
/// #### Usage
/// To create a tree you have to call one of the seven `nk_tree_xxx_push_xxx`
/// functions to start a collapsible UI section and `nk_tree_xxx_pop` to mark the
/// end.
/// Each starting function will either return `false(0)` if the tree is collapsed
/// or hidden and therefore does not need to be filled with content or `true(1)`
/// if visible and required to be filled.
///
/// !!! Note
///     The tree header does not require and layouting function and instead
///     calculates a auto height based on the currently used font size
///
/// The tree ending functions only need to be called if the tree content is
/// actually visible. So make sure the tree push function is guarded by `if`
/// and the pop call is only taken if the tree is visible.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// if (nk_tree_push(ctx, NK_TREE_TAB, "Tree", NK_MINIMIZED)) {
///     nk_layout_row_dynamic(...);
///     nk_widget(...);
///     nk_tree_pop(ctx);
/// }
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// #### Reference
/// Function                    | Description
/// ----------------------------|-------------------------------------------
/// nk_tree_push                | Start a collapsible UI section with internal state management
/// nk_tree_push_id             | Start a collapsible UI section with internal state management callable in a look
/// nk_tree_push_hashed         | Start a collapsible UI section with internal state management with full control over internal unique ID use to store state
/// nk_tree_image_push          | Start a collapsible UI section with image and label header
/// nk_tree_image_push_id       | Start a collapsible UI section with image and label header and internal state management callable in a look
/// nk_tree_image_push_hashed   | Start a collapsible UI section with image and label header and internal state management with full control over internal unique ID use to store state
/// nk_tree_pop                 | Ends a collapsible UI section
//
/// nk_tree_state_push          | Start a collapsible UI section with external state management
/// nk_tree_state_image_push    | Start a collapsible UI section with image and label header and external state management
/// nk_tree_state_pop           | Ends a collapsabale UI section
///
/// #### nk_tree_type
/// Flag            | Description
/// ----------------|----------------------------------------
/// NK_TREE_NODE    | Highlighted tree header to mark a collapsible UI section
/// NK_TREE_TAB     | Non-highlighted tree header closer to tree representations
*/
/*/// #### nk_tree_push
/// Starts a collapsible UI section with internal state management
/// !!! WARNING
///     To keep track of the runtime tree collapsible state this function uses
///     defines `__FILE__` and `__LINE__` to generate a unique ID. If you want
///     to call this function in a loop please use `nk_tree_push_id` or
///     `nk_tree_push_hashed` instead.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// #define nk_tree_push(ctx, type, title, state)
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __type__    | Value from the nk_tree_type section to visually mark a tree node header as either a collapseable UI section or tree node
/// __title__   | Label printed in the tree header
/// __state__   | Initial tree state value out of nk_collapse_states
///
/// Returns `true(1)` if visible and fillable with widgets or `false(0)` otherwise
*/
#define nk_tree_push(ctx, type, title, state) nk_tree_push_hashed(ctx, type, title, state, NK_FILE_LINE,nk_strlen(NK_FILE_LINE),__LINE__)
/*/// #### nk_tree_push_id
/// Starts a collapsible UI section with internal state management callable in a look
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// #define nk_tree_push_id(ctx, type, title, state, id)
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __type__    | Value from the nk_tree_type section to visually mark a tree node header as either a collapseable UI section or tree node
/// __title__   | Label printed in the tree header
/// __state__   | Initial tree state value out of nk_collapse_states
/// __id__      | Loop counter index if this function is called in a loop
///
/// Returns `true(1)` if visible and fillable with widgets or `false(0)` otherwise
*/
#define nk_tree_push_id(ctx, type, title, state, id) nk_tree_push_hashed(ctx, type, title, state, NK_FILE_LINE,nk_strlen(NK_FILE_LINE),id)
/*/// #### nk_tree_push_hashed
/// Start a collapsible UI section with internal state management with full
/// control over internal unique ID used to store state
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_bool nk_tree_push_hashed(struct nk_context*, enum nk_tree_type, const char *title, enum nk_collapse_states initial_state, const char *hash, int len,int seed);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __type__    | Value from the nk_tree_type section to visually mark a tree node header as either a collapseable UI section or tree node
/// __title__   | Label printed in the tree header
/// __state__   | Initial tree state value out of nk_collapse_states
/// __hash__    | Memory block or string to generate the ID from
/// __len__     | Size of passed memory block or string in __hash__
/// __seed__    | Seeding value if this function is called in a loop or default to `0`
///
/// Returns `true(1)` if visible and fillable with widgets or `false(0)` otherwise
*/
NK_API nk_bool nk_tree_push_hashed(struct nk_context*, enum nk_tree_type, const char *title, enum nk_collapse_states initial_state, const char *hash, int len,int seed);
/*/// #### nk_tree_image_push
/// Start a collapsible UI section with image and label header
/// !!! WARNING
///     To keep track of the runtime tree collapsible state this function uses
///     defines `__FILE__` and `__LINE__` to generate a unique ID. If you want
///     to call this function in a loop please use `nk_tree_image_push_id` or
///     `nk_tree_image_push_hashed` instead.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// #define nk_tree_image_push(ctx, type, img, title, state)
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __type__    | Value from the nk_tree_type section to visually mark a tree node header as either a collapseable UI section or tree node
/// __img__     | Image to display inside the header on the left of the label
/// __title__   | Label printed in the tree header
/// __state__   | Initial tree state value out of nk_collapse_states
///
/// Returns `true(1)` if visible and fillable with widgets or `false(0)` otherwise
*/
#define nk_tree_image_push(ctx, type, img, title, state) nk_tree_image_push_hashed(ctx, type, img, title, state, NK_FILE_LINE,nk_strlen(NK_FILE_LINE),__LINE__)
/*/// #### nk_tree_image_push_id
/// Start a collapsible UI section with image and label header and internal state
/// management callable in a look
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// #define nk_tree_image_push_id(ctx, type, img, title, state, id)
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __type__    | Value from the nk_tree_type section to visually mark a tree node header as either a collapseable UI section or tree node
/// __img__     | Image to display inside the header on the left of the label
/// __title__   | Label printed in the tree header
/// __state__   | Initial tree state value out of nk_collapse_states
/// __id__      | Loop counter index if this function is called in a loop
///
/// Returns `true(1)` if visible and fillable with widgets or `false(0)` otherwise
*/
#define nk_tree_image_push_id(ctx, type, img, title, state, id) nk_tree_image_push_hashed(ctx, type, img, title, state, NK_FILE_LINE,nk_strlen(NK_FILE_LINE),id)
/*/// #### nk_tree_image_push_hashed
/// Start a collapsible UI section with internal state management with full
/// control over internal unique ID used to store state
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_bool nk_tree_image_push_hashed(struct nk_context*, enum nk_tree_type, struct nk_image, const char *title, enum nk_collapse_states initial_state, const char *hash, int len,int seed);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct
/// __type__    | Value from the nk_tree_type section to visually mark a tree node header as either a collapseable UI section or tree node
/// __img__     | Image to display inside the header on the left of the label
/// __title__   | Label printed in the tree header
/// __state__   | Initial tree state value out of nk_collapse_states
/// __hash__    | Memory block or string to generate the ID from
/// __len__     | Size of passed memory block or string in __hash__
/// __seed__    | Seeding value if this function is called in a loop or default to `0`
///
/// Returns `true(1)` if visible and fillable with widgets or `false(0)` otherwise
*/
NK_API nk_bool nk_tree_image_push_hashed(struct nk_context*, enum nk_tree_type, struct nk_image, const char *title, enum nk_collapse_states initial_state, const char *hash, int len,int seed);
/*/// #### nk_tree_pop
/// Ends a collapsabale UI section
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_tree_pop(struct nk_context*);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after calling `nk_tree_xxx_push_xxx`
*/
NK_API void nk_tree_pop(struct nk_context*);
/*/// #### nk_tree_state_push
/// Start a collapsible UI section with external state management
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_bool nk_tree_state_push(struct nk_context*, enum nk_tree_type, const char *title, enum nk_collapse_states *state);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after calling `nk_tree_xxx_push_xxx`
/// __type__    | Value from the nk_tree_type section to visually mark a tree node header as either a collapseable UI section or tree node
/// __title__   | Label printed in the tree header
/// __state__   | Persistent state to update
///
/// Returns `true(1)` if visible and fillable with widgets or `false(0)` otherwise
*/
NK_API nk_bool nk_tree_state_push(struct nk_context*, enum nk_tree_type, const char *title, enum nk_collapse_states *state);
/*/// #### nk_tree_state_image_push
/// Start a collapsible UI section with image and label header and external state management
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// nk_bool nk_tree_state_image_push(struct nk_context*, enum nk_tree_type, struct nk_image, const char *title, enum nk_collapse_states *state);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after calling `nk_tree_xxx_push_xxx`
/// __img__     | Image to display inside the header on the left of the label
/// __type__    | Value from the nk_tree_type section to visually mark a tree node header as either a collapseable UI section or tree node
/// __title__   | Label printed in the tree header
/// __state__   | Persistent state to update
///
/// Returns `true(1)` if visible and fillable with widgets or `false(0)` otherwise
*/
NK_API nk_bool nk_tree_state_image_push(struct nk_context*, enum nk_tree_type, struct nk_image, const char *title, enum nk_collapse_states *state);
/*/// #### nk_tree_state_pop
/// Ends a collapsabale UI section
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_tree_state_pop(struct nk_context*);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter   | Description
/// ------------|-----------------------------------------------------------
/// __ctx__     | Must point to an previously initialized `nk_context` struct after calling `nk_tree_xxx_push_xxx`
*/
NK_API void nk_tree_state_pop(struct nk_context*);

#define nk_tree_element_push(ctx, type, title, state, sel) nk_tree_element_push_hashed(ctx, type, title, state, sel, NK_FILE_LINE,nk_strlen(NK_FILE_LINE),__LINE__)
#define nk_tree_element_push_id(ctx, type, title, state, sel, id) nk_tree_element_push_hashed(ctx, type, title, state, sel, NK_FILE_LINE,nk_strlen(NK_FILE_LINE),id)
NK_API nk_bool nk_tree_element_push_hashed(struct nk_context*, enum nk_tree_type, const char *title, enum nk_collapse_states initial_state, nk_bool *selected, const char *hash, int len, int seed);
NK_API nk_bool nk_tree_element_image_push_hashed(struct nk_context*, enum nk_tree_type, struct nk_image, const char *title, enum nk_collapse_states initial_state, nk_bool *selected, const char *hash, int len,int seed);
NK_API void nk_tree_element_pop(struct nk_context*);

/* =============================================================================
 *
 *                                  LIST VIEW
 *
 * ============================================================================= */
struct nk_list_view {
/* public: */
    int begin, end, count;
/* private: */
    int total_height;
    struct nk_context *ctx;
    nk_uint *scroll_pointer;
    nk_uint scroll_value;
};
NK_API nk_bool nk_list_view_begin(struct nk_context*, struct nk_list_view *out, const char *id, nk_flags, int row_height, int row_count);
NK_API void nk_list_view_end(struct nk_list_view*);
/* =============================================================================
 *
 *                                  WIDGET
 *
 * ============================================================================= */
enum nk_widget_layout_states {
    NK_WIDGET_INVALID, /* The widget cannot be seen and is completely out of view */
    NK_WIDGET_VALID, /* The widget is completely inside the window and can be updated and drawn */
    NK_WIDGET_ROM, /* The widget is partially visible and cannot be updated */
    NK_WIDGET_DISABLED /* The widget is manually disabled and acts like NK_WIDGET_ROM */
};
enum nk_widget_states {
    NK_WIDGET_STATE_MODIFIED    = NK_FLAG(1),
    NK_WIDGET_STATE_INACTIVE    = NK_FLAG(2), /* widget is neither active nor hovered */
    NK_WIDGET_STATE_ENTERED     = NK_FLAG(3), /* widget has been hovered on the current frame */
    NK_WIDGET_STATE_HOVER       = NK_FLAG(4), /* widget is being hovered */
    NK_WIDGET_STATE_ACTIVED     = NK_FLAG(5),/* widget is currently activated */
    NK_WIDGET_STATE_LEFT        = NK_FLAG(6), /* widget is from this frame on not hovered anymore */
    NK_WIDGET_STATE_HOVERED     = NK_WIDGET_STATE_HOVER|NK_WIDGET_STATE_MODIFIED, /* widget is being hovered */
    NK_WIDGET_STATE_ACTIVE      = NK_WIDGET_STATE_ACTIVED|NK_WIDGET_STATE_MODIFIED /* widget is currently activated */
};
NK_API enum nk_widget_layout_states nk_widget(struct nk_rect*, const struct nk_context*);
NK_API enum nk_widget_layout_states nk_widget_fitting(struct nk_rect*, struct nk_context*, struct nk_vec2);
NK_API struct nk_rect nk_widget_bounds(struct nk_context*);
NK_API struct nk_vec2 nk_widget_position(struct nk_context*);
NK_API struct nk_vec2 nk_widget_size(struct nk_context*);
NK_API float nk_widget_width(struct nk_context*);
NK_API float nk_widget_height(struct nk_context*);
NK_API nk_bool nk_widget_is_hovered(struct nk_context*);
NK_API nk_bool nk_widget_is_mouse_clicked(struct nk_context*, enum nk_buttons);
NK_API nk_bool nk_widget_has_mouse_click_down(struct nk_context*, enum nk_buttons, nk_bool down);
NK_API void nk_spacing(struct nk_context*, int cols);
NK_API void nk_widget_disable_begin(struct nk_context* ctx);
NK_API void nk_widget_disable_end(struct nk_context* ctx);
/* =============================================================================
 *
 *                                  TEXT
 *
 * ============================================================================= */
enum nk_text_align {
    NK_TEXT_ALIGN_LEFT        = 0x01,
    NK_TEXT_ALIGN_CENTERED    = 0x02,
    NK_TEXT_ALIGN_RIGHT       = 0x04,
    NK_TEXT_ALIGN_TOP         = 0x08,
    NK_TEXT_ALIGN_MIDDLE      = 0x10,
    NK_TEXT_ALIGN_BOTTOM      = 0x20
};
enum nk_text_alignment {
    NK_TEXT_LEFT        = NK_TEXT_ALIGN_MIDDLE|NK_TEXT_ALIGN_LEFT,
    NK_TEXT_CENTERED    = NK_TEXT_ALIGN_MIDDLE|NK_TEXT_ALIGN_CENTERED,
    NK_TEXT_RIGHT       = NK_TEXT_ALIGN_MIDDLE|NK_TEXT_ALIGN_RIGHT
};
NK_API void nk_text(struct nk_context*, const char*, int, nk_flags);
NK_API void nk_text_colored(struct nk_context*, const char*, int, nk_flags, struct nk_color);
NK_API void nk_text_wrap(struct nk_context*, const char*, int);
NK_API void nk_text_wrap_colored(struct nk_context*, const char*, int, struct nk_color);
NK_API void nk_label(struct nk_context*, const char*, nk_flags align);
NK_API void nk_label_colored(struct nk_context*, const char*, nk_flags align, struct nk_color);
NK_API void nk_label_wrap(struct nk_context*, const char*);
NK_API void nk_label_colored_wrap(struct nk_context*, const char*, struct nk_color);
NK_API void nk_image(struct nk_context*, struct nk_image);
NK_API void nk_image_color(struct nk_context*, struct nk_image, struct nk_color);
#ifdef NK_INCLUDE_STANDARD_VARARGS
NK_API void nk_labelf(struct nk_context*, nk_flags, NK_PRINTF_FORMAT_STRING const char*, ...) NK_PRINTF_VARARG_FUNC(3);
NK_API void nk_labelf_colored(struct nk_context*, nk_flags, struct nk_color, NK_PRINTF_FORMAT_STRING const char*,...) NK_PRINTF_VARARG_FUNC(4);
NK_API void nk_labelf_wrap(struct nk_context*, NK_PRINTF_FORMAT_STRING const char*,...) NK_PRINTF_VARARG_FUNC(2);
NK_API void nk_labelf_colored_wrap(struct nk_context*, struct nk_color, NK_PRINTF_FORMAT_STRING const char*,...) NK_PRINTF_VARARG_FUNC(3);
NK_API void nk_labelfv(struct nk_context*, nk_flags, NK_PRINTF_FORMAT_STRING const char*, va_list) NK_PRINTF_VALIST_FUNC(3);
NK_API void nk_labelfv_colored(struct nk_context*, nk_flags, struct nk_color, NK_PRINTF_FORMAT_STRING const char*, va_list) NK_PRINTF_VALIST_FUNC(4);
NK_API void nk_labelfv_wrap(struct nk_context*, NK_PRINTF_FORMAT_STRING const char*, va_list) NK_PRINTF_VALIST_FUNC(2);
NK_API void nk_labelfv_colored_wrap(struct nk_context*, struct nk_color, NK_PRINTF_FORMAT_STRING const char*, va_list) NK_PRINTF_VALIST_FUNC(3);
NK_API void nk_value_bool(struct nk_context*, const char *prefix, int);
NK_API void nk_value_int(struct nk_context*, const char *prefix, int);
NK_API void nk_value_uint(struct nk_context*, const char *prefix, unsigned int);
NK_API void nk_value_float(struct nk_context*, const char *prefix, float);
NK_API void nk_value_color_byte(struct nk_context*, const char *prefix, struct nk_color);
NK_API void nk_value_color_float(struct nk_context*, const char *prefix, struct nk_color);
NK_API void nk_value_color_hex(struct nk_context*, const char *prefix, struct nk_color);
#endif
/* =============================================================================
 *
 *                                  BUTTON
 *
 * ============================================================================= */
NK_API nk_bool nk_button_text(struct nk_context*, const char *title, int len);
NK_API nk_bool nk_button_label(struct nk_context*, const char *title);
NK_API nk_bool nk_button_color(struct nk_context*, struct nk_color);
NK_API nk_bool nk_button_symbol(struct nk_context*, enum nk_symbol_type);
NK_API nk_bool nk_button_image(struct nk_context*, struct nk_image img);
NK_API nk_bool nk_button_symbol_label(struct nk_context*, enum nk_symbol_type, const char*, nk_flags text_alignment);
NK_API nk_bool nk_button_symbol_text(struct nk_context*, enum nk_symbol_type, const char*, int, nk_flags alignment);
NK_API nk_bool nk_button_image_label(struct nk_context*, struct nk_image img, const char*, nk_flags text_alignment);
NK_API nk_bool nk_button_image_text(struct nk_context*, struct nk_image img, const char*, int, nk_flags alignment);
NK_API nk_bool nk_button_text_styled(struct nk_context*, const struct nk_style_button*, const char *title, int len);
NK_API nk_bool nk_button_label_styled(struct nk_context*, const struct nk_style_button*, const char *title);
NK_API nk_bool nk_button_symbol_styled(struct nk_context*, const struct nk_style_button*, enum nk_symbol_type);
NK_API nk_bool nk_button_image_styled(struct nk_context*, const struct nk_style_button*, struct nk_image img);
NK_API nk_bool nk_button_symbol_text_styled(struct nk_context*,const struct nk_style_button*, enum nk_symbol_type, const char*, int, nk_flags alignment);
NK_API nk_bool nk_button_symbol_label_styled(struct nk_context *ctx, const struct nk_style_button *style, enum nk_symbol_type symbol, const char *title, nk_flags align);
NK_API nk_bool nk_button_image_label_styled(struct nk_context*,const struct nk_style_button*, struct nk_image img, const char*, nk_flags text_alignment);
NK_API nk_bool nk_button_image_text_styled(struct nk_context*,const struct nk_style_button*, struct nk_image img, const char*, int, nk_flags alignment);
NK_API void nk_button_set_behavior(struct nk_context*, enum nk_button_behavior);
NK_API nk_bool nk_button_push_behavior(struct nk_context*, enum nk_button_behavior);
NK_API nk_bool nk_button_pop_behavior(struct nk_context*);
/* =============================================================================
 *
 *                                  CHECKBOX
 *
 * ============================================================================= */
NK_API nk_bool nk_check_label(struct nk_context*, const char*, nk_bool active);
NK_API nk_bool nk_check_text(struct nk_context*, const char*, int, nk_bool active);
NK_API nk_bool nk_check_text_align(struct nk_context*, const char*, int, nk_bool active, nk_flags widget_alignment, nk_flags text_alignment);
NK_API unsigned nk_check_flags_label(struct nk_context*, const char*, unsigned int flags, unsigned int value);
NK_API unsigned nk_check_flags_text(struct nk_context*, const char*, int, unsigned int flags, unsigned int value);
NK_API nk_bool nk_checkbox_label(struct nk_context*, const char*, nk_bool *active);
NK_API nk_bool nk_checkbox_label_align(struct nk_context *ctx, const char *label, nk_bool *active, nk_flags widget_alignment, nk_flags text_alignment);
NK_API nk_bool nk_checkbox_text(struct nk_context*, const char*, int, nk_bool *active);
NK_API nk_bool nk_checkbox_text_align(struct nk_context *ctx, const char *text, int len, nk_bool *active, nk_flags widget_alignment, nk_flags text_alignment);
NK_API nk_bool nk_checkbox_flags_label(struct nk_context*, const char*, unsigned int *flags, unsigned int value);
NK_API nk_bool nk_checkbox_flags_text(struct nk_context*, const char*, int, unsigned int *flags, unsigned int value);
/* =============================================================================
 *
 *                                  RADIO BUTTON
 *
 * ============================================================================= */
NK_API nk_bool nk_radio_label(struct nk_context*, const char*, nk_bool *active);
NK_API nk_bool nk_radio_label_align(struct nk_context *ctx, const char *label, nk_bool *active, nk_flags widget_alignment, nk_flags text_alignment);
NK_API nk_bool nk_radio_text(struct nk_context*, const char*, int, nk_bool *active);
NK_API nk_bool nk_radio_text_align(struct nk_context *ctx, const char *text, int len, nk_bool *active, nk_flags widget_alignment, nk_flags text_alignment);
NK_API nk_bool nk_option_label(struct nk_context*, const char*, nk_bool active);
NK_API nk_bool nk_option_label_align(struct nk_context *ctx, const char *label, nk_bool active, nk_flags widget_alignment, nk_flags text_alignment);
NK_API nk_bool nk_option_text(struct nk_context*, const char*, int, nk_bool active);
NK_API nk_bool nk_option_text_align(struct nk_context *ctx, const char *text, int len, nk_bool is_active, nk_flags widget_alignment, nk_flags text_alignment);
/* =============================================================================
 *
 *                                  SELECTABLE
 *
 * ============================================================================= */
NK_API nk_bool nk_selectable_label(struct nk_context*, const char*, nk_flags align, nk_bool *value);
NK_API nk_bool nk_selectable_text(struct nk_context*, const char*, int, nk_flags align, nk_bool *value);
NK_API nk_bool nk_selectable_image_label(struct nk_context*,struct nk_image,  const char*, nk_flags align, nk_bool *value);
NK_API nk_bool nk_selectable_image_text(struct nk_context*,struct nk_image, const char*, int, nk_flags align, nk_bool *value);
NK_API nk_bool nk_selectable_symbol_label(struct nk_context*,enum nk_symbol_type,  const char*, nk_flags align, nk_bool *value);
NK_API nk_bool nk_selectable_symbol_text(struct nk_context*,enum nk_symbol_type, const char*, int, nk_flags align, nk_bool *value);

NK_API nk_bool nk_select_label(struct nk_context*, const char*, nk_flags align, nk_bool value);
NK_API nk_bool nk_select_text(struct nk_context*, const char*, int, nk_flags align, nk_bool value);
NK_API nk_bool nk_select_image_label(struct nk_context*, struct nk_image,const char*, nk_flags align, nk_bool value);
NK_API nk_bool nk_select_image_text(struct nk_context*, struct nk_image,const char*, int, nk_flags align, nk_bool value);
NK_API nk_bool nk_select_symbol_label(struct nk_context*,enum nk_symbol_type,  const char*, nk_flags align, nk_bool value);
NK_API nk_bool nk_select_symbol_text(struct nk_context*,enum nk_symbol_type, const char*, int, nk_flags align, nk_bool value);

/* =============================================================================
 *
 *                                  SLIDER
 *
 * ============================================================================= */
NK_API float nk_slide_float(struct nk_context*, float min, float val, float max, float step);
NK_API int nk_slide_int(struct nk_context*, int min, int val, int max, int step);
NK_API nk_bool nk_slider_float(struct nk_context*, float min, float *val, float max, float step);
NK_API nk_bool nk_slider_int(struct nk_context*, int min, int *val, int max, int step);

/* =============================================================================
 *
 *                                   KNOB
 *
 * ============================================================================= */
NK_API nk_bool nk_knob_float(struct nk_context*, float min, float *val, float max, float step, enum nk_heading zero_direction, float dead_zone_degrees);
NK_API nk_bool nk_knob_int(struct nk_context*, int min, int *val, int max, int step, enum nk_heading zero_direction, float dead_zone_degrees);

/* =============================================================================
 *
 *                                  PROGRESSBAR
 *
 * ============================================================================= */
NK_API nk_bool nk_progress(struct nk_context*, nk_size *cur, nk_size max, nk_bool modifyable);
NK_API nk_size nk_prog(struct nk_context*, nk_size cur, nk_size max, nk_bool modifyable);

/* =============================================================================
 *
 *                                  COLOR PICKER
 *
 * ============================================================================= */
NK_API struct nk_colorf nk_color_picker(struct nk_context*, struct nk_colorf, enum nk_color_format);
NK_API nk_bool nk_color_pick(struct nk_context*, struct nk_colorf*, enum nk_color_format);
/* =============================================================================
 *
 *                                  PROPERTIES
 *
 * =============================================================================
/// ### Properties
/// Properties are the main value modification widgets in Nuklear. Changing a value
/// can be achieved by dragging, adding/removing incremental steps on button click
/// or by directly typing a number.
///
/// #### Usage
/// Each property requires a unique name for identification that is also used for
/// displaying a label. If you want to use the same name multiple times make sure
/// add a '#' before your name. The '#' will not be shown but will generate a
/// unique ID. Each property also takes in a minimum and maximum value. If you want
/// to make use of the complete number range of a type just use the provided
/// type limits from `limits.h`. For example `INT_MIN` and `INT_MAX` for
/// `nk_property_int` and `nk_propertyi`. In additional each property takes in
/// a increment value that will be added or subtracted if either the increment
/// decrement button is clicked. Finally there is a value for increment per pixel
/// dragged that is added or subtracted from the value.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// int value = 0;
/// struct nk_context ctx;
/// nk_init_xxx(&ctx, ...);
/// while (1) {
///     // Input
///     Event evt;
///     nk_input_begin(&ctx);
///     while (GetEvent(&evt)) {
///         if (evt.type == MOUSE_MOVE)
///             nk_input_motion(&ctx, evt.motion.x, evt.motion.y);
///         else if (evt.type == [...]) {
///             nk_input_xxx(...);
///         }
///     }
///     nk_input_end(&ctx);
///     //
///     // Window
///     if (nk_begin_xxx(...) {
///         // Property
///         nk_layout_row_dynamic(...);
///         nk_property_int(ctx, "ID", INT_MIN, &value, INT_MAX, 1, 1);
///     }
///     nk_end(ctx);
///     //
///     // Draw
///     const struct nk_command *cmd = 0;
///     nk_foreach(cmd, &ctx) {
///     switch (cmd->type) {
///     case NK_COMMAND_LINE:
///         your_draw_line_function(...)
///         break;
///     case NK_COMMAND_RECT
///         your_draw_rect_function(...)
///         break;
///     case ...:
///         // [...]
///     }
///     nk_clear(&ctx);
/// }
/// nk_free(&ctx);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// #### Reference
/// Function            | Description
/// --------------------|-------------------------------------------
/// nk_property_int     | Integer property directly modifying a passed in value
/// nk_property_float   | Float property directly modifying a passed in value
/// nk_property_double  | Double property directly modifying a passed in value
/// nk_propertyi        | Integer property returning the modified int value
/// nk_propertyf        | Float property returning the modified float value
/// nk_propertyd        | Double property returning the modified double value
///
*/
/*/// #### nk_property_int
/// Integer property directly modifying a passed in value
/// !!! WARNING
///     To generate a unique property ID using the same label make sure to insert
///     a `#` at the beginning. It will not be shown but guarantees correct behavior.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_property_int(struct nk_context *ctx, const char *name, int min, int *val, int max, int step, float inc_per_pixel);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter           | Description
/// --------------------|-----------------------------------------------------------
/// __ctx__             | Must point to an previously initialized `nk_context` struct after calling a layouting function
/// __name__            | String used both as a label as well as a unique identifier
/// __min__             | Minimum value not allowed to be underflown
/// __val__             | Integer pointer to be modified
/// __max__             | Maximum value not allowed to be overflown
/// __step__            | Increment added and subtracted on increment and decrement button
/// __inc_per_pixel__   | Value per pixel added or subtracted on dragging
*/
NK_API void nk_property_int(struct nk_context*, const char *name, int min, int *val, int max, int step, float inc_per_pixel);
/*/// #### nk_property_float
/// Float property directly modifying a passed in value
/// !!! WARNING
///     To generate a unique property ID using the same label make sure to insert
///     a `#` at the beginning. It will not be shown but guarantees correct behavior.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_property_float(struct nk_context *ctx, const char *name, float min, float *val, float max, float step, float inc_per_pixel);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter           | Description
/// --------------------|-----------------------------------------------------------
/// __ctx__             | Must point to an previously initialized `nk_context` struct after calling a layouting function
/// __name__            | String used both as a label as well as a unique identifier
/// __min__             | Minimum value not allowed to be underflown
/// __val__             | Float pointer to be modified
/// __max__             | Maximum value not allowed to be overflown
/// __step__            | Increment added and subtracted on increment and decrement button
/// __inc_per_pixel__   | Value per pixel added or subtracted on dragging
*/
NK_API void nk_property_float(struct nk_context*, const char *name, float min, float *val, float max, float step, float inc_per_pixel);
/*/// #### nk_property_double
/// Double property directly modifying a passed in value
/// !!! WARNING
///     To generate a unique property ID using the same label make sure to insert
///     a `#` at the beginning. It will not be shown but guarantees correct behavior.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// void nk_property_double(struct nk_context *ctx, const char *name, double min, double *val, double max, double step, double inc_per_pixel);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter           | Description
/// --------------------|-----------------------------------------------------------
/// __ctx__             | Must point to an previously initialized `nk_context` struct after calling a layouting function
/// __name__            | String used both as a label as well as a unique identifier
/// __min__             | Minimum value not allowed to be underflown
/// __val__             | Double pointer to be modified
/// __max__             | Maximum value not allowed to be overflown
/// __step__            | Increment added and subtracted on increment and decrement button
/// __inc_per_pixel__   | Value per pixel added or subtracted on dragging
*/
NK_API void nk_property_double(struct nk_context*, const char *name, double min, double *val, double max, double step, float inc_per_pixel);
/*/// #### nk_propertyi
/// Integer property modifying a passed in value and returning the new value
/// !!! WARNING
///     To generate a unique property ID using the same label make sure to insert
///     a `#` at the beginning. It will not be shown but guarantees correct behavior.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// int nk_propertyi(struct nk_context *ctx, const char *name, int min, int val, int max, int step, float inc_per_pixel);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter           | Description
/// --------------------|-----------------------------------------------------------
/// __ctx__             | Must point to an previously initialized `nk_context` struct after calling a layouting function
/// __name__            | String used both as a label as well as a unique identifier
/// __min__             | Minimum value not allowed to be underflown
/// __val__             | Current integer value to be modified and returned
/// __max__             | Maximum value not allowed to be overflown
/// __step__            | Increment added and subtracted on increment and decrement button
/// __inc_per_pixel__   | Value per pixel added or subtracted on dragging
///
/// Returns the new modified integer value
*/
NK_API int nk_propertyi(struct nk_context*, const char *name, int min, int val, int max, int step, float inc_per_pixel);
/*/// #### nk_propertyf
/// Float property modifying a passed in value and returning the new value
/// !!! WARNING
///     To generate a unique property ID using the same label make sure to insert
///     a `#` at the beginning. It will not be shown but guarantees correct behavior.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// float nk_propertyf(struct nk_context *ctx, const char *name, float min, float val, float max, float step, float inc_per_pixel);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter           | Description
/// --------------------|-----------------------------------------------------------
/// __ctx__             | Must point to an previously initialized `nk_context` struct after calling a layouting function
/// __name__            | String used both as a label as well as a unique identifier
/// __min__             | Minimum value not allowed to be underflown
/// __val__             | Current float value to be modified and returned
/// __max__             | Maximum value not allowed to be overflown
/// __step__            | Increment added and subtracted on increment and decrement button
/// __inc_per_pixel__   | Value per pixel added or subtracted on dragging
///
/// Returns the new modified float value
*/
NK_API float nk_propertyf(struct nk_context*, const char *name, float min, float val, float max, float step, float inc_per_pixel);
/*/// #### nk_propertyd
/// Float property modifying a passed in value and returning the new value
/// !!! WARNING
///     To generate a unique property ID using the same label make sure to insert
///     a `#` at the beginning. It will not be shown but guarantees correct behavior.
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~c
/// float nk_propertyd(struct nk_context *ctx, const char *name, double min, double val, double max, double step, double inc_per_pixel);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Parameter           | Description
/// --------------------|-----------------------------------------------------------
/// __ctx__             | Must point to an previously initialized `nk_context` struct after calling a layouting function
/// __name__            | String used both as a label as well as a unique identifier
/// __min__             | Minimum value not allowed to be underflown
/// __val__             | Current double value to be modified and returned
/// __max__             | Maximum value not allowed to be overflown
/// __step__            | Increment added and subtracted on increment and decrement button
/// __inc_per_pixel__   | Value per pixel added or subtracted on dragging
///
/// Returns the new modified double value
*/
NK_API double nk_propertyd(struct nk_context*, const char *name, double min, double val, double max, double step, float inc_per_pixel);
NK_API void nk_property_focus(struct nk_context *ctx);
NK_API int nk_property_int_unfocus(struct nk_context *ctx, const char *name, int min, int *val, int max, int step, int keypress);
NK_API int nk_property_float_unfocus(struct nk_context *ctx, const char *name, float min, float *val, float max, float step, int keypress);
/* =============================================================================
 *
 *                                  TEXT EDIT
 *
 * ============================================================================= */
enum nk_edit_flags {
    NK_EDIT_DEFAULT                 = 0,
    NK_EDIT_READ_ONLY               = NK_FLAG(0),
    NK_EDIT_AUTO_SELECT             = NK_FLAG(1),
    NK_EDIT_SIG_ENTER               = NK_FLAG(2),
    NK_EDIT_ALLOW_TAB               = NK_FLAG(3),
    NK_EDIT_NO_CURSOR               = NK_FLAG(4),
    NK_EDIT_SELECTABLE              = NK_FLAG(5),
    NK_EDIT_CLIPBOARD               = NK_FLAG(6),
    NK_EDIT_CTRL_ENTER_NEWLINE      = NK_FLAG(7),
    NK_EDIT_NO_HORIZONTAL_SCROLL    = NK_FLAG(8),
    NK_EDIT_ALWAYS_INSERT_MODE      = NK_FLAG(9),
    NK_EDIT_MULTILINE               = NK_FLAG(10),
    NK_EDIT_GOTO_END_ON_ACTIVATE    = NK_FLAG(11)
};
enum nk_edit_types {
    NK_EDIT_SIMPLE  = NK_EDIT_ALWAYS_INSERT_MODE,
    NK_EDIT_FIELD   = NK_EDIT_SIMPLE|NK_EDIT_SELECTABLE|NK_EDIT_CLIPBOARD,
    NK_EDIT_BOX     = NK_EDIT_ALWAYS_INSERT_MODE| NK_EDIT_SELECTABLE| NK_EDIT_MULTILINE|NK_EDIT_ALLOW_TAB|NK_EDIT_CLIPBOARD,
    NK_EDIT_EDITOR  = NK_EDIT_SELECTABLE|NK_EDIT_MULTILINE|NK_EDIT_ALLOW_TAB| NK_EDIT_CLIPBOARD
};
enum nk_edit_events {
    NK_EDIT_ACTIVE      = NK_FLAG(0), /* edit widget is currently being modified */
    NK_EDIT_INACTIVE    = NK_FLAG(1), /* edit widget is not active and is not being modified */
    NK_EDIT_ACTIVATED   = NK_FLAG(2), /* edit widget went from state inactive to state active */
    NK_EDIT_DEACTIVATED = NK_FLAG(3), /* edit widget went from state active to state inactive */
    NK_EDIT_COMMITED    = NK_FLAG(4) /* edit widget has received an enter and lost focus */
};
NK_API nk_flags nk_edit_string(struct nk_context*, nk_flags, char *buffer, int *len, int max, nk_plugin_filter);
NK_API nk_flags nk_edit_string_zero_terminated(struct nk_context*, nk_flags, char *buffer, int max, nk_plugin_filter);
NK_API nk_flags nk_edit_buffer(struct nk_context*, nk_flags, struct nk_text_edit*, nk_plugin_filter);
NK_API void nk_edit_focus(struct nk_context*, nk_flags flags);
NK_API void nk_edit_unfocus(struct nk_context*);
/* =============================================================================
 *
 *                                  CHART
 *
 * ============================================================================= */
NK_API nk_bool nk_chart_begin(struct nk_context*, enum nk_chart_type, int num, float min, float max);
NK_API nk_bool nk_chart_begin_colored(struct nk_context*, enum nk_chart_type, struct nk_color, struct nk_color active, int num, float min, float max);
NK_API void nk_chart_add_slot(struct nk_context *ctx, const enum nk_chart_type, int count, float min_value, float max_value);
NK_API void nk_chart_add_slot_colored(struct nk_context *ctx, const enum nk_chart_type, struct nk_color, struct nk_color active, int count, float min_value, float max_value);
NK_API nk_flags nk_chart_push(struct nk_context*, float);
NK_API nk_flags nk_chart_push_slot(struct nk_context*, float, int);
NK_API void nk_chart_end(struct nk_context*);
NK_API void nk_plot(struct nk_context*, enum nk_chart_type, const float *values, int count, int offset);
NK_API void nk_plot_function(struct nk_context*, enum nk_chart_type, void *userdata, float(*value_getter)(void* user, int index), int count, int offset);
/* =============================================================================
 *
 *                                  POPUP
 *
 * ============================================================================= */
NK_API nk_bool nk_popup_begin(struct nk_context*, enum nk_popup_type, const char*, nk_flags, struct nk_rect bounds);
NK_API void nk_popup_close(struct nk_context*);
NK_API void nk_popup_end(struct nk_context*);
NK_API void nk_popup_get_scroll(struct nk_context*, nk_uint *offset_x, nk_uint *offset_y);
NK_API void nk_popup_set_scroll(struct nk_context*, nk_uint offset_x, nk_uint offset_y);
/* =============================================================================
 *
 *                                  COMBOBOX
 *
 * ============================================================================= */
NK_API int nk_combo(struct nk_context*, const char **items, int count, int selected, int item_height, struct nk_vec2 size);
NK_API int nk_combo_separator(struct nk_context*, const char *items_separated_by_separator, int separator, int selected, int count, int item_height, struct nk_vec2 size);
NK_API int nk_combo_string(struct nk_context*, const char *items_separated_by_zeros, int selected, int count, int item_height, struct nk_vec2 size);
NK_API int nk_combo_callback(struct nk_context*, void(*item_getter)(void*, int, const char**), void *userdata, int selected, int count, int item_height, struct nk_vec2 size);
NK_API void nk_combobox(struct nk_context*, const char **items, int count, int *selected, int item_height, struct nk_vec2 size);
NK_API void nk_combobox_string(struct nk_context*, const char *items_separated_by_zeros, int *selected, int count, int item_height, struct nk_vec2 size);
NK_API void nk_combobox_separator(struct nk_context*, const char *items_separated_by_separator, int separator, int *selected, int count, int item_height, struct nk_vec2 size);
NK_API void nk_combobox_callback(struct nk_context*, void(*item_getter)(void*, int, const char**), void*, int *selected, int count, int item_height, struct nk_vec2 size);
/* =============================================================================
 *
 *                                  ABSTRACT COMBOBOX
 *
 * ============================================================================= */
NK_API nk_bool nk_combo_begin_text(struct nk_context*, const char *selected, int, struct nk_vec2 size);
NK_API nk_bool nk_combo_begin_label(struct nk_context*, const char *selected, struct nk_vec2 size);
NK_API nk_bool nk_combo_begin_color(struct nk_context*, struct nk_color color, struct nk_vec2 size);
NK_API nk_bool nk_combo_begin_symbol(struct nk_context*,  enum nk_symbol_type,  struct nk_vec2 size);
NK_API nk_bool nk_combo_begin_symbol_label(struct nk_context*, const char *selected, enum nk_symbol_type, struct nk_vec2 size);
NK_API nk_bool nk_combo_begin_symbol_text(struct nk_context*, const char *selected, int, enum nk_symbol_type, struct nk_vec2 size);
NK_API nk_bool nk_combo_begin_image(struct nk_context*, struct nk_image img,  struct nk_vec2 size);
NK_API nk_bool nk_combo_begin_image_label(struct nk_context*, const char *selected, struct nk_image, struct nk_vec2 size);
NK_API nk_bool nk_combo_begin_image_text(struct nk_context*,  const char *selected, int, struct nk_image, struct nk_vec2 size);
NK_API nk_bool nk_combo_item_label(struct nk_context*, const char*, nk_flags alignment);
NK_API nk_bool nk_combo_item_text(struct nk_context*, const char*,int, nk_flags alignment);
NK_API nk_bool nk_combo_item_image_label(struct nk_context*, struct nk_image, const char*, nk_flags alignment);
NK_API nk_bool nk_combo_item_image_text(struct nk_context*, struct nk_image, const char*, int,nk_flags alignment);
NK_API nk_bool nk_combo_item_symbol_label(struct nk_context*, enum nk_symbol_type, const char*, nk_flags alignment);
NK_API nk_bool nk_combo_item_symbol_text(struct nk_context*, enum nk_symbol_type, const char*, int, nk_flags alignment);
NK_API void nk_combo_close(struct nk_context*);
NK_API void nk_combo_end(struct nk_context*);
/* =============================================================================
 *
 *                                  CONTEXTUAL
 *
 * ============================================================================= */
NK_API nk_bool nk_contextual_begin(struct nk_context*, nk_flags, struct nk_vec2, struct nk_rect trigger_bounds);
NK_API nk_bool nk_contextual_item_text(struct nk_context*, const char*, int,nk_flags align);
NK_API nk_bool nk_contextual_item_label(struct nk_context*, const char*, nk_flags align);
NK_API nk_bool nk_contextual_item_image_label(struct nk_context*, struct nk_image, const char*, nk_flags alignment);
NK_API nk_bool nk_contextual_item_image_text(struct nk_context*, struct nk_image, const char*, int len, nk_flags alignment);
NK_API nk_bool nk_contextual_item_symbol_label(struct nk_context*, enum nk_symbol_type, const char*, nk_flags alignment);
NK_API nk_bool nk_contextual_item_symbol_text(struct nk_context*, enum nk_symbol_type, const char*, int, nk_flags alignment);
NK_API void nk_contextual_close(struct nk_context*);
NK_API void nk_contextual_end(struct nk_context*);
/* =============================================================================
 *
 *                                  TOOLTIP
 *
 * ============================================================================= */
NK_API void nk_tooltip(struct nk_context*, const char*);
#ifdef NK_INCLUDE_STANDARD_VARARGS
NK_API void nk_tooltipf(struct nk_context*, NK_PRINTF_FORMAT_STRING const char*, ...) NK_PRINTF_VARARG_FUNC(2);
NK_API void nk_tooltipfv(struct nk_context*, NK_PRINTF_FORMAT_STRING const char*, va_list) NK_PRINTF_VALIST_FUNC(2);
#endif
NK_API nk_bool nk_tooltip_begin(struct nk_context*, float width);
NK_API void nk_tooltip_end(struct nk_context*);
/* =============================================================================
 *
 *                                  MENU
 *
 * ============================================================================= */
NK_API void nk_menubar_begin(struct nk_context*);
NK_API void nk_menubar_end(struct nk_context*);
NK_API nk_bool nk_menu_begin_text(struct nk_context*, const char* title, int title_len, nk_flags align, struct nk_vec2 size);
NK_API nk_bool nk_menu_begin_label(struct nk_context*, const char*, nk_flags align, struct nk_vec2 size);
NK_API nk_bool nk_menu_begin_image(struct nk_context*, const char*, struct nk_image, struct nk_vec2 size);
NK_API nk_bool nk_menu_begin_image_text(struct nk_context*, const char*, int,nk_flags align,struct nk_image, struct nk_vec2 size);
NK_API nk_bool nk_menu_begin_image_label(struct nk_context*, const char*, nk_flags align,struct nk_image, struct nk_vec2 size);
NK_API nk_bool nk_menu_begin_symbol(struct nk_context*, const char*, enum nk_symbol_type, struct nk_vec2 size);
NK_API nk_bool nk_menu_begin_symbol_text(struct nk_context*, const char*, int,nk_flags align,enum nk_symbol_type, struct nk_vec2 size);
NK_API nk_bool nk_menu_begin_symbol_label(struct nk_context*, const char*, nk_flags align,enum nk_symbol_type, struct nk_vec2 size);
NK_API nk_bool nk_menu_item_text(struct nk_context*, const char*, int,nk_flags align);
NK_API nk_bool nk_menu_item_label(struct nk_context*, const char*, nk_flags alignment);
NK_API nk_bool nk_menu_item_image_label(struct nk_context*, struct nk_image, const char*, nk_flags alignment);
NK_API nk_bool nk_menu_item_image_text(struct nk_context*, struct nk_image, const char*, int len, nk_flags alignment);
NK_API nk_bool nk_menu_item_symbol_text(struct nk_context*, enum nk_symbol_type, const char*, int, nk_flags alignment);
NK_API nk_bool nk_menu_item_symbol_label(struct nk_context*, enum nk_symbol_type, const char*, nk_flags alignment);
NK_API void nk_menu_close(struct nk_context*);
NK_API void nk_menu_end(struct nk_context*);
/* =============================================================================
 *
 *                                  STYLE
 *
 * ============================================================================= */

#define NK_WIDGET_DISABLED_FACTOR 0.5f

enum nk_style_colors {
    NK_COLOR_TEXT,
    NK_COLOR_WINDOW,
    NK_COLOR_HEADER,
    NK_COLOR_BORDER,
    NK_COLOR_BUTTON,
    NK_COLOR_BUTTON_HOVER,
    NK_COLOR_BUTTON_ACTIVE,
    NK_COLOR_TOGGLE,
    NK_COLOR_TOGGLE_HOVER,
    NK_COLOR_TOGGLE_CURSOR,
    NK_COLOR_SELECT,
    NK_COLOR_SELECT_ACTIVE,
    NK_COLOR_SLIDER,
    NK_COLOR_SLIDER_CURSOR,
    NK_COLOR_SLIDER_CURSOR_HOVER,
    NK_COLOR_SLIDER_CURSOR_ACTIVE,
    NK_COLOR_PROPERTY,
    NK_COLOR_EDIT,
    NK_COLOR_EDIT_CURSOR,
    NK_COLOR_COMBO,
    NK_COLOR_CHART,
    NK_COLOR_CHART_COLOR,
    NK_COLOR_CHART_COLOR_HIGHLIGHT,
    NK_COLOR_SCROLLBAR,
    NK_COLOR_SCROLLBAR_CURSOR,
    NK_COLOR_SCROLLBAR_CURSOR_HOVER,
    NK_COLOR_SCROLLBAR_CURSOR_ACTIVE,
    NK_COLOR_TAB_HEADER,
    NK_COLOR_KNOB,
    NK_COLOR_KNOB_CURSOR,
    NK_COLOR_KNOB_CURSOR_HOVER,
    NK_COLOR_KNOB_CURSOR_ACTIVE,
    NK_COLOR_COUNT
};
enum nk_style_cursor {
    NK_CURSOR_ARROW,
    NK_CURSOR_TEXT,
    NK_CURSOR_MOVE,
    NK_CURSOR_RESIZE_VERTICAL,
    NK_CURSOR_RESIZE_HORIZONTAL,
    NK_CURSOR_RESIZE_TOP_LEFT_DOWN_RIGHT,
    NK_CURSOR_RESIZE_TOP_RIGHT_DOWN_LEFT,
    NK_CURSOR_COUNT
};
NK_API void nk_style_default(struct nk_context*);
NK_API void nk_style_from_table(struct nk_context*, const struct nk_color*);
NK_API void nk_style_load_cursor(struct nk_context*, enum nk_style_cursor, const struct nk_cursor*);
NK_API void nk_style_load_all_cursors(struct nk_context*, struct nk_cursor*);
NK_API const char* nk_style_get_color_by_name(enum nk_style_colors);
NK_API void nk_style_set_font(struct nk_context*, const struct nk_user_font*);
NK_API nk_bool nk_style_set_cursor(struct nk_context*, enum nk_style_cursor);
NK_API void nk_style_show_cursor(struct nk_context*);
NK_API void nk_style_hide_cursor(struct nk_context*);

NK_API nk_bool nk_style_push_font(struct nk_context*, const struct nk_user_font*);
NK_API nk_bool nk_style_push_float(struct nk_context*, float*, float);
NK_API nk_bool nk_style_push_vec2(struct nk_context*, struct nk_vec2*, struct nk_vec2);
NK_API nk_bool nk_style_push_style_item(struct nk_context*, struct nk_style_item*, struct nk_style_item);
NK_API nk_bool nk_style_push_flags(struct nk_context*, nk_flags*, nk_flags);
NK_API nk_bool nk_style_push_color(struct nk_context*, struct nk_color*, struct nk_color);

NK_API nk_bool nk_style_pop_font(struct nk_context*);
NK_API nk_bool nk_style_pop_float(struct nk_context*);
NK_API nk_bool nk_style_pop_vec2(struct nk_context*);
NK_API nk_bool nk_style_pop_style_item(struct nk_context*);
NK_API nk_bool nk_style_pop_flags(struct nk_context*);
NK_API nk_bool nk_style_pop_color(struct nk_context*);
/* =============================================================================
 *
 *                                  COLOR
 *
 * ============================================================================= */
NK_API struct nk_color nk_rgb(int r, int g, int b);
NK_API struct nk_color nk_rgb_iv(const int *rgb);
NK_API struct nk_color nk_rgb_bv(const nk_byte* rgb);
NK_API struct nk_color nk_rgb_f(float r, float g, float b);
NK_API struct nk_color nk_rgb_fv(const float *rgb);
NK_API struct nk_color nk_rgb_cf(struct nk_colorf c);
NK_API struct nk_color nk_rgb_hex(const char *rgb);
NK_API struct nk_color nk_rgb_factor(struct nk_color col, const float factor);

NK_API struct nk_color nk_rgba(int r, int g, int b, int a);
NK_API struct nk_color nk_rgba_u32(nk_uint);
NK_API struct nk_color nk_rgba_iv(const int *rgba);
NK_API struct nk_color nk_rgba_bv(const nk_byte *rgba);
NK_API struct nk_color nk_rgba_f(float r, float g, float b, float a);
NK_API struct nk_color nk_rgba_fv(const float *rgba);
NK_API struct nk_color nk_rgba_cf(struct nk_colorf c);
NK_API struct nk_color nk_rgba_hex(const char *rgb);

NK_API struct nk_colorf nk_hsva_colorf(float h, float s, float v, float a);
NK_API struct nk_colorf nk_hsva_colorfv(float *c);
NK_API void nk_colorf_hsva_f(float *out_h, float *out_s, float *out_v, float *out_a, struct nk_colorf in);
NK_API void nk_colorf_hsva_fv(float *hsva, struct nk_colorf in);

NK_API struct nk_color nk_hsv(int h, int s, int v);
NK_API struct nk_color nk_hsv_iv(const int *hsv);
NK_API struct nk_color nk_hsv_bv(const nk_byte *hsv);
NK_API struct nk_color nk_hsv_f(float h, float s, float v);
NK_API struct nk_color nk_hsv_fv(const float *hsv);

NK_API struct nk_color nk_hsva(int h, int s, int v, int a);
NK_API struct nk_color nk_hsva_iv(const int *hsva);
NK_API struct nk_color nk_hsva_bv(const nk_byte *hsva);
NK_API struct nk_color nk_hsva_f(float h, float s, float v, float a);
NK_API struct nk_color nk_hsva_fv(const float *hsva);

/* color (conversion nuklear --> user) */
NK_API void nk_color_f(float *r, float *g, float *b, float *a, struct nk_color);
NK_API void nk_color_fv(float *rgba_out, struct nk_color);
NK_API struct nk_colorf nk_color_cf(struct nk_color);
NK_API void nk_color_d(double *r, double *g, double *b, double *a, struct nk_color);
NK_API void nk_color_dv(double *rgba_out, struct nk_color);

NK_API nk_uint nk_color_u32(struct nk_color);
NK_API void nk_color_hex_rgba(char *output, struct nk_color);
NK_API void nk_color_hex_rgb(char *output, struct nk_color);

NK_API void nk_color_hsv_i(int *out_h, int *out_s, int *out_v, struct nk_color);
NK_API void nk_color_hsv_b(nk_byte *out_h, nk_byte *out_s, nk_byte *out_v, struct nk_color);
NK_API void nk_color_hsv_iv(int *hsv_out, struct nk_color);
NK_API void nk_color_hsv_bv(nk_byte *hsv_out, struct nk_color);
NK_API void nk_color_hsv_f(float *out_h, float *out_s, float *out_v, struct nk_color);
NK_API void nk_color_hsv_fv(float *hsv_out, struct nk_color);

NK_API void nk_color_hsva_i(int *h, int *s, int *v, int *a, struct nk_color);
NK_API void nk_color_hsva_b(nk_byte *h, nk_byte *s, nk_byte *v, nk_byte *a, struct nk_color);
NK_API void nk_color_hsva_iv(int *hsva_out, struct nk_color);
NK_API void nk_color_hsva_bv(nk_byte *hsva_out, struct nk_color);
NK_API void nk_color_hsva_f(float *out_h, float *out_s, float *out_v, float *out_a, struct nk_color);
NK_API void nk_color_hsva_fv(float *hsva_out, struct nk_color);
/* =============================================================================
 *
 *                                  IMAGE
 *
 * ============================================================================= */
NK_API nk_handle nk_handle_ptr(void*);
NK_API nk_handle nk_handle_id(int);
NK_API struct nk_image nk_image_handle(nk_handle);
NK_API struct nk_image nk_image_ptr(void*);
NK_API struct nk_image nk_image_id(int);
NK_API nk_bool nk_image_is_subimage(const struct nk_image* img);
NK_API struct nk_image nk_subimage_ptr(void*, nk_ushort w, nk_ushort h, struct nk_rect sub_region);
NK_API struct nk_image nk_subimage_id(int, nk_ushort w, nk_ushort h, struct nk_rect sub_region);
NK_API struct nk_image nk_subimage_handle(nk_handle, nk_ushort w, nk_ushort h, struct nk_rect sub_region);
/* =============================================================================
 *
 *                                  9-SLICE
 *
 * ============================================================================= */
NK_API struct nk_nine_slice nk_nine_slice_handle(nk_handle, nk_ushort l, nk_ushort t, nk_ushort r, nk_ushort b);
NK_API struct nk_nine_slice nk_nine_slice_ptr(void*, nk_ushort l, nk_ushort t, nk_ushort r, nk_ushort b);
NK_API struct nk_nine_slice nk_nine_slice_id(int, nk_ushort l, nk_ushort t, nk_ushort r, nk_ushort b);
NK_API int nk_nine_slice_is_sub9slice(const struct nk_nine_slice* img);
NK_API struct nk_nine_slice nk_sub9slice_ptr(void*, nk_ushort w, nk_ushort h, struct nk_rect sub_region, nk_ushort l, nk_ushort t, nk_ushort r, nk_ushort b);
NK_API struct nk_nine_slice nk_sub9slice_id(int, nk_ushort w, nk_ushort h, struct nk_rect sub_region, nk_ushort l, nk_ushort t, nk_ushort r, nk_ushort b);
NK_API struct nk_nine_slice nk_sub9slice_handle(nk_handle, nk_ushort w, nk_ushort h, struct nk_rect sub_region, nk_ushort l, nk_ushort t, nk_ushort r, nk_ushort b);
/* =============================================================================
 *
 *                                  MATH
 *
 * ============================================================================= */
NK_API nk_hash nk_murmur_hash(const void *key, int len, nk_hash seed);
NK_API void nk_triangle_from_direction(struct nk_vec2 *result, struct nk_rect r, float pad_x, float pad_y, enum nk_heading);

NK_API struct nk_vec2 nk_vec2(float x, float y);
NK_API struct nk_vec2 nk_vec2i(int x, int y);
NK_API struct nk_vec2 nk_vec2v(const float *xy);
NK_API struct nk_vec2 nk_vec2iv(const int *xy);

NK_API struct nk_rect nk_get_null_rect(void);
NK_API struct nk_rect nk_rect(float x, float y, float w, float h);
NK_API struct nk_rect nk_recti(int x, int y, int w, int h);
NK_API struct nk_rect nk_recta(struct nk_vec2 pos, struct nk_vec2 size);
NK_API struct nk_rect nk_rectv(const float *xywh);
NK_API struct nk_rect nk_rectiv(const int *xywh);
NK_API struct nk_vec2 nk_rect_pos(struct nk_rect);
NK_API struct nk_vec2 nk_rect_size(struct nk_rect);
/* =============================================================================
 *
 *                                  STRING
 *
 * ============================================================================= */
NK_API int nk_strlen(const char *str);
NK_API int nk_stricmp(const char *s1, const char *s2);
NK_API int nk_stricmpn(const char *s1, const char *s2, int n);
NK_API int nk_strtoi(const char *str, char **endptr);
NK_API float nk_strtof(const char *str, char **endptr);
#ifndef NK_STRTOD
#define NK_STRTOD nk_strtod
NK_API double nk_strtod(const char *str, char **endptr);
#endif
NK_API int nk_strfilter(const char *text, const char *regexp);
NK_API int nk_strmatch_fuzzy_string(char const *str, char const *pattern, int *out_score);
NK_API int nk_strmatch_fuzzy_text(const char *txt, int txt_len, const char *pattern, int *out_score);
/* =============================================================================
 *
 *                                  UTF-8
 *
 * ============================================================================= */
NK_API int nk_utf_decode(const char*, nk_rune*, int);
NK_API int nk_utf_encode(nk_rune, char*, int);
NK_API int nk_utf_len(const char*, int byte_len);
NK_API const char* nk_utf_at(const char *buffer, int length, int index, nk_rune *unicode, int *len);
/* ===============================================================
 *
 *                          FONT
 *
 * ===============================================================*/
/*/// ### Font
/// Font handling in this library was designed to be quite customizable and lets
/// you decide what you want to use and what you want to provide. There are three
/// different ways to use the font atlas. The first two will use your font
/// handling scheme and only requires essential data to run nuklear. The next
/// slightly more advanced features is font handling with vertex buffer output.
/// Finally the most complex API wise is using nuklear's font baking API.
//
/// #### Using your own implementation without vertex buffer output
///
/// So first up the easiest way to do font handling is by just providing a
/// `nk_user_font` struct which only requires the height in pixel of the used
/// font and a callback to calculate the width of a string. This way of handling
/// fonts is best fitted for using the normal draw shape command API where you
/// do all the text drawing yourself and the library does not require any kind
/// of deeper knowledge about which font handling mechanism you use.
/// IMPORTANT: the `nk_user_font` pointer provided to nuklear has to persist
/// over the complete life time! I know this sucks but it is currently the only
/// way to switch between fonts.
///
/// ```c
///     float your_text_width_calculation(nk_handle handle, float height, const char *text, int len)
///     {
///         your_font_type *type = handle.ptr;
///         float text_width = ...;
///         return text_width;
///     }
///
///     struct nk_user_font font;
///     font.userdata.ptr = &your_font_class_or_struct;
///     font.height = your_font_height;
///     font.width = your_text_width_calculation;
///
///     struct nk_context ctx;
///     nk_init_default(&ctx, &font);
/// ```
/// #### Using your own implementation with vertex buffer output
///
/// While the first approach works fine if you don't want to use the optional
/// vertex buffer output it is not enough if you do. To get font handling working
/// for these cases you have to provide two additional parameters inside the
/// `nk_user_font`. First a texture atlas handle used to draw text as subimages
/// of a bigger font atlas texture and a callback to query a character's glyph
/// information (offset, size, ...). So it is still possible to provide your own
/// font and use the vertex buffer output.
///
/// ```c
///     float your_text_width_calculation(nk_handle handle, float height, const char *text, int len)
///     {
///         your_font_type *type = handle.ptr;
///         float text_width = ...;
///         return text_width;
///     }
///     void query_your_font_glyph(nk_handle handle, float font_height, struct nk_user_font_glyph *glyph, nk_rune codepoint, nk_rune next_codepoint)
///     {
///         your_font_type *type = handle.ptr;
///         glyph.width = ...;
///         glyph.height = ...;
///         glyph.xadvance = ...;
///         glyph.uv[0].x = ...;
///         glyph.uv[0].y = ...;
///         glyph.uv[1].x = ...;
///         glyph.uv[1].y = ...;
///         glyph.offset.x = ...;
///         glyph.offset.y = ...;
///     }
///
///     struct nk_user_font font;
///     font.userdata.ptr = &your_font_class_or_struct;
///     font.height = your_font_height;
///     font.width = your_text_width_calculation;
///     font.query = query_your_font_glyph;
///     font.texture.id = your_font_texture;
///
///     struct nk_context ctx;
///     nk_init_default(&ctx, &font);
/// ```
///
/// #### Nuklear font baker
///
/// The final approach if you do not have a font handling functionality or don't
/// want to use it in this library is by using the optional font baker.
/// The font baker APIs can be used to create a font plus font atlas texture
/// and can be used with or without the vertex buffer output.
///
/// It still uses the `nk_user_font` struct and the two different approaches
/// previously stated still work. The font baker is not located inside
/// `nk_context` like all other systems since it can be understood as more of
/// an extension to nuklear and does not really depend on any `nk_context` state.
///
/// Font baker need to be initialized first by one of the nk_font_atlas_init_xxx
/// functions. If you don't care about memory just call the default version
/// `nk_font_atlas_init_default` which will allocate all memory from the standard library.
/// If you want to control memory allocation but you don't care if the allocated
/// memory is temporary and therefore can be freed directly after the baking process
/// is over or permanent you can call `nk_font_atlas_init`.
///
/// After successfully initializing the font baker you can add Truetype(.ttf) fonts from
/// different sources like memory or from file by calling one of the `nk_font_atlas_add_xxx`.
/// functions. Adding font will permanently store each font, font config and ttf memory block(!)
/// inside the font atlas and allows to reuse the font atlas. If you don't want to reuse
/// the font baker by for example adding additional fonts you can call
/// `nk_font_atlas_cleanup` after the baking process is over (after calling nk_font_atlas_end).
///
/// As soon as you added all fonts you wanted you can now start the baking process
/// for every selected glyph to image by calling `nk_font_atlas_bake`.
/// The baking process returns image memory, width and height which can be used to
/// either create your own image object or upload it to any graphics library.
/// No matter which case you finally have to call `nk_font_atlas_end` which
/// will free all temporary memory including the font atlas image so make sure
/// you created our texture beforehand. `nk_font_atlas_end` requires a handle
/// to your font texture or object and optionally fills a `struct nk_draw_null_texture`
/// which can be used for the optional vertex output. If you don't want it just
/// set the argument to `NULL`.
///
/// At this point you are done and if you don't want to reuse the font atlas you
/// can call `nk_font_atlas_cleanup` to free all truetype blobs and configuration
/// memory. Finally if you don't use the font atlas and any of it's fonts anymore
/// you need to call `nk_font_atlas_clear` to free all memory still being used.
///
/// ```c
///     struct nk_font_atlas atlas;
///     nk_font_atlas_init_default(&atlas);
///     nk_font_atlas_begin(&atlas);
///     nk_font *font = nk_font_atlas_add_from_file(&atlas, "Path/To/Your/TTF_Font.ttf", 13, 0);
///     nk_font *font2 = nk_font_atlas_add_from_file(&atlas, "Path/To/Your/TTF_Font2.ttf", 16, 0);
///     const void* img = nk_font_atlas_bake(&atlas, &img_width, &img_height, NK_FONT_ATLAS_RGBA32);
///     nk_font_atlas_end(&atlas, nk_handle_id(texture), 0);
///
///     struct nk_context ctx;
///     nk_init_default(&ctx, &font->handle);
///     while (1) {
///
///     }
///     nk_font_atlas_clear(&atlas);
/// ```
/// The font baker API is probably the most complex API inside this library and
/// I would suggest reading some of my examples `example/` to get a grip on how
/// to use the font atlas. There are a number of details I left out. For example
/// how to merge fonts, configure a font with `nk_font_config` to use other languages,
/// use another texture coordinate format and a lot more:
///
/// ```c
///     struct nk_font_config cfg = nk_font_config(font_pixel_height);
///     cfg.merge_mode = nk_false or nk_true;
///     cfg.range = nk_font_korean_glyph_ranges();
///     cfg.coord_type = NK_COORD_PIXEL;
///     nk_font *font = nk_font_atlas_add_from_file(&atlas, "Path/To/Your/TTF_Font.ttf", 13, &cfg);
/// ```
*/
struct nk_user_font_glyph;
typedef float(*nk_text_width_f)(nk_handle, float h, const char*, int len);
typedef void(*nk_query_font_glyph_f)(nk_handle handle, float font_height,
                                    struct nk_user_font_glyph *glyph,
                                    nk_rune codepoint, nk_rune next_codepoint);

#if defined(NK_INCLUDE_VERTEX_BUFFER_OUTPUT) || defined(NK_INCLUDE_SOFTWARE_FONT)
struct nk_user_font_glyph {
    struct nk_vec2 uv[2];
    /* texture coordinates */
    struct nk_vec2 offset;
    /* offset between top left and glyph */
    float width, height;
    /* size of the glyph  */
    float xadvance;
    /* offset to the next glyph */
};
#endif

struct nk_user_font {
    nk_handle userdata;
    /* user provided font handle */
    float height;
    /* max height of the font */
    nk_text_width_f width;
    /* font string width in pixel callback */
#ifdef NK_INCLUDE_VERTEX_BUFFER_OUTPUT
    nk_query_font_glyph_f query;
    /* font glyph callback to query drawing info */
    nk_handle texture;
    /* texture handle to the used font atlas or texture */
#endif
};

#ifdef NK_INCLUDE_FONT_BAKING
enum nk_font_coord_type {
    NK_COORD_UV, /* texture coordinates inside font glyphs are clamped between 0-1 */
    NK_COORD_PIXEL /* texture coordinates inside font glyphs are in absolute pixel */
};

struct nk_font;
struct nk_baked_font {
    float height;
    /* height of the font  */
    float ascent, descent;
    /* font glyphs ascent and descent  */
    nk_rune glyph_offset;
    /* glyph array offset inside the font glyph baking output array  */
    nk_rune glyph_count;
    /* number of glyphs of this font inside the glyph baking array output */
    const nk_rune *ranges;
    /* font codepoint ranges as pairs of (from/to) and 0 as last element */
};

struct nk_font_config {
    struct nk_font_config *next;
    /* NOTE: only used internally */
    void *ttf_blob;
    /* pointer to loaded TTF file memory block.
     * NOTE: not needed for nk_font_atlas_add_from_memory and nk_font_atlas_add_from_file. */
    nk_size ttf_size;
    /* size of the loaded TTF file memory block
     * NOTE: not needed for nk_font_atlas_add_from_memory and nk_font_atlas_add_from_file. */

    unsigned char ttf_data_owned_by_atlas;
    /* used inside font atlas: default to: 0*/
    unsigned char merge_mode;
    /* merges this font into the last font */
    unsigned char pixel_snap;
    /* align every character to pixel boundary (if true set oversample (1,1)) */
    unsigned char oversample_v, oversample_h;
    /* rasterize at high quality for sub-pixel position */
    unsigned char padding[3];

    float size;
    /* baked pixel height of the font */
    enum nk_font_coord_type coord_type;
    /* texture coordinate format with either pixel or UV coordinates */
    struct nk_vec2 spacing;
    /* extra pixel spacing between glyphs  */
    const nk_rune *range;
    /* list of unicode ranges (2 values per range, zero terminated) */
    struct nk_baked_font *font;
    /* font to setup in the baking process: NOTE: not needed for font atlas */
    nk_rune fallback_glyph;
    /* fallback glyph to use if a given rune is not found */
    struct nk_font_config *n;
    struct nk_font_config *p;
};

struct nk_font_glyph {
    nk_rune codepoint;
    float xadvance;
    float x0, y0, x1, y1, w, h;
    float u0, v0, u1, v1;
};

struct nk_font {
    struct nk_font *next;
    struct nk_user_font handle;
    struct nk_baked_font info;
    float scale;
    struct nk_font_glyph *glyphs;
    const struct nk_font_glyph *fallback;
    nk_rune fallback_codepoint;
    nk_handle texture;
    struct nk_font_config *config;
};

enum nk_font_atlas_format {
    NK_FONT_ATLAS_ALPHA8,
    NK_FONT_ATLAS_RGBA32
};

struct nk_font_atlas {
    void *pixel;
    int tex_width;
    int tex_height;

    struct nk_allocator permanent;
    struct nk_allocator temporary;

    struct nk_recti custom;
    struct nk_cursor cursors[NK_CURSOR_COUNT];

    int glyph_count;
    struct nk_font_glyph *glyphs;
    struct nk_font *default_font;
    struct nk_font *fonts;
    struct nk_font_config *config;
    int font_num;
};

/* some language glyph codepoint ranges */
NK_API const nk_rune *nk_font_default_glyph_ranges(void);
NK_API const nk_rune *nk_font_chinese_glyph_ranges(void);
NK_API const nk_rune *nk_font_cyrillic_glyph_ranges(void);
NK_API const nk_rune *nk_font_korean_glyph_ranges(void);

#ifdef NK_INCLUDE_DEFAULT_ALLOCATOR
NK_API void nk_font_atlas_init_default(struct nk_font_atlas*);
#endif
NK_API void nk_font_atlas_init(struct nk_font_atlas*, const struct nk_allocator*);
NK_API void nk_font_atlas_init_custom(struct nk_font_atlas*, const struct nk_allocator *persistent, const struct nk_allocator *transient);
NK_API void nk_font_atlas_begin(struct nk_font_atlas*);
NK_API struct nk_font_config nk_font_config(float pixel_height);
NK_API struct nk_font *nk_font_atlas_add(struct nk_font_atlas*, const struct nk_font_config*);
#ifdef NK_INCLUDE_DEFAULT_FONT
NK_API struct nk_font* nk_font_atlas_add_default(struct nk_font_atlas*, float height, const struct nk_font_config*);
#endif
NK_API struct nk_font* nk_font_atlas_add_from_memory(struct nk_font_atlas *atlas, void *memory, nk_size size, float height, const struct nk_font_config *config);
#ifdef NK_INCLUDE_STANDARD_IO
NK_API struct nk_font* nk_font_atlas_add_from_file(struct nk_font_atlas *atlas, const char *file_path, float height, const struct nk_font_config*);
#endif
NK_API struct nk_font *nk_font_atlas_add_compressed(struct nk_font_atlas*, void *memory, nk_size size, float height, const struct nk_font_config*);
NK_API struct nk_font* nk_font_atlas_add_compressed_base85(struct nk_font_atlas*, const char *data, float height, const struct nk_font_config *config);
NK_API const void* nk_font_atlas_bake(struct nk_font_atlas*, int *width, int *height, enum nk_font_atlas_format);
NK_API void nk_font_atlas_end(struct nk_font_atlas*, nk_handle tex, struct nk_draw_null_texture*);
NK_API const struct nk_font_glyph* nk_font_find_glyph(struct nk_font*, nk_rune unicode);
NK_API void nk_font_atlas_cleanup(struct nk_font_atlas *atlas);
NK_API void nk_font_atlas_clear(struct nk_font_atlas*);

#endif

/* ==============================================================
 *
 *                          MEMORY BUFFER
 *
 * ===============================================================*/
/*/// ### Memory Buffer
/// A basic (double)-buffer with linear allocation and resetting as only
/// freeing policy. The buffer's main purpose is to control all memory management
/// inside the GUI toolkit and still leave memory control as much as possible in
/// the hand of the user while also making sure the library is easy to use if
/// not as much control is needed.
/// In general all memory inside this library can be provided from the user in
/// three different ways.
///
/// The first way and the one providing most control is by just passing a fixed
/// size memory block. In this case all control lies in the hand of the user
/// since he can exactly control where the memory comes from and how much memory
/// the library should consume. Of course using the fixed size API removes the
/// ability to automatically resize a buffer if not enough memory is provided so
/// you have to take over the resizing. While being a fixed sized buffer sounds
/// quite limiting, it is very effective in this library since the actual memory
/// consumption is quite stable and has a fixed upper bound for a lot of cases.
///
/// If you don't want to think about how much memory the library should allocate
/// at all time or have a very dynamic UI with unpredictable memory consumption
/// habits but still want control over memory allocation you can use the dynamic
/// allocator based API. The allocator consists of two callbacks for allocating
/// and freeing memory and optional userdata so you can plugin your own allocator.
///
/// The final and easiest way can be used by defining
/// NK_INCLUDE_DEFAULT_ALLOCATOR which uses the standard library memory
/// allocation functions malloc and free and takes over complete control over
/// memory in this library.
*/
struct nk_memory_status {
    void *memory;
    unsigned int type;
    nk_size size;
    nk_size allocated;
    nk_size needed;
    nk_size calls;
};

enum nk_allocation_type {
    NK_BUFFER_FIXED,
    NK_BUFFER_DYNAMIC
};

enum nk_buffer_allocation_type {
    NK_BUFFER_FRONT,
    NK_BUFFER_BACK,
    NK_BUFFER_MAX
};

struct nk_buffer_marker {
    nk_bool active;
    nk_size offset;
};

struct nk_memory {void *ptr;nk_size size;};
struct nk_buffer {
    struct nk_buffer_marker marker[NK_BUFFER_MAX];
    /* buffer marker to free a buffer to a certain offset */
    struct nk_allocator pool;
    /* allocator callback for dynamic buffers */
    enum nk_allocation_type type;
    /* memory management type */
    struct nk_memory memory;
    /* memory and size of the current memory block */
    float grow_factor;
    /* growing factor for dynamic memory management */
    nk_size allocated;
    /* total amount of memory allocated */
    nk_size needed;
    /* totally consumed memory given that enough memory is present */
    nk_size calls;
    /* number of allocation calls */
    nk_size size;
    /* current size of the buffer */
};

#ifdef NK_INCLUDE_DEFAULT_ALLOCATOR
NK_API void nk_buffer_init_default(struct nk_buffer*);
#endif
NK_API void nk_buffer_init(struct nk_buffer*, const struct nk_allocator*, nk_size size);
NK_API void nk_buffer_init_fixed(struct nk_buffer*, void *memory, nk_size size);
NK_API void nk_buffer_info(struct nk_memory_status*, struct nk_buffer*);
NK_API void nk_buffer_push(struct nk_buffer*, enum nk_buffer_allocation_type type, const void *memory, nk_size size, nk_size align);
NK_API void nk_buffer_mark(struct nk_buffer*, enum nk_buffer_allocation_type type);
NK_API void nk_buffer_reset(struct nk_buffer*, enum nk_buffer_allocation_type type);
NK_API void nk_buffer_clear(struct nk_buffer*);
NK_API void nk_buffer_free(struct nk_buffer*);
NK_API void *nk_buffer_memory(struct nk_buffer*);
NK_API const void *nk_buffer_memory_const(const struct nk_buffer*);
NK_API nk_size nk_buffer_total(struct nk_buffer*);

/* ==============================================================
 *
 *                          STRING
 *
 * ===============================================================*/
/*  Basic string buffer which is only used in context with the text editor
 *  to manage and manipulate dynamic or fixed size string content. This is _NOT_
 *  the default string handling method. The only instance you should have any contact
 *  with this API is if you interact with an `nk_text_edit` object inside one of the
 *  copy and paste functions and even there only for more advanced cases. */
struct nk_str {
    struct nk_buffer buffer;
    int len; /* in codepoints/runes/glyphs */
};

#ifdef NK_INCLUDE_DEFAULT_ALLOCATOR
NK_API void nk_str_init_default(struct nk_str*);
#endif
NK_API void nk_str_init(struct nk_str*, const struct nk_allocator*, nk_size size);
NK_API void nk_str_init_fixed(struct nk_str*, void *memory, nk_size size);
NK_API void nk_str_clear(struct nk_str*);
NK_API void nk_str_free(struct nk_str*);

NK_API int nk_str_append_text_char(struct nk_str*, const char*, int);
NK_API int nk_str_append_str_char(struct nk_str*, const char*);
NK_API int nk_str_append_text_utf8(struct nk_str*, const char*, int);
NK_API int nk_str_append_str_utf8(struct nk_str*, const char*);
NK_API int nk_str_append_text_runes(struct nk_str*, const nk_rune*, int);
NK_API int nk_str_append_str_runes(struct nk_str*, const nk_rune*);

NK_API int nk_str_insert_at_char(struct nk_str*, int pos, const char*, int);
NK_API int nk_str_insert_at_rune(struct nk_str*, int pos, const char*, int);

NK_API int nk_str_insert_text_char(struct nk_str*, int pos, const char*, int);
NK_API int nk_str_insert_str_char(struct nk_str*, int pos, const char*);
NK_API int nk_str_insert_text_utf8(struct nk_str*, int pos, const char*, int);
NK_API int nk_str_insert_str_utf8(struct nk_str*, int pos, const char*);
NK_API int nk_str_insert_text_runes(struct nk_str*, int pos, const nk_rune*, int);
NK_API int nk_str_insert_str_runes(struct nk_str*, int pos, const nk_rune*);

NK_API void nk_str_remove_chars(struct nk_str*, int len);
NK_API void nk_str_remove_runes(struct nk_str *str, int len);
NK_API void nk_str_delete_chars(struct nk_str*, int pos, int len);
NK_API void nk_str_delete_runes(struct nk_str*, int pos, int len);

NK_API char *nk_str_at_char(struct nk_str*, int pos);
NK_API char *nk_str_at_rune(struct nk_str*, int pos, nk_rune *unicode, int *len);
NK_API nk_rune nk_str_rune_at(const struct nk_str*, int pos);
NK_API const char *nk_str_at_char_const(const struct nk_str*, int pos);
NK_API const char *nk_str_at_const(const struct nk_str*, int pos, nk_rune *unicode, int *len);

NK_API char *nk_str_get(struct nk_str*);
NK_API const char *nk_str_get_const(const struct nk_str*);
NK_API int nk_str_len(struct nk_str*);
NK_API int nk_str_len_char(struct nk_str*);

/*===============================================================
 *
 *                      TEXT EDITOR
 *
 * ===============================================================*/
/*/// ### Text Editor
/// Editing text in this library is handled by either `nk_edit_string` or
/// `nk_edit_buffer`. But like almost everything in this library there are multiple
/// ways of doing it and a balance between control and ease of use with memory
/// as well as functionality controlled by flags.
///
/// This library generally allows three different levels of memory control:
/// First of is the most basic way of just providing a simple char array with
/// string length. This method is probably the easiest way of handling simple
/// user text input. Main upside is complete control over memory while the biggest
/// downside in comparison with the other two approaches is missing undo/redo.
///
/// For UIs that require undo/redo the second way was created. It is based on
/// a fixed size nk_text_edit struct, which has an internal undo/redo stack.
/// This is mainly useful if you want something more like a text editor but don't want
/// to have a dynamically growing buffer.
///
/// The final way is using a dynamically growing nk_text_edit struct, which
/// has both a default version if you don't care where memory comes from and an
/// allocator version if you do. While the text editor is quite powerful for its
/// complexity I would not recommend editing gigabytes of data with it.
/// It is rather designed for uses cases which make sense for a GUI library not for
/// an full blown text editor.
 */
#ifndef NK_TEXTEDIT_UNDOSTATECOUNT
#define NK_TEXTEDIT_UNDOSTATECOUNT     99
#endif

#ifndef NK_TEXTEDIT_UNDOCHARCOUNT
#define NK_TEXTEDIT_UNDOCHARCOUNT      999
#endif

struct nk_text_edit;
struct nk_clipboard {
    nk_handle userdata;
    nk_plugin_paste paste;
    nk_plugin_copy copy;
};

struct nk_text_undo_record {
   int where;
   short insert_length;
   short delete_length;
   short char_storage;
};

struct nk_text_undo_state {
   struct nk_text_undo_record undo_rec[NK_TEXTEDIT_UNDOSTATECOUNT];
   nk_rune undo_char[NK_TEXTEDIT_UNDOCHARCOUNT];
   short undo_point;
   short redo_point;
   short undo_char_point;
   short redo_char_point;
};

enum nk_text_edit_type {
    NK_TEXT_EDIT_SINGLE_LINE,
    NK_TEXT_EDIT_MULTI_LINE
};

enum nk_text_edit_mode {
    NK_TEXT_EDIT_MODE_VIEW,
    NK_TEXT_EDIT_MODE_INSERT,
    NK_TEXT_EDIT_MODE_REPLACE
};

struct nk_text_edit {
    struct nk_clipboard clip;
    struct nk_str string;
    nk_plugin_filter filter;
    struct nk_vec2 scrollbar;

    int cursor;
    int select_start;
    int select_end;
    unsigned char mode;
    unsigned char cursor_at_end_of_line;
    unsigned char initialized;
    unsigned char has_preferred_x;
    unsigned char single_line;
    unsigned char active;
    unsigned char padding1;
    float preferred_x;
    struct nk_text_undo_state undo;
};

/* filter function */
NK_API nk_bool nk_filter_default(const struct nk_text_edit*, nk_rune unicode);
NK_API nk_bool nk_filter_ascii(const struct nk_text_edit*, nk_rune unicode);
NK_API nk_bool nk_filter_float(const struct nk_text_edit*, nk_rune unicode);
NK_API nk_bool nk_filter_decimal(const struct nk_text_edit*, nk_rune unicode);
NK_API nk_bool nk_filter_hex(const struct nk_text_edit*, nk_rune unicode);
NK_API nk_bool nk_filter_oct(const struct nk_text_edit*, nk_rune unicode);
NK_API nk_bool nk_filter_binary(const struct nk_text_edit*, nk_rune unicode);

/* text editor */
#ifdef NK_INCLUDE_DEFAULT_ALLOCATOR
NK_API void nk_textedit_init_default(struct nk_text_edit*);
#endif
NK_API void nk_textedit_init(struct nk_text_edit*, const struct nk_allocator*, nk_size size);
NK_API void nk_textedit_init_fixed(struct nk_text_edit*, void *memory, nk_size size);
NK_API void nk_textedit_free(struct nk_text_edit*);
NK_API void nk_textedit_text(struct nk_text_edit*, const char*, int total_len);
NK_API void nk_textedit_delete(struct nk_text_edit*, int where, int len);
NK_API void nk_textedit_delete_selection(struct nk_text_edit*);
NK_API void nk_textedit_select_all(struct nk_text_edit*);
NK_API nk_bool nk_textedit_cut(struct nk_text_edit*);
NK_API nk_bool nk_textedit_paste(struct nk_text_edit*, char const*, int len);
NK_API void nk_textedit_undo(struct nk_text_edit*);
NK_API void nk_textedit_redo(struct nk_text_edit*);

/* ===============================================================
 *
 *                          DRAWING
 *
 * ===============================================================*/
/*/// ### Drawing
/// This library was designed to be render backend agnostic so it does
/// not draw anything to screen. Instead all drawn shapes, widgets
/// are made of, are buffered into memory and make up a command queue.
/// Each frame therefore fills the command buffer with draw commands
/// that then need to be executed by the user and his own render backend.
/// After that the command buffer needs to be cleared and a new frame can be
/// started. It is probably important to note that the command buffer is the main
/// drawing API and the optional vertex buffer API only takes this format and
/// converts it into a hardware accessible format.
///
/// To use the command queue to draw your own widgets you can access the
/// command buffer of each window by calling `nk_window_get_canvas` after
/// previously having called `nk_begin`:
///
/// ```c
///     void draw_red_rectangle_widget(struct nk_context *ctx)
///     {
///         struct nk_command_buffer *canvas;
///         struct nk_input *input = &ctx->input;
///         canvas = nk_window_get_canvas(ctx);
///
///         struct nk_rect space;
///         enum nk_widget_layout_states state;
///         state = nk_widget(&space, ctx);
///         if (!state) return;
///
///         if (state != NK_WIDGET_ROM)
///             update_your_widget_by_user_input(...);
///         nk_fill_rect(canvas, space, 0, nk_rgb(255,0,0));
///     }
///
///     if (nk_begin(...)) {
///         nk_layout_row_dynamic(ctx, 25, 1);
///         draw_red_rectangle_widget(ctx);
///     }
///     nk_end(..)
///
/// ```
/// Important to know if you want to create your own widgets is the `nk_widget`
/// call. It allocates space on the panel reserved for this widget to be used,
/// but also returns the state of the widget space. If your widget is not seen and does
/// not have to be updated it is '0' and you can just return. If it only has
/// to be drawn the state will be `NK_WIDGET_ROM` otherwise you can do both
/// update and draw your widget. The reason for separating is to only draw and
/// update what is actually necessary which is crucial for performance.
*/
enum nk_command_type {
    NK_COMMAND_NOP,
    NK_COMMAND_SCISSOR,
    NK_COMMAND_LINE,
    NK_COMMAND_CURVE,
    NK_COMMAND_RECT,
    NK_COMMAND_RECT_FILLED,
    NK_COMMAND_RECT_MULTI_COLOR,
    NK_COMMAND_CIRCLE,
    NK_COMMAND_CIRCLE_FILLED,
    NK_COMMAND_ARC,
    NK_COMMAND_ARC_FILLED,
    NK_COMMAND_TRIANGLE,
    NK_COMMAND_TRIANGLE_FILLED,
    NK_COMMAND_POLYGON,
    NK_COMMAND_POLYGON_FILLED,
    NK_COMMAND_POLYLINE,
    NK_COMMAND_TEXT,
    NK_COMMAND_IMAGE,
    NK_COMMAND_CUSTOM
};

/* command base and header of every command inside the buffer */
struct nk_command {
    enum nk_command_type type;
    nk_size next;
#ifdef NK_INCLUDE_COMMAND_USERDATA
    nk_handle userdata;
#endif
};

struct nk_command_scissor {
    struct nk_command header;
    short x, y;
    unsigned short w, h;
};

struct nk_command_line {
    struct nk_command header;
    unsigned short line_thickness;
    struct nk_vec2i begin;
    struct nk_vec2i end;
    struct nk_color color;
};

struct nk_command_curve {
    struct nk_command header;
    unsigned short line_thickness;
    struct nk_vec2i begin;
    struct nk_vec2i end;
    struct nk_vec2i ctrl[2];
    struct nk_color color;
};

struct nk_command_rect {
    struct nk_command header;
    unsigned short rounding;
    unsigned short line_thickness;
    short x, y;
    unsigned short w, h;
    struct nk_color color;
};

struct nk_command_rect_filled {
    struct nk_command header;
    unsigned short rounding;
    short x, y;
    unsigned short w, h;
    struct nk_color color;
};

struct nk_command_rect_multi_color {
    struct nk_command header;
    short x, y;
    unsigned short w, h;
    struct nk_color left;
    struct nk_color top;
    struct nk_color bottom;
    struct nk_color right;
};

struct nk_command_triangle {
    struct nk_command header;
    unsigned short line_thickness;
    struct nk_vec2i a;
    struct nk_vec2i b;
    struct nk_vec2i c;
    struct nk_color color;
};

struct nk_command_triangle_filled {
    struct nk_command header;
    struct nk_vec2i a;
    struct nk_vec2i b;
    struct nk_vec2i c;
    struct nk_color color;
};

struct nk_command_circle {
    struct nk_command header;
    short x, y;
    unsigned short line_thickness;
    unsigned short w, h;
    struct nk_color color;
};

struct nk_command_circle_filled {
    struct nk_command header;
    short x, y;
    unsigned short w, h;
    struct nk_color color;
};

struct nk_command_arc {
    struct nk_command header;
    short cx, cy;
    unsigned short r;
    unsigned short line_thickness;
    float a[2];
    struct nk_color color;
};

struct nk_command_arc_filled {
    struct nk_command header;
    short cx, cy;
    unsigned short r;
    float a[2];
    struct nk_color color;
};

struct nk_command_polygon {
    struct nk_command header;
    struct nk_color color;
    unsigned short line_thickness;
    unsigned short point_count;
    struct nk_vec2i points[1];
};

struct nk_command_polygon_filled {
    struct nk_command header;
    struct nk_color color;
    unsigned short point_count;
    struct nk_vec2i points[1];
};

struct nk_command_polyline {
    struct nk_command header;
    struct nk_color color;
    unsigned short line_thickness;
    unsigned short point_count;
    struct nk_vec2i points[1];
};

struct nk_command_image {
    struct nk_command header;
    short x, y;
    unsigned short w, h;
    struct nk_image img;
    struct nk_color col;
};

typedef void (*nk_command_custom_callback)(void *canvas, short x,short y,
    unsigned short w, unsigned short h, nk_handle callback_data);
struct nk_command_custom {
    struct nk_command header;
    short x, y;
    unsigned short w, h;
    nk_handle callback_data;
    nk_command_custom_callback callback;
};

struct nk_command_text {
    struct nk_command header;
    const struct nk_user_font *font;
    struct nk_color background;
    struct nk_color foreground;
    short x, y;
    unsigned short w, h;
    float height;
    int length;
    char string[1];
};

enum nk_command_clipping {
    NK_CLIPPING_OFF = nk_false,
    NK_CLIPPING_ON = nk_true
};

struct nk_command_buffer {
    struct nk_buffer *base;
    struct nk_rect clip;
    int use_clipping;
    nk_handle userdata;
    nk_size begin, end, last;
};

/* shape outlines */
NK_API void nk_stroke_line(struct nk_command_buffer *b, float x0, float y0, float x1, float y1, float line_thickness, struct nk_color);
NK_API void nk_stroke_curve(struct nk_command_buffer*, float, float, float, float, float, float, float, float, float line_thickness, struct nk_color);
NK_API void nk_stroke_rect(struct nk_command_buffer*, struct nk_rect, float rounding, float line_thickness, struct nk_color);
NK_API void nk_stroke_circle(struct nk_command_buffer*, struct nk_rect, float line_thickness, struct nk_color);
NK_API void nk_stroke_arc(struct nk_command_buffer*, float cx, float cy, float radius, float a_min, float a_max, float line_thickness, struct nk_color);
NK_API void nk_stroke_triangle(struct nk_command_buffer*, float, float, float, float, float, float, float line_thichness, struct nk_color);
NK_API void nk_stroke_polyline(struct nk_command_buffer*, float *points, int point_count, float line_thickness, struct nk_color col);
NK_API void nk_stroke_polygon(struct nk_command_buffer*, float*, int point_count, float line_thickness, struct nk_color);

/* filled shades */
NK_API void nk_fill_rect(struct nk_command_buffer*, struct nk_rect, float rounding, struct nk_color);
NK_API void nk_fill_rect_multi_color(struct nk_command_buffer*, struct nk_rect, struct nk_color left, struct nk_color top, struct nk_color right, struct nk_color bottom);
NK_API void nk_fill_circle(struct nk_command_buffer*, struct nk_rect, struct nk_color);
NK_API void nk_fill_arc(struct nk_command_buffer*, float cx, float cy, float radius, float a_min, float a_max, struct nk_color);
NK_API void nk_fill_triangle(struct nk_command_buffer*, float x0, float y0, float x1, float y1, float x2, float y2, struct nk_color);
NK_API void nk_fill_polygon(struct nk_command_buffer*, float*, int point_count, struct nk_color);

/* misc */
NK_API void nk_draw_image(struct nk_command_buffer*, struct nk_rect, const struct nk_image*, struct nk_color);
NK_API void nk_draw_nine_slice(struct nk_command_buffer*, struct nk_rect, const struct nk_nine_slice*, struct nk_color);
NK_API void nk_draw_text(struct nk_command_buffer*, struct nk_rect, const char *text, int len, const struct nk_user_font*, struct nk_color, struct nk_color);
NK_API void nk_push_scissor(struct nk_command_buffer*, struct nk_rect);
NK_API void nk_push_custom(struct nk_command_buffer*, struct nk_rect, nk_command_custom_callback, nk_handle usr);

/* ===============================================================
 *
 *                          INPUT
 *
 * ===============================================================*/
struct nk_mouse_button {
    nk_bool down;
    unsigned int clicked;
    struct nk_vec2 clicked_pos;
};
struct nk_mouse {
    struct nk_mouse_button buttons[NK_BUTTON_MAX];
    struct nk_vec2 pos;
#ifdef NK_BUTTON_TRIGGER_ON_RELEASE
    struct nk_vec2 down_pos;
#endif
    struct nk_vec2 prev;
    struct nk_vec2 delta;
    struct nk_vec2 scroll_delta;
    unsigned char grab;
    unsigned char grabbed;
    unsigned char ungrab;
};

struct nk_key {
    nk_bool down;
    unsigned int clicked;
};
struct nk_keyboard {
    struct nk_key keys[NK_KEY_MAX];
    char text[NK_INPUT_MAX];
    int text_len;
};

struct nk_input {
    struct nk_keyboard keyboard;
    struct nk_mouse mouse;
    int focus_next;
};

NK_API nk_bool nk_input_has_mouse_click(const struct nk_input*, enum nk_buttons);
NK_API nk_bool nk_input_has_mouse_click_in_rect(const struct nk_input*, enum nk_buttons, struct nk_rect);
NK_API nk_bool nk_input_has_mouse_click_in_button_rect(const struct nk_input*, enum nk_buttons, struct nk_rect);
NK_API nk_bool nk_input_has_mouse_click_down_in_rect(const struct nk_input*, enum nk_buttons, struct nk_rect, nk_bool down);
NK_API nk_bool nk_input_is_mouse_click_in_rect(const struct nk_input*, enum nk_buttons, struct nk_rect);
NK_API nk_bool nk_input_is_mouse_click_down_in_rect(const struct nk_input *i, enum nk_buttons id, struct nk_rect b, nk_bool down);
NK_API nk_bool nk_input_any_mouse_click_in_rect(const struct nk_input*, struct nk_rect);
NK_API nk_bool nk_input_is_mouse_prev_hovering_rect(const struct nk_input*, struct nk_rect);
NK_API nk_bool nk_input_is_mouse_hovering_rect(const struct nk_input*, struct nk_rect);
NK_API nk_bool nk_input_mouse_clicked(const struct nk_input*, enum nk_buttons, struct nk_rect);
NK_API nk_bool nk_input_is_mouse_down(const struct nk_input*, enum nk_buttons);
NK_API nk_bool nk_input_is_mouse_pressed(const struct nk_input*, enum nk_buttons);
NK_API nk_bool nk_input_is_mouse_released(const struct nk_input*, enum nk_buttons);
NK_API nk_bool nk_input_is_key_pressed(const struct nk_input*, enum nk_keys);
NK_API nk_bool nk_input_is_key_released(const struct nk_input*, enum nk_keys);
NK_API nk_bool nk_input_is_key_down(const struct nk_input*, enum nk_keys);

/* ===============================================================
 *
 *                          DRAW LIST
 *
 * ===============================================================*/
#ifdef NK_INCLUDE_VERTEX_BUFFER_OUTPUT
/* ### Draw List
/// The optional vertex buffer draw list provides a 2D drawing context
/// with antialiasing functionality which takes basic filled or outlined shapes
/// or a path and outputs vertexes, elements and draw commands.
/// The actual draw list API is not required to be used directly while using this
/// library since converting the default library draw command output is done by
/// just calling `nk_convert` but I decided to still make this library accessible
/// since it can be useful.
///
/// The draw list is based on a path buffering and polygon and polyline
/// rendering API which allows a lot of ways to draw 2D content to screen.
/// In fact it is probably more powerful than needed but allows even more crazy
/// things than this library provides by default.
*/
#ifdef NK_UINT_DRAW_INDEX
typedef nk_uint nk_draw_index;
#else
typedef nk_ushort nk_draw_index;
#endif
enum nk_draw_list_stroke {
    NK_STROKE_OPEN = nk_false,
    /* build up path has no connection back to the beginning */
    NK_STROKE_CLOSED = nk_true
    /* build up path has a connection back to the beginning */
};

enum nk_draw_vertex_layout_attribute {
    NK_VERTEX_POSITION,
    NK_VERTEX_COLOR,
    NK_VERTEX_TEXCOORD,
    NK_VERTEX_ATTRIBUTE_COUNT
};

enum nk_draw_vertex_layout_format {
    NK_FORMAT_SCHAR,
    NK_FORMAT_SSHORT,
    NK_FORMAT_SINT,
    NK_FORMAT_UCHAR,
    NK_FORMAT_USHORT,
    NK_FORMAT_UINT,
    NK_FORMAT_FLOAT,
    NK_FORMAT_DOUBLE,

NK_FORMAT_COLOR_BEGIN,
    NK_FORMAT_R8G8B8 = NK_FORMAT_COLOR_BEGIN,
    NK_FORMAT_R16G15B16,
    NK_FORMAT_R32G32B32,

    NK_FORMAT_R8G8B8A8,
    NK_FORMAT_B8G8R8A8,
    NK_FORMAT_R16G15B16A16,
    NK_FORMAT_R32G32B32A32,
    NK_FORMAT_R32G32B32A32_FLOAT,
    NK_FORMAT_R32G32B32A32_DOUBLE,

    NK_FORMAT_RGB32,
    NK_FORMAT_RGBA32,
NK_FORMAT_COLOR_END = NK_FORMAT_RGBA32,
    NK_FORMAT_COUNT
};

#define NK_VERTEX_LAYOUT_END NK_VERTEX_ATTRIBUTE_COUNT,NK_FORMAT_COUNT,0
struct nk_draw_vertex_layout_element {
    enum nk_draw_vertex_layout_attribute attribute;
    enum nk_draw_vertex_layout_format format;
    nk_size offset;
};

struct nk_draw_command {
    unsigned int elem_count;
    /* number of elements in the current draw batch */
    float strength; /* for font rendering */
    struct nk_rect clip_rect;
    /* current screen clipping rectangle */
    nk_handle texture;
    /* current texture to set */
#ifdef NK_INCLUDE_COMMAND_USERDATA
    nk_handle userdata;
#endif
};

struct nk_draw_list {
    struct nk_rect clip_rect;
    struct nk_vec2 circle_vtx[12];
    struct nk_convert_config config;

    struct nk_buffer *buffer;
    struct nk_buffer *vertices;
    struct nk_buffer *elements;

    unsigned int element_count;
    unsigned int vertex_count;
    unsigned int cmd_count;
    nk_size cmd_offset;

    unsigned int path_count;
    unsigned int path_offset;

    enum nk_anti_aliasing line_AA;
    enum nk_anti_aliasing shape_AA;

#ifdef NK_INCLUDE_COMMAND_USERDATA
    nk_handle userdata;
#endif
};

/* draw list */
NK_API void nk_draw_list_init(struct nk_draw_list*);
NK_API void nk_draw_list_setup(struct nk_draw_list*, const struct nk_convert_config*, struct nk_buffer *cmds, struct nk_buffer *vertices, struct nk_buffer *elements, enum nk_anti_aliasing line_aa,enum nk_anti_aliasing shape_aa);

/* drawing */
#define nk_draw_list_foreach(cmd, can, b) for((cmd)=nk__draw_list_begin(can, b); (cmd)!=0; (cmd)=nk__draw_list_next(cmd, b, can))
NK_API const struct nk_draw_command* nk__draw_list_begin(const struct nk_draw_list*, const struct nk_buffer*);
NK_API const struct nk_draw_command* nk__draw_list_next(const struct nk_draw_command*, const struct nk_buffer*, const struct nk_draw_list*);
NK_API const struct nk_draw_command* nk__draw_list_end(const struct nk_draw_list*, const struct nk_buffer*);

/* path */
NK_API void nk_draw_list_path_clear(struct nk_draw_list*);
NK_API void nk_draw_list_path_line_to(struct nk_draw_list*, struct nk_vec2 pos);
NK_API void nk_draw_list_path_arc_to_fast(struct nk_draw_list*, struct nk_vec2 center, float radius, int a_min, int a_max);
NK_API void nk_draw_list_path_arc_to(struct nk_draw_list*, struct nk_vec2 center, float radius, float a_min, float a_max, unsigned int segments);
NK_API void nk_draw_list_path_rect_to(struct nk_draw_list*, struct nk_vec2 a, struct nk_vec2 b, float rounding);
NK_API void nk_draw_list_path_curve_to(struct nk_draw_list*, struct nk_vec2 p2, struct nk_vec2 p3, struct nk_vec2 p4, unsigned int num_segments);
NK_API void nk_draw_list_path_fill(struct nk_draw_list*, struct nk_color);
NK_API void nk_draw_list_path_stroke(struct nk_draw_list*, struct nk_color, enum nk_draw_list_stroke closed, float thickness);

/* stroke */
NK_API void nk_draw_list_stroke_line(struct nk_draw_list*, struct nk_vec2 a, struct nk_vec2 b, struct nk_color, float thickness);
NK_API void nk_draw_list_stroke_rect(struct nk_draw_list*, struct nk_rect rect, struct nk_color, float rounding, float thickness);
NK_API void nk_draw_list_stroke_triangle(struct nk_draw_list*, struct nk_vec2 a, struct nk_vec2 b, struct nk_vec2 c, struct nk_color, float thickness);
NK_API void nk_draw_list_stroke_circle(struct nk_draw_list*, struct nk_vec2 center, float radius, struct nk_color, unsigned int segs, float thickness);
NK_API void nk_draw_list_stroke_curve(struct nk_draw_list*, struct nk_vec2 p0, struct nk_vec2 cp0, struct nk_vec2 cp1, struct nk_vec2 p1, struct nk_color, unsigned int segments, float thickness);
NK_API void nk_draw_list_stroke_poly_line(struct nk_draw_list*, const struct nk_vec2 *pnts, const unsigned int cnt, struct nk_color, enum nk_draw_list_stroke, float thickness, enum nk_anti_aliasing);

/* fill */
NK_API void nk_draw_list_fill_rect(struct nk_draw_list*, struct nk_rect rect, struct nk_color, float rounding);
NK_API void nk_draw_list_fill_rect_multi_color(struct nk_draw_list*, struct nk_rect rect, struct nk_color left, struct nk_color top, struct nk_color right, struct nk_color bottom);
NK_API void nk_draw_list_fill_triangle(struct nk_draw_list*, struct nk_vec2 a, struct nk_vec2 b, struct nk_vec2 c, struct nk_color);
NK_API void nk_draw_list_fill_circle(struct nk_draw_list*, struct nk_vec2 center, float radius, struct nk_color col, unsigned int segs);
NK_API void nk_draw_list_fill_poly_convex(struct nk_draw_list*, const struct nk_vec2 *points, const unsigned int count, struct nk_color, enum nk_anti_aliasing);

/* misc */
NK_API void nk_draw_list_add_image(struct nk_draw_list*, struct nk_image texture, struct nk_rect rect, struct nk_color);
NK_API void nk_draw_list_add_text(struct nk_draw_list*, const struct nk_user_font*, struct nk_rect, const char *text, int len, float font_height, struct nk_color);
#ifdef NK_INCLUDE_COMMAND_USERDATA
NK_API void nk_draw_list_push_userdata(struct nk_draw_list*, nk_handle userdata);
#endif

#endif

/* ===============================================================
 *
 *                          GUI
 *
 * ===============================================================*/
enum nk_style_item_type {
    NK_STYLE_ITEM_COLOR,
    NK_STYLE_ITEM_IMAGE,
    NK_STYLE_ITEM_NINE_SLICE
};

union nk_style_item_data {
    struct nk_color color;
    struct nk_image image;
    struct nk_nine_slice slice;
};

struct nk_style_item {
    enum nk_style_item_type type;
    union nk_style_item_data data;
};

struct nk_style_text {
    struct nk_color color;
    struct nk_vec2 padding;
    float color_factor;
    float disabled_factor;
};

struct nk_style_button {
    /* background */
    struct nk_style_item normal;
    struct nk_style_item hover;
    struct nk_style_item active;
    struct nk_color border_color;
    float color_factor_background;

    /* text */
    struct nk_color text_background;
    struct nk_color text_normal;
    struct nk_color text_hover;
    struct nk_color text_active;
    nk_flags text_alignment;
    float color_factor_text;

    /* properties */
    float border;
    float rounding;
    struct nk_vec2 padding;
    struct nk_vec2 image_padding;
    struct nk_vec2 touch_padding;
    float disabled_factor;

    /* optional user callbacks */
    nk_handle userdata;
    void(*draw_begin)(struct nk_command_buffer*, nk_handle userdata);
    void(*draw_end)(struct nk_command_buffer*, nk_handle userdata);
};

struct nk_style_toggle {
    /* background */
    struct nk_style_item normal;
    struct nk_style_item hover;
    struct nk_style_item active;
    struct nk_color border_color;

    /* cursor */
    struct nk_style_item cursor_normal;
    struct nk_style_item cursor_hover;

    /* text */
    struct nk_color text_normal;
    struct nk_color text_hover;
    struct nk_color text_active;
    struct nk_color text_background;
    nk_flags text_alignment;

    /* properties */
    struct nk_vec2 padding;
    struct nk_vec2 touch_padding;
    float spacing;
    float border;
    float color_factor;
    float disabled_factor;

    /* optional user callbacks */
    nk_handle userdata;
    void(*draw_begin)(struct nk_command_buffer*, nk_handle);
    void(*draw_end)(struct nk_command_buffer*, nk_handle);
};

struct nk_style_selectable {
    /* background (inactive) */
    struct nk_style_item normal;
    struct nk_style_item hover;
    struct nk_style_item pressed;

    /* background (active) */
    struct nk_style_item normal_active;
    struct nk_style_item hover_active;
    struct nk_style_item pressed_active;

    /* text color (inactive) */
    struct nk_color text_normal;
    struct nk_color text_hover;
    struct nk_color text_pressed;

    /* text color (active) */
    struct nk_color text_normal_active;
    struct nk_color text_hover_active;
    struct nk_color text_pressed_active;
    struct nk_color text_background;
    nk_flags text_alignment;

    /* properties */
    float rounding;
    struct nk_vec2 padding;
    struct nk_vec2 touch_padding;
    struct nk_vec2 image_padding;
    float color_factor;
    float disabled_factor;

    /* optional user callbacks */
    nk_handle userdata;
    void(*draw_begin)(struct nk_command_buffer*, nk_handle);
    void(*draw_end)(struct nk_command_buffer*, nk_handle);
};

struct nk_style_slider {
    /* background */
    struct nk_style_item normal;
    struct nk_style_item hover;
    struct nk_style_item active;
    struct nk_color border_color;

    /* background bar */
    struct nk_color bar_normal;
    struct nk_color bar_hover;
    struct nk_color bar_active;
    struct nk_color bar_filled;

    /* cursor */
    struct nk_style_item cursor_normal;
    struct nk_style_item cursor_hover;
    struct nk_style_item cursor_active;

    /* properties */
    float border;
    float rounding;
    float bar_height;
    struct nk_vec2 padding;
    struct nk_vec2 spacing;
    struct nk_vec2 cursor_size;
    float color_factor;
    float disabled_factor;

    /* optional buttons */
    int show_buttons;
    struct nk_style_button inc_button;
    struct nk_style_button dec_button;
    enum nk_symbol_type inc_symbol;
    enum nk_symbol_type dec_symbol;

    /* optional user callbacks */
    nk_handle userdata;
    void(*draw_begin)(struct nk_command_buffer*, nk_handle);
    void(*draw_end)(struct nk_command_buffer*, nk_handle);
};

struct nk_style_knob {
    /* background */
    struct nk_style_item normal;
    struct nk_style_item hover;
    struct nk_style_item active;
    struct nk_color border_color;

    /* knob */
    struct nk_color knob_normal;
    struct nk_color knob_hover;
    struct nk_color knob_active;
    struct nk_color knob_border_color;

    /* cursor */
    struct nk_color cursor_normal;
    struct nk_color cursor_hover;
    struct nk_color cursor_active;

    /* properties */
    float border;
    float knob_border;
    struct nk_vec2 padding;
    struct nk_vec2 spacing;
    float cursor_width;
    float color_factor;
    float disabled_factor;

    /* optional user callbacks */
    nk_handle userdata;
    void(*draw_begin)(struct nk_command_buffer*, nk_handle);
    void(*draw_end)(struct nk_command_buffer*, nk_handle);
};

struct nk_style_progress {
    /* background */
    struct nk_style_item normal;
    struct nk_style_item hover;
    struct nk_style_item active;
    struct nk_color border_color;

    /* cursor */
    struct nk_style_item cursor_normal;
    struct nk_style_item cursor_hover;
    struct nk_style_item cursor_active;
    struct nk_color cursor_border_color;

    /* properties */
    float rounding;
    float border;
    float cursor_border;
    float cursor_rounding;
    struct nk_vec2 padding;
    float color_factor;
    float disabled_factor;

    /* optional user callbacks */
    nk_handle userdata;
    void(*draw_begin)(struct nk_command_buffer*, nk_handle);
    void(*draw_end)(struct nk_command_buffer*, nk_handle);
};

struct nk_style_scrollbar {
    /* background */
    struct nk_style_item normal;
    struct nk_style_item hover;
    struct nk_style_item active;
    struct nk_color border_color;

    /* cursor */
    struct nk_style_item cursor_normal;
    struct nk_style_item cursor_hover;
    struct nk_style_item cursor_active;
    struct nk_color cursor_border_color;

    /* properties */
    float border;
    float rounding;
    float border_cursor;
    float rounding_cursor;
    struct nk_vec2 padding;
    float color_factor;
    float disabled_factor;

    /* optional buttons */
    int show_buttons;
    struct nk_style_button inc_button;
    struct nk_style_button dec_button;
    enum nk_symbol_type inc_symbol;
    enum nk_symbol_type dec_symbol;

    /* optional user callbacks */
    nk_handle userdata;
    void(*draw_begin)(struct nk_command_buffer*, nk_handle);
    void(*draw_end)(struct nk_command_buffer*, nk_handle);
};

struct nk_style_edit {
    /* background */
    struct nk_style_item normal;
    struct nk_style_item hover;
    struct nk_style_item active;
    struct nk_color border_color;
    struct nk_style_scrollbar scrollbar;

    /* cursor  */
    struct nk_color cursor_normal;
    struct nk_color cursor_hover;
    struct nk_color cursor_text_normal;
    struct nk_color cursor_text_hover;

    /* text (unselected) */
    struct nk_color text_normal;
    struct nk_color text_hover;
    struct nk_color text_active;

    /* text (selected) */
    struct nk_color selected_normal;
    struct nk_color selected_hover;
    struct nk_color selected_text_normal;
    struct nk_color selected_text_hover;

    /* properties */
    float border;
    float rounding;
    float cursor_size;
    struct nk_vec2 scrollbar_size;
    struct nk_vec2 padding;
    float row_padding;
    float color_factor;
    float disabled_factor;
};

struct nk_style_property {
    /* background */
    struct nk_style_item normal;
    struct nk_style_item hover;
    struct nk_style_item active;
    struct nk_color border_color;

    /* text */
    struct nk_color label_normal;
    struct nk_color label_hover;
    struct nk_color label_active;

    /* symbols */
    enum nk_symbol_type sym_left;
    enum nk_symbol_type sym_right;

    /* properties */
    float border;
    float rounding;
    struct nk_vec2 padding;
    float color_factor;
    float disabled_factor;

    struct nk_style_edit edit;
    struct nk_style_button inc_button;
    struct nk_style_button dec_button;

    /* optional user callbacks */
    nk_handle userdata;
    void(*draw_begin)(struct nk_command_buffer*, nk_handle);
    void(*draw_end)(struct nk_command_buffer*, nk_handle);
};

struct nk_style_chart {
    /* colors */
    struct nk_style_item background;
    struct nk_color border_color;
    struct nk_color selected_color;
    struct nk_color color;

    /* properties */
    float border;
    float rounding;
    struct nk_vec2 padding;
    float color_factor;
    float disabled_factor;
    nk_bool show_markers;
};

struct nk_style_combo {
    /* background */
    struct nk_style_item normal;
    struct nk_style_item hover;
    struct nk_style_item active;
    struct nk_color border_color;

    /* label */
    struct nk_color label_normal;
    struct nk_color label_hover;
    struct nk_color label_active;

    /* symbol */
    struct nk_color symbol_normal;
    struct nk_color symbol_hover;
    struct nk_color symbol_active;

    /* button */
    struct nk_style_button button;
    enum nk_symbol_type sym_normal;
    enum nk_symbol_type sym_hover;
    enum nk_symbol_type sym_active;

    /* properties */
    float border;
    float rounding;
    struct nk_vec2 content_padding;
    struct nk_vec2 button_padding;
    struct nk_vec2 spacing;
    float color_factor;
    float disabled_factor;
};

struct nk_style_tab {
    /* background */
    struct nk_style_item background;
    struct nk_color border_color;
    struct nk_color text;

    /* button */
    struct nk_style_button tab_maximize_button;
    struct nk_style_button tab_minimize_button;
    struct nk_style_button node_maximize_button;
    struct nk_style_button node_minimize_button;
    enum nk_symbol_type sym_minimize;
    enum nk_symbol_type sym_maximize;

    /* properties */
    float border;
    float rounding;
    float indent;
    struct nk_vec2 padding;
    struct nk_vec2 spacing;
    float color_factor;
    float disabled_factor;
};

enum nk_style_header_align {
    NK_HEADER_LEFT,
    NK_HEADER_RIGHT
};
struct nk_style_window_header {
    /* background */
    struct nk_style_item normal;
    struct nk_style_item hover;
    struct nk_style_item active;

    /* button */
    struct nk_style_button close_button;
    struct nk_style_button minimize_button;
    enum nk_symbol_type close_symbol;
    enum nk_symbol_type minimize_symbol;
    enum nk_symbol_type maximize_symbol;

    /* title */
    struct nk_color label_normal;
    struct nk_color label_hover;
    struct nk_color label_active;

    /* properties */
    enum nk_style_header_align align;
    struct nk_vec2 padding;
    struct nk_vec2 label_padding;
    struct nk_vec2 spacing;
};

struct nk_style_window {
    struct nk_style_window_header header;
    struct nk_style_item fixed_background;
    struct nk_color background;

    struct nk_color border_color;
    struct nk_color popup_border_color;
    struct nk_color combo_border_color;
    struct nk_color contextual_border_color;
    struct nk_color menu_border_color;
    struct nk_color group_border_color;
    struct nk_color tooltip_border_color;
    struct nk_style_item scaler;

    float border;
    float combo_border;
    float contextual_border;
    float menu_border;
    float group_border;
    float tooltip_border;
    float popup_border;
    float min_row_height_padding;

    float rounding;
    struct nk_vec2 spacing;
    struct nk_vec2 scrollbar_size;
    struct nk_vec2 min_size;

    struct nk_vec2 padding;
    struct nk_vec2 group_padding;
    struct nk_vec2 popup_padding;
    struct nk_vec2 combo_padding;
    struct nk_vec2 contextual_padding;
    struct nk_vec2 menu_padding;
    struct nk_vec2 tooltip_padding;
};

struct nk_style {
    const struct nk_user_font *font;
    const struct nk_cursor *cursors[NK_CURSOR_COUNT];
    const struct nk_cursor *cursor_active;
    struct nk_cursor *cursor_last;
    int cursor_visible;

    struct nk_style_text text;
    struct nk_style_button button;
    struct nk_style_button contextual_button;
    struct nk_style_button menu_button;
    struct nk_style_toggle option;
    struct nk_style_toggle checkbox;
    struct nk_style_selectable selectable;
    struct nk_style_slider slider;
    struct nk_style_knob knob;
    struct nk_style_progress progress;
    struct nk_style_property property;
    struct nk_style_edit edit;
    struct nk_style_chart chart;
    struct nk_style_scrollbar scrollh;
    struct nk_style_scrollbar scrollv;
    struct nk_style_tab tab;
    struct nk_style_combo combo;
    struct nk_style_window window;
};

NK_API struct nk_style_item nk_style_item_color(struct nk_color);
NK_API struct nk_style_item nk_style_item_image(struct nk_image img);
NK_API struct nk_style_item nk_style_item_nine_slice(struct nk_nine_slice slice);
NK_API struct nk_style_item nk_style_item_hide(void);

/*==============================================================
 *                          PANEL
 * =============================================================*/
#ifndef NK_MAX_LAYOUT_ROW_TEMPLATE_COLUMNS
#define NK_MAX_LAYOUT_ROW_TEMPLATE_COLUMNS 16
#endif
#ifndef NK_CHART_MAX_SLOT
#define NK_CHART_MAX_SLOT 4
#endif

enum nk_panel_type {
    NK_PANEL_NONE       = 0,
    NK_PANEL_WINDOW     = NK_FLAG(0),
    NK_PANEL_GROUP      = NK_FLAG(1),
    NK_PANEL_POPUP      = NK_FLAG(2),
    NK_PANEL_CONTEXTUAL = NK_FLAG(4),
    NK_PANEL_COMBO      = NK_FLAG(5),
    NK_PANEL_MENU       = NK_FLAG(6),
    NK_PANEL_TOOLTIP    = NK_FLAG(7)
};
enum nk_panel_set {
    NK_PANEL_SET_NONBLOCK = NK_PANEL_CONTEXTUAL|NK_PANEL_COMBO|NK_PANEL_MENU|NK_PANEL_TOOLTIP,
    NK_PANEL_SET_POPUP = NK_PANEL_SET_NONBLOCK|NK_PANEL_POPUP,
    NK_PANEL_SET_SUB = NK_PANEL_SET_POPUP|NK_PANEL_GROUP
};

struct nk_chart_slot {
    enum nk_chart_type type;
    struct nk_color color;
    struct nk_color highlight;
    float min, max, range;
    int count;
    struct nk_vec2 last;
    int index;
    nk_bool show_markers;
};

struct nk_chart {
    int slot;
    float x, y, w, h;
    struct nk_chart_slot slots[NK_CHART_MAX_SLOT];
};

enum nk_panel_row_layout_type {
    NK_LAYOUT_DYNAMIC_FIXED = 0,
    NK_LAYOUT_DYNAMIC_ROW,
    NK_LAYOUT_DYNAMIC_FREE,
    NK_LAYOUT_DYNAMIC,
    NK_LAYOUT_STATIC_FIXED,
    NK_LAYOUT_STATIC_ROW,
    NK_LAYOUT_STATIC_FREE,
    NK_LAYOUT_STATIC,
    NK_LAYOUT_TEMPLATE,
    NK_LAYOUT_COUNT
};
struct nk_row_layout {
    enum nk_panel_row_layout_type type;
    int index;
    float height;
    float min_height;
    int columns;
    const float *ratio;
    float item_width;
    float item_height;
    float item_offset;
    float filled;
    struct nk_rect item;
    int tree_depth;
    float templates[NK_MAX_LAYOUT_ROW_TEMPLATE_COLUMNS];
};

struct nk_popup_buffer {
    nk_size begin;
    nk_size parent;
    nk_size last;
    nk_size end;
    nk_bool active;
};

struct nk_menu_state {
    float x, y, w, h;
    struct nk_scroll offset;
};

struct nk_panel {
    enum nk_panel_type type;
    nk_flags flags;
    struct nk_rect bounds;
    nk_uint *offset_x;
    nk_uint *offset_y;
    float at_x, at_y, max_x;
    float footer_height;
    float header_height;
    float border;
    unsigned int has_scrolling;
    struct nk_rect clip;
    struct nk_menu_state menu;
    struct nk_row_layout row;
    struct nk_chart chart;
    struct nk_command_buffer *buffer;
    struct nk_panel *parent;
};

/*==============================================================
 *                          WINDOW
 * =============================================================*/
#ifndef NK_WINDOW_MAX_NAME
#define NK_WINDOW_MAX_NAME 64
#endif

struct nk_table;
enum nk_window_flags {
    NK_WINDOW_PRIVATE       = NK_FLAG(11),
    NK_WINDOW_DYNAMIC       = NK_WINDOW_PRIVATE,
    /* special window type growing up in height while being filled to a certain maximum height */
    NK_WINDOW_ROM           = NK_FLAG(12),
    /* sets window widgets into a read only mode and does not allow input changes */
    NK_WINDOW_NOT_INTERACTIVE = NK_WINDOW_ROM|NK_WINDOW_NO_INPUT,
    /* prevents all interaction caused by input to either window or widgets inside */
    NK_WINDOW_HIDDEN        = NK_FLAG(13),
    /* Hides window and stops any window interaction and drawing */
    NK_WINDOW_CLOSED        = NK_FLAG(14),
    /* Directly closes and frees the window at the end of the frame */
    NK_WINDOW_MINIMIZED     = NK_FLAG(15),
    /* marks the window as minimized */
    NK_WINDOW_REMOVE_ROM    = NK_FLAG(16)
    /* Removes read only mode at the end of the window */
};

struct nk_popup_state {
    struct nk_window *win;
    enum nk_panel_type type;
    struct nk_popup_buffer buf;
    nk_hash name;
    nk_bool active;
    unsigned combo_count;
    unsigned con_count, con_old;
    unsigned active_con;
    struct nk_rect header;
};

struct nk_edit_state {
    nk_hash name;
    unsigned int seq;
    unsigned int old;
    int active, prev;
    int cursor;
    int sel_start;
    int sel_end;
    struct nk_scroll scrollbar;
    unsigned char mode;
    unsigned char single_line;
};

struct nk_property_state {
    int active, prev;
    char buffer[NK_MAX_NUMBER_BUFFER];
    int length;
    int cursor;
    int select_start;
    int select_end;
    nk_hash name;
    unsigned int seq;
    unsigned int old;
    int state;
    int prev_state;
    nk_hash prev_name;
    char prev_buffer[NK_MAX_NUMBER_BUFFER];
    int prev_length;
};

struct nk_window {
    unsigned int seq;
    nk_hash name;
    char name_string[NK_WINDOW_MAX_NAME];
    nk_flags flags;

    struct nk_rect bounds;
    struct nk_scroll scrollbar;
    struct nk_command_buffer buffer;
    struct nk_panel *layout;
    float scrollbar_hiding_timer;

    /* persistent widget state */
    struct nk_property_state property;
    struct nk_popup_state popup;
    struct nk_edit_state edit;
    unsigned int scrolled;
    nk_bool widgets_disabled;

    struct nk_table *tables;
    unsigned int table_count;

    /* window list hooks */
    struct nk_window *next;
    struct nk_window *prev;
    struct nk_window *parent;
};

/*==============================================================
 *                          STACK
 * =============================================================*/
/*/// ### Stack
/// The style modifier stack can be used to temporarily change a
/// property inside `nk_style`. For example if you want a special
/// red button you can temporarily push the old button color onto a stack
/// draw the button with a red color and then you just pop the old color
/// back from the stack:
///
///     nk_style_push_style_item(ctx, &ctx->style.button.normal, nk_style_item_color(nk_rgb(255,0,0)));
///     nk_style_push_style_item(ctx, &ctx->style.button.hover, nk_style_item_color(nk_rgb(255,0,0)));
///     nk_style_push_style_item(ctx, &ctx->style.button.active, nk_style_item_color(nk_rgb(255,0,0)));
///     nk_style_push_vec2(ctx, &cx->style.button.padding, nk_vec2(2,2));
///
///     nk_button(...);
///
///     nk_style_pop_style_item(ctx);
///     nk_style_pop_style_item(ctx);
///     nk_style_pop_style_item(ctx);
///     nk_style_pop_vec2(ctx);
///
/// Nuklear has a stack for style_items, float properties, vector properties,
/// flags, colors, fonts and for button_behavior. Each has it's own fixed size stack
/// which can be changed at compile time.
 */
#ifndef NK_BUTTON_BEHAVIOR_STACK_SIZE
#define NK_BUTTON_BEHAVIOR_STACK_SIZE 8
#endif

#ifndef NK_FONT_STACK_SIZE
#define NK_FONT_STACK_SIZE 8
#endif

#ifndef NK_STYLE_ITEM_STACK_SIZE
#define NK_STYLE_ITEM_STACK_SIZE 16
#endif

#ifndef NK_FLOAT_STACK_SIZE
#define NK_FLOAT_STACK_SIZE 32
#endif

#ifndef NK_VECTOR_STACK_SIZE
#define NK_VECTOR_STACK_SIZE 16
#endif

#ifndef NK_FLAGS_STACK_SIZE
#define NK_FLAGS_STACK_SIZE 32
#endif

#ifndef NK_COLOR_STACK_SIZE
#define NK_COLOR_STACK_SIZE 32
#endif

#define NK_CONFIGURATION_STACK_TYPE(prefix, name, type)\
    struct nk_config_stack_##name##_element {\
        prefix##_##type *address;\
        prefix##_##type old_value;\
    }
#define NK_CONFIG_STACK(type,size)\
    struct nk_config_stack_##type {\
        int head;\
        struct nk_config_stack_##type##_element elements[size];\
    }

#define nk_float float
NK_CONFIGURATION_STACK_TYPE(struct nk, style_item, style_item);
NK_CONFIGURATION_STACK_TYPE(nk ,float, float);
NK_CONFIGURATION_STACK_TYPE(struct nk, vec2, vec2);
NK_CONFIGURATION_STACK_TYPE(nk ,flags, flags);
NK_CONFIGURATION_STACK_TYPE(struct nk, color, color);
NK_CONFIGURATION_STACK_TYPE(const struct nk, user_font, user_font*);
NK_CONFIGURATION_STACK_TYPE(enum nk, button_behavior, button_behavior);

NK_CONFIG_STACK(style_item, NK_STYLE_ITEM_STACK_SIZE);
NK_CONFIG_STACK(float, NK_FLOAT_STACK_SIZE);
NK_CONFIG_STACK(vec2, NK_VECTOR_STACK_SIZE);
NK_CONFIG_STACK(flags, NK_FLAGS_STACK_SIZE);
NK_CONFIG_STACK(color, NK_COLOR_STACK_SIZE);
NK_CONFIG_STACK(user_font, NK_FONT_STACK_SIZE);
NK_CONFIG_STACK(button_behavior, NK_BUTTON_BEHAVIOR_STACK_SIZE);

struct nk_configuration_stacks {
    struct nk_config_stack_style_item style_items;
    struct nk_config_stack_float floats;
    struct nk_config_stack_vec2 vectors;
    struct nk_config_stack_flags flags;
    struct nk_config_stack_color colors;
    struct nk_config_stack_user_font fonts;
    struct nk_config_stack_button_behavior button_behaviors;
};

/*==============================================================
 *                          CONTEXT
 * =============================================================*/
#define NK_VALUE_PAGE_CAPACITY \
    (((NK_MAX(sizeof(struct nk_window),sizeof(struct nk_panel)) / sizeof(nk_uint))) / 2)

struct nk_table {
    unsigned int seq;
    unsigned int size;
    nk_hash keys[NK_VALUE_PAGE_CAPACITY];
    nk_uint values[NK_VALUE_PAGE_CAPACITY];
    struct nk_table *next, *prev;
};

union nk_page_data {
    struct nk_table tbl;
    struct nk_panel pan;
    struct nk_window win;
};

struct nk_page_element {
    union nk_page_data data;
    struct nk_page_element *next;
    struct nk_page_element *prev;
};

struct nk_page {
    unsigned int size;
    struct nk_page *next;
    struct nk_page_element win[1];
};

struct nk_pool {
    struct nk_allocator alloc;
    enum nk_allocation_type type;
    unsigned int page_count;
    struct nk_page *pages;
    struct nk_page_element *freelist;
    unsigned capacity;
    nk_size size;
    nk_size cap;
};

struct nk_context {
/* public: can be accessed freely */
    struct nk_input input;
    struct nk_style style;
    struct nk_buffer memory;
    struct nk_clipboard clip;
    nk_flags last_widget_state;
    enum nk_button_behavior button_behavior;
    struct nk_configuration_stacks stacks;
    float delta_time_seconds;

/* private:
    should only be accessed if you
    know what you are doing */
#ifdef NK_INCLUDE_VERTEX_BUFFER_OUTPUT
    struct nk_draw_list draw_list;
#endif
#ifdef NK_INCLUDE_COMMAND_USERDATA
    nk_handle userdata;
#endif
    /* text editor objects are quite big because of an internal
     * undo/redo stack. Therefore it does not make sense to have one for
     * each window for temporary use cases, so I only provide *one* instance
     * for all windows. This works because the content is cleared anyway */
    struct nk_text_edit text_edit;
    /* draw buffer used for overlay drawing operation like cursor */
    struct nk_command_buffer overlay;

    /* windows */
    int build;
    int use_pool;
    struct nk_pool pool;
    struct nk_window *begin;
    struct nk_window *end;
    struct nk_window *active;
    struct nk_window *current;
    struct nk_page_element *freelist;
    unsigned int count;
    unsigned int seq;
};

/* ==============================================================
 *                          MATH
 * =============================================================== */
#define NK_PI 3.141592654f
#define NK_PI_HALF 1.570796326f
#define NK_UTF_INVALID 0xFFFD
#define NK_MAX_FLOAT_PRECISION 4

#define NK_UNUSED(x) ((void)(x))
#define NK_SATURATE(x) (NK_MAX(0, NK_MIN(1.0f, x)))
#define NK_LEN(a) (sizeof(a)/sizeof(a)[0])
#define NK_ABS(a) (((a) < 0) ? -(a) : (a))
#define NK_BETWEEN(x, a, b) ((a) <= (x) && (x) < (b))
#define NK_INBOX(px, py, x, y, w, h)\
    (NK_BETWEEN(px,x,x+w) && NK_BETWEEN(py,y,y+h))
#define NK_INTERSECT(x0, y0, w0, h0, x1, y1, w1, h1) \
    ((x1 < (x0 + w0)) && (x0 < (x1 + w1)) && \
    (y1 < (y0 + h0)) && (y0 < (y1 + h1)))
#define NK_CONTAINS(x, y, w, h, bx, by, bw, bh)\
    (NK_INBOX(x,y, bx, by, bw, bh) && NK_INBOX(x+w,y+h, bx, by, bw, bh))

#define nk_vec2_sub(a, b) nk_vec2((a).x - (b).x, (a).y - (b).y)
#define nk_vec2_add(a, b) nk_vec2((a).x + (b).x, (a).y + (b).y)
#define nk_vec2_len_sqr(a) ((a).x*(a).x+(a).y*(a).y)
#define nk_vec2_muls(a, t) nk_vec2((a).x * (t), (a).y * (t))

#define nk_ptr_add(t, p, i) ((t*)((void*)((nk_byte*)(p) + (i))))
#define nk_ptr_add_const(t, p, i) ((const t*)((const void*)((const nk_byte*)(p) + (i))))
#define nk_zero_struct(s) nk_zero(&s, sizeof(s))

/* ==============================================================
 *                          ALIGNMENT
 * =============================================================== */
/* Pointer to Integer type conversion for pointer alignment */
#if defined(__PTRDIFF_TYPE__) /* This case should work for GCC*/
# define NK_UINT_TO_PTR(x) ((void*)(__PTRDIFF_TYPE__)(x))
# define NK_PTR_TO_UINT(x) ((nk_size)(__PTRDIFF_TYPE__)(x))
#elif !defined(__GNUC__) /* works for compilers other than LLVM */
# define NK_UINT_TO_PTR(x) ((void*)&((char*)0)[x])
# define NK_PTR_TO_UINT(x) ((nk_size)(((char*)x)-(char*)0))
#elif defined(NK_USE_FIXED_TYPES) /* used if we have <stdint.h> */
# define NK_UINT_TO_PTR(x) ((void*)(uintptr_t)(x))
# define NK_PTR_TO_UINT(x) ((uintptr_t)(x))
#else /* generates warning but works */
# define NK_UINT_TO_PTR(x) ((void*)(x))
# define NK_PTR_TO_UINT(x) ((nk_size)(x))
#endif

#define NK_ALIGN_PTR(x, mask)\
    (NK_UINT_TO_PTR((NK_PTR_TO_UINT((nk_byte*)(x) + (mask-1)) & ~(mask-1))))
#define NK_ALIGN_PTR_BACK(x, mask)\
    (NK_UINT_TO_PTR((NK_PTR_TO_UINT((nk_byte*)(x)) & ~(mask-1))))

#if ((defined(__GNUC__) && __GNUC__ >= 4) || defined(__clang__)) && !defined(EMSCRIPTEN)
#define NK_OFFSETOF(st,m) (__builtin_offsetof(st,m))
#else
#define NK_OFFSETOF(st,m) ((nk_ptr)&(((st*)0)->m))
#endif

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
template<typename T> struct nk_alignof;
template<typename T, int size_diff> struct nk_helper{enum {value = size_diff};};
template<typename T> struct nk_helper<T,0>{enum {value = nk_alignof<T>::value};};
template<typename T> struct nk_alignof{struct Big {T x; char c;}; enum {
    diff = sizeof(Big) - sizeof(T), value = nk_helper<Big, diff>::value};};
#define NK_ALIGNOF(t) (nk_alignof<t>::value)
#else
#define NK_ALIGNOF(t) NK_OFFSETOF(struct {char c; t _h;}, _h)
#endif

#define NK_CONTAINER_OF(ptr,type,member)\
    (type*)((void*)((char*)(1 ? (ptr): &((type*)0)->member) - NK_OFFSETOF(type, member)))



#endif /* NK_NUKLEAR_H_ */

#ifdef NK_IMPLEMENTATION

#ifndef NK_INTERNAL_H
#define NK_INTERNAL_H

#ifndef NK_POOL_DEFAULT_CAPACITY
#define NK_POOL_DEFAULT_CAPACITY 16
#endif

#ifndef NK_DEFAULT_COMMAND_BUFFER_SIZE
#define NK_DEFAULT_COMMAND_BUFFER_SIZE (4*1024)
#endif

#ifndef NK_BUFFER_DEFAULT_INITIAL_SIZE
#define NK_BUFFER_DEFAULT_INITIAL_SIZE (4*1024)
#endif

/* standard library headers */
#ifdef NK_INCLUDE_DEFAULT_ALLOCATOR
#include <stdlib.h> /* malloc, free */
#endif
#ifdef NK_INCLUDE_STANDARD_IO
#include <stdio.h> /* fopen, fclose,... */
#endif
#ifdef NK_INCLUDE_STANDARD_VARARGS
#include <stdarg.h> /* valist, va_start, va_end, ... */
#endif
#ifndef NK_ASSERT
#include <assert.h>
#define NK_ASSERT(expr) assert(expr)
#endif

#define NK_DEFAULT (-1)

#ifndef NK_VSNPRINTF
/* If your compiler does support `vsnprintf` I would highly recommend
 * defining this to vsnprintf instead since `vsprintf` is basically
 * unbelievable unsafe and should *NEVER* be used. But I have to support
 * it since C89 only provides this unsafe version. */
  #if (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)) ||\
      (defined(__cplusplus) && (__cplusplus >= 201103L)) || \
      (defined(_POSIX_C_SOURCE) && (_POSIX_C_SOURCE >= 200112L)) ||\
      (defined(_XOPEN_SOURCE) && (_XOPEN_SOURCE >= 500)) ||\
       defined(_ISOC99_SOURCE) || defined(_BSD_SOURCE)
      #define NK_VSNPRINTF(s,n,f,a) vsnprintf(s,n,f,a)
  #else
    #define NK_VSNPRINTF(s,n,f,a) vsprintf(s,f,a)
  #endif
#endif

#define NK_SCHAR_MIN (-127)
#define NK_SCHAR_MAX 127
#define NK_UCHAR_MIN 0
#define NK_UCHAR_MAX 256
#define NK_SSHORT_MIN (-32767)
#define NK_SSHORT_MAX 32767
#define NK_USHORT_MIN 0
#define NK_USHORT_MAX 65535
#define NK_SINT_MIN (-2147483647)
#define NK_SINT_MAX 2147483647
#define NK_UINT_MIN 0
#define NK_UINT_MAX 4294967295u

/* Make sure correct type size:
 * This will fire with a negative subscript error if the type sizes
 * are set incorrectly by the compiler, and compile out if not */
NK_STATIC_ASSERT(sizeof(nk_size) >= sizeof(void*));
NK_STATIC_ASSERT(sizeof(nk_ptr) == sizeof(void*));
NK_STATIC_ASSERT(sizeof(nk_flags) >= 4);
NK_STATIC_ASSERT(sizeof(nk_rune) >= 4);
NK_STATIC_ASSERT(sizeof(nk_ushort) == 2);
NK_STATIC_ASSERT(sizeof(nk_short) == 2);
NK_STATIC_ASSERT(sizeof(nk_uint) == 4);
NK_STATIC_ASSERT(sizeof(nk_int) == 4);
NK_STATIC_ASSERT(sizeof(nk_byte) == 1);
#ifdef NK_INCLUDE_STANDARD_BOOL
NK_STATIC_ASSERT(sizeof(nk_bool) == sizeof(bool));
#else
NK_STATIC_ASSERT(sizeof(nk_bool) == 4);
#endif

NK_GLOBAL const struct nk_rect nk_null_rect = {-8192.0f, -8192.0f, 16384, 16384};
#define NK_FLOAT_PRECISION 0.00000000000001

NK_GLOBAL const struct nk_color nk_red = {255,0,0,255};
NK_GLOBAL const struct nk_color nk_green = {0,255,0,255};
NK_GLOBAL const struct nk_color nk_blue = {0,0,255,255};
NK_GLOBAL const struct nk_color nk_white = {255,255,255,255};
NK_GLOBAL const struct nk_color nk_black = {0,0,0,255};
NK_GLOBAL const struct nk_color nk_yellow = {255,255,0,255};

/* widget */
#define nk_widget_state_reset(s)\
    if ((*(s)) & NK_WIDGET_STATE_MODIFIED)\
        (*(s)) = NK_WIDGET_STATE_INACTIVE|NK_WIDGET_STATE_MODIFIED;\
    else (*(s)) = NK_WIDGET_STATE_INACTIVE;

/* math */
#ifndef NK_INV_SQRT
NK_LIB float nk_inv_sqrt(float n);
#endif
#ifndef NK_SIN
NK_LIB float nk_sin(float x);
#endif
#ifndef NK_COS
NK_LIB float nk_cos(float x);
#endif
#ifndef NK_ATAN
NK_LIB float nk_atan(float x);
#endif
#ifndef NK_ATAN2
NK_LIB float nk_atan2(float y, float x);
#endif
NK_LIB nk_uint nk_round_up_pow2(nk_uint v);
NK_LIB struct nk_rect nk_shrink_rect(struct nk_rect r, float amount);
NK_LIB struct nk_rect nk_pad_rect(struct nk_rect r, struct nk_vec2 pad);
NK_LIB void nk_unify(struct nk_rect *clip, const struct nk_rect *a, float x0, float y0, float x1, float y1);
#ifndef NK_DTOA
NK_LIB double nk_pow(double x, int n);
NK_LIB int nk_ifloord(double x);
NK_LIB int nk_log10(double n);
#endif
NK_LIB int nk_ifloorf(float x);
NK_LIB int nk_iceilf(float x);
NK_LIB float nk_roundf(float x);

/* util */
enum {NK_DO_NOT_STOP_ON_NEW_LINE, NK_STOP_ON_NEW_LINE};
NK_LIB nk_bool nk_is_lower(int c);
NK_LIB nk_bool nk_is_upper(int c);
NK_LIB int nk_to_upper(int c);
NK_LIB int nk_to_lower(int c);

#ifndef NK_MEMCPY
NK_LIB void* nk_memcopy(void *dst, const void *src, nk_size n);
#endif
#ifndef NK_MEMSET
NK_LIB void nk_memset(void *ptr, int c0, nk_size size);
#endif
NK_LIB void nk_zero(void *ptr, nk_size size);
NK_LIB char *nk_itoa(char *s, long n);
NK_LIB int nk_string_float_limit(char *string, int prec);
#ifndef NK_DTOA
NK_LIB char *nk_dtoa(char *s, double n);
#endif
NK_LIB int nk_text_clamp(const struct nk_user_font *font, const char *text, int text_len, float space, int *glyphs, float *text_width, nk_rune *sep_list, int sep_count);
NK_LIB struct nk_vec2 nk_text_calculate_text_bounds(const struct nk_user_font *font, const char *begin, int byte_len, float row_height, const char **remaining, struct nk_vec2 *out_offset, int *glyphs, int op);
#ifdef NK_INCLUDE_STANDARD_VARARGS
NK_LIB int nk_strfmt(char *buf, int buf_size, const char *fmt, va_list args);
#endif
#ifdef NK_INCLUDE_STANDARD_IO
#ifdef NK_INCLUDE_FONT_BAKING
NK_LIB char *nk_file_load(const char* path, nk_size* siz, const struct nk_allocator *alloc);
#endif
#endif

/* buffer */
#ifdef NK_INCLUDE_DEFAULT_ALLOCATOR
NK_LIB void* nk_malloc(nk_handle unused, void *old,nk_size size);
NK_LIB void nk_mfree(nk_handle unused, void *ptr);
#endif
NK_LIB void* nk_buffer_align(void *unaligned, nk_size align, nk_size *alignment, enum nk_buffer_allocation_type type);
NK_LIB void* nk_buffer_alloc(struct nk_buffer *b, enum nk_buffer_allocation_type type, nk_size size, nk_size align);
NK_LIB void* nk_buffer_realloc(struct nk_buffer *b, nk_size capacity, nk_size *size);

/* draw */
NK_LIB void nk_command_buffer_init(struct nk_command_buffer *cb, struct nk_buffer *b, enum nk_command_clipping clip);
NK_LIB void nk_command_buffer_reset(struct nk_command_buffer *b);
NK_LIB void* nk_command_buffer_push(struct nk_command_buffer* b, enum nk_command_type t, nk_size size);
NK_LIB void nk_draw_symbol(struct nk_command_buffer *out, enum nk_symbol_type type, struct nk_rect content, struct nk_color background, struct nk_color foreground, float border_width, const struct nk_user_font *font);

/* buffering */
NK_LIB void nk_start_buffer(struct nk_context *ctx, struct nk_command_buffer *b);
NK_LIB void nk_start(struct nk_context *ctx, struct nk_window *win);
NK_LIB void nk_start_popup(struct nk_context *ctx, struct nk_window *win);
NK_LIB void nk_finish_popup(struct nk_context *ctx, struct nk_window*);
NK_LIB void nk_finish_buffer(struct nk_context *ctx, struct nk_command_buffer *b);
NK_LIB void nk_finish(struct nk_context *ctx, struct nk_window *w);
NK_LIB void nk_build(struct nk_context *ctx);

/* text editor */
NK_LIB void nk_textedit_clear_state(struct nk_text_edit *state, enum nk_text_edit_type type, nk_plugin_filter filter);
NK_LIB void nk_textedit_click(struct nk_text_edit *state, float x, float y, const struct nk_user_font *font, float row_height);
NK_LIB void nk_textedit_drag(struct nk_text_edit *state, float x, float y, const struct nk_user_font *font, float row_height);
NK_LIB void nk_textedit_key(struct nk_text_edit *state, enum nk_keys key, int shift_mod, const struct nk_user_font *font, float row_height);

/* window */
enum nk_window_insert_location {
    NK_INSERT_BACK, /* inserts window into the back of list (front of screen) */
    NK_INSERT_FRONT /* inserts window into the front of list (back of screen) */
};
NK_LIB void *nk_create_window(struct nk_context *ctx);
NK_LIB void nk_remove_window(struct nk_context*, struct nk_window*);
NK_LIB void nk_free_window(struct nk_context *ctx, struct nk_window *win);
NK_LIB struct nk_window *nk_find_window(struct nk_context *ctx, nk_hash hash, const char *name);
NK_LIB void nk_insert_window(struct nk_context *ctx, struct nk_window *win, enum nk_window_insert_location loc);

/* pool */
NK_LIB void nk_pool_init(struct nk_pool *pool, const struct nk_allocator *alloc, unsigned int capacity);
NK_LIB void nk_pool_free(struct nk_pool *pool);
NK_LIB void nk_pool_init_fixed(struct nk_pool *pool, void *memory, nk_size size);
NK_LIB struct nk_page_element *nk_pool_alloc(struct nk_pool *pool);

/* page-element */
NK_LIB struct nk_page_element* nk_create_page_element(struct nk_context *ctx);
NK_LIB void nk_link_page_element_into_freelist(struct nk_context *ctx, struct nk_page_element *elem);
NK_LIB void nk_free_page_element(struct nk_context *ctx, struct nk_page_element *elem);

/* table */
NK_LIB struct nk_table* nk_create_table(struct nk_context *ctx);
NK_LIB void nk_remove_table(struct nk_window *win, struct nk_table *tbl);
NK_LIB void nk_free_table(struct nk_context *ctx, struct nk_table *tbl);
NK_LIB void nk_push_table(struct nk_window *win, struct nk_table *tbl);
NK_LIB nk_uint *nk_add_value(struct nk_context *ctx, struct nk_window *win, nk_hash name, nk_uint value);
NK_LIB nk_uint *nk_find_value(struct nk_window *win, nk_hash name);

/* panel */
NK_LIB void *nk_create_panel(struct nk_context *ctx);
NK_LIB void nk_free_panel(struct nk_context*, struct nk_panel *pan);
NK_LIB nk_bool nk_panel_has_header(nk_flags flags, const char *title);
NK_LIB struct nk_vec2 nk_panel_get_padding(const struct nk_style *style, enum nk_panel_type type);
NK_LIB float nk_panel_get_border(const struct nk_style *style, nk_flags flags, enum nk_panel_type type);
NK_LIB struct nk_color nk_panel_get_border_color(const struct nk_style *style, enum nk_panel_type type);
NK_LIB nk_bool nk_panel_is_sub(enum nk_panel_type type);
NK_LIB nk_bool nk_panel_is_nonblock(enum nk_panel_type type);
NK_LIB nk_bool nk_panel_begin(struct nk_context *ctx, const char *title, enum nk_panel_type panel_type);
NK_LIB void nk_panel_end(struct nk_context *ctx);

/* layout */
NK_LIB float nk_layout_row_calculate_usable_space(const struct nk_style *style, enum nk_panel_type type, float total_space, int columns);
NK_LIB void nk_panel_layout(const struct nk_context *ctx, struct nk_window *win, float height, int cols);
NK_LIB void nk_row_layout(struct nk_context *ctx, enum nk_layout_format fmt, float height, int cols, int width);
NK_LIB void nk_panel_alloc_row(const struct nk_context *ctx, struct nk_window *win);
NK_LIB void nk_layout_widget_space(struct nk_rect *bounds, const struct nk_context *ctx, struct nk_window *win, int modify);
NK_LIB void nk_panel_alloc_space(struct nk_rect *bounds, const struct nk_context *ctx);
NK_LIB void nk_layout_peek(struct nk_rect *bounds, struct nk_context *ctx);

/* popup */
NK_LIB nk_bool nk_nonblock_begin(struct nk_context *ctx, nk_flags flags, struct nk_rect body, struct nk_rect header, enum nk_panel_type panel_type);

/* text */
struct nk_text {
    struct nk_vec2 padding;
    struct nk_color background;
    struct nk_color text;
};
NK_LIB void nk_widget_text(struct nk_command_buffer *o, struct nk_rect b, const char *string, int len, const struct nk_text *t, nk_flags a, const struct nk_user_font *f);
NK_LIB void nk_widget_text_wrap(struct nk_command_buffer *o, struct nk_rect b, const char *string, int len, const struct nk_text *t, const struct nk_user_font *f);

/* button */
NK_LIB nk_bool nk_button_behavior(nk_flags *state, struct nk_rect r, const struct nk_input *i, enum nk_button_behavior behavior);
NK_LIB const struct nk_style_item* nk_draw_button(struct nk_command_buffer *out, const struct nk_rect *bounds, nk_flags state, const struct nk_style_button *style);
NK_LIB nk_bool nk_do_button(nk_flags *state, struct nk_command_buffer *out, struct nk_rect r, const struct nk_style_button *style, const struct nk_input *in, enum nk_button_behavior behavior, struct nk_rect *content);
NK_LIB void nk_draw_button_text(struct nk_command_buffer *out, const struct nk_rect *bounds, const struct nk_rect *content, nk_flags state, const struct nk_style_button *style, const char *txt, int len, nk_flags text_alignment, const struct nk_user_font *font);
NK_LIB nk_bool nk_do_button_text(nk_flags *state, struct nk_command_buffer *out, struct nk_rect bounds, const char *string, int len, nk_flags align, enum nk_button_behavior behavior, const struct nk_style_button *style, const struct nk_input *in, const struct nk_user_font *font);
NK_LIB void nk_draw_button_symbol(struct nk_command_buffer *out, const struct nk_rect *bounds, const struct nk_rect *content, nk_flags state, const struct nk_style_button *style, enum nk_symbol_type type, const struct nk_user_font *font);
NK_LIB nk_bool nk_do_button_symbol(nk_flags *state, struct nk_command_buffer *out, struct nk_rect bounds, enum nk_symbol_type symbol, enum nk_button_behavior behavior, const struct nk_style_button *style, const struct nk_input *in, const struct nk_user_font *font);
NK_LIB void nk_draw_button_image(struct nk_command_buffer *out, const struct nk_rect *bounds, const struct nk_rect *content, nk_flags state, const struct nk_style_button *style, const struct nk_image *img);
NK_LIB nk_bool nk_do_button_image(nk_flags *state, struct nk_command_buffer *out, struct nk_rect bounds, struct nk_image img, enum nk_button_behavior b, const struct nk_style_button *style, const struct nk_input *in);
NK_LIB void nk_draw_button_text_symbol(struct nk_command_buffer *out, const struct nk_rect *bounds, const struct nk_rect *label, const struct nk_rect *symbol, nk_flags state, const struct nk_style_button *style, const char *str, int len, enum nk_symbol_type type, const struct nk_user_font *font);
NK_LIB nk_bool nk_do_button_text_symbol(nk_flags *state, struct nk_command_buffer *out, struct nk_rect bounds, enum nk_symbol_type symbol, const char *str, int len, nk_flags align, enum nk_button_behavior behavior, const struct nk_style_button *style, const struct nk_user_font *font, const struct nk_input *in);
NK_LIB void nk_draw_button_text_image(struct nk_command_buffer *out, const struct nk_rect *bounds, const struct nk_rect *label, const struct nk_rect *image, nk_flags state, const struct nk_style_button *style, const char *str, int len, const struct nk_user_font *font, const struct nk_image *img);
NK_LIB nk_bool nk_do_button_text_image(nk_flags *state, struct nk_command_buffer *out, struct nk_rect bounds, struct nk_image img, const char* str, int len, nk_flags align, enum nk_button_behavior behavior, const struct nk_style_button *style, const struct nk_user_font *font, const struct nk_input *in);

/* toggle */
enum nk_toggle_type {
    NK_TOGGLE_CHECK,
    NK_TOGGLE_OPTION
};
NK_LIB nk_bool nk_toggle_behavior(const struct nk_input *in, struct nk_rect select, nk_flags *state, nk_bool active);
NK_LIB void nk_draw_checkbox(struct nk_command_buffer *out, nk_flags state, const struct nk_style_toggle *style, nk_bool active, const struct nk_rect *label, const struct nk_rect *selector, const struct nk_rect *cursors, const char *string, int len, const struct nk_user_font *font, nk_flags text_alignment);
NK_LIB void nk_draw_option(struct nk_command_buffer *out, nk_flags state, const struct nk_style_toggle *style, nk_bool active, const struct nk_rect *label, const struct nk_rect *selector, const struct nk_rect *cursors, const char *string, int len, const struct nk_user_font *font, nk_flags text_alignment);
NK_LIB nk_bool nk_do_toggle(nk_flags *state, struct nk_command_buffer *out, struct nk_rect r, nk_bool *active, const char *str, int len, enum nk_toggle_type type, const struct nk_style_toggle *style, const struct nk_input *in, const struct nk_user_font *font, nk_flags widget_alignment, nk_flags text_alignment);

/* progress */
NK_LIB nk_size nk_progress_behavior(nk_flags *state, struct nk_input *in, struct nk_rect r, struct nk_rect cursor, nk_size max, nk_size value, nk_bool modifiable);
NK_LIB void nk_draw_progress(struct nk_command_buffer *out, nk_flags state, const struct nk_style_progress *style, const struct nk_rect *bounds, const struct nk_rect *scursor, nk_size value, nk_size max);
NK_LIB nk_size nk_do_progress(nk_flags *state, struct nk_command_buffer *out, struct nk_rect bounds, nk_size value, nk_size max, nk_bool modifiable, const struct nk_style_progress *style, struct nk_input *in);

/* slider */
NK_LIB float nk_slider_behavior(nk_flags *state, struct nk_rect *logical_cursor, struct nk_rect *visual_cursor, struct nk_input *in, struct nk_rect bounds, float slider_min, float slider_max, float slider_value, float slider_step, float slider_steps);
NK_LIB void nk_draw_slider(struct nk_command_buffer *out, nk_flags state, const struct nk_style_slider *style, const struct nk_rect *bounds, const struct nk_rect *visual_cursor, float min, float value, float max);
NK_LIB float nk_do_slider(nk_flags *state, struct nk_command_buffer *out, struct nk_rect bounds, float min, float val, float max, float step, const struct nk_style_slider *style, struct nk_input *in, const struct nk_user_font *font);

/* scrollbar */
NK_LIB float nk_scrollbar_behavior(nk_flags *state, struct nk_input *in, int has_scrolling, const struct nk_rect *scroll, const struct nk_rect *cursor, const struct nk_rect *empty0, const struct nk_rect *empty1, float scroll_offset, float target, float scroll_step, enum nk_orientation o);
NK_LIB void nk_draw_scrollbar(struct nk_command_buffer *out, nk_flags state, const struct nk_style_scrollbar *style, const struct nk_rect *bounds, const struct nk_rect *scroll);
NK_LIB float nk_do_scrollbarv(nk_flags *state, struct nk_command_buffer *out, struct nk_rect scroll, int has_scrolling, float offset, float target, float step, float button_pixel_inc, const struct nk_style_scrollbar *style, struct nk_input *in, const struct nk_user_font *font);
NK_LIB float nk_do_scrollbarh(nk_flags *state, struct nk_command_buffer *out, struct nk_rect scroll, int has_scrolling, float offset, float target, float step, float button_pixel_inc, const struct nk_style_scrollbar *style, struct nk_input *in, const struct nk_user_font *font);

/* selectable */
NK_LIB void nk_draw_selectable(struct nk_command_buffer *out, nk_flags state, const struct nk_style_selectable *style, nk_bool active, const struct nk_rect *bounds, const struct nk_rect *icon, const struct nk_image *img, enum nk_symbol_type sym, const char *string, int len, nk_flags align, const struct nk_user_font *font);
NK_LIB nk_bool nk_do_selectable(nk_flags *state, struct nk_command_buffer *out, struct nk_rect bounds, const char *str, int len, nk_flags align, nk_bool *value, const struct nk_style_selectable *style, const struct nk_input *in, const struct nk_user_font *font);
NK_LIB nk_bool nk_do_selectable_image(nk_flags *state, struct nk_command_buffer *out, struct nk_rect bounds, const char *str, int len, nk_flags align, nk_bool *value, const struct nk_image *img, const struct nk_style_selectable *style, const struct nk_input *in, const struct nk_user_font *font);

/* edit */
NK_LIB void nk_edit_draw_text(struct nk_command_buffer *out, const struct nk_style_edit *style, float pos_x, float pos_y, float x_offset, const char *text, int byte_len, float row_height, const struct nk_user_font *font, struct nk_color background, struct nk_color foreground, nk_bool is_selected);
NK_LIB nk_flags nk_do_edit(nk_flags *state, struct nk_command_buffer *out, struct nk_rect bounds, nk_flags flags, nk_plugin_filter filter, struct nk_text_edit *edit, const struct nk_style_edit *style, struct nk_input *in, const struct nk_user_font *font);

/* color-picker */
NK_LIB nk_bool nk_color_picker_behavior(nk_flags *state, const struct nk_rect *bounds, const struct nk_rect *matrix, const struct nk_rect *hue_bar, const struct nk_rect *alpha_bar, struct nk_colorf *color, const struct nk_input *in);
NK_LIB void nk_draw_color_picker(struct nk_command_buffer *o, const struct nk_rect *matrix, const struct nk_rect *hue_bar, const struct nk_rect *alpha_bar, struct nk_colorf col);
NK_LIB nk_bool nk_do_color_picker(nk_flags *state, struct nk_command_buffer *out, struct nk_colorf *col, enum nk_color_format fmt, struct nk_rect bounds, struct nk_vec2 padding, const struct nk_input *in, const struct nk_user_font *font);

/* property */
enum nk_property_status {
    NK_PROPERTY_DEFAULT,
    NK_PROPERTY_EDIT,
    NK_PROPERTY_DRAG
};
enum nk_property_filter {
    NK_FILTER_INT,
    NK_FILTER_FLOAT
};
enum nk_property_kind {
    NK_PROPERTY_INT,
    NK_PROPERTY_FLOAT,
    NK_PROPERTY_DOUBLE
};
union nk_property {
    int i;
    float f;
    double d;
};
struct nk_property_variant {
    enum nk_property_kind kind;
    union nk_property value;
    union nk_property min_value;
    union nk_property max_value;
    union nk_property step;
};
NK_LIB struct nk_property_variant nk_property_variant_int(int value, int min_value, int max_value, int step);
NK_LIB struct nk_property_variant nk_property_variant_float(float value, float min_value, float max_value, float step);
NK_LIB struct nk_property_variant nk_property_variant_double(double value, double min_value, double max_value, double step);

NK_LIB void nk_drag_behavior(nk_flags *state, const struct nk_input *in, struct nk_rect drag, struct nk_property_variant *variant, float inc_per_pixel);
NK_LIB void nk_property_behavior(nk_flags *ws, const struct nk_input *in, struct nk_rect property,  struct nk_rect label, struct nk_rect edit, struct nk_rect empty, int *state, struct nk_property_variant *variant, float inc_per_pixel);
NK_LIB void nk_draw_property(struct nk_command_buffer *out, const struct nk_style_property *style, const struct nk_rect *bounds, const struct nk_rect *label, nk_flags state, const char *name, int len, const struct nk_user_font *font);
NK_LIB void nk_do_property(nk_flags *ws, struct nk_command_buffer *out, struct nk_rect property, const char *name, struct nk_property_variant *variant, float inc_per_pixel, char *buffer, int *len, int *state, int *cursor, int *select_begin, int *select_end, const struct nk_style_property *style, enum nk_property_filter filter, struct nk_input *in, const struct nk_user_font *font, struct nk_text_edit *text_edit, enum nk_button_behavior behavior);
NK_LIB void nk_property(struct nk_context *ctx, const char *name, struct nk_property_variant *variant, float inc_per_pixel, const enum nk_property_filter filter);

#ifdef NK_INCLUDE_FONT_BAKING

/**
 * @def NK_NO_STB_RECT_PACK_IMPLEMENTATION
 *
 * When defined, will avoid enabling STB_RECT_PACK_IMPLEMENTATION for when stb_rect_pack.h is already implemented elsewhere.
 */
#ifndef NK_NO_STB_RECT_PACK_IMPLEMENTATION
#define STB_RECT_PACK_IMPLEMENTATION
#endif /* NK_NO_STB_RECT_PACK_IMPLEMENTATION */

/**
 * @def NK_NO_STB_TRUETYPE_IMPLEMENTATION
 *
 * When defined, will avoid enabling STB_TRUETYPE_IMPLEMENTATION for when stb_truetype.h is already implemented elsewhere.
 */
#ifndef NK_NO_STB_TRUETYPE_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION
#endif /* NK_NO_STB_TRUETYPE_IMPLEMENTATION */

/* Allow consumer to define own STBTT_malloc/STBTT_free, and use the font atlas' allocator otherwise */
#ifndef STBTT_malloc
static void*
nk_stbtt_malloc(nk_size size, void *user_data) {
    struct nk_allocator *alloc = (struct nk_allocator *) user_data;
    return alloc->alloc(alloc->userdata, 0, size);
}

static void
nk_stbtt_free(void *ptr, void *user_data) {
    struct nk_allocator *alloc = (struct nk_allocator *) user_data;
    alloc->free(alloc->userdata, ptr);
}

#define STBTT_malloc(x,u)  nk_stbtt_malloc(x,u)
#define STBTT_free(x,u)    nk_stbtt_free(x,u)

#endif /* STBTT_malloc */

#endif /* NK_INCLUDE_FONT_BAKING */

#endif





/* ===============================================================
 *
 *                              MATH
 *
 * ===============================================================*/
/*/// ### Math
///  Since nuklear is supposed to work on all systems providing floating point
///  math without any dependencies I also had to implement my own math functions
///  for sqrt, sin and cos. Since the actual highly accurate implementations for
///  the standard library functions are quite complex and I do not need high
///  precision for my use cases I use approximations.
///
///  Sqrt
///  ----
///  For square root nuklear uses the famous fast inverse square root:
///  https://en.wikipedia.org/wiki/Fast_inverse_square_root with
///  slightly tweaked magic constant. While on today's hardware it is
///  probably not faster it is still fast and accurate enough for
///  nuklear's use cases. IMPORTANT: this requires float format IEEE 754
///
///  Sine/Cosine
///  -----------
///  All constants inside both function are generated Remez's minimax
///  approximations for value range 0...2*PI. The reason why I decided to
///  approximate exactly that range is that nuklear only needs sine and
///  cosine to generate circles which only requires that exact range.
///  In addition I used Remez instead of Taylor for additional precision:
///  www.lolengine.net/blog/2011/12/21/better-function-approximations.
///
///  The tool I used to generate constants for both sine and cosine
///  (it can actually approximate a lot more functions) can be
///  found here: www.lolengine.net/wiki/oss/lolremez
*/
#ifndef NK_INV_SQRT
#define NK_INV_SQRT nk_inv_sqrt
NK_LIB float
nk_inv_sqrt(float n)
{
    float x2;
    const float threehalfs = 1.5f;
    union {nk_uint i; float f;} conv = {0};
    conv.f = n;
    x2 = n * 0.5f;
    conv.i = 0x5f375A84 - (conv.i >> 1);
    conv.f = conv.f * (threehalfs - (x2 * conv.f * conv.f));
    return conv.f;
}
#endif
#ifndef NK_SIN
#define NK_SIN nk_sin
NK_LIB float
nk_sin(float x)
{
    NK_STORAGE const float a0 = +1.91059300966915117e-31f;
    NK_STORAGE const float a1 = +1.00086760103908896f;
    NK_STORAGE const float a2 = -1.21276126894734565e-2f;
    NK_STORAGE const float a3 = -1.38078780785773762e-1f;
    NK_STORAGE const float a4 = -2.67353392911981221e-2f;
    NK_STORAGE const float a5 = +2.08026600266304389e-2f;
    NK_STORAGE const float a6 = -3.03996055049204407e-3f;
    NK_STORAGE const float a7 = +1.38235642404333740e-4f;
    return a0 + x*(a1 + x*(a2 + x*(a3 + x*(a4 + x*(a5 + x*(a6 + x*a7))))));
}
#endif
#ifndef NK_COS
#define NK_COS nk_cos
NK_LIB float
nk_cos(float x)
{
    /* New implementation. Also generated using lolremez. */
    /* Old version significantly deviated from expected results. */
    NK_STORAGE const float a0 = 9.9995999154986614e-1f;
    NK_STORAGE const float a1 = 1.2548995793001028e-3f;
    NK_STORAGE const float a2 = -5.0648546280678015e-1f;
    NK_STORAGE const float a3 = 1.2942246466519995e-2f;
    NK_STORAGE const float a4 = 2.8668384702547972e-2f;
    NK_STORAGE const float a5 = 7.3726485210586547e-3f;
    NK_STORAGE const float a6 = -3.8510875386947414e-3f;
    NK_STORAGE const float a7 = 4.7196604604366623e-4f;
    NK_STORAGE const float a8 = -1.8776444013090451e-5f;
    return a0 + x*(a1 + x*(a2 + x*(a3 + x*(a4 + x*(a5 + x*(a6 + x*(a7 + x*a8)))))));
}
#endif
#ifndef NK_ATAN
#define NK_ATAN nk_atan
NK_LIB float
nk_atan(float x)
{
    /* ./lolremez --progress --float -d 9 -r "0:pi*2" "atan(x)" */
    float u = -1.0989005e-05f;
    NK_ASSERT(x >= 0.0f && "TODO support negative floats");
    u = u * x + 0.00034117949f;
    u = u * x + -0.0044932296f;
    u = u * x + 0.032596264f;
    u = u * x + -0.14088021f;
    u = u * x + 0.36040401f;
    u = u * x + -0.47017866f;
    u = u * x + 0.00050198776f;
    u = u * x + 1.0077682f;
    u = u * x + -0.0004765437f;
    return u;
}
#endif
#ifndef NK_ATAN2
#define NK_ATAN2 nk_atan2
NK_LIB float
nk_atan2(float y, float x)
{
    float ax = NK_ABS(x),
          ay = NK_ABS(y);
    /* 0 = +y +x    1 = -y +x
       2 = +y -x    3 = -y -x */
    nk_uint signs = (y < 0) | ((x < 0) << 1);

    float a;
    if(y == 0.0 && x == 0.0) return 0.0f;
    a = (ay > ax)
        ? NK_PI_HALF - NK_ATAN(ax / ay)
        : NK_ATAN(ay / ax);

    switch(signs){
        case 0: return a;
        case 1: return -a;
        case 2: return -a + NK_PI;
        case 3: return a - NK_PI;
    }
    return 0.0f; /* prevents warning */
}
#endif
NK_LIB nk_uint
nk_round_up_pow2(nk_uint v)
{
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}
#ifndef NK_DTOA
NK_LIB double
nk_pow(double x, int n)
{
    /*  check the sign of n */
    double r = 1;
    int plus = n >= 0;
    n = (plus) ? n : -n;
    while (n > 0) {
        if ((n & 1) == 1)
            r *= x;
        n /= 2;
        x *= x;
    }
    return plus ? r : 1.0 / r;
}
NK_LIB int
nk_ifloord(double x)
{
    x = (double)((int)x - ((x < 0.0) ? 1 : 0));
    return (int)x;
}
#endif
NK_LIB int
nk_ifloorf(float x)
{
    x = (float)((int)x - ((x < 0.0f) ? 1 : 0));
    return (int)x;
}
NK_LIB int
nk_iceilf(float x)
{
    if (x >= 0) {
        int i = (int)x;
        return (x > i) ? i+1: i;
    } else {
        int t = (int)x;
        float r = x - (float)t;
        return (r > 0.0f) ? t+1: t;
    }
}
#ifndef NK_DTOA
NK_LIB int
nk_log10(double n)
{
    int neg;
    int ret;
    int exp = 0;

    neg = (n < 0) ? 1 : 0;
    ret = (neg) ? (int)-n : (int)n;
    while ((ret / 10) > 0) {
        ret /= 10;
        exp++;
    }
    if (neg) exp = -exp;
    return exp;
}
#endif
NK_LIB float
nk_roundf(float x)
{
    return (x >= 0.0) ? nk_ifloorf(x + 0.5) : nk_iceilf(x - 0.5);
}
NK_API struct nk_rect
nk_get_null_rect(void)
{
    return nk_null_rect;
}
NK_API struct nk_rect
nk_rect(float x, float y, float w, float h)
{
    struct nk_rect r;
    r.x = x; r.y = y;
    r.w = w; r.h = h;
    return r;
}
NK_API struct nk_rect
nk_recti(int x, int y, int w, int h)
{
    struct nk_rect r;
    r.x = (float)x;
    r.y = (float)y;
    r.w = (float)w;
    r.h = (float)h;
    return r;
}
NK_API struct nk_rect
nk_recta(struct nk_vec2 pos, struct nk_vec2 size)
{
    return nk_rect(pos.x, pos.y, size.x, size.y);
}
NK_API struct nk_rect
nk_rectv(const float *r)
{
    return nk_rect(r[0], r[1], r[2], r[3]);
}
NK_API struct nk_rect
nk_rectiv(const int *r)
{
    return nk_recti(r[0], r[1], r[2], r[3]);
}
NK_API struct nk_vec2
nk_rect_pos(struct nk_rect r)
{
    struct nk_vec2 ret;
    ret.x = r.x; ret.y = r.y;
    return ret;
}
NK_API struct nk_vec2
nk_rect_size(struct nk_rect r)
{
    struct nk_vec2 ret;
    ret.x = r.w; ret.y = r.h;
    return ret;
}
NK_LIB struct nk_rect
nk_shrink_rect(struct nk_rect r, float amount)
{
    struct nk_rect res;
    r.w = NK_MAX(r.w, 2 * amount);
    r.h = NK_MAX(r.h, 2 * amount);
    res.x = r.x + amount;
    res.y = r.y + amount;
    res.w = r.w - 2 * amount;
    res.h = r.h - 2 * amount;
    return res;
}
NK_LIB struct nk_rect
nk_pad_rect(struct nk_rect r, struct nk_vec2 pad)
{
    r.w = NK_MAX(r.w, 2 * pad.x);
    r.h = NK_MAX(r.h, 2 * pad.y);
    r.x += pad.x; r.y += pad.y;
    r.w -= 2 * pad.x;
    r.h -= 2 * pad.y;
    return r;
}
NK_API struct nk_vec2
nk_vec2(float x, float y)
{
    struct nk_vec2 ret;
    ret.x = x; ret.y = y;
    return ret;
}
NK_API struct nk_vec2
nk_vec2i(int x, int y)
{
    struct nk_vec2 ret;
    ret.x = (float)x;
    ret.y = (float)y;
    return ret;
}
NK_API struct nk_vec2
nk_vec2v(const float *v)
{
    return nk_vec2(v[0], v[1]);
}
NK_API struct nk_vec2
nk_vec2iv(const int *v)
{
    return nk_vec2i(v[0], v[1]);
}
NK_LIB void
nk_unify(struct nk_rect *clip, const struct nk_rect *a, float x0, float y0,
    float x1, float y1)
{
    NK_ASSERT(a);
    NK_ASSERT(clip);
    clip->x = NK_MAX(a->x, x0);
    clip->y = NK_MAX(a->y, y0);
    clip->w = NK_MIN(a->x + a->w, x1) - clip->x;
    clip->h = NK_MIN(a->y + a->h, y1) - clip->y;
    clip->w = NK_MAX(0, clip->w);
    clip->h = NK_MAX(0, clip->h);
}

NK_API void
nk_triangle_from_direction(struct nk_vec2 *result, struct nk_rect r,
    float pad_x, float pad_y, enum nk_heading direction)
{
    float w_half, h_half;
    NK_ASSERT(result);

    r.w = NK_MAX(2 * pad_x, r.w);
    r.h = NK_MAX(2 * pad_y, r.h);
    r.w = r.w - 2 * pad_x;
    r.h = r.h - 2 * pad_y;

    r.x = r.x + pad_x;
    r.y = r.y + pad_y;

    w_half = r.w / 2.0f;
    h_half = r.h / 2.0f;

    if (direction == NK_UP) {
        result[0] = nk_vec2(r.x + w_half, r.y);
        result[1] = nk_vec2(r.x + r.w, r.y + r.h);
        result[2] = nk_vec2(r.x, r.y + r.h);
    } else if (direction == NK_RIGHT) {
        result[0] = nk_vec2(r.x, r.y);
        result[1] = nk_vec2(r.x + r.w, r.y + h_half);
        result[2] = nk_vec2(r.x, r.y + r.h);
    } else if (direction == NK_DOWN) {
        result[0] = nk_vec2(r.x, r.y);
        result[1] = nk_vec2(r.x + r.w, r.y);
        result[2] = nk_vec2(r.x + w_half, r.y + r.h);
    } else {
        result[0] = nk_vec2(r.x, r.y + h_half);
        result[1] = nk_vec2(r.x + r.w, r.y);
        result[2] = nk_vec2(r.x + r.w, r.y + r.h);
    }
}





/* ===============================================================
 *
 *                              UTIL
 *
 * ===============================================================*/
NK_INTERN int nk_str_match_here(const char *regexp, const char *text);
NK_INTERN int nk_str_match_star(int c, const char *regexp, const char *text);
NK_LIB nk_bool nk_is_lower(int c) {return (c >= 'a' && c <= 'z') || (c >= 0xE0 && c <= 0xFF);}
NK_LIB nk_bool nk_is_upper(int c){return (c >= 'A' && c <= 'Z') || (c >= 0xC0 && c <= 0xDF);}
NK_LIB int nk_to_upper(int c) {return (c >= 'a' && c <= 'z') ? (c - ('a' - 'A')) : c;}
NK_LIB int nk_to_lower(int c) {return (c >= 'A' && c <= 'Z') ? (c - ('a' + 'A')) : c;}

#ifndef NK_MEMCPY
#define NK_MEMCPY nk_memcopy
NK_LIB void*
nk_memcopy(void *dst0, const void *src0, nk_size length)
{
    nk_ptr t;
    char *dst = (char*)dst0;
    const char *src = (const char*)src0;
    if (length == 0 || dst == src)
        goto done;

    #define nk_word int
    #define nk_wsize sizeof(nk_word)
    #define nk_wmask (nk_wsize-1)
    #define NK_TLOOP(s) if (t) NK_TLOOP1(s)
    #define NK_TLOOP1(s) do { s; } while (--t)

    if (dst < src) {
        t = (nk_ptr)src; /* only need low bits */
        if ((t | (nk_ptr)dst) & nk_wmask) {
            if ((t ^ (nk_ptr)dst) & nk_wmask || length < nk_wsize)
                t = length;
            else
                t = nk_wsize - (t & nk_wmask);
            length -= t;
            NK_TLOOP1(*dst++ = *src++);
        }
        t = length / nk_wsize;
        NK_TLOOP(*(nk_word*)(void*)dst = *(const nk_word*)(const void*)src;
            src += nk_wsize; dst += nk_wsize);
        t = length & nk_wmask;
        NK_TLOOP(*dst++ = *src++);
    } else {
        src += length;
        dst += length;
        t = (nk_ptr)src;
        if ((t | (nk_ptr)dst) & nk_wmask) {
            if ((t ^ (nk_ptr)dst) & nk_wmask || length <= nk_wsize)
                t = length;
            else
                t &= nk_wmask;
            length -= t;
            NK_TLOOP1(*--dst = *--src);
        }
        t = length / nk_wsize;
        NK_TLOOP(src -= nk_wsize; dst -= nk_wsize;
            *(nk_word*)(void*)dst = *(const nk_word*)(const void*)src);
        t = length & nk_wmask;
        NK_TLOOP(*--dst = *--src);
    }
    #undef nk_word
    #undef nk_wsize
    #undef nk_wmask
    #undef NK_TLOOP
    #undef NK_TLOOP1
done:
    return (dst0);
}
#endif
#ifndef NK_MEMSET
#define NK_MEMSET nk_memset
NK_LIB void
nk_memset(void *ptr, int c0, nk_size size)
{
    #define nk_word unsigned
    #define nk_wsize sizeof(nk_word)
    #define nk_wmask (nk_wsize - 1)
    nk_byte *dst = (nk_byte*)ptr;
    unsigned c = 0;
    nk_size t = 0;

    if ((c = (nk_byte)c0) != 0) {
        c = (c << 8) | c; /* at least 16-bits  */
        if (sizeof(unsigned int) > 2)
            c = (c << 16) | c; /* at least 32-bits*/
    }

    /* too small of a word count */
    dst = (nk_byte*)ptr;
    if (size < 3 * nk_wsize) {
        while (size--) *dst++ = (nk_byte)c0;
        return;
    }

    /* align destination */
    if ((t = NK_PTR_TO_UINT(dst) & nk_wmask) != 0) {
        t = nk_wsize -t;
        size -= t;
        do {
            *dst++ = (nk_byte)c0;
        } while (--t != 0);
    }

    /* fill word */
    t = size / nk_wsize;
    do {
        *(nk_word*)((void*)dst) = c;
        dst += nk_wsize;
    } while (--t != 0);

    /* fill trailing bytes */
    t = (size & nk_wmask);
    if (t != 0) {
        do {
            *dst++ = (nk_byte)c0;
        } while (--t != 0);
    }

    #undef nk_word
    #undef nk_wsize
    #undef nk_wmask
}
#endif
NK_LIB void
nk_zero(void *ptr, nk_size size)
{
    NK_ASSERT(ptr);
    NK_MEMSET(ptr, 0, size);
}
NK_API int
nk_strlen(const char *str)
{
    int siz = 0;
    NK_ASSERT(str);
    while (str && *str++ != '\0') siz++;
    return siz;
}
NK_API int
nk_strtoi(const char *str, char **endptr)
{
    int neg = 1;
    const char *p = str;
    int value = 0;

    NK_ASSERT(str);
    if (!str) return 0;

    /* skip whitespace */
    while (*p == ' ') p++;
    if (*p == '-') {
        neg = -1;
        p++;
    }
    while (*p && *p >= '0' && *p <= '9') {
        value = value * 10 + (int) (*p - '0');
        p++;
    }
    if (endptr)
        *endptr = (char *)p;
    return neg*value;
}
NK_API double
nk_strtod(const char *str, char **endptr)
{
    double m;
    double neg = 1.0;
    char *p = (char *)str;
    double value = 0;
    double number = 0;

    NK_ASSERT(str);
    if (!str) return 0;

    /* skip whitespace */
    while (*p == ' ') p++;
    if (*p == '-') {
        neg = -1.0;
        p++;
    }

    while (*p && *p != '.' && *p != 'e') {
        value = value * 10.0 + (double) (*p - '0');
        p++;
    }

    if (*p == '.') {
        p++;
        for(m = 0.1; *p && *p != 'e'; p++ ) {
            value = value + (double) (*p - '0') * m;
            m *= 0.1;
        }
    }
    if (*p == 'e') {
        int i, pow, div;
        p++;
        if (*p == '-') {
            div = nk_true;
            p++;
        } else if (*p == '+') {
            div = nk_false;
            p++;
        } else div = nk_false;

        for (pow = 0; *p; p++)
            pow = pow * 10 + (int) (*p - '0');

        for (m = 1.0, i = 0; i < pow; i++)
            m *= 10.0;

        if (div)
            value /= m;
        else value *= m;
    }
    number = value * neg;
    if (endptr)
        *endptr = (char *)p;
    return number;
}
NK_API float
nk_strtof(const char *str, char **endptr)
{
    float float_value;
    double double_value;
    double_value = NK_STRTOD(str, endptr);
    float_value = (float)double_value;
    return float_value;
}
NK_API int
nk_stricmp(const char *s1, const char *s2)
{
    nk_int c1,c2,d;
    do {
        c1 = *s1++;
        c2 = *s2++;
        d = c1 - c2;
        while (d) {
            if (c1 <= 'Z' && c1 >= 'A') {
                d += ('a' - 'A');
                if (!d) break;
            }
            if (c2 <= 'Z' && c2 >= 'A') {
                d -= ('a' - 'A');
                if (!d) break;
            }
            return ((d >= 0) << 1) - 1;
        }
    } while (c1);
    return 0;
}
NK_API int
nk_stricmpn(const char *s1, const char *s2, int n)
{
    int c1,c2,d;
    NK_ASSERT(n >= 0);
    do {
        c1 = *s1++;
        c2 = *s2++;
        if (!n--) return 0;

        d = c1 - c2;
        while (d) {
            if (c1 <= 'Z' && c1 >= 'A') {
                d += ('a' - 'A');
                if (!d) break;
            }
            if (c2 <= 'Z' && c2 >= 'A') {
                d -= ('a' - 'A');
                if (!d) break;
            }
            return ((d >= 0) << 1) - 1;
        }
    } while (c1);
    return 0;
}
NK_INTERN int
nk_str_match_here(const char *regexp, const char *text)
{
    if (regexp[0] == '\0')
        return 1;
    if (regexp[1] == '*')
        return nk_str_match_star(regexp[0], regexp+2, text);
    if (regexp[0] == '$' && regexp[1] == '\0')
        return *text == '\0';
    if (*text!='\0' && (regexp[0]=='.' || regexp[0]==*text))
        return nk_str_match_here(regexp+1, text+1);
    return 0;
}
NK_INTERN int
nk_str_match_star(int c, const char *regexp, const char *text)
{
    do {/* a '* matches zero or more instances */
        if (nk_str_match_here(regexp, text))
            return 1;
    } while (*text != '\0' && (*text++ == c || c == '.'));
    return 0;
}
NK_API int
nk_strfilter(const char *text, const char *regexp)
{
    /*
    c    matches any literal character c
    .    matches any single character
    ^    matches the beginning of the input string
    $    matches the end of the input string
    *    matches zero or more occurrences of the previous character*/
    if (regexp[0] == '^')
        return nk_str_match_here(regexp+1, text);
    do {    /* must look even if string is empty */
        if (nk_str_match_here(regexp, text))
            return 1;
    } while (*text++ != '\0');
    return 0;
}
NK_API int
nk_strmatch_fuzzy_text(const char *str, int str_len,
    const char *pattern, int *out_score)
{
    /* Returns true if each character in pattern is found sequentially within str
     * if found then out_score is also set. Score value has no intrinsic meaning.
     * Range varies with pattern. Can only compare scores with same search pattern. */

    /* bonus for adjacent matches */
    #define NK_ADJACENCY_BONUS 5
    /* bonus if match occurs after a separator */
    #define NK_SEPARATOR_BONUS 10
    /* bonus if match is uppercase and prev is lower */
    #define NK_CAMEL_BONUS 10
    /* penalty applied for every letter in str before the first match */
    #define NK_LEADING_LETTER_PENALTY (-3)
    /* maximum penalty for leading letters */
    #define NK_MAX_LEADING_LETTER_PENALTY (-9)
    /* penalty for every letter that doesn't matter */
    #define NK_UNMATCHED_LETTER_PENALTY (-1)

    /* loop variables */
    int score = 0;
    char const * pattern_iter = pattern;
    int str_iter = 0;
    int prev_matched = nk_false;
    int prev_lower = nk_false;
    /* true so if first letter match gets separator bonus*/
    int prev_separator = nk_true;

    /* use "best" matched letter if multiple string letters match the pattern */
    char const * best_letter = 0;
    int best_letter_score = 0;

    /* loop over strings */
    NK_ASSERT(str);
    NK_ASSERT(pattern);
    if (!str || !str_len || !pattern) return 0;
    while (str_iter < str_len)
    {
        const char pattern_letter = *pattern_iter;
        const char str_letter = str[str_iter];

        int next_match = *pattern_iter != '\0' &&
            nk_to_lower(pattern_letter) == nk_to_lower(str_letter);
        int rematch = best_letter && nk_to_upper(*best_letter) == nk_to_upper(str_letter);

        int advanced = next_match && best_letter;
        int pattern_repeat = best_letter && *pattern_iter != '\0';
        pattern_repeat = pattern_repeat &&
            nk_to_lower(*best_letter) == nk_to_lower(pattern_letter);

        if (advanced || pattern_repeat) {
            score += best_letter_score;
            best_letter = 0;
            best_letter_score = 0;
        }

        if (next_match || rematch)
        {
            int new_score = 0;
            /* Apply penalty for each letter before the first pattern match */
            if (pattern_iter == pattern) {
                int count = (int)(&str[str_iter] - str);
                int penalty = NK_LEADING_LETTER_PENALTY * count;
                if (penalty < NK_MAX_LEADING_LETTER_PENALTY)
                    penalty = NK_MAX_LEADING_LETTER_PENALTY;

                score += penalty;
            }

            /* apply bonus for consecutive bonuses */
            if (prev_matched)
                new_score += NK_ADJACENCY_BONUS;

            /* apply bonus for matches after a separator */
            if (prev_separator)
                new_score += NK_SEPARATOR_BONUS;

            /* apply bonus across camel case boundaries */
            if (prev_lower && nk_is_upper(str_letter))
                new_score += NK_CAMEL_BONUS;

            /* update pattern iter IFF the next pattern letter was matched */
            if (next_match)
                ++pattern_iter;

            /* update best letter in str which may be for a "next" letter or a rematch */
            if (new_score >= best_letter_score) {
                /* apply penalty for now skipped letter */
                if (best_letter != 0)
                    score += NK_UNMATCHED_LETTER_PENALTY;

                best_letter = &str[str_iter];
                best_letter_score = new_score;
            }
            prev_matched = nk_true;
        } else {
            score += NK_UNMATCHED_LETTER_PENALTY;
            prev_matched = nk_false;
        }

        /* separators should be more easily defined */
        prev_lower = nk_is_lower(str_letter) != 0;
        prev_separator = str_letter == '_' || str_letter == ' ';

        ++str_iter;
    }

    /* apply score for last match */
    if (best_letter)
        score += best_letter_score;

    /* did not match full pattern */
    if (*pattern_iter != '\0')
        return nk_false;

    if (out_score)
        *out_score = score;
    return nk_true;
}
NK_API int
nk_strmatch_fuzzy_string(char const *str, char const *pattern, int *out_score)
{
    return nk_strmatch_fuzzy_text(str, nk_strlen(str), pattern, out_score);
}
NK_LIB int
nk_string_float_limit(char *string, int prec)
{
    int dot = 0;
    char *c = string;
    while (*c) {
        if (*c == '.') {
            dot = 1;
            c++;
            continue;
        }
        if (dot == (prec+1)) {
            *c = 0;
            break;
        }
        if (dot > 0) dot++;
        c++;
    }
    return (int)(c - string);
}
NK_INTERN void
nk_strrev_ascii(char *s)
{
    int len = nk_strlen(s);
    int end = len / 2;
    int i = 0;
    char t;
    for (; i < end; ++i) {
        t = s[i];
        s[i] = s[len - 1 - i];
        s[len -1 - i] = t;
    }
}
NK_LIB char*
nk_itoa(char *s, long n)
{
    long i = 0;
    if (n == 0) {
        s[i++] = '0';
        s[i] = 0;
        return s;
    }
    if (n < 0) {
        s[i++] = '-';
        n = -n;
    }
    while (n > 0) {
        s[i++] = (char)('0' + (n % 10));
        n /= 10;
    }
    s[i] = 0;
    if (s[0] == '-')
        ++s;

    nk_strrev_ascii(s);
    return s;
}
#ifndef NK_DTOA
#define NK_DTOA nk_dtoa
NK_LIB char*
nk_dtoa(char *s, double n)
{
    int useExp = 0;
    int digit = 0, m = 0, m1 = 0;
    char *c = s;
    int neg = 0;

    NK_ASSERT(s);
    if (!s) return 0;

    if (n == 0.0) {
        s[0] = '0'; s[1] = '\0';
        return s;
    }

    neg = (n < 0);
    if (neg) n = -n;

    /* calculate magnitude */
    m = nk_log10(n);
    useExp = (m >= 14 || (neg && m >= 9) || m <= -9);
    if (neg) *(c++) = '-';

    /* set up for scientific notation */
    if (useExp) {
        if (m < 0)
           m -= 1;
        n = n / (double)nk_pow(10.0, m);
        m1 = m;
        m = 0;
    }
    if (m < 1.0) {
        m = 0;
    }

    /* convert the number */
    while (n > NK_FLOAT_PRECISION || m >= 0) {
        double weight = nk_pow(10.0, m);
        if (weight > 0) {
            double t = (double)n / weight;
            digit = nk_ifloord(t);
            n -= ((double)digit * weight);
            *(c++) = (char)('0' + (char)digit);
        }
        if (m == 0 && n > 0)
            *(c++) = '.';
        m--;
    }

    if (useExp) {
        /* convert the exponent */
        int i, j;
        *(c++) = 'e';
        if (m1 > 0) {
            *(c++) = '+';
        } else {
            *(c++) = '-';
            m1 = -m1;
        }
        m = 0;
        while (m1 > 0) {
            *(c++) = (char)('0' + (char)(m1 % 10));
            m1 /= 10;
            m++;
        }
        c -= m;
        for (i = 0, j = m-1; i<j; i++, j--) {
            /* swap without temporary */
            c[i] ^= c[j];
            c[j] ^= c[i];
            c[i] ^= c[j];
        }
        c += m;
    }
    *(c) = '\0';
    return s;
}
#endif
#ifdef NK_INCLUDE_STANDARD_VARARGS
#ifndef NK_INCLUDE_STANDARD_IO
NK_INTERN int
nk_vsnprintf(char *buf, int buf_size, const char *fmt, va_list args)
{
    enum nk_arg_type {
        NK_ARG_TYPE_CHAR,
        NK_ARG_TYPE_SHORT,
        NK_ARG_TYPE_DEFAULT,
        NK_ARG_TYPE_LONG
    };
    enum nk_arg_flags {
        NK_ARG_FLAG_LEFT = 0x01,
        NK_ARG_FLAG_PLUS = 0x02,
        NK_ARG_FLAG_SPACE = 0x04,
        NK_ARG_FLAG_NUM = 0x10,
        NK_ARG_FLAG_ZERO = 0x20
    };

    char number_buffer[NK_MAX_NUMBER_BUFFER];
    enum nk_arg_type arg_type = NK_ARG_TYPE_DEFAULT;
    int precision = NK_DEFAULT;
    int width = NK_DEFAULT;
    nk_flags flag = 0;

    int len = 0;
    int result = -1;
    const char *iter = fmt;

    NK_ASSERT(buf);
    NK_ASSERT(buf_size);
    if (!buf || !buf_size || !fmt) return 0;
    for (iter = fmt; *iter && len < buf_size; iter++) {
        /* copy all non-format characters */
        while (*iter && (*iter != '%') && (len < buf_size))
            buf[len++] = *iter++;
        if (!(*iter) || len >= buf_size) break;
        iter++;

        /* flag arguments */
        while (*iter) {
            if (*iter == '-') flag |= NK_ARG_FLAG_LEFT;
            else if (*iter == '+') flag |= NK_ARG_FLAG_PLUS;
            else if (*iter == ' ') flag |= NK_ARG_FLAG_SPACE;
            else if (*iter == '#') flag |= NK_ARG_FLAG_NUM;
            else if (*iter == '0') flag |= NK_ARG_FLAG_ZERO;
            else break;
            iter++;
        }

        /* width argument */
        width = NK_DEFAULT;
        if (*iter >= '1' && *iter <= '9') {
            const char *end;
            width = nk_strtoi(iter, &end);
            if (end == iter)
                width = -1;
            else iter = end;
        } else if (*iter == '*') {
            width = va_arg(args, int);
            iter++;
        }

        /* precision argument */
        precision = NK_DEFAULT;
        if (*iter == '.') {
            iter++;
            if (*iter == '*') {
                precision = va_arg(args, int);
                iter++;
            } else {
                const char *end;
                precision = nk_strtoi(iter, &end);
                if (end == iter)
                    precision = -1;
                else iter = end;
            }
        }

        /* length modifier */
        if (*iter == 'h') {
            if (*(iter+1) == 'h') {
                arg_type = NK_ARG_TYPE_CHAR;
                iter++;
            } else arg_type = NK_ARG_TYPE_SHORT;
            iter++;
        } else if (*iter == 'l') {
            arg_type = NK_ARG_TYPE_LONG;
            iter++;
        } else arg_type = NK_ARG_TYPE_DEFAULT;

        /* specifier */
        if (*iter == '%') {
            NK_ASSERT(arg_type == NK_ARG_TYPE_DEFAULT);
            NK_ASSERT(precision == NK_DEFAULT);
            NK_ASSERT(width == NK_DEFAULT);
            if (len < buf_size)
                buf[len++] = '%';
        } else if (*iter == 's') {
            /* string  */
            const char *str = va_arg(args, const char*);
            NK_ASSERT(str != buf && "buffer and argument are not allowed to overlap!");
            NK_ASSERT(arg_type == NK_ARG_TYPE_DEFAULT);
            NK_ASSERT(precision == NK_DEFAULT);
            NK_ASSERT(width == NK_DEFAULT);
            if (str == buf) return -1;
            while (str && *str && len < buf_size)
                buf[len++] = *str++;
        } else if (*iter == 'n') {
            /* current length callback */
            signed int *n = va_arg(args, int*);
            NK_ASSERT(arg_type == NK_ARG_TYPE_DEFAULT);
            NK_ASSERT(precision == NK_DEFAULT);
            NK_ASSERT(width == NK_DEFAULT);
            if (n) *n = len;
        } else if (*iter == 'c' || *iter == 'i' || *iter == 'd') {
            /* signed integer */
            long value = 0;
            const char *num_iter;
            int num_len, num_print, padding;
            int cur_precision = NK_MAX(precision, 1);
            int cur_width = NK_MAX(width, 0);

            /* retrieve correct value type */
            if (arg_type == NK_ARG_TYPE_CHAR)
                value = (signed char)va_arg(args, int);
            else if (arg_type == NK_ARG_TYPE_SHORT)
                value = (signed short)va_arg(args, int);
            else if (arg_type == NK_ARG_TYPE_LONG)
                value = va_arg(args, signed long);
            else if (*iter == 'c')
                value = (unsigned char)va_arg(args, int);
            else value = va_arg(args, signed int);

            /* convert number to string */
            nk_itoa(number_buffer, value);
            num_len = nk_strlen(number_buffer);
            padding = NK_MAX(cur_width - NK_MAX(cur_precision, num_len), 0);
            if ((flag & NK_ARG_FLAG_PLUS) || (flag & NK_ARG_FLAG_SPACE))
                padding = NK_MAX(padding-1, 0);

            /* fill left padding up to a total of `width` characters */
            if (!(flag & NK_ARG_FLAG_LEFT)) {
                while (padding-- > 0 && (len < buf_size)) {
                    if ((flag & NK_ARG_FLAG_ZERO) && (precision == NK_DEFAULT))
                        buf[len++] = '0';
                    else buf[len++] = ' ';
                }
            }

            /* copy string value representation into buffer */
            if ((flag & NK_ARG_FLAG_PLUS) && value >= 0 && len < buf_size)
                buf[len++] = '+';
            else if ((flag & NK_ARG_FLAG_SPACE) && value >= 0 && len < buf_size)
                buf[len++] = ' ';

            /* fill up to precision number of digits with '0' */
            num_print = NK_MAX(cur_precision, num_len);
            while (precision && (num_print > num_len) && (len < buf_size)) {
                buf[len++] = '0';
                num_print--;
            }

            /* copy string value representation into buffer */
            num_iter = number_buffer;
            while (precision && *num_iter && len < buf_size)
                buf[len++] = *num_iter++;

            /* fill right padding up to width characters */
            if (flag & NK_ARG_FLAG_LEFT) {
                while ((padding-- > 0) && (len < buf_size))
                    buf[len++] = ' ';
            }
        } else if (*iter == 'o' || *iter == 'x' || *iter == 'X' || *iter == 'u') {
            /* unsigned integer */
            unsigned long value = 0;
            int num_len = 0, num_print, padding = 0;
            int cur_precision = NK_MAX(precision, 1);
            int cur_width = NK_MAX(width, 0);
            unsigned int base = (*iter == 'o') ? 8: (*iter == 'u')? 10: 16;

            /* print oct/hex/dec value */
            const char *upper_output_format = "0123456789ABCDEF";
            const char *lower_output_format = "0123456789abcdef";
            const char *output_format = (*iter == 'x') ?
                lower_output_format: upper_output_format;

            /* retrieve correct value type */
            if (arg_type == NK_ARG_TYPE_CHAR)
                value = (unsigned char)va_arg(args, int);
            else if (arg_type == NK_ARG_TYPE_SHORT)
                value = (unsigned short)va_arg(args, int);
            else if (arg_type == NK_ARG_TYPE_LONG)
                value = va_arg(args, unsigned long);
            else value = va_arg(args, unsigned int);

            do {
                /* convert decimal number into hex/oct number */
                int digit = output_format[value % base];
                if (num_len < NK_MAX_NUMBER_BUFFER)
                    number_buffer[num_len++] = (char)digit;
                value /= base;
            } while (value > 0);

            num_print = NK_MAX(cur_precision, num_len);
            padding = NK_MAX(cur_width - NK_MAX(cur_precision, num_len), 0);
            if (flag & NK_ARG_FLAG_NUM)
                padding = NK_MAX(padding-1, 0);

            /* fill left padding up to a total of `width` characters */
            if (!(flag & NK_ARG_FLAG_LEFT)) {
                while ((padding-- > 0) && (len < buf_size)) {
                    if ((flag & NK_ARG_FLAG_ZERO) && (precision == NK_DEFAULT))
                        buf[len++] = '0';
                    else buf[len++] = ' ';
                }
            }

            /* fill up to precision number of digits */
            if (num_print && (flag & NK_ARG_FLAG_NUM)) {
                if ((*iter == 'o') && (len < buf_size)) {
                    buf[len++] = '0';
                } else if ((*iter == 'x') && ((len+1) < buf_size)) {
                    buf[len++] = '0';
                    buf[len++] = 'x';
                } else if ((*iter == 'X') && ((len+1) < buf_size)) {
                    buf[len++] = '0';
                    buf[len++] = 'X';
                }
            }
            while (precision && (num_print > num_len) && (len < buf_size)) {
                buf[len++] = '0';
                num_print--;
            }

            /* reverse number direction */
            while (num_len > 0) {
                if (precision && (len < buf_size))
                    buf[len++] = number_buffer[num_len-1];
                num_len--;
            }

            /* fill right padding up to width characters */
            if (flag & NK_ARG_FLAG_LEFT) {
                while ((padding-- > 0) && (len < buf_size))
                    buf[len++] = ' ';
            }
        } else if (*iter == 'f') {
            /* floating point */
            const char *num_iter;
            int cur_precision = (precision < 0) ? 6: precision;
            int prefix, cur_width = NK_MAX(width, 0);
            double value = va_arg(args, double);
            int num_len = 0, frac_len = 0, dot = 0;
            int padding = 0;

            NK_ASSERT(arg_type == NK_ARG_TYPE_DEFAULT);
            NK_DTOA(number_buffer, value);
            num_len = nk_strlen(number_buffer);

            /* calculate padding */
            num_iter = number_buffer;
            while (*num_iter && *num_iter != '.')
                num_iter++;

            prefix = (*num_iter == '.')?(int)(num_iter - number_buffer)+1:0;
            padding = NK_MAX(cur_width - (prefix + NK_MIN(cur_precision, num_len - prefix)) , 0);
            if ((flag & NK_ARG_FLAG_PLUS) || (flag & NK_ARG_FLAG_SPACE))
                padding = NK_MAX(padding-1, 0);

            /* fill left padding up to a total of `width` characters */
            if (!(flag & NK_ARG_FLAG_LEFT)) {
                while (padding-- > 0 && (len < buf_size)) {
                    if (flag & NK_ARG_FLAG_ZERO)
                        buf[len++] = '0';
                    else buf[len++] = ' ';
                }
            }

            /* copy string value representation into buffer */
            num_iter = number_buffer;
            if ((flag & NK_ARG_FLAG_PLUS) && (value >= 0) && (len < buf_size))
                buf[len++] = '+';
            else if ((flag & NK_ARG_FLAG_SPACE) && (value >= 0) && (len < buf_size))
                buf[len++] = ' ';
            while (*num_iter) {
                if (dot) frac_len++;
                if (len < buf_size)
                    buf[len++] = *num_iter;
                if (*num_iter == '.') dot = 1;
                if (frac_len >= cur_precision) break;
                num_iter++;
            }

            /* fill number up to precision */
            while (frac_len < cur_precision) {
                if (!dot && len < buf_size) {
                    buf[len++] = '.';
                    dot = 1;
                }
                if (len < buf_size)
                    buf[len++] = '0';
                frac_len++;
            }

            /* fill right padding up to width characters */
            if (flag & NK_ARG_FLAG_LEFT) {
                while ((padding-- > 0) && (len < buf_size))
                    buf[len++] = ' ';
            }
        } else {
            /* Specifier not supported: g,G,e,E,p,z */
            NK_ASSERT(0 && "specifier is not supported!");
            return result;
        }
    }
    buf[(len >= buf_size)?(buf_size-1):len] = 0;
    result = (len >= buf_size)?-1:len;
    return result;
}
#endif
NK_LIB int
nk_strfmt(char *buf, int buf_size, const char *fmt, va_list args)
{
    int result = -1;
    NK_ASSERT(buf);
    NK_ASSERT(buf_size);
    if (!buf || !buf_size || !fmt) return 0;
#ifdef NK_INCLUDE_STANDARD_IO
    result = NK_VSNPRINTF(buf, (nk_size)buf_size, fmt, args);
    result = (result >= buf_size) ? -1: result;
    buf[buf_size-1] = 0;
#else
    result = nk_vsnprintf(buf, buf_size, fmt, args);
#endif
    return result;
}
#endif
NK_API nk_hash
nk_murmur_hash(const void * key, int len, nk_hash seed)
{
    /* 32-Bit MurmurHash3: https://code.google.com/p/smhasher/wiki/MurmurHash3*/
    #define NK_ROTL(x,r) ((x) << (r) | ((x) >> (32 - r)))

    nk_uint h1 = seed;
    nk_uint k1;
    const nk_byte *data = (const nk_byte*)key;
    const nk_byte *keyptr = data;
    nk_byte *k1ptr;
    const int bsize = sizeof(k1);
    const int nblocks = len/4;

    const nk_uint c1 = 0xcc9e2d51;
    const nk_uint c2 = 0x1b873593;
    const nk_byte *tail;
    int i;

    /* body */
    if (!key) return 0;
    for (i = 0; i < nblocks; ++i, keyptr += bsize) {
        k1ptr = (nk_byte*)&k1;
        k1ptr[0] = keyptr[0];
        k1ptr[1] = keyptr[1];
        k1ptr[2] = keyptr[2];
        k1ptr[3] = keyptr[3];

        k1 *= c1;
        k1 = NK_ROTL(k1,15);
        k1 *= c2;

        h1 ^= k1;
        h1 = NK_ROTL(h1,13);
        h1 = h1*5+0xe6546b64;
    }

    /* tail */
    tail = (const nk_byte*)(data + nblocks*4);
    k1 = 0;
    switch (len & 3) {
        case 3: k1 ^= (nk_uint)(tail[2] << 16); /* fallthrough */
        case 2: k1 ^= (nk_uint)(tail[1] << 8u); /* fallthrough */
        case 1: k1 ^= tail[0];
            k1 *= c1;
            k1 = NK_ROTL(k1,15);
            k1 *= c2;
            h1 ^= k1;
            break;
        default: break;
    }

    /* finalization */
    h1 ^= (nk_uint)len;
    /* fmix32 */
    h1 ^= h1 >> 16;
    h1 *= 0x85ebca6b;
    h1 ^= h1 >> 13;
    h1 *= 0xc2b2ae35;
    h1 ^= h1 >> 16;

    #undef NK_ROTL
    return h1;
}
#ifdef NK_INCLUDE_STANDARD_IO
#ifdef NK_INCLUDE_FONT_BAKING
NK_LIB char*
nk_file_load(const char* path, nk_size* siz, const struct nk_allocator *alloc)
{
    char *buf;
    FILE *fd;
    long ret;

    NK_ASSERT(path);
    NK_ASSERT(siz);
    NK_ASSERT(alloc);
    if (!path || !siz || !alloc)
        return 0;

    fd = fopen(path, "rb");
    if (!fd) return 0;
    fseek(fd, 0, SEEK_END);
    ret = ftell(fd);
    if (ret < 0) {
        fclose(fd);
        return 0;
    }
    *siz = (nk_size)ret;
    fseek(fd, 0, SEEK_SET);
    buf = (char*)alloc->alloc(alloc->userdata,0, *siz);
    NK_ASSERT(buf);
    if (!buf) {
        fclose(fd);
        return 0;
    }
    *siz = (nk_size)fread(buf, 1,*siz, fd);
    fclose(fd);
    return buf;
}
#endif
#endif
NK_LIB int
nk_text_clamp(const struct nk_user_font *font, const char *text,
    int text_len, float space, int *glyphs, float *text_width,
    nk_rune *sep_list, int sep_count)
{
    int i = 0;
    int glyph_len = 0;
    float last_width = 0;
    nk_rune unicode = 0;
    float width = 0;
    int len = 0;
    int g = 0;
    float s;

    int sep_len = 0;
    int sep_g = 0;
    float sep_width = 0;
    sep_count = NK_MAX(sep_count,0);

    glyph_len = nk_utf_decode(text, &unicode, text_len);
    while (glyph_len && (width < space) && (len < text_len)) {
        len += glyph_len;
        s = font->width(font->userdata, font->height, text, len);
        for (i = 0; i < sep_count; ++i) {
            if (unicode != sep_list[i]) continue;
            sep_width = last_width = width;
            sep_g = g+1;
            sep_len = len;
            break;
        }
        if (i == sep_count){
            last_width = sep_width = width;
            sep_g = g+1;
        }
        width = s;
        glyph_len = nk_utf_decode(&text[len], &unicode, text_len - len);
        g++;
        if(unicode == '\n')
        {
          len += glyph_len;
          sep_width = last_width = width;
          sep_g = g;
          sep_len = len;
          *glyphs = sep_g;
          *text_width = sep_width;
          assert(sep_len > 0);
          return sep_len-1; // - 1 because the last one is the separator
        }
    }
    if (len >= text_len) {
        *glyphs = g;
        *text_width = last_width;
        return len; // last one is a good character
    } else {
        *glyphs = sep_g;
        *text_width = sep_width;
        return ((!sep_len) ? len: sep_len) - 1; // last one is a separator
    }
}
NK_LIB struct nk_vec2
nk_text_calculate_text_bounds(const struct nk_user_font *font,
    const char *begin, int byte_len, float row_height, const char **remaining,
    struct nk_vec2 *out_offset, int *glyphs, int op)
{
    float line_height = row_height;
    struct nk_vec2 text_size = nk_vec2(0,0);
    float line_width = 0.0f;

    float glyph_width;
    int glyph_len = 0;
    nk_rune unicode = 0;
    int text_len = 0;
    if (!begin || byte_len <= 0 || !font)
        return nk_vec2(0,row_height);

    glyph_len = nk_utf_decode(begin, &unicode, byte_len);
    if (!glyph_len) return text_size;
    glyph_width = font->width(font->userdata, font->height, begin, glyph_len);

    *glyphs = 0;
    while ((text_len < byte_len) && glyph_len) {
        if (unicode == '\n') {
            text_size.x = NK_MAX(text_size.x, line_width);
            text_size.y += line_height;
            line_width = 0;
            *glyphs+=1;
            if (op == NK_STOP_ON_NEW_LINE)
                break;

            text_len++;
            glyph_len = nk_utf_decode(begin + text_len, &unicode, byte_len-text_len);
            continue;
        }

        if (unicode == '\r') {
            text_len++;
            *glyphs+=1;
            glyph_len = nk_utf_decode(begin + text_len, &unicode, byte_len-text_len);
            continue;
        }

        *glyphs = *glyphs + 1;
        text_len += glyph_len;
        line_width += (float)glyph_width;
        glyph_len = nk_utf_decode(begin + text_len, &unicode, byte_len-text_len);
        glyph_width = font->width(font->userdata, font->height, begin+text_len, glyph_len);
        continue;
    }

    if (text_size.x < line_width)
        text_size.x = line_width;
    if (out_offset)
        *out_offset = nk_vec2(line_width, text_size.y + line_height);
    if (line_width > 0 || text_size.y == 0.0f)
        text_size.y += line_height;
    if (remaining)
        *remaining = begin+text_len;
    return text_size;
}





/* ==============================================================
 *
 *                          COLOR
 *
 * ===============================================================*/
NK_INTERN int
nk_parse_hex(const char *p, int length)
{
    int i = 0;
    int len = 0;
    while (len < length) {
        i <<= 4;
        if (p[len] >= 'a' && p[len] <= 'f')
            i += ((p[len] - 'a') + 10);
        else if (p[len] >= 'A' && p[len] <= 'F')
            i += ((p[len] - 'A') + 10);
        else i += (p[len] - '0');
        len++;
    }
    return i;
}
NK_API struct nk_color
nk_rgb_factor(struct nk_color col, const float factor)
{
    if (factor == 1.0f)
        return col;
    col.r = (nk_byte)(col.r * factor);
    col.g = (nk_byte)(col.g * factor);
    col.b = (nk_byte)(col.b * factor);
    return col;
}
NK_API struct nk_color
nk_rgba(int r, int g, int b, int a)
{
    struct nk_color ret;
    ret.r = (nk_byte)NK_CLAMP(0, r, 255);
    ret.g = (nk_byte)NK_CLAMP(0, g, 255);
    ret.b = (nk_byte)NK_CLAMP(0, b, 255);
    ret.a = (nk_byte)NK_CLAMP(0, a, 255);
    return ret;
}
NK_API struct nk_color
nk_rgb_hex(const char *rgb)
{
    struct nk_color col;
    const char *c = rgb;
    if (*c == '#') c++;
    col.r = (nk_byte)nk_parse_hex(c, 2);
    col.g = (nk_byte)nk_parse_hex(c+2, 2);
    col.b = (nk_byte)nk_parse_hex(c+4, 2);
    col.a = 255;
    return col;
}
NK_API struct nk_color
nk_rgba_hex(const char *rgb)
{
    struct nk_color col;
    const char *c = rgb;
    if (*c == '#') c++;
    col.r = (nk_byte)nk_parse_hex(c, 2);
    col.g = (nk_byte)nk_parse_hex(c+2, 2);
    col.b = (nk_byte)nk_parse_hex(c+4, 2);
    col.a = (nk_byte)nk_parse_hex(c+6, 2);
    return col;
}
NK_API void
nk_color_hex_rgba(char *output, struct nk_color col)
{
    #define NK_TO_HEX(i) ((i) <= 9 ? '0' + (i): 'A' - 10 + (i))
    output[0] = (char)NK_TO_HEX((col.r & 0xF0) >> 4);
    output[1] = (char)NK_TO_HEX((col.r & 0x0F));
    output[2] = (char)NK_TO_HEX((col.g & 0xF0) >> 4);
    output[3] = (char)NK_TO_HEX((col.g & 0x0F));
    output[4] = (char)NK_TO_HEX((col.b & 0xF0) >> 4);
    output[5] = (char)NK_TO_HEX((col.b & 0x0F));
    output[6] = (char)NK_TO_HEX((col.a & 0xF0) >> 4);
    output[7] = (char)NK_TO_HEX((col.a & 0x0F));
    output[8] = '\0';
    #undef NK_TO_HEX
}
NK_API void
nk_color_hex_rgb(char *output, struct nk_color col)
{
    #define NK_TO_HEX(i) ((i) <= 9 ? '0' + (i): 'A' - 10 + (i))
    output[0] = (char)NK_TO_HEX((col.r & 0xF0) >> 4);
    output[1] = (char)NK_TO_HEX((col.r & 0x0F));
    output[2] = (char)NK_TO_HEX((col.g & 0xF0) >> 4);
    output[3] = (char)NK_TO_HEX((col.g & 0x0F));
    output[4] = (char)NK_TO_HEX((col.b & 0xF0) >> 4);
    output[5] = (char)NK_TO_HEX((col.b & 0x0F));
    output[6] = '\0';
    #undef NK_TO_HEX
}
NK_API struct nk_color
nk_rgba_iv(const int *c)
{
    return nk_rgba(c[0], c[1], c[2], c[3]);
}
NK_API struct nk_color
nk_rgba_bv(const nk_byte *c)
{
    return nk_rgba(c[0], c[1], c[2], c[3]);
}
NK_API struct nk_color
nk_rgb(int r, int g, int b)
{
    struct nk_color ret;
    ret.r = (nk_byte)NK_CLAMP(0, r, 255);
    ret.g = (nk_byte)NK_CLAMP(0, g, 255);
    ret.b = (nk_byte)NK_CLAMP(0, b, 255);
    ret.a = (nk_byte)255;
    return ret;
}
NK_API struct nk_color
nk_rgb_iv(const int *c)
{
    return nk_rgb(c[0], c[1], c[2]);
}
NK_API struct nk_color
nk_rgb_bv(const nk_byte* c)
{
    return nk_rgb(c[0], c[1], c[2]);
}
NK_API struct nk_color
nk_rgba_u32(nk_uint in)
{
    struct nk_color ret;
    ret.r = (in & 0xFF);
    ret.g = ((in >> 8) & 0xFF);
    ret.b = ((in >> 16) & 0xFF);
    ret.a = (nk_byte)((in >> 24) & 0xFF);
    return ret;
}
NK_API struct nk_color
nk_rgba_f(float r, float g, float b, float a)
{
    struct nk_color ret;
    ret.r = (nk_byte)(NK_SATURATE(r) * 255.0f);
    ret.g = (nk_byte)(NK_SATURATE(g) * 255.0f);
    ret.b = (nk_byte)(NK_SATURATE(b) * 255.0f);
    ret.a = (nk_byte)(NK_SATURATE(a) * 255.0f);
    return ret;
}
NK_API struct nk_color
nk_rgba_fv(const float *c)
{
    return nk_rgba_f(c[0], c[1], c[2], c[3]);
}
NK_API struct nk_color
nk_rgba_cf(struct nk_colorf c)
{
    return nk_rgba_f(c.r, c.g, c.b, c.a);
}
NK_API struct nk_color
nk_rgb_f(float r, float g, float b)
{
    struct nk_color ret;
    ret.r = (nk_byte)(NK_SATURATE(r) * 255.0f);
    ret.g = (nk_byte)(NK_SATURATE(g) * 255.0f);
    ret.b = (nk_byte)(NK_SATURATE(b) * 255.0f);
    ret.a = 255;
    return ret;
}
NK_API struct nk_color
nk_rgb_fv(const float *c)
{
    return nk_rgb_f(c[0], c[1], c[2]);
}
NK_API struct nk_color
nk_rgb_cf(struct nk_colorf c)
{
    return nk_rgb_f(c.r, c.g, c.b);
}
NK_API struct nk_color
nk_hsv(int h, int s, int v)
{
    return nk_hsva(h, s, v, 255);
}
NK_API struct nk_color
nk_hsv_iv(const int *c)
{
    return nk_hsv(c[0], c[1], c[2]);
}
NK_API struct nk_color
nk_hsv_bv(const nk_byte *c)
{
    return nk_hsv(c[0], c[1], c[2]);
}
NK_API struct nk_color
nk_hsv_f(float h, float s, float v)
{
    return nk_hsva_f(h, s, v, 1.0f);
}
NK_API struct nk_color
nk_hsv_fv(const float *c)
{
    return nk_hsv_f(c[0], c[1], c[2]);
}
NK_API struct nk_color
nk_hsva(int h, int s, int v, int a)
{
    float hf = ((float)NK_CLAMP(0, h, 255)) / 255.0f;
    float sf = ((float)NK_CLAMP(0, s, 255)) / 255.0f;
    float vf = ((float)NK_CLAMP(0, v, 255)) / 255.0f;
    float af = ((float)NK_CLAMP(0, a, 255)) / 255.0f;
    return nk_hsva_f(hf, sf, vf, af);
}
NK_API struct nk_color
nk_hsva_iv(const int *c)
{
    return nk_hsva(c[0], c[1], c[2], c[3]);
}
NK_API struct nk_color
nk_hsva_bv(const nk_byte *c)
{
    return nk_hsva(c[0], c[1], c[2], c[3]);
}
NK_API struct nk_colorf
nk_hsva_colorf(float h, float s, float v, float a)
{
    int i;
    float p, q, t, f;
    struct nk_colorf out = {0,0,0,0};
    if (s <= 0.0f) {
        out.r = v; out.g = v; out.b = v; out.a = a;
        return out;
    }
    h = h / (60.0f/360.0f);
    i = (int)h;
    f = h - (float)i;
    p = v * (1.0f - s);
    q = v * (1.0f - (s * f));
    t = v * (1.0f - s * (1.0f - f));

    switch (i) {
    case 0: default: out.r = v; out.g = t; out.b = p; break;
    case 1: out.r = q; out.g = v; out.b = p; break;
    case 2: out.r = p; out.g = v; out.b = t; break;
    case 3: out.r = p; out.g = q; out.b = v; break;
    case 4: out.r = t; out.g = p; out.b = v; break;
    case 5: out.r = v; out.g = p; out.b = q; break;}
    out.a = a;
    return out;
}
NK_API struct nk_colorf
nk_hsva_colorfv(float *c)
{
    return nk_hsva_colorf(c[0], c[1], c[2], c[3]);
}
NK_API struct nk_color
nk_hsva_f(float h, float s, float v, float a)
{
    struct nk_colorf c = nk_hsva_colorf(h, s, v, a);
    return nk_rgba_f(c.r, c.g, c.b, c.a);
}
NK_API struct nk_color
nk_hsva_fv(const float *c)
{
    return nk_hsva_f(c[0], c[1], c[2], c[3]);
}
NK_API nk_uint
nk_color_u32(struct nk_color in)
{
    nk_uint out = (nk_uint)in.r;
    out |= ((nk_uint)in.g << 8);
    out |= ((nk_uint)in.b << 16);
    out |= ((nk_uint)in.a << 24);
    return out;
}
NK_API void
nk_color_f(float *r, float *g, float *b, float *a, struct nk_color in)
{
    NK_STORAGE const float s = 1.0f/255.0f;
    *r = (float)in.r * s;
    *g = (float)in.g * s;
    *b = (float)in.b * s;
    *a = (float)in.a * s;
}
NK_API void
nk_color_fv(float *c, struct nk_color in)
{
    nk_color_f(&c[0], &c[1], &c[2], &c[3], in);
}
NK_API struct nk_colorf
nk_color_cf(struct nk_color in)
{
    struct nk_colorf o;
    nk_color_f(&o.r, &o.g, &o.b, &o.a, in);
    return o;
}
NK_API void
nk_color_d(double *r, double *g, double *b, double *a, struct nk_color in)
{
    NK_STORAGE const double s = 1.0/255.0;
    *r = (double)in.r * s;
    *g = (double)in.g * s;
    *b = (double)in.b * s;
    *a = (double)in.a * s;
}
NK_API void
nk_color_dv(double *c, struct nk_color in)
{
    nk_color_d(&c[0], &c[1], &c[2], &c[3], in);
}
NK_API void
nk_color_hsv_f(float *out_h, float *out_s, float *out_v, struct nk_color in)
{
    float a;
    nk_color_hsva_f(out_h, out_s, out_v, &a, in);
}
NK_API void
nk_color_hsv_fv(float *out, struct nk_color in)
{
    float a;
    nk_color_hsva_f(&out[0], &out[1], &out[2], &a, in);
}
NK_API void
nk_colorf_hsva_f(float *out_h, float *out_s,
    float *out_v, float *out_a, struct nk_colorf in)
{
    float chroma;
    float K = 0.0f;
    if (in.g < in.b) {
        const float t = in.g; in.g = in.b; in.b = t;
        K = -1.f;
    }
    if (in.r < in.g) {
        const float t = in.r; in.r = in.g; in.g = t;
        K = -2.f/6.0f - K;
    }
    chroma = in.r - ((in.g < in.b) ? in.g: in.b);
    *out_h = NK_ABS(K + (in.g - in.b)/(6.0f * chroma + 1e-20f));
    *out_s = chroma / (in.r + 1e-20f);
    *out_v = in.r;
    *out_a = in.a;

}
NK_API void
nk_colorf_hsva_fv(float *hsva, struct nk_colorf in)
{
    nk_colorf_hsva_f(&hsva[0], &hsva[1], &hsva[2], &hsva[3], in);
}
NK_API void
nk_color_hsva_f(float *out_h, float *out_s,
    float *out_v, float *out_a, struct nk_color in)
{
    struct nk_colorf col;
    nk_color_f(&col.r,&col.g,&col.b,&col.a, in);
    nk_colorf_hsva_f(out_h, out_s, out_v, out_a, col);
}
NK_API void
nk_color_hsva_fv(float *out, struct nk_color in)
{
    nk_color_hsva_f(&out[0], &out[1], &out[2], &out[3], in);
}
NK_API void
nk_color_hsva_i(int *out_h, int *out_s, int *out_v,
                int *out_a, struct nk_color in)
{
    float h,s,v,a;
    nk_color_hsva_f(&h, &s, &v, &a, in);
    *out_h = (nk_byte)(h * 255.0f);
    *out_s = (nk_byte)(s * 255.0f);
    *out_v = (nk_byte)(v * 255.0f);
    *out_a = (nk_byte)(a * 255.0f);
}
NK_API void
nk_color_hsva_iv(int *out, struct nk_color in)
{
    nk_color_hsva_i(&out[0], &out[1], &out[2], &out[3], in);
}
NK_API void
nk_color_hsva_bv(nk_byte *out, struct nk_color in)
{
    int tmp[4];
    nk_color_hsva_i(&tmp[0], &tmp[1], &tmp[2], &tmp[3], in);
    out[0] = (nk_byte)tmp[0];
    out[1] = (nk_byte)tmp[1];
    out[2] = (nk_byte)tmp[2];
    out[3] = (nk_byte)tmp[3];
}
NK_API void
nk_color_hsva_b(nk_byte *h, nk_byte *s, nk_byte *v, nk_byte *a, struct nk_color in)
{
    int tmp[4];
    nk_color_hsva_i(&tmp[0], &tmp[1], &tmp[2], &tmp[3], in);
    *h = (nk_byte)tmp[0];
    *s = (nk_byte)tmp[1];
    *v = (nk_byte)tmp[2];
    *a = (nk_byte)tmp[3];
}
NK_API void
nk_color_hsv_i(int *out_h, int *out_s, int *out_v, struct nk_color in)
{
    int a;
    nk_color_hsva_i(out_h, out_s, out_v, &a, in);
}
NK_API void
nk_color_hsv_b(nk_byte *out_h, nk_byte *out_s, nk_byte *out_v, struct nk_color in)
{
    int tmp[4];
    nk_color_hsva_i(&tmp[0], &tmp[1], &tmp[2], &tmp[3], in);
    *out_h = (nk_byte)tmp[0];
    *out_s = (nk_byte)tmp[1];
    *out_v = (nk_byte)tmp[2];
}
NK_API void
nk_color_hsv_iv(int *out, struct nk_color in)
{
    nk_color_hsv_i(&out[0], &out[1], &out[2], in);
}
NK_API void
nk_color_hsv_bv(nk_byte *out, struct nk_color in)
{
    int tmp[4];
    nk_color_hsv_i(&tmp[0], &tmp[1], &tmp[2], in);
    out[0] = (nk_byte)tmp[0];
    out[1] = (nk_byte)tmp[1];
    out[2] = (nk_byte)tmp[2];
}





/* ===============================================================
 *
 *                              UTF-8
 *
 * ===============================================================*/
NK_GLOBAL const nk_byte nk_utfbyte[NK_UTF_SIZE+1] = {0x80, 0, 0xC0, 0xE0, 0xF0};
NK_GLOBAL const nk_byte nk_utfmask[NK_UTF_SIZE+1] = {0xC0, 0x80, 0xE0, 0xF0, 0xF8};
NK_GLOBAL const nk_uint nk_utfmin[NK_UTF_SIZE+1] = {0, 0, 0x80, 0x800, 0x10000};
NK_GLOBAL const nk_uint nk_utfmax[NK_UTF_SIZE+1] = {0x10FFFF, 0x7F, 0x7FF, 0xFFFF, 0x10FFFF};

NK_INTERN int
nk_utf_validate(nk_rune *u, int i)
{
    NK_ASSERT(u);
    if (!u) return 0;
    if (!NK_BETWEEN(*u, nk_utfmin[i], nk_utfmax[i]) ||
         NK_BETWEEN(*u, 0xD800, 0xDFFF))
            *u = NK_UTF_INVALID;
    for (i = 1; *u > nk_utfmax[i]; ++i);
    return i;
}
NK_INTERN nk_rune
nk_utf_decode_byte(char c, int *i)
{
    NK_ASSERT(i);
    if (!i) return 0;
    for(*i = 0; *i < (int)NK_LEN(nk_utfmask); ++(*i)) {
        if (((nk_byte)c & nk_utfmask[*i]) == nk_utfbyte[*i])
            return (nk_byte)(c & ~nk_utfmask[*i]);
    }
    return 0;
}
NK_API int
nk_utf_decode(const char *c, nk_rune *u, int clen)
{
    int i, j, len, type=0;
    nk_rune udecoded;

    NK_ASSERT(c);
    NK_ASSERT(u);

    if (!c || !u) return 0;
    if (!clen) return 0;
    *u = NK_UTF_INVALID;

    udecoded = nk_utf_decode_byte(c[0], &len);
    if (!NK_BETWEEN(len, 1, NK_UTF_SIZE))
        return 1;

    for (i = 1, j = 1; i < clen && j < len; ++i, ++j) {
        udecoded = (udecoded << 6) | nk_utf_decode_byte(c[i], &type);
        if (type != 0)
            return j;
    }
    if (j < len)
        return 0;
    *u = udecoded;
    nk_utf_validate(u, len);
    return len;
}
NK_INTERN char
nk_utf_encode_byte(nk_rune u, int i)
{
    return (char)((nk_utfbyte[i]) | ((nk_byte)u & ~nk_utfmask[i]));
}
NK_API int
nk_utf_encode(nk_rune u, char *c, int clen)
{
    int len, i;
    len = nk_utf_validate(&u, 0);
    if (clen < len || !len || len > NK_UTF_SIZE)
        return 0;

    for (i = len - 1; i != 0; --i) {
        c[i] = nk_utf_encode_byte(u, 0);
        u >>= 6;
    }
    c[0] = nk_utf_encode_byte(u, len);
    return len;
}
NK_API int
nk_utf_len(const char *str, int len)
{
    const char *text;
    int glyphs = 0;
    int text_len;
    int glyph_len;
    int src_len = 0;
    nk_rune unicode;

    NK_ASSERT(str);
    if (!str || !len) return 0;

    text = str;
    text_len = len;
    glyph_len = nk_utf_decode(text, &unicode, text_len);
    while (glyph_len && src_len < len) {
        glyphs++;
        src_len = src_len + glyph_len;
        glyph_len = nk_utf_decode(text + src_len, &unicode, text_len - src_len);
    }
    return glyphs;
}
NK_API const char*
nk_utf_at(const char *buffer, int length, int index,
    nk_rune *unicode, int *len)
{
    int i = 0;
    int src_len = 0;
    int glyph_len = 0;
    const char *text;
    int text_len;

    NK_ASSERT(buffer);
    NK_ASSERT(unicode);
    NK_ASSERT(len);

    if (!buffer || !unicode || !len) return 0;
    if (index < 0) {
        *unicode = NK_UTF_INVALID;
        *len = 0;
        return 0;
    }

    text = buffer;
    text_len = length;
    glyph_len = nk_utf_decode(text, unicode, text_len);
    while (glyph_len) {
        if (i == index) {
            *len = glyph_len;
            break;
        }

        i++;
        src_len = src_len + glyph_len;
        glyph_len = nk_utf_decode(text + src_len, unicode, text_len - src_len);
    }
    if (i != index) return 0;
    return buffer + src_len;
}





/* ==============================================================
 *
 *                          BUFFER
 *
 * ===============================================================*/
#ifdef NK_INCLUDE_DEFAULT_ALLOCATOR
NK_LIB void*
nk_malloc(nk_handle unused, void *old,nk_size size)
{
    NK_UNUSED(unused);
    NK_UNUSED(old);
    return malloc(size);
}
NK_LIB void
nk_mfree(nk_handle unused, void *ptr)
{
    NK_UNUSED(unused);
    free(ptr);
}
NK_API void
nk_buffer_init_default(struct nk_buffer *buffer)
{
    struct nk_allocator alloc;
    alloc.userdata.ptr = 0;
    alloc.alloc = nk_malloc;
    alloc.free = nk_mfree;
    nk_buffer_init(buffer, &alloc, NK_BUFFER_DEFAULT_INITIAL_SIZE);
}
#endif

NK_API void
nk_buffer_init(struct nk_buffer *b, const struct nk_allocator *a,
    nk_size initial_size)
{
    NK_ASSERT(b);
    NK_ASSERT(a);
    NK_ASSERT(initial_size);
    if (!b || !a || !initial_size) return;

    nk_zero(b, sizeof(*b));
    b->type = NK_BUFFER_DYNAMIC;
    b->memory.ptr = a->alloc(a->userdata,0, initial_size);
    b->memory.size = initial_size;
    b->size = initial_size;
    b->grow_factor = 2.0f;
    b->pool = *a;
}
NK_API void
nk_buffer_init_fixed(struct nk_buffer *b, void *m, nk_size size)
{
    NK_ASSERT(b);
    NK_ASSERT(m);
    NK_ASSERT(size);
    if (!b || !m || !size) return;

    nk_zero(b, sizeof(*b));
    b->type = NK_BUFFER_FIXED;
    b->memory.ptr = m;
    b->memory.size = size;
    b->size = size;
}
NK_LIB void*
nk_buffer_align(void *unaligned,
    nk_size align, nk_size *alignment,
    enum nk_buffer_allocation_type type)
{
    void *memory = 0;
    switch (type) {
    default:
    case NK_BUFFER_MAX:
    case NK_BUFFER_FRONT:
        if (align) {
            memory = NK_ALIGN_PTR(unaligned, align);
            *alignment = (nk_size)((nk_byte*)memory - (nk_byte*)unaligned);
        } else {
            memory = unaligned;
            *alignment = 0;
        }
        break;
    case NK_BUFFER_BACK:
        if (align) {
            memory = NK_ALIGN_PTR_BACK(unaligned, align);
            *alignment = (nk_size)((nk_byte*)unaligned - (nk_byte*)memory);
        } else {
            memory = unaligned;
            *alignment = 0;
        }
        break;
    }
    return memory;
}
NK_LIB void*
nk_buffer_realloc(struct nk_buffer *b, nk_size capacity, nk_size *size)
{
    void *temp;
    nk_size buffer_size;

    NK_ASSERT(b);
    NK_ASSERT(size);
    if (!b || !size || !b->pool.alloc || !b->pool.free)
        return 0;

    buffer_size = b->memory.size;
    temp = b->pool.alloc(b->pool.userdata, b->memory.ptr, capacity);
    NK_ASSERT(temp);
    if (!temp) return 0;

    *size = capacity;
    if (temp != b->memory.ptr) {
        NK_MEMCPY(temp, b->memory.ptr, buffer_size);
        b->pool.free(b->pool.userdata, b->memory.ptr);
    }

    if (b->size == buffer_size) {
        /* no back buffer so just set correct size */
        b->size = capacity;
        return temp;
    } else {
        /* copy back buffer to the end of the new buffer */
        void *dst, *src;
        nk_size back_size;
        back_size = buffer_size - b->size;
        dst = nk_ptr_add(void, temp, capacity - back_size);
        src = nk_ptr_add(void, temp, b->size);
        NK_MEMCPY(dst, src, back_size);
        b->size = capacity - back_size;
    }
    return temp;
}
NK_LIB void*
nk_buffer_alloc(struct nk_buffer *b, enum nk_buffer_allocation_type type,
    nk_size size, nk_size align)
{
    int full;
    nk_size alignment;
    void *unaligned;
    void *memory;

    NK_ASSERT(b);
    NK_ASSERT(size);
    if (!b || !size) return 0;
    b->needed += size;

    /* calculate total size with needed alignment + size */
    if (type == NK_BUFFER_FRONT)
        unaligned = nk_ptr_add(void, b->memory.ptr, b->allocated);
    else unaligned = nk_ptr_add(void, b->memory.ptr, b->size - size);
    memory = nk_buffer_align(unaligned, align, &alignment, type);

    /* check if buffer has enough memory*/
    if (type == NK_BUFFER_FRONT)
        full = ((b->allocated + size + alignment) > b->size);
    else full = ((b->size - NK_MIN(b->size,(size + alignment))) <= b->allocated);

    if (full) {
        nk_size capacity;
        if (b->type != NK_BUFFER_DYNAMIC)
            return 0;
        NK_ASSERT(b->pool.alloc && b->pool.free);
        if (b->type != NK_BUFFER_DYNAMIC || !b->pool.alloc || !b->pool.free)
            return 0;

        /* buffer is full so allocate bigger buffer if dynamic */
        capacity = (nk_size)((float)b->memory.size * b->grow_factor);
        capacity = NK_MAX(capacity, nk_round_up_pow2((nk_uint)(b->allocated + size)));
        b->memory.ptr = nk_buffer_realloc(b, capacity, &b->memory.size);
        if (!b->memory.ptr) return 0;

        /* align newly allocated pointer */
        if (type == NK_BUFFER_FRONT)
            unaligned = nk_ptr_add(void, b->memory.ptr, b->allocated);
        else unaligned = nk_ptr_add(void, b->memory.ptr, b->size - size);
        memory = nk_buffer_align(unaligned, align, &alignment, type);
    }
    if (type == NK_BUFFER_FRONT)
        b->allocated += size + alignment;
    else b->size -= (size + alignment);
    b->needed += alignment;
    b->calls++;
    return memory;
}
NK_API void
nk_buffer_push(struct nk_buffer *b, enum nk_buffer_allocation_type type,
    const void *memory, nk_size size, nk_size align)
{
    void *mem = nk_buffer_alloc(b, type, size, align);
    if (!mem) return;
    NK_MEMCPY(mem, memory, size);
}
NK_API void
nk_buffer_mark(struct nk_buffer *buffer, enum nk_buffer_allocation_type type)
{
    NK_ASSERT(buffer);
    if (!buffer) return;
    buffer->marker[type].active = nk_true;
    if (type == NK_BUFFER_BACK)
        buffer->marker[type].offset = buffer->size;
    else buffer->marker[type].offset = buffer->allocated;
}
NK_API void
nk_buffer_reset(struct nk_buffer *buffer, enum nk_buffer_allocation_type type)
{
    NK_ASSERT(buffer);
    if (!buffer) return;
    if (type == NK_BUFFER_BACK) {
        /* reset back buffer either back to marker or empty */
        buffer->needed -= (buffer->memory.size - buffer->marker[type].offset);
        if (buffer->marker[type].active)
            buffer->size = buffer->marker[type].offset;
        else buffer->size = buffer->memory.size;
        buffer->marker[type].active = nk_false;
    } else {
        /* reset front buffer either back to back marker or empty */
        buffer->needed -= (buffer->allocated - buffer->marker[type].offset);
        if (buffer->marker[type].active)
            buffer->allocated = buffer->marker[type].offset;
        else buffer->allocated = 0;
        buffer->marker[type].active = nk_false;
    }
}
NK_API void
nk_buffer_clear(struct nk_buffer *b)
{
    NK_ASSERT(b);
    if (!b) return;
    b->allocated = 0;
    b->size = b->memory.size;
    b->calls = 0;
    b->needed = 0;
}
NK_API void
nk_buffer_free(struct nk_buffer *b)
{
    NK_ASSERT(b);
    if (!b || !b->memory.ptr) return;
    if (b->type == NK_BUFFER_FIXED) return;
    if (!b->pool.free) return;
    NK_ASSERT(b->pool.free);
    b->pool.free(b->pool.userdata, b->memory.ptr);
}
NK_API void
nk_buffer_info(struct nk_memory_status *s, struct nk_buffer *b)
{
    NK_ASSERT(b);
    NK_ASSERT(s);
    if (!s || !b) return;
    s->allocated = b->allocated;
    s->size =  b->memory.size;
    s->needed = b->needed;
    s->memory = b->memory.ptr;
    s->calls = b->calls;
}
NK_API void*
nk_buffer_memory(struct nk_buffer *buffer)
{
    NK_ASSERT(buffer);
    if (!buffer) return 0;
    return buffer->memory.ptr;
}
NK_API const void*
nk_buffer_memory_const(const struct nk_buffer *buffer)
{
    NK_ASSERT(buffer);
    if (!buffer) return 0;
    return buffer->memory.ptr;
}
NK_API nk_size
nk_buffer_total(struct nk_buffer *buffer)
{
    NK_ASSERT(buffer);
    if (!buffer) return 0;
    return buffer->memory.size;
}





/* ===============================================================
 *
 *                              STRING
 *
 * ===============================================================*/
#ifdef NK_INCLUDE_DEFAULT_ALLOCATOR
NK_API void
nk_str_init_default(struct nk_str *str)
{
    struct nk_allocator alloc;
    alloc.userdata.ptr = 0;
    alloc.alloc = nk_malloc;
    alloc.free = nk_mfree;
    nk_buffer_init(&str->buffer, &alloc, 32);
    str->len = 0;
}
#endif

NK_API void
nk_str_init(struct nk_str *str, const struct nk_allocator *alloc, nk_size size)
{
    nk_buffer_init(&str->buffer, alloc, size);
    str->len = 0;
}
NK_API void
nk_str_init_fixed(struct nk_str *str, void *memory, nk_size size)
{
    nk_buffer_init_fixed(&str->buffer, memory, size);
    str->len = 0;
}
NK_API int
nk_str_append_text_char(struct nk_str *s, const char *str, int len)
{
    char *mem;
    NK_ASSERT(s);
    NK_ASSERT(str);
    if (!s || !str || !len) return 0;
    mem = (char*)nk_buffer_alloc(&s->buffer, NK_BUFFER_FRONT, (nk_size)len * sizeof(char), 0);
    if (!mem) return 0;
    NK_MEMCPY(mem, str, (nk_size)len * sizeof(char));
    s->len += nk_utf_len(str, len);
    return len;
}
NK_API int
nk_str_append_str_char(struct nk_str *s, const char *str)
{
    return nk_str_append_text_char(s, str, nk_strlen(str));
}
NK_API int
nk_str_append_text_utf8(struct nk_str *str, const char *text, int len)
{
    int i = 0;
    int byte_len = 0;
    nk_rune unicode;
    if (!str || !text || !len) return 0;
    for (i = 0; i < len; ++i)
        byte_len += nk_utf_decode(text+byte_len, &unicode, 4);
    nk_str_append_text_char(str, text, byte_len);
    return len;
}
NK_API int
nk_str_append_str_utf8(struct nk_str *str, const char *text)
{
    int byte_len = 0;
    int num_runes = 0;
    int glyph_len = 0;
    nk_rune unicode;
    if (!str || !text) return 0;

    glyph_len = byte_len = nk_utf_decode(text+byte_len, &unicode, 4);
    while (unicode != '\0' && glyph_len) {
        glyph_len = nk_utf_decode(text+byte_len, &unicode, 4);
        byte_len += glyph_len;
        num_runes++;
    }
    nk_str_append_text_char(str, text, byte_len);
    return num_runes;
}
NK_API int
nk_str_append_text_runes(struct nk_str *str, const nk_rune *text, int len)
{
    int i = 0;
    int byte_len = 0;
    nk_glyph glyph;

    NK_ASSERT(str);
    if (!str || !text || !len) return 0;
    for (i = 0; i < len; ++i) {
        byte_len = nk_utf_encode(text[i], glyph, NK_UTF_SIZE);
        if (!byte_len) break;
        nk_str_append_text_char(str, glyph, byte_len);
    }
    return len;
}
NK_API int
nk_str_append_str_runes(struct nk_str *str, const nk_rune *runes)
{
    int i = 0;
    nk_glyph glyph;
    int byte_len;
    NK_ASSERT(str);
    if (!str || !runes) return 0;
    while (runes[i] != '\0') {
        byte_len = nk_utf_encode(runes[i], glyph, NK_UTF_SIZE);
        nk_str_append_text_char(str, glyph, byte_len);
        i++;
    }
    return i;
}
NK_API int
nk_str_insert_at_char(struct nk_str *s, int pos, const char *str, int len)
{
    int i;
    void *mem;
    char *src;
    char *dst;

    int copylen;
    NK_ASSERT(s);
    NK_ASSERT(str);
    NK_ASSERT(len >= 0);
    if (!s || !str || !len || (nk_size)pos > s->buffer.allocated) return 0;
    if ((s->buffer.allocated + (nk_size)len >= s->buffer.memory.size) &&
        (s->buffer.type == NK_BUFFER_FIXED)) return 0;

    copylen = (int)s->buffer.allocated - pos;
    if (!copylen) {
        nk_str_append_text_char(s, str, len);
        return 1;
    }
    mem = nk_buffer_alloc(&s->buffer, NK_BUFFER_FRONT, (nk_size)len * sizeof(char), 0);
    if (!mem) return 0;

    /* memmove */
    NK_ASSERT(((int)pos + (int)len + ((int)copylen - 1)) >= 0);
    NK_ASSERT(((int)pos + ((int)copylen - 1)) >= 0);
    dst = nk_ptr_add(char, s->buffer.memory.ptr, pos + len + (copylen - 1));
    src = nk_ptr_add(char, s->buffer.memory.ptr, pos + (copylen-1));
    for (i = 0; i < copylen; ++i) *dst-- = *src--;
    mem = nk_ptr_add(void, s->buffer.memory.ptr, pos);
    NK_MEMCPY(mem, str, (nk_size)len * sizeof(char));
    s->len = nk_utf_len((char *)s->buffer.memory.ptr, (int)s->buffer.allocated);
    return 1;
}
NK_API int
nk_str_insert_at_rune(struct nk_str *str, int pos, const char *cstr, int len)
{
    int glyph_len;
    nk_rune unicode;
    const char *begin;
    const char *buffer;

    NK_ASSERT(str);
    NK_ASSERT(cstr);
    NK_ASSERT(len);
    if (!str || !cstr || !len) return 0;
    begin = nk_str_at_rune(str, pos, &unicode, &glyph_len);
    if (!str->len)
        return nk_str_append_text_char(str, cstr, len);
    buffer = nk_str_get_const(str);
    if (!begin) return 0;
    return nk_str_insert_at_char(str, (int)(begin - buffer), cstr, len);
}
NK_API int
nk_str_insert_text_char(struct nk_str *str, int pos, const char *text, int len)
{
    return nk_str_insert_text_utf8(str, pos, text, len);
}
NK_API int
nk_str_insert_str_char(struct nk_str *str, int pos, const char *text)
{
    return nk_str_insert_text_utf8(str, pos, text, nk_strlen(text));
}
NK_API int
nk_str_insert_text_utf8(struct nk_str *str, int pos, const char *text, int len)
{
    int i = 0;
    int byte_len = 0;
    nk_rune unicode;

    NK_ASSERT(str);
    NK_ASSERT(text);
    if (!str || !text || !len) return 0;
    for (i = 0; i < len; ++i)
        byte_len += nk_utf_decode(text+byte_len, &unicode, 4);
    nk_str_insert_at_rune(str, pos, text, byte_len);
    return len;
}
NK_API int
nk_str_insert_str_utf8(struct nk_str *str, int pos, const char *text)
{
    int byte_len = 0;
    int num_runes = 0;
    int glyph_len = 0;
    nk_rune unicode;
    if (!str || !text) return 0;

    glyph_len = byte_len = nk_utf_decode(text+byte_len, &unicode, 4);
    while (unicode != '\0' && glyph_len) {
        glyph_len = nk_utf_decode(text+byte_len, &unicode, 4);
        byte_len += glyph_len;
        num_runes++;
    }
    nk_str_insert_at_rune(str, pos, text, byte_len);
    return num_runes;
}
NK_API int
nk_str_insert_text_runes(struct nk_str *str, int pos, const nk_rune *runes, int len)
{
    int i = 0;
    int byte_len = 0;
    nk_glyph glyph;

    NK_ASSERT(str);
    if (!str || !runes || !len) return 0;
    for (i = 0; i < len; ++i) {
        byte_len = nk_utf_encode(runes[i], glyph, NK_UTF_SIZE);
        if (!byte_len) break;
        nk_str_insert_at_rune(str, pos+i, glyph, byte_len);
    }
    return len;
}
NK_API int
nk_str_insert_str_runes(struct nk_str *str, int pos, const nk_rune *runes)
{
    int i = 0;
    nk_glyph glyph;
    int byte_len;
    NK_ASSERT(str);
    if (!str || !runes) return 0;
    while (runes[i] != '\0') {
        byte_len = nk_utf_encode(runes[i], glyph, NK_UTF_SIZE);
        nk_str_insert_at_rune(str, pos+i, glyph, byte_len);
        i++;
    }
    return i;
}
NK_API void
nk_str_remove_chars(struct nk_str *s, int len)
{
    NK_ASSERT(s);
    NK_ASSERT(len >= 0);
    if (!s || len < 0 || (nk_size)len > s->buffer.allocated) return;
    NK_ASSERT(((int)s->buffer.allocated - (int)len) >= 0);
    s->buffer.allocated -= (nk_size)len;
    s->len = nk_utf_len((char *)s->buffer.memory.ptr, (int)s->buffer.allocated);
}
NK_API void
nk_str_remove_runes(struct nk_str *str, int len)
{
    int index;
    const char *begin;
    const char *end;
    nk_rune unicode;

    NK_ASSERT(str);
    NK_ASSERT(len >= 0);
    if (!str || len < 0) return;
    if (len >= str->len) {
        str->len = 0;
        return;
    }

    index = str->len - len;
    begin = nk_str_at_rune(str, index, &unicode, &len);
    end = (const char*)str->buffer.memory.ptr + str->buffer.allocated;
    nk_str_remove_chars(str, (int)(end-begin)+1);
}
NK_API void
nk_str_delete_chars(struct nk_str *s, int pos, int len)
{
    NK_ASSERT(s);
    if (!s || !len || (nk_size)pos > s->buffer.allocated ||
        (nk_size)(pos + len) > s->buffer.allocated) return;

    if ((nk_size)(pos + len) < s->buffer.allocated) {
        /* memmove */
        char *dst = nk_ptr_add(char, s->buffer.memory.ptr, pos);
        char *src = nk_ptr_add(char, s->buffer.memory.ptr, pos + len);
        NK_MEMCPY(dst, src, s->buffer.allocated - (nk_size)(pos + len));
        NK_ASSERT(((int)s->buffer.allocated - (int)len) >= 0);
        s->buffer.allocated -= (nk_size)len;
    } else nk_str_remove_chars(s, len);
    s->len = nk_utf_len((char *)s->buffer.memory.ptr, (int)s->buffer.allocated);
}
NK_API void
nk_str_delete_runes(struct nk_str *s, int pos, int len)
{
    char *temp;
    nk_rune unicode;
    char *begin;
    char *end;
    int unused;

    NK_ASSERT(s);
    NK_ASSERT(s->len >= pos + len);
    if (s->len < pos + len)
        len = NK_CLAMP(0, (s->len - pos), s->len);
    if (!len) return;

    temp = (char *)s->buffer.memory.ptr;
    begin = nk_str_at_rune(s, pos, &unicode, &unused);
    if (!begin) return;
    s->buffer.memory.ptr = begin;
    end = nk_str_at_rune(s, len, &unicode, &unused);
    s->buffer.memory.ptr = temp;
    if (!end) return;
    nk_str_delete_chars(s, (int)(begin - temp), (int)(end - begin));
}
NK_API char*
nk_str_at_char(struct nk_str *s, int pos)
{
    NK_ASSERT(s);
    if (!s || pos > (int)s->buffer.allocated) return 0;
    return nk_ptr_add(char, s->buffer.memory.ptr, pos);
}
NK_API char*
nk_str_at_rune(struct nk_str *str, int pos, nk_rune *unicode, int *len)
{
    int i = 0;
    int src_len = 0;
    int glyph_len = 0;
    char *text;
    int text_len;

    NK_ASSERT(str);
    NK_ASSERT(unicode);
    NK_ASSERT(len);

    if (!str || !unicode || !len) return 0;
    if (pos < 0) {
        *unicode = 0;
        *len = 0;
        return 0;
    }

    text = (char*)str->buffer.memory.ptr;
    text_len = (int)str->buffer.allocated;
    glyph_len = nk_utf_decode(text, unicode, text_len);
    while (glyph_len) {
        if (i == pos) {
            *len = glyph_len;
            break;
        }

        i++;
        src_len = src_len + glyph_len;
        glyph_len = nk_utf_decode(text + src_len, unicode, text_len - src_len);
    }
    if (i != pos) return 0;
    return text + src_len;
}
NK_API const char*
nk_str_at_char_const(const struct nk_str *s, int pos)
{
    NK_ASSERT(s);
    if (!s || pos > (int)s->buffer.allocated) return 0;
    return nk_ptr_add(char, s->buffer.memory.ptr, pos);
}
NK_API const char*
nk_str_at_const(const struct nk_str *str, int pos, nk_rune *unicode, int *len)
{
    int i = 0;
    int src_len = 0;
    int glyph_len = 0;
    char *text;
    int text_len;

    NK_ASSERT(str);
    NK_ASSERT(unicode);
    NK_ASSERT(len);

    if (!str || !unicode || !len) return 0;
    if (pos < 0) {
        *unicode = 0;
        *len = 0;
        return 0;
    }

    text = (char*)str->buffer.memory.ptr;
    text_len = (int)str->buffer.allocated;
    glyph_len = nk_utf_decode(text, unicode, text_len);
    while (glyph_len) {
        if (i == pos) {
            *len = glyph_len;
            break;
        }

        i++;
        src_len = src_len + glyph_len;
        glyph_len = nk_utf_decode(text + src_len, unicode, text_len - src_len);
    }
    if (i != pos) return 0;
    return text + src_len;
}
NK_API nk_rune
nk_str_rune_at(const struct nk_str *str, int pos)
{
    int len;
    nk_rune unicode = 0;
    nk_str_at_const(str, pos, &unicode, &len);
    return unicode;
}
NK_API char*
nk_str_get(struct nk_str *s)
{
    NK_ASSERT(s);
    if (!s || !s->len || !s->buffer.allocated) return 0;
    return (char*)s->buffer.memory.ptr;
}
NK_API const char*
nk_str_get_const(const struct nk_str *s)
{
    NK_ASSERT(s);
    if (!s || !s->len || !s->buffer.allocated) return 0;
    return (const char*)s->buffer.memory.ptr;
}
NK_API int
nk_str_len(struct nk_str *s)
{
    NK_ASSERT(s);
    if (!s || !s->len || !s->buffer.allocated) return 0;
    return s->len;
}
NK_API int
nk_str_len_char(struct nk_str *s)
{
    NK_ASSERT(s);
    if (!s || !s->len || !s->buffer.allocated) return 0;
    return (int)s->buffer.allocated;
}
NK_API void
nk_str_clear(struct nk_str *str)
{
    NK_ASSERT(str);
    nk_buffer_clear(&str->buffer);
    str->len = 0;
}
NK_API void
nk_str_free(struct nk_str *str)
{
    NK_ASSERT(str);
    nk_buffer_free(&str->buffer);
    str->len = 0;
}





/* ==============================================================
 *
 *                          DRAW
 *
 * ===============================================================*/
NK_LIB void
nk_command_buffer_init(struct nk_command_buffer *cb,
    struct nk_buffer *b, enum nk_command_clipping clip)
{
    NK_ASSERT(cb);
    NK_ASSERT(b);
    if (!cb || !b) return;
    cb->base = b;
    cb->use_clipping = (int)clip;
    cb->begin = b->allocated;
    cb->end = b->allocated;
    cb->last = b->allocated;
}
NK_LIB void
nk_command_buffer_reset(struct nk_command_buffer *b)
{
    NK_ASSERT(b);
    if (!b) return;
    b->begin = 0;
    b->end = 0;
    b->last = 0;
    b->clip = nk_null_rect;
#ifdef NK_INCLUDE_COMMAND_USERDATA
    b->userdata.ptr = 0;
#endif
}
NK_LIB void*
nk_command_buffer_push(struct nk_command_buffer* b,
    enum nk_command_type t, nk_size size)
{
    NK_STORAGE const nk_size align = NK_ALIGNOF(struct nk_command);
    struct nk_command *cmd;
    nk_size alignment;
    void *unaligned;
    void *memory;

    NK_ASSERT(b);
    NK_ASSERT(b->base);
    if (!b) return 0;
    cmd = (struct nk_command*)nk_buffer_alloc(b->base,NK_BUFFER_FRONT,size,align);
    if (!cmd) return 0;

    /* make sure the offset to the next command is aligned */
    b->last = (nk_size)((nk_byte*)cmd - (nk_byte*)b->base->memory.ptr);
    unaligned = (nk_byte*)cmd + size;
    memory = NK_ALIGN_PTR(unaligned, align);
    alignment = (nk_size)((nk_byte*)memory - (nk_byte*)unaligned);
#ifdef NK_ZERO_COMMAND_MEMORY
    NK_MEMSET(cmd, 0, size + alignment);
#endif

    cmd->type = t;
    cmd->next = b->base->allocated + alignment;
#ifdef NK_INCLUDE_COMMAND_USERDATA
    cmd->userdata = b->userdata;
#endif
    b->end = cmd->next;
    return cmd;
}
NK_API void
nk_push_scissor(struct nk_command_buffer *b, struct nk_rect r)
{
    struct nk_command_scissor *cmd;
    NK_ASSERT(b);
    if (!b) return;

    b->clip.x = r.x;
    b->clip.y = r.y;
    b->clip.w = r.w;
    b->clip.h = r.h;
    cmd = (struct nk_command_scissor*)
        nk_command_buffer_push(b, NK_COMMAND_SCISSOR, sizeof(*cmd));

    if (!cmd) return;
    cmd->x = (short)r.x;
    cmd->y = (short)r.y;
    cmd->w = (unsigned short)NK_MAX(0, r.w);
    cmd->h = (unsigned short)NK_MAX(0, r.h);
}
NK_API void
nk_stroke_line(struct nk_command_buffer *b, float x0, float y0,
    float x1, float y1, float line_thickness, struct nk_color c)
{
    struct nk_command_line *cmd;
    NK_ASSERT(b);
    if (!b || line_thickness <= 0) return;
    cmd = (struct nk_command_line*)
        nk_command_buffer_push(b, NK_COMMAND_LINE, sizeof(*cmd));
    if (!cmd) return;
    cmd->line_thickness = (unsigned short)line_thickness;
    cmd->begin.x = (short)x0;
    cmd->begin.y = (short)y0;
    cmd->end.x = (short)x1;
    cmd->end.y = (short)y1;
    cmd->color = c;
}
NK_API void
nk_stroke_curve(struct nk_command_buffer *b, float ax, float ay,
    float ctrl0x, float ctrl0y, float ctrl1x, float ctrl1y,
    float bx, float by, float line_thickness, struct nk_color col)
{
    struct nk_command_curve *cmd;
    NK_ASSERT(b);
    if (!b || col.a == 0 || line_thickness <= 0) return;

    cmd = (struct nk_command_curve*)
        nk_command_buffer_push(b, NK_COMMAND_CURVE, sizeof(*cmd));
    if (!cmd) return;
    cmd->line_thickness = (unsigned short)line_thickness;
    cmd->begin.x = (short)ax;
    cmd->begin.y = (short)ay;
    cmd->ctrl[0].x = (short)ctrl0x;
    cmd->ctrl[0].y = (short)ctrl0y;
    cmd->ctrl[1].x = (short)ctrl1x;
    cmd->ctrl[1].y = (short)ctrl1y;
    cmd->end.x = (short)bx;
    cmd->end.y = (short)by;
    cmd->color = col;
}
NK_API void
nk_stroke_rect(struct nk_command_buffer *b, struct nk_rect rect,
    float rounding, float line_thickness, struct nk_color c)
{
    struct nk_command_rect *cmd;
    NK_ASSERT(b);
    if (!b || c.a == 0 || rect.w == 0 || rect.h == 0 || line_thickness <= 0) return;
    if (b->use_clipping) {
        const struct nk_rect *clip = &b->clip;
        if (!NK_INTERSECT(rect.x, rect.y, rect.w, rect.h,
            clip->x, clip->y, clip->w, clip->h)) return;
    }
    cmd = (struct nk_command_rect*)
        nk_command_buffer_push(b, NK_COMMAND_RECT, sizeof(*cmd));
    if (!cmd) return;
    cmd->rounding = (unsigned short)rounding;
    cmd->line_thickness = (unsigned short)line_thickness;
    cmd->x = (short)rect.x;
    cmd->y = (short)rect.y;
    cmd->w = (unsigned short)NK_MAX(0, rect.w);
    cmd->h = (unsigned short)NK_MAX(0, rect.h);
    cmd->color = c;
}
NK_API void
nk_fill_rect(struct nk_command_buffer *b, struct nk_rect rect,
    float rounding, struct nk_color c)
{
    struct nk_command_rect_filled *cmd;
    NK_ASSERT(b);
    if (!b || c.a == 0 || rect.w == 0 || rect.h == 0) return;
    if (b->use_clipping) {
        const struct nk_rect *clip = &b->clip;
        if (!NK_INTERSECT(rect.x, rect.y, rect.w, rect.h,
            clip->x, clip->y, clip->w, clip->h)) return;
    }

    cmd = (struct nk_command_rect_filled*)
        nk_command_buffer_push(b, NK_COMMAND_RECT_FILLED, sizeof(*cmd));
    if (!cmd) return;
    cmd->rounding = (unsigned short)rounding;
    cmd->x = (short)rect.x;
    cmd->y = (short)rect.y;
    cmd->w = (unsigned short)NK_MAX(0, rect.w);
    cmd->h = (unsigned short)NK_MAX(0, rect.h);
    cmd->color = c;
}
NK_API void
nk_fill_rect_multi_color(struct nk_command_buffer *b, struct nk_rect rect,
    struct nk_color left, struct nk_color top, struct nk_color right,
    struct nk_color bottom)
{
    struct nk_command_rect_multi_color *cmd;
    NK_ASSERT(b);
    if (!b || rect.w == 0 || rect.h == 0) return;
    if (b->use_clipping) {
        const struct nk_rect *clip = &b->clip;
        if (!NK_INTERSECT(rect.x, rect.y, rect.w, rect.h,
            clip->x, clip->y, clip->w, clip->h)) return;
    }

    cmd = (struct nk_command_rect_multi_color*)
        nk_command_buffer_push(b, NK_COMMAND_RECT_MULTI_COLOR, sizeof(*cmd));
    if (!cmd) return;
    cmd->x = (short)rect.x;
    cmd->y = (short)rect.y;
    cmd->w = (unsigned short)NK_MAX(0, rect.w);
    cmd->h = (unsigned short)NK_MAX(0, rect.h);
    cmd->left = left;
    cmd->top = top;
    cmd->right = right;
    cmd->bottom = bottom;
}
NK_API void
nk_stroke_circle(struct nk_command_buffer *b, struct nk_rect r,
    float line_thickness, struct nk_color c)
{
    struct nk_command_circle *cmd;
    if (!b || r.w == 0 || r.h == 0 || line_thickness <= 0) return;
    if (b->use_clipping) {
        const struct nk_rect *clip = &b->clip;
        if (!NK_INTERSECT(r.x, r.y, r.w, r.h, clip->x, clip->y, clip->w, clip->h))
            return;
    }

    cmd = (struct nk_command_circle*)
        nk_command_buffer_push(b, NK_COMMAND_CIRCLE, sizeof(*cmd));
    if (!cmd) return;
    cmd->line_thickness = (unsigned short)line_thickness;
    cmd->x = (short)r.x;
    cmd->y = (short)r.y;
    cmd->w = (unsigned short)NK_MAX(r.w, 0);
    cmd->h = (unsigned short)NK_MAX(r.h, 0);
    cmd->color = c;
}
NK_API void
nk_fill_circle(struct nk_command_buffer *b, struct nk_rect r, struct nk_color c)
{
    struct nk_command_circle_filled *cmd;
    NK_ASSERT(b);
    if (!b || c.a == 0 || r.w == 0 || r.h == 0) return;
    if (b->use_clipping) {
        const struct nk_rect *clip = &b->clip;
        if (!NK_INTERSECT(r.x, r.y, r.w, r.h, clip->x, clip->y, clip->w, clip->h))
            return;
    }

    cmd = (struct nk_command_circle_filled*)
        nk_command_buffer_push(b, NK_COMMAND_CIRCLE_FILLED, sizeof(*cmd));
    if (!cmd) return;
    cmd->x = (short)r.x;
    cmd->y = (short)r.y;
    cmd->w = (unsigned short)NK_MAX(r.w, 0);
    cmd->h = (unsigned short)NK_MAX(r.h, 0);
    cmd->color = c;
}
NK_API void
nk_stroke_arc(struct nk_command_buffer *b, float cx, float cy, float radius,
    float a_min, float a_max, float line_thickness, struct nk_color c)
{
    struct nk_command_arc *cmd;
    if (!b || c.a == 0 || line_thickness <= 0) return;
    cmd = (struct nk_command_arc*)
        nk_command_buffer_push(b, NK_COMMAND_ARC, sizeof(*cmd));
    if (!cmd) return;
    cmd->line_thickness = (unsigned short)line_thickness;
    cmd->cx = (short)cx;
    cmd->cy = (short)cy;
    cmd->r = (unsigned short)radius;
    cmd->a[0] = a_min;
    cmd->a[1] = a_max;
    cmd->color = c;
}
NK_API void
nk_fill_arc(struct nk_command_buffer *b, float cx, float cy, float radius,
    float a_min, float a_max, struct nk_color c)
{
    struct nk_command_arc_filled *cmd;
    NK_ASSERT(b);
    if (!b || c.a == 0) return;
    cmd = (struct nk_command_arc_filled*)
        nk_command_buffer_push(b, NK_COMMAND_ARC_FILLED, sizeof(*cmd));
    if (!cmd) return;
    cmd->cx = (short)cx;
    cmd->cy = (short)cy;
    cmd->r = (unsigned short)radius;
    cmd->a[0] = a_min;
    cmd->a[1] = a_max;
    cmd->color = c;
}
NK_API void
nk_stroke_triangle(struct nk_command_buffer *b, float x0, float y0, float x1,
    float y1, float x2, float y2, float line_thickness, struct nk_color c)
{
    struct nk_command_triangle *cmd;
    NK_ASSERT(b);
    if (!b || c.a == 0 || line_thickness <= 0) return;
    if (b->use_clipping) {
        const struct nk_rect *clip = &b->clip;
        if (!NK_INBOX(x0, y0, clip->x, clip->y, clip->w, clip->h) &&
            !NK_INBOX(x1, y1, clip->x, clip->y, clip->w, clip->h) &&
            !NK_INBOX(x2, y2, clip->x, clip->y, clip->w, clip->h))
            return;
    }

    cmd = (struct nk_command_triangle*)
        nk_command_buffer_push(b, NK_COMMAND_TRIANGLE, sizeof(*cmd));
    if (!cmd) return;
    cmd->line_thickness = (unsigned short)line_thickness;
    cmd->a.x = (short)x0;
    cmd->a.y = (short)y0;
    cmd->b.x = (short)x1;
    cmd->b.y = (short)y1;
    cmd->c.x = (short)x2;
    cmd->c.y = (short)y2;
    cmd->color = c;
}
NK_API void
nk_fill_triangle(struct nk_command_buffer *b, float x0, float y0, float x1,
    float y1, float x2, float y2, struct nk_color c)
{
    struct nk_command_triangle_filled *cmd;
    NK_ASSERT(b);
    if (!b || c.a == 0) return;
    if (!b) return;
    if (b->use_clipping) {
        const struct nk_rect *clip = &b->clip;
        if (!NK_INBOX(x0, y0, clip->x, clip->y, clip->w, clip->h) &&
            !NK_INBOX(x1, y1, clip->x, clip->y, clip->w, clip->h) &&
            !NK_INBOX(x2, y2, clip->x, clip->y, clip->w, clip->h))
            return;
    }

    cmd = (struct nk_command_triangle_filled*)
        nk_command_buffer_push(b, NK_COMMAND_TRIANGLE_FILLED, sizeof(*cmd));
    if (!cmd) return;
    cmd->a.x = (short)x0;
    cmd->a.y = (short)y0;
    cmd->b.x = (short)x1;
    cmd->b.y = (short)y1;
    cmd->c.x = (short)x2;
    cmd->c.y = (short)y2;
    cmd->color = c;
}
NK_API void
nk_stroke_polygon(struct nk_command_buffer *b,  float *points, int point_count,
    float line_thickness, struct nk_color col)
{
    int i;
    nk_size size = 0;
    struct nk_command_polygon *cmd;

    NK_ASSERT(b);
    if (!b || col.a == 0 || line_thickness <= 0) return;
    size = sizeof(*cmd) + sizeof(short) * 2 * (nk_size)point_count;
    cmd = (struct nk_command_polygon*) nk_command_buffer_push(b, NK_COMMAND_POLYGON, size);
    if (!cmd) return;
    cmd->color = col;
    cmd->line_thickness = (unsigned short)line_thickness;
    cmd->point_count = (unsigned short)point_count;
    for (i = 0; i < point_count; ++i) {
        cmd->points[i].x = (short)points[i*2];
        cmd->points[i].y = (short)points[i*2+1];
    }
}
NK_API void
nk_fill_polygon(struct nk_command_buffer *b, float *points, int point_count,
    struct nk_color col)
{
    int i;
    nk_size size = 0;
    struct nk_command_polygon_filled *cmd;

    NK_ASSERT(b);
    if (!b || col.a == 0) return;
    size = sizeof(*cmd) + sizeof(short) * 2 * (nk_size)point_count;
    cmd = (struct nk_command_polygon_filled*)
        nk_command_buffer_push(b, NK_COMMAND_POLYGON_FILLED, size);
    if (!cmd) return;
    cmd->color = col;
    cmd->point_count = (unsigned short)point_count;
    for (i = 0; i < point_count; ++i) {
        cmd->points[i].x = (short)points[i*2+0];
        cmd->points[i].y = (short)points[i*2+1];
    }
}
NK_API void
nk_stroke_polyline(struct nk_command_buffer *b, float *points, int point_count,
    float line_thickness, struct nk_color col)
{
    int i;
    nk_size size = 0;
    struct nk_command_polyline *cmd;

    NK_ASSERT(b);
    if (!b || col.a == 0 || line_thickness <= 0) return;
    size = sizeof(*cmd) + sizeof(short) * 2 * (nk_size)point_count;
    cmd = (struct nk_command_polyline*) nk_command_buffer_push(b, NK_COMMAND_POLYLINE, size);
    if (!cmd) return;
    cmd->color = col;
    cmd->point_count = (unsigned short)point_count;
    cmd->line_thickness = (unsigned short)line_thickness;
    for (i = 0; i < point_count; ++i) {
        cmd->points[i].x = (short)points[i*2];
        cmd->points[i].y = (short)points[i*2+1];
    }
}
NK_API void
nk_draw_image(struct nk_command_buffer *b, struct nk_rect r,
    const struct nk_image *img, struct nk_color col)
{
    struct nk_command_image *cmd;
    NK_ASSERT(b);
    if (!b) return;
    if (b->use_clipping) {
        const struct nk_rect *c = &b->clip;
        if (c->w == 0 || c->h == 0 || !NK_INTERSECT(r.x, r.y, r.w, r.h, c->x, c->y, c->w, c->h))
            return;
    }

    cmd = (struct nk_command_image*)
        nk_command_buffer_push(b, NK_COMMAND_IMAGE, sizeof(*cmd));
    if (!cmd) return;
    cmd->x = (short)r.x;
    cmd->y = (short)r.y;
    cmd->w = (unsigned short)NK_MAX(0, r.w);
    cmd->h = (unsigned short)NK_MAX(0, r.h);
    cmd->img = *img;
    cmd->col = col;
}
NK_API void
nk_draw_nine_slice(struct nk_command_buffer *b, struct nk_rect r,
    const struct nk_nine_slice *slc, struct nk_color col)
{
    struct nk_image img;
    const struct nk_image *slcimg = (const struct nk_image*)slc;
    nk_ushort rgnX, rgnY, rgnW, rgnH;
    rgnX = slcimg->region[0];
    rgnY = slcimg->region[1];
    rgnW = slcimg->region[2];
    rgnH = slcimg->region[3];

    /* top-left */
    img.handle = slcimg->handle;
    img.w = slcimg->w;
    img.h = slcimg->h;
    img.region[0] = rgnX;
    img.region[1] = rgnY;
    img.region[2] = slc->l;
    img.region[3] = slc->t;

    nk_draw_image(b,
        nk_rect(r.x, r.y, (float)slc->l, (float)slc->t),
        &img, col);

#define IMG_RGN(x, y, w, h) img.region[0] = (nk_ushort)(x); img.region[1] = (nk_ushort)(y); img.region[2] = (nk_ushort)(w); img.region[3] = (nk_ushort)(h);

    /* top-center */
    IMG_RGN(rgnX + slc->l, rgnY, rgnW - slc->l - slc->r, slc->t);
    nk_draw_image(b,
        nk_rect(r.x + (float)slc->l, r.y, (float)(r.w - slc->l - slc->r), (float)slc->t),
        &img, col);

    /* top-right */
    IMG_RGN(rgnX + rgnW - slc->r, rgnY, slc->r, slc->t);
    nk_draw_image(b,
        nk_rect(r.x + r.w - (float)slc->r, r.y, (float)slc->r, (float)slc->t),
        &img, col);

    /* center-left */
    IMG_RGN(rgnX, rgnY + slc->t, slc->l, rgnH - slc->t - slc->b);
    nk_draw_image(b,
        nk_rect(r.x, r.y + (float)slc->t, (float)slc->l, (float)(r.h - slc->t - slc->b)),
        &img, col);

    /* center */
    IMG_RGN(rgnX + slc->l, rgnY + slc->t, rgnW - slc->l - slc->r, rgnH - slc->t - slc->b);
    nk_draw_image(b,
        nk_rect(r.x + (float)slc->l, r.y + (float)slc->t, (float)(r.w - slc->l - slc->r), (float)(r.h - slc->t - slc->b)),
        &img, col);

    /* center-right */
    IMG_RGN(rgnX + rgnW - slc->r, rgnY + slc->t, slc->r, rgnH - slc->t - slc->b);
    nk_draw_image(b,
        nk_rect(r.x + r.w - (float)slc->r, r.y + (float)slc->t, (float)slc->r, (float)(r.h - slc->t - slc->b)),
        &img, col);

    /* bottom-left */
    IMG_RGN(rgnX, rgnY + rgnH - slc->b, slc->l, slc->b);
    nk_draw_image(b,
        nk_rect(r.x, r.y + r.h - (float)slc->b, (float)slc->l, (float)slc->b),
        &img, col);

    /* bottom-center */
    IMG_RGN(rgnX + slc->l, rgnY + rgnH - slc->b, rgnW - slc->l - slc->r, slc->b);
    nk_draw_image(b,
        nk_rect(r.x + (float)slc->l, r.y + r.h - (float)slc->b, (float)(r.w - slc->l - slc->r), (float)slc->b),
        &img, col);

    /* bottom-right */
    IMG_RGN(rgnX + rgnW - slc->r, rgnY + rgnH - slc->b, slc->r, slc->b);
    nk_draw_image(b,
        nk_rect(r.x + r.w - (float)slc->r, r.y + r.h - (float)slc->b, (float)slc->r, (float)slc->b),
        &img, col);

#undef IMG_RGN
}
NK_API void
nk_push_custom(struct nk_command_buffer *b, struct nk_rect r,
    nk_command_custom_callback cb, nk_handle usr)
{
    struct nk_command_custom *cmd;
    NK_ASSERT(b);
    if (!b) return;
    if (b->use_clipping) {
        const struct nk_rect *c = &b->clip;
        if (c->w == 0 || c->h == 0 || !NK_INTERSECT(r.x, r.y, r.w, r.h, c->x, c->y, c->w, c->h))
            return;
    }

    cmd = (struct nk_command_custom*)
        nk_command_buffer_push(b, NK_COMMAND_CUSTOM, sizeof(*cmd));
    if (!cmd) return;
    cmd->x = (short)r.x;
    cmd->y = (short)r.y;
    cmd->w = (unsigned short)NK_MAX(0, r.w);
    cmd->h = (unsigned short)NK_MAX(0, r.h);
    cmd->callback_data = usr;
    cmd->callback = cb;
}
NK_API void
nk_draw_text(struct nk_command_buffer *b, struct nk_rect r,
    const char *string, int length, const struct nk_user_font *font,
    struct nk_color bg, struct nk_color fg)
{
    float text_width = 0;
    struct nk_command_text *cmd;

    NK_ASSERT(b);
    NK_ASSERT(font);
    if (!b || !string || !length || (bg.a == 0 && fg.a == 0)) return;
    if (b->use_clipping) {
        const struct nk_rect *c = &b->clip;
        if (c->w == 0 || c->h == 0 || !NK_INTERSECT(r.x, r.y, r.w, r.h, c->x, c->y, c->w, c->h))
            return;
    }

    /* make sure text fits inside bounds */
    text_width = font->width(font->userdata, font->height, string, length);
    if (text_width > r.w){
        int glyphs = 0;
        float txt_width = (float)text_width;
        length = nk_text_clamp(font, string, length, r.w, &glyphs, &txt_width, 0,0);
    }

    if (length <= 0) return;
    cmd = (struct nk_command_text*)
        nk_command_buffer_push(b, NK_COMMAND_TEXT, sizeof(*cmd) + (nk_size)(length + 1));
    if (!cmd) return;
    cmd->x = (short)r.x;
    cmd->y = (short)r.y;
    cmd->w = (unsigned short)r.w;
    cmd->h = (unsigned short)r.h;
    cmd->background = bg;
    cmd->foreground = fg;
    cmd->font = font;
    cmd->length = length;
    cmd->height = font->height;
    NK_MEMCPY(cmd->string, string, (nk_size)length);
    cmd->string[length] = '\0';
}





/* ===============================================================
 *
 *                              VERTEX
 *
 * ===============================================================*/
#ifdef NK_INCLUDE_VERTEX_BUFFER_OUTPUT
NK_API void
nk_draw_list_init(struct nk_draw_list *list)
{
    nk_size i = 0;
    NK_ASSERT(list);
    if (!list) return;
    nk_zero(list, sizeof(*list));
    for (i = 0; i < NK_LEN(list->circle_vtx); ++i) {
        const float a = ((float)i / (float)NK_LEN(list->circle_vtx)) * 2 * NK_PI;
        list->circle_vtx[i].x = (float)NK_COS(a);
        list->circle_vtx[i].y = (float)NK_SIN(a);
    }
}
NK_API void
nk_draw_list_setup(struct nk_draw_list *canvas, const struct nk_convert_config *config,
    struct nk_buffer *cmds, struct nk_buffer *vertices, struct nk_buffer *elements,
    enum nk_anti_aliasing line_aa, enum nk_anti_aliasing shape_aa)
{
    NK_ASSERT(canvas);
    NK_ASSERT(config);
    NK_ASSERT(cmds);
    NK_ASSERT(vertices);
    NK_ASSERT(elements);
    if (!canvas || !config || !cmds || !vertices || !elements)
        return;

    canvas->buffer = cmds;
    canvas->config = *config;
    canvas->elements = elements;
    canvas->vertices = vertices;
    canvas->line_AA = line_aa;
    canvas->shape_AA = shape_aa;
    canvas->clip_rect = nk_null_rect;

    canvas->cmd_offset = 0;
    canvas->element_count = 0;
    canvas->vertex_count = 0;
    canvas->cmd_offset = 0;
    canvas->cmd_count = 0;
    canvas->path_count = 0;
}
NK_API const struct nk_draw_command*
nk__draw_list_begin(const struct nk_draw_list *canvas, const struct nk_buffer *buffer)
{
    nk_byte *memory;
    nk_size offset;
    const struct nk_draw_command *cmd;

    NK_ASSERT(buffer);
    if (!buffer || !buffer->size || !canvas->cmd_count)
        return 0;

    memory = (nk_byte*)buffer->memory.ptr;
    offset = buffer->memory.size - canvas->cmd_offset;
    cmd = nk_ptr_add(const struct nk_draw_command, memory, offset);
    return cmd;
}
NK_API const struct nk_draw_command*
nk__draw_list_end(const struct nk_draw_list *canvas, const struct nk_buffer *buffer)
{
    nk_size size;
    nk_size offset;
    nk_byte *memory;
    const struct nk_draw_command *end;

    NK_ASSERT(buffer);
    NK_ASSERT(canvas);
    if (!buffer || !canvas)
        return 0;

    memory = (nk_byte*)buffer->memory.ptr;
    size = buffer->memory.size;
    offset = size - canvas->cmd_offset;
    end = nk_ptr_add(const struct nk_draw_command, memory, offset);
    end -= (canvas->cmd_count-1);
    return end;
}
NK_API const struct nk_draw_command*
nk__draw_list_next(const struct nk_draw_command *cmd,
    const struct nk_buffer *buffer, const struct nk_draw_list *canvas)
{
    const struct nk_draw_command *end;
    NK_ASSERT(buffer);
    NK_ASSERT(canvas);
    if (!cmd || !buffer || !canvas)
        return 0;

    end = nk__draw_list_end(canvas, buffer);
    if (cmd <= end) return 0;
    return (cmd-1);
}
NK_INTERN struct nk_vec2*
nk_draw_list_alloc_path(struct nk_draw_list *list, int count)
{
    struct nk_vec2 *points;
    NK_STORAGE const nk_size point_align = NK_ALIGNOF(struct nk_vec2);
    NK_STORAGE const nk_size point_size = sizeof(struct nk_vec2);
    points = (struct nk_vec2*)
        nk_buffer_alloc(list->buffer, NK_BUFFER_FRONT,
                        point_size * (nk_size)count, point_align);

    if (!points) return 0;
    if (!list->path_offset) {
        void *memory = nk_buffer_memory(list->buffer);
        list->path_offset = (unsigned int)((nk_byte*)points - (nk_byte*)memory);
    }
    list->path_count += (unsigned int)count;
    return points;
}
NK_INTERN struct nk_vec2
nk_draw_list_path_last(struct nk_draw_list *list)
{
    void *memory;
    struct nk_vec2 *point;
    NK_ASSERT(list->path_count);
    memory = nk_buffer_memory(list->buffer);
    point = nk_ptr_add(struct nk_vec2, memory, list->path_offset);
    point += (list->path_count-1);
    return *point;
}
NK_INTERN struct nk_draw_command*
nk_draw_list_push_command(struct nk_draw_list *list, struct nk_rect clip,
    nk_handle texture)
{
    NK_STORAGE const nk_size cmd_align = NK_ALIGNOF(struct nk_draw_command);
    NK_STORAGE const nk_size cmd_size = sizeof(struct nk_draw_command);
    struct nk_draw_command *cmd;

    NK_ASSERT(list);
    cmd = (struct nk_draw_command*)
        nk_buffer_alloc(list->buffer, NK_BUFFER_BACK, cmd_size, cmd_align);

    if (!cmd) return 0;
    if (!list->cmd_count) {
        nk_byte *memory = (nk_byte*)nk_buffer_memory(list->buffer);
        nk_size total = nk_buffer_total(list->buffer);
        memory = nk_ptr_add(nk_byte, memory, total);
        list->cmd_offset = (nk_size)(memory - (nk_byte*)cmd);
    }

    cmd->strength = 0;
    cmd->elem_count = 0;
    cmd->clip_rect = clip;
    cmd->texture = texture;
#ifdef NK_INCLUDE_COMMAND_USERDATA
    cmd->userdata = list->userdata;
#endif

    list->cmd_count++;
    list->clip_rect = clip;
    return cmd;
}
NK_INTERN struct nk_draw_command*
nk_draw_list_command_last(struct nk_draw_list *list)
{
    void *memory;
    nk_size size;
    struct nk_draw_command *cmd;
    NK_ASSERT(list->cmd_count);

    memory = nk_buffer_memory(list->buffer);
    size = nk_buffer_total(list->buffer);
    cmd = nk_ptr_add(struct nk_draw_command, memory, size - list->cmd_offset);
    return (cmd - (list->cmd_count-1));
}
NK_INTERN void
nk_draw_list_add_clip(struct nk_draw_list *list, struct nk_rect rect)
{
    NK_ASSERT(list);
    if (!list) return;
    if (!list->cmd_count) {
        nk_draw_list_push_command(list, rect, list->config.tex_null.texture);
    } else {
        struct nk_draw_command *prev = nk_draw_list_command_last(list);
        if (prev->elem_count == 0)
            prev->clip_rect = rect;
        nk_draw_list_push_command(list, rect, prev->texture);
    }
}
NK_INTERN void
nk_draw_list_push_image(struct nk_draw_list *list, nk_handle texture)
{
    NK_ASSERT(list);
    if (!list) return;
    if (!list->cmd_count) {
        nk_draw_list_push_command(list, nk_null_rect, texture);
    } else {
        struct nk_draw_command *prev = nk_draw_list_command_last(list);
        if (prev->elem_count == 0) {
            prev->texture = texture;
            #ifdef NK_INCLUDE_COMMAND_USERDATA
            prev->userdata = list->userdata;
            #endif
        } else if (prev->texture.id != texture.id
                #ifdef NK_INCLUDE_COMMAND_USERDATA
                || prev->userdata.id != list->userdata.id
                #endif
                ) {
            nk_draw_list_push_command(list, prev->clip_rect, texture);
        }
    }
}
#ifdef NK_INCLUDE_COMMAND_USERDATA
NK_API void
nk_draw_list_push_userdata(struct nk_draw_list *list, nk_handle userdata)
{
    list->userdata = userdata;
}
#endif
NK_INTERN void*
nk_draw_list_alloc_vertices(struct nk_draw_list *list, nk_size count)
{
    void *vtx;
    NK_ASSERT(list);
    if (!list) return 0;
    vtx = nk_buffer_alloc(list->vertices, NK_BUFFER_FRONT,
        list->config.vertex_size*count, list->config.vertex_alignment);
    if (!vtx) return 0;
    list->vertex_count += (unsigned int)count;

    /* This assert triggers because your are drawing a lot of stuff and nuklear
     * defined `nk_draw_index` as `nk_ushort` to safe space be default.
     *
     * So you reached the maximum number of indices or rather vertexes.
     * To solve this issue please change typedef `nk_draw_index` to `nk_uint`
     * and don't forget to specify the new element size in your drawing
     * backend (OpenGL, DirectX, ...). For example in OpenGL for `glDrawElements`
     * instead of specifying `GL_UNSIGNED_SHORT` you have to define `GL_UNSIGNED_INT`.
     * Sorry for the inconvenience. */
    if(sizeof(nk_draw_index)==2) NK_ASSERT((list->vertex_count < NK_USHORT_MAX &&
        "To many vertices for 16-bit vertex indices. Please read comment above on how to solve this problem"));
    return vtx;
}
NK_INTERN nk_draw_index*
nk_draw_list_alloc_elements(struct nk_draw_list *list, nk_size count)
{
    nk_draw_index *ids;
    struct nk_draw_command *cmd;
    NK_STORAGE const nk_size elem_align = NK_ALIGNOF(nk_draw_index);
    NK_STORAGE const nk_size elem_size = sizeof(nk_draw_index);
    NK_ASSERT(list);
    if (!list) return 0;

    ids = (nk_draw_index*)
        nk_buffer_alloc(list->elements, NK_BUFFER_FRONT, elem_size*count, elem_align);
    if (!ids) return 0;
    cmd = nk_draw_list_command_last(list);
    list->element_count += (unsigned int)count;
    cmd->elem_count += (unsigned int)count;
    return ids;
}
NK_INTERN int
nk_draw_vertex_layout_element_is_end_of_layout(
    const struct nk_draw_vertex_layout_element *element)
{
    return (element->attribute == NK_VERTEX_ATTRIBUTE_COUNT ||
            element->format == NK_FORMAT_COUNT);
}
NK_INTERN void
nk_draw_vertex_color(void *attr, const float *vals,
    enum nk_draw_vertex_layout_format format)
{
    /* if this triggers you tried to provide a value format for a color */
    float val[4];
    NK_ASSERT(format >= NK_FORMAT_COLOR_BEGIN);
    NK_ASSERT(format <= NK_FORMAT_COLOR_END);
    if (format < NK_FORMAT_COLOR_BEGIN || format > NK_FORMAT_COLOR_END) return;

    val[0] = NK_SATURATE(vals[0]);
    val[1] = NK_SATURATE(vals[1]);
    val[2] = NK_SATURATE(vals[2]);
    val[3] = NK_SATURATE(vals[3]);

    switch (format) {
    default: NK_ASSERT(0 && "Invalid vertex layout color format"); break;
    case NK_FORMAT_R8G8B8A8:
    case NK_FORMAT_R8G8B8: {
        struct nk_color col = nk_rgba_fv(val);
        NK_MEMCPY(attr, &col.r, sizeof(col));
    } break;
    case NK_FORMAT_B8G8R8A8: {
        struct nk_color col = nk_rgba_fv(val);
        struct nk_color bgra = nk_rgba(col.b, col.g, col.r, col.a);
        NK_MEMCPY(attr, &bgra, sizeof(bgra));
    } break;
    case NK_FORMAT_R16G15B16: {
        nk_ushort col[3];
        col[0] = (nk_ushort)(val[0]*(float)NK_USHORT_MAX);
        col[1] = (nk_ushort)(val[1]*(float)NK_USHORT_MAX);
        col[2] = (nk_ushort)(val[2]*(float)NK_USHORT_MAX);
        NK_MEMCPY(attr, col, sizeof(col));
    } break;
    case NK_FORMAT_R16G15B16A16: {
        nk_ushort col[4];
        col[0] = (nk_ushort)(val[0]*(float)NK_USHORT_MAX);
        col[1] = (nk_ushort)(val[1]*(float)NK_USHORT_MAX);
        col[2] = (nk_ushort)(val[2]*(float)NK_USHORT_MAX);
        col[3] = (nk_ushort)(val[3]*(float)NK_USHORT_MAX);
        NK_MEMCPY(attr, col, sizeof(col));
    } break;
    case NK_FORMAT_R32G32B32: {
        nk_uint col[3];
        col[0] = (nk_uint)(val[0]*(float)NK_UINT_MAX);
        col[1] = (nk_uint)(val[1]*(float)NK_UINT_MAX);
        col[2] = (nk_uint)(val[2]*(float)NK_UINT_MAX);
        NK_MEMCPY(attr, col, sizeof(col));
    } break;
    case NK_FORMAT_R32G32B32A32: {
        nk_uint col[4];
        col[0] = (nk_uint)(val[0]*(float)NK_UINT_MAX);
        col[1] = (nk_uint)(val[1]*(float)NK_UINT_MAX);
        col[2] = (nk_uint)(val[2]*(float)NK_UINT_MAX);
        col[3] = (nk_uint)(val[3]*(float)NK_UINT_MAX);
        NK_MEMCPY(attr, col, sizeof(col));
    } break;
    case NK_FORMAT_R32G32B32A32_FLOAT:
        NK_MEMCPY(attr, val, sizeof(float)*4);
        break;
    case NK_FORMAT_R32G32B32A32_DOUBLE: {
        double col[4];
        col[0] = (double)val[0];
        col[1] = (double)val[1];
        col[2] = (double)val[2];
        col[3] = (double)val[3];
        NK_MEMCPY(attr, col, sizeof(col));
    } break;
    case NK_FORMAT_RGB32:
    case NK_FORMAT_RGBA32: {
        struct nk_color col = nk_rgba_fv(val);
        nk_uint color = nk_color_u32(col);
        NK_MEMCPY(attr, &color, sizeof(color));
    } break; }
}
NK_INTERN void
nk_draw_vertex_element(void *dst, const float *values, int value_count,
    enum nk_draw_vertex_layout_format format)
{
    int value_index;
    void *attribute = dst;
    /* if this triggers you tried to provide a color format for a value */
    NK_ASSERT(format < NK_FORMAT_COLOR_BEGIN);
    if (format >= NK_FORMAT_COLOR_BEGIN && format <= NK_FORMAT_COLOR_END) return;
    for (value_index = 0; value_index < value_count; ++value_index) {
        switch (format) {
        default: NK_ASSERT(0 && "invalid vertex layout format"); break;
        case NK_FORMAT_SCHAR: {
            char value = (char)NK_CLAMP((float)NK_SCHAR_MIN, values[value_index], (float)NK_SCHAR_MAX);
            NK_MEMCPY(attribute, &value, sizeof(value));
            attribute = (void*)((char*)attribute + sizeof(char));
        } break;
        case NK_FORMAT_SSHORT: {
            nk_short value = (nk_short)NK_CLAMP((float)NK_SSHORT_MIN, values[value_index], (float)NK_SSHORT_MAX);
            NK_MEMCPY(attribute, &value, sizeof(value));
            attribute = (void*)((char*)attribute + sizeof(value));
        } break;
        case NK_FORMAT_SINT: {
            nk_int value = (nk_int)NK_CLAMP((float)NK_SINT_MIN, values[value_index], (float)NK_SINT_MAX);
            NK_MEMCPY(attribute, &value, sizeof(value));
            attribute = (void*)((char*)attribute + sizeof(nk_int));
        } break;
        case NK_FORMAT_UCHAR: {
            unsigned char value = (unsigned char)NK_CLAMP((float)NK_UCHAR_MIN, values[value_index], (float)NK_UCHAR_MAX);
            NK_MEMCPY(attribute, &value, sizeof(value));
            attribute = (void*)((char*)attribute + sizeof(unsigned char));
        } break;
        case NK_FORMAT_USHORT: {
            nk_ushort value = (nk_ushort)NK_CLAMP((float)NK_USHORT_MIN, values[value_index], (float)NK_USHORT_MAX);
            NK_MEMCPY(attribute, &value, sizeof(value));
            attribute = (void*)((char*)attribute + sizeof(value));
            } break;
        case NK_FORMAT_UINT: {
            nk_uint value = (nk_uint)NK_CLAMP((float)NK_UINT_MIN, values[value_index], (float)NK_UINT_MAX);
            NK_MEMCPY(attribute, &value, sizeof(value));
            attribute = (void*)((char*)attribute + sizeof(nk_uint));
        } break;
        case NK_FORMAT_FLOAT:
            NK_MEMCPY(attribute, &values[value_index], sizeof(values[value_index]));
            attribute = (void*)((char*)attribute + sizeof(float));
            break;
        case NK_FORMAT_DOUBLE: {
            double value = (double)values[value_index];
            NK_MEMCPY(attribute, &value, sizeof(value));
            attribute = (void*)((char*)attribute + sizeof(double));
            } break;
        }
    }
}
NK_INTERN void*
nk_draw_vertex(void *dst, const struct nk_convert_config *config,
    struct nk_vec2 pos, struct nk_vec2 uv, struct nk_colorf color)
{
    void *result = (void*)((char*)dst + config->vertex_size);
    const struct nk_draw_vertex_layout_element *elem_iter = config->vertex_layout;
    while (!nk_draw_vertex_layout_element_is_end_of_layout(elem_iter)) {
        void *address = (void*)((char*)dst + elem_iter->offset);
        switch (elem_iter->attribute) {
        case NK_VERTEX_ATTRIBUTE_COUNT:
        default: NK_ASSERT(0 && "wrong element attribute"); break;
        case NK_VERTEX_POSITION: nk_draw_vertex_element(address, &pos.x, 2, elem_iter->format); break;
        case NK_VERTEX_TEXCOORD: nk_draw_vertex_element(address, &uv.x, 2, elem_iter->format); break;
        case NK_VERTEX_COLOR: nk_draw_vertex_color(address, &color.r, elem_iter->format); break;
        }
        elem_iter++;
    }
    return result;
}
NK_API void
nk_draw_list_stroke_poly_line(struct nk_draw_list *list, const struct nk_vec2 *points,
    const unsigned int points_count, struct nk_color color, enum nk_draw_list_stroke closed,
    float thickness, enum nk_anti_aliasing aliasing)
{
    nk_size count;
    int thick_line;
    struct nk_colorf col;
    struct nk_colorf col_trans;
    NK_ASSERT(list);
    if (!list || points_count < 2) return;

    color.a = (nk_byte)((float)color.a * list->config.global_alpha);
    count = points_count;
    if (!closed) count = points_count-1;
    thick_line = thickness > 1.0f;

#ifdef NK_INCLUDE_COMMAND_USERDATA
    nk_draw_list_push_userdata(list, list->userdata);
#endif

    color.a = (nk_byte)((float)color.a * list->config.global_alpha);
    nk_color_fv(&col.r, color);
    col_trans = col;
    col_trans.a = 0;

    if (aliasing == NK_ANTI_ALIASING_ON) {
        /* ANTI-ALIASED STROKE */
        const float AA_SIZE = 1.0f;
        NK_STORAGE const nk_size pnt_align = NK_ALIGNOF(struct nk_vec2);
        NK_STORAGE const nk_size pnt_size = sizeof(struct nk_vec2);

        /* allocate vertices and elements  */
        nk_size i1 = 0;
        nk_size vertex_offset;
        nk_size index = list->vertex_count;

        const nk_size idx_count = (thick_line) ?  (count * 18) : (count * 12);
        const nk_size vtx_count = (thick_line) ? (points_count * 4): (points_count *3);

        void *vtx = nk_draw_list_alloc_vertices(list, vtx_count);
        nk_draw_index *ids = nk_draw_list_alloc_elements(list, idx_count);

        nk_size size;
        struct nk_vec2 *normals, *temp;
        if (!vtx || !ids) return;

        /* temporary allocate normals + points */
        vertex_offset = (nk_size)((nk_byte*)vtx - (nk_byte*)list->vertices->memory.ptr);
        nk_buffer_mark(list->vertices, NK_BUFFER_FRONT);
        size = pnt_size * ((thick_line) ? 5 : 3) * points_count;
        normals = (struct nk_vec2*) nk_buffer_alloc(list->vertices, NK_BUFFER_FRONT, size, pnt_align);
        if (!normals) return;
        temp = normals + points_count;

        /* make sure vertex pointer is still correct */
        vtx = (void*)((nk_byte*)list->vertices->memory.ptr + vertex_offset);

        /* calculate normals */
        for (i1 = 0; i1 < count; ++i1) {
            const nk_size i2 = ((i1 + 1) == points_count) ? 0 : (i1 + 1);
            struct nk_vec2 diff = nk_vec2_sub(points[i2], points[i1]);
            float len;

            /* vec2 inverted length  */
            len = nk_vec2_len_sqr(diff);
            if (len != 0.0f)
                len = NK_INV_SQRT(len);
            else len = 1.0f;

            diff = nk_vec2_muls(diff, len);
            normals[i1].x = diff.y;
            normals[i1].y = -diff.x;
        }

        if (!closed)
            normals[points_count-1] = normals[points_count-2];

        if (!thick_line) {
            nk_size idx1, i;
            if (!closed) {
                struct nk_vec2 d;
                temp[0] = nk_vec2_add(points[0], nk_vec2_muls(normals[0], AA_SIZE));
                temp[1] = nk_vec2_sub(points[0], nk_vec2_muls(normals[0], AA_SIZE));
                d = nk_vec2_muls(normals[points_count-1], AA_SIZE);
                temp[(points_count-1) * 2 + 0] = nk_vec2_add(points[points_count-1], d);
                temp[(points_count-1) * 2 + 1] = nk_vec2_sub(points[points_count-1], d);
            }

            /* fill elements */
            idx1 = index;
            for (i1 = 0; i1 < count; i1++) {
                struct nk_vec2 dm;
                float dmr2;
                nk_size i2 = ((i1 + 1) == points_count) ? 0 : (i1 + 1);
                nk_size idx2 = ((i1+1) == points_count) ? index: (idx1 + 3);

                /* average normals */
                dm = nk_vec2_muls(nk_vec2_add(normals[i1], normals[i2]), 0.5f);
                dmr2 = dm.x * dm.x + dm.y* dm.y;
                if (dmr2 > 0.000001f) {
                    float scale = 1.0f/dmr2;
                    scale = NK_MIN(100.0f, scale);
                    dm = nk_vec2_muls(dm, scale);
                }

                dm = nk_vec2_muls(dm, AA_SIZE);
                temp[i2*2+0] = nk_vec2_add(points[i2], dm);
                temp[i2*2+1] = nk_vec2_sub(points[i2], dm);

                ids[0] = (nk_draw_index)(idx2 + 0); ids[1] = (nk_draw_index)(idx1+0);
                ids[2] = (nk_draw_index)(idx1 + 2); ids[3] = (nk_draw_index)(idx1+2);
                ids[4] = (nk_draw_index)(idx2 + 2); ids[5] = (nk_draw_index)(idx2+0);
                ids[6] = (nk_draw_index)(idx2 + 1); ids[7] = (nk_draw_index)(idx1+1);
                ids[8] = (nk_draw_index)(idx1 + 0); ids[9] = (nk_draw_index)(idx1+0);
                ids[10]= (nk_draw_index)(idx2 + 0); ids[11]= (nk_draw_index)(idx2+1);
                ids += 12;
                idx1 = idx2;
            }

            /* fill vertices */
            for (i = 0; i < points_count; ++i) {
                const struct nk_vec2 uv = list->config.tex_null.uv;
                vtx = nk_draw_vertex(vtx, &list->config, points[i], uv, col);
                vtx = nk_draw_vertex(vtx, &list->config, temp[i*2+0], uv, col_trans);
                vtx = nk_draw_vertex(vtx, &list->config, temp[i*2+1], uv, col_trans);
            }
        } else {
            nk_size idx1, i;
            const float half_inner_thickness = (thickness - AA_SIZE) * 0.5f;
            if (!closed) {
                struct nk_vec2 d1 = nk_vec2_muls(normals[0], half_inner_thickness + AA_SIZE);
                struct nk_vec2 d2 = nk_vec2_muls(normals[0], half_inner_thickness);

                temp[0] = nk_vec2_add(points[0], d1);
                temp[1] = nk_vec2_add(points[0], d2);
                temp[2] = nk_vec2_sub(points[0], d2);
                temp[3] = nk_vec2_sub(points[0], d1);

                d1 = nk_vec2_muls(normals[points_count-1], half_inner_thickness + AA_SIZE);
                d2 = nk_vec2_muls(normals[points_count-1], half_inner_thickness);

                temp[(points_count-1)*4+0] = nk_vec2_add(points[points_count-1], d1);
                temp[(points_count-1)*4+1] = nk_vec2_add(points[points_count-1], d2);
                temp[(points_count-1)*4+2] = nk_vec2_sub(points[points_count-1], d2);
                temp[(points_count-1)*4+3] = nk_vec2_sub(points[points_count-1], d1);
            }

            /* add all elements */
            idx1 = index;
            for (i1 = 0; i1 < count; ++i1) {
                struct nk_vec2 dm_out, dm_in;
                const nk_size i2 = ((i1+1) == points_count) ? 0: (i1 + 1);
                nk_size idx2 = ((i1+1) == points_count) ? index: (idx1 + 4);

                /* average normals */
                struct nk_vec2 dm = nk_vec2_muls(nk_vec2_add(normals[i1], normals[i2]), 0.5f);
                float dmr2 = dm.x * dm.x + dm.y* dm.y;
                if (dmr2 > 0.000001f) {
                    float scale = 1.0f/dmr2;
                    scale = NK_MIN(100.0f, scale);
                    dm = nk_vec2_muls(dm, scale);
                }

                dm_out = nk_vec2_muls(dm, ((half_inner_thickness) + AA_SIZE));
                dm_in = nk_vec2_muls(dm, half_inner_thickness);
                temp[i2*4+0] = nk_vec2_add(points[i2], dm_out);
                temp[i2*4+1] = nk_vec2_add(points[i2], dm_in);
                temp[i2*4+2] = nk_vec2_sub(points[i2], dm_in);
                temp[i2*4+3] = nk_vec2_sub(points[i2], dm_out);

                /* add indexes */
                ids[0] = (nk_draw_index)(idx2 + 1); ids[1] = (nk_draw_index)(idx1+1);
                ids[2] = (nk_draw_index)(idx1 + 2); ids[3] = (nk_draw_index)(idx1+2);
                ids[4] = (nk_draw_index)(idx2 + 2); ids[5] = (nk_draw_index)(idx2+1);
                ids[6] = (nk_draw_index)(idx2 + 1); ids[7] = (nk_draw_index)(idx1+1);
                ids[8] = (nk_draw_index)(idx1 + 0); ids[9] = (nk_draw_index)(idx1+0);
                ids[10]= (nk_draw_index)(idx2 + 0); ids[11] = (nk_draw_index)(idx2+1);
                ids[12]= (nk_draw_index)(idx2 + 2); ids[13] = (nk_draw_index)(idx1+2);
                ids[14]= (nk_draw_index)(idx1 + 3); ids[15] = (nk_draw_index)(idx1+3);
                ids[16]= (nk_draw_index)(idx2 + 3); ids[17] = (nk_draw_index)(idx2+2);
                ids += 18;
                idx1 = idx2;
            }

            /* add vertices */
            for (i = 0; i < points_count; ++i) {
                const struct nk_vec2 uv = list->config.tex_null.uv;
                vtx = nk_draw_vertex(vtx, &list->config, temp[i*4+0], uv, col_trans);
                vtx = nk_draw_vertex(vtx, &list->config, temp[i*4+1], uv, col);
                vtx = nk_draw_vertex(vtx, &list->config, temp[i*4+2], uv, col);
                vtx = nk_draw_vertex(vtx, &list->config, temp[i*4+3], uv, col_trans);
            }
        }
        /* free temporary normals + points */
        nk_buffer_reset(list->vertices, NK_BUFFER_FRONT);
    } else {
        /* NON ANTI-ALIASED STROKE */
        nk_size i1 = 0;
        nk_size idx = list->vertex_count;
        const nk_size idx_count = count * 6;
        const nk_size vtx_count = count * 4;
        void *vtx = nk_draw_list_alloc_vertices(list, vtx_count);
        nk_draw_index *ids = nk_draw_list_alloc_elements(list, idx_count);
        if (!vtx || !ids) return;

        for (i1 = 0; i1 < count; ++i1) {
            float dx, dy;
            const struct nk_vec2 uv = list->config.tex_null.uv;
            const nk_size i2 = ((i1+1) == points_count) ? 0 : i1 + 1;
            const struct nk_vec2 p1 = points[i1];
            const struct nk_vec2 p2 = points[i2];
            struct nk_vec2 diff = nk_vec2_sub(p2, p1);
            float len;

            /* vec2 inverted length  */
            len = nk_vec2_len_sqr(diff);
            if (len != 0.0f)
                len = NK_INV_SQRT(len);
            else len = 1.0f;
            diff = nk_vec2_muls(diff, len);

            /* add vertices */
            dx = diff.x * (thickness * 0.5f);
            dy = diff.y * (thickness * 0.5f);

            vtx = nk_draw_vertex(vtx, &list->config, nk_vec2(p1.x + dy, p1.y - dx), uv, col);
            vtx = nk_draw_vertex(vtx, &list->config, nk_vec2(p2.x + dy, p2.y - dx), uv, col);
            vtx = nk_draw_vertex(vtx, &list->config, nk_vec2(p2.x - dy, p2.y + dx), uv, col);
            vtx = nk_draw_vertex(vtx, &list->config, nk_vec2(p1.x - dy, p1.y + dx), uv, col);

            ids[0] = (nk_draw_index)(idx+0); ids[1] = (nk_draw_index)(idx+1);
            ids[2] = (nk_draw_index)(idx+2); ids[3] = (nk_draw_index)(idx+0);
            ids[4] = (nk_draw_index)(idx+2); ids[5] = (nk_draw_index)(idx+3);

            ids += 6;
            idx += 4;
        }
    }
}
NK_API void
nk_draw_list_fill_poly_convex(struct nk_draw_list *list,
    const struct nk_vec2 *points, const unsigned int points_count,
    struct nk_color color, enum nk_anti_aliasing aliasing)
{
    struct nk_colorf col;
    struct nk_colorf col_trans;

    NK_STORAGE const nk_size pnt_align = NK_ALIGNOF(struct nk_vec2);
    NK_STORAGE const nk_size pnt_size = sizeof(struct nk_vec2);
    NK_ASSERT(list);
    if (!list || points_count < 3) return;

#ifdef NK_INCLUDE_COMMAND_USERDATA
    nk_draw_list_push_userdata(list, list->userdata);
#endif

    color.a = (nk_byte)((float)color.a * list->config.global_alpha);
    nk_color_fv(&col.r, color);
    col_trans = col;
    col_trans.a = 0;

    if (aliasing == NK_ANTI_ALIASING_ON) {
        nk_size i = 0;
        nk_size i0 = 0;
        nk_size i1 = 0;

        const float AA_SIZE = 1.0f;
        nk_size vertex_offset = 0;
        nk_size index = list->vertex_count;

        const nk_size idx_count = (points_count-2)*3 + points_count*6;
        const nk_size vtx_count = (points_count*2);

        void *vtx = nk_draw_list_alloc_vertices(list, vtx_count);
        nk_draw_index *ids = nk_draw_list_alloc_elements(list, idx_count);

        nk_size size = 0;
        struct nk_vec2 *normals = 0;
        unsigned int vtx_inner_idx = (unsigned int)(index + 0);
        unsigned int vtx_outer_idx = (unsigned int)(index + 1);
        if (!vtx || !ids) return;

        /* temporary allocate normals */
        vertex_offset = (nk_size)((nk_byte*)vtx - (nk_byte*)list->vertices->memory.ptr);
        nk_buffer_mark(list->vertices, NK_BUFFER_FRONT);
        size = pnt_size * points_count;
        normals = (struct nk_vec2*) nk_buffer_alloc(list->vertices, NK_BUFFER_FRONT, size, pnt_align);
        if (!normals) return;
        vtx = (void*)((nk_byte*)list->vertices->memory.ptr + vertex_offset);

        /* add elements */
        for (i = 2; i < points_count; i++) {
            ids[0] = (nk_draw_index)(vtx_inner_idx);
            ids[1] = (nk_draw_index)(vtx_inner_idx + ((i-1) << 1));
            ids[2] = (nk_draw_index)(vtx_inner_idx + (i << 1));
            ids += 3;
        }

        /* compute normals */
        for (i0 = points_count-1, i1 = 0; i1 < points_count; i0 = i1++) {
            struct nk_vec2 p0 = points[i0];
            struct nk_vec2 p1 = points[i1];
            struct nk_vec2 diff = nk_vec2_sub(p1, p0);

            /* vec2 inverted length  */
            float len = nk_vec2_len_sqr(diff);
            if (len != 0.0f)
                len = NK_INV_SQRT(len);
            else len = 1.0f;
            diff = nk_vec2_muls(diff, len);

            normals[i0].x = diff.y;
            normals[i0].y = -diff.x;
        }

        /* add vertices + indexes */
        for (i0 = points_count-1, i1 = 0; i1 < points_count; i0 = i1++) {
            const struct nk_vec2 uv = list->config.tex_null.uv;
            struct nk_vec2 n0 = normals[i0];
            struct nk_vec2 n1 = normals[i1];
            struct nk_vec2 dm = nk_vec2_muls(nk_vec2_add(n0, n1), 0.5f);
            float dmr2 = dm.x*dm.x + dm.y*dm.y;
            if (dmr2 > 0.000001f) {
                float scale = 1.0f / dmr2;
                scale = NK_MIN(scale, 100.0f);
                dm = nk_vec2_muls(dm, scale);
            }
            dm = nk_vec2_muls(dm, AA_SIZE * 0.5f);

            /* add vertices */
            vtx = nk_draw_vertex(vtx, &list->config, nk_vec2_sub(points[i1], dm), uv, col);
            vtx = nk_draw_vertex(vtx, &list->config, nk_vec2_add(points[i1], dm), uv, col_trans);

            /* add indexes */
            ids[0] = (nk_draw_index)(vtx_inner_idx+(i1<<1));
            ids[1] = (nk_draw_index)(vtx_inner_idx+(i0<<1));
            ids[2] = (nk_draw_index)(vtx_outer_idx+(i0<<1));
            ids[3] = (nk_draw_index)(vtx_outer_idx+(i0<<1));
            ids[4] = (nk_draw_index)(vtx_outer_idx+(i1<<1));
            ids[5] = (nk_draw_index)(vtx_inner_idx+(i1<<1));
            ids += 6;
        }
        /* free temporary normals + points */
        nk_buffer_reset(list->vertices, NK_BUFFER_FRONT);
    } else {
        nk_size i = 0;
        nk_size index = list->vertex_count;
        const nk_size idx_count = (points_count-2)*3;
        const nk_size vtx_count = points_count;
        void *vtx = nk_draw_list_alloc_vertices(list, vtx_count);
        nk_draw_index *ids = nk_draw_list_alloc_elements(list, idx_count);

        if (!vtx || !ids) return;
        for (i = 0; i < vtx_count; ++i)
            vtx = nk_draw_vertex(vtx, &list->config, points[i], list->config.tex_null.uv, col);
        for (i = 2; i < points_count; ++i) {
            ids[0] = (nk_draw_index)index;
            ids[1] = (nk_draw_index)(index+ i - 1);
            ids[2] = (nk_draw_index)(index+i);
            ids += 3;
        }
    }
}
NK_API void
nk_draw_list_path_clear(struct nk_draw_list *list)
{
    NK_ASSERT(list);
    if (!list) return;
    nk_buffer_reset(list->buffer, NK_BUFFER_FRONT);
    list->path_count = 0;
    list->path_offset = 0;
}
NK_API void
nk_draw_list_path_line_to(struct nk_draw_list *list, struct nk_vec2 pos)
{
    struct nk_vec2 *points = 0;
    struct nk_draw_command *cmd = 0;
    NK_ASSERT(list);
    if (!list) return;
    if (!list->cmd_count)
        nk_draw_list_add_clip(list, nk_null_rect);

    cmd = nk_draw_list_command_last(list);
    if (cmd && cmd->texture.ptr != list->config.tex_null.texture.ptr)
        nk_draw_list_push_image(list, list->config.tex_null.texture);

    points = nk_draw_list_alloc_path(list, 1);
    if (!points) return;
    points[0] = pos;
}
NK_API void
nk_draw_list_path_arc_to_fast(struct nk_draw_list *list, struct nk_vec2 center,
    float radius, int a_min, int a_max)
{
    int a = 0;
    NK_ASSERT(list);
    if (!list) return;
    if (a_min <= a_max) {
        for (a = a_min; a <= a_max; a++) {
            const struct nk_vec2 c = list->circle_vtx[(nk_size)a % NK_LEN(list->circle_vtx)];
            const float x = center.x + c.x * radius;
            const float y = center.y + c.y * radius;
            nk_draw_list_path_line_to(list, nk_vec2(x, y));
        }
    }
}
NK_API void
nk_draw_list_path_arc_to(struct nk_draw_list *list, struct nk_vec2 center,
    float radius, float a_min, float a_max, unsigned int segments)
{
    unsigned int i = 0;
    NK_ASSERT(list);
    if (!list) return;
    if (radius == 0.0f) return;

    /*  This algorithm for arc drawing relies on these two trigonometric identities[1]:
            sin(a + b) = sin(a) * cos(b) + cos(a) * sin(b)
            cos(a + b) = cos(a) * cos(b) - sin(a) * sin(b)

        Two coordinates (x, y) of a point on a circle centered on
        the origin can be written in polar form as:
            x = r * cos(a)
            y = r * sin(a)
        where r is the radius of the circle,
            a is the angle between (x, y) and the origin.

        This allows us to rotate the coordinates around the
        origin by an angle b using the following transformation:
            x' = r * cos(a + b) = x * cos(b) - y * sin(b)
            y' = r * sin(a + b) = y * cos(b) + x * sin(b)

        [1] https://en.wikipedia.org/wiki/List_of_trigonometric_identities#Angle_sum_and_difference_identities
    */
    {const float d_angle = (a_max - a_min) / (float)segments;
    const float sin_d = (float)NK_SIN(d_angle);
    const float cos_d = (float)NK_COS(d_angle);

    float cx = (float)NK_COS(a_min) * radius;
    float cy = (float)NK_SIN(a_min) * radius;
    for(i = 0; i <= segments; ++i) {
        float new_cx, new_cy;
        const float x = center.x + cx;
        const float y = center.y + cy;
        nk_draw_list_path_line_to(list, nk_vec2(x, y));

        new_cx = cx * cos_d - cy * sin_d;
        new_cy = cy * cos_d + cx * sin_d;
        cx = new_cx;
        cy = new_cy;
    }}
}
NK_API void
nk_draw_list_path_rect_to(struct nk_draw_list *list, struct nk_vec2 a,
    struct nk_vec2 b, float rounding)
{
    float r;
    NK_ASSERT(list);
    if (!list) return;
    r = rounding;
    r = NK_MIN(r, ((b.x-a.x) < 0) ? -(b.x-a.x): (b.x-a.x));
    r = NK_MIN(r, ((b.y-a.y) < 0) ? -(b.y-a.y): (b.y-a.y));

    if (r == 0.0f) {
        nk_draw_list_path_line_to(list, a);
        nk_draw_list_path_line_to(list, nk_vec2(b.x,a.y));
        nk_draw_list_path_line_to(list, b);
        nk_draw_list_path_line_to(list, nk_vec2(a.x,b.y));
    } else {
        nk_draw_list_path_arc_to_fast(list, nk_vec2(a.x + r, a.y + r), r, 6, 9);
        nk_draw_list_path_arc_to_fast(list, nk_vec2(b.x - r, a.y + r), r, 9, 12);
        nk_draw_list_path_arc_to_fast(list, nk_vec2(b.x - r, b.y - r), r, 0, 3);
        nk_draw_list_path_arc_to_fast(list, nk_vec2(a.x + r, b.y - r), r, 3, 6);
    }
}
NK_API void
nk_draw_list_path_curve_to(struct nk_draw_list *list, struct nk_vec2 p2,
    struct nk_vec2 p3, struct nk_vec2 p4, unsigned int num_segments)
{
    float t_step;
    unsigned int i_step;
    struct nk_vec2 p1;

    NK_ASSERT(list);
    NK_ASSERT(list->path_count);
    if (!list || !list->path_count) return;
    num_segments = NK_MAX(num_segments, 1);

    p1 = nk_draw_list_path_last(list);
    t_step = 1.0f/(float)num_segments;
    for (i_step = 1; i_step <= num_segments; ++i_step) {
        float t = t_step * (float)i_step;
        float u = 1.0f - t;
        float w1 = u*u*u;
        float w2 = 3*u*u*t;
        float w3 = 3*u*t*t;
        float w4 = t * t *t;
        float x = w1 * p1.x + w2 * p2.x + w3 * p3.x + w4 * p4.x;
        float y = w1 * p1.y + w2 * p2.y + w3 * p3.y + w4 * p4.y;
        nk_draw_list_path_line_to(list, nk_vec2(x,y));
    }
}
NK_API void
nk_draw_list_path_fill(struct nk_draw_list *list, struct nk_color color)
{
    struct nk_vec2 *points;
    NK_ASSERT(list);
    if (!list) return;
    points = (struct nk_vec2*)nk_buffer_memory(list->buffer);
    nk_draw_list_fill_poly_convex(list, points, list->path_count, color, list->config.shape_AA);
    nk_draw_list_path_clear(list);
}
NK_API void
nk_draw_list_path_stroke(struct nk_draw_list *list, struct nk_color color,
    enum nk_draw_list_stroke closed, float thickness)
{
    struct nk_vec2 *points;
    NK_ASSERT(list);
    if (!list) return;
    points = (struct nk_vec2*)nk_buffer_memory(list->buffer);
    nk_draw_list_stroke_poly_line(list, points, list->path_count, color,
        closed, thickness, list->config.line_AA);
    nk_draw_list_path_clear(list);
}
NK_API void
nk_draw_list_stroke_line(struct nk_draw_list *list, struct nk_vec2 a,
    struct nk_vec2 b, struct nk_color col, float thickness)
{
    NK_ASSERT(list);
    if (!list || !col.a) return;
    if (list->line_AA == NK_ANTI_ALIASING_ON) {
        nk_draw_list_path_line_to(list, a);
        nk_draw_list_path_line_to(list, b);
    } else {
        nk_draw_list_path_line_to(list, nk_vec2_sub(a,nk_vec2(0.5f,0.5f)));
        nk_draw_list_path_line_to(list, nk_vec2_sub(b,nk_vec2(0.5f,0.5f)));
    }
    nk_draw_list_path_stroke(list,  col, NK_STROKE_OPEN, thickness);
}
NK_API void
nk_draw_list_fill_rect(struct nk_draw_list *list, struct nk_rect rect,
    struct nk_color col, float rounding)
{
    NK_ASSERT(list);
    if (!list || !col.a) return;

    if (list->line_AA == NK_ANTI_ALIASING_ON) {
        nk_draw_list_path_rect_to(list, nk_vec2(rect.x, rect.y),
            nk_vec2(rect.x + rect.w, rect.y + rect.h), rounding);
    } else {
        nk_draw_list_path_rect_to(list, nk_vec2(rect.x-0.5f, rect.y-0.5f),
            nk_vec2(rect.x + rect.w, rect.y + rect.h), rounding);
    } nk_draw_list_path_fill(list,  col);
}
NK_API void
nk_draw_list_stroke_rect(struct nk_draw_list *list, struct nk_rect rect,
    struct nk_color col, float rounding, float thickness)
{
    NK_ASSERT(list);
    if (!list || !col.a) return;
    if (list->line_AA == NK_ANTI_ALIASING_ON) {
        nk_draw_list_path_rect_to(list, nk_vec2(rect.x, rect.y),
            nk_vec2(rect.x + rect.w, rect.y + rect.h), rounding);
    } else {
        nk_draw_list_path_rect_to(list, nk_vec2(rect.x-0.5f, rect.y-0.5f),
            nk_vec2(rect.x + rect.w, rect.y + rect.h), rounding);
    } nk_draw_list_path_stroke(list,  col, NK_STROKE_CLOSED, thickness);
}
NK_API void
nk_draw_list_fill_rect_multi_color(struct nk_draw_list *list, struct nk_rect rect,
    struct nk_color left, struct nk_color top, struct nk_color right,
    struct nk_color bottom)
{
    void *vtx;
    struct nk_colorf col_left, col_top;
    struct nk_colorf col_right, col_bottom;
    nk_draw_index *idx;
    nk_draw_index index;

    nk_color_fv(&col_left.r, left);
    nk_color_fv(&col_right.r, right);
    nk_color_fv(&col_top.r, top);
    nk_color_fv(&col_bottom.r, bottom);

    NK_ASSERT(list);
    if (!list) return;

    nk_draw_list_push_image(list, list->config.tex_null.texture);
    index = (nk_draw_index)list->vertex_count;
    vtx = nk_draw_list_alloc_vertices(list, 4);
    idx = nk_draw_list_alloc_elements(list, 6);
    if (!vtx || !idx) return;

    idx[0] = (nk_draw_index)(index+0); idx[1] = (nk_draw_index)(index+1);
    idx[2] = (nk_draw_index)(index+2); idx[3] = (nk_draw_index)(index+0);
    idx[4] = (nk_draw_index)(index+2); idx[5] = (nk_draw_index)(index+3);

    vtx = nk_draw_vertex(vtx, &list->config, nk_vec2(rect.x, rect.y), list->config.tex_null.uv, col_left);
    vtx = nk_draw_vertex(vtx, &list->config, nk_vec2(rect.x + rect.w, rect.y), list->config.tex_null.uv, col_top);
    vtx = nk_draw_vertex(vtx, &list->config, nk_vec2(rect.x + rect.w, rect.y + rect.h), list->config.tex_null.uv, col_right);
    vtx = nk_draw_vertex(vtx, &list->config, nk_vec2(rect.x, rect.y + rect.h), list->config.tex_null.uv, col_bottom);
}
NK_API void
nk_draw_list_fill_triangle(struct nk_draw_list *list, struct nk_vec2 a,
    struct nk_vec2 b, struct nk_vec2 c, struct nk_color col)
{
    NK_ASSERT(list);
    if (!list || !col.a) return;
    nk_draw_list_path_line_to(list, a);
    nk_draw_list_path_line_to(list, b);
    nk_draw_list_path_line_to(list, c);
    nk_draw_list_path_fill(list, col);
}
NK_API void
nk_draw_list_stroke_triangle(struct nk_draw_list *list, struct nk_vec2 a,
    struct nk_vec2 b, struct nk_vec2 c, struct nk_color col, float thickness)
{
    NK_ASSERT(list);
    if (!list || !col.a) return;
    nk_draw_list_path_line_to(list, a);
    nk_draw_list_path_line_to(list, b);
    nk_draw_list_path_line_to(list, c);
    nk_draw_list_path_stroke(list, col, NK_STROKE_CLOSED, thickness);
}
NK_API void
nk_draw_list_fill_circle(struct nk_draw_list *list, struct nk_vec2 center,
    float radius, struct nk_color col, unsigned int segs)
{
    float a_max;
    NK_ASSERT(list);
    if (!list || !col.a) return;
    a_max = NK_PI * 2.0f * ((float)segs - 1.0f) / (float)segs;
    nk_draw_list_path_arc_to(list, center, radius, 0.0f, a_max, segs);
    nk_draw_list_path_fill(list, col);
}
NK_API void
nk_draw_list_stroke_circle(struct nk_draw_list *list, struct nk_vec2 center,
    float radius, struct nk_color col, unsigned int segs, float thickness)
{
    float a_max;
    NK_ASSERT(list);
    if (!list || !col.a) return;
    a_max = NK_PI * 2.0f * ((float)segs - 1.0f) / (float)segs;
    nk_draw_list_path_arc_to(list, center, radius, 0.0f, a_max, segs);
    nk_draw_list_path_stroke(list, col, NK_STROKE_CLOSED, thickness);
}
NK_API void
nk_draw_list_stroke_curve(struct nk_draw_list *list, struct nk_vec2 p0,
    struct nk_vec2 cp0, struct nk_vec2 cp1, struct nk_vec2 p1,
    struct nk_color col, unsigned int segments, float thickness)
{
    NK_ASSERT(list);
    if (!list || !col.a) return;
    nk_draw_list_path_line_to(list, p0);
    nk_draw_list_path_curve_to(list, cp0, cp1, p1, segments);
    nk_draw_list_path_stroke(list, col, NK_STROKE_OPEN, thickness);
}
NK_INTERN void
nk_draw_list_push_rect_uv(struct nk_draw_list *list, struct nk_vec2 a,
    struct nk_vec2 c, struct nk_vec2 uva, struct nk_vec2 uvc,
    struct nk_color color)
{
    void *vtx;
    struct nk_vec2 uvb;
    struct nk_vec2 uvd;
    struct nk_vec2 b;
    struct nk_vec2 d;

    struct nk_colorf col;
    nk_draw_index *idx;
    nk_draw_index index;
    NK_ASSERT(list);
    if (!list) return;

    nk_color_fv(&col.r, color);
    uvb = nk_vec2(uvc.x, uva.y);
    uvd = nk_vec2(uva.x, uvc.y);
    b = nk_vec2(c.x, a.y);
    d = nk_vec2(a.x, c.y);

    index = (nk_draw_index)list->vertex_count;
    vtx = nk_draw_list_alloc_vertices(list, 4);
    idx = nk_draw_list_alloc_elements(list, 6);
    if (!vtx || !idx) return;

    idx[0] = (nk_draw_index)(index+0); idx[1] = (nk_draw_index)(index+1);
    idx[2] = (nk_draw_index)(index+2); idx[3] = (nk_draw_index)(index+0);
    idx[4] = (nk_draw_index)(index+2); idx[5] = (nk_draw_index)(index+3);

    vtx = nk_draw_vertex(vtx, &list->config, a, uva, col);
    vtx = nk_draw_vertex(vtx, &list->config, b, uvb, col);
    vtx = nk_draw_vertex(vtx, &list->config, c, uvc, col);
    vtx = nk_draw_vertex(vtx, &list->config, d, uvd, col);
}
NK_API void
nk_draw_list_add_image(struct nk_draw_list *list, struct nk_image texture,
    struct nk_rect rect, struct nk_color color)
{
    NK_ASSERT(list);
    if (!list) return;
    /* push new command with given texture */
    nk_draw_list_push_image(list, texture.handle);
    if (nk_image_is_subimage(&texture)) {
        /* add region inside of the texture  */
        struct nk_vec2 uv[2];
        uv[0].x = (float)texture.region[0]/(float)texture.w;
        uv[0].y = (float)texture.region[1]/(float)texture.h;
        uv[1].x = (float)(texture.region[0] + texture.region[2])/(float)texture.w;
        uv[1].y = (float)(texture.region[1] + texture.region[3])/(float)texture.h;
        nk_draw_list_push_rect_uv(list, nk_vec2(rect.x, rect.y),
            nk_vec2(rect.x + rect.w, rect.y + rect.h),  uv[0], uv[1], color);
    } else nk_draw_list_push_rect_uv(list, nk_vec2(rect.x, rect.y),
            nk_vec2(rect.x + rect.w, rect.y + rect.h),
            nk_vec2(0.0f, 0.0f), nk_vec2(1.0f, 1.0f),color);
}
NK_API void
nk_draw_list_add_text(struct nk_draw_list *list, const struct nk_user_font *font,
    struct nk_rect rect, const char *text, int len, float font_height,
    struct nk_color fg)
{
    float x = 0;
    int text_len = 0;
    nk_rune unicode = 0;
    nk_rune next = 0;
    int glyph_len = 0;
    int next_glyph_len = 0;
    struct nk_user_font_glyph g;

    NK_ASSERT(list);
    if (!list || !len || !text) return;
    if (!NK_INTERSECT(rect.x, rect.y, rect.w, rect.h,
        list->clip_rect.x, list->clip_rect.y, list->clip_rect.w, list->clip_rect.h)) return;

    nk_draw_list_push_image(list, font->texture);
    struct nk_draw_command *cmd = nk_draw_list_command_last(list);
    cmd->strength = 0.5f; // XXX read from nk_user_font?
    x = rect.x;
    glyph_len = nk_utf_decode(text, &unicode, len);
    if (!glyph_len) return;

    /* draw every glyph image */
    fg.a = (nk_byte)((float)fg.a * list->config.global_alpha);
    while (text_len < len && glyph_len) {
        float gx, gy, gh, gw;
        float char_width = 0;
        if (unicode == NK_UTF_INVALID) break;

        /* query currently drawn glyph information */
        next_glyph_len = nk_utf_decode(text + text_len + glyph_len, &next, (int)len - text_len);
        font->query(font->userdata, font_height, &g, unicode,
                    (next == NK_UTF_INVALID) ? '\0' : next);

        /* calculate and draw glyph drawing rectangle and image */
        gx = x + g.offset.x;
        gy = rect.y + g.offset.y;
        gw = g.width; gh = g.height;
        char_width = g.xadvance;
        nk_draw_list_push_rect_uv(list, nk_vec2(gx, gy), nk_vec2(gx + gw, gy + gh),
            g.uv[0], g.uv[1], fg);

        /* offset next glyph */
        text_len += glyph_len;
        x += char_width;
        glyph_len = next_glyph_len;
        unicode = next;
    }
}
NK_API nk_flags
nk_convert(struct nk_context *ctx, struct nk_buffer *cmds,
    struct nk_buffer *vertices, struct nk_buffer *elements,
    const struct nk_convert_config *config)
{
    nk_flags res = NK_CONVERT_SUCCESS;
    const struct nk_command *cmd;
    NK_ASSERT(ctx);
    NK_ASSERT(cmds);
    NK_ASSERT(vertices);
    NK_ASSERT(elements);
    NK_ASSERT(config);
    NK_ASSERT(config->vertex_layout);
    NK_ASSERT(config->vertex_size);
    if (!ctx || !cmds || !vertices || !elements || !config || !config->vertex_layout)
        return NK_CONVERT_INVALID_PARAM;

    nk_draw_list_setup(&ctx->draw_list, config, cmds, vertices, elements,
        config->line_AA, config->shape_AA);
    nk_foreach(cmd, ctx)
    {
#ifdef NK_INCLUDE_COMMAND_USERDATA
        ctx->draw_list.userdata = cmd->userdata;
#endif
        switch (cmd->type) {
        case NK_COMMAND_NOP: break;
        case NK_COMMAND_SCISSOR: {
            const struct nk_command_scissor *s = (const struct nk_command_scissor*)cmd;
            nk_draw_list_add_clip(&ctx->draw_list, nk_rect(s->x, s->y, s->w, s->h));
        } break;
        case NK_COMMAND_LINE: {
            const struct nk_command_line *l = (const struct nk_command_line*)cmd;
            nk_draw_list_stroke_line(&ctx->draw_list, nk_vec2(l->begin.x, l->begin.y),
                nk_vec2(l->end.x, l->end.y), l->color, l->line_thickness);
        } break;
        case NK_COMMAND_CURVE: {
            const struct nk_command_curve *q = (const struct nk_command_curve*)cmd;
            nk_draw_list_stroke_curve(&ctx->draw_list, nk_vec2(q->begin.x, q->begin.y),
                nk_vec2(q->ctrl[0].x, q->ctrl[0].y), nk_vec2(q->ctrl[1].x,
                q->ctrl[1].y), nk_vec2(q->end.x, q->end.y), q->color,
                config->curve_segment_count, q->line_thickness);
        } break;
        case NK_COMMAND_RECT: {
            const struct nk_command_rect *r = (const struct nk_command_rect*)cmd;
            nk_draw_list_stroke_rect(&ctx->draw_list, nk_rect(r->x, r->y, r->w, r->h),
                r->color, (float)r->rounding, r->line_thickness);
        } break;
        case NK_COMMAND_RECT_FILLED: {
            const struct nk_command_rect_filled *r = (const struct nk_command_rect_filled*)cmd;
            nk_draw_list_fill_rect(&ctx->draw_list, nk_rect(r->x, r->y, r->w, r->h),
                r->color, (float)r->rounding);
        } break;
        case NK_COMMAND_RECT_MULTI_COLOR: {
            const struct nk_command_rect_multi_color *r = (const struct nk_command_rect_multi_color*)cmd;
            nk_draw_list_fill_rect_multi_color(&ctx->draw_list, nk_rect(r->x, r->y, r->w, r->h),
                r->left, r->top, r->right, r->bottom);
        } break;
        case NK_COMMAND_CIRCLE: {
            const struct nk_command_circle *c = (const struct nk_command_circle*)cmd;
            nk_draw_list_stroke_circle(&ctx->draw_list, nk_vec2((float)c->x + (float)c->w/2,
                (float)c->y + (float)c->h/2), (float)c->w/2, c->color,
                config->circle_segment_count, c->line_thickness);
        } break;
        case NK_COMMAND_CIRCLE_FILLED: {
            const struct nk_command_circle_filled *c = (const struct nk_command_circle_filled *)cmd;
            nk_draw_list_fill_circle(&ctx->draw_list, nk_vec2((float)c->x + (float)c->w/2,
                (float)c->y + (float)c->h/2), (float)c->w/2, c->color,
                config->circle_segment_count);
        } break;
        case NK_COMMAND_ARC: {
            const struct nk_command_arc *c = (const struct nk_command_arc*)cmd;
            nk_draw_list_path_line_to(&ctx->draw_list, nk_vec2(c->cx, c->cy));
            nk_draw_list_path_arc_to(&ctx->draw_list, nk_vec2(c->cx, c->cy), c->r,
                c->a[0], c->a[1], config->arc_segment_count);
            nk_draw_list_path_stroke(&ctx->draw_list, c->color, NK_STROKE_CLOSED, c->line_thickness);
        } break;
        case NK_COMMAND_ARC_FILLED: {
            const struct nk_command_arc_filled *c = (const struct nk_command_arc_filled*)cmd;
            nk_draw_list_path_line_to(&ctx->draw_list, nk_vec2(c->cx, c->cy));
            nk_draw_list_path_arc_to(&ctx->draw_list, nk_vec2(c->cx, c->cy), c->r,
                c->a[0], c->a[1], config->arc_segment_count);
            nk_draw_list_path_fill(&ctx->draw_list, c->color);
        } break;
        case NK_COMMAND_TRIANGLE: {
            const struct nk_command_triangle *t = (const struct nk_command_triangle*)cmd;
            nk_draw_list_stroke_triangle(&ctx->draw_list, nk_vec2(t->a.x, t->a.y),
                nk_vec2(t->b.x, t->b.y), nk_vec2(t->c.x, t->c.y), t->color,
                t->line_thickness);
        } break;
        case NK_COMMAND_TRIANGLE_FILLED: {
            const struct nk_command_triangle_filled *t = (const struct nk_command_triangle_filled*)cmd;
            nk_draw_list_fill_triangle(&ctx->draw_list, nk_vec2(t->a.x, t->a.y),
                nk_vec2(t->b.x, t->b.y), nk_vec2(t->c.x, t->c.y), t->color);
        } break;
        case NK_COMMAND_POLYGON: {
            int i;
            const struct nk_command_polygon*p = (const struct nk_command_polygon*)cmd;
            for (i = 0; i < p->point_count; ++i) {
                struct nk_vec2 pnt = nk_vec2((float)p->points[i].x, (float)p->points[i].y);
                nk_draw_list_path_line_to(&ctx->draw_list, pnt);
            }
            nk_draw_list_path_stroke(&ctx->draw_list, p->color, NK_STROKE_CLOSED, p->line_thickness);
        } break;
        case NK_COMMAND_POLYGON_FILLED: {
            int i;
            const struct nk_command_polygon_filled *p = (const struct nk_command_polygon_filled*)cmd;
            for (i = 0; i < p->point_count; ++i) {
                struct nk_vec2 pnt = nk_vec2((float)p->points[i].x, (float)p->points[i].y);
                nk_draw_list_path_line_to(&ctx->draw_list, pnt);
            }
            nk_draw_list_path_fill(&ctx->draw_list, p->color);
        } break;
        case NK_COMMAND_POLYLINE: {
            int i;
            const struct nk_command_polyline *p = (const struct nk_command_polyline*)cmd;
            for (i = 0; i < p->point_count; ++i) {
                struct nk_vec2 pnt = nk_vec2((float)p->points[i].x, (float)p->points[i].y);
                nk_draw_list_path_line_to(&ctx->draw_list, pnt);
            }
            nk_draw_list_path_stroke(&ctx->draw_list, p->color, NK_STROKE_OPEN, p->line_thickness);
        } break;
        case NK_COMMAND_TEXT: {
            const struct nk_command_text *t = (const struct nk_command_text*)cmd;
            nk_draw_list_add_text(&ctx->draw_list, t->font, nk_rect(t->x, t->y, t->w, t->h),
                t->string, t->length, t->height, t->foreground);
        } break;
        case NK_COMMAND_IMAGE: {
            const struct nk_command_image *i = (const struct nk_command_image*)cmd;
            nk_draw_list_add_image(&ctx->draw_list, i->img, nk_rect(i->x, i->y, i->w, i->h), i->col);
        } break;
        case NK_COMMAND_CUSTOM: {
            const struct nk_command_custom *c = (const struct nk_command_custom*)cmd;
            c->callback(&ctx->draw_list, c->x, c->y, c->w, c->h, c->callback_data);
        } break;
        default: break;
        }
    }
    res |= (cmds->needed > cmds->allocated + (cmds->memory.size - cmds->size)) ? NK_CONVERT_COMMAND_BUFFER_FULL: 0;
    res |= (vertices->needed > vertices->allocated) ? NK_CONVERT_VERTEX_BUFFER_FULL: 0;
    res |= (elements->needed > elements->allocated) ? NK_CONVERT_ELEMENT_BUFFER_FULL: 0;
    return res;
}
NK_API const struct nk_draw_command*
nk__draw_begin(const struct nk_context *ctx,
    const struct nk_buffer *buffer)
{
    return nk__draw_list_begin(&ctx->draw_list, buffer);
}
NK_API const struct nk_draw_command*
nk__draw_end(const struct nk_context *ctx, const struct nk_buffer *buffer)
{
    return nk__draw_list_end(&ctx->draw_list, buffer);
}
NK_API const struct nk_draw_command*
nk__draw_next(const struct nk_draw_command *cmd,
    const struct nk_buffer *buffer, const struct nk_context *ctx)
{
    return nk__draw_list_next(cmd, buffer, &ctx->draw_list);
}
#endif





/* ===============================================================
 *
 *                          INPUT
 *
 * ===============================================================*/
NK_API void
nk_input_begin(struct nk_context *ctx)
{
    int i;
    struct nk_input *in;
    NK_ASSERT(ctx);
    if (!ctx) return;
    in = &ctx->input;
    for (i = 0; i < NK_BUTTON_MAX; ++i)
        in->mouse.buttons[i].clicked = 0;

    in->keyboard.text_len = 0;
    in->mouse.scroll_delta = nk_vec2(0,0);
    in->mouse.prev.x = in->mouse.pos.x;
    in->mouse.prev.y = in->mouse.pos.y;
    in->mouse.delta.x = 0;
    in->mouse.delta.y = 0;
    for (i = 0; i < NK_KEY_MAX; i++)
        in->keyboard.keys[i].clicked = 0;
}
NK_API void
nk_input_end(struct nk_context *ctx)
{
    struct nk_input *in;
    NK_ASSERT(ctx);
    if (!ctx) return;
    in = &ctx->input;
    if (in->mouse.grab)
        in->mouse.grab = 0;
    if (in->mouse.ungrab) {
        in->mouse.grabbed = 0;
        in->mouse.ungrab = 0;
        in->mouse.grab = 0;
    }
}
NK_API void
nk_input_motion(struct nk_context *ctx, float x, float y)
{
    struct nk_input *in;
    NK_ASSERT(ctx);
    if (!ctx) return;
    in = &ctx->input;
    in->mouse.pos.x = x;
    in->mouse.pos.y = y;
    in->mouse.delta.x = in->mouse.pos.x - in->mouse.prev.x;
    in->mouse.delta.y = in->mouse.pos.y - in->mouse.prev.y;
}
NK_API void
nk_input_key(struct nk_context *ctx, enum nk_keys key, nk_bool down)
{
    struct nk_input *in;
    NK_ASSERT(ctx);
    if (!ctx) return;
    in = &ctx->input;
#ifdef NK_KEYSTATE_BASED_INPUT
    if (in->keyboard.keys[key].down != down)
        in->keyboard.keys[key].clicked++;
#else
    in->keyboard.keys[key].clicked++;
#endif
    in->keyboard.keys[key].down = down;
}
NK_API void
nk_input_button(struct nk_context *ctx, enum nk_buttons id, int x, int y, nk_bool down)
{
    struct nk_mouse_button *btn;
    struct nk_input *in;
    NK_ASSERT(ctx);
    if (!ctx) return;
    in = &ctx->input;
    if (in->mouse.buttons[id].down == down) return;

    btn = &in->mouse.buttons[id];
    btn->clicked_pos.x = (float)x;
    btn->clicked_pos.y = (float)y;
    btn->down = down;
    btn->clicked++;

    /* Fix Click-Drag for touch events. */
    in->mouse.delta.x = 0;
    in->mouse.delta.y = 0;
#ifdef NK_BUTTON_TRIGGER_ON_RELEASE
    if (down == 1 && id == NK_BUTTON_LEFT)
    {
        in->mouse.down_pos.x = btn->clicked_pos.x;
        in->mouse.down_pos.y = btn->clicked_pos.y;
    }
#endif
}
NK_API void
nk_input_scroll(struct nk_context *ctx, struct nk_vec2 val)
{
    NK_ASSERT(ctx);
    if (!ctx) return;
    ctx->input.mouse.scroll_delta.x += val.x;
    ctx->input.mouse.scroll_delta.y += val.y;
}
NK_API void
nk_input_glyph(struct nk_context *ctx, const nk_glyph glyph)
{
    int len = 0;
    nk_rune unicode;
    struct nk_input *in;

    NK_ASSERT(ctx);
    if (!ctx) return;
    in = &ctx->input;

    len = nk_utf_decode(glyph, &unicode, NK_UTF_SIZE);
    if (len && ((in->keyboard.text_len + len) < NK_INPUT_MAX)) {
        nk_utf_encode(unicode, &in->keyboard.text[in->keyboard.text_len],
            NK_INPUT_MAX - in->keyboard.text_len);
        in->keyboard.text_len += len;
    }
}
NK_API void
nk_input_char(struct nk_context *ctx, char c)
{
    nk_glyph glyph;
    NK_ASSERT(ctx);
    if (!ctx) return;
    glyph[0] = c;
    nk_input_glyph(ctx, glyph);
}
NK_API void
nk_input_unicode(struct nk_context *ctx, nk_rune unicode)
{
    nk_glyph rune;
    NK_ASSERT(ctx);
    if (!ctx) return;
    nk_utf_encode(unicode, rune, NK_UTF_SIZE);
    nk_input_glyph(ctx, rune);
}
NK_API nk_bool
nk_input_has_mouse_click(const struct nk_input *i, enum nk_buttons id)
{
    const struct nk_mouse_button *btn;
    if (!i) return nk_false;
    btn = &i->mouse.buttons[id];
    return (btn->clicked && btn->down == nk_false) ? nk_true : nk_false;
}
NK_API nk_bool
nk_input_has_mouse_click_in_rect(const struct nk_input *i, enum nk_buttons id,
    struct nk_rect b)
{
    const struct nk_mouse_button *btn;
    if (!i) return nk_false;
    btn = &i->mouse.buttons[id];
    if (!NK_INBOX(btn->clicked_pos.x,btn->clicked_pos.y,b.x,b.y,b.w,b.h))
        return nk_false;
    return nk_true;
}
NK_API nk_bool
nk_input_has_mouse_click_in_button_rect(const struct nk_input *i, enum nk_buttons id,
    struct nk_rect b)
{
    const struct nk_mouse_button *btn;
    if (!i) return nk_false;
    btn = &i->mouse.buttons[id];
#ifdef NK_BUTTON_TRIGGER_ON_RELEASE
    if (!NK_INBOX(btn->clicked_pos.x,btn->clicked_pos.y,b.x,b.y,b.w,b.h)
        || !NK_INBOX(i->mouse.down_pos.x,i->mouse.down_pos.y,b.x,b.y,b.w,b.h))
#else
    if (!NK_INBOX(btn->clicked_pos.x,btn->clicked_pos.y,b.x,b.y,b.w,b.h))
#endif
        return nk_false;
    return nk_true;
}
NK_API nk_bool
nk_input_has_mouse_click_down_in_rect(const struct nk_input *i, enum nk_buttons id,
    struct nk_rect b, nk_bool down)
{
    const struct nk_mouse_button *btn;
    if (!i) return nk_false;
    btn = &i->mouse.buttons[id];
    return nk_input_has_mouse_click_in_rect(i, id, b) && (btn->down == down);
}
NK_API nk_bool
nk_input_is_mouse_click_in_rect(const struct nk_input *i, enum nk_buttons id,
    struct nk_rect b)
{
    const struct nk_mouse_button *btn;
    if (!i) return nk_false;
    btn = &i->mouse.buttons[id];
    return (nk_input_has_mouse_click_down_in_rect(i, id, b, nk_false) &&
            btn->clicked) ? nk_true : nk_false;
}
NK_API nk_bool
nk_input_is_mouse_click_down_in_rect(const struct nk_input *i, enum nk_buttons id,
    struct nk_rect b, nk_bool down)
{
    const struct nk_mouse_button *btn;
    if (!i) return nk_false;
    btn = &i->mouse.buttons[id];
    return (nk_input_has_mouse_click_down_in_rect(i, id, b, down) &&
            btn->clicked) ? nk_true : nk_false;
}
NK_API nk_bool
nk_input_any_mouse_click_in_rect(const struct nk_input *in, struct nk_rect b)
{
    int i, down = 0;
    for (i = 0; i < NK_BUTTON_MAX; ++i)
        down = down || nk_input_is_mouse_click_in_rect(in, (enum nk_buttons)i, b);
    return down;
}
NK_API nk_bool
nk_input_is_mouse_hovering_rect(const struct nk_input *i, struct nk_rect rect)
{
    if (!i) return nk_false;
    return NK_INBOX(i->mouse.pos.x, i->mouse.pos.y, rect.x, rect.y, rect.w, rect.h);
}
NK_API nk_bool
nk_input_is_mouse_prev_hovering_rect(const struct nk_input *i, struct nk_rect rect)
{
    if (!i) return nk_false;
    return NK_INBOX(i->mouse.prev.x, i->mouse.prev.y, rect.x, rect.y, rect.w, rect.h);
}
NK_API nk_bool
nk_input_mouse_clicked(const struct nk_input *i, enum nk_buttons id, struct nk_rect rect)
{
    if (!i) return nk_false;
    if (!nk_input_is_mouse_hovering_rect(i, rect)) return nk_false;
    return nk_input_is_mouse_click_in_rect(i, id, rect);
}
NK_API nk_bool
nk_input_is_mouse_down(const struct nk_input *i, enum nk_buttons id)
{
    if (!i) return nk_false;
    return i->mouse.buttons[id].down;
}
NK_API nk_bool
nk_input_is_mouse_pressed(const struct nk_input *i, enum nk_buttons id)
{
    const struct nk_mouse_button *b;
    if (!i) return nk_false;
    b = &i->mouse.buttons[id];
    if (b->down && b->clicked)
        return nk_true;
    return nk_false;
}
NK_API nk_bool
nk_input_is_mouse_released(const struct nk_input *i, enum nk_buttons id)
{
    if (!i) return nk_false;
    return (!i->mouse.buttons[id].down && i->mouse.buttons[id].clicked);
}
NK_API nk_bool
nk_input_is_key_pressed(const struct nk_input *i, enum nk_keys key)
{
    const struct nk_key *k;
    if (!i) return nk_false;
    k = &i->keyboard.keys[key];
    if ((k->down && k->clicked) || (!k->down && k->clicked >= 2))
        return nk_true;
    return nk_false;
}
NK_API nk_bool
nk_input_is_key_released(const struct nk_input *i, enum nk_keys key)
{
    const struct nk_key *k;
    if (!i) return nk_false;
    k = &i->keyboard.keys[key];
    if ((!k->down && k->clicked) || (k->down && k->clicked >= 2))
        return nk_true;
    return nk_false;
}
NK_API nk_bool
nk_input_is_key_down(const struct nk_input *i, enum nk_keys key)
{
    const struct nk_key *k;
    if (!i) return nk_false;
    k = &i->keyboard.keys[key];
    if (k->down) return nk_true;
    return nk_false;
}





/* ===============================================================
 *
 *                              STYLE
 *
 * ===============================================================*/
NK_API void nk_style_default(struct nk_context *ctx){nk_style_from_table(ctx, 0);}
#define NK_COLOR_MAP(NK_COLOR)\
    NK_COLOR(NK_COLOR_TEXT,                     175,175,175,255) \
    NK_COLOR(NK_COLOR_WINDOW,                   45, 45, 45, 255) \
    NK_COLOR(NK_COLOR_HEADER,                   40, 40, 40, 255) \
    NK_COLOR(NK_COLOR_BORDER,                   65, 65, 65, 255) \
    NK_COLOR(NK_COLOR_BUTTON,                   50, 50, 50, 255) \
    NK_COLOR(NK_COLOR_BUTTON_HOVER,             40, 40, 40, 255) \
    NK_COLOR(NK_COLOR_BUTTON_ACTIVE,            35, 35, 35, 255) \
    NK_COLOR(NK_COLOR_TOGGLE,                   100,100,100,255) \
    NK_COLOR(NK_COLOR_TOGGLE_HOVER,             120,120,120,255) \
    NK_COLOR(NK_COLOR_TOGGLE_CURSOR,            45, 45, 45, 255) \
    NK_COLOR(NK_COLOR_SELECT,                   45, 45, 45, 255) \
    NK_COLOR(NK_COLOR_SELECT_ACTIVE,            35, 35, 35,255)  \
    NK_COLOR(NK_COLOR_SLIDER,                   38, 38, 38, 255) \
    NK_COLOR(NK_COLOR_SLIDER_CURSOR,            100,100,100,255) \
    NK_COLOR(NK_COLOR_SLIDER_CURSOR_HOVER,      120,120,120,255) \
    NK_COLOR(NK_COLOR_SLIDER_CURSOR_ACTIVE,     150,150,150,255) \
    NK_COLOR(NK_COLOR_PROPERTY,                 38, 38, 38, 255) \
    NK_COLOR(NK_COLOR_EDIT,                     38, 38, 38, 255) \
    NK_COLOR(NK_COLOR_EDIT_CURSOR,              175,175,175,255) \
    NK_COLOR(NK_COLOR_COMBO,                    45, 45, 45, 255) \
    NK_COLOR(NK_COLOR_CHART,                    120,120,120,255) \
    NK_COLOR(NK_COLOR_CHART_COLOR,              45, 45, 45, 255) \
    NK_COLOR(NK_COLOR_CHART_COLOR_HIGHLIGHT,    255, 0,  0, 255) \
    NK_COLOR(NK_COLOR_SCROLLBAR,                40, 40, 40, 255) \
    NK_COLOR(NK_COLOR_SCROLLBAR_CURSOR,         100,100,100,255) \
    NK_COLOR(NK_COLOR_SCROLLBAR_CURSOR_HOVER,   120,120,120,255) \
    NK_COLOR(NK_COLOR_SCROLLBAR_CURSOR_ACTIVE,  150,150,150,255) \
    NK_COLOR(NK_COLOR_TAB_HEADER,               40, 40, 40,255)  \
    NK_COLOR(NK_COLOR_KNOB,                     38, 38, 38, 255) \
    NK_COLOR(NK_COLOR_KNOB_CURSOR,              100,100,100,255) \
    NK_COLOR(NK_COLOR_KNOB_CURSOR_HOVER,        120,120,120,255) \
    NK_COLOR(NK_COLOR_KNOB_CURSOR_ACTIVE,       150,150,150,255)

NK_GLOBAL const struct nk_color
nk_default_color_style[NK_COLOR_COUNT] = {
#define NK_COLOR(a,b,c,d,e) {b,c,d,e},
    NK_COLOR_MAP(NK_COLOR)
#undef NK_COLOR
};
NK_GLOBAL const char *nk_color_names[NK_COLOR_COUNT] = {
#define NK_COLOR(a,b,c,d,e) #a,
    NK_COLOR_MAP(NK_COLOR)
#undef NK_COLOR
};

NK_API const char*
nk_style_get_color_by_name(enum nk_style_colors c)
{
    return nk_color_names[c];
}
NK_API struct nk_style_item
nk_style_item_color(struct nk_color col)
{
    struct nk_style_item i;
    i.type = NK_STYLE_ITEM_COLOR;
    i.data.color = col;
    return i;
}
NK_API struct nk_style_item
nk_style_item_image(struct nk_image img)
{
    struct nk_style_item i;
    i.type = NK_STYLE_ITEM_IMAGE;
    i.data.image = img;
    return i;
}
NK_API struct nk_style_item
nk_style_item_nine_slice(struct nk_nine_slice slice)
{
    struct nk_style_item i;
    i.type = NK_STYLE_ITEM_NINE_SLICE;
    i.data.slice = slice;
    return i;
}
NK_API struct nk_style_item
nk_style_item_hide(void)
{
    struct nk_style_item i;
    i.type = NK_STYLE_ITEM_COLOR;
    i.data.color = nk_rgba(0,0,0,0);
    return i;
}
NK_API void
nk_style_from_table(struct nk_context *ctx, const struct nk_color *table)
{
    struct nk_style *style;
    struct nk_style_text *text;
    struct nk_style_button *button;
    struct nk_style_toggle *toggle;
    struct nk_style_selectable *select;
    struct nk_style_slider *slider;
    struct nk_style_knob *knob;
    struct nk_style_progress *prog;
    struct nk_style_scrollbar *scroll;
    struct nk_style_edit *edit;
    struct nk_style_property *property;
    struct nk_style_combo *combo;
    struct nk_style_chart *chart;
    struct nk_style_tab *tab;
    struct nk_style_window *win;

    NK_ASSERT(ctx);
    if (!ctx) return;
    style = &ctx->style;
    const float scale = style->font ? style->font->height / 20.0f : 1.0f;
#define S(X) ((int)(scale*(X)+0.5f))
    table = (!table) ? nk_default_color_style: table;

    /* default text */
    text = &style->text;
    text->color = table[NK_COLOR_TEXT];
    text->padding = nk_vec2(0,0);
    text->color_factor = 1.0f;
    text->disabled_factor = NK_WIDGET_DISABLED_FACTOR;

    /* default button */
    button = &style->button;
    nk_zero_struct(*button);
    button->normal                     = nk_style_item_color(table[NK_COLOR_BUTTON]);
    button->hover                      = nk_style_item_color(table[NK_COLOR_BUTTON_HOVER]);
    button->active                     = nk_style_item_color(table[NK_COLOR_BUTTON_ACTIVE]);
    button->border_color               = table[NK_COLOR_BORDER];
    button->text_background            = table[NK_COLOR_BUTTON];
    button->text_normal                = table[NK_COLOR_TEXT];
    button->text_hover                 = table[NK_COLOR_TEXT];
    button->text_active                = table[NK_COLOR_TEXT];
    button->padding                    = nk_vec2(S(2.0f),S(2.0f));
    button->image_padding              = nk_vec2(0.0f, 0.0f);
    button->touch_padding              = nk_vec2(0.0f, 0.0f);
    button->userdata                   = nk_handle_ptr(0);
    button->text_alignment             = NK_TEXT_CENTERED;
    button->border                     = S(1.0f);
    button->rounding                   = S(4.0f);
    button->color_factor_text          = 1.0f;
    button->color_factor_background    = 1.0f;
    button->disabled_factor            = NK_WIDGET_DISABLED_FACTOR;
    button->draw_begin                 = 0;
    button->draw_end                   = 0;

    /* contextual button */
    button = &style->contextual_button;
    nk_zero_struct(*button);
    button->normal          = nk_style_item_color(table[NK_COLOR_WINDOW]);
    button->hover           = nk_style_item_color(table[NK_COLOR_BUTTON_HOVER]);
    button->active          = nk_style_item_color(table[NK_COLOR_BUTTON_ACTIVE]);
    button->border_color    = table[NK_COLOR_WINDOW];
    button->text_background = table[NK_COLOR_WINDOW];
    button->text_normal     = table[NK_COLOR_TEXT];
    button->text_hover      = table[NK_COLOR_TEXT];
    button->text_active     = table[NK_COLOR_TEXT];
    button->padding         = nk_vec2(S(2.0f),S(2.0f));
    button->touch_padding   = nk_vec2(0.0f,0.0f);
    button->userdata        = nk_handle_ptr(0);
    button->text_alignment  = NK_TEXT_CENTERED;
    button->border          = 0.0f;
    button->rounding        = 0.0f;
    button->color_factor_text    = 1.0f;
    button->color_factor_background = 1.0f;
    button->disabled_factor = NK_WIDGET_DISABLED_FACTOR;
    button->draw_begin      = 0;
    button->draw_end        = 0;

    /* menu button */
    button = &style->menu_button;
    nk_zero_struct(*button);
    button->normal          = nk_style_item_color(table[NK_COLOR_WINDOW]);
    button->hover           = nk_style_item_color(table[NK_COLOR_WINDOW]);
    button->active          = nk_style_item_color(table[NK_COLOR_WINDOW]);
    button->border_color    = table[NK_COLOR_WINDOW];
    button->text_background = table[NK_COLOR_WINDOW];
    button->text_normal     = table[NK_COLOR_TEXT];
    button->text_hover      = table[NK_COLOR_TEXT];
    button->text_active     = table[NK_COLOR_TEXT];
    button->padding         = nk_vec2(S(2.0f),S(2.0f));
    button->touch_padding   = nk_vec2(0.0f,0.0f);
    button->userdata        = nk_handle_ptr(0);
    button->text_alignment  = NK_TEXT_CENTERED;
    button->border          = 0.0f;
    button->rounding        = S(1.0f);
    button->color_factor_text    = 1.0f;
    button->color_factor_background = 1.0f;
    button->disabled_factor = NK_WIDGET_DISABLED_FACTOR;
    button->draw_begin      = 0;
    button->draw_end        = 0;

    /* checkbox toggle */
    toggle = &style->checkbox;
    nk_zero_struct(*toggle);
    toggle->normal          = nk_style_item_color(table[NK_COLOR_TOGGLE]);
    toggle->hover           = nk_style_item_color(table[NK_COLOR_TOGGLE_HOVER]);
    toggle->active          = nk_style_item_color(table[NK_COLOR_TOGGLE_HOVER]);
    toggle->cursor_normal   = nk_style_item_color(table[NK_COLOR_TOGGLE_CURSOR]);
    toggle->cursor_hover    = nk_style_item_color(table[NK_COLOR_TOGGLE_CURSOR]);
    toggle->userdata        = nk_handle_ptr(0);
    toggle->text_background = table[NK_COLOR_WINDOW];
    toggle->text_normal     = table[NK_COLOR_TEXT];
    toggle->text_hover      = table[NK_COLOR_TEXT];
    toggle->text_active     = table[NK_COLOR_TEXT];
    toggle->padding         = nk_vec2(S(2.0f), S(2.0f));
    toggle->touch_padding   = nk_vec2(0,0);
    toggle->border_color    = nk_rgba(0,0,0,0);
    toggle->border          = 0.0f;
    toggle->spacing         = S(4);
    toggle->color_factor    = 1.0f;
    toggle->disabled_factor = NK_WIDGET_DISABLED_FACTOR;

    /* option toggle */
    toggle = &style->option;
    nk_zero_struct(*toggle);
    toggle->normal          = nk_style_item_color(table[NK_COLOR_TOGGLE]);
    toggle->hover           = nk_style_item_color(table[NK_COLOR_TOGGLE_HOVER]);
    toggle->active          = nk_style_item_color(table[NK_COLOR_TOGGLE_HOVER]);
    toggle->cursor_normal   = nk_style_item_color(table[NK_COLOR_TOGGLE_CURSOR]);
    toggle->cursor_hover    = nk_style_item_color(table[NK_COLOR_TOGGLE_CURSOR]);
    toggle->userdata        = nk_handle_ptr(0);
    toggle->text_background = table[NK_COLOR_WINDOW];
    toggle->text_normal     = table[NK_COLOR_TEXT];
    toggle->text_hover      = table[NK_COLOR_TEXT];
    toggle->text_active     = table[NK_COLOR_TEXT];
    toggle->padding         = nk_vec2(S(3.0f), S(3.0f));
    toggle->touch_padding   = nk_vec2(0,0);
    toggle->border_color    = nk_rgba(0,0,0,0);
    toggle->border          = 0.0f;
    toggle->spacing         = S(4);
    toggle->color_factor    = 1.0f;
    toggle->disabled_factor = NK_WIDGET_DISABLED_FACTOR;

    /* selectable */
    select = &style->selectable;
    nk_zero_struct(*select);
    select->normal          = nk_style_item_color(table[NK_COLOR_SELECT]);
    select->hover           = nk_style_item_color(table[NK_COLOR_SELECT]);
    select->pressed         = nk_style_item_color(table[NK_COLOR_SELECT]);
    select->normal_active   = nk_style_item_color(table[NK_COLOR_SELECT_ACTIVE]);
    select->hover_active    = nk_style_item_color(table[NK_COLOR_SELECT_ACTIVE]);
    select->pressed_active  = nk_style_item_color(table[NK_COLOR_SELECT_ACTIVE]);
    select->text_normal     = table[NK_COLOR_TEXT];
    select->text_hover      = table[NK_COLOR_TEXT];
    select->text_pressed    = table[NK_COLOR_TEXT];
    select->text_normal_active  = table[NK_COLOR_TEXT];
    select->text_hover_active   = table[NK_COLOR_TEXT];
    select->text_pressed_active = table[NK_COLOR_TEXT];
    select->padding         = nk_vec2(S(2.0f),S(2.0f));
    select->image_padding   = nk_vec2(S(2.0f),S(2.0f));
    select->touch_padding   = nk_vec2(0,0);
    select->userdata        = nk_handle_ptr(0);
    select->rounding        = 0.0f;
    select->color_factor    = 1.0f;
    select->disabled_factor = NK_WIDGET_DISABLED_FACTOR;
    select->draw_begin      = 0;
    select->draw_end        = 0;

    /* slider */
    slider = &style->slider;
    nk_zero_struct(*slider);
    slider->normal          = nk_style_item_hide();
    slider->hover           = nk_style_item_hide();
    slider->active          = nk_style_item_hide();
    slider->bar_normal      = table[NK_COLOR_SLIDER];
    slider->bar_hover       = table[NK_COLOR_SLIDER];
    slider->bar_active      = table[NK_COLOR_SLIDER];
    slider->bar_filled      = table[NK_COLOR_SLIDER_CURSOR];
    slider->cursor_normal   = nk_style_item_color(table[NK_COLOR_SLIDER_CURSOR]);
    slider->cursor_hover    = nk_style_item_color(table[NK_COLOR_SLIDER_CURSOR_HOVER]);
    slider->cursor_active   = nk_style_item_color(table[NK_COLOR_SLIDER_CURSOR_ACTIVE]);
    slider->inc_symbol      = NK_SYMBOL_TRIANGLE_RIGHT;
    slider->dec_symbol      = NK_SYMBOL_TRIANGLE_LEFT;
    slider->cursor_size     = nk_vec2(S(16),S(16));
    slider->padding         = nk_vec2(S(2),S(2));
    slider->spacing         = nk_vec2(S(2),S(2));
    slider->userdata        = nk_handle_ptr(0);
    slider->show_buttons    = nk_false;
    slider->bar_height      = S(8);
    slider->rounding        = 0;
    slider->color_factor    = 1.0f;
    slider->disabled_factor = NK_WIDGET_DISABLED_FACTOR;
    slider->draw_begin      = 0;
    slider->draw_end        = 0;

    /* slider buttons */
    button = &style->slider.inc_button;
    button->normal          = nk_style_item_color(nk_rgb(40,40,40));
    button->hover           = nk_style_item_color(nk_rgb(42,42,42));
    button->active          = nk_style_item_color(nk_rgb(44,44,44));
    button->border_color    = nk_rgb(65,65,65);
    button->text_background = nk_rgb(40,40,40);
    button->text_normal     = nk_rgb(175,175,175);
    button->text_hover      = nk_rgb(175,175,175);
    button->text_active     = nk_rgb(175,175,175);
    button->padding         = nk_vec2(S(8.0f),S(8.0f));
    button->touch_padding   = nk_vec2(0.0f,0.0f);
    button->userdata        = nk_handle_ptr(0);
    button->text_alignment  = NK_TEXT_CENTERED;
    button->border          = S(1.0f);
    button->rounding        = 0.0f;
    button->color_factor_text    = 1.0f;
    button->color_factor_background = 1.0f;
    button->disabled_factor = NK_WIDGET_DISABLED_FACTOR;
    button->draw_begin      = 0;
    button->draw_end        = 0;
    style->slider.dec_button = style->slider.inc_button;

    /* knob */
    knob = &style->knob;
    nk_zero_struct(*knob);
    knob->normal          = nk_style_item_hide();
    knob->hover           = nk_style_item_hide();
    knob->active          = nk_style_item_hide();
    knob->knob_normal     = table[NK_COLOR_KNOB];
    knob->knob_hover      = table[NK_COLOR_KNOB];
    knob->knob_active     = table[NK_COLOR_KNOB];
    knob->cursor_normal   = table[NK_COLOR_KNOB_CURSOR];
    knob->cursor_hover    = table[NK_COLOR_KNOB_CURSOR_HOVER];
    knob->cursor_active   = table[NK_COLOR_KNOB_CURSOR_ACTIVE];

    knob->knob_border_color = table[NK_COLOR_BORDER];
    knob->knob_border       = S(1.0f);

    knob->padding         = nk_vec2(S(2),S(2));
    knob->spacing         = nk_vec2(S(2),S(2));
    knob->cursor_width    = S(2);
    knob->color_factor    = 1.0f;
    knob->disabled_factor = NK_WIDGET_DISABLED_FACTOR;

    knob->userdata        = nk_handle_ptr(0);
    knob->draw_begin      = 0;
    knob->draw_end        = 0;

    /* progressbar */
    prog = &style->progress;
    nk_zero_struct(*prog);
    prog->normal            = nk_style_item_color(table[NK_COLOR_SLIDER]);
    prog->hover             = nk_style_item_color(table[NK_COLOR_SLIDER]);
    prog->active            = nk_style_item_color(table[NK_COLOR_SLIDER]);
    prog->cursor_normal     = nk_style_item_color(table[NK_COLOR_SLIDER_CURSOR]);
    prog->cursor_hover      = nk_style_item_color(table[NK_COLOR_SLIDER_CURSOR_HOVER]);
    prog->cursor_active     = nk_style_item_color(table[NK_COLOR_SLIDER_CURSOR_ACTIVE]);
    prog->border_color      = nk_rgba(0,0,0,0);
    prog->cursor_border_color = nk_rgba(0,0,0,0);
    prog->userdata          = nk_handle_ptr(0);
    prog->padding           = nk_vec2(S(4),S(4));
    prog->rounding          = 0;
    prog->border            = 0;
    prog->cursor_rounding   = 0;
    prog->cursor_border     = 0;
    prog->color_factor      = 1.0f;
    prog->disabled_factor   = NK_WIDGET_DISABLED_FACTOR;
    prog->draw_begin        = 0;
    prog->draw_end          = 0;

    /* scrollbars */
    scroll = &style->scrollh;
    nk_zero_struct(*scroll);
    scroll->normal          = nk_style_item_color(table[NK_COLOR_SCROLLBAR]);
    scroll->hover           = nk_style_item_color(table[NK_COLOR_SCROLLBAR]);
    scroll->active          = nk_style_item_color(table[NK_COLOR_SCROLLBAR]);
    scroll->cursor_normal   = nk_style_item_color(table[NK_COLOR_SCROLLBAR_CURSOR]);
    scroll->cursor_hover    = nk_style_item_color(table[NK_COLOR_SCROLLBAR_CURSOR_HOVER]);
    scroll->cursor_active   = nk_style_item_color(table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE]);
    scroll->dec_symbol      = NK_SYMBOL_CIRCLE_SOLID;
    scroll->inc_symbol      = NK_SYMBOL_CIRCLE_SOLID;
    scroll->userdata        = nk_handle_ptr(0);
    scroll->border_color    = table[NK_COLOR_SCROLLBAR];
    scroll->cursor_border_color = table[NK_COLOR_SCROLLBAR];
    scroll->padding         = nk_vec2(0,0);
    scroll->show_buttons    = nk_false;
    scroll->border          = 0;
    scroll->rounding        = 0;
    scroll->border_cursor   = 0;
    scroll->rounding_cursor = 0;
    scroll->color_factor    = 1.0f;
    scroll->disabled_factor = NK_WIDGET_DISABLED_FACTOR;
    scroll->draw_begin      = 0;
    scroll->draw_end        = 0;
    style->scrollv = style->scrollh;

    /* scrollbars buttons */
    button = &style->scrollh.inc_button;
    button->normal          = nk_style_item_color(nk_rgb(40,40,40));
    button->hover           = nk_style_item_color(nk_rgb(42,42,42));
    button->active          = nk_style_item_color(nk_rgb(44,44,44));
    button->border_color    = nk_rgb(65,65,65);
    button->text_background = nk_rgb(40,40,40);
    button->text_normal     = nk_rgb(175,175,175);
    button->text_hover      = nk_rgb(175,175,175);
    button->text_active     = nk_rgb(175,175,175);
    button->padding         = nk_vec2(S(4.0f),S(4.0f));
    button->touch_padding   = nk_vec2(0.0f,0.0f);
    button->userdata        = nk_handle_ptr(0);
    button->text_alignment  = NK_TEXT_CENTERED;
    button->border          = S(1.0f);
    button->rounding        = 0.0f;
    button->color_factor_text    = 1.0f;
    button->color_factor_background = 1.0f;
    button->disabled_factor = NK_WIDGET_DISABLED_FACTOR;
    button->draw_begin      = 0;
    button->draw_end        = 0;
    style->scrollh.dec_button = style->scrollh.inc_button;
    style->scrollv.inc_button = style->scrollh.inc_button;
    style->scrollv.dec_button = style->scrollh.inc_button;

    /* edit */
    edit = &style->edit;
    nk_zero_struct(*edit);
    edit->normal            = nk_style_item_color(table[NK_COLOR_EDIT]);
    edit->hover             = nk_style_item_color(table[NK_COLOR_EDIT]);
    edit->active            = nk_style_item_color(table[NK_COLOR_EDIT]);
    edit->cursor_normal     = table[NK_COLOR_TEXT];
    edit->cursor_hover      = table[NK_COLOR_TEXT];
    edit->cursor_text_normal= table[NK_COLOR_EDIT];
    edit->cursor_text_hover = table[NK_COLOR_EDIT];
    edit->border_color      = table[NK_COLOR_BORDER];
    edit->text_normal       = table[NK_COLOR_TEXT];
    edit->text_hover        = table[NK_COLOR_TEXT];
    edit->text_active       = table[NK_COLOR_TEXT];
    edit->selected_normal   = table[NK_COLOR_TEXT];
    edit->selected_hover    = table[NK_COLOR_TEXT];
    edit->selected_text_normal  = table[NK_COLOR_EDIT];
    edit->selected_text_hover   = table[NK_COLOR_EDIT];
    edit->scrollbar_size    = nk_vec2(S(10),S(10));
    edit->scrollbar         = style->scrollv;
    edit->padding           = nk_vec2(S(4),S(4));
    edit->row_padding       = S(2);
    edit->cursor_size       = S(4);
    edit->border            = S(1);
    edit->rounding          = 0;
    edit->color_factor      = 1.0f;
    edit->disabled_factor   = NK_WIDGET_DISABLED_FACTOR;

    /* property */
    property = &style->property;
    nk_zero_struct(*property);
    property->normal        = nk_style_item_color(table[NK_COLOR_PROPERTY]);
    property->hover         = nk_style_item_color(table[NK_COLOR_PROPERTY]);
    property->active        = nk_style_item_color(table[NK_COLOR_PROPERTY]);
    property->border_color  = table[NK_COLOR_BORDER];
    property->label_normal  = table[NK_COLOR_TEXT];
    property->label_hover   = table[NK_COLOR_TEXT];
    property->label_active  = table[NK_COLOR_TEXT];
    property->sym_left      = NK_SYMBOL_TRIANGLE_LEFT;
    property->sym_right     = NK_SYMBOL_TRIANGLE_RIGHT;
    property->userdata      = nk_handle_ptr(0);
    property->padding       = nk_vec2(S(4),S(4));
    property->border        = S(1);
    property->rounding      = S(10);
    property->draw_begin    = 0;
    property->draw_end      = 0;
    property->color_factor  = 1.0f;
    property->disabled_factor = NK_WIDGET_DISABLED_FACTOR;

    /* property buttons */
    button = &style->property.dec_button;
    nk_zero_struct(*button);
    button->normal          = nk_style_item_color(table[NK_COLOR_PROPERTY]);
    button->hover           = nk_style_item_color(table[NK_COLOR_PROPERTY]);
    button->active          = nk_style_item_color(table[NK_COLOR_PROPERTY]);
    button->border_color    = nk_rgba(0,0,0,0);
    button->text_background = table[NK_COLOR_PROPERTY];
    button->text_normal     = table[NK_COLOR_TEXT];
    button->text_hover      = table[NK_COLOR_TEXT];
    button->text_active     = table[NK_COLOR_TEXT];
    button->padding         = nk_vec2(0.0f,0.0f);
    button->touch_padding   = nk_vec2(0.0f,0.0f);
    button->userdata        = nk_handle_ptr(0);
    button->text_alignment  = NK_TEXT_CENTERED;
    button->border          = 0.0f;
    button->rounding        = 0.0f;
    button->color_factor_text    = 1.0f;
    button->color_factor_background = 1.0f;
    button->disabled_factor = NK_WIDGET_DISABLED_FACTOR;
    button->draw_begin      = 0;
    button->draw_end        = 0;
    style->property.inc_button = style->property.dec_button;

    /* property edit */
    edit = &style->property.edit;
    nk_zero_struct(*edit);
    edit->normal            = nk_style_item_color(table[NK_COLOR_PROPERTY]);
    edit->hover             = nk_style_item_color(table[NK_COLOR_PROPERTY]);
    edit->active            = nk_style_item_color(table[NK_COLOR_PROPERTY]);
    edit->border_color      = nk_rgba(0,0,0,0);
    edit->cursor_normal     = table[NK_COLOR_TEXT];
    edit->cursor_hover      = table[NK_COLOR_TEXT];
    edit->cursor_text_normal= table[NK_COLOR_EDIT];
    edit->cursor_text_hover = table[NK_COLOR_EDIT];
    edit->text_normal       = table[NK_COLOR_TEXT];
    edit->text_hover        = table[NK_COLOR_TEXT];
    edit->text_active       = table[NK_COLOR_TEXT];
    edit->selected_normal   = table[NK_COLOR_TEXT];
    edit->selected_hover    = table[NK_COLOR_TEXT];
    edit->selected_text_normal  = table[NK_COLOR_EDIT];
    edit->selected_text_hover   = table[NK_COLOR_EDIT];
    edit->padding           = nk_vec2(0,0);
    edit->cursor_size       = S(8);
    edit->border            = 0;
    edit->rounding          = 0;
    edit->color_factor      = 1.0f;
    edit->disabled_factor   = NK_WIDGET_DISABLED_FACTOR;

    /* chart */
    chart = &style->chart;
    nk_zero_struct(*chart);
    chart->background       = nk_style_item_color(table[NK_COLOR_CHART]);
    chart->border_color     = table[NK_COLOR_BORDER];
    chart->selected_color   = table[NK_COLOR_CHART_COLOR_HIGHLIGHT];
    chart->color            = table[NK_COLOR_CHART_COLOR];
    chart->padding          = nk_vec2(S(4),S(4));
    chart->border           = 0;
    chart->rounding         = 0;
    chart->color_factor     = 1.0f;
    chart->disabled_factor  = NK_WIDGET_DISABLED_FACTOR;
    chart->show_markers     = nk_true;

    /* combo */
    combo = &style->combo;
    combo->normal           = nk_style_item_color(table[NK_COLOR_COMBO]);
    combo->hover            = nk_style_item_color(table[NK_COLOR_COMBO]);
    combo->active           = nk_style_item_color(table[NK_COLOR_COMBO]);
    combo->border_color     = table[NK_COLOR_BORDER];
    combo->label_normal     = table[NK_COLOR_TEXT];
    combo->label_hover      = table[NK_COLOR_TEXT];
    combo->label_active     = table[NK_COLOR_TEXT];
    combo->sym_normal       = NK_SYMBOL_TRIANGLE_DOWN;
    combo->sym_hover        = NK_SYMBOL_TRIANGLE_DOWN;
    combo->sym_active       = NK_SYMBOL_TRIANGLE_DOWN;
    combo->content_padding  = nk_vec2(S(4),S(4));
    combo->button_padding   = nk_vec2(0,S(4));
    combo->spacing          = nk_vec2(S(4),0);
    combo->border           = S(1);
    combo->rounding         = 0;
    combo->color_factor     = 1.0f;
    combo->disabled_factor  = NK_WIDGET_DISABLED_FACTOR;

    /* combo button */
    button = &style->combo.button;
    nk_zero_struct(*button);
    button->normal          = nk_style_item_color(table[NK_COLOR_COMBO]);
    button->hover           = nk_style_item_color(table[NK_COLOR_COMBO]);
    button->active          = nk_style_item_color(table[NK_COLOR_COMBO]);
    button->border_color    = nk_rgba(0,0,0,0);
    button->text_background = table[NK_COLOR_COMBO];
    button->text_normal     = table[NK_COLOR_TEXT];
    button->text_hover      = table[NK_COLOR_TEXT];
    button->text_active     = table[NK_COLOR_TEXT];
    button->padding         = nk_vec2(S(2.0f),S(2.0f));
    button->touch_padding   = nk_vec2(0.0f,0.0f);
    button->userdata        = nk_handle_ptr(0);
    button->text_alignment  = NK_TEXT_CENTERED;
    button->border          = 0.0f;
    button->rounding        = 0.0f;
    button->color_factor_text    = 1.0f;
    button->color_factor_background = 1.0f;
    button->disabled_factor = NK_WIDGET_DISABLED_FACTOR;
    button->draw_begin      = 0;
    button->draw_end        = 0;

    /* tab */
    tab = &style->tab;
    tab->background         = nk_style_item_color(table[NK_COLOR_TAB_HEADER]);
    tab->border_color       = table[NK_COLOR_BORDER];
    tab->text               = table[NK_COLOR_TEXT];
    tab->sym_minimize       = NK_SYMBOL_TRIANGLE_RIGHT;
    tab->sym_maximize       = NK_SYMBOL_TRIANGLE_DOWN;
    tab->padding            = nk_vec2(S(4),S(4));
    tab->spacing            = nk_vec2(S(4),S(4));
    tab->indent             = S(10.0f);
    tab->border             = S(1);
    tab->rounding           = 0;
    tab->color_factor       = 1.0f;
    tab->disabled_factor    = NK_WIDGET_DISABLED_FACTOR;

    /* tab button */
    button = &style->tab.tab_minimize_button;
    nk_zero_struct(*button);
    button->normal          = nk_style_item_color(table[NK_COLOR_TAB_HEADER]);
    button->hover           = nk_style_item_color(table[NK_COLOR_TAB_HEADER]);
    button->active          = nk_style_item_color(table[NK_COLOR_TAB_HEADER]);
    button->border_color    = nk_rgba(0,0,0,0);
    button->text_background = table[NK_COLOR_TAB_HEADER];
    button->text_normal     = table[NK_COLOR_TEXT];
    button->text_hover      = table[NK_COLOR_TEXT];
    button->text_active     = table[NK_COLOR_TEXT];
    button->padding         = nk_vec2(S(2.0f),S(2.0f));
    button->touch_padding   = nk_vec2(0.0f,0.0f);
    button->userdata        = nk_handle_ptr(0);
    button->text_alignment  = NK_TEXT_CENTERED;
    button->border          = 0.0f;
    button->rounding        = 0.0f;
    button->color_factor_text    = 1.0f;
    button->color_factor_background = 1.0f;
    button->disabled_factor = NK_WIDGET_DISABLED_FACTOR;
    button->draw_begin      = 0;
    button->draw_end        = 0;
    style->tab.tab_maximize_button =*button;

    /* node button */
    button = &style->tab.node_minimize_button;
    nk_zero_struct(*button);
    button->normal          = nk_style_item_color(table[NK_COLOR_WINDOW]);
    button->hover           = nk_style_item_color(table[NK_COLOR_WINDOW]);
    button->active          = nk_style_item_color(table[NK_COLOR_WINDOW]);
    button->border_color    = nk_rgba(0,0,0,0);
    button->text_background = table[NK_COLOR_TAB_HEADER];
    button->text_normal     = table[NK_COLOR_TEXT];
    button->text_hover      = table[NK_COLOR_TEXT];
    button->text_active     = table[NK_COLOR_TEXT];
    button->padding         = nk_vec2(S(2.0f),S(2.0f));
    button->touch_padding   = nk_vec2(0.0f,0.0f);
    button->userdata        = nk_handle_ptr(0);
    button->text_alignment  = NK_TEXT_CENTERED;
    button->border          = 0.0f;
    button->rounding        = 0.0f;
    button->color_factor_text    = 1.0f;
    button->color_factor_background = 1.0f;
    button->disabled_factor = NK_WIDGET_DISABLED_FACTOR;
    button->draw_begin      = 0;
    button->draw_end        = 0;
    style->tab.node_maximize_button =*button;

    /* window header */
    win = &style->window;
    win->header.align = NK_HEADER_RIGHT;
    win->header.close_symbol = NK_SYMBOL_X;
    win->header.minimize_symbol = NK_SYMBOL_MINUS;
    win->header.maximize_symbol = NK_SYMBOL_PLUS;
    win->header.normal = nk_style_item_color(table[NK_COLOR_HEADER]);
    win->header.hover = nk_style_item_color(table[NK_COLOR_HEADER]);
    win->header.active = nk_style_item_color(table[NK_COLOR_HEADER]);
    win->header.label_normal = table[NK_COLOR_TEXT];
    win->header.label_hover = table[NK_COLOR_TEXT];
    win->header.label_active = table[NK_COLOR_TEXT];
    win->header.label_padding = nk_vec2(S(4),S(4));
    win->header.padding = nk_vec2(S(4),S(4));
    win->header.spacing = nk_vec2(0,0);

    /* window header close button */
    button = &style->window.header.close_button;
    nk_zero_struct(*button);
    button->normal          = nk_style_item_color(table[NK_COLOR_HEADER]);
    button->hover           = nk_style_item_color(table[NK_COLOR_HEADER]);
    button->active          = nk_style_item_color(table[NK_COLOR_HEADER]);
    button->border_color    = nk_rgba(0,0,0,0);
    button->text_background = table[NK_COLOR_HEADER];
    button->text_normal     = table[NK_COLOR_TEXT];
    button->text_hover      = table[NK_COLOR_TEXT];
    button->text_active     = table[NK_COLOR_TEXT];
    button->padding         = nk_vec2(0.0f,0.0f);
    button->touch_padding   = nk_vec2(0.0f,0.0f);
    button->userdata        = nk_handle_ptr(0);
    button->text_alignment  = NK_TEXT_CENTERED;
    button->border          = 0.0f;
    button->rounding        = 0.0f;
    button->color_factor_text    = 1.0f;
    button->color_factor_background = 1.0f;
    button->disabled_factor = NK_WIDGET_DISABLED_FACTOR;
    button->draw_begin      = 0;
    button->draw_end        = 0;

    /* window header minimize button */
    button = &style->window.header.minimize_button;
    nk_zero_struct(*button);
    button->normal          = nk_style_item_color(table[NK_COLOR_HEADER]);
    button->hover           = nk_style_item_color(table[NK_COLOR_HEADER]);
    button->active          = nk_style_item_color(table[NK_COLOR_HEADER]);
    button->border_color    = nk_rgba(0,0,0,0);
    button->text_background = table[NK_COLOR_HEADER];
    button->text_normal     = table[NK_COLOR_TEXT];
    button->text_hover      = table[NK_COLOR_TEXT];
    button->text_active     = table[NK_COLOR_TEXT];
    button->padding         = nk_vec2(0.0f,0.0f);
    button->touch_padding   = nk_vec2(0.0f,0.0f);
    button->userdata        = nk_handle_ptr(0);
    button->text_alignment  = NK_TEXT_CENTERED;
    button->border          = 0.0f;
    button->rounding        = 0.0f;
    button->color_factor_text    = 1.0f;
    button->color_factor_background = 1.0f;
    button->disabled_factor = NK_WIDGET_DISABLED_FACTOR;
    button->draw_begin      = 0;
    button->draw_end        = 0;

    /* window */
    win->background = table[NK_COLOR_WINDOW];
    win->fixed_background = nk_style_item_color(table[NK_COLOR_WINDOW]);
    win->border_color = table[NK_COLOR_BORDER];
    win->popup_border_color = table[NK_COLOR_BORDER];
    win->combo_border_color = table[NK_COLOR_BORDER];
    win->contextual_border_color = table[NK_COLOR_BORDER];
    win->menu_border_color = table[NK_COLOR_BORDER];
    win->group_border_color = table[NK_COLOR_BORDER];
    win->tooltip_border_color = table[NK_COLOR_BORDER];
    win->scaler = nk_style_item_color(table[NK_COLOR_TEXT]);

    win->rounding = 0.0f;
    win->spacing = nk_vec2(S(4),S(4));
    win->scrollbar_size = nk_vec2(S(5),S(5));
    win->min_size = nk_vec2(64,64);

    win->combo_border = S(1.0f);
    win->contextual_border = S(1.0f);
    win->menu_border = S(1.0f);
    win->group_border = S(1.0f);
    win->tooltip_border = S(1.0f);
    win->popup_border = S(1.0f);
    win->border = S(2.0f);
    win->min_row_height_padding = S(8);

    win->padding = nk_vec2(S(4),S(4));
    win->group_padding = nk_vec2(S(4),S(4));
    win->popup_padding = nk_vec2(S(4),S(4));
    win->combo_padding = nk_vec2(S(4),S(4));
    win->contextual_padding = nk_vec2(S(4),S(4));
    win->menu_padding = nk_vec2(S(4),S(4));
    win->tooltip_padding = nk_vec2(S(4),S(4));
#undef S
}
NK_API void
nk_style_set_font(struct nk_context *ctx, const struct nk_user_font *font)
{
    struct nk_style *style;
    NK_ASSERT(ctx);

    if (!ctx) return;
    style = &ctx->style;
    style->font = font;
    ctx->stacks.fonts.head = 0;
    if (ctx->current)
        nk_layout_reset_min_row_height(ctx);
}
NK_API nk_bool
nk_style_push_font(struct nk_context *ctx, const struct nk_user_font *font)
{
    struct nk_config_stack_user_font *font_stack;
    struct nk_config_stack_user_font_element *element;

    NK_ASSERT(ctx);
    if (!ctx) return 0;

    font_stack = &ctx->stacks.fonts;
    NK_ASSERT(font_stack->head < (int)NK_LEN(font_stack->elements));
    if (font_stack->head >= (int)NK_LEN(font_stack->elements))
        return 0;

    element = &font_stack->elements[font_stack->head++];
    element->address = &ctx->style.font;
    element->old_value = ctx->style.font;
    ctx->style.font = font;
    return 1;
}
NK_API nk_bool
nk_style_pop_font(struct nk_context *ctx)
{
    struct nk_config_stack_user_font *font_stack;
    struct nk_config_stack_user_font_element *element;

    NK_ASSERT(ctx);
    if (!ctx) return 0;

    font_stack = &ctx->stacks.fonts;
    NK_ASSERT(font_stack->head > 0);
    if (font_stack->head < 1)
        return 0;

    element = &font_stack->elements[--font_stack->head];
    *element->address = element->old_value;
    return 1;
}
#define NK_STYLE_PUSH_IMPLEMENATION(prefix, type, stack) \
nk_style_push_##type(struct nk_context *ctx, prefix##_##type *address, prefix##_##type value)\
{\
    struct nk_config_stack_##type * type_stack;\
    struct nk_config_stack_##type##_element *element;\
    NK_ASSERT(ctx);\
    if (!ctx) return 0;\
    type_stack = &ctx->stacks.stack;\
    NK_ASSERT(type_stack->head < (int)NK_LEN(type_stack->elements));\
    if (type_stack->head >= (int)NK_LEN(type_stack->elements))\
        return 0;\
    element = &type_stack->elements[type_stack->head++];\
    element->address = address;\
    element->old_value = *address;\
    *address = value;\
    return 1;\
}
#define NK_STYLE_POP_IMPLEMENATION(type, stack) \
nk_style_pop_##type(struct nk_context *ctx)\
{\
    struct nk_config_stack_##type *type_stack;\
    struct nk_config_stack_##type##_element *element;\
    NK_ASSERT(ctx);\
    if (!ctx) return 0;\
    type_stack = &ctx->stacks.stack;\
    NK_ASSERT(type_stack->head > 0);\
    if (type_stack->head < 1)\
        return 0;\
    element = &type_stack->elements[--type_stack->head];\
    *element->address = element->old_value;\
    return 1;\
}
NK_API nk_bool NK_STYLE_PUSH_IMPLEMENATION(struct nk, style_item, style_items)
NK_API nk_bool NK_STYLE_PUSH_IMPLEMENATION(nk,float, floats)
NK_API nk_bool NK_STYLE_PUSH_IMPLEMENATION(struct nk, vec2, vectors)
NK_API nk_bool NK_STYLE_PUSH_IMPLEMENATION(nk,flags, flags)
NK_API nk_bool NK_STYLE_PUSH_IMPLEMENATION(struct nk,color, colors)

NK_API nk_bool NK_STYLE_POP_IMPLEMENATION(style_item, style_items)
NK_API nk_bool NK_STYLE_POP_IMPLEMENATION(float,floats)
NK_API nk_bool NK_STYLE_POP_IMPLEMENATION(vec2, vectors)
NK_API nk_bool NK_STYLE_POP_IMPLEMENATION(flags,flags)
NK_API nk_bool NK_STYLE_POP_IMPLEMENATION(color,colors)

NK_API nk_bool
nk_style_set_cursor(struct nk_context *ctx, enum nk_style_cursor c)
{
    struct nk_style *style;
    NK_ASSERT(ctx);
    if (!ctx) return 0;
    style = &ctx->style;
    if (style->cursors[c]) {
        style->cursor_active = style->cursors[c];
        return 1;
    }
    return 0;
}
NK_API void
nk_style_show_cursor(struct nk_context *ctx)
{
    ctx->style.cursor_visible = nk_true;
}
NK_API void
nk_style_hide_cursor(struct nk_context *ctx)
{
    ctx->style.cursor_visible = nk_false;
}
NK_API void
nk_style_load_cursor(struct nk_context *ctx, enum nk_style_cursor cursor,
    const struct nk_cursor *c)
{
    struct nk_style *style;
    NK_ASSERT(ctx);
    if (!ctx) return;
    style = &ctx->style;
    style->cursors[cursor] = c;
}
NK_API void
nk_style_load_all_cursors(struct nk_context *ctx, struct nk_cursor *cursors)
{
    int i = 0;
    struct nk_style *style;
    NK_ASSERT(ctx);
    if (!ctx) return;
    style = &ctx->style;
    for (i = 0; i < NK_CURSOR_COUNT; ++i)
        style->cursors[i] = &cursors[i];
    style->cursor_visible = nk_true;
}





/* ==============================================================
 *
 *                          CONTEXT
 *
 * ===============================================================*/
NK_INTERN void
nk_setup(struct nk_context *ctx, const struct nk_user_font *font)
{
    NK_ASSERT(ctx);
    if (!ctx) return;
    nk_zero_struct(*ctx);
    nk_style_default(ctx);
    ctx->seq = 1;
    if (font) ctx->style.font = font;
#ifdef NK_INCLUDE_VERTEX_BUFFER_OUTPUT
    nk_draw_list_init(&ctx->draw_list);
#endif
}
#ifdef NK_INCLUDE_DEFAULT_ALLOCATOR
NK_API nk_bool
nk_init_default(struct nk_context *ctx, const struct nk_user_font *font)
{
    struct nk_allocator alloc;
    alloc.userdata.ptr = 0;
    alloc.alloc = nk_malloc;
    alloc.free = nk_mfree;
    return nk_init(ctx, &alloc, font);
}
#endif
NK_API nk_bool
nk_init_fixed(struct nk_context *ctx, void *memory, nk_size size,
    const struct nk_user_font *font)
{
    NK_ASSERT(memory);
    if (!memory) return 0;
    nk_setup(ctx, font);
    nk_buffer_init_fixed(&ctx->memory, memory, size);
    ctx->use_pool = nk_false;
    return 1;
}
NK_API nk_bool
nk_init_custom(struct nk_context *ctx, struct nk_buffer *cmds,
    struct nk_buffer *pool, const struct nk_user_font *font)
{
    NK_ASSERT(cmds);
    NK_ASSERT(pool);
    if (!cmds || !pool) return 0;

    nk_setup(ctx, font);
    ctx->memory = *cmds;
    if (pool->type == NK_BUFFER_FIXED) {
        /* take memory from buffer and alloc fixed pool */
        nk_pool_init_fixed(&ctx->pool, pool->memory.ptr, pool->memory.size);
    } else {
        /* create dynamic pool from buffer allocator */
        struct nk_allocator *alloc = &pool->pool;
        nk_pool_init(&ctx->pool, alloc, NK_POOL_DEFAULT_CAPACITY);
    }
    ctx->use_pool = nk_true;
    return 1;
}
NK_API nk_bool
nk_init(struct nk_context *ctx, const struct nk_allocator *alloc,
    const struct nk_user_font *font)
{
    NK_ASSERT(alloc);
    if (!alloc) return 0;
    nk_setup(ctx, font);
    nk_buffer_init(&ctx->memory, alloc, NK_DEFAULT_COMMAND_BUFFER_SIZE);
    nk_pool_init(&ctx->pool, alloc, NK_POOL_DEFAULT_CAPACITY);
    ctx->use_pool = nk_true;
    return 1;
}
#ifdef NK_INCLUDE_COMMAND_USERDATA
NK_API void
nk_set_user_data(struct nk_context *ctx, nk_handle handle)
{
    if (!ctx) return;
    ctx->userdata = handle;
    if (ctx->current)
        ctx->current->buffer.userdata = handle;
}
#endif
NK_API void
nk_free(struct nk_context *ctx)
{
    NK_ASSERT(ctx);
    if (!ctx) return;
    nk_buffer_free(&ctx->memory);
    if (ctx->use_pool)
        nk_pool_free(&ctx->pool);

    nk_zero(&ctx->input, sizeof(ctx->input));
    nk_zero(&ctx->style, sizeof(ctx->style));
    nk_zero(&ctx->memory, sizeof(ctx->memory));

    ctx->seq = 0;
    ctx->build = 0;
    ctx->begin = 0;
    ctx->end = 0;
    ctx->active = 0;
    ctx->current = 0;
    ctx->freelist = 0;
    ctx->count = 0;
}
NK_API void
nk_clear(struct nk_context *ctx)
{
    struct nk_window *iter;
    struct nk_window *next;
    NK_ASSERT(ctx);

    if (!ctx) return;
    if (ctx->use_pool)
        nk_buffer_clear(&ctx->memory);
    else nk_buffer_reset(&ctx->memory, NK_BUFFER_FRONT);

    ctx->build = 0;
    ctx->memory.calls = 0;
    ctx->last_widget_state = 0;
    ctx->style.cursor_active = ctx->style.cursors[NK_CURSOR_ARROW];
    NK_MEMSET(&ctx->overlay, 0, sizeof(ctx->overlay));

    /* garbage collector */
    iter = ctx->begin;
    while (iter) {
        /* make sure valid minimized windows do not get removed */
        if ((iter->flags & NK_WINDOW_MINIMIZED) &&
            !(iter->flags & NK_WINDOW_CLOSED) &&
            iter->seq == ctx->seq) {
            iter = iter->next;
            continue;
        }
        /* remove hotness from hidden or closed windows*/
        if (((iter->flags & NK_WINDOW_HIDDEN) ||
            (iter->flags & NK_WINDOW_CLOSED)) &&
            iter == ctx->active) {
            ctx->active = iter->prev;
            ctx->end = iter->prev;
            if (!ctx->end)
                ctx->begin = 0;
            if (ctx->active)
                ctx->active->flags &= ~(unsigned)NK_WINDOW_ROM;
        }
        /* free unused popup windows */
        if (iter->popup.win && iter->popup.win->seq != ctx->seq) {
            nk_free_window(ctx, iter->popup.win);
            iter->popup.win = 0;
        }
        /* remove unused window state tables */
        {struct nk_table *n, *it = iter->tables;
        while (it) {
            n = it->next;
            if (it->seq != ctx->seq) {
                nk_remove_table(iter, it);
                nk_zero(it, sizeof(union nk_page_data));
                nk_free_table(ctx, it);
                if (it == iter->tables)
                    iter->tables = n;
            } it = n;
        }}
        /* window itself is not used anymore so free */
        if (iter->seq != ctx->seq || iter->flags & NK_WINDOW_CLOSED) {
            next = iter->next;
            nk_remove_window(ctx, iter);
            nk_free_window(ctx, iter);
            iter = next;
        } else iter = iter->next;
    }
    ctx->seq++;
}
NK_LIB void
nk_start_buffer(struct nk_context *ctx, struct nk_command_buffer *buffer)
{
    NK_ASSERT(ctx);
    NK_ASSERT(buffer);
    if (!ctx || !buffer) return;
    buffer->begin = ctx->memory.allocated;
    buffer->end = buffer->begin;
    buffer->last = buffer->begin;
    buffer->clip = nk_null_rect;
}
NK_LIB void
nk_start(struct nk_context *ctx, struct nk_window *win)
{
    NK_ASSERT(ctx);
    NK_ASSERT(win);
    nk_start_buffer(ctx, &win->buffer);
}
NK_LIB void
nk_start_popup(struct nk_context *ctx, struct nk_window *win)
{
    struct nk_popup_buffer *buf;
    NK_ASSERT(ctx);
    NK_ASSERT(win);
    if (!ctx || !win) return;

    /* save buffer fill state for popup */
    buf = &win->popup.buf;
    buf->begin = win->buffer.end;
    buf->end = win->buffer.end;
    buf->parent = win->buffer.last;
    buf->last = buf->begin;
    buf->active = nk_true;
}
NK_LIB void
nk_finish_popup(struct nk_context *ctx, struct nk_window *win)
{
    struct nk_popup_buffer *buf;
    NK_ASSERT(ctx);
    NK_ASSERT(win);
    if (!ctx || !win) return;

    buf = &win->popup.buf;
    buf->last = win->buffer.last;
    buf->end = win->buffer.end;
}
NK_LIB void
nk_finish_buffer(struct nk_context *ctx, struct nk_command_buffer *buffer)
{
    NK_ASSERT(ctx);
    NK_ASSERT(buffer);
    if (!ctx || !buffer) return;
    buffer->end = ctx->memory.allocated;
}
NK_LIB void
nk_finish(struct nk_context *ctx, struct nk_window *win)
{
    struct nk_popup_buffer *buf;
    struct nk_command *parent_last;
    void *memory;

    NK_ASSERT(ctx);
    NK_ASSERT(win);
    if (!ctx || !win) return;
    nk_finish_buffer(ctx, &win->buffer);
    if (!win->popup.buf.active) return;

    buf = &win->popup.buf;
    memory = ctx->memory.memory.ptr;
    parent_last = nk_ptr_add(struct nk_command, memory, buf->parent);
    parent_last->next = buf->end;
}
NK_LIB void
nk_build(struct nk_context *ctx)
{
    struct nk_window *it = 0;
    struct nk_command *cmd = 0;
    nk_byte *buffer = 0;

    /* draw cursor overlay */
    if (!ctx->style.cursor_active)
        ctx->style.cursor_active = ctx->style.cursors[NK_CURSOR_ARROW];
    if (ctx->style.cursor_active && !ctx->input.mouse.grabbed && ctx->style.cursor_visible) {
        struct nk_rect mouse_bounds;
        const struct nk_cursor *cursor = ctx->style.cursor_active;
        nk_command_buffer_init(&ctx->overlay, &ctx->memory, NK_CLIPPING_OFF);
        nk_start_buffer(ctx, &ctx->overlay);

        mouse_bounds.x = ctx->input.mouse.pos.x - cursor->offset.x;
        mouse_bounds.y = ctx->input.mouse.pos.y - cursor->offset.y;
        mouse_bounds.w = cursor->size.x;
        mouse_bounds.h = cursor->size.y;

        nk_draw_image(&ctx->overlay, mouse_bounds, &cursor->img, nk_white);
        nk_finish_buffer(ctx, &ctx->overlay);
    }
    /* build one big draw command list out of all window buffers */
    it = ctx->begin;
    buffer = (nk_byte*)ctx->memory.memory.ptr;
    while (it != 0) {
        struct nk_window *next = it->next;
        if (it->buffer.last == it->buffer.begin || (it->flags & NK_WINDOW_HIDDEN)||
            it->seq != ctx->seq)
            goto cont;

        cmd = nk_ptr_add(struct nk_command, buffer, it->buffer.last);
        while (next && ((next->buffer.last == next->buffer.begin) ||
            (next->flags & NK_WINDOW_HIDDEN) || next->seq != ctx->seq))
            next = next->next; /* skip empty command buffers */

        if (next) cmd->next = next->buffer.begin;
        cont: it = next;
    }
    /* append all popup draw commands into lists */
    it = ctx->begin;
    while (it != 0) {
        struct nk_window *next = it->next;
        struct nk_popup_buffer *buf;
        if (!it->popup.buf.active)
            goto skip;

        buf = &it->popup.buf;
        cmd->next = buf->begin;
        cmd = nk_ptr_add(struct nk_command, buffer, buf->last);
        buf->active = nk_false;
        skip: it = next;
    }
    if (cmd) {
        /* append overlay commands */
        if (ctx->overlay.end != ctx->overlay.begin)
            cmd->next = ctx->overlay.begin;
        else cmd->next = ctx->memory.allocated;
    }
}
NK_API const struct nk_command*
nk__begin(struct nk_context *ctx)
{
    struct nk_window *iter;
    nk_byte *buffer;
    NK_ASSERT(ctx);
    if (!ctx) return 0;
    if (!ctx->count) return 0;

    buffer = (nk_byte*)ctx->memory.memory.ptr;
    if (!ctx->build) {
        nk_build(ctx);
        ctx->build = nk_true;
    }
    iter = ctx->begin;
    while (iter && ((iter->buffer.begin == iter->buffer.end) ||
        (iter->flags & NK_WINDOW_HIDDEN) || iter->seq != ctx->seq))
        iter = iter->next;
    if (!iter) return 0;
    return nk_ptr_add_const(struct nk_command, buffer, iter->buffer.begin);
}

NK_API const struct nk_command*
nk__next(struct nk_context *ctx, const struct nk_command *cmd)
{
    nk_byte *buffer;
    const struct nk_command *next;
    NK_ASSERT(ctx);
    if (!ctx || !cmd || !ctx->count) return 0;
    if (cmd->next >= ctx->memory.allocated) return 0;
    buffer = (nk_byte*)ctx->memory.memory.ptr;
    next = nk_ptr_add_const(struct nk_command, buffer, cmd->next);
    return next;
}






/* ===============================================================
 *
 *                              POOL
 *
 * ===============================================================*/
NK_LIB void
nk_pool_init(struct nk_pool *pool, const struct nk_allocator *alloc,
    unsigned int capacity)
{
    NK_ASSERT(capacity >= 1);
    nk_zero(pool, sizeof(*pool));
    pool->alloc = *alloc;
    pool->capacity = capacity;
    pool->type = NK_BUFFER_DYNAMIC;
    pool->pages = 0;
}
NK_LIB void
nk_pool_free(struct nk_pool *pool)
{
    struct nk_page *iter;
    if (!pool) return;
    iter = pool->pages;
    if (pool->type == NK_BUFFER_FIXED) return;
    while (iter) {
        struct nk_page *next = iter->next;
        pool->alloc.free(pool->alloc.userdata, iter);
        iter = next;
    }
}
NK_LIB void
nk_pool_init_fixed(struct nk_pool *pool, void *memory, nk_size size)
{
    nk_zero(pool, sizeof(*pool));
    NK_ASSERT(size >= sizeof(struct nk_page));
    if (size < sizeof(struct nk_page)) return;
    /* first nk_page_element is embedded in nk_page, additional elements follow in adjacent space */
    pool->capacity = (unsigned)(1 + (size - sizeof(struct nk_page)) / sizeof(struct nk_page_element));
    pool->pages = (struct nk_page*)memory;
    pool->type = NK_BUFFER_FIXED;
    pool->size = size;
}
NK_LIB struct nk_page_element*
nk_pool_alloc(struct nk_pool *pool)
{
    if (!pool->pages || pool->pages->size >= pool->capacity) {
        /* allocate new page */
        struct nk_page *page;
        if (pool->type == NK_BUFFER_FIXED) {
            NK_ASSERT(pool->pages);
            if (!pool->pages) return 0;
            NK_ASSERT(pool->pages->size < pool->capacity);
            return 0;
        } else {
            nk_size size = sizeof(struct nk_page);
            size += (pool->capacity - 1) * sizeof(struct nk_page_element);
            page = (struct nk_page*)pool->alloc.alloc(pool->alloc.userdata,0, size);
            page->next = pool->pages;
            pool->pages = page;
            page->size = 0;
        }
    } return &pool->pages->win[pool->pages->size++];
}





/* ===============================================================
 *
 *                          PAGE ELEMENT
 *
 * ===============================================================*/
NK_LIB struct nk_page_element*
nk_create_page_element(struct nk_context *ctx)
{
    struct nk_page_element *elem;
    if (ctx->freelist) {
        /* unlink page element from free list */
        elem = ctx->freelist;
        ctx->freelist = elem->next;
    } else if (ctx->use_pool) {
        /* allocate page element from memory pool */
        elem = nk_pool_alloc(&ctx->pool);
        NK_ASSERT(elem);
        if (!elem) return 0;
    } else {
        /* allocate new page element from back of fixed size memory buffer */
        NK_STORAGE const nk_size size = sizeof(struct nk_page_element);
        NK_STORAGE const nk_size align = NK_ALIGNOF(struct nk_page_element);
        elem = (struct nk_page_element*)nk_buffer_alloc(&ctx->memory, NK_BUFFER_BACK, size, align);
        NK_ASSERT(elem);
        if (!elem) return 0;
    }
    nk_zero_struct(*elem);
    elem->next = 0;
    elem->prev = 0;
    return elem;
}
NK_LIB void
nk_link_page_element_into_freelist(struct nk_context *ctx,
    struct nk_page_element *elem)
{
    /* link table into freelist */
    if (!ctx->freelist) {
        ctx->freelist = elem;
    } else {
        elem->next = ctx->freelist;
        ctx->freelist = elem;
    }
}
NK_LIB void
nk_free_page_element(struct nk_context *ctx, struct nk_page_element *elem)
{
    /* we have a pool so just add to free list */
    if (ctx->use_pool) {
        nk_link_page_element_into_freelist(ctx, elem);
        return;
    }
    /* if possible remove last element from back of fixed memory buffer */
    {void *elem_end = (void*)(elem + 1);
    void *buffer_end = (nk_byte*)ctx->memory.memory.ptr + ctx->memory.size;
    if (elem_end == buffer_end)
        ctx->memory.size -= sizeof(struct nk_page_element);
    else nk_link_page_element_into_freelist(ctx, elem);}
}





/* ===============================================================
 *
 *                              TABLE
 *
 * ===============================================================*/
NK_LIB struct nk_table*
nk_create_table(struct nk_context *ctx)
{
    struct nk_page_element *elem;
    elem = nk_create_page_element(ctx);
    if (!elem) return 0;
    nk_zero_struct(*elem);
    return &elem->data.tbl;
}
NK_LIB void
nk_free_table(struct nk_context *ctx, struct nk_table *tbl)
{
    union nk_page_data *pd = NK_CONTAINER_OF(tbl, union nk_page_data, tbl);
    struct nk_page_element *pe = NK_CONTAINER_OF(pd, struct nk_page_element, data);
    nk_free_page_element(ctx, pe);
}
NK_LIB void
nk_push_table(struct nk_window *win, struct nk_table *tbl)
{
    if (!win->tables) {
        win->tables = tbl;
        tbl->next = 0;
        tbl->prev = 0;
        tbl->size = 0;
        win->table_count = 1;
        return;
    }
    win->tables->prev = tbl;
    tbl->next = win->tables;
    tbl->prev = 0;
    tbl->size = 0;
    win->tables = tbl;
    win->table_count++;
}
NK_LIB void
nk_remove_table(struct nk_window *win, struct nk_table *tbl)
{
    if (win->tables == tbl)
        win->tables = tbl->next;
    if (tbl->next)
        tbl->next->prev = tbl->prev;
    if (tbl->prev)
        tbl->prev->next = tbl->next;
    tbl->next = 0;
    tbl->prev = 0;
}
NK_LIB nk_uint*
nk_add_value(struct nk_context *ctx, struct nk_window *win,
            nk_hash name, nk_uint value)
{
    NK_ASSERT(ctx);
    NK_ASSERT(win);
    if (!win || !ctx) return 0;
    if (!win->tables || win->tables->size >= NK_VALUE_PAGE_CAPACITY) {
        struct nk_table *tbl = nk_create_table(ctx);
        NK_ASSERT(tbl);
        if (!tbl) return 0;
        nk_push_table(win, tbl);
    }
    win->tables->seq = win->seq;
    win->tables->keys[win->tables->size] = name;
    win->tables->values[win->tables->size] = value;
    return &win->tables->values[win->tables->size++];
}
NK_LIB nk_uint*
nk_find_value(struct nk_window *win, nk_hash name)
{
    struct nk_table *iter = win->tables;
    while (iter) {
        unsigned int i = 0;
        unsigned int size = iter->size;
        for (i = 0; i < size; ++i) {
            if (iter->keys[i] == name) {
                iter->seq = win->seq;
                return &iter->values[i];
            }
        } size = NK_VALUE_PAGE_CAPACITY;
        iter = iter->next;
    }
    return 0;
}





/* ===============================================================
 *
 *                              PANEL
 *
 * ===============================================================*/
NK_LIB void*
nk_create_panel(struct nk_context *ctx)
{
    struct nk_page_element *elem;
    elem = nk_create_page_element(ctx);
    if (!elem) return 0;
    nk_zero_struct(*elem);
    return &elem->data.pan;
}
NK_LIB void
nk_free_panel(struct nk_context *ctx, struct nk_panel *pan)
{
    union nk_page_data *pd = NK_CONTAINER_OF(pan, union nk_page_data, pan);
    struct nk_page_element *pe = NK_CONTAINER_OF(pd, struct nk_page_element, data);
    nk_free_page_element(ctx, pe);
}
NK_LIB nk_bool
nk_panel_has_header(nk_flags flags, const char *title)
{
    nk_bool active = 0;
    active = (flags & (NK_WINDOW_CLOSABLE|NK_WINDOW_MINIMIZABLE));
    active = active || (flags & NK_WINDOW_TITLE);
    active = active && !(flags & NK_WINDOW_HIDDEN) && title;
    return active;
}
NK_LIB struct nk_vec2
nk_panel_get_padding(const struct nk_style *style, enum nk_panel_type type)
{
    switch (type) {
    default:
    case NK_PANEL_WINDOW: return style->window.padding;
    case NK_PANEL_GROUP: return style->window.group_padding;
    case NK_PANEL_POPUP: return style->window.popup_padding;
    case NK_PANEL_CONTEXTUAL: return style->window.contextual_padding;
    case NK_PANEL_COMBO: return style->window.combo_padding;
    case NK_PANEL_MENU: return style->window.menu_padding;
    case NK_PANEL_TOOLTIP: return style->window.tooltip_padding;}
}
NK_LIB float
nk_panel_get_border(const struct nk_style *style, nk_flags flags,
    enum nk_panel_type type)
{
    if (flags & NK_WINDOW_BORDER) {
        switch (type) {
        default:
        case NK_PANEL_WINDOW: return style->window.border;
        case NK_PANEL_GROUP: return style->window.group_border;
        case NK_PANEL_POPUP: return style->window.popup_border;
        case NK_PANEL_CONTEXTUAL: return style->window.contextual_border;
        case NK_PANEL_COMBO: return style->window.combo_border;
        case NK_PANEL_MENU: return style->window.menu_border;
        case NK_PANEL_TOOLTIP: return style->window.tooltip_border;
    }} else return 0;
}
NK_LIB struct nk_color
nk_panel_get_border_color(const struct nk_style *style, enum nk_panel_type type)
{
    switch (type) {
    default:
    case NK_PANEL_WINDOW: return style->window.border_color;
    case NK_PANEL_GROUP: return style->window.group_border_color;
    case NK_PANEL_POPUP: return style->window.popup_border_color;
    case NK_PANEL_CONTEXTUAL: return style->window.contextual_border_color;
    case NK_PANEL_COMBO: return style->window.combo_border_color;
    case NK_PANEL_MENU: return style->window.menu_border_color;
    case NK_PANEL_TOOLTIP: return style->window.tooltip_border_color;}
}
NK_LIB nk_bool
nk_panel_is_sub(enum nk_panel_type type)
{
    return ((int)type & (int)NK_PANEL_SET_SUB)?1:0;
}
NK_LIB nk_bool
nk_panel_is_nonblock(enum nk_panel_type type)
{
    return ((int)type & (int)NK_PANEL_SET_NONBLOCK)?1:0;
}
NK_LIB nk_bool
nk_panel_begin(struct nk_context *ctx, const char *title, enum nk_panel_type panel_type)
{
    struct nk_input *in;
    struct nk_window *win;
    struct nk_panel *layout;
    struct nk_command_buffer *out;
    const struct nk_style *style;
    const struct nk_user_font *font;

    struct nk_vec2 scrollbar_size;
    struct nk_vec2 panel_padding;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout) return 0;
    nk_zero(ctx->current->layout, sizeof(*ctx->current->layout));
    if ((ctx->current->flags & NK_WINDOW_HIDDEN) || (ctx->current->flags & NK_WINDOW_CLOSED)) {
        nk_zero(ctx->current->layout, sizeof(struct nk_panel));
        ctx->current->layout->type = panel_type;
        return 0;
    }
    /* pull state into local stack */
    style = &ctx->style;
    font = style->font;
    win = ctx->current;
    layout = win->layout;
    out = &win->buffer;
    in = (win->flags & NK_WINDOW_NO_INPUT) ? 0: &ctx->input;
#ifdef NK_INCLUDE_COMMAND_USERDATA
    win->buffer.userdata = ctx->userdata;
#endif
    /* pull style configuration into local stack */
    scrollbar_size = style->window.scrollbar_size;
    panel_padding = nk_panel_get_padding(style, panel_type);

    /* window movement */
    if ((win->flags & NK_WINDOW_MOVABLE) && !(win->flags & NK_WINDOW_ROM)) {
        nk_bool left_mouse_down;
        unsigned int left_mouse_clicked;
        int left_mouse_click_in_cursor;

        /* calculate draggable window space */
        struct nk_rect header;
        header.x = win->bounds.x;
        header.y = win->bounds.y;
        header.w = win->bounds.w;
        if (nk_panel_has_header(win->flags, title)) {
            header.h = font->height + 2.0f * style->window.header.padding.y;
            header.h += 2.0f * style->window.header.label_padding.y;
        } else header.h = panel_padding.y;

        /* window movement by dragging */
        left_mouse_down = in->mouse.buttons[NK_BUTTON_LEFT].down;
        left_mouse_clicked = in->mouse.buttons[NK_BUTTON_LEFT].clicked;
        left_mouse_click_in_cursor = nk_input_has_mouse_click_down_in_rect(in,
            NK_BUTTON_LEFT, header, nk_true);
        if (left_mouse_down && left_mouse_click_in_cursor && !left_mouse_clicked) {
            win->bounds.x = win->bounds.x + in->mouse.delta.x;
            win->bounds.y = win->bounds.y + in->mouse.delta.y;
            in->mouse.buttons[NK_BUTTON_LEFT].clicked_pos.x += in->mouse.delta.x;
            in->mouse.buttons[NK_BUTTON_LEFT].clicked_pos.y += in->mouse.delta.y;
            ctx->style.cursor_active = ctx->style.cursors[NK_CURSOR_MOVE];
        }
    }

    /* setup panel */
    layout->type = panel_type;
    layout->flags = win->flags;
    layout->bounds = win->bounds;
    layout->bounds.x += panel_padding.x;
    layout->bounds.w -= 2*panel_padding.x;
    if (win->flags & NK_WINDOW_BORDER) {
        layout->border = nk_panel_get_border(style, win->flags, panel_type);
        layout->bounds = nk_shrink_rect(layout->bounds, layout->border);
    } else layout->border = 0;
    layout->at_y = layout->bounds.y;
    layout->at_x = layout->bounds.x;
    layout->max_x = 0;
    layout->header_height = 0;
    layout->footer_height = 0;
    nk_layout_reset_min_row_height(ctx);
    layout->row.index = 0;
    layout->row.columns = 0;
    layout->row.ratio = 0;
    layout->row.item_width = 0;
    layout->row.tree_depth = 0;
    layout->row.height = panel_padding.y;
    layout->has_scrolling = nk_true;
    if (!(win->flags & NK_WINDOW_NO_SCROLLBAR))
        layout->bounds.w -= scrollbar_size.x;
    if (!nk_panel_is_nonblock(panel_type)) {
        layout->footer_height = 0;
        if (!(win->flags & NK_WINDOW_NO_SCROLLBAR) || win->flags & NK_WINDOW_SCALABLE)
            layout->footer_height = scrollbar_size.y;
        layout->bounds.h -= layout->footer_height;
    }

    /* panel header */
    if (nk_panel_has_header(win->flags, title))
    {
        struct nk_text text;
        struct nk_rect header;
        const struct nk_style_item *background = 0;

        /* calculate header bounds */
        header.x = win->bounds.x;
        header.y = win->bounds.y;
        header.w = win->bounds.w;
        header.h = font->height + 2.0f * style->window.header.padding.y;
        header.h += (2.0f * style->window.header.label_padding.y);

        /* shrink panel by header */
        layout->header_height = header.h;
        layout->bounds.y += header.h;
        layout->bounds.h -= header.h;
        layout->at_y += header.h;

        /* select correct header background and text color */
        if (ctx->active == win) {
            background = &style->window.header.active;
            text.text = style->window.header.label_active;
        } else if (nk_input_is_mouse_hovering_rect(&ctx->input, header)) {
            background = &style->window.header.hover;
            text.text = style->window.header.label_hover;
        } else {
            background = &style->window.header.normal;
            text.text = style->window.header.label_normal;
        }

        /* draw header background */
        header.h += 1.0f;

        switch(background->type) {
            case NK_STYLE_ITEM_IMAGE:
                text.background = nk_rgba(0,0,0,0);
                nk_draw_image(&win->buffer, header, &background->data.image, nk_white);
                break;
            case NK_STYLE_ITEM_NINE_SLICE:
                text.background = nk_rgba(0, 0, 0, 0);
                nk_draw_nine_slice(&win->buffer, header, &background->data.slice, nk_white);
                break;
            case NK_STYLE_ITEM_COLOR:
                text.background = background->data.color;
                nk_fill_rect(out, header, 0, background->data.color);
                break;
        }

        /* window close button */
        {struct nk_rect button;
        button.y = header.y + style->window.header.padding.y;
        button.h = header.h - 2 * style->window.header.padding.y;
        button.w = button.h;
        if (win->flags & NK_WINDOW_CLOSABLE) {
            nk_flags ws = 0;
            if (style->window.header.align == NK_HEADER_RIGHT) {
                button.x = (header.w + header.x) - (button.w + style->window.header.padding.x);
                header.w -= button.w + style->window.header.spacing.x + style->window.header.padding.x;
            } else {
                button.x = header.x + style->window.header.padding.x;
                header.x += button.w + style->window.header.spacing.x + style->window.header.padding.x;
            }

            if (nk_do_button_symbol(&ws, &win->buffer, button,
                style->window.header.close_symbol, NK_BUTTON_DEFAULT,
                &style->window.header.close_button, in, style->font) && !(win->flags & NK_WINDOW_ROM))
            {
                layout->flags |= NK_WINDOW_HIDDEN;
                layout->flags &= (nk_flags)~NK_WINDOW_MINIMIZED;
            }
        }

        /* window minimize button */
        if (win->flags & NK_WINDOW_MINIMIZABLE) {
            nk_flags ws = 0;
            if (style->window.header.align == NK_HEADER_RIGHT) {
                button.x = (header.w + header.x) - button.w;
                if (!(win->flags & NK_WINDOW_CLOSABLE)) {
                    button.x -= style->window.header.padding.x;
                    header.w -= style->window.header.padding.x;
                }
                header.w -= button.w + style->window.header.spacing.x;
            } else {
                button.x = header.x;
                header.x += button.w + style->window.header.spacing.x + style->window.header.padding.x;
            }
            if (nk_do_button_symbol(&ws, &win->buffer, button, (layout->flags & NK_WINDOW_MINIMIZED)?
                style->window.header.maximize_symbol: style->window.header.minimize_symbol,
                NK_BUTTON_DEFAULT, &style->window.header.minimize_button, in, style->font) && !(win->flags & NK_WINDOW_ROM))
                layout->flags = (layout->flags & NK_WINDOW_MINIMIZED) ?
                    layout->flags & (nk_flags)~NK_WINDOW_MINIMIZED:
                    layout->flags | NK_WINDOW_MINIMIZED;
        }}

        {/* window header title */
        int text_len = nk_strlen(title);
        struct nk_rect label = {0,0,0,0};
        float t = font->width(font->userdata, font->height, title, text_len);
        text.padding = nk_vec2(0,0);

        label.x = header.x + style->window.header.padding.x;
        label.x += style->window.header.label_padding.x;
        label.y = header.y + style->window.header.label_padding.y;
        label.h = font->height + 2 * style->window.header.label_padding.y;
        label.w = t + 2 * style->window.header.spacing.x;
        label.w = NK_CLAMP(0, label.w, header.x + header.w - label.x);
        nk_widget_text(out, label, (const char*)title, text_len, &text, NK_TEXT_LEFT, font);}
    }

    /* draw window background */
    if (!(layout->flags & NK_WINDOW_MINIMIZED) && !(layout->flags & NK_WINDOW_DYNAMIC)) {
        struct nk_rect body;
        body.x = win->bounds.x;
        body.w = win->bounds.w;
        body.y = (win->bounds.y + layout->header_height);
        body.h = (win->bounds.h - layout->header_height);

        switch(style->window.fixed_background.type) {
            case NK_STYLE_ITEM_IMAGE:
                nk_draw_image(out, body, &style->window.fixed_background.data.image, nk_white);
                break;
            case NK_STYLE_ITEM_NINE_SLICE:
                nk_draw_nine_slice(out, body, &style->window.fixed_background.data.slice, nk_white);
                break;
            case NK_STYLE_ITEM_COLOR:
                nk_fill_rect(out, body, style->window.rounding, style->window.fixed_background.data.color);
                break;
        }
    }

    /* set clipping rectangle */
    {struct nk_rect clip;
    layout->clip = layout->bounds;
    nk_unify(&clip, &win->buffer.clip, layout->clip.x, layout->clip.y,
        layout->clip.x + layout->clip.w, layout->clip.y + layout->clip.h);
    nk_push_scissor(out, clip);
    layout->clip = clip;}
    return !(layout->flags & NK_WINDOW_HIDDEN) && !(layout->flags & NK_WINDOW_MINIMIZED);
}
NK_LIB void
nk_panel_end(struct nk_context *ctx)
{
    struct nk_input *in;
    struct nk_window *window;
    struct nk_panel *layout;
    const struct nk_style *style;
    struct nk_command_buffer *out;

    struct nk_vec2 scrollbar_size;
    struct nk_vec2 panel_padding;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    window = ctx->current;
    layout = window->layout;
    style = &ctx->style;
    out = &window->buffer;
    in = (layout->flags & NK_WINDOW_ROM || layout->flags & NK_WINDOW_NO_INPUT) ? 0 :&ctx->input;
    if (!nk_panel_is_sub(layout->type))
        nk_push_scissor(out, nk_null_rect);

    /* cache configuration data */
    scrollbar_size = style->window.scrollbar_size;
    panel_padding = nk_panel_get_padding(style, layout->type);

    /* update the current cursor Y-position to point over the last added widget */
    layout->at_y += layout->row.height;

    /* dynamic panels */
    if (layout->flags & NK_WINDOW_DYNAMIC && !(layout->flags & NK_WINDOW_MINIMIZED))
    {
        /* update panel height to fit dynamic growth */
        struct nk_rect empty_space;
        if (layout->at_y < (layout->bounds.y + layout->bounds.h))
            layout->bounds.h = layout->at_y - layout->bounds.y;

        /* fill top empty space */
        empty_space.x = window->bounds.x;
        empty_space.y = layout->bounds.y - layout->border;
        empty_space.h = panel_padding.y  + layout->border;
        empty_space.w = window->bounds.w;
        nk_fill_rect(out, empty_space, 0, style->window.background);

        /* fill left empty space */
        empty_space.x = window->bounds.x;
        empty_space.y = layout->bounds.y;
        empty_space.w = panel_padding.x + layout->border;
        empty_space.h = layout->bounds.h;
        nk_fill_rect(out, empty_space, 0, style->window.background);

        /* fill right empty space */
        empty_space.x = layout->bounds.x + layout->bounds.w;
        empty_space.y = layout->bounds.y;
        empty_space.w = panel_padding.x + layout->border;
        empty_space.h = layout->bounds.h;
        if (*layout->offset_y == 0 && !(layout->flags & NK_WINDOW_NO_SCROLLBAR))
            empty_space.w += scrollbar_size.x;
        nk_fill_rect(out, empty_space, 0, style->window.background);

        /* fill bottom empty space */
        if (layout->footer_height > 0) {
            empty_space.x = window->bounds.x;
            empty_space.y = layout->bounds.y + layout->bounds.h;
            empty_space.w = window->bounds.w;
            empty_space.h = layout->footer_height;
            nk_fill_rect(out, empty_space, 0, style->window.background);
        }
    }

    /* scrollbars */
    if (!(layout->flags & NK_WINDOW_NO_SCROLLBAR) &&
        !(layout->flags & NK_WINDOW_MINIMIZED) &&
        window->scrollbar_hiding_timer < NK_SCROLLBAR_HIDING_TIMEOUT)
    {
        struct nk_rect scroll;
        int scroll_has_scrolling;
        float scroll_target;
        float scroll_offset;
        float scroll_step;
        float scroll_inc;

        /* mouse wheel scrolling */
        if (nk_panel_is_sub(layout->type))
        {
            /* sub-window mouse wheel scrolling */
            struct nk_window *root_window = window;
            struct nk_panel *root_panel = window->layout;
            while (root_panel->parent)
                root_panel = root_panel->parent;
            while (root_window->parent)
                root_window = root_window->parent;

            /* only allow scrolling if parent window is active */
            scroll_has_scrolling = 0;
            if ((root_window == ctx->active) && layout->has_scrolling) {
                /* and panel is being hovered and inside clip rect*/
                if (nk_input_is_mouse_hovering_rect(in, layout->bounds) &&
                    NK_INTERSECT(layout->bounds.x, layout->bounds.y, layout->bounds.w, layout->bounds.h,
                        root_panel->clip.x, root_panel->clip.y, root_panel->clip.w, root_panel->clip.h))
                {
                    /* deactivate all parent scrolling */
                    root_panel = window->layout;
                    while (root_panel->parent) {
                        root_panel->has_scrolling = nk_false;
                        root_panel = root_panel->parent;
                    }
                    root_panel->has_scrolling = nk_false;
                    scroll_has_scrolling = nk_true;
                }
            }
        } else if (!nk_panel_is_sub(layout->type)) {
            /* window mouse wheel scrolling */
            scroll_has_scrolling = (window == ctx->active) && layout->has_scrolling;
            if (in && (in->mouse.scroll_delta.y > 0 || in->mouse.scroll_delta.x > 0) && scroll_has_scrolling)
                window->scrolled = nk_true;
            else window->scrolled = nk_false;
        } else scroll_has_scrolling = nk_false;

        {
            /* vertical scrollbar */
            nk_flags state = 0;
            scroll.x = layout->bounds.x + layout->bounds.w + panel_padding.x;
            scroll.y = layout->bounds.y;
            scroll.w = scrollbar_size.x;
            scroll.h = layout->bounds.h;

            scroll_offset = (float)*layout->offset_y;
            scroll_step = scroll.h * 0.10f;
            scroll_inc = scroll.h * 0.01f;
            scroll_target = (float)(int)(layout->at_y - scroll.y);
            scroll_offset = nk_do_scrollbarv(&state, out, scroll, scroll_has_scrolling,
                scroll_offset, scroll_target, scroll_step, scroll_inc,
                &ctx->style.scrollv, in, style->font);
            *layout->offset_y = (nk_uint)scroll_offset;
            if (in && scroll_has_scrolling)
                in->mouse.scroll_delta.y = 0;
        }
        {
            /* horizontal scrollbar */
            nk_flags state = 0;
            scroll.x = layout->bounds.x;
            scroll.y = layout->bounds.y + layout->bounds.h;
            scroll.w = layout->bounds.w;
            scroll.h = scrollbar_size.y;

            scroll_offset = (float)*layout->offset_x;
            scroll_target = (float)(int)(layout->max_x - scroll.x);
            scroll_step = layout->max_x * 0.05f;
            scroll_inc = layout->max_x * 0.005f;
            scroll_offset = nk_do_scrollbarh(&state, out, scroll, scroll_has_scrolling,
                scroll_offset, scroll_target, scroll_step, scroll_inc,
                &ctx->style.scrollh, in, style->font);
            *layout->offset_x = (nk_uint)scroll_offset;
        }
    }

    /* hide scroll if no user input */
    if (window->flags & NK_WINDOW_SCROLL_AUTO_HIDE) {
        int has_input = ctx->input.mouse.delta.x != 0 || ctx->input.mouse.delta.y != 0 || ctx->input.mouse.scroll_delta.y != 0;
        int is_window_hovered = nk_window_is_hovered(ctx);
        int any_item_active = (ctx->last_widget_state & NK_WIDGET_STATE_MODIFIED);
        if ((!has_input && is_window_hovered) || (!is_window_hovered && !any_item_active))
            window->scrollbar_hiding_timer += ctx->delta_time_seconds;
        else window->scrollbar_hiding_timer = 0;
    } else window->scrollbar_hiding_timer = 0;

    /* window border */
    if (layout->flags & NK_WINDOW_BORDER)
    {
        struct nk_color border_color = nk_panel_get_border_color(style, layout->type);
        const float padding_y = (layout->flags & NK_WINDOW_MINIMIZED)
            ? (style->window.border + window->bounds.y + layout->header_height)
            : ((layout->flags & NK_WINDOW_DYNAMIC)
                ? (layout->bounds.y + layout->bounds.h + layout->footer_height)
                : (window->bounds.y + window->bounds.h));
        struct nk_rect b = window->bounds;
        b.h = padding_y - window->bounds.y;
        nk_stroke_rect(out, b, style->window.rounding, layout->border, border_color);
    }

    /* scaler */
    if ((layout->flags & NK_WINDOW_SCALABLE) && in && !(layout->flags & NK_WINDOW_MINIMIZED))
    {
        /* calculate scaler bounds */
        struct nk_rect scaler;
        scaler.w = scrollbar_size.x;
        scaler.h = scrollbar_size.y;
        scaler.y = layout->bounds.y + layout->bounds.h;
        if (layout->flags & NK_WINDOW_SCALE_LEFT)
            scaler.x = layout->bounds.x - panel_padding.x * 0.5f;
        else scaler.x = layout->bounds.x + layout->bounds.w + panel_padding.x;
        if (layout->flags & NK_WINDOW_NO_SCROLLBAR)
            scaler.x -= scaler.w;

        /* draw scaler */
        {const struct nk_style_item *item = &style->window.scaler;
        if (item->type == NK_STYLE_ITEM_IMAGE)
            nk_draw_image(out, scaler, &item->data.image, nk_white);
        else {
            if (layout->flags & NK_WINDOW_SCALE_LEFT) {
                nk_fill_triangle(out, scaler.x, scaler.y, scaler.x,
                    scaler.y + scaler.h, scaler.x + scaler.w,
                    scaler.y + scaler.h, item->data.color);
            } else {
                nk_fill_triangle(out, scaler.x + scaler.w, scaler.y, scaler.x + scaler.w,
                    scaler.y + scaler.h, scaler.x, scaler.y + scaler.h, item->data.color);
            }
        }}

        /* do window scaling */
        if (!(window->flags & NK_WINDOW_ROM)) {
            struct nk_vec2 window_size = style->window.min_size;
            int left_mouse_down = in->mouse.buttons[NK_BUTTON_LEFT].down;
            int left_mouse_click_in_scaler = nk_input_has_mouse_click_down_in_rect(in,
                    NK_BUTTON_LEFT, scaler, nk_true);

            if (left_mouse_down && left_mouse_click_in_scaler) {
                float delta_x = in->mouse.delta.x;
                if (layout->flags & NK_WINDOW_SCALE_LEFT) {
                    delta_x = -delta_x;
                    window->bounds.x += in->mouse.delta.x;
                }
                /* dragging in x-direction  */
                if (window->bounds.w + delta_x >= window_size.x) {
                    if ((delta_x < 0) || (delta_x > 0 && in->mouse.pos.x >= scaler.x)) {
                        window->bounds.w = window->bounds.w + delta_x;
                        scaler.x += in->mouse.delta.x;
                    }
                }
                /* dragging in y-direction (only possible if static window) */
                if (!(layout->flags & NK_WINDOW_DYNAMIC)) {
                    if (window_size.y < window->bounds.h + in->mouse.delta.y) {
                        if ((in->mouse.delta.y < 0) || (in->mouse.delta.y > 0 && in->mouse.pos.y >= scaler.y)) {
                            window->bounds.h = window->bounds.h + in->mouse.delta.y;
                            scaler.y += in->mouse.delta.y;
                        }
                    }
                }
                ctx->style.cursor_active = ctx->style.cursors[NK_CURSOR_RESIZE_TOP_RIGHT_DOWN_LEFT];
                in->mouse.buttons[NK_BUTTON_LEFT].clicked_pos.x = scaler.x + scaler.w/2.0f;
                in->mouse.buttons[NK_BUTTON_LEFT].clicked_pos.y = scaler.y + scaler.h/2.0f;
            }
        }
    }
    if (!nk_panel_is_sub(layout->type)) {
        /* window is hidden so clear command buffer  */
        if (layout->flags & NK_WINDOW_HIDDEN)
            nk_command_buffer_reset(&window->buffer);
        /* window is visible and not tab */
        else nk_finish(ctx, window);
    }

    /* NK_WINDOW_REMOVE_ROM flag was set so remove NK_WINDOW_ROM */
    if (layout->flags & NK_WINDOW_REMOVE_ROM) {
        layout->flags &= ~(nk_flags)NK_WINDOW_ROM;
        layout->flags &= ~(nk_flags)NK_WINDOW_REMOVE_ROM;
    }
    window->flags = layout->flags;

    /* property garbage collector */
    if (window->property.active && window->property.old != window->property.seq &&
        window->property.active == window->property.prev) {
        nk_zero(&window->property, sizeof(window->property));
    } else {
        window->property.old = window->property.seq;
        window->property.prev = window->property.active;
        window->property.seq = 0;
    }
    /* edit garbage collector */
    if (window->edit.active && window->edit.old != window->edit.seq &&
       window->edit.active == window->edit.prev) {
        nk_zero(&window->edit, sizeof(window->edit));
    } else {
        window->edit.old = window->edit.seq;
        window->edit.prev = window->edit.active;
        window->edit.seq = 0;
    }
    /* contextual garbage collector */
    if (window->popup.active_con && window->popup.con_old != window->popup.con_count) {
        window->popup.con_count = 0;
        window->popup.con_old = 0;
        window->popup.active_con = 0;
    } else {
        window->popup.con_old = window->popup.con_count;
        window->popup.con_count = 0;
    }
    window->popup.combo_count = 0;
    /* helper to make sure you have a 'nk_tree_push' for every 'nk_tree_pop' */
    NK_ASSERT(!layout->row.tree_depth);
}





/* ===============================================================
 *
 *                              WINDOW
 *
 * ===============================================================*/
NK_LIB void*
nk_create_window(struct nk_context *ctx)
{
    struct nk_page_element *elem;
    elem = nk_create_page_element(ctx);
    if (!elem) return 0;
    elem->data.win.seq = ctx->seq;
    return &elem->data.win;
}
NK_LIB void
nk_free_window(struct nk_context *ctx, struct nk_window *win)
{
    /* unlink windows from list */
    struct nk_table *it = win->tables;
    if (win->popup.win) {
        nk_free_window(ctx, win->popup.win);
        win->popup.win = 0;
    }
    win->next = 0;
    win->prev = 0;

    while (it) {
        /*free window state tables */
        struct nk_table *n = it->next;
        nk_remove_table(win, it);
        nk_free_table(ctx, it);
        if (it == win->tables)
            win->tables = n;
        it = n;
    }

    /* link windows into freelist */
    {union nk_page_data *pd = NK_CONTAINER_OF(win, union nk_page_data, win);
    struct nk_page_element *pe = NK_CONTAINER_OF(pd, struct nk_page_element, data);
    nk_free_page_element(ctx, pe);}
}
NK_LIB struct nk_window*
nk_find_window(struct nk_context *ctx, nk_hash hash, const char *name)
{
    struct nk_window *iter;
    iter = ctx->begin;
    while (iter) {
        NK_ASSERT(iter != iter->next);
        if (iter->name == hash) {
            int max_len = nk_strlen(iter->name_string);
            if (!nk_stricmpn(iter->name_string, name, max_len))
                return iter;
        }
        iter = iter->next;
    }
    return 0;
}
NK_LIB void
nk_insert_window(struct nk_context *ctx, struct nk_window *win,
    enum nk_window_insert_location loc)
{
    const struct nk_window *iter;
    NK_ASSERT(ctx);
    NK_ASSERT(win);
    if (!win || !ctx) return;

    iter = ctx->begin;
    while (iter) {
        NK_ASSERT(iter != iter->next);
        NK_ASSERT(iter != win);
        if (iter == win) return;
        iter = iter->next;
    }

    if (!ctx->begin) {
        win->next = 0;
        win->prev = 0;
        ctx->begin = win;
        ctx->end = win;
        ctx->count = 1;
        return;
    }
    if (loc == NK_INSERT_BACK) {
        struct nk_window *end;
        end = ctx->end;
        end->flags |= NK_WINDOW_ROM;
        end->next = win;
        win->prev = ctx->end;
        win->next = 0;
        ctx->end = win;
        ctx->active = ctx->end;
        ctx->end->flags &= ~(nk_flags)NK_WINDOW_ROM;
    } else {
        /*ctx->end->flags |= NK_WINDOW_ROM;*/
        ctx->begin->prev = win;
        win->next = ctx->begin;
        win->prev = 0;
        ctx->begin = win;
        ctx->begin->flags &= ~(nk_flags)NK_WINDOW_ROM;
    }
    ctx->count++;
}
NK_LIB void
nk_remove_window(struct nk_context *ctx, struct nk_window *win)
{
    if (win == ctx->begin || win == ctx->end) {
        if (win == ctx->begin) {
            ctx->begin = win->next;
            if (win->next)
                win->next->prev = 0;
        }
        if (win == ctx->end) {
            ctx->end = win->prev;
            if (win->prev)
                win->prev->next = 0;
        }
    } else {
        if (win->next)
            win->next->prev = win->prev;
        if (win->prev)
            win->prev->next = win->next;
    }
    if (win == ctx->active || !ctx->active) {
        ctx->active = ctx->end;
        if (ctx->end)
            ctx->end->flags &= ~(nk_flags)NK_WINDOW_ROM;
    }
    win->next = 0;
    win->prev = 0;
    ctx->count--;
}
NK_API nk_bool
nk_begin(struct nk_context *ctx, const char *title,
    struct nk_rect bounds, nk_flags flags)
{
    return nk_begin_titled(ctx, title, title, bounds, flags);
}
NK_API nk_bool
nk_begin_titled(struct nk_context *ctx, const char *name, const char *title,
    struct nk_rect bounds, nk_flags flags)
{
    struct nk_window *win;
    struct nk_style *style;
    nk_hash name_hash;
    int name_len;
    int ret = 0;

    NK_ASSERT(ctx);
    NK_ASSERT(name);
    NK_ASSERT(title);
    NK_ASSERT(ctx->style.font && ctx->style.font->width && "if this triggers you forgot to add a font");
    NK_ASSERT(!ctx->current && "if this triggers you missed a `nk_end` call");
    if (!ctx || ctx->current || !title || !name)
        return 0;

    /* find or create window */
    style = &ctx->style;
    name_len = (int)nk_strlen(name);
    name_hash = nk_murmur_hash(name, (int)name_len, NK_WINDOW_TITLE);
    win = nk_find_window(ctx, name_hash, name);
    if (!win) {
        /* create new window */
        nk_size name_length = (nk_size)name_len;
        win = (struct nk_window*)nk_create_window(ctx);
        NK_ASSERT(win);
        if (!win) return 0;

        if (flags & NK_WINDOW_BACKGROUND)
            nk_insert_window(ctx, win, NK_INSERT_FRONT);
        else nk_insert_window(ctx, win, NK_INSERT_BACK);
        nk_command_buffer_init(&win->buffer, &ctx->memory, NK_CLIPPING_ON);

        win->flags = flags;
        win->bounds = bounds;
        win->name = name_hash;
        name_length = NK_MIN(name_length, NK_WINDOW_MAX_NAME-1);
        NK_MEMCPY(win->name_string, name, name_length);
        win->name_string[name_length] = 0;
        win->popup.win = 0;
        win->widgets_disabled = nk_false;
        if (!ctx->active)
            ctx->active = win;
    } else {
        /* update window */
        win->flags &= ~(nk_flags)(NK_WINDOW_PRIVATE-1);
        win->flags |= flags;
        if (!(win->flags & (NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE)))
            win->bounds = bounds;
        /* If this assert triggers you either:
         *
         * I.) Have more than one window with the same name or
         * II.) You forgot to actually draw the window.
         *      More specific you did not call `nk_clear` (nk_clear will be
         *      automatically called for you if you are using one of the
         *      provided demo backends). */
        NK_ASSERT(win->seq != ctx->seq);
        win->seq = ctx->seq;
        if (!ctx->active && !(win->flags & NK_WINDOW_HIDDEN)) {
            ctx->active = win;
            ctx->end = win;
        }
    }
    if (win->flags & NK_WINDOW_HIDDEN) {
        ctx->current = win;
        win->layout = 0;
        return 0;
    } else nk_start(ctx, win);

    /* window overlapping */
    if (!(win->flags & NK_WINDOW_HIDDEN) && !(win->flags & NK_WINDOW_NO_INPUT))
    {
        int inpanel, ishovered;
        struct nk_window *iter = win;
        float h = ctx->style.font->height + 2.0f * style->window.header.padding.y +
            (2.0f * style->window.header.label_padding.y);
        struct nk_rect win_bounds = (!(win->flags & NK_WINDOW_MINIMIZED))?
            win->bounds: nk_rect(win->bounds.x, win->bounds.y, win->bounds.w, h);

        /* activate window if hovered and no other window is overlapping this window */
        inpanel = nk_input_has_mouse_click_down_in_rect(&ctx->input, NK_BUTTON_LEFT, win_bounds, nk_true);
        inpanel = inpanel && ctx->input.mouse.buttons[NK_BUTTON_LEFT].clicked;
        ishovered = nk_input_is_mouse_hovering_rect(&ctx->input, win_bounds);
        if ((win != ctx->active) && ishovered && !ctx->input.mouse.buttons[NK_BUTTON_LEFT].down) {
            iter = win->next;
            while (iter) {
                struct nk_rect iter_bounds = (!(iter->flags & NK_WINDOW_MINIMIZED))?
                    iter->bounds: nk_rect(iter->bounds.x, iter->bounds.y, iter->bounds.w, h);
                if (NK_INTERSECT(win_bounds.x, win_bounds.y, win_bounds.w, win_bounds.h,
                    iter_bounds.x, iter_bounds.y, iter_bounds.w, iter_bounds.h) &&
                    (!(iter->flags & NK_WINDOW_HIDDEN)))
                    break;

                if (iter->popup.win && iter->popup.active && !(iter->flags & NK_WINDOW_HIDDEN) &&
                    NK_INTERSECT(win->bounds.x, win_bounds.y, win_bounds.w, win_bounds.h,
                    iter->popup.win->bounds.x, iter->popup.win->bounds.y,
                    iter->popup.win->bounds.w, iter->popup.win->bounds.h))
                    break;
                iter = iter->next;
            }
        }

        /* activate window if clicked */
        if (iter && inpanel && (win != ctx->end)) {
            iter = win->next;
            while (iter) {
                /* try to find a panel with higher priority in the same position */
                struct nk_rect iter_bounds = (!(iter->flags & NK_WINDOW_MINIMIZED))?
                iter->bounds: nk_rect(iter->bounds.x, iter->bounds.y, iter->bounds.w, h);
                if (NK_INBOX(ctx->input.mouse.pos.x, ctx->input.mouse.pos.y,
                    iter_bounds.x, iter_bounds.y, iter_bounds.w, iter_bounds.h) &&
                    !(iter->flags & NK_WINDOW_HIDDEN))
                    break;
                if (iter->popup.win && iter->popup.active && !(iter->flags & NK_WINDOW_HIDDEN) &&
                    NK_INTERSECT(win_bounds.x, win_bounds.y, win_bounds.w, win_bounds.h,
                    iter->popup.win->bounds.x, iter->popup.win->bounds.y,
                    iter->popup.win->bounds.w, iter->popup.win->bounds.h))
                    break;
                iter = iter->next;
            }
        }
        if (iter && !(win->flags & NK_WINDOW_ROM) && (win->flags & NK_WINDOW_BACKGROUND)) {
            win->flags |= (nk_flags)NK_WINDOW_ROM;
            iter->flags &= ~(nk_flags)NK_WINDOW_ROM;
            ctx->active = iter;
            if (!(iter->flags & NK_WINDOW_BACKGROUND)) {
                /* current window is active in that position so transfer to top
                 * at the highest priority in stack */
                nk_remove_window(ctx, iter);
                nk_insert_window(ctx, iter, NK_INSERT_BACK);
            }
        } else {
            if (!iter && ctx->end != win) {
                if (!(win->flags & NK_WINDOW_BACKGROUND)) {
                    /* current window is active in that position so transfer to top
                     * at the highest priority in stack */
                    nk_remove_window(ctx, win);
                    nk_insert_window(ctx, win, NK_INSERT_BACK);
                }
                win->flags &= ~(nk_flags)NK_WINDOW_ROM;
                ctx->active = win;
            }
            if (ctx->end != win && !(win->flags & NK_WINDOW_BACKGROUND))
                win->flags |= NK_WINDOW_ROM;
        }
    }
    win->layout = (struct nk_panel*)nk_create_panel(ctx);
    ctx->current = win;
    ret = nk_panel_begin(ctx, title, NK_PANEL_WINDOW);
    win->layout->offset_x = &win->scrollbar.x;
    win->layout->offset_y = &win->scrollbar.y;
    return ret;
}
NK_API void
nk_end(struct nk_context *ctx)
{
    struct nk_panel *layout;
    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current && "if this triggers you forgot to call `nk_begin`");
    if (!ctx || !ctx->current)
        return;

    layout = ctx->current->layout;
    if (!layout || (layout->type == NK_PANEL_WINDOW && (ctx->current->flags & NK_WINDOW_HIDDEN))) {
        ctx->current = 0;
        return;
    }
    nk_panel_end(ctx);
    nk_free_panel(ctx, ctx->current->layout);
    ctx->current = 0;
}
NK_API struct nk_rect
nk_window_get_bounds(const struct nk_context *ctx)
{
    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    if (!ctx || !ctx->current) return nk_rect(0,0,0,0);
    return ctx->current->bounds;
}
NK_API struct nk_vec2
nk_window_get_position(const struct nk_context *ctx)
{
    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    if (!ctx || !ctx->current) return nk_vec2(0,0);
    return nk_vec2(ctx->current->bounds.x, ctx->current->bounds.y);
}
NK_API struct nk_vec2
nk_window_get_size(const struct nk_context *ctx)
{
    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    if (!ctx || !ctx->current) return nk_vec2(0,0);
    return nk_vec2(ctx->current->bounds.w, ctx->current->bounds.h);
}
NK_API float
nk_window_get_width(const struct nk_context *ctx)
{
    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    if (!ctx || !ctx->current) return 0;
    return ctx->current->bounds.w;
}
NK_API float
nk_window_get_height(const struct nk_context *ctx)
{
    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    if (!ctx || !ctx->current) return 0;
    return ctx->current->bounds.h;
}
NK_API struct nk_rect
nk_window_get_content_region(struct nk_context *ctx)
{
    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    if (!ctx || !ctx->current) return nk_rect(0,0,0,0);
    return ctx->current->layout->clip;
}
NK_API struct nk_vec2
nk_window_get_content_region_min(struct nk_context *ctx)
{
    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current) return nk_vec2(0,0);
    return nk_vec2(ctx->current->layout->clip.x, ctx->current->layout->clip.y);
}
NK_API struct nk_vec2
nk_window_get_content_region_max(struct nk_context *ctx)
{
    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current) return nk_vec2(0,0);
    return nk_vec2(ctx->current->layout->clip.x + ctx->current->layout->clip.w,
        ctx->current->layout->clip.y + ctx->current->layout->clip.h);
}
NK_API struct nk_vec2
nk_window_get_content_region_size(struct nk_context *ctx)
{
    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current) return nk_vec2(0,0);
    return nk_vec2(ctx->current->layout->clip.w, ctx->current->layout->clip.h);
}
NK_API struct nk_command_buffer*
nk_window_get_canvas(struct nk_context *ctx)
{
    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current) return 0;
    return &ctx->current->buffer;
}
NK_API struct nk_panel*
nk_window_get_panel(struct nk_context *ctx)
{
    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    if (!ctx || !ctx->current) return 0;
    return ctx->current->layout;
}
NK_API void
nk_window_get_scroll(struct nk_context *ctx, nk_uint *offset_x, nk_uint *offset_y)
{
    struct nk_window *win;
    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    if (!ctx || !ctx->current)
        return ;
    win = ctx->current;
    if (offset_x)
      *offset_x = win->scrollbar.x;
    if (offset_y)
      *offset_y = win->scrollbar.y;
}
NK_API nk_bool
nk_window_has_focus(const struct nk_context *ctx)
{
    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current) return 0;
    return ctx->current == ctx->active;
}
NK_API nk_bool
nk_window_is_hovered(struct nk_context *ctx)
{
    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    if (!ctx || !ctx->current || (ctx->current->flags & NK_WINDOW_HIDDEN))
        return 0;
    else {
        struct nk_rect actual_bounds = ctx->current->bounds;
        if (ctx->begin->flags & NK_WINDOW_MINIMIZED) {
            actual_bounds.h = ctx->current->layout->header_height;
        }
        return nk_input_is_mouse_hovering_rect(&ctx->input, actual_bounds);
    }
}
NK_API nk_bool
nk_window_is_any_hovered(struct nk_context *ctx)
{
    struct nk_window *iter;
    NK_ASSERT(ctx);
    if (!ctx) return 0;
    iter = ctx->begin;
    while (iter) {
        /* check if window is being hovered */
        if(!(iter->flags & NK_WINDOW_HIDDEN)) {
            /* check if window popup is being hovered */
            if (iter->popup.active && iter->popup.win && nk_input_is_mouse_hovering_rect(&ctx->input, iter->popup.win->bounds))
                return 1;

            if (iter->flags & NK_WINDOW_MINIMIZED) {
                struct nk_rect header = iter->bounds;
                header.h = ctx->style.font->height + 2 * ctx->style.window.header.padding.y;
                if (nk_input_is_mouse_hovering_rect(&ctx->input, header))
                    return 1;
            } else if (nk_input_is_mouse_hovering_rect(&ctx->input, iter->bounds)) {
                return 1;
            }
        }
        iter = iter->next;
    }
    return 0;
}
NK_API nk_bool
nk_item_is_any_active(struct nk_context *ctx)
{
    int any_hovered = nk_window_is_any_hovered(ctx);
    int any_active = (ctx->last_widget_state & NK_WIDGET_STATE_MODIFIED);
    return any_hovered || any_active;
}
NK_API nk_bool
nk_window_is_collapsed(struct nk_context *ctx, const char *name)
{
    int title_len;
    nk_hash title_hash;
    struct nk_window *win;
    NK_ASSERT(ctx);
    if (!ctx) return 0;

    title_len = (int)nk_strlen(name);
    title_hash = nk_murmur_hash(name, (int)title_len, NK_WINDOW_TITLE);
    win = nk_find_window(ctx, title_hash, name);
    if (!win) return 0;
    return win->flags & NK_WINDOW_MINIMIZED;
}
NK_API nk_bool
nk_window_is_closed(struct nk_context *ctx, const char *name)
{
    int title_len;
    nk_hash title_hash;
    struct nk_window *win;
    NK_ASSERT(ctx);
    if (!ctx) return 1;

    title_len = (int)nk_strlen(name);
    title_hash = nk_murmur_hash(name, (int)title_len, NK_WINDOW_TITLE);
    win = nk_find_window(ctx, title_hash, name);
    if (!win) return 1;
    return (win->flags & NK_WINDOW_CLOSED);
}
NK_API nk_bool
nk_window_is_hidden(struct nk_context *ctx, const char *name)
{
    int title_len;
    nk_hash title_hash;
    struct nk_window *win;
    NK_ASSERT(ctx);
    if (!ctx) return 1;

    title_len = (int)nk_strlen(name);
    title_hash = nk_murmur_hash(name, (int)title_len, NK_WINDOW_TITLE);
    win = nk_find_window(ctx, title_hash, name);
    if (!win) return 1;
    return (win->flags & NK_WINDOW_HIDDEN);
}
NK_API nk_bool
nk_window_is_active(struct nk_context *ctx, const char *name)
{
    int title_len;
    nk_hash title_hash;
    struct nk_window *win;
    NK_ASSERT(ctx);
    if (!ctx) return 0;

    title_len = (int)nk_strlen(name);
    title_hash = nk_murmur_hash(name, (int)title_len, NK_WINDOW_TITLE);
    win = nk_find_window(ctx, title_hash, name);
    if (!win) return 0;
    return win == ctx->active;
}
NK_API struct nk_window*
nk_window_find(struct nk_context *ctx, const char *name)
{
    int title_len;
    nk_hash title_hash;
    title_len = (int)nk_strlen(name);
    title_hash = nk_murmur_hash(name, (int)title_len, NK_WINDOW_TITLE);
    return nk_find_window(ctx, title_hash, name);
}
NK_API void
nk_window_close(struct nk_context *ctx, const char *name)
{
    struct nk_window *win;
    NK_ASSERT(ctx);
    if (!ctx) return;
    win = nk_window_find(ctx, name);
    if (!win) return;
    NK_ASSERT(ctx->current != win && "You cannot close a currently active window");
    if (ctx->current == win) return;
    win->flags |= NK_WINDOW_HIDDEN;
    win->flags |= NK_WINDOW_CLOSED;
}
NK_API void
nk_window_set_bounds(struct nk_context *ctx,
    const char *name, struct nk_rect bounds)
{
    struct nk_window *win;
    NK_ASSERT(ctx);
    if (!ctx) return;
    win = nk_window_find(ctx, name);
    if (!win) return;
    win->bounds = bounds;
}
NK_API void
nk_window_set_position(struct nk_context *ctx,
    const char *name, struct nk_vec2 pos)
{
    struct nk_window *win = nk_window_find(ctx, name);
    if (!win) return;
    win->bounds.x = pos.x;
    win->bounds.y = pos.y;
}
NK_API void
nk_window_set_size(struct nk_context *ctx,
    const char *name, struct nk_vec2 size)
{
    struct nk_window *win = nk_window_find(ctx, name);
    if (!win) return;
    win->bounds.w = size.x;
    win->bounds.h = size.y;
}
NK_API void
nk_window_set_scroll(struct nk_context *ctx, nk_uint offset_x, nk_uint offset_y)
{
    struct nk_window *win;
    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    if (!ctx || !ctx->current)
        return;
    win = ctx->current;
    win->scrollbar.x = offset_x;
    win->scrollbar.y = offset_y;
}
NK_API void
nk_window_collapse(struct nk_context *ctx, const char *name,
                    enum nk_collapse_states c)
{
    int title_len;
    nk_hash title_hash;
    struct nk_window *win;
    NK_ASSERT(ctx);
    if (!ctx) return;

    title_len = (int)nk_strlen(name);
    title_hash = nk_murmur_hash(name, (int)title_len, NK_WINDOW_TITLE);
    win = nk_find_window(ctx, title_hash, name);
    if (!win) return;
    if (c == NK_MINIMIZED)
        win->flags |= NK_WINDOW_MINIMIZED;
    else win->flags &= ~(nk_flags)NK_WINDOW_MINIMIZED;
}
NK_API void
nk_window_collapse_if(struct nk_context *ctx, const char *name,
    enum nk_collapse_states c, int cond)
{
    NK_ASSERT(ctx);
    if (!ctx || !cond) return;
    nk_window_collapse(ctx, name, c);
}
NK_API void
nk_window_show(struct nk_context *ctx, const char *name, enum nk_show_states s)
{
    int title_len;
    nk_hash title_hash;
    struct nk_window *win;
    NK_ASSERT(ctx);
    if (!ctx) return;

    title_len = (int)nk_strlen(name);
    title_hash = nk_murmur_hash(name, (int)title_len, NK_WINDOW_TITLE);
    win = nk_find_window(ctx, title_hash, name);
    if (!win) return;
    if (s == NK_HIDDEN) {
        win->flags |= NK_WINDOW_HIDDEN;
    } else win->flags &= ~(nk_flags)NK_WINDOW_HIDDEN;
}
NK_API void
nk_window_show_if(struct nk_context *ctx, const char *name,
    enum nk_show_states s, int cond)
{
    NK_ASSERT(ctx);
    if (!ctx || !cond) return;
    nk_window_show(ctx, name, s);
}

NK_API void
nk_window_set_focus(struct nk_context *ctx, const char *name)
{
    int title_len;
    nk_hash title_hash;
    struct nk_window *win;
    NK_ASSERT(ctx);
    if (!ctx) return;

    title_len = (int)nk_strlen(name);
    title_hash = nk_murmur_hash(name, (int)title_len, NK_WINDOW_TITLE);
    win = nk_find_window(ctx, title_hash, name);
    if (win && ctx->end != win) {
        nk_remove_window(ctx, win);
        nk_insert_window(ctx, win, NK_INSERT_BACK);
    }
    ctx->active = win;
}
NK_API void
nk_rule_horizontal(struct nk_context *ctx, struct nk_color color, nk_bool rounding)
{
    struct nk_rect space;
    enum nk_widget_layout_states state = nk_widget(&space, ctx);
    struct nk_command_buffer *canvas = nk_window_get_canvas(ctx);
    if (!state) return;
    nk_fill_rect(canvas, space, rounding && space.h > 1.5f ? space.h / 2.0f : 0, color);
}




/* ===============================================================
 *
 *                              POPUP
 *
 * ===============================================================*/
NK_API nk_bool
nk_popup_begin(struct nk_context *ctx, enum nk_popup_type type,
    const char *title, nk_flags flags, struct nk_rect rect)
{
    struct nk_window *popup;
    struct nk_window *win;
    struct nk_panel *panel;

    int title_len;
    nk_hash title_hash;
    nk_size allocated;

    NK_ASSERT(ctx);
    NK_ASSERT(title);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    win = ctx->current;
    panel = win->layout;
    NK_ASSERT(!((int)panel->type & (int)NK_PANEL_SET_POPUP) && "popups are not allowed to have popups");
    (void)panel;
    title_len = (int)nk_strlen(title);
    title_hash = nk_murmur_hash(title, (int)title_len, NK_PANEL_POPUP);

    popup = win->popup.win;
    if (!popup) {
        popup = (struct nk_window*)nk_create_window(ctx);
        popup->parent = win;
        win->popup.win = popup;
        win->popup.active = 0;
        win->popup.type = NK_PANEL_POPUP;
    }

    /* make sure we have correct popup */
    if (win->popup.name != title_hash) {
        if (!win->popup.active) {
            nk_zero(popup, sizeof(*popup));
            win->popup.name = title_hash;
            win->popup.active = 1;
            win->popup.type = NK_PANEL_POPUP;
        } else return 0;
    }

    /* popup position is local to window */
    ctx->current = popup;
    rect.x += win->layout->clip.x;
    rect.y += win->layout->clip.y;

    /* setup popup data */
    popup->parent = win;
    popup->bounds = rect;
    popup->seq = ctx->seq;
    popup->layout = (struct nk_panel*)nk_create_panel(ctx);
    popup->flags = flags;
    popup->flags |= NK_WINDOW_BORDER;
    if (type == NK_POPUP_DYNAMIC)
        popup->flags |= NK_WINDOW_DYNAMIC;

    popup->buffer = win->buffer;
    nk_start_popup(ctx, win);
    allocated = ctx->memory.allocated;
    nk_push_scissor(&popup->buffer, nk_null_rect);

    if (nk_panel_begin(ctx, title, NK_PANEL_POPUP)) {
        /* popup is running therefore invalidate parent panels */
        struct nk_panel *root;
        root = win->layout;
        while (root) {
            root->flags |= NK_WINDOW_ROM;
            root->flags &= ~(nk_flags)NK_WINDOW_REMOVE_ROM;
            root = root->parent;
        }
        win->popup.active = 1;
        popup->layout->offset_x = &popup->scrollbar.x;
        popup->layout->offset_y = &popup->scrollbar.y;
        popup->layout->parent = win->layout;
        return 1;
    } else {
        /* popup was closed/is invalid so cleanup */
        struct nk_panel *root;
        root = win->layout;
        while (root) {
            root->flags |= NK_WINDOW_REMOVE_ROM;
            root = root->parent;
        }
        win->popup.buf.active = 0;
        win->popup.active = 0;
        ctx->memory.allocated = allocated;
        ctx->current = win;
        nk_free_panel(ctx, popup->layout);
        popup->layout = 0;
        return 0;
    }
}
NK_LIB nk_bool
nk_nonblock_begin(struct nk_context *ctx,
    nk_flags flags, struct nk_rect body, struct nk_rect header,
    enum nk_panel_type panel_type)
{
    struct nk_window *popup;
    struct nk_window *win;
    struct nk_panel *panel;
    int is_active = nk_true;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    /* popups cannot have popups */
    win = ctx->current;
    panel = win->layout;
    NK_ASSERT(!((int)panel->type & (int)NK_PANEL_SET_POPUP));
    (void)panel;
    popup = win->popup.win;
    if (!popup) {
        /* create window for nonblocking popup */
        popup = (struct nk_window*)nk_create_window(ctx);
        popup->parent = win;
        win->popup.win = popup;
        win->popup.type = panel_type;
        nk_command_buffer_init(&popup->buffer, &ctx->memory, NK_CLIPPING_ON);
    } else {
        /* close the popup if user pressed outside or in the header */
        int pressed, in_body, in_header;
#ifdef NK_BUTTON_TRIGGER_ON_RELEASE
        pressed = nk_input_is_mouse_released(&ctx->input, NK_BUTTON_LEFT);
#else
        pressed = nk_input_is_mouse_pressed(&ctx->input, NK_BUTTON_LEFT);
#endif
        in_body = nk_input_is_mouse_hovering_rect(&ctx->input, body);
        in_header = nk_input_is_mouse_hovering_rect(&ctx->input, header);
        if (pressed && (!in_body || in_header))
            is_active = nk_false;
    }
    win->popup.header = header;

    if (!is_active) {
        /* remove read only mode from all parent panels */
        struct nk_panel *root = win->layout;
        while (root) {
            root->flags |= NK_WINDOW_REMOVE_ROM;
            root = root->parent;
        }
        return is_active;
    }
    popup->bounds = body;
    popup->parent = win;
    popup->layout = (struct nk_panel*)nk_create_panel(ctx);
    popup->flags = flags;
    popup->flags |= NK_WINDOW_BORDER;
    popup->flags |= NK_WINDOW_DYNAMIC;
    popup->seq = ctx->seq;
    win->popup.active = 1;
    NK_ASSERT(popup->layout);

    nk_start_popup(ctx, win);
    popup->buffer = win->buffer;
    nk_push_scissor(&popup->buffer, nk_null_rect);
    ctx->current = popup;

    nk_panel_begin(ctx, 0, panel_type);
    win->buffer = popup->buffer;
    popup->layout->parent = win->layout;
    popup->layout->offset_x = &popup->scrollbar.x;
    popup->layout->offset_y = &popup->scrollbar.y;

    /* set read only mode to all parent panels */
    {struct nk_panel *root;
    root = win->layout;
    while (root) {
        root->flags |= NK_WINDOW_ROM;
        root = root->parent;
    }}
    return is_active;
}
NK_API void
nk_popup_close(struct nk_context *ctx)
{
    struct nk_window *popup;
    NK_ASSERT(ctx);
    if (!ctx || !ctx->current) return;

    popup = ctx->current;
    NK_ASSERT(popup->parent);
    NK_ASSERT((int)popup->layout->type & (int)NK_PANEL_SET_POPUP);
    popup->flags |= NK_WINDOW_HIDDEN;
}
NK_API void
nk_popup_end(struct nk_context *ctx)
{
    struct nk_window *win;
    struct nk_window *popup;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    popup = ctx->current;
    if (!popup->parent) return;
    win = popup->parent;
    if (popup->flags & NK_WINDOW_HIDDEN) {
        struct nk_panel *root;
        root = win->layout;
        while (root) {
            root->flags |= NK_WINDOW_REMOVE_ROM;
            root = root->parent;
        }
        win->popup.active = 0;
    }
    nk_push_scissor(&popup->buffer, nk_null_rect);
    nk_end(ctx);

    win->buffer = popup->buffer;
    nk_finish_popup(ctx, win);
    ctx->current = win;
    nk_push_scissor(&win->buffer, win->layout->clip);
}
NK_API void
nk_popup_get_scroll(struct nk_context *ctx, nk_uint *offset_x, nk_uint *offset_y)
{
    struct nk_window *popup;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    popup = ctx->current;
    if (offset_x)
      *offset_x = popup->scrollbar.x;
    if (offset_y)
      *offset_y = popup->scrollbar.y;
}
NK_API void
nk_popup_set_scroll(struct nk_context *ctx, nk_uint offset_x, nk_uint offset_y)
{
    struct nk_window *popup;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    popup = ctx->current;
    popup->scrollbar.x = offset_x;
    popup->scrollbar.y = offset_y;
}




/* ==============================================================
 *
 *                          CONTEXTUAL
 *
 * ===============================================================*/
NK_API nk_bool
nk_contextual_begin(struct nk_context *ctx, nk_flags flags, struct nk_vec2 size,
    struct nk_rect trigger_bounds)
{
    struct nk_window *win;
    struct nk_window *popup;
    struct nk_rect body;
    struct nk_input* in;

    NK_STORAGE const struct nk_rect null_rect = {-1,-1,0,0};
    int is_clicked = 0;
    int is_open = 0;
    int ret = 0;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    win = ctx->current;
    ++win->popup.con_count;
    if (ctx->current != ctx->active)
        return 0;

    /* check if currently active contextual is active */
    popup = win->popup.win;
    is_open = (popup && win->popup.type == NK_PANEL_CONTEXTUAL);
    in = win->widgets_disabled ? 0 : &ctx->input;
    if (in) {
        is_clicked = nk_input_mouse_clicked(in, NK_BUTTON_RIGHT, trigger_bounds);
        if (win->popup.active_con && win->popup.con_count != win->popup.active_con)
            return 0;
        if (!is_open && win->popup.active_con)
            win->popup.active_con = 0;
        if ((!is_open && !is_clicked))
            return 0;

        /* calculate contextual position on click */
        win->popup.active_con = win->popup.con_count;
        if (is_clicked) {
            body.x = in->mouse.pos.x;
            body.y = in->mouse.pos.y;
        } else {
            body.x = popup->bounds.x;
            body.y = popup->bounds.y;
        }

        body.w = size.x;
        body.h = size.y;

        /* start nonblocking contextual popup */
        ret = nk_nonblock_begin(ctx, flags | NK_WINDOW_NO_SCROLLBAR, body,
            null_rect, NK_PANEL_CONTEXTUAL);
        if (ret) win->popup.type = NK_PANEL_CONTEXTUAL;
        else {
            win->popup.active_con = 0;
            win->popup.type = NK_PANEL_NONE;
            if (win->popup.win)
                win->popup.win->flags = 0;
        }
    }
    return ret;
}
NK_API nk_bool
nk_contextual_item_text(struct nk_context *ctx, const char *text, int len,
    nk_flags alignment)
{
    struct nk_window *win;
    const struct nk_input *in;
    const struct nk_style *style;

    struct nk_rect bounds;
    enum nk_widget_layout_states state;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    win = ctx->current;
    style = &ctx->style;
    state = nk_widget_fitting(&bounds, ctx, style->contextual_button.padding);
    if (!state) return nk_false;

    in = (state == NK_WIDGET_ROM || win->layout->flags & NK_WINDOW_ROM) ? 0 : &ctx->input;
    if (nk_do_button_text(&ctx->last_widget_state, &win->buffer, bounds,
        text, len, alignment, NK_BUTTON_DEFAULT, &style->contextual_button, in, style->font)) {
        nk_contextual_close(ctx);
        return nk_true;
    }
    return nk_false;
}
NK_API nk_bool
nk_contextual_item_label(struct nk_context *ctx, const char *label, nk_flags align)
{
    return nk_contextual_item_text(ctx, label, nk_strlen(label), align);
}
NK_API nk_bool
nk_contextual_item_image_text(struct nk_context *ctx, struct nk_image img,
    const char *text, int len, nk_flags align)
{
    struct nk_window *win;
    const struct nk_input *in;
    const struct nk_style *style;

    struct nk_rect bounds;
    enum nk_widget_layout_states state;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    win = ctx->current;
    style = &ctx->style;
    state = nk_widget_fitting(&bounds, ctx, style->contextual_button.padding);
    if (!state) return nk_false;

    in = (state == NK_WIDGET_ROM || win->layout->flags & NK_WINDOW_ROM) ? 0 : &ctx->input;
    if (nk_do_button_text_image(&ctx->last_widget_state, &win->buffer, bounds,
        img, text, len, align, NK_BUTTON_DEFAULT, &style->contextual_button, style->font, in)){
        nk_contextual_close(ctx);
        return nk_true;
    }
    return nk_false;
}
NK_API nk_bool
nk_contextual_item_image_label(struct nk_context *ctx, struct nk_image img,
    const char *label, nk_flags align)
{
    return nk_contextual_item_image_text(ctx, img, label, nk_strlen(label), align);
}
NK_API nk_bool
nk_contextual_item_symbol_text(struct nk_context *ctx, enum nk_symbol_type symbol,
    const char *text, int len, nk_flags align)
{
    struct nk_window *win;
    const struct nk_input *in;
    const struct nk_style *style;

    struct nk_rect bounds;
    enum nk_widget_layout_states state;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    win = ctx->current;
    style = &ctx->style;
    state = nk_widget_fitting(&bounds, ctx, style->contextual_button.padding);
    if (!state) return nk_false;

    in = (state == NK_WIDGET_ROM || win->layout->flags & NK_WINDOW_ROM) ? 0 : &ctx->input;
    if (nk_do_button_text_symbol(&ctx->last_widget_state, &win->buffer, bounds,
        symbol, text, len, align, NK_BUTTON_DEFAULT, &style->contextual_button, style->font, in)) {
        nk_contextual_close(ctx);
        return nk_true;
    }
    return nk_false;
}
NK_API nk_bool
nk_contextual_item_symbol_label(struct nk_context *ctx, enum nk_symbol_type symbol,
    const char *text, nk_flags align)
{
    return nk_contextual_item_symbol_text(ctx, symbol, text, nk_strlen(text), align);
}
NK_API void
nk_contextual_close(struct nk_context *ctx)
{
    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout) return;
    nk_popup_close(ctx);
}
NK_API void
nk_contextual_end(struct nk_context *ctx)
{
    struct nk_window *popup;
    struct nk_panel *panel;
    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    if (!ctx || !ctx->current) return;

    popup = ctx->current;
    panel = popup->layout;
    NK_ASSERT(popup->parent);
    NK_ASSERT((int)panel->type & (int)NK_PANEL_SET_POPUP);
    if (panel->flags & NK_WINDOW_DYNAMIC) {
        /* Close behavior
        This is a bit of a hack solution since we do not know before we end our popup
        how big it will be. We therefore do not directly know when a
        click outside the non-blocking popup must close it at that direct frame.
        Instead it will be closed in the next frame.*/
        struct nk_rect body = {0,0,0,0};
        if (panel->at_y < (panel->bounds.y + panel->bounds.h)) {
            struct nk_vec2 padding = nk_panel_get_padding(&ctx->style, panel->type);
            body = panel->bounds;
            body.y = (panel->at_y + panel->footer_height + panel->border + padding.y + panel->row.height);
            body.h = (panel->bounds.y + panel->bounds.h) - body.y;
        }
        {int pressed = nk_input_is_mouse_pressed(&ctx->input, NK_BUTTON_LEFT);
        int in_body = nk_input_is_mouse_hovering_rect(&ctx->input, body);
        if (pressed && in_body)
            popup->flags |= NK_WINDOW_HIDDEN;
        }
    }
    if (popup->flags & NK_WINDOW_HIDDEN)
        popup->seq = 0;
    nk_popup_end(ctx);
    return;
}





/* ===============================================================
 *
 *                              MENU
 *
 * ===============================================================*/
NK_API void
nk_menubar_begin(struct nk_context *ctx)
{
    struct nk_panel *layout;
    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    layout = ctx->current->layout;
    NK_ASSERT(layout->at_y == layout->bounds.y);
    /* if this assert triggers you allocated space between nk_begin and nk_menubar_begin.
    If you want a menubar the first nuklear function after `nk_begin` has to be a
    `nk_menubar_begin` call. Inside the menubar you then have to allocate space for
    widgets (also supports multiple rows).
    Example:
        if (nk_begin(...)) {
            nk_menubar_begin(...);
                nk_layout_xxxx(...);
                nk_button(...);
                nk_layout_xxxx(...);
                nk_button(...);
            nk_menubar_end(...);
        }
        nk_end(...);
    */
    if (layout->flags & NK_WINDOW_HIDDEN || layout->flags & NK_WINDOW_MINIMIZED)
        return;

    layout->menu.x = layout->at_x;
    layout->menu.y = layout->at_y + layout->row.height;
    layout->menu.w = layout->bounds.w;
    layout->menu.offset.x = *layout->offset_x;
    layout->menu.offset.y = *layout->offset_y;
    *layout->offset_y = 0;
}
NK_API void
nk_menubar_end(struct nk_context *ctx)
{
    struct nk_window *win;
    struct nk_panel *layout;
    struct nk_command_buffer *out;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    win = ctx->current;
    out = &win->buffer;
    layout = win->layout;
    if (layout->flags & NK_WINDOW_HIDDEN || layout->flags & NK_WINDOW_MINIMIZED)
        return;

    layout->menu.h  = layout->at_y - layout->menu.y;
    layout->menu.h += layout->row.height + ctx->style.window.spacing.y;

    layout->bounds.y += layout->menu.h;
    layout->bounds.h -= layout->menu.h;

    *layout->offset_x = layout->menu.offset.x;
    *layout->offset_y = layout->menu.offset.y;
    layout->at_y      = layout->bounds.y - layout->row.height;

    layout->clip.y = layout->bounds.y;
    layout->clip.h = layout->bounds.h;
    nk_push_scissor(out, layout->clip);
}
NK_INTERN int
nk_menu_begin(struct nk_context *ctx, struct nk_window *win,
    const char *id, int is_clicked, struct nk_rect header, struct nk_vec2 size)
{
    int is_open = 0;
    int is_active = 0;
    struct nk_rect body;
    struct nk_window *popup;
    nk_hash hash = nk_murmur_hash(id, (int)nk_strlen(id), NK_PANEL_MENU);

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    body.x = header.x;
    body.w = size.x;
    body.y = header.y + header.h;
    body.h = size.y;

    popup = win->popup.win;
    is_open = popup ? nk_true : nk_false;
    is_active = (popup && (win->popup.name == hash) && win->popup.type == NK_PANEL_MENU);
    if ((is_clicked && is_open && !is_active) || (is_open && !is_active) ||
        (!is_open && !is_active && !is_clicked)) return 0;
    if (!nk_nonblock_begin(ctx, NK_WINDOW_NO_SCROLLBAR, body, header, NK_PANEL_MENU))
        return 0;

    win->popup.type = NK_PANEL_MENU;
    win->popup.name = hash;
    return 1;
}
NK_API nk_bool
nk_menu_begin_text(struct nk_context *ctx, const char *title, int len,
    nk_flags align, struct nk_vec2 size)
{
    struct nk_window *win;
    const struct nk_input *in;
    struct nk_rect header;
    int is_clicked = nk_false;
    nk_flags state;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    win = ctx->current;
    state = nk_widget(&header, ctx);
    if (!state) return 0;
    in = (state == NK_WIDGET_ROM || state == NK_WIDGET_DISABLED || win->flags & NK_WINDOW_ROM) ? 0 : &ctx->input;
    if (nk_do_button_text(&ctx->last_widget_state, &win->buffer, header,
        title, len, align, NK_BUTTON_DEFAULT, &ctx->style.menu_button, in, ctx->style.font))
        is_clicked = nk_true;
    return nk_menu_begin(ctx, win, title, is_clicked, header, size);
}
NK_API nk_bool nk_menu_begin_label(struct nk_context *ctx,
    const char *text, nk_flags align, struct nk_vec2 size)
{
    return nk_menu_begin_text(ctx, text, nk_strlen(text), align, size);
}
NK_API nk_bool
nk_menu_begin_image(struct nk_context *ctx, const char *id, struct nk_image img,
    struct nk_vec2 size)
{
    struct nk_window *win;
    struct nk_rect header;
    const struct nk_input *in;
    int is_clicked = nk_false;
    nk_flags state;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    win = ctx->current;
    state = nk_widget(&header, ctx);
    if (!state) return 0;
    in = (state == NK_WIDGET_ROM || state == NK_WIDGET_DISABLED || win->layout->flags & NK_WINDOW_ROM) ? 0 : &ctx->input;
    if (nk_do_button_image(&ctx->last_widget_state, &win->buffer, header,
        img, NK_BUTTON_DEFAULT, &ctx->style.menu_button, in))
        is_clicked = nk_true;
    return nk_menu_begin(ctx, win, id, is_clicked, header, size);
}
NK_API nk_bool
nk_menu_begin_symbol(struct nk_context *ctx, const char *id,
    enum nk_symbol_type sym, struct nk_vec2 size)
{
    struct nk_window *win;
    const struct nk_input *in;
    struct nk_rect header;
    int is_clicked = nk_false;
    nk_flags state;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    win = ctx->current;
    state = nk_widget(&header, ctx);
    if (!state) return 0;
    in = (state == NK_WIDGET_ROM || state == NK_WIDGET_DISABLED || win->layout->flags & NK_WINDOW_ROM) ? 0 : &ctx->input;
    if (nk_do_button_symbol(&ctx->last_widget_state,  &win->buffer, header,
        sym, NK_BUTTON_DEFAULT, &ctx->style.menu_button, in, ctx->style.font))
        is_clicked = nk_true;
    return nk_menu_begin(ctx, win, id, is_clicked, header, size);
}
NK_API nk_bool
nk_menu_begin_image_text(struct nk_context *ctx, const char *title, int len,
    nk_flags align, struct nk_image img, struct nk_vec2 size)
{
    struct nk_window *win;
    struct nk_rect header;
    const struct nk_input *in;
    int is_clicked = nk_false;
    nk_flags state;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    win = ctx->current;
    state = nk_widget(&header, ctx);
    if (!state) return 0;
    in = (state == NK_WIDGET_ROM || state == NK_WIDGET_DISABLED || win->layout->flags & NK_WINDOW_ROM) ? 0 : &ctx->input;
    if (nk_do_button_text_image(&ctx->last_widget_state, &win->buffer,
        header, img, title, len, align, NK_BUTTON_DEFAULT, &ctx->style.menu_button,
        ctx->style.font, in))
        is_clicked = nk_true;
    return nk_menu_begin(ctx, win, title, is_clicked, header, size);
}
NK_API nk_bool
nk_menu_begin_image_label(struct nk_context *ctx,
    const char *title, nk_flags align, struct nk_image img, struct nk_vec2 size)
{
    return nk_menu_begin_image_text(ctx, title, nk_strlen(title), align, img, size);
}
NK_API nk_bool
nk_menu_begin_symbol_text(struct nk_context *ctx, const char *title, int len,
    nk_flags align, enum nk_symbol_type sym, struct nk_vec2 size)
{
    struct nk_window *win;
    struct nk_rect header;
    const struct nk_input *in;
    int is_clicked = nk_false;
    nk_flags state;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    win = ctx->current;
    state = nk_widget(&header, ctx);
    if (!state) return 0;

    in = (state == NK_WIDGET_ROM || state == NK_WIDGET_DISABLED || win->layout->flags & NK_WINDOW_ROM) ? 0 : &ctx->input;
    if (nk_do_button_text_symbol(&ctx->last_widget_state, &win->buffer,
        header, sym, title, len, align, NK_BUTTON_DEFAULT, &ctx->style.menu_button,
        ctx->style.font, in)) is_clicked = nk_true;
    return nk_menu_begin(ctx, win, title, is_clicked, header, size);
}
NK_API nk_bool
nk_menu_begin_symbol_label(struct nk_context *ctx,
    const char *title, nk_flags align, enum nk_symbol_type sym, struct nk_vec2 size )
{
    return nk_menu_begin_symbol_text(ctx, title, nk_strlen(title), align,sym,size);
}
NK_API nk_bool
nk_menu_item_text(struct nk_context *ctx, const char *title, int len, nk_flags align)
{
    return nk_contextual_item_text(ctx, title, len, align);
}
NK_API nk_bool
nk_menu_item_label(struct nk_context *ctx, const char *label, nk_flags align)
{
    return nk_contextual_item_label(ctx, label, align);
}
NK_API nk_bool
nk_menu_item_image_label(struct nk_context *ctx, struct nk_image img,
    const char *label, nk_flags align)
{
    return nk_contextual_item_image_label(ctx, img, label, align);
}
NK_API nk_bool
nk_menu_item_image_text(struct nk_context *ctx, struct nk_image img,
    const char *text, int len, nk_flags align)
{
    return nk_contextual_item_image_text(ctx, img, text, len, align);
}
NK_API nk_bool nk_menu_item_symbol_text(struct nk_context *ctx, enum nk_symbol_type sym,
    const char *text, int len, nk_flags align)
{
    return nk_contextual_item_symbol_text(ctx, sym, text, len, align);
}
NK_API nk_bool nk_menu_item_symbol_label(struct nk_context *ctx, enum nk_symbol_type sym,
    const char *label, nk_flags align)
{
    return nk_contextual_item_symbol_label(ctx, sym, label, align);
}
NK_API void nk_menu_close(struct nk_context *ctx)
{
    nk_contextual_close(ctx);
}
NK_API void
nk_menu_end(struct nk_context *ctx)
{
    nk_contextual_end(ctx);
}





/* ===============================================================
 *
 *                          LAYOUT
 *
 * ===============================================================*/
NK_API void
nk_layout_set_min_row_height(struct nk_context *ctx, float height)
{
    struct nk_window *win;
    struct nk_panel *layout;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    win = ctx->current;
    layout = win->layout;
    layout->row.min_height = height;
}
NK_API void
nk_layout_reset_min_row_height(struct nk_context *ctx)
{
    struct nk_window *win;
    struct nk_panel *layout;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    win = ctx->current;
    layout = win->layout;
    layout->row.min_height = ctx->style.font->height;
    layout->row.min_height += ctx->style.text.padding.y*2;
    layout->row.min_height += ctx->style.window.min_row_height_padding*2;
}
NK_LIB float
nk_layout_row_calculate_usable_space(const struct nk_style *style, enum nk_panel_type type,
    float total_space, int columns)
{
    float panel_spacing;
    float panel_space;

    struct nk_vec2 spacing;

    NK_UNUSED(type);

    spacing = style->window.spacing;

    /* calculate the usable panel space */
    panel_spacing = (float)NK_MAX(columns - 1, 0) * spacing.x;
    panel_space  = total_space - panel_spacing;
    return panel_space;
}
NK_LIB void
nk_panel_layout(const struct nk_context *ctx, struct nk_window *win,
    float height, int cols)
{
    struct nk_panel *layout;
    const struct nk_style *style;
    struct nk_command_buffer *out;

    struct nk_vec2 item_spacing;
    struct nk_color color;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    /* prefetch some configuration data */
    layout = win->layout;
    style = &ctx->style;
    out = &win->buffer;
    color = style->window.background;
    item_spacing = style->window.spacing;

    /*  if one of these triggers you forgot to add an `if` condition around either
        a window, group, popup, combobox or contextual menu `begin` and `end` block.
        Example:
            if (nk_begin(...) {...} nk_end(...); or
            if (nk_group_begin(...) { nk_group_end(...);} */
    NK_ASSERT(!(layout->flags & NK_WINDOW_MINIMIZED));
    NK_ASSERT(!(layout->flags & NK_WINDOW_HIDDEN));
    NK_ASSERT(!(layout->flags & NK_WINDOW_CLOSED));

    /* update the current row and set the current row layout */
    layout->row.index = 0;
    layout->at_y += layout->row.height;
    layout->row.columns = cols;
    if (height == 0.0f)
        layout->row.height = NK_MAX(height, layout->row.min_height) + item_spacing.y;
    else layout->row.height = height + item_spacing.y;

    layout->row.item_offset = 0;
    if (layout->flags & NK_WINDOW_DYNAMIC) {
        /* draw background for dynamic panels */
        struct nk_rect background;
        background.x = win->bounds.x;
        background.w = win->bounds.w;
        background.y = layout->at_y - 1.0f;
        background.h = layout->row.height + 1.0f;
        nk_fill_rect(out, background, 0, color);
    }
}
NK_LIB void
nk_row_layout(struct nk_context *ctx, enum nk_layout_format fmt,
    float height, int cols, int width)
{
    /* update the current row and set the current row layout */
    struct nk_window *win;
    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    win = ctx->current;
    nk_panel_layout(ctx, win, height, cols);
    if (fmt == NK_DYNAMIC)
        win->layout->row.type = NK_LAYOUT_DYNAMIC_FIXED;
    else win->layout->row.type = NK_LAYOUT_STATIC_FIXED;

    win->layout->row.ratio = 0;
    win->layout->row.filled = 0;
    win->layout->row.item_offset = 0;
    win->layout->row.item_width = (float)width;
}
NK_API float
nk_layout_ratio_from_pixel(struct nk_context *ctx, float pixel_width)
{
    struct nk_window *win;
    NK_ASSERT(ctx);
    NK_ASSERT(pixel_width);
    if (!ctx || !ctx->current || !ctx->current->layout) return 0;
    win = ctx->current;
    return NK_CLAMP(0.0f, pixel_width/win->bounds.x, 1.0f);
}
NK_API void
nk_layout_row_dynamic(struct nk_context *ctx, float height, int cols)
{
    nk_row_layout(ctx, NK_DYNAMIC, height, cols, 0);
}
NK_API void
nk_layout_row_static(struct nk_context *ctx, float height, int item_width, int cols)
{
    nk_row_layout(ctx, NK_STATIC, height, cols, item_width);
}
NK_API void
nk_layout_row_begin(struct nk_context *ctx, enum nk_layout_format fmt,
    float row_height, int cols)
{
    struct nk_window *win;
    struct nk_panel *layout;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    win = ctx->current;
    layout = win->layout;
    nk_panel_layout(ctx, win, row_height, cols);
    if (fmt == NK_DYNAMIC)
        layout->row.type = NK_LAYOUT_DYNAMIC_ROW;
    else layout->row.type = NK_LAYOUT_STATIC_ROW;

    layout->row.ratio = 0;
    layout->row.filled = 0;
    layout->row.item_width = 0;
    layout->row.item_offset = 0;
    layout->row.columns = cols;
}
NK_API void
nk_layout_row_push(struct nk_context *ctx, float ratio_or_width)
{
    struct nk_window *win;
    struct nk_panel *layout;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    win = ctx->current;
    layout = win->layout;
    NK_ASSERT(layout->row.type == NK_LAYOUT_STATIC_ROW || layout->row.type == NK_LAYOUT_DYNAMIC_ROW);
    if (layout->row.type != NK_LAYOUT_STATIC_ROW && layout->row.type != NK_LAYOUT_DYNAMIC_ROW)
        return;

    if (layout->row.type == NK_LAYOUT_DYNAMIC_ROW) {
        float ratio = ratio_or_width;
        if ((ratio + layout->row.filled) > 1.0f) return;
        if (ratio > 0.0f)
            layout->row.item_width = NK_SATURATE(ratio);
        else layout->row.item_width = 1.0f - layout->row.filled;
    } else layout->row.item_width = ratio_or_width;
}
NK_API void
nk_layout_row_end(struct nk_context *ctx)
{
    struct nk_window *win;
    struct nk_panel *layout;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    win = ctx->current;
    layout = win->layout;
    NK_ASSERT(layout->row.type == NK_LAYOUT_STATIC_ROW || layout->row.type == NK_LAYOUT_DYNAMIC_ROW);
    if (layout->row.type != NK_LAYOUT_STATIC_ROW && layout->row.type != NK_LAYOUT_DYNAMIC_ROW)
        return;
    layout->row.item_width = 0;
    layout->row.item_offset = 0;
}
NK_API void
nk_layout_row(struct nk_context *ctx, enum nk_layout_format fmt,
    float height, int cols, const float *ratio)
{
    int i;
    int n_undef = 0;
    struct nk_window *win;
    struct nk_panel *layout;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    win = ctx->current;
    layout = win->layout;
    nk_panel_layout(ctx, win, height, cols);
    if (fmt == NK_DYNAMIC) {
        /* calculate width of undefined widget ratios */
        float r = 0;
        layout->row.ratio = ratio;
        for (i = 0; i < cols; ++i) {
            if (ratio[i] < 0.0f)
                n_undef++;
            else r += ratio[i];
        }
        r = NK_SATURATE(1.0f - r);
        layout->row.type = NK_LAYOUT_DYNAMIC;
        layout->row.item_width = (r > 0 && n_undef > 0) ? (r / (float)n_undef):0;
    } else {
        layout->row.ratio = ratio;
        layout->row.type = NK_LAYOUT_STATIC;
        layout->row.item_width = 0;
        layout->row.item_offset = 0;
    }
    layout->row.item_offset = 0;
    layout->row.filled = 0;
}
NK_API void
nk_layout_row_template_begin(struct nk_context *ctx, float height)
{
    struct nk_window *win;
    struct nk_panel *layout;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    win = ctx->current;
    layout = win->layout;
    nk_panel_layout(ctx, win, height, 1);
    layout->row.type = NK_LAYOUT_TEMPLATE;
    layout->row.columns = 0;
    layout->row.ratio = 0;
    layout->row.item_width = 0;
    layout->row.item_height = 0;
    layout->row.item_offset = 0;
    layout->row.filled = 0;
    layout->row.item.x = 0;
    layout->row.item.y = 0;
    layout->row.item.w = 0;
    layout->row.item.h = 0;
}
NK_API void
nk_layout_row_template_push_dynamic(struct nk_context *ctx)
{
    struct nk_window *win;
    struct nk_panel *layout;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    win = ctx->current;
    layout = win->layout;
    NK_ASSERT(layout->row.type == NK_LAYOUT_TEMPLATE);
    NK_ASSERT(layout->row.columns < NK_MAX_LAYOUT_ROW_TEMPLATE_COLUMNS);
    if (layout->row.type != NK_LAYOUT_TEMPLATE) return;
    if (layout->row.columns >= NK_MAX_LAYOUT_ROW_TEMPLATE_COLUMNS) return;
    layout->row.templates[layout->row.columns++] = -1.0f;
}
NK_API void
nk_layout_row_template_push_variable(struct nk_context *ctx, float min_width)
{
    struct nk_window *win;
    struct nk_panel *layout;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    win = ctx->current;
    layout = win->layout;
    NK_ASSERT(layout->row.type == NK_LAYOUT_TEMPLATE);
    NK_ASSERT(layout->row.columns < NK_MAX_LAYOUT_ROW_TEMPLATE_COLUMNS);
    if (layout->row.type != NK_LAYOUT_TEMPLATE) return;
    if (layout->row.columns >= NK_MAX_LAYOUT_ROW_TEMPLATE_COLUMNS) return;
    layout->row.templates[layout->row.columns++] = -min_width;
}
NK_API void
nk_layout_row_template_push_static(struct nk_context *ctx, float width)
{
    struct nk_window *win;
    struct nk_panel *layout;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    win = ctx->current;
    layout = win->layout;
    NK_ASSERT(layout->row.type == NK_LAYOUT_TEMPLATE);
    NK_ASSERT(layout->row.columns < NK_MAX_LAYOUT_ROW_TEMPLATE_COLUMNS);
    if (layout->row.type != NK_LAYOUT_TEMPLATE) return;
    if (layout->row.columns >= NK_MAX_LAYOUT_ROW_TEMPLATE_COLUMNS) return;
    layout->row.templates[layout->row.columns++] = width;
}
NK_API void
nk_layout_row_template_end(struct nk_context *ctx)
{
    struct nk_window *win;
    struct nk_panel *layout;

    int i = 0;
    int variable_count = 0;
    int min_variable_count = 0;
    float min_fixed_width = 0.0f;
    float total_fixed_width = 0.0f;
    float max_variable_width = 0.0f;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    win = ctx->current;
    layout = win->layout;
    NK_ASSERT(layout->row.type == NK_LAYOUT_TEMPLATE);
    if (layout->row.type != NK_LAYOUT_TEMPLATE) return;
    for (i = 0; i < layout->row.columns; ++i) {
        float width = layout->row.templates[i];
        if (width >= 0.0f) {
            total_fixed_width += width;
            min_fixed_width += width;
        } else if (width < -1.0f) {
            width = -width;
            total_fixed_width += width;
            max_variable_width = NK_MAX(max_variable_width, width);
            variable_count++;
        } else {
            min_variable_count++;
            variable_count++;
        }
    }
    if (variable_count) {
        float space = nk_layout_row_calculate_usable_space(&ctx->style, layout->type,
                            layout->bounds.w, layout->row.columns);
        float var_width = (NK_MAX(space-min_fixed_width,0.0f)) / (float)variable_count;
        int enough_space = var_width >= max_variable_width;
        if (!enough_space)
            var_width = (NK_MAX(space-total_fixed_width,0)) / (float)min_variable_count;
        for (i = 0; i < layout->row.columns; ++i) {
            float *width = &layout->row.templates[i];
            *width = (*width >= 0.0f)? *width: (*width < -1.0f && !enough_space)? -(*width): var_width;
        }
    }
}
NK_API void
nk_layout_space_begin(struct nk_context *ctx, enum nk_layout_format fmt,
    float height, int widget_count)
{
    struct nk_window *win;
    struct nk_panel *layout;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    win = ctx->current;
    layout = win->layout;
    nk_panel_layout(ctx, win, height, widget_count);
    if (fmt == NK_STATIC)
        layout->row.type = NK_LAYOUT_STATIC_FREE;
    else layout->row.type = NK_LAYOUT_DYNAMIC_FREE;

    layout->row.ratio = 0;
    layout->row.filled = 0;
    layout->row.item_width = 0;
    layout->row.item_offset = 0;
}
NK_API void
nk_layout_space_end(struct nk_context *ctx)
{
    struct nk_window *win;
    struct nk_panel *layout;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    win = ctx->current;
    layout = win->layout;
    layout->row.item_width = 0;
    layout->row.item_height = 0;
    layout->row.item_offset = 0;
    nk_zero(&layout->row.item, sizeof(layout->row.item));
}
NK_API void
nk_layout_space_push(struct nk_context *ctx, struct nk_rect rect)
{
    struct nk_window *win;
    struct nk_panel *layout;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    win = ctx->current;
    layout = win->layout;
    layout->row.item = rect;
}
NK_API struct nk_rect
nk_layout_space_bounds(struct nk_context *ctx)
{
    struct nk_rect ret;
    struct nk_window *win;
    struct nk_panel *layout;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    win = ctx->current;
    layout = win->layout;

    ret.x = layout->clip.x;
    ret.y = layout->clip.y;
    ret.w = layout->clip.w;
    ret.h = layout->row.height;
    return ret;
}
NK_API struct nk_rect
nk_layout_widget_bounds(struct nk_context *ctx)
{
    struct nk_rect ret;
    struct nk_window *win;
    struct nk_panel *layout;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    win = ctx->current;
    layout = win->layout;

    ret.x = layout->at_x;
    ret.y = layout->at_y;
    ret.w = layout->bounds.w - NK_MAX(layout->at_x - layout->bounds.x,0);
    ret.h = layout->row.height;
    return ret;
}
NK_API struct nk_vec2
nk_layout_space_to_screen(struct nk_context *ctx, struct nk_vec2 ret)
{
    struct nk_window *win;
    struct nk_panel *layout;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    win = ctx->current;
    layout = win->layout;

    ret.x += layout->at_x - (float)*layout->offset_x;
    ret.y += layout->at_y - (float)*layout->offset_y;
    return ret;
}
NK_API struct nk_vec2
nk_layout_space_to_local(struct nk_context *ctx, struct nk_vec2 ret)
{
    struct nk_window *win;
    struct nk_panel *layout;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    win = ctx->current;
    layout = win->layout;

    ret.x += -layout->at_x + (float)*layout->offset_x;
    ret.y += -layout->at_y + (float)*layout->offset_y;
    return ret;
}
NK_API struct nk_rect
nk_layout_space_rect_to_screen(struct nk_context *ctx, struct nk_rect ret)
{
    struct nk_window *win;
    struct nk_panel *layout;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    win = ctx->current;
    layout = win->layout;

    ret.x += layout->at_x - (float)*layout->offset_x;
    ret.y += layout->at_y - (float)*layout->offset_y;
    return ret;
}
NK_API struct nk_rect
nk_layout_space_rect_to_local(struct nk_context *ctx, struct nk_rect ret)
{
    struct nk_window *win;
    struct nk_panel *layout;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    win = ctx->current;
    layout = win->layout;

    ret.x += -layout->at_x + (float)*layout->offset_x;
    ret.y += -layout->at_y + (float)*layout->offset_y;
    return ret;
}
NK_LIB void
nk_panel_alloc_row(const struct nk_context *ctx, struct nk_window *win)
{
    struct nk_panel *layout = win->layout;
    struct nk_vec2 spacing = ctx->style.window.spacing;
    const float row_height = layout->row.height - spacing.y;
    nk_panel_layout(ctx, win, row_height, layout->row.columns);
}
NK_LIB void
nk_layout_widget_space(struct nk_rect *bounds, const struct nk_context *ctx,
    struct nk_window *win, int modify)
{
    struct nk_panel *layout;
    const struct nk_style *style;

    struct nk_vec2 spacing;

    float item_offset = 0;
    float item_width = 0;
    float item_spacing = 0;
    float panel_space = 0;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    win = ctx->current;
    layout = win->layout;
    style = &ctx->style;
    NK_ASSERT(bounds);

    spacing = style->window.spacing;
    panel_space = nk_layout_row_calculate_usable_space(&ctx->style, layout->type,
                                            layout->bounds.w, layout->row.columns);

    #define NK_FRAC(x) (x - (float)(int)nk_roundf(x)) /* will be used to remove fookin gaps */
    /* calculate the width of one item inside the current layout space */
    switch (layout->row.type) {
    case NK_LAYOUT_DYNAMIC_FIXED: {
        /* scaling fixed size widgets item width */
        float w = NK_MAX(1.0f,panel_space) / (float)layout->row.columns;
        item_offset = (float)layout->row.index * w;
        item_width = w + NK_FRAC(item_offset);
        item_spacing = (float)layout->row.index * spacing.x;
    } break;
    case NK_LAYOUT_DYNAMIC_ROW: {
        /* scaling single ratio widget width */
        float w = layout->row.item_width * panel_space;
        item_offset = layout->row.item_offset;
        item_width = w + NK_FRAC(item_offset);
        item_spacing = 0;

        if (modify) {
            layout->row.item_offset += w + spacing.x;
            layout->row.filled += layout->row.item_width;
            layout->row.index = 0;
        }
    } break;
    case NK_LAYOUT_DYNAMIC_FREE: {
        /* panel width depended free widget placing */
        bounds->x = layout->at_x + (layout->bounds.w * layout->row.item.x);
        bounds->x -= (float)*layout->offset_x;
        bounds->y = layout->at_y + (layout->row.height * layout->row.item.y);
        bounds->y -= (float)*layout->offset_y;
        bounds->w = layout->bounds.w  * layout->row.item.w + NK_FRAC(bounds->x);
        bounds->h = layout->row.height * layout->row.item.h + NK_FRAC(bounds->y);
        return;
    }
    case NK_LAYOUT_DYNAMIC: {
        /* scaling arrays of panel width ratios for every widget */
        float ratio, w;
        NK_ASSERT(layout->row.ratio);
        ratio = (layout->row.ratio[layout->row.index] < 0) ?
            layout->row.item_width : layout->row.ratio[layout->row.index];

        w = (ratio * panel_space);
        item_spacing = (float)layout->row.index * spacing.x;
        item_offset = layout->row.item_offset;
        item_width = w + NK_FRAC(item_offset);

        if (modify) {
            layout->row.item_offset += w;
            layout->row.filled += ratio;
        }
    } break;
    case NK_LAYOUT_STATIC_FIXED: {
        /* non-scaling fixed widgets item width */
        item_width = layout->row.item_width;
        item_offset = (float)layout->row.index * item_width;
        item_spacing = (float)layout->row.index * spacing.x;
    } break;
    case NK_LAYOUT_STATIC_ROW: {
        /* scaling single ratio widget width */
        item_width = layout->row.item_width;
        item_offset = layout->row.item_offset;
        item_spacing = (float)layout->row.index * spacing.x;
        if (modify) layout->row.item_offset += item_width;
    } break;
    case NK_LAYOUT_STATIC_FREE: {
        /* free widget placing */
        bounds->x = layout->at_x + layout->row.item.x;
        bounds->w = layout->row.item.w;
        if (((bounds->x + bounds->w) > layout->max_x) && modify)
            layout->max_x = (bounds->x + bounds->w);
        bounds->x -= (float)*layout->offset_x;
        bounds->y = layout->at_y + layout->row.item.y;
        bounds->y -= (float)*layout->offset_y;
        bounds->h = layout->row.item.h;
        return;
    }
    case NK_LAYOUT_STATIC: {
        /* non-scaling array of panel pixel width for every widget */
        item_spacing = (float)layout->row.index * spacing.x;
        item_width = layout->row.ratio[layout->row.index];
        item_offset = layout->row.item_offset;
        if (modify) layout->row.item_offset += item_width;
    } break;
    case NK_LAYOUT_TEMPLATE: {
        /* stretchy row layout with combined dynamic/static widget width*/
        float w;
        NK_ASSERT(layout->row.index < layout->row.columns);
        NK_ASSERT(layout->row.index < NK_MAX_LAYOUT_ROW_TEMPLATE_COLUMNS);
        w = layout->row.templates[layout->row.index];
        item_offset = layout->row.item_offset;
        item_width = w + NK_FRAC(item_offset);
        item_spacing = (float)layout->row.index * spacing.x;
        if (modify) layout->row.item_offset += w;
    } break;
    #undef NK_FRAC
    default: NK_ASSERT(0); break;
    };

    /* set the bounds of the newly allocated widget */
    bounds->w = item_width;
    bounds->h = layout->row.height - spacing.y;
    bounds->y = layout->at_y - (float)*layout->offset_y;
    bounds->x = layout->at_x + item_offset + item_spacing;
    if (((bounds->x + bounds->w) > layout->max_x) && modify)
        layout->max_x = bounds->x + bounds->w;
    bounds->x -= (float)*layout->offset_x;
}
NK_LIB void
nk_panel_alloc_space(struct nk_rect *bounds, const struct nk_context *ctx)
{
    struct nk_window *win;
    struct nk_panel *layout;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    /* check if the end of the row has been hit and begin new row if so */
    win = ctx->current;
    layout = win->layout;
    if (layout->row.index >= layout->row.columns)
        nk_panel_alloc_row(ctx, win);

    /* calculate widget position and size */
    nk_layout_widget_space(bounds, ctx, win, nk_true);
    layout->row.index++;
}
NK_LIB void
nk_layout_peek(struct nk_rect *bounds, struct nk_context *ctx)
{
    float y;
    int index;
    struct nk_window *win;
    struct nk_panel *layout;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout) {
        *bounds = nk_rect(0,0,0,0);
        return;
    }

    win = ctx->current;
    layout = win->layout;
    y = layout->at_y;
    index = layout->row.index;
    if (layout->row.index >= layout->row.columns) {
        layout->at_y += layout->row.height;
        layout->row.index = 0;
    }
    nk_layout_widget_space(bounds, ctx, win, nk_false);
    if (!layout->row.index) {
        bounds->x -= layout->row.item_offset;
    }
    layout->at_y = y;
    layout->row.index = index;
}
NK_API void
nk_spacer(struct nk_context *ctx )
{
    struct nk_rect dummy_rect = { 0, 0, 0, 0 };
    nk_panel_alloc_space( &dummy_rect, ctx );
}




/* ===============================================================
 *
 *                              TREE
 *
 * ===============================================================*/
NK_INTERN int
nk_tree_state_base(struct nk_context *ctx, enum nk_tree_type type,
    struct nk_image *img, const char *title, enum nk_collapse_states *state)
{
    struct nk_window *win;
    struct nk_panel *layout;
    const struct nk_style *style;
    struct nk_command_buffer *out;
    const struct nk_input *in;
    const struct nk_style_button *button;
    enum nk_symbol_type symbol;
    float row_height;

    struct nk_vec2 item_spacing;
    struct nk_rect header = {0,0,0,0};
    struct nk_rect sym = {0,0,0,0};
    struct nk_text text;

    nk_flags ws = 0;
    enum nk_widget_layout_states widget_state;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    /* cache some data */
    win = ctx->current;
    layout = win->layout;
    out = &win->buffer;
    style = &ctx->style;
    item_spacing = style->window.spacing;

    /* calculate header bounds and draw background */
    row_height = style->font->height + 2 * style->tab.padding.y;
    nk_layout_set_min_row_height(ctx, row_height);
    nk_layout_row_dynamic(ctx, row_height, 1);
    nk_layout_reset_min_row_height(ctx);

    widget_state = nk_widget(&header, ctx);
    if (type == NK_TREE_TAB) {
        const struct nk_style_item *background = &style->tab.background;

        switch(background->type) {
            case NK_STYLE_ITEM_IMAGE:
                nk_draw_image(out, header, &background->data.image, nk_rgb_factor(nk_white, style->tab.color_factor));
                break;
            case NK_STYLE_ITEM_NINE_SLICE:
                nk_draw_nine_slice(out, header, &background->data.slice, nk_rgb_factor(nk_white, style->tab.color_factor));
                break;
            case NK_STYLE_ITEM_COLOR:
                nk_fill_rect(out, header, 0, nk_rgb_factor(style->tab.border_color, style->tab.color_factor));
                nk_fill_rect(out, nk_shrink_rect(header, style->tab.border),
                    style->tab.rounding, nk_rgb_factor(background->data.color, style->tab.color_factor));
                break;
        }
    } else text.background = style->window.background;

    /* update node state */
    in = (!(layout->flags & NK_WINDOW_ROM)) ? &ctx->input: 0;
    in = (in && widget_state == NK_WIDGET_VALID) ? &ctx->input : 0;
    if (nk_button_behavior(&ws, header, in, NK_BUTTON_DEFAULT))
        *state = (*state == NK_MAXIMIZED) ? NK_MINIMIZED : NK_MAXIMIZED;

    /* select correct button style */
    if (*state == NK_MAXIMIZED) {
        symbol = style->tab.sym_maximize;
        if (type == NK_TREE_TAB)
            button = &style->tab.tab_maximize_button;
        else button = &style->tab.node_maximize_button;
    } else {
        symbol = style->tab.sym_minimize;
        if (type == NK_TREE_TAB)
            button = &style->tab.tab_minimize_button;
        else button = &style->tab.node_minimize_button;
    }

    {/* draw triangle button */
    sym.w = sym.h = style->font->height;
    sym.y = header.y + style->tab.padding.y;
    sym.x = header.x + style->tab.padding.x;
    nk_do_button_symbol(&ws, &win->buffer, sym, symbol, NK_BUTTON_DEFAULT,
        button, 0, style->font);

    if (img) {
        /* draw optional image icon */
        sym.x = sym.x + sym.w + 4 * item_spacing.x;
        nk_draw_image(&win->buffer, sym, img, nk_white);
        sym.w = style->font->height + style->tab.spacing.x;}
    }

    {/* draw label */
    struct nk_rect label;
    header.w = NK_MAX(header.w, sym.w + item_spacing.x);
    label.x = sym.x + sym.w + item_spacing.x;
    label.y = sym.y;
    label.w = header.w - (sym.w + item_spacing.y + style->tab.indent);
    label.h = style->font->height;
    text.text = nk_rgb_factor(style->tab.text, style->tab.color_factor);
    text.padding = nk_vec2(0,0);
    nk_widget_text(out, label, title, nk_strlen(title), &text,
        NK_TEXT_LEFT, style->font);}

    /* increase x-axis cursor widget position pointer */
    if (*state == NK_MAXIMIZED) {
        layout->at_x = header.x + (float)*layout->offset_x + style->tab.indent;
        layout->bounds.w = NK_MAX(layout->bounds.w, style->tab.indent);
        layout->bounds.w -= (style->tab.indent + style->window.padding.x);
        layout->row.tree_depth++;
        return nk_true;
    } else return nk_false;
}
NK_INTERN int
nk_tree_base(struct nk_context *ctx, enum nk_tree_type type,
    struct nk_image *img, const char *title, enum nk_collapse_states initial_state,
    const char *hash, int len, int line)
{
    struct nk_window *win = ctx->current;
    int title_len = 0;
    nk_hash tree_hash = 0;
    nk_uint *state = 0;

    /* retrieve tree state from internal widget state tables */
    if (!hash) {
        title_len = (int)nk_strlen(title);
        tree_hash = nk_murmur_hash(title, (int)title_len, (nk_hash)line);
    } else tree_hash = nk_murmur_hash(hash, len, (nk_hash)line);
    state = nk_find_value(win, tree_hash);
    if (!state) {
        state = nk_add_value(ctx, win, tree_hash, 0);
        *state = initial_state;
    }
    if(initial_state == NK_FORCE_MINIMIZED) *state = NK_MINIMIZED;
    if(initial_state == NK_FORCE_MAXIMIZED) *state = NK_MAXIMIZED;
    return nk_tree_state_base(ctx, type, img, title, (enum nk_collapse_states*)state);
}
NK_API nk_bool
nk_tree_state_push(struct nk_context *ctx, enum nk_tree_type type,
    const char *title, enum nk_collapse_states *state)
{
    return nk_tree_state_base(ctx, type, 0, title, state);
}
NK_API nk_bool
nk_tree_state_image_push(struct nk_context *ctx, enum nk_tree_type type,
    struct nk_image img, const char *title, enum nk_collapse_states *state)
{
    return nk_tree_state_base(ctx, type, &img, title, state);
}
NK_API void
nk_tree_state_pop(struct nk_context *ctx)
{
    struct nk_window *win = 0;
    struct nk_panel *layout = 0;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    win = ctx->current;
    layout = win->layout;
    layout->at_x -= ctx->style.tab.indent + (float)*layout->offset_x;
    layout->bounds.w += ctx->style.tab.indent + ctx->style.window.padding.x;
    NK_ASSERT(layout->row.tree_depth);
    layout->row.tree_depth--;
}
NK_API nk_bool
nk_tree_push_hashed(struct nk_context *ctx, enum nk_tree_type type,
    const char *title, enum nk_collapse_states initial_state,
    const char *hash, int len, int line)
{
    return nk_tree_base(ctx, type, 0, title, initial_state, hash, len, line);
}
NK_API nk_bool
nk_tree_image_push_hashed(struct nk_context *ctx, enum nk_tree_type type,
    struct nk_image img, const char *title, enum nk_collapse_states initial_state,
    const char *hash, int len,int seed)
{
    return nk_tree_base(ctx, type, &img, title, initial_state, hash, len, seed);
}
NK_API void
nk_tree_pop(struct nk_context *ctx)
{
    nk_tree_state_pop(ctx);
}
NK_INTERN int
nk_tree_element_image_push_hashed_base(struct nk_context *ctx, enum nk_tree_type type,
    struct nk_image *img, const char *title, int title_len,
    enum nk_collapse_states *state, nk_bool *selected)
{
    struct nk_window *win;
    struct nk_panel *layout;
    const struct nk_style *style;
    struct nk_command_buffer *out;
    const struct nk_input *in;
    const struct nk_style_button *button;
    enum nk_symbol_type symbol;
    float row_height;
    struct nk_vec2 padding;

    int text_len;
    float text_width;

    struct nk_vec2 item_spacing;
    struct nk_rect header = {0,0,0,0};
    struct nk_rect sym = {0,0,0,0};

    nk_flags ws = 0;
    enum nk_widget_layout_states widget_state;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    /* cache some data */
    win = ctx->current;
    layout = win->layout;
    out = &win->buffer;
    style = &ctx->style;
    item_spacing = style->window.spacing;
    padding = style->selectable.padding;

    /* calculate header bounds and draw background */
    row_height = style->font->height + 2 * style->tab.padding.y;
    nk_layout_set_min_row_height(ctx, row_height);
    nk_layout_row_dynamic(ctx, row_height, 1);
    nk_layout_reset_min_row_height(ctx);

    widget_state = nk_widget(&header, ctx);
    if (type == NK_TREE_TAB) {
        const struct nk_style_item *background = &style->tab.background;

        switch (background->type) {
            case NK_STYLE_ITEM_IMAGE:
                nk_draw_image(out, header, &background->data.image, nk_rgb_factor(nk_white, style->tab.color_factor));
                break;
            case NK_STYLE_ITEM_NINE_SLICE:
                nk_draw_nine_slice(out, header, &background->data.slice, nk_rgb_factor(nk_white, style->tab.color_factor));
                break;
            case NK_STYLE_ITEM_COLOR:
                nk_fill_rect(out, header, 0, nk_rgb_factor(style->tab.border_color, style->tab.color_factor));
                nk_fill_rect(out, nk_shrink_rect(header, style->tab.border),
                    style->tab.rounding, nk_rgb_factor(background->data.color, style->tab.color_factor));

                break;
        }
    }

    in = (!(layout->flags & NK_WINDOW_ROM)) ? &ctx->input: 0;
    in = (in && widget_state == NK_WIDGET_VALID) ? &ctx->input : 0;

    /* select correct button style */
    if (*state == NK_MAXIMIZED) {
        symbol = style->tab.sym_maximize;
        if (type == NK_TREE_TAB)
            button = &style->tab.tab_maximize_button;
        else button = &style->tab.node_maximize_button;
    } else {
        symbol = style->tab.sym_minimize;
        if (type == NK_TREE_TAB)
            button = &style->tab.tab_minimize_button;
        else button = &style->tab.node_minimize_button;
    }
    {/* draw triangle button */
    sym.w = sym.h = style->font->height;
    sym.y = header.y + style->tab.padding.y;
    sym.x = header.x + style->tab.padding.x;
    if (nk_do_button_symbol(&ws, &win->buffer, sym, symbol, NK_BUTTON_DEFAULT, button, in, style->font))
        *state = (*state == NK_MAXIMIZED) ? NK_MINIMIZED : NK_MAXIMIZED;}

    /* draw label */
    {nk_flags dummy = 0;
    struct nk_rect label;
    /* calculate size of the text and tooltip */
    text_len = nk_strlen(title);
    text_width = style->font->width(style->font->userdata, style->font->height, title, text_len);
    text_width += (4 * padding.x);

    header.w = NK_MAX(header.w, sym.w + item_spacing.x);
    label.x = sym.x + sym.w + item_spacing.x;
    label.y = sym.y;
    label.w = NK_MIN(header.w - (sym.w + item_spacing.y + style->tab.indent), text_width);
    label.h = style->font->height;

    if (img) {
        nk_do_selectable_image(&dummy, &win->buffer, label, title, title_len, NK_TEXT_LEFT,
            selected, img, &style->selectable, in, style->font);
    } else nk_do_selectable(&dummy, &win->buffer, label, title, title_len, NK_TEXT_LEFT,
            selected, &style->selectable, in, style->font);
    }
    /* increase x-axis cursor widget position pointer */
    if (*state == NK_MAXIMIZED) {
        layout->at_x = header.x + (float)*layout->offset_x + style->tab.indent;
        layout->bounds.w = NK_MAX(layout->bounds.w, style->tab.indent);
        layout->bounds.w -= (style->tab.indent + style->window.padding.x);
        layout->row.tree_depth++;
        return nk_true;
    } else return nk_false;
}
NK_INTERN int
nk_tree_element_base(struct nk_context *ctx, enum nk_tree_type type,
    struct nk_image *img, const char *title, enum nk_collapse_states initial_state,
    nk_bool *selected, const char *hash, int len, int line)
{
    struct nk_window *win = ctx->current;
    int title_len = 0;
    nk_hash tree_hash = 0;
    nk_uint *state = 0;

    /* retrieve tree state from internal widget state tables */
    if (!hash) {
        title_len = (int)nk_strlen(title);
        tree_hash = nk_murmur_hash(title, (int)title_len, (nk_hash)line);
    } else tree_hash = nk_murmur_hash(hash, len, (nk_hash)line);
    state = nk_find_value(win, tree_hash);
    if (!state) {
        state = nk_add_value(ctx, win, tree_hash, 0);
        *state = initial_state;
    } return nk_tree_element_image_push_hashed_base(ctx, type, img, title,
        nk_strlen(title), (enum nk_collapse_states*)state, selected);
}
NK_API nk_bool
nk_tree_element_push_hashed(struct nk_context *ctx, enum nk_tree_type type,
    const char *title, enum nk_collapse_states initial_state,
    nk_bool *selected, const char *hash, int len, int seed)
{
    return nk_tree_element_base(ctx, type, 0, title, initial_state, selected, hash, len, seed);
}
NK_API nk_bool
nk_tree_element_image_push_hashed(struct nk_context *ctx, enum nk_tree_type type,
    struct nk_image img, const char *title, enum nk_collapse_states initial_state,
    nk_bool *selected, const char *hash, int len,int seed)
{
    return nk_tree_element_base(ctx, type, &img, title, initial_state, selected, hash, len, seed);
}
NK_API void
nk_tree_element_pop(struct nk_context *ctx)
{
    nk_tree_state_pop(ctx);
}





/* ===============================================================
 *
 *                          GROUP
 *
 * ===============================================================*/
NK_API nk_bool
nk_group_scrolled_offset_begin(struct nk_context *ctx,
    nk_uint *x_offset, nk_uint *y_offset, const char *title, nk_flags flags)
{
    struct nk_rect bounds;
    struct nk_window panel;
    struct nk_window *win;

    win = ctx->current;
    nk_panel_alloc_space(&bounds, ctx);
    {const struct nk_rect *c = &win->layout->clip;
    if (!NK_INTERSECT(c->x, c->y, c->w, c->h, bounds.x, bounds.y, bounds.w, bounds.h) &&
        !(flags & NK_WINDOW_MOVABLE)) {
        return 0;
    }}
    if (win->flags & NK_WINDOW_ROM)
        flags |= NK_WINDOW_ROM;

    /* initialize a fake window to create the panel from */
    nk_zero(&panel, sizeof(panel));
    panel.bounds = bounds;
    panel.flags = flags;
    panel.scrollbar.x = *x_offset;
    panel.scrollbar.y = *y_offset;
    panel.buffer = win->buffer;
    panel.layout = (struct nk_panel*)nk_create_panel(ctx);
    ctx->current = &panel;
    nk_panel_begin(ctx, (flags & NK_WINDOW_TITLE) ? title: 0, NK_PANEL_GROUP);

    win->buffer = panel.buffer;
    win->buffer.clip = panel.layout->clip;
    panel.layout->offset_x = x_offset;
    panel.layout->offset_y = y_offset;
    panel.layout->parent = win->layout;
    win->layout = panel.layout;

    ctx->current = win;
    if ((panel.layout->flags & NK_WINDOW_CLOSED) ||
        (panel.layout->flags & NK_WINDOW_MINIMIZED))
    {
        nk_flags f = panel.layout->flags;
        nk_group_scrolled_end(ctx);
        if (f & NK_WINDOW_CLOSED)
            return NK_WINDOW_CLOSED;
        if (f & NK_WINDOW_MINIMIZED)
            return NK_WINDOW_MINIMIZED;
    }
    return 1;
}
NK_API void
nk_group_scrolled_end(struct nk_context *ctx)
{
    struct nk_window *win;
    struct nk_panel *parent;
    struct nk_panel *g;

    struct nk_rect clip;
    struct nk_window pan;
    struct nk_vec2 panel_padding;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    if (!ctx || !ctx->current)
        return;

    /* make sure nk_group_begin was called correctly */
    NK_ASSERT(ctx->current);
    win = ctx->current;
    NK_ASSERT(win->layout);
    g = win->layout;
    NK_ASSERT(g->parent);
    parent = g->parent;

    /* dummy window */
    nk_zero_struct(pan);
    panel_padding = nk_panel_get_padding(&ctx->style, NK_PANEL_GROUP);
    pan.bounds.y = g->bounds.y - (g->header_height + g->menu.h);
    pan.bounds.x = g->bounds.x - panel_padding.x;
    pan.bounds.w = g->bounds.w + 2 * panel_padding.x;
    pan.bounds.h = g->bounds.h + g->header_height + g->menu.h;
    if (g->flags & NK_WINDOW_BORDER) {
        pan.bounds.x -= g->border;
        pan.bounds.y -= g->border;
        pan.bounds.w += 2*g->border;
        pan.bounds.h += 2*g->border;
    }
    if (!(g->flags & NK_WINDOW_NO_SCROLLBAR)) {
        pan.bounds.w += ctx->style.window.scrollbar_size.x;
        pan.bounds.h += ctx->style.window.scrollbar_size.y;
    }
    pan.scrollbar.x = *g->offset_x;
    pan.scrollbar.y = *g->offset_y;
    pan.flags = g->flags;
    pan.buffer = win->buffer;
    pan.layout = g;
    pan.parent = win;
    ctx->current = &pan;

    /* make sure group has correct clipping rectangle */
    nk_unify(&clip, &parent->clip, pan.bounds.x, pan.bounds.y,
        pan.bounds.x + pan.bounds.w, pan.bounds.y + pan.bounds.h + panel_padding.x);
    nk_push_scissor(&pan.buffer, clip);
    nk_end(ctx);

    win->buffer = pan.buffer;
    nk_push_scissor(&win->buffer, parent->clip);
    ctx->current = win;
    win->layout = parent;
    g->bounds = pan.bounds;
    return;
}
NK_API nk_bool
nk_group_scrolled_begin(struct nk_context *ctx,
    struct nk_scroll *scroll, const char *title, nk_flags flags)
{
    return nk_group_scrolled_offset_begin(ctx, &scroll->x, &scroll->y, title, flags);
}
NK_API nk_bool
nk_group_begin_titled(struct nk_context *ctx, const char *id,
    const char *title, nk_flags flags)
{
    int id_len;
    nk_hash id_hash;
    struct nk_window *win;
    nk_uint *x_offset;
    nk_uint *y_offset;

    NK_ASSERT(ctx);
    NK_ASSERT(id);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout || !id)
        return 0;

    /* find persistent group scrollbar value */
    win = ctx->current;
    id_len = (int)nk_strlen(id);
    id_hash = nk_murmur_hash(id, (int)id_len, NK_PANEL_GROUP);
    x_offset = nk_find_value(win, id_hash);
    if (!x_offset) {
        x_offset = nk_add_value(ctx, win, id_hash, 0);
        y_offset = nk_add_value(ctx, win, id_hash+1, 0);

        NK_ASSERT(x_offset);
        NK_ASSERT(y_offset);
        if (!x_offset || !y_offset) return 0;
        *x_offset = *y_offset = 0;
    } else y_offset = nk_find_value(win, id_hash+1);
    return nk_group_scrolled_offset_begin(ctx, x_offset, y_offset, title, flags);
}
NK_API nk_bool
nk_group_begin(struct nk_context *ctx, const char *title, nk_flags flags)
{
    return nk_group_begin_titled(ctx, title, title, flags);
}
NK_API void
nk_group_end(struct nk_context *ctx)
{
    nk_group_scrolled_end(ctx);
}
NK_API void
nk_group_get_scroll(struct nk_context *ctx, const char *id, nk_uint *x_offset, nk_uint *y_offset)
{
    int id_len;
    nk_hash id_hash;
    struct nk_window *win;
    nk_uint *x_offset_ptr;
    nk_uint *y_offset_ptr;

    NK_ASSERT(ctx);
    NK_ASSERT(id);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout || !id)
        return;

    /* find persistent group scrollbar value */
    win = ctx->current;
    id_len = (int)nk_strlen(id);
    id_hash = nk_murmur_hash(id, (int)id_len, NK_PANEL_GROUP);
    x_offset_ptr = nk_find_value(win, id_hash);
    if (!x_offset_ptr) {
        x_offset_ptr = nk_add_value(ctx, win, id_hash, 0);
        y_offset_ptr = nk_add_value(ctx, win, id_hash+1, 0);

        NK_ASSERT(x_offset_ptr);
        NK_ASSERT(y_offset_ptr);
        if (!x_offset_ptr || !y_offset_ptr) return;
        *x_offset_ptr = *y_offset_ptr = 0;
    } else y_offset_ptr = nk_find_value(win, id_hash+1);
    if (x_offset)
      *x_offset = *x_offset_ptr;
    if (y_offset)
      *y_offset = *y_offset_ptr;
}
NK_API void
nk_group_set_scroll(struct nk_context *ctx, const char *id, nk_uint x_offset, nk_uint y_offset)
{
    int id_len;
    nk_hash id_hash;
    struct nk_window *win;
    nk_uint *x_offset_ptr;
    nk_uint *y_offset_ptr;

    NK_ASSERT(ctx);
    NK_ASSERT(id);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout || !id)
        return;

    /* find persistent group scrollbar value */
    win = ctx->current;
    id_len = (int)nk_strlen(id);
    id_hash = nk_murmur_hash(id, (int)id_len, NK_PANEL_GROUP);
    x_offset_ptr = nk_find_value(win, id_hash);
    if (!x_offset_ptr) {
        x_offset_ptr = nk_add_value(ctx, win, id_hash, 0);
        y_offset_ptr = nk_add_value(ctx, win, id_hash+1, 0);

        NK_ASSERT(x_offset_ptr);
        NK_ASSERT(y_offset_ptr);
        if (!x_offset_ptr || !y_offset_ptr) return;
        *x_offset_ptr = *y_offset_ptr = 0;
    } else y_offset_ptr = nk_find_value(win, id_hash+1);
    *x_offset_ptr = x_offset;
    *y_offset_ptr = y_offset;
}




/* ===============================================================
 *
 *                          LIST VIEW
 *
 * ===============================================================*/
NK_API nk_bool
nk_list_view_begin(struct nk_context *ctx, struct nk_list_view *view,
    const char *title, nk_flags flags, int row_height, int row_count)
{
    int title_len;
    nk_hash title_hash;
    nk_uint *x_offset;
    nk_uint *y_offset;

    int result;
    struct nk_window *win;
    struct nk_panel *layout;
    const struct nk_style *style;
    struct nk_vec2 item_spacing;

    NK_ASSERT(ctx);
    NK_ASSERT(view);
    NK_ASSERT(title);
    if (!ctx || !view || !title) return 0;

    win = ctx->current;
    style = &ctx->style;
    item_spacing = style->window.spacing;
    row_height += NK_MAX(0, (int)item_spacing.y);

    /* find persistent list view scrollbar offset */
    title_len = (int)nk_strlen(title);
    title_hash = nk_murmur_hash(title, (int)title_len, NK_PANEL_GROUP);
    x_offset = nk_find_value(win, title_hash);
    if (!x_offset) {
        x_offset = nk_add_value(ctx, win, title_hash, 0);
        y_offset = nk_add_value(ctx, win, title_hash+1, 0);

        NK_ASSERT(x_offset);
        NK_ASSERT(y_offset);
        if (!x_offset || !y_offset) return 0;
        *x_offset = *y_offset = 0;
    } else y_offset = nk_find_value(win, title_hash+1);
    view->scroll_value = *y_offset;
    view->scroll_pointer = y_offset;

    *y_offset = 0;
    result = nk_group_scrolled_offset_begin(ctx, x_offset, y_offset, title, flags);
    win = ctx->current;
    layout = win->layout;

    view->total_height = row_height * NK_MAX(row_count,1);
    view->begin = (int)NK_MAX(((float)view->scroll_value / (float)row_height), 0.0f);
    view->count = (int)NK_MAX(nk_iceilf((layout->clip.h)/(float)row_height),0);
    view->count = NK_MIN(view->count, row_count - view->begin);
    view->end = view->begin + view->count;
    view->ctx = ctx;
    return result;
}
NK_API void
nk_list_view_end(struct nk_list_view *view)
{
    struct nk_context *ctx;
    struct nk_window *win;
    struct nk_panel *layout;

    NK_ASSERT(view);
    NK_ASSERT(view->ctx);
    NK_ASSERT(view->scroll_pointer);
    if (!view || !view->ctx) return;

    ctx = view->ctx;
    win = ctx->current;
    layout = win->layout;
    layout->at_y = layout->bounds.y + (float)view->total_height;
    *view->scroll_pointer = *view->scroll_pointer + view->scroll_value;
    nk_group_end(view->ctx);
}





/* ===============================================================
 *
 *                              WIDGET
 *
 * ===============================================================*/
NK_API struct nk_rect
nk_widget_bounds(struct nk_context *ctx)
{
    struct nk_rect bounds;
    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    if (!ctx || !ctx->current)
        return nk_rect(0,0,0,0);
    nk_layout_peek(&bounds, ctx);
    return bounds;
}
NK_API struct nk_vec2
nk_widget_position(struct nk_context *ctx)
{
    struct nk_rect bounds;
    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    if (!ctx || !ctx->current)
        return nk_vec2(0,0);

    nk_layout_peek(&bounds, ctx);
    return nk_vec2(bounds.x, bounds.y);
}
NK_API struct nk_vec2
nk_widget_size(struct nk_context *ctx)
{
    struct nk_rect bounds;
    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    if (!ctx || !ctx->current)
        return nk_vec2(0,0);

    nk_layout_peek(&bounds, ctx);
    return nk_vec2(bounds.w, bounds.h);
}
NK_API float
nk_widget_width(struct nk_context *ctx)
{
    struct nk_rect bounds;
    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    if (!ctx || !ctx->current)
        return 0;

    nk_layout_peek(&bounds, ctx);
    return bounds.w;
}
NK_API float
nk_widget_height(struct nk_context *ctx)
{
    struct nk_rect bounds;
    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    if (!ctx || !ctx->current)
        return 0;

    nk_layout_peek(&bounds, ctx);
    return bounds.h;
}
NK_API nk_bool
nk_widget_is_hovered(struct nk_context *ctx)
{
    struct nk_rect c, v;
    struct nk_rect bounds;
    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    if (!ctx || !ctx->current || ctx->active != ctx->current)
        return 0;

    c = ctx->current->layout->clip;
    c.x = (float)((int)c.x);
    c.y = (float)((int)c.y);
    c.w = (float)((int)c.w);
    c.h = (float)((int)c.h);

    nk_layout_peek(&bounds, ctx);
    nk_unify(&v, &c, bounds.x, bounds.y, bounds.x + bounds.w, bounds.y + bounds.h);
    if (!NK_INTERSECT(c.x, c.y, c.w, c.h, bounds.x, bounds.y, bounds.w, bounds.h))
        return 0;
    return nk_input_is_mouse_hovering_rect(&ctx->input, bounds);
}
NK_API nk_bool
nk_widget_is_mouse_clicked(struct nk_context *ctx, enum nk_buttons btn)
{
    struct nk_rect c, v;
    struct nk_rect bounds;
    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    if (!ctx || !ctx->current || ctx->active != ctx->current)
        return 0;

    c = ctx->current->layout->clip;
    c.x = (float)((int)c.x);
    c.y = (float)((int)c.y);
    c.w = (float)((int)c.w);
    c.h = (float)((int)c.h);

    nk_layout_peek(&bounds, ctx);
    nk_unify(&v, &c, bounds.x, bounds.y, bounds.x + bounds.w, bounds.y + bounds.h);
    if (!NK_INTERSECT(c.x, c.y, c.w, c.h, bounds.x, bounds.y, bounds.w, bounds.h))
        return 0;
    return nk_input_mouse_clicked(&ctx->input, btn, bounds);
}
NK_API nk_bool
nk_widget_has_mouse_click_down(struct nk_context *ctx, enum nk_buttons btn, nk_bool down)
{
    struct nk_rect c, v;
    struct nk_rect bounds;
    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    if (!ctx || !ctx->current || ctx->active != ctx->current)
        return 0;

    c = ctx->current->layout->clip;
    c.x = (float)((int)c.x);
    c.y = (float)((int)c.y);
    c.w = (float)((int)c.w);
    c.h = (float)((int)c.h);

    nk_layout_peek(&bounds, ctx);
    nk_unify(&v, &c, bounds.x, bounds.y, bounds.x + bounds.w, bounds.y + bounds.h);
    if (!NK_INTERSECT(c.x, c.y, c.w, c.h, bounds.x, bounds.y, bounds.w, bounds.h))
        return 0;
    return nk_input_has_mouse_click_down_in_rect(&ctx->input, btn, bounds, down);
}
NK_API enum nk_widget_layout_states
nk_widget(struct nk_rect *bounds, const struct nk_context *ctx)
{
    struct nk_rect c, v;
    struct nk_window *win;
    struct nk_panel *layout;
    const struct nk_input *in;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return NK_WIDGET_INVALID;

    /* allocate space and check if the widget needs to be updated and drawn */
    nk_panel_alloc_space(bounds, ctx);
    win = ctx->current;
    layout = win->layout;
    in = &ctx->input;
    c = layout->clip;

    /*  if one of these triggers you forgot to add an `if` condition around either
        a window, group, popup, combobox or contextual menu `begin` and `end` block.
        Example:
            if (nk_begin(...) {...} nk_end(...); or
            if (nk_group_begin(...) { nk_group_end(...);} */
    NK_ASSERT(!(layout->flags & NK_WINDOW_MINIMIZED));
    NK_ASSERT(!(layout->flags & NK_WINDOW_HIDDEN));
    NK_ASSERT(!(layout->flags & NK_WINDOW_CLOSED));

    /* need to convert to int here to remove floating point errors */
    bounds->x = (float)((int)bounds->x);
    bounds->y = (float)((int)bounds->y);
    bounds->w = (float)((int)bounds->w);
    bounds->h = (float)((int)bounds->h);

    c.x = (float)((int)c.x);
    c.y = (float)((int)c.y);
    c.w = (float)((int)c.w);
    c.h = (float)((int)c.h);

    nk_unify(&v, &c, bounds->x, bounds->y, bounds->x + bounds->w, bounds->y + bounds->h);
    if (!NK_INTERSECT(c.x, c.y, c.w, c.h, bounds->x, bounds->y, bounds->w, bounds->h))
        return NK_WIDGET_INVALID;
    if (win->widgets_disabled)
        return NK_WIDGET_DISABLED;
    if (!NK_INBOX(in->mouse.pos.x, in->mouse.pos.y, v.x, v.y, v.w, v.h))
        return NK_WIDGET_ROM;
    return NK_WIDGET_VALID;
}
NK_API enum nk_widget_layout_states
nk_widget_fitting(struct nk_rect *bounds, struct nk_context *ctx,
    struct nk_vec2 item_padding)
{
    /* update the bounds to stand without padding  */
    enum nk_widget_layout_states state;
    NK_UNUSED(item_padding);

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return NK_WIDGET_INVALID;

    state = nk_widget(bounds, ctx);
    return state;
}
NK_API void
nk_spacing(struct nk_context *ctx, int cols)
{
    struct nk_window *win;
    struct nk_panel *layout;
    struct nk_rect none;
    int i, index, rows;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    /* spacing over row boundaries */
    win = ctx->current;
    layout = win->layout;
    index = (layout->row.index + cols) % layout->row.columns;
    rows = (layout->row.index + cols) / layout->row.columns;
    if (rows) {
        for (i = 0; i < rows; ++i)
            nk_panel_alloc_row(ctx, win);
        cols = index;
    }
    /* non table layout need to allocate space */
    if (layout->row.type != NK_LAYOUT_DYNAMIC_FIXED &&
        layout->row.type != NK_LAYOUT_STATIC_FIXED) {
        for (i = 0; i < cols; ++i)
            nk_panel_alloc_space(&none, ctx);
    } layout->row.index = index;
}
NK_API void
nk_widget_disable_begin(struct nk_context* ctx)
{
    struct nk_window* win;
    struct nk_style* style;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);

    if (!ctx || !ctx->current)
        return;

    win = ctx->current;
    style = &ctx->style;

    win->widgets_disabled = nk_true;

    style->button.color_factor_text = style->button.disabled_factor;
    style->button.color_factor_background = style->button.disabled_factor;
    style->chart.color_factor = style->chart.disabled_factor;
    style->checkbox.color_factor = style->checkbox.disabled_factor;
    style->combo.color_factor = style->combo.disabled_factor;
    style->combo.button.color_factor_text = style->combo.button.disabled_factor;
    style->combo.button.color_factor_background = style->combo.button.disabled_factor;
    style->contextual_button.color_factor_text = style->contextual_button.disabled_factor;
    style->contextual_button.color_factor_background = style->contextual_button.disabled_factor;
    style->edit.color_factor = style->edit.disabled_factor;
    style->edit.scrollbar.color_factor = style->edit.scrollbar.disabled_factor;
    style->menu_button.color_factor_text = style->menu_button.disabled_factor;
    style->menu_button.color_factor_background = style->menu_button.disabled_factor;
    style->option.color_factor = style->option.disabled_factor;
    style->progress.color_factor = style->progress.disabled_factor;
    style->property.color_factor = style->property.disabled_factor;
    style->property.inc_button.color_factor_text = style->property.inc_button.disabled_factor;
    style->property.inc_button.color_factor_background = style->property.inc_button.disabled_factor;
    style->property.dec_button.color_factor_text = style->property.dec_button.disabled_factor;
    style->property.dec_button.color_factor_background = style->property.dec_button.disabled_factor;
    style->property.edit.color_factor = style->property.edit.disabled_factor;
    style->scrollh.color_factor = style->scrollh.disabled_factor;
    style->scrollh.inc_button.color_factor_text = style->scrollh.inc_button.disabled_factor;
    style->scrollh.inc_button.color_factor_background = style->scrollh.inc_button.disabled_factor;
    style->scrollh.dec_button.color_factor_text = style->scrollh.dec_button.disabled_factor;
    style->scrollh.dec_button.color_factor_background = style->scrollh.dec_button.disabled_factor;
    style->scrollv.color_factor = style->scrollv.disabled_factor;
    style->scrollv.inc_button.color_factor_text = style->scrollv.inc_button.disabled_factor;
    style->scrollv.inc_button.color_factor_background = style->scrollv.inc_button.disabled_factor;
    style->scrollv.dec_button.color_factor_text = style->scrollv.dec_button.disabled_factor;
    style->scrollv.dec_button.color_factor_background = style->scrollv.dec_button.disabled_factor;
    style->selectable.color_factor = style->selectable.disabled_factor;
    style->slider.color_factor = style->slider.disabled_factor;
    style->slider.inc_button.color_factor_text = style->slider.inc_button.disabled_factor;
    style->slider.inc_button.color_factor_background = style->slider.inc_button.disabled_factor;
    style->slider.dec_button.color_factor_text = style->slider.dec_button.disabled_factor;
    style->slider.dec_button.color_factor_background = style->slider.dec_button.disabled_factor;
    style->tab.color_factor = style->tab.disabled_factor;
    style->tab.node_maximize_button.color_factor_text = style->tab.node_maximize_button.disabled_factor;
    style->tab.node_minimize_button.color_factor_text = style->tab.node_minimize_button.disabled_factor;
    style->tab.tab_maximize_button.color_factor_text = style->tab.tab_maximize_button.disabled_factor;
    style->tab.tab_maximize_button.color_factor_background = style->tab.tab_maximize_button.disabled_factor;
    style->tab.tab_minimize_button.color_factor_text = style->tab.tab_minimize_button.disabled_factor;
    style->tab.tab_minimize_button.color_factor_background = style->tab.tab_minimize_button.disabled_factor;
    style->text.color_factor = style->text.disabled_factor;
}
NK_API void
nk_widget_disable_end(struct nk_context* ctx)
{
    struct nk_window* win;
    struct nk_style* style;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);

    if (!ctx || !ctx->current)
        return;

    win = ctx->current;
    style = &ctx->style;

    win->widgets_disabled = nk_false;

    style->button.color_factor_text = 1.0f;
    style->button.color_factor_background = 1.0f;
    style->chart.color_factor = 1.0f;
    style->checkbox.color_factor = 1.0f;
    style->combo.color_factor = 1.0f;
    style->combo.button.color_factor_text = 1.0f;
    style->combo.button.color_factor_background = 1.0f;
    style->contextual_button.color_factor_text = 1.0f;
    style->contextual_button.color_factor_background = 1.0f;
    style->edit.color_factor = 1.0f;
    style->edit.scrollbar.color_factor = 1.0f;
    style->menu_button.color_factor_text = 1.0f;
    style->menu_button.color_factor_background = 1.0f;
    style->option.color_factor = 1.0f;
    style->progress.color_factor = 1.0f;
    style->property.color_factor = 1.0f;
    style->property.inc_button.color_factor_text = 1.0f;
    style->property.inc_button.color_factor_background = 1.0f;
    style->property.dec_button.color_factor_text = 1.0f;
    style->property.dec_button.color_factor_background = 1.0f;
    style->property.edit.color_factor = 1.0f;
    style->scrollh.color_factor = 1.0f;
    style->scrollh.inc_button.color_factor_text = 1.0f;
    style->scrollh.inc_button.color_factor_background = 1.0f;
    style->scrollh.dec_button.color_factor_text = 1.0f;
    style->scrollh.dec_button.color_factor_background = 1.0f;
    style->scrollv.color_factor = 1.0f;
    style->scrollv.inc_button.color_factor_text = 1.0f;
    style->scrollv.inc_button.color_factor_background = 1.0f;
    style->scrollv.dec_button.color_factor_text = 1.0f;
    style->scrollv.dec_button.color_factor_background = 1.0f;
    style->selectable.color_factor = 1.0f;
    style->slider.color_factor = 1.0f;
    style->slider.inc_button.color_factor_text = 1.0f;
    style->slider.inc_button.color_factor_background = 1.0f;
    style->slider.dec_button.color_factor_text = 1.0f;
    style->slider.dec_button.color_factor_background = 1.0f;
    style->tab.color_factor = 1.0f;
    style->tab.node_maximize_button.color_factor_text = 1.0f;
    style->tab.node_minimize_button.color_factor_text = 1.0f;
    style->tab.tab_maximize_button.color_factor_text = 1.0f;
    style->tab.tab_maximize_button.color_factor_background = 1.0f;
    style->tab.tab_minimize_button.color_factor_text = 1.0f;
    style->tab.tab_minimize_button.color_factor_background = 1.0f;
    style->text.color_factor = 1.0f;
}




/* ===============================================================
 *
 *                              TEXT
 *
 * ===============================================================*/
NK_LIB void
nk_widget_text(struct nk_command_buffer *o, struct nk_rect b,
    const char *string, int len, const struct nk_text *t,
    nk_flags a, const struct nk_user_font *f)
{
    struct nk_rect label;
    float text_width;

    NK_ASSERT(o);
    NK_ASSERT(t);
    if (!o || !t) return;

    b.h = NK_MAX(b.h, 2 * t->padding.y);
    label.x = 0; label.w = 0;
    label.y = b.y + t->padding.y;
    label.h = NK_MIN(f->height, b.h - 2 * t->padding.y);

    text_width = f->width(f->userdata, f->height, (const char*)string, len);
    text_width += (2.0f * t->padding.x);

    /* align in x-axis */
    if (a & NK_TEXT_ALIGN_LEFT) {
        label.x = b.x + t->padding.x;
        label.w = NK_MAX(0, b.w - 2 * t->padding.x);
    } else if (a & NK_TEXT_ALIGN_CENTERED) {
        label.w = NK_MAX(1, 2 * t->padding.x + (float)text_width);
        label.x = (b.x + t->padding.x + ((b.w - 2 * t->padding.x) - label.w) / 2);
        label.x = NK_MAX(b.x + t->padding.x, label.x);
        label.w = NK_MIN(b.x + b.w, label.x + label.w);
        if (label.w >= label.x) label.w -= label.x;
    } else if (a & NK_TEXT_ALIGN_RIGHT) {
        label.x = NK_MAX(b.x + t->padding.x, (b.x + b.w) - (2 * t->padding.x + (float)text_width));
        label.w = (float)text_width + 2 * t->padding.x;
    } else return;

    /* align in y-axis */
    if (a & NK_TEXT_ALIGN_MIDDLE) { /* add 0.5 for rounding and 1.0 to err on the high side (optical middle) */
        label.y = (int)(b.y + b.h/2.0f - (float)(f->height+3)/2.0f);
        label.h = NK_MAX(b.h/2.0f, b.h - (b.h/2.0f + (f->height+1)/2.0f));
    } else if (a & NK_TEXT_ALIGN_BOTTOM) {
        label.y = b.y + b.h - f->height;
        label.h = f->height;
    }
    nk_draw_text(o, label, (const char*)string, len, f, t->background, t->text);
}
NK_LIB void
nk_widget_text_wrap(struct nk_command_buffer *o, struct nk_rect b,
    const char *string, int len, const struct nk_text *t,
    const struct nk_user_font *f)
{
    float width;
    int glyphs = 0;
    int fitting = 0;
    int done = 0;
    struct nk_rect line;
    struct nk_text text;
    NK_INTERN nk_rune seperator[] = {' ', '\n'};

    NK_ASSERT(o);
    NK_ASSERT(t);
    if (!o || !t) return;

    text.padding = nk_vec2(0,0);
    text.background = t->background;
    text.text = t->text;

    b.w = NK_MAX(b.w, 2 * t->padding.x);
    b.h = NK_MAX(b.h, 2 * t->padding.y);
    b.h = b.h - 2 * t->padding.y;

    line.x = b.x + t->padding.x;
    line.y = b.y + t->padding.y;
    line.w = b.w - 2 * t->padding.x;
    line.h = 2 * t->padding.y + f->height;

    fitting = nk_text_clamp(f, string, len, line.w, &glyphs, &width, seperator,NK_LEN(seperator));
    while (done < len) {
        if (!fitting || line.y + line.h >= (b.y + b.h)) break;
        nk_widget_text(o, line, &string[done], fitting, &text, NK_TEXT_LEFT, f);
        done += fitting + 1; // also skip separator, if any
        line.y += f->height + 2 * t->padding.y;
        fitting = nk_text_clamp(f, &string[done], len - done, line.w, &glyphs, &width, seperator,NK_LEN(seperator));
    }
}
NK_API void
nk_text_colored(struct nk_context *ctx, const char *str, int len,
    nk_flags alignment, struct nk_color color)
{
    struct nk_window *win;
    const struct nk_style *style;

    struct nk_vec2 item_padding;
    struct nk_rect bounds;
    struct nk_text text;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout) return;

    win = ctx->current;
    style = &ctx->style;
    nk_panel_alloc_space(&bounds, ctx);
    item_padding = style->text.padding;

    text.padding.x = item_padding.x;
    text.padding.y = item_padding.y;
    text.background = style->window.background;
    text.text = nk_rgb_factor(color, style->text.color_factor);
    nk_widget_text(&win->buffer, bounds, str, len, &text, alignment, style->font);
}
NK_API void
nk_text_wrap_colored(struct nk_context *ctx, const char *str,
    int len, struct nk_color color)
{
    struct nk_window *win;
    const struct nk_style *style;

    struct nk_vec2 item_padding;
    struct nk_rect bounds;
    struct nk_text text;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout) return;

    win = ctx->current;
    style = &ctx->style;
    nk_panel_alloc_space(&bounds, ctx);
    item_padding = style->text.padding;

    text.padding.x = item_padding.x;
    text.padding.y = item_padding.y;
    text.background = style->window.background;
    text.text = nk_rgb_factor(color, style->text.color_factor);
    nk_widget_text_wrap(&win->buffer, bounds, str, len, &text, style->font);
}
#ifdef NK_INCLUDE_STANDARD_VARARGS
NK_API void
nk_labelf_colored(struct nk_context *ctx, nk_flags flags,
    struct nk_color color, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    nk_labelfv_colored(ctx, flags, color, fmt, args);
    va_end(args);
}
NK_API void
nk_labelf_colored_wrap(struct nk_context *ctx, struct nk_color color,
    const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    nk_labelfv_colored_wrap(ctx, color, fmt, args);
    va_end(args);
}
NK_API void
nk_labelf(struct nk_context *ctx, nk_flags flags, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    nk_labelfv(ctx, flags, fmt, args);
    va_end(args);
}
NK_API void
nk_labelf_wrap(struct nk_context *ctx, const char *fmt,...)
{
    va_list args;
    va_start(args, fmt);
    nk_labelfv_wrap(ctx, fmt, args);
    va_end(args);
}
NK_API void
nk_labelfv_colored(struct nk_context *ctx, nk_flags flags,
    struct nk_color color, const char *fmt, va_list args)
{
    char buf[256];
    nk_strfmt(buf, NK_LEN(buf), fmt, args);
    nk_label_colored(ctx, buf, flags, color);
}

NK_API void
nk_labelfv_colored_wrap(struct nk_context *ctx, struct nk_color color,
    const char *fmt, va_list args)
{
    char buf[256];
    nk_strfmt(buf, NK_LEN(buf), fmt, args);
    nk_label_colored_wrap(ctx, buf, color);
}

NK_API void
nk_labelfv(struct nk_context *ctx, nk_flags flags, const char *fmt, va_list args)
{
    char buf[256];
    nk_strfmt(buf, NK_LEN(buf), fmt, args);
    nk_label(ctx, buf, flags);
}

NK_API void
nk_labelfv_wrap(struct nk_context *ctx, const char *fmt, va_list args)
{
    char buf[256];
    nk_strfmt(buf, NK_LEN(buf), fmt, args);
    nk_label_wrap(ctx, buf);
}

NK_API void
nk_value_bool(struct nk_context *ctx, const char *prefix, int value)
{
    nk_labelf(ctx, NK_TEXT_LEFT, "%s: %s", prefix, ((value) ? "true": "false"));
}
NK_API void
nk_value_int(struct nk_context *ctx, const char *prefix, int value)
{
    nk_labelf(ctx, NK_TEXT_LEFT, "%s: %d", prefix, value);
}
NK_API void
nk_value_uint(struct nk_context *ctx, const char *prefix, unsigned int value)
{
    nk_labelf(ctx, NK_TEXT_LEFT, "%s: %u", prefix, value);
}
NK_API void
nk_value_float(struct nk_context *ctx, const char *prefix, float value)
{
    double double_value = (double)value;
    nk_labelf(ctx, NK_TEXT_LEFT, "%s: %.3f", prefix, double_value);
}
NK_API void
nk_value_color_byte(struct nk_context *ctx, const char *p, struct nk_color c)
{
    nk_labelf(ctx, NK_TEXT_LEFT, "%s: (%d, %d, %d, %d)", p, c.r, c.g, c.b, c.a);
}
NK_API void
nk_value_color_float(struct nk_context *ctx, const char *p, struct nk_color color)
{
    double c[4]; nk_color_dv(c, color);
    nk_labelf(ctx, NK_TEXT_LEFT, "%s: (%.2f, %.2f, %.2f, %.2f)",
        p, c[0], c[1], c[2], c[3]);
}
NK_API void
nk_value_color_hex(struct nk_context *ctx, const char *prefix, struct nk_color color)
{
    char hex[16];
    nk_color_hex_rgba(hex, color);
    nk_labelf(ctx, NK_TEXT_LEFT, "%s: %s", prefix, hex);
}
#endif
NK_API void
nk_text(struct nk_context *ctx, const char *str, int len, nk_flags alignment)
{
    NK_ASSERT(ctx);
    if (!ctx) return;
    nk_text_colored(ctx, str, len, alignment, ctx->style.text.color);
}
NK_API void
nk_text_wrap(struct nk_context *ctx, const char *str, int len)
{
    NK_ASSERT(ctx);
    if (!ctx) return;
    nk_text_wrap_colored(ctx, str, len, ctx->style.text.color);
}
NK_API void
nk_label(struct nk_context *ctx, const char *str, nk_flags alignment)
{
    nk_text(ctx, str, nk_strlen(str), alignment);
}
NK_API void
nk_label_colored(struct nk_context *ctx, const char *str, nk_flags align,
    struct nk_color color)
{
    nk_text_colored(ctx, str, nk_strlen(str), align, color);
}
NK_API void
nk_label_wrap(struct nk_context *ctx, const char *str)
{
    nk_text_wrap(ctx, str, nk_strlen(str));
}
NK_API void
nk_label_colored_wrap(struct nk_context *ctx, const char *str, struct nk_color color)
{
    nk_text_wrap_colored(ctx, str, nk_strlen(str), color);
}





/* ===============================================================
 *
 *                          IMAGE
 *
 * ===============================================================*/
NK_API nk_handle
nk_handle_ptr(void *ptr)
{
    nk_handle handle = {0};
    handle.ptr = ptr;
    return handle;
}
NK_API nk_handle
nk_handle_id(int id)
{
    nk_handle handle;
    nk_zero_struct(handle);
    handle.id = id;
    return handle;
}
NK_API struct nk_image
nk_subimage_ptr(void *ptr, nk_ushort w, nk_ushort h, struct nk_rect r)
{
    struct nk_image s;
    nk_zero(&s, sizeof(s));
    s.handle.ptr = ptr;
    s.w = w; s.h = h;
    s.region[0] = (nk_ushort)r.x;
    s.region[1] = (nk_ushort)r.y;
    s.region[2] = (nk_ushort)r.w;
    s.region[3] = (nk_ushort)r.h;
    return s;
}
NK_API struct nk_image
nk_subimage_id(int id, nk_ushort w, nk_ushort h, struct nk_rect r)
{
    struct nk_image s;
    nk_zero(&s, sizeof(s));
    s.handle.id = id;
    s.w = w; s.h = h;
    s.region[0] = (nk_ushort)r.x;
    s.region[1] = (nk_ushort)r.y;
    s.region[2] = (nk_ushort)r.w;
    s.region[3] = (nk_ushort)r.h;
    return s;
}
NK_API struct nk_image
nk_subimage_handle(nk_handle handle, nk_ushort w, nk_ushort h, struct nk_rect r)
{
    struct nk_image s;
    nk_zero(&s, sizeof(s));
    s.handle = handle;
    s.w = w; s.h = h;
    s.region[0] = (nk_ushort)r.x;
    s.region[1] = (nk_ushort)r.y;
    s.region[2] = (nk_ushort)r.w;
    s.region[3] = (nk_ushort)r.h;
    return s;
}
NK_API struct nk_image
nk_image_handle(nk_handle handle)
{
    struct nk_image s;
    nk_zero(&s, sizeof(s));
    s.handle = handle;
    s.w = 0; s.h = 0;
    s.region[0] = 0;
    s.region[1] = 0;
    s.region[2] = 0;
    s.region[3] = 0;
    return s;
}
NK_API struct nk_image
nk_image_ptr(void *ptr)
{
    struct nk_image s;
    nk_zero(&s, sizeof(s));
    NK_ASSERT(ptr);
    s.handle.ptr = ptr;
    s.w = 0; s.h = 0;
    s.region[0] = 0;
    s.region[1] = 0;
    s.region[2] = 0;
    s.region[3] = 0;
    return s;
}
NK_API struct nk_image
nk_image_id(int id)
{
    struct nk_image s;
    nk_zero(&s, sizeof(s));
    s.handle.id = id;
    s.w = 0; s.h = 0;
    s.region[0] = 0;
    s.region[1] = 0;
    s.region[2] = 0;
    s.region[3] = 0;
    return s;
}
NK_API nk_bool
nk_image_is_subimage(const struct nk_image* img)
{
    NK_ASSERT(img);
    return !(img->w == 0 && img->h == 0);
}
NK_API void
nk_image(struct nk_context *ctx, struct nk_image img)
{
    struct nk_window *win;
    struct nk_rect bounds;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout) return;

    win = ctx->current;
    if (!nk_widget(&bounds, ctx)) return;
    nk_draw_image(&win->buffer, bounds, &img, nk_white);
}
NK_API void
nk_image_color(struct nk_context *ctx, struct nk_image img, struct nk_color col)
{
    struct nk_window *win;
    struct nk_rect bounds;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout) return;

    win = ctx->current;
    if (!nk_widget(&bounds, ctx)) return;
    nk_draw_image(&win->buffer, bounds, &img, col);
}





/* ===============================================================
 *
 *                          9-SLICE
 *
 * ===============================================================*/
NK_API struct nk_nine_slice
nk_sub9slice_ptr(void *ptr, nk_ushort w, nk_ushort h, struct nk_rect rgn, nk_ushort l, nk_ushort t, nk_ushort r, nk_ushort b)
{
    struct nk_nine_slice s;
    struct nk_image *i = &s.img;
    nk_zero(&s, sizeof(s));
    i->handle.ptr = ptr;
    i->w = w; i->h = h;
    i->region[0] = (nk_ushort)rgn.x;
    i->region[1] = (nk_ushort)rgn.y;
    i->region[2] = (nk_ushort)rgn.w;
    i->region[3] = (nk_ushort)rgn.h;
    s.l = l; s.t = t; s.r = r; s.b = b;
    return s;
}
NK_API struct nk_nine_slice
nk_sub9slice_id(int id, nk_ushort w, nk_ushort h, struct nk_rect rgn, nk_ushort l, nk_ushort t, nk_ushort r, nk_ushort b)
{
    struct nk_nine_slice s;
    struct nk_image *i = &s.img;
    nk_zero(&s, sizeof(s));
    i->handle.id = id;
    i->w = w; i->h = h;
    i->region[0] = (nk_ushort)rgn.x;
    i->region[1] = (nk_ushort)rgn.y;
    i->region[2] = (nk_ushort)rgn.w;
    i->region[3] = (nk_ushort)rgn.h;
    s.l = l; s.t = t; s.r = r; s.b = b;
    return s;
}
NK_API struct nk_nine_slice
nk_sub9slice_handle(nk_handle handle, nk_ushort w, nk_ushort h, struct nk_rect rgn, nk_ushort l, nk_ushort t, nk_ushort r, nk_ushort b)
{
    struct nk_nine_slice s;
    struct nk_image *i = &s.img;
    nk_zero(&s, sizeof(s));
    i->handle = handle;
    i->w = w; i->h = h;
    i->region[0] = (nk_ushort)rgn.x;
    i->region[1] = (nk_ushort)rgn.y;
    i->region[2] = (nk_ushort)rgn.w;
    i->region[3] = (nk_ushort)rgn.h;
    s.l = l; s.t = t; s.r = r; s.b = b;
    return s;
}
NK_API struct nk_nine_slice
nk_nine_slice_handle(nk_handle handle, nk_ushort l, nk_ushort t, nk_ushort r, nk_ushort b)
{
    struct nk_nine_slice s;
    struct nk_image *i = &s.img;
    nk_zero(&s, sizeof(s));
    i->handle = handle;
    i->w = 0; i->h = 0;
    i->region[0] = 0;
    i->region[1] = 0;
    i->region[2] = 0;
    i->region[3] = 0;
    s.l = l; s.t = t; s.r = r; s.b = b;
    return s;
}
NK_API struct nk_nine_slice
nk_nine_slice_ptr(void *ptr, nk_ushort l, nk_ushort t, nk_ushort r, nk_ushort b)
{
    struct nk_nine_slice s;
    struct nk_image *i = &s.img;
    nk_zero(&s, sizeof(s));
    NK_ASSERT(ptr);
    i->handle.ptr = ptr;
    i->w = 0; i->h = 0;
    i->region[0] = 0;
    i->region[1] = 0;
    i->region[2] = 0;
    i->region[3] = 0;
    s.l = l; s.t = t; s.r = r; s.b = b;
    return s;
}
NK_API struct nk_nine_slice
nk_nine_slice_id(int id, nk_ushort l, nk_ushort t, nk_ushort r, nk_ushort b)
{
    struct nk_nine_slice s;
    struct nk_image *i = &s.img;
    nk_zero(&s, sizeof(s));
    i->handle.id = id;
    i->w = 0; i->h = 0;
    i->region[0] = 0;
    i->region[1] = 0;
    i->region[2] = 0;
    i->region[3] = 0;
    s.l = l; s.t = t; s.r = r; s.b = b;
    return s;
}
NK_API int
nk_nine_slice_is_sub9slice(const struct nk_nine_slice* slice)
{
    NK_ASSERT(slice);
    return !(slice->img.w == 0 && slice->img.h == 0);
}





/* ==============================================================
 *
 *                          BUTTON
 *
 * ===============================================================*/
NK_LIB void
nk_draw_symbol(struct nk_command_buffer *out, enum nk_symbol_type type,
    struct nk_rect content, struct nk_color background, struct nk_color foreground,
    float border_width, const struct nk_user_font *font)
{
    switch (type) {
    case NK_SYMBOL_X:
    case NK_SYMBOL_UNDERSCORE:
    case NK_SYMBOL_PLUS:
    case NK_SYMBOL_MINUS: {
        /* single character text symbol */
        const char *X = (type == NK_SYMBOL_X) ? "x":
            (type == NK_SYMBOL_UNDERSCORE) ? "_":
            (type == NK_SYMBOL_PLUS) ? "+": "-";
        struct nk_text text;
        text.padding = nk_vec2(0,0);
        text.background = background;
        text.text = foreground;
        nk_widget_text(out, content, X, 1, &text, NK_TEXT_CENTERED, font);
    } break;
    case NK_SYMBOL_CIRCLE_SOLID:
    case NK_SYMBOL_CIRCLE_OUTLINE:
    case NK_SYMBOL_RECT_SOLID:
    case NK_SYMBOL_RECT_OUTLINE: {
        /* simple empty/filled shapes */
        if (type == NK_SYMBOL_RECT_SOLID || type == NK_SYMBOL_RECT_OUTLINE) {
            nk_fill_rect(out, content,  0, foreground);
            if (type == NK_SYMBOL_RECT_OUTLINE)
                nk_fill_rect(out, nk_shrink_rect(content, border_width), 0, background);
        } else {
            nk_fill_circle(out, content, foreground);
            if (type == NK_SYMBOL_CIRCLE_OUTLINE)
                nk_fill_circle(out, nk_shrink_rect(content, 1), background);
        }
    } break;
    case NK_SYMBOL_TRIANGLE_UP:
    case NK_SYMBOL_TRIANGLE_DOWN:
    case NK_SYMBOL_TRIANGLE_LEFT:
    case NK_SYMBOL_TRIANGLE_RIGHT: {
        enum nk_heading heading;
        struct nk_vec2 points[3];
        heading = (type == NK_SYMBOL_TRIANGLE_RIGHT) ? NK_RIGHT :
            (type == NK_SYMBOL_TRIANGLE_LEFT) ? NK_LEFT:
            (type == NK_SYMBOL_TRIANGLE_UP) ? NK_UP: NK_DOWN;
        nk_triangle_from_direction(points, content, 0.15*content.w, 0.15*content.w, heading);
        nk_fill_triangle(out, points[0].x, points[0].y, points[1].x, points[1].y,
            points[2].x, points[2].y, foreground);
    } break;
    case NK_SYMBOL_TRIANGLE_UP_OUTLINE:
    case NK_SYMBOL_TRIANGLE_DOWN_OUTLINE:
    case NK_SYMBOL_TRIANGLE_LEFT_OUTLINE:
    case NK_SYMBOL_TRIANGLE_RIGHT_OUTLINE: {
        enum nk_heading heading;
        struct nk_vec2 points[3];
        heading = (type == NK_SYMBOL_TRIANGLE_RIGHT_OUTLINE) ? NK_RIGHT :
            (type == NK_SYMBOL_TRIANGLE_LEFT_OUTLINE) ? NK_LEFT:
            (type == NK_SYMBOL_TRIANGLE_UP_OUTLINE) ? NK_UP: NK_DOWN;
        nk_triangle_from_direction(points, content, 0, 0, heading);
        nk_stroke_triangle(out, points[0].x, points[0].y, points[1].x, points[1].y,
            points[2].x, points[2].y, border_width, foreground);
    } break;
    default:
    case NK_SYMBOL_NONE:
    case NK_SYMBOL_MAX: break;
    }
}
NK_LIB nk_bool
nk_button_behavior(nk_flags *state, struct nk_rect r,
    const struct nk_input *i, enum nk_button_behavior behavior)
{
    int ret = 0;
    nk_widget_state_reset(state);
    if (!i) return 0;
    if (nk_input_is_mouse_hovering_rect(i, r)) {
        *state = NK_WIDGET_STATE_HOVERED;
        if (nk_input_is_mouse_down(i, NK_BUTTON_LEFT))
            *state = NK_WIDGET_STATE_ACTIVE;
        if (nk_input_has_mouse_click_in_button_rect(i, NK_BUTTON_LEFT, r)) {
            ret = (behavior != NK_BUTTON_DEFAULT) ?
                nk_input_is_mouse_down(i, NK_BUTTON_LEFT):
#ifdef NK_BUTTON_TRIGGER_ON_RELEASE
                nk_input_is_mouse_released(i, NK_BUTTON_LEFT);
#else
                nk_input_is_mouse_pressed(i, NK_BUTTON_LEFT);
#endif
        }
    }
    if (*state & NK_WIDGET_STATE_HOVER && !nk_input_is_mouse_prev_hovering_rect(i, r))
        *state |= NK_WIDGET_STATE_ENTERED;
    else if (nk_input_is_mouse_prev_hovering_rect(i, r))
        *state |= NK_WIDGET_STATE_LEFT;
    return ret;
}
NK_LIB const struct nk_style_item*
nk_draw_button(struct nk_command_buffer *out,
    const struct nk_rect *bounds, nk_flags state,
    const struct nk_style_button *style)
{
    const struct nk_style_item *background;
    if (state & NK_WIDGET_STATE_HOVER)
        background = &style->hover;
    else if (state & NK_WIDGET_STATE_ACTIVED)
        background = &style->active;
    else background = &style->normal;

    switch (background->type) {
        case NK_STYLE_ITEM_IMAGE:
            nk_draw_image(out, *bounds, &background->data.image, nk_rgb_factor(nk_white, style->color_factor_background));
            break;
        case NK_STYLE_ITEM_NINE_SLICE:
            nk_draw_nine_slice(out, *bounds, &background->data.slice, nk_rgb_factor(nk_white, style->color_factor_background));
            break;
        case NK_STYLE_ITEM_COLOR:
            nk_fill_rect(out, *bounds, style->rounding, nk_rgb_factor(background->data.color, style->color_factor_background));
            nk_stroke_rect(out, *bounds, style->rounding, style->border, nk_rgb_factor(style->border_color, style->color_factor_background));
            break;
    }
    return background;
}
NK_LIB nk_bool
nk_do_button(nk_flags *state, struct nk_command_buffer *out, struct nk_rect r,
    const struct nk_style_button *style, const struct nk_input *in,
    enum nk_button_behavior behavior, struct nk_rect *content)
{
    struct nk_rect bounds;
    NK_ASSERT(style);
    NK_ASSERT(state);
    NK_ASSERT(out);
    if (!out || !style)
        return nk_false;

    /* calculate button content space */
    content->x = r.x + style->padding.x + style->border + style->rounding;
    content->y = r.y + style->padding.y + style->border + style->rounding;
    content->w = r.w - (2 * (style->padding.x + style->border + style->rounding));
    content->h = r.h - (2 * (style->padding.y + style->border + style->rounding));

    /* execute button behavior */
    bounds.x = r.x - style->touch_padding.x;
    bounds.y = r.y - style->touch_padding.y;
    bounds.w = r.w + 2 * style->touch_padding.x;
    bounds.h = r.h + 2 * style->touch_padding.y;
    return nk_button_behavior(state, bounds, in, behavior);
}
NK_LIB void
nk_draw_button_text(struct nk_command_buffer *out,
    const struct nk_rect *bounds, const struct nk_rect *content, nk_flags state,
    const struct nk_style_button *style, const char *txt, int len,
    nk_flags text_alignment, const struct nk_user_font *font)
{
    struct nk_text text;
    const struct nk_style_item *background;
    background = nk_draw_button(out, bounds, state, style);

    /* select correct colors/images */
    if (background->type == NK_STYLE_ITEM_COLOR)
        text.background = background->data.color;
    else text.background = style->text_background;
    if (state & NK_WIDGET_STATE_HOVER)
        text.text = style->text_hover;
    else if (state & NK_WIDGET_STATE_ACTIVED)
        text.text = style->text_active;
    else text.text = style->text_normal;

    text.text = nk_rgb_factor(text.text, style->color_factor_text);

    text.padding = nk_vec2(0,0);
    nk_widget_text(out, *content, txt, len, &text, text_alignment, font);
}
NK_LIB nk_bool
nk_do_button_text(nk_flags *state,
    struct nk_command_buffer *out, struct nk_rect bounds,
    const char *string, int len, nk_flags align, enum nk_button_behavior behavior,
    const struct nk_style_button *style, const struct nk_input *in,
    const struct nk_user_font *font)
{
    struct nk_rect content;
    int ret = nk_false;

    NK_ASSERT(state);
    NK_ASSERT(style);
    NK_ASSERT(out);
    NK_ASSERT(string);
    NK_ASSERT(font);
    if (!out || !style || !font || !string)
        return nk_false;

    ret = nk_do_button(state, out, bounds, style, in, behavior, &content);
    if (style->draw_begin) style->draw_begin(out, style->userdata);
    nk_draw_button_text(out, &bounds, &content, *state, style, string, len, align, font);
    if (style->draw_end) style->draw_end(out, style->userdata);
    return ret;
}
NK_LIB void
nk_draw_button_symbol(struct nk_command_buffer *out,
    const struct nk_rect *bounds, const struct nk_rect *content,
    nk_flags state, const struct nk_style_button *style,
    enum nk_symbol_type type, const struct nk_user_font *font)
{
    struct nk_color sym, bg;
    const struct nk_style_item *background;

    /* select correct colors/images */
    background = nk_draw_button(out, bounds, state, style);
    if (background->type == NK_STYLE_ITEM_COLOR)
        bg = background->data.color;
    else bg = style->text_background;

    if (state & NK_WIDGET_STATE_HOVER)
        sym = style->text_hover;
    else if (state & NK_WIDGET_STATE_ACTIVED)
        sym = style->text_active;
    else sym = style->text_normal;

    sym = nk_rgb_factor(sym, style->color_factor_text);
    nk_draw_symbol(out, type, *content, bg, sym, 1, font);
}
NK_LIB nk_bool
nk_do_button_symbol(nk_flags *state,
    struct nk_command_buffer *out, struct nk_rect bounds,
    enum nk_symbol_type symbol, enum nk_button_behavior behavior,
    const struct nk_style_button *style, const struct nk_input *in,
    const struct nk_user_font *font)
{
    int ret;
    struct nk_rect content;

    NK_ASSERT(state);
    NK_ASSERT(style);
    NK_ASSERT(font);
    NK_ASSERT(out);
    if (!out || !style || !font || !state)
        return nk_false;

    ret = nk_do_button(state, out, bounds, style, in, behavior, &content);
    if (style->draw_begin) style->draw_begin(out, style->userdata);
    nk_draw_button_symbol(out, &bounds, &content, *state, style, symbol, font);
    if (style->draw_end) style->draw_end(out, style->userdata);
    return ret;
}
NK_LIB void
nk_draw_button_image(struct nk_command_buffer *out,
    const struct nk_rect *bounds, const struct nk_rect *content,
    nk_flags state, const struct nk_style_button *style, const struct nk_image *img)
{
    nk_draw_button(out, bounds, state, style);
    nk_draw_image(out, *content, img, nk_rgb_factor(nk_white, style->color_factor_background));
}
NK_LIB nk_bool
nk_do_button_image(nk_flags *state,
    struct nk_command_buffer *out, struct nk_rect bounds,
    struct nk_image img, enum nk_button_behavior b,
    const struct nk_style_button *style, const struct nk_input *in)
{
    int ret;
    struct nk_rect content;

    NK_ASSERT(state);
    NK_ASSERT(style);
    NK_ASSERT(out);
    if (!out || !style || !state)
        return nk_false;

    ret = nk_do_button(state, out, bounds, style, in, b, &content);
    content.x += style->image_padding.x;
    content.y += style->image_padding.y;
    content.w -= 2 * style->image_padding.x;
    content.h -= 2 * style->image_padding.y;

    if (style->draw_begin) style->draw_begin(out, style->userdata);
    nk_draw_button_image(out, &bounds, &content, *state, style, &img);
    if (style->draw_end) style->draw_end(out, style->userdata);
    return ret;
}
NK_LIB void
nk_draw_button_text_symbol(struct nk_command_buffer *out,
    const struct nk_rect *bounds, const struct nk_rect *label,
    const struct nk_rect *symbol, nk_flags state, const struct nk_style_button *style,
    const char *str, int len, enum nk_symbol_type type,
    const struct nk_user_font *font)
{
    struct nk_color sym;
    struct nk_text text;
    const struct nk_style_item *background;

    /* select correct background colors/images */
    background = nk_draw_button(out, bounds, state, style);
    if (background->type == NK_STYLE_ITEM_COLOR)
        text.background = background->data.color;
    else text.background = style->text_background;

    /* select correct text colors */
    if (state & NK_WIDGET_STATE_HOVER) {
        sym = style->text_hover;
        text.text = style->text_hover;
    } else if (state & NK_WIDGET_STATE_ACTIVED) {
        sym = style->text_active;
        text.text = style->text_active;
    } else {
        sym = style->text_normal;
        text.text = style->text_normal;
    }

    sym = nk_rgb_factor(sym, style->color_factor_text);
    text.text = nk_rgb_factor(text.text, style->color_factor_text);
    text.padding = nk_vec2(0,0);
    nk_draw_symbol(out, type, *symbol, style->text_background, sym, 0, font);
    nk_widget_text(out, *label, str, len, &text, NK_TEXT_CENTERED, font);
}
NK_LIB nk_bool
nk_do_button_text_symbol(nk_flags *state,
    struct nk_command_buffer *out, struct nk_rect bounds,
    enum nk_symbol_type symbol, const char *str, int len, nk_flags align,
    enum nk_button_behavior behavior, const struct nk_style_button *style,
    const struct nk_user_font *font, const struct nk_input *in)
{
    int ret;
    struct nk_rect tri = {0,0,0,0};
    struct nk_rect content;

    NK_ASSERT(style);
    NK_ASSERT(out);
    NK_ASSERT(font);
    if (!out || !style || !font)
        return nk_false;

    ret = nk_do_button(state, out, bounds, style, in, behavior, &content);
    tri.y = content.y + (content.h/2) - font->height/2;
    tri.w = font->height; tri.h = font->height;
    if (align & NK_TEXT_ALIGN_LEFT) {
        tri.x = (content.x + content.w) - (2 * style->padding.x + tri.w);
        tri.x = NK_MAX(tri.x, 0);
    } else tri.x = content.x + 2 * style->padding.x;

    /* draw button */
    if (style->draw_begin) style->draw_begin(out, style->userdata);
    nk_draw_button_text_symbol(out, &bounds, &content, &tri,
        *state, style, str, len, symbol, font);
    if (style->draw_end) style->draw_end(out, style->userdata);
    return ret;
}
NK_LIB void
nk_draw_button_text_image(struct nk_command_buffer *out,
    const struct nk_rect *bounds, const struct nk_rect *label,
    const struct nk_rect *image, nk_flags state, const struct nk_style_button *style,
    const char *str, int len, const struct nk_user_font *font,
    const struct nk_image *img)
{
    struct nk_text text;
    const struct nk_style_item *background;
    background = nk_draw_button(out, bounds, state, style);

    /* select correct colors */
    if (background->type == NK_STYLE_ITEM_COLOR)
        text.background = background->data.color;
    else text.background = style->text_background;
    if (state & NK_WIDGET_STATE_HOVER)
        text.text = style->text_hover;
    else if (state & NK_WIDGET_STATE_ACTIVED)
        text.text = style->text_active;
    else text.text = style->text_normal;

    text.text = nk_rgb_factor(text.text, style->color_factor_text);
    text.padding = nk_vec2(0, 0);
    nk_widget_text(out, *label, str, len, &text, NK_TEXT_CENTERED, font);
    nk_draw_image(out, *image, img, nk_rgb_factor(nk_white, style->color_factor_background));
}
NK_LIB nk_bool
nk_do_button_text_image(nk_flags *state,
    struct nk_command_buffer *out, struct nk_rect bounds,
    struct nk_image img, const char* str, int len, nk_flags align,
    enum nk_button_behavior behavior, const struct nk_style_button *style,
    const struct nk_user_font *font, const struct nk_input *in)
{
    int ret;
    struct nk_rect icon;
    struct nk_rect content;

    NK_ASSERT(style);
    NK_ASSERT(state);
    NK_ASSERT(font);
    NK_ASSERT(out);
    if (!out || !font || !style || !str)
        return nk_false;

    ret = nk_do_button(state, out, bounds, style, in, behavior, &content);
    icon.y = bounds.y + style->padding.y;
    icon.w = icon.h = bounds.h - 2 * style->padding.y;
    if (align & NK_TEXT_ALIGN_LEFT) {
        icon.x = (bounds.x + bounds.w) - (2 * style->padding.x + icon.w);
        icon.x = NK_MAX(icon.x, 0);
    } else icon.x = bounds.x + 2 * style->padding.x;

    icon.x += style->image_padding.x;
    icon.y += style->image_padding.y;
    icon.w -= 2 * style->image_padding.x;
    icon.h -= 2 * style->image_padding.y;

    if (style->draw_begin) style->draw_begin(out, style->userdata);
    nk_draw_button_text_image(out, &bounds, &content, &icon, *state, style, str, len, font, &img);
    if (style->draw_end) style->draw_end(out, style->userdata);
    return ret;
}
NK_API void
nk_button_set_behavior(struct nk_context *ctx, enum nk_button_behavior behavior)
{
    NK_ASSERT(ctx);
    if (!ctx) return;
    ctx->button_behavior = behavior;
}
NK_API nk_bool
nk_button_push_behavior(struct nk_context *ctx, enum nk_button_behavior behavior)
{
    struct nk_config_stack_button_behavior *button_stack;
    struct nk_config_stack_button_behavior_element *element;

    NK_ASSERT(ctx);
    if (!ctx) return 0;

    button_stack = &ctx->stacks.button_behaviors;
    NK_ASSERT(button_stack->head < (int)NK_LEN(button_stack->elements));
    if (button_stack->head >= (int)NK_LEN(button_stack->elements))
        return 0;

    element = &button_stack->elements[button_stack->head++];
    element->address = &ctx->button_behavior;
    element->old_value = ctx->button_behavior;
    ctx->button_behavior = behavior;
    return 1;
}
NK_API nk_bool
nk_button_pop_behavior(struct nk_context *ctx)
{
    struct nk_config_stack_button_behavior *button_stack;
    struct nk_config_stack_button_behavior_element *element;

    NK_ASSERT(ctx);
    if (!ctx) return 0;

    button_stack = &ctx->stacks.button_behaviors;
    NK_ASSERT(button_stack->head > 0);
    if (button_stack->head < 1)
        return 0;

    element = &button_stack->elements[--button_stack->head];
    *element->address = element->old_value;
    return 1;
}
NK_API nk_bool
nk_button_text_styled(struct nk_context *ctx,
    const struct nk_style_button *style, const char *title, int len)
{
    struct nk_window *win;
    struct nk_panel *layout;
    const struct nk_input *in;

    struct nk_rect bounds;
    enum nk_widget_layout_states state;

    NK_ASSERT(ctx);
    NK_ASSERT(style);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!style || !ctx || !ctx->current || !ctx->current->layout) return 0;

    win = ctx->current;
    layout = win->layout;
    state = nk_widget(&bounds, ctx);

    if (!state) return 0;
    in = (state == NK_WIDGET_ROM || state == NK_WIDGET_DISABLED || layout->flags & NK_WINDOW_ROM) ? 0 : &ctx->input;
    return nk_do_button_text(&ctx->last_widget_state, &win->buffer, bounds,
                    title, len, style->text_alignment, ctx->button_behavior,
                    style, in, ctx->style.font);
}
NK_API nk_bool
nk_button_text(struct nk_context *ctx, const char *title, int len)
{
    NK_ASSERT(ctx);
    if (!ctx) return 0;
    return nk_button_text_styled(ctx, &ctx->style.button, title, len);
}
NK_API nk_bool nk_button_label_styled(struct nk_context *ctx,
    const struct nk_style_button *style, const char *title)
{
    return nk_button_text_styled(ctx, style, title, nk_strlen(title));
}
NK_API nk_bool nk_button_label(struct nk_context *ctx, const char *title)
{
    return nk_button_text(ctx, title, nk_strlen(title));
}
NK_API nk_bool
nk_button_color(struct nk_context *ctx, struct nk_color color)
{
    struct nk_window *win;
    struct nk_panel *layout;
    const struct nk_input *in;
    struct nk_style_button button;

    int ret = 0;
    struct nk_rect bounds;
    struct nk_rect content;
    enum nk_widget_layout_states state;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    win = ctx->current;
    layout = win->layout;

    state = nk_widget(&bounds, ctx);
    if (!state) return 0;
    in = (state == NK_WIDGET_ROM || state == NK_WIDGET_DISABLED || layout->flags & NK_WINDOW_ROM) ? 0 : &ctx->input;

    button = ctx->style.button;
    button.normal = nk_style_item_color(color);
    button.hover = nk_style_item_color(color);
    button.active = nk_style_item_color(color);
    ret = nk_do_button(&ctx->last_widget_state, &win->buffer, bounds,
                &button, in, ctx->button_behavior, &content);
    nk_draw_button(&win->buffer, &bounds, ctx->last_widget_state, &button);
    return ret;
}
NK_API nk_bool
nk_button_symbol_styled(struct nk_context *ctx,
    const struct nk_style_button *style, enum nk_symbol_type symbol)
{
    struct nk_window *win;
    struct nk_panel *layout;
    const struct nk_input *in;

    struct nk_rect bounds;
    enum nk_widget_layout_states state;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    win = ctx->current;
    layout = win->layout;
    state = nk_widget(&bounds, ctx);
    if (!state) return 0;
    in = (state == NK_WIDGET_ROM || state == NK_WIDGET_DISABLED || layout->flags & NK_WINDOW_ROM) ? 0 : &ctx->input;
    return nk_do_button_symbol(&ctx->last_widget_state, &win->buffer, bounds,
            symbol, ctx->button_behavior, style, in, ctx->style.font);
}
NK_API nk_bool
nk_button_symbol(struct nk_context *ctx, enum nk_symbol_type symbol)
{
    NK_ASSERT(ctx);
    if (!ctx) return 0;
    return nk_button_symbol_styled(ctx, &ctx->style.button, symbol);
}
NK_API nk_bool
nk_button_image_styled(struct nk_context *ctx, const struct nk_style_button *style,
    struct nk_image img)
{
    struct nk_window *win;
    struct nk_panel *layout;
    const struct nk_input *in;

    struct nk_rect bounds;
    enum nk_widget_layout_states state;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    win = ctx->current;
    layout = win->layout;

    state = nk_widget(&bounds, ctx);
    if (!state) return 0;
    in = (state == NK_WIDGET_ROM || state == NK_WIDGET_DISABLED || layout->flags & NK_WINDOW_ROM) ? 0 : &ctx->input;
    return nk_do_button_image(&ctx->last_widget_state, &win->buffer, bounds,
                img, ctx->button_behavior, style, in);
}
NK_API nk_bool
nk_button_image(struct nk_context *ctx, struct nk_image img)
{
    NK_ASSERT(ctx);
    if (!ctx) return 0;
    return nk_button_image_styled(ctx, &ctx->style.button, img);
}
NK_API nk_bool
nk_button_symbol_text_styled(struct nk_context *ctx,
    const struct nk_style_button *style, enum nk_symbol_type symbol,
    const char *text, int len, nk_flags align)
{
    struct nk_window *win;
    struct nk_panel *layout;
    const struct nk_input *in;

    struct nk_rect bounds;
    enum nk_widget_layout_states state;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    win = ctx->current;
    layout = win->layout;

    state = nk_widget(&bounds, ctx);
    if (!state) return 0;
    in = (state == NK_WIDGET_ROM || state == NK_WIDGET_DISABLED || layout->flags & NK_WINDOW_ROM) ? 0 : &ctx->input;
    return nk_do_button_text_symbol(&ctx->last_widget_state, &win->buffer, bounds,
                symbol, text, len, align, ctx->button_behavior,
                style, ctx->style.font, in);
}
NK_API nk_bool
nk_button_symbol_text(struct nk_context *ctx, enum nk_symbol_type symbol,
    const char* text, int len, nk_flags align)
{
    NK_ASSERT(ctx);
    if (!ctx) return 0;
    return nk_button_symbol_text_styled(ctx, &ctx->style.button, symbol, text, len, align);
}
NK_API nk_bool nk_button_symbol_label(struct nk_context *ctx, enum nk_symbol_type symbol,
    const char *label, nk_flags align)
{
    return nk_button_symbol_text(ctx, symbol, label, nk_strlen(label), align);
}
NK_API nk_bool nk_button_symbol_label_styled(struct nk_context *ctx,
    const struct nk_style_button *style, enum nk_symbol_type symbol,
    const char *title, nk_flags align)
{
    return nk_button_symbol_text_styled(ctx, style, symbol, title, nk_strlen(title), align);
}
NK_API nk_bool
nk_button_image_text_styled(struct nk_context *ctx,
    const struct nk_style_button *style, struct nk_image img, const char *text,
    int len, nk_flags align)
{
    struct nk_window *win;
    struct nk_panel *layout;
    const struct nk_input *in;

    struct nk_rect bounds;
    enum nk_widget_layout_states state;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    win = ctx->current;
    layout = win->layout;

    state = nk_widget(&bounds, ctx);
    if (!state) return 0;
    in = (state == NK_WIDGET_ROM || state == NK_WIDGET_DISABLED || layout->flags & NK_WINDOW_ROM) ? 0 : &ctx->input;
    return nk_do_button_text_image(&ctx->last_widget_state, &win->buffer,
            bounds, img, text, len, align, ctx->button_behavior,
            style, ctx->style.font, in);
}
NK_API nk_bool
nk_button_image_text(struct nk_context *ctx, struct nk_image img,
    const char *text, int len, nk_flags align)
{
    return nk_button_image_text_styled(ctx, &ctx->style.button,img, text, len, align);
}
NK_API nk_bool nk_button_image_label(struct nk_context *ctx, struct nk_image img,
    const char *label, nk_flags align)
{
    return nk_button_image_text(ctx, img, label, nk_strlen(label), align);
}
NK_API nk_bool nk_button_image_label_styled(struct nk_context *ctx,
    const struct nk_style_button *style, struct nk_image img,
    const char *label, nk_flags text_alignment)
{
    return nk_button_image_text_styled(ctx, style, img, label, nk_strlen(label), text_alignment);
}





/* ===============================================================
 *
 *                              TOGGLE
 *
 * ===============================================================*/
NK_LIB nk_bool
nk_toggle_behavior(const struct nk_input *in, struct nk_rect select,
    nk_flags *state, nk_bool active)
{
    nk_widget_state_reset(state);
    if (nk_button_behavior(state, select, in, NK_BUTTON_DEFAULT)) {
        *state = NK_WIDGET_STATE_ACTIVE;
        active = !active;
    }
    if (*state & NK_WIDGET_STATE_HOVER && !nk_input_is_mouse_prev_hovering_rect(in, select))
        *state |= NK_WIDGET_STATE_ENTERED;
    else if (nk_input_is_mouse_prev_hovering_rect(in, select))
        *state |= NK_WIDGET_STATE_LEFT;
    return active;
}
NK_LIB void
nk_draw_checkbox(struct nk_command_buffer *out,
    nk_flags state, const struct nk_style_toggle *style, nk_bool active,
    const struct nk_rect *label, const struct nk_rect *selector,
    const struct nk_rect *cursors, const char *string, int len,
    const struct nk_user_font *font, nk_flags text_alignment)
{
    const struct nk_style_item *background;
    const struct nk_style_item *cursor;
    struct nk_text text;

    /* select correct colors/images */
    if (state & NK_WIDGET_STATE_HOVER) {
        background = &style->hover;
        cursor = &style->cursor_hover;
        text.text = style->text_hover;
    } else if (state & NK_WIDGET_STATE_ACTIVED) {
        background = &style->hover;
        cursor = &style->cursor_hover;
        text.text = style->text_active;
    } else {
        background = &style->normal;
        cursor = &style->cursor_normal;
        text.text = style->text_normal;
    }

    text.text = nk_rgb_factor(text.text, style->color_factor);
    text.padding.x = 0;
    text.padding.y = 0;
    text.background = style->text_background;
    nk_widget_text(out, *label, string, len, &text, text_alignment, font);

    /* draw background and cursor */
    if (background->type == NK_STYLE_ITEM_COLOR) {
        nk_fill_rect(out, *selector, 0, nk_rgb_factor(style->border_color, style->color_factor));
        nk_fill_rect(out, nk_shrink_rect(*selector, style->border), 0, nk_rgb_factor(background->data.color, style->color_factor));
    } else nk_draw_image(out, *selector, &background->data.image, nk_rgb_factor(nk_white, style->color_factor));
    if (active) {
        if (cursor->type == NK_STYLE_ITEM_IMAGE)
            nk_draw_image(out, *cursors, &cursor->data.image, nk_rgb_factor(nk_white, style->color_factor));
        else nk_fill_rect(out, *cursors, 0, cursor->data.color);
    }
}
NK_LIB void
nk_draw_option(struct nk_command_buffer *out,
    nk_flags state, const struct nk_style_toggle *style, nk_bool active,
    const struct nk_rect *label, const struct nk_rect *selector,
    const struct nk_rect *cursors, const char *string, int len,
    const struct nk_user_font *font, nk_flags text_alignment)
{
    const struct nk_style_item *background;
    const struct nk_style_item *cursor;
    struct nk_text text;

    /* select correct colors/images */
    if (state & NK_WIDGET_STATE_HOVER) {
        background = &style->hover;
        cursor = &style->cursor_hover;
        text.text = style->text_hover;
    } else if (state & NK_WIDGET_STATE_ACTIVED) {
        background = &style->hover;
        cursor = &style->cursor_hover;
        text.text = style->text_active;
    } else {
        background = &style->normal;
        cursor = &style->cursor_normal;
        text.text = style->text_normal;
    }

    text.text = nk_rgb_factor(text.text, style->color_factor);
    text.padding.x = 0;
    text.padding.y = 0;
    text.background = style->text_background;
    nk_widget_text(out, *label, string, len, &text, text_alignment, font);

    /* draw background and cursor */
    if (background->type == NK_STYLE_ITEM_COLOR) {
        nk_fill_circle(out, *selector, nk_rgb_factor(style->border_color, style->color_factor));
        nk_fill_circle(out, nk_shrink_rect(*selector, style->border), nk_rgb_factor(background->data.color, style->color_factor));
    } else nk_draw_image(out, *selector, &background->data.image, nk_rgb_factor(nk_white, style->color_factor));
    if (active) {
        if (cursor->type == NK_STYLE_ITEM_IMAGE)
            nk_draw_image(out, *cursors, &cursor->data.image, nk_rgb_factor(nk_white, style->color_factor));
        else nk_fill_circle(out, *cursors, cursor->data.color);
    }
}
NK_LIB nk_bool
nk_do_toggle(nk_flags *state,
    struct nk_command_buffer *out, struct nk_rect r,
    nk_bool *active, const char *str, int len, enum nk_toggle_type type,
    const struct nk_style_toggle *style, const struct nk_input *in,
    const struct nk_user_font *font, nk_flags widget_alignment, nk_flags text_alignment)
{
    int was_active;
    struct nk_rect bounds;
    struct nk_rect select;
    struct nk_rect cursor;
    struct nk_rect label;

    NK_ASSERT(style);
    NK_ASSERT(out);
    NK_ASSERT(font);
    if (!out || !style || !font || !active)
        return 0;

    r.w = NK_MAX(r.w, font->height + 2 * style->padding.x);
    r.h = NK_MAX(r.h, font->height + 2 * style->padding.y);

    /* add additional touch padding for touch screen devices */
    bounds.x = r.x - style->touch_padding.x;
    bounds.y = r.y - style->touch_padding.y;
    bounds.w = r.w + 2 * style->touch_padding.x;
    bounds.h = r.h + 2 * style->touch_padding.y;

    /* calculate the selector space */
    select.w = font->height;
    select.h = select.w;

    if (widget_alignment & NK_WIDGET_ALIGN_RIGHT) {
        select.x = r.x + r.w - font->height;

        /* label in front of the selector */
        label.x = r.x;
        label.w = r.w - select.w - style->spacing * 2;
    } else if (widget_alignment & NK_WIDGET_ALIGN_CENTERED) {
        select.x = r.x + (r.w - select.w) / 2;

        /* label in front of selector */
        label.x = r.x;
        label.w = (r.w - select.w - style->spacing * 2) / 2;
    } else { /* Default: NK_WIDGET_ALIGN_LEFT */
        select.x = r.x;

        /* label behind the selector */
        label.x = select.x + select.w + style->spacing;
        label.w = NK_MAX(r.x + r.w, label.x) - label.x;
    }

    if (widget_alignment & NK_WIDGET_ALIGN_TOP) {
        select.y = r.y;
    } else if (widget_alignment & NK_WIDGET_ALIGN_BOTTOM) {
        select.y = r.y + r.h - select.h - 2 * style->padding.y;
    } else { /* Default: NK_WIDGET_ALIGN_MIDDLE */
        select.y = r.y + r.h/2.0f - select.h/2.0f;
    }

    label.y = select.y;
    label.h = select.w;

    /* calculate the bounds of the cursor inside the selector */
    cursor.x = select.x + style->padding.x + style->border;
    cursor.y = select.y + style->padding.y + style->border;
    cursor.w = select.w - (2 * style->padding.x + 2 * style->border);
    cursor.h = select.h - (2 * style->padding.y + 2 * style->border);

    /* update selector */
    was_active = *active;
    *active = nk_toggle_behavior(in, bounds, state, *active);

    /* draw selector */
    if (style->draw_begin)
        style->draw_begin(out, style->userdata);
    if (type == NK_TOGGLE_CHECK) {
        nk_draw_checkbox(out, *state, style, *active, &label, &select, &cursor, str, len, font, text_alignment);
    } else {
        nk_draw_option(out, *state, style, *active, &label, &select, &cursor, str, len, font, text_alignment);
    }
    if (style->draw_end)
        style->draw_end(out, style->userdata);
    return (was_active != *active);
}
/*----------------------------------------------------------------
 *
 *                          CHECKBOX
 *
 * --------------------------------------------------------------*/
NK_API nk_bool
nk_check_text(struct nk_context *ctx, const char *text, int len, nk_bool active)
{
    struct nk_window *win;
    struct nk_panel *layout;
    const struct nk_input *in;
    const struct nk_style *style;

    struct nk_rect bounds;
    enum nk_widget_layout_states state;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return active;

    win = ctx->current;
    style = &ctx->style;
    layout = win->layout;

    state = nk_widget(&bounds, ctx);
    if (!state) return active;
    in = (state == NK_WIDGET_ROM || state == NK_WIDGET_DISABLED || layout->flags & NK_WINDOW_ROM) ? 0 : &ctx->input;
    nk_do_toggle(&ctx->last_widget_state, &win->buffer, bounds, &active,
        text, len, NK_TOGGLE_CHECK, &style->checkbox, in, style->font, NK_WIDGET_LEFT, NK_TEXT_LEFT);
    return active;
}
NK_API nk_bool
nk_check_text_align(struct nk_context *ctx, const char *text, int len, nk_bool active, nk_flags widget_alignment, nk_flags text_alignment)
{
    struct nk_window *win;
    struct nk_panel *layout;
    const struct nk_input *in;
    const struct nk_style *style;

    struct nk_rect bounds;
    enum nk_widget_layout_states state;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return active;

    win = ctx->current;
    style = &ctx->style;
    layout = win->layout;

    state = nk_widget(&bounds, ctx);
    if (!state) return active;
    in = (state == NK_WIDGET_ROM || state == NK_WIDGET_DISABLED || layout->flags & NK_WINDOW_ROM) ? 0 : &ctx->input;
    nk_do_toggle(&ctx->last_widget_state, &win->buffer, bounds, &active,
        text, len, NK_TOGGLE_CHECK, &style->checkbox, in, style->font, widget_alignment, text_alignment);
    return active;
}
NK_API unsigned int
nk_check_flags_text(struct nk_context *ctx, const char *text, int len,
    unsigned int flags, unsigned int value)
{
    int old_active;
    NK_ASSERT(ctx);
    NK_ASSERT(text);
    if (!ctx || !text) return flags;
    old_active = (int)((flags & value) & value);
    if (nk_check_text(ctx, text, len, old_active))
        flags |= value;
    else flags &= ~value;
    return flags;
}
NK_API nk_bool
nk_checkbox_text(struct nk_context *ctx, const char *text, int len, nk_bool *active)
{
    int old_val;
    NK_ASSERT(ctx);
    NK_ASSERT(text);
    NK_ASSERT(active);
    if (!ctx || !text || !active) return 0;
    old_val = *active;
    *active = nk_check_text(ctx, text, len, *active);
    return old_val != *active;
}
NK_API nk_bool
nk_checkbox_text_align(struct nk_context *ctx, const char *text, int len, nk_bool *active, nk_flags widget_alignment, nk_flags text_alignment)
{
    int old_val;
    NK_ASSERT(ctx);
    NK_ASSERT(text);
    NK_ASSERT(active);
    if (!ctx || !text || !active) return 0;
    old_val = *active;
    *active = nk_check_text_align(ctx, text, len, *active, widget_alignment, text_alignment);
    return old_val != *active;
}
NK_API nk_bool
nk_checkbox_flags_text(struct nk_context *ctx, const char *text, int len,
    unsigned int *flags, unsigned int value)
{
    nk_bool active;
    NK_ASSERT(ctx);
    NK_ASSERT(text);
    NK_ASSERT(flags);
    if (!ctx || !text || !flags) return 0;

    active = (int)((*flags & value) & value);
    if (nk_checkbox_text(ctx, text, len, &active)) {
        if (active) *flags |= value;
        else *flags &= ~value;
        return 1;
    }
    return 0;
}
NK_API nk_bool nk_check_label(struct nk_context *ctx, const char *label, nk_bool active)
{
    return nk_check_text(ctx, label, nk_strlen(label), active);
}
NK_API unsigned int nk_check_flags_label(struct nk_context *ctx, const char *label,
    unsigned int flags, unsigned int value)
{
    return nk_check_flags_text(ctx, label, nk_strlen(label), flags, value);
}
NK_API nk_bool nk_checkbox_label(struct nk_context *ctx, const char *label, nk_bool *active)
{
    return nk_checkbox_text(ctx, label, nk_strlen(label), active);
}
NK_API nk_bool nk_checkbox_label_align(struct nk_context *ctx, const char *label, nk_bool *active, nk_flags widget_alignment, nk_flags text_alignment)
{
    return nk_checkbox_text_align(ctx, label, nk_strlen(label), active, widget_alignment, text_alignment);
}
NK_API nk_bool nk_checkbox_flags_label(struct nk_context *ctx, const char *label,
    unsigned int *flags, unsigned int value)
{
    return nk_checkbox_flags_text(ctx, label, nk_strlen(label), flags, value);
}
/*----------------------------------------------------------------
 *
 *                          OPTION
 *
 * --------------------------------------------------------------*/
NK_API nk_bool
nk_option_text(struct nk_context *ctx, const char *text, int len, nk_bool is_active)
{
    struct nk_window *win;
    struct nk_panel *layout;
    const struct nk_input *in;
    const struct nk_style *style;

    struct nk_rect bounds;
    enum nk_widget_layout_states state;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return is_active;

    win = ctx->current;
    style = &ctx->style;
    layout = win->layout;

    state = nk_widget(&bounds, ctx);
    if (!state) return (int)state;
    in = (state == NK_WIDGET_ROM || state == NK_WIDGET_DISABLED || layout->flags & NK_WINDOW_ROM) ? 0 : &ctx->input;
    nk_do_toggle(&ctx->last_widget_state, &win->buffer, bounds, &is_active,
        text, len, NK_TOGGLE_OPTION, &style->option, in, style->font, NK_WIDGET_LEFT, NK_TEXT_LEFT);
    return is_active;
}
NK_API nk_bool
nk_option_text_align(struct nk_context *ctx, const char *text, int len, nk_bool is_active, nk_flags widget_alignment, nk_flags text_alignment)
{
    struct nk_window *win;
    struct nk_panel *layout;
    const struct nk_input *in;
    const struct nk_style *style;

    struct nk_rect bounds;
    enum nk_widget_layout_states state;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return is_active;

    win = ctx->current;
    style = &ctx->style;
    layout = win->layout;

    state = nk_widget(&bounds, ctx);
    if (!state) return (int)state;
    in = (state == NK_WIDGET_ROM || state == NK_WIDGET_DISABLED || layout->flags & NK_WINDOW_ROM) ? 0 : &ctx->input;
    nk_do_toggle(&ctx->last_widget_state, &win->buffer, bounds, &is_active,
        text, len, NK_TOGGLE_OPTION, &style->option, in, style->font, widget_alignment, text_alignment);
    return is_active;
}
NK_API nk_bool
nk_radio_text(struct nk_context *ctx, const char *text, int len, nk_bool *active)
{
    int old_value;
    NK_ASSERT(ctx);
    NK_ASSERT(text);
    NK_ASSERT(active);
    if (!ctx || !text || !active) return 0;
    old_value = *active;
    *active = nk_option_text(ctx, text, len, old_value);
    return old_value != *active;
}
NK_API nk_bool
nk_radio_text_align(struct nk_context *ctx, const char *text, int len, nk_bool *active, nk_flags widget_alignment, nk_flags text_alignment)
{
    int old_value;
    NK_ASSERT(ctx);
    NK_ASSERT(text);
    NK_ASSERT(active);
    if (!ctx || !text || !active) return 0;
    old_value = *active;
    *active = nk_option_text_align(ctx, text, len, old_value, widget_alignment, text_alignment);
    return old_value != *active;
}
NK_API nk_bool
nk_option_label(struct nk_context *ctx, const char *label, nk_bool active)
{
    return nk_option_text(ctx, label, nk_strlen(label), active);
}
NK_API nk_bool
nk_option_label_align(struct nk_context *ctx, const char *label, nk_bool active, nk_flags widget_alignment, nk_flags text_alignment)
{
    return nk_option_text_align(ctx, label, nk_strlen(label), active, widget_alignment, text_alignment);
}
NK_API nk_bool
nk_radio_label(struct nk_context *ctx, const char *label, nk_bool *active)
{
    return nk_radio_text(ctx, label, nk_strlen(label), active);
}
NK_API nk_bool
nk_radio_label_align(struct nk_context *ctx, const char *label, nk_bool *active, nk_flags widget_alignment, nk_flags text_alignment)
{
    return nk_radio_text_align(ctx, label, nk_strlen(label), active, widget_alignment, text_alignment);
}





/* ===============================================================
 *
 *                              SELECTABLE
 *
 * ===============================================================*/
NK_LIB void
nk_draw_selectable(struct nk_command_buffer *out,
    nk_flags state, const struct nk_style_selectable *style, nk_bool active,
    const struct nk_rect *bounds,
    const struct nk_rect *icon, const struct nk_image *img, enum nk_symbol_type sym,
    const char *string, int len, nk_flags align, const struct nk_user_font *font)
{
    const struct nk_style_item *background;
    struct nk_text text;
    text.padding = style->padding;

    /* select correct colors/images */
    if (!active) {
        if (state & NK_WIDGET_STATE_ACTIVED) {
            background = &style->pressed;
            text.text = style->text_pressed;
        } else if (state & NK_WIDGET_STATE_HOVER) {
            background = &style->hover;
            text.text = style->text_hover;
        } else {
            background = &style->normal;
            text.text = style->text_normal;
        }
    } else {
        if (state & NK_WIDGET_STATE_ACTIVED) {
            background = &style->pressed_active;
            text.text = style->text_pressed_active;
        } else if (state & NK_WIDGET_STATE_HOVER) {
            background = &style->hover_active;
            text.text = style->text_hover_active;
        } else {
            background = &style->normal_active;
            text.text = style->text_normal_active;
        }
    }

    text.text = nk_rgb_factor(text.text, style->color_factor);

    /* draw selectable background and text */
    switch (background->type) {
        case NK_STYLE_ITEM_IMAGE:
            text.background = nk_rgba(0, 0, 0, 0);
            nk_draw_image(out, *bounds, &background->data.image, nk_rgb_factor(nk_white, style->color_factor));
            break;
        case NK_STYLE_ITEM_NINE_SLICE:
            text.background = nk_rgba(0, 0, 0, 0);
            nk_draw_nine_slice(out, *bounds, &background->data.slice, nk_rgb_factor(nk_white, style->color_factor));
            break;
        case NK_STYLE_ITEM_COLOR:
            text.background = background->data.color;
            nk_fill_rect(out, *bounds, style->rounding, background->data.color);
            break;
    }
    if (icon) {
        if (img) nk_draw_image(out, *icon, img, nk_rgb_factor(nk_white, style->color_factor));
        else nk_draw_symbol(out, sym, *icon, text.background, text.text, 1, font);
    }
    nk_widget_text(out, *bounds, string, len, &text, align, font);
}
NK_LIB nk_bool
nk_do_selectable(nk_flags *state, struct nk_command_buffer *out,
    struct nk_rect bounds, const char *str, int len, nk_flags align, nk_bool *value,
    const struct nk_style_selectable *style, const struct nk_input *in,
    const struct nk_user_font *font)
{
    int old_value;
    struct nk_rect touch;

    NK_ASSERT(state);
    NK_ASSERT(out);
    NK_ASSERT(str);
    NK_ASSERT(len);
    NK_ASSERT(value);
    NK_ASSERT(style);
    NK_ASSERT(font);

    if (!state || !out || !str || !len || !value || !style || !font) return 0;
    old_value = *value;

    /* remove padding */
    touch.x = bounds.x - style->touch_padding.x;
    touch.y = bounds.y - style->touch_padding.y;
    touch.w = bounds.w + style->touch_padding.x * 2;
    touch.h = bounds.h + style->touch_padding.y * 2;

    /* update button */
    if (nk_button_behavior(state, touch, in, NK_BUTTON_DEFAULT))
        *value = !(*value);

    /* draw selectable */
    if (style->draw_begin) style->draw_begin(out, style->userdata);
    nk_draw_selectable(out, *state, style, *value, &bounds, 0,0,NK_SYMBOL_NONE, str, len, align, font);
    if (style->draw_end) style->draw_end(out, style->userdata);
    return old_value != *value;
}
NK_LIB nk_bool
nk_do_selectable_image(nk_flags *state, struct nk_command_buffer *out,
    struct nk_rect bounds, const char *str, int len, nk_flags align, nk_bool *value,
    const struct nk_image *img, const struct nk_style_selectable *style,
    const struct nk_input *in, const struct nk_user_font *font)
{
    nk_bool old_value;
    struct nk_rect touch;
    struct nk_rect icon;

    NK_ASSERT(state);
    NK_ASSERT(out);
    NK_ASSERT(str);
    NK_ASSERT(len);
    NK_ASSERT(value);
    NK_ASSERT(style);
    NK_ASSERT(font);

    if (!state || !out || !str || !len || !value || !style || !font) return 0;
    old_value = *value;

    /* toggle behavior */
    touch.x = bounds.x - style->touch_padding.x;
    touch.y = bounds.y - style->touch_padding.y;
    touch.w = bounds.w + style->touch_padding.x * 2;
    touch.h = bounds.h + style->touch_padding.y * 2;
    if (nk_button_behavior(state, touch, in, NK_BUTTON_DEFAULT))
        *value = !(*value);

    icon.y = bounds.y + style->padding.y;
    icon.w = icon.h = bounds.h - 2 * style->padding.y;
    if (align & NK_TEXT_ALIGN_LEFT) {
        icon.x = (bounds.x + bounds.w) - (2 * style->padding.x + icon.w);
        icon.x = NK_MAX(icon.x, 0);
    } else icon.x = bounds.x + 2 * style->padding.x;

    icon.x += style->image_padding.x;
    icon.y += style->image_padding.y;
    icon.w -= 2 * style->image_padding.x;
    icon.h -= 2 * style->image_padding.y;

    /* draw selectable */
    if (style->draw_begin) style->draw_begin(out, style->userdata);
    nk_draw_selectable(out, *state, style, *value, &bounds, &icon, img, NK_SYMBOL_NONE, str, len, align, font);
    if (style->draw_end) style->draw_end(out, style->userdata);
    return old_value != *value;
}
NK_LIB nk_bool
nk_do_selectable_symbol(nk_flags *state, struct nk_command_buffer *out,
    struct nk_rect bounds, const char *str, int len, nk_flags align, nk_bool *value,
    enum nk_symbol_type sym, const struct nk_style_selectable *style,
    const struct nk_input *in, const struct nk_user_font *font)
{
    int old_value;
    struct nk_rect touch;
    struct nk_rect icon;

    NK_ASSERT(state);
    NK_ASSERT(out);
    NK_ASSERT(str);
    NK_ASSERT(len);
    NK_ASSERT(value);
    NK_ASSERT(style);
    NK_ASSERT(font);

    if (!state || !out || !str || !len || !value || !style || !font) return 0;
    old_value = *value;

    /* toggle behavior */
    touch.x = bounds.x - style->touch_padding.x;
    touch.y = bounds.y - style->touch_padding.y;
    touch.w = bounds.w + style->touch_padding.x * 2;
    touch.h = bounds.h + style->touch_padding.y * 2;
    if (nk_button_behavior(state, touch, in, NK_BUTTON_DEFAULT))
        *value = !(*value);

    icon.y = bounds.y + style->padding.y;
    icon.w = icon.h = bounds.h - 2 * style->padding.y;
    if (align & NK_TEXT_ALIGN_LEFT) {
        icon.x = (bounds.x + bounds.w) - (2 * style->padding.x + icon.w);
        icon.x = NK_MAX(icon.x, 0);
    } else icon.x = bounds.x + 2 * style->padding.x;

    icon.x += style->image_padding.x;
    icon.y += style->image_padding.y;
    icon.w -= 2 * style->image_padding.x;
    icon.h -= 2 * style->image_padding.y;

    /* draw selectable */
    if (style->draw_begin) style->draw_begin(out, style->userdata);
    nk_draw_selectable(out, *state, style, *value, &bounds, &icon, 0, sym, str, len, align, font);
    if (style->draw_end) style->draw_end(out, style->userdata);
    return old_value != *value;
}

NK_API nk_bool
nk_selectable_text(struct nk_context *ctx, const char *str, int len,
    nk_flags align, nk_bool *value)
{
    struct nk_window *win;
    struct nk_panel *layout;
    const struct nk_input *in;
    const struct nk_style *style;

    enum nk_widget_layout_states state;
    struct nk_rect bounds;

    NK_ASSERT(ctx);
    NK_ASSERT(value);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout || !value)
        return 0;

    win = ctx->current;
    layout = win->layout;
    style = &ctx->style;

    state = nk_widget(&bounds, ctx);
    if (!state) return 0;
    in = (state == NK_WIDGET_ROM || state == NK_WIDGET_DISABLED || layout->flags & NK_WINDOW_ROM) ? 0 : &ctx->input;
    return nk_do_selectable(&ctx->last_widget_state, &win->buffer, bounds,
                str, len, align, value, &style->selectable, in, style->font);
}
NK_API nk_bool
nk_selectable_image_text(struct nk_context *ctx, struct nk_image img,
    const char *str, int len, nk_flags align, nk_bool *value)
{
    struct nk_window *win;
    struct nk_panel *layout;
    const struct nk_input *in;
    const struct nk_style *style;

    enum nk_widget_layout_states state;
    struct nk_rect bounds;

    NK_ASSERT(ctx);
    NK_ASSERT(value);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout || !value)
        return 0;

    win = ctx->current;
    layout = win->layout;
    style = &ctx->style;

    state = nk_widget(&bounds, ctx);
    if (!state) return 0;
    in = (state == NK_WIDGET_ROM || state == NK_WIDGET_DISABLED || layout->flags & NK_WINDOW_ROM) ? 0 : &ctx->input;
    return nk_do_selectable_image(&ctx->last_widget_state, &win->buffer, bounds,
                str, len, align, value, &img, &style->selectable, in, style->font);
}
NK_API nk_bool
nk_selectable_symbol_text(struct nk_context *ctx, enum nk_symbol_type sym,
    const char *str, int len, nk_flags align, nk_bool *value)
{
    struct nk_window *win;
    struct nk_panel *layout;
    const struct nk_input *in;
    const struct nk_style *style;

    enum nk_widget_layout_states state;
    struct nk_rect bounds;

    NK_ASSERT(ctx);
    NK_ASSERT(value);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout || !value)
        return 0;

    win = ctx->current;
    layout = win->layout;
    style = &ctx->style;

    state = nk_widget(&bounds, ctx);
    if (!state) return 0;
    in = (state == NK_WIDGET_ROM || state == NK_WIDGET_DISABLED || layout->flags & NK_WINDOW_ROM) ? 0 : &ctx->input;
    return nk_do_selectable_symbol(&ctx->last_widget_state, &win->buffer, bounds,
                str, len, align, value, sym, &style->selectable, in, style->font);
}
NK_API nk_bool
nk_selectable_symbol_label(struct nk_context *ctx, enum nk_symbol_type sym,
    const char *title, nk_flags align, nk_bool *value)
{
    return nk_selectable_symbol_text(ctx, sym, title, nk_strlen(title), align, value);
}
NK_API nk_bool nk_select_text(struct nk_context *ctx, const char *str, int len,
    nk_flags align, nk_bool value)
{
    nk_selectable_text(ctx, str, len, align, &value);return value;
}
NK_API nk_bool nk_selectable_label(struct nk_context *ctx, const char *str, nk_flags align, nk_bool *value)
{
    return nk_selectable_text(ctx, str, nk_strlen(str), align, value);
}
NK_API nk_bool nk_selectable_image_label(struct nk_context *ctx,struct nk_image img,
    const char *str, nk_flags align, nk_bool *value)
{
    return nk_selectable_image_text(ctx, img, str, nk_strlen(str), align, value);
}
NK_API nk_bool nk_select_label(struct nk_context *ctx, const char *str, nk_flags align, nk_bool value)
{
    nk_selectable_text(ctx, str, nk_strlen(str), align, &value);return value;
}
NK_API nk_bool nk_select_image_label(struct nk_context *ctx, struct nk_image img,
    const char *str, nk_flags align, nk_bool value)
{
    nk_selectable_image_text(ctx, img, str, nk_strlen(str), align, &value);return value;
}
NK_API nk_bool nk_select_image_text(struct nk_context *ctx, struct nk_image img,
    const char *str, int len, nk_flags align, nk_bool value)
{
    nk_selectable_image_text(ctx, img, str, len, align, &value);return value;
}
NK_API nk_bool
nk_select_symbol_text(struct nk_context *ctx, enum nk_symbol_type sym,
    const char *title, int title_len, nk_flags align, nk_bool value)
{
    nk_selectable_symbol_text(ctx, sym, title, title_len, align, &value);return value;
}
NK_API nk_bool
nk_select_symbol_label(struct nk_context *ctx, enum nk_symbol_type sym,
    const char *title, nk_flags align, nk_bool value)
{
    return nk_select_symbol_text(ctx, sym, title, nk_strlen(title), align, value);
}





/* ===============================================================
 *
 *                              SLIDER
 *
 * ===============================================================*/
NK_LIB float
nk_slider_behavior(nk_flags *state, struct nk_rect *logical_cursor,
    struct nk_rect *visual_cursor, struct nk_input *in,
    struct nk_rect bounds, float slider_min, float slider_max, float slider_value,
    float slider_step, float slider_steps)
{
    int left_mouse_down;
    int left_mouse_click_in_cursor;

    /* check if visual cursor is being dragged */
    nk_widget_state_reset(state);
    left_mouse_down = in && in->mouse.buttons[NK_BUTTON_LEFT].down;
    left_mouse_click_in_cursor = in && nk_input_has_mouse_click_down_in_rect(in,
            NK_BUTTON_LEFT, *visual_cursor, nk_true);

    if (left_mouse_down && left_mouse_click_in_cursor) {
        float ratio = 0;
        const float d = in->mouse.pos.x - (visual_cursor->x+visual_cursor->w*0.5f);
        const float pxstep = bounds.w / slider_steps;

        /* only update value if the next slider step is reached */
        *state = NK_WIDGET_STATE_ACTIVE;
        if (NK_ABS(d) >= pxstep) {
            const float steps = (float)((int)(NK_ABS(d) / pxstep));
            slider_value += (d > 0) ? (slider_step*steps) : -(slider_step*steps);
            slider_value = NK_CLAMP(slider_min, slider_value, slider_max);
            ratio = (slider_value - slider_min)/slider_step;
            logical_cursor->x = bounds.x + (logical_cursor->w * ratio);
            in->mouse.buttons[NK_BUTTON_LEFT].clicked_pos.x = logical_cursor->x;
        }
    }

    /* slider widget state */
    if (nk_input_is_mouse_hovering_rect(in, bounds))
        *state = NK_WIDGET_STATE_HOVERED;
    if (*state & NK_WIDGET_STATE_HOVER &&
        !nk_input_is_mouse_prev_hovering_rect(in, bounds))
        *state |= NK_WIDGET_STATE_ENTERED;
    else if (nk_input_is_mouse_prev_hovering_rect(in, bounds))
        *state |= NK_WIDGET_STATE_LEFT;
    return slider_value;
}
NK_LIB void
nk_draw_slider(struct nk_command_buffer *out, nk_flags state,
    const struct nk_style_slider *style, const struct nk_rect *bounds,
    const struct nk_rect *visual_cursor, float min, float value, float max)
{
    struct nk_rect fill;
    struct nk_rect bar;
    const struct nk_style_item *background;

    /* select correct slider images/colors */
    struct nk_color bar_color;
    const struct nk_style_item *cursor;

    NK_UNUSED(min);
    NK_UNUSED(max);
    NK_UNUSED(value);

    if (state & NK_WIDGET_STATE_ACTIVED) {
        background = &style->active;
        bar_color = style->bar_active;
        cursor = &style->cursor_active;
    } else if (state & NK_WIDGET_STATE_HOVER) {
        background = &style->hover;
        bar_color = style->bar_hover;
        cursor = &style->cursor_hover;
    } else {
        background = &style->normal;
        bar_color = style->bar_normal;
        cursor = &style->cursor_normal;
    }

    /* calculate slider background bar */
    bar.x = bounds->x;
    bar.y = (visual_cursor->y + visual_cursor->h/2) - bounds->h/12;
    bar.w = bounds->w;
    bar.h = bounds->h/6;

    /* filled background bar style */
    fill.w = (visual_cursor->x + (visual_cursor->w/2.0f)) - bar.x;
    fill.x = bar.x;
    fill.y = bar.y;
    fill.h = bar.h;

    /* draw background */
    switch(background->type) {
        case NK_STYLE_ITEM_IMAGE:
            nk_draw_image(out, *bounds, &background->data.image, nk_rgb_factor(nk_white, style->color_factor));
            break;
        case NK_STYLE_ITEM_NINE_SLICE:
            nk_draw_nine_slice(out, *bounds, &background->data.slice, nk_rgb_factor(nk_white, style->color_factor));
            break;
        case NK_STYLE_ITEM_COLOR:
            nk_fill_rect(out, *bounds, style->rounding, nk_rgb_factor(background->data.color, style->color_factor));
            nk_stroke_rect(out, *bounds, style->rounding, style->border, nk_rgb_factor(style->border_color, style->color_factor));
            break;
    }

    /* draw slider bar */
    nk_fill_rect(out, bar, style->rounding, nk_rgb_factor(bar_color, style->color_factor));
    nk_fill_rect(out, fill, style->rounding, nk_rgb_factor(style->bar_filled, style->color_factor));

    /* draw cursor */
    if (cursor->type == NK_STYLE_ITEM_IMAGE)
        nk_draw_image(out, *visual_cursor, &cursor->data.image, nk_rgb_factor(nk_white, style->color_factor));
    else
        nk_fill_circle(out, *visual_cursor, nk_rgb_factor(cursor->data.color, style->color_factor));
}
NK_LIB float
nk_do_slider(nk_flags *state,
    struct nk_command_buffer *out, struct nk_rect bounds,
    float min, float val, float max, float step,
    const struct nk_style_slider *style, struct nk_input *in,
    const struct nk_user_font *font)
{
    float slider_range;
    float slider_min;
    float slider_max;
    float slider_value;
    float slider_steps;
    float cursor_offset;

    struct nk_rect visual_cursor;
    struct nk_rect logical_cursor;

    NK_ASSERT(style);
    NK_ASSERT(out);
    if (!out || !style)
        return 0;

    /* remove padding from slider bounds */
    bounds.x = bounds.x + style->padding.x;
    bounds.y = bounds.y + style->padding.y;
    bounds.h = NK_MAX(bounds.h, 2*style->padding.y);
    bounds.w = NK_MAX(bounds.w, 2*style->padding.x + style->cursor_size.x);
    bounds.w -= 2 * style->padding.x;
    bounds.h -= 2 * style->padding.y;

    /* optional buttons */
    if (style->show_buttons) {
        nk_flags ws;
        struct nk_rect button;
        button.y = bounds.y;
        button.w = bounds.h;
        button.h = bounds.h;

        /* decrement button */
        button.x = bounds.x;
        if (nk_do_button_symbol(&ws, out, button, style->dec_symbol, NK_BUTTON_DEFAULT,
            &style->dec_button, in, font))
            val -= step;

        /* increment button */
        button.x = (bounds.x + bounds.w) - button.w;
        if (nk_do_button_symbol(&ws, out, button, style->inc_symbol, NK_BUTTON_DEFAULT,
            &style->inc_button, in, font))
            val += step;

        bounds.x = bounds.x + button.w + style->spacing.x;
        bounds.w = bounds.w - (2*button.w + 2*style->spacing.x);
    }

    /* remove one cursor size to support visual cursor */
    bounds.x += style->cursor_size.x*0.5f;
    bounds.w -= style->cursor_size.x;

    /* make sure the provided values are correct */
    slider_max = NK_MAX(min, max);
    slider_min = NK_MIN(min, max);
    slider_value = NK_CLAMP(slider_min, val, slider_max);
    slider_range = slider_max - slider_min;
    slider_steps = slider_range / step;
    cursor_offset = (slider_value - slider_min) / step;

    /* calculate cursor
    Basically you have two cursors. One for visual representation and interaction
    and one for updating the actual cursor value. */
    logical_cursor.h = bounds.h;
    logical_cursor.w = bounds.w / slider_steps;
    logical_cursor.x = bounds.x + (logical_cursor.w * cursor_offset);
    logical_cursor.y = bounds.y;

    visual_cursor.h = style->cursor_size.y;
    visual_cursor.w = style->cursor_size.x;
    visual_cursor.y = (bounds.y + bounds.h*0.5f) - visual_cursor.h*0.5f;
    visual_cursor.x = logical_cursor.x - visual_cursor.w*0.5f;

    slider_value = nk_slider_behavior(state, &logical_cursor, &visual_cursor,
        in, bounds, slider_min, slider_max, slider_value, step, slider_steps);
    visual_cursor.x = logical_cursor.x - visual_cursor.w*0.5f;

    /* draw slider */
    if (style->draw_begin) style->draw_begin(out, style->userdata);
    nk_draw_slider(out, *state, style, &bounds, &visual_cursor, slider_min, slider_value, slider_max);
    if (style->draw_end) style->draw_end(out, style->userdata);
    return slider_value;
}
NK_API nk_bool
nk_slider_float(struct nk_context *ctx, float min_value, float *value, float max_value,
    float value_step)
{
    struct nk_window *win;
    struct nk_panel *layout;
    struct nk_input *in;
    const struct nk_style *style;

    int ret = 0;
    float old_value;
    struct nk_rect bounds;
    enum nk_widget_layout_states state;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    NK_ASSERT(value);
    if (!ctx || !ctx->current || !ctx->current->layout || !value)
        return ret;

    win = ctx->current;
    style = &ctx->style;
    layout = win->layout;

    state = nk_widget(&bounds, ctx);
    if (!state) return ret;
    in = (/*state == NK_WIDGET_ROM || */ state == NK_WIDGET_DISABLED || layout->flags & NK_WINDOW_ROM) ? 0 : &ctx->input;

    old_value = *value;
    *value = nk_do_slider(&ctx->last_widget_state, &win->buffer, bounds, min_value,
                old_value, max_value, value_step, &style->slider, in, style->font);
    return (old_value > *value || old_value < *value);
}
NK_API float
nk_slide_float(struct nk_context *ctx, float min, float val, float max, float step)
{
    nk_slider_float(ctx, min, &val, max, step); return val;
}
NK_API int
nk_slide_int(struct nk_context *ctx, int min, int val, int max, int step)
{
    float value = (float)val;
    nk_slider_float(ctx, (float)min, &value, (float)max, (float)step);
    return (int)value;
}
NK_API nk_bool
nk_slider_int(struct nk_context *ctx, int min, int *val, int max, int step)
{
    int ret;
    float value = (float)*val;
    ret = nk_slider_float(ctx, (float)min, &value, (float)max, (float)step);
    *val =  (int)value;
    return ret;
}





/* ===============================================================
 *
 *                               KNOB
 *
 * ===============================================================*/

NK_LIB float
nk_knob_behavior(nk_flags *state, struct nk_input *in,
    struct nk_rect bounds, float knob_min, float knob_max, float knob_value,
    float knob_step, float knob_steps,
    enum nk_heading zero_direction, float dead_zone_percent)
{
    struct nk_vec2 origin;
    float angle = 0.0f;
    origin.x = bounds.x + (bounds.w / 2);
    origin.y = bounds.y + (bounds.h / 2);

    nk_widget_state_reset(state);

    /* handle click and drag input */
    if(in &&
       in->mouse.buttons[NK_BUTTON_LEFT].down &&
       nk_input_has_mouse_click_down_in_rect(in, NK_BUTTON_LEFT, bounds, nk_true)){
        /* calculate angle from origin and rotate */
        const float direction_rads[4] = {
            NK_PI * 2.5f, /* 90  NK_UP */
            NK_PI * 2.0f, /* 0   NK_RIGHT */
            NK_PI * 1.5f, /* 270 NK_DOWN */
            NK_PI,        /* 180 NK_LEFT */
        };
        *state = NK_WIDGET_STATE_ACTIVE;

        angle = NK_ATAN2(in->mouse.pos.y - origin.y, in->mouse.pos.x - origin.x) + direction_rads[zero_direction];
        angle -= (angle > NK_PI * 2) ? NK_PI * 3 : NK_PI;

        /* account for dead space applied when drawing */
        angle *= 1.0f / (1.0f - dead_zone_percent);
        angle = NK_CLAMP(-NK_PI, angle, NK_PI);

        /* convert -pi -> pi range to 0.0 -> 1.0 */
        angle = (angle + NK_PI) / (NK_PI * 2);

        float dx = in->mouse.pos.x - origin.x, dy = in->mouse.pos.y - origin.y;
        if(dx*dx+dy*dy < bounds.w*bounds.w * 25.0)
        { /* click to closest step only if mouse pointer is close */
          if(dx*dx+dy*dy > bounds.w*bounds.w)
            knob_value = knob_min + ( (int)(angle * knob_steps + (knob_step / 2)) ) * knob_step;
        }
        else knob_value = knob_min + ( angle * knob_steps + (knob_step / 2.0f) ) * knob_step;
        knob_value = NK_CLAMP(knob_min, knob_value, knob_max);
    }

    /* knob widget state */
    if (nk_input_is_mouse_hovering_rect(in, bounds)){
        *state = NK_WIDGET_STATE_HOVERED;
        if (in) {
            /* handle scroll and arrow inputs */
            if (in->mouse.scroll_delta.y > 0 ||
               (in->keyboard.keys[NK_KEY_UP].down && in->keyboard.keys[NK_KEY_UP].clicked))
                knob_value += knob_step;

            if (in->mouse.scroll_delta.y < 0 ||
               (in->keyboard.keys[NK_KEY_DOWN].down && in->keyboard.keys[NK_KEY_DOWN].clicked))
                knob_value -= knob_step;
        }
        knob_value = NK_CLAMP(knob_min, knob_value, knob_max);
    }
    if (*state & NK_WIDGET_STATE_HOVER &&
        !nk_input_is_mouse_prev_hovering_rect(in, bounds))
        *state |= NK_WIDGET_STATE_ENTERED;
    else if (nk_input_is_mouse_prev_hovering_rect(in, bounds))
        *state |= NK_WIDGET_STATE_LEFT;

    return knob_value;
}
NK_LIB void
nk_draw_knob(struct nk_command_buffer *out, nk_flags state,
    const struct nk_style_knob *style, const struct nk_rect *bounds, float min, float value, float max,
    enum nk_heading zero_direction, float dead_zone_percent)
{
    const struct nk_style_item *background;
    struct nk_color knob_color, cursor;

    NK_UNUSED(min);
    NK_UNUSED(max);
    NK_UNUSED(value);

    if (state & NK_WIDGET_STATE_ACTIVED) {
        background = &style->active;
        knob_color = style->knob_active;
        cursor = style->cursor_active;
    } else if (state & NK_WIDGET_STATE_HOVER) {
        background = &style->hover;
        knob_color = style->knob_hover;
        cursor = style->cursor_hover;
    } else {
        background = &style->normal;
        knob_color = style->knob_normal;
        cursor = style->cursor_normal;
    }

    /* draw background */
    switch(background->type) {
        case NK_STYLE_ITEM_IMAGE:
            nk_draw_image(out, *bounds, &background->data.image, nk_rgb_factor(nk_white, style->color_factor));
            break;
        case NK_STYLE_ITEM_NINE_SLICE:
            nk_draw_nine_slice(out, *bounds, &background->data.slice, nk_rgb_factor(nk_white, style->color_factor));
            break;
        case NK_STYLE_ITEM_COLOR:
            nk_fill_rect(out, *bounds, 0, nk_rgb_factor(background->data.color, style->color_factor));
            nk_stroke_rect(out, *bounds, 0, style->border, nk_rgb_factor(style->border_color, style->color_factor));
            break;
    }

    /* draw knob */
    nk_fill_circle(out, *bounds, nk_rgb_factor(knob_color, style->color_factor));
    if(style->knob_border > 0){
        struct nk_rect border_bounds = *bounds;
        border_bounds.x += style->knob_border / 2;
        border_bounds.y += style->knob_border / 2;
        border_bounds.w -= style->knob_border;
        border_bounds.h -= style->knob_border;
        nk_stroke_circle(out, border_bounds, style->knob_border, nk_rgb_factor(style->knob_border_color, style->color_factor));
    }
    { /* calculate cursor line cords */
    float half_circle_size = (bounds->w / 2);
    float angle = (value - min) / (max - min);
    float alive_zone =  1.0f - dead_zone_percent;
    struct nk_vec2 cursor_start, cursor_end;
    const float direction_rads[4] = {
        NK_PI * 1.5f, /* 90  NK_UP */
        0.0f,         /* 0   NK_RIGHT */
        NK_PI * 0.5f, /* 270 NK_DOWN */
        NK_PI,        /* 180 NK_LEFT */
    };
    /* calculate + apply dead zone */
    angle = (angle * alive_zone) + (dead_zone_percent / 2);

    /* percentage 0.0 -> 1.0 to radians, rads are 0.0 to (2*pi) NOT -pi to pi */
    angle *= NK_PI * 2;

    /* apply zero angle */
    angle += direction_rads[zero_direction];
    if(angle > NK_PI * 2)
        angle -= NK_PI * 2;

    cursor_start.x = bounds->x + half_circle_size + (angle > NK_PI);
    cursor_start.y = bounds->y + half_circle_size + (angle < NK_PI_HALF || angle > (NK_PI * 1.5f));

    cursor_end.x = cursor_start.x + (half_circle_size * NK_COS(angle));
    cursor_end.y = cursor_start.y + (half_circle_size * NK_SIN(angle));

    /* cut off half of the cursor */
    cursor_start.x = (cursor_start.x + cursor_end.x) / 2;
    cursor_start.y = (cursor_start.y + cursor_end.y) / 2;

    /* draw cursor */
    nk_stroke_line(out, cursor_start.x, cursor_start.y, cursor_end.x, cursor_end.y, 0.3*half_circle_size, nk_rgb_factor(cursor, style->color_factor));
    }
}
NK_LIB float
nk_do_knob(nk_flags *state,
    struct nk_command_buffer *out, struct nk_rect bounds,
    float min, float val, float max, float step,
    enum nk_heading zero_direction, float dead_zone_percent,
    const struct nk_style_knob *style, struct nk_input *in)
{
    float knob_range;
    float knob_min;
    float knob_max;
    float knob_value;
    float knob_steps;

    NK_ASSERT(style);
    NK_ASSERT(out);
    if (!out || !style)
        return 0;

    /* remove padding from knob bounds */
    bounds.y = bounds.y + style->padding.y;
    bounds.x = bounds.x + style->padding.x;
    bounds.h = NK_MAX(bounds.h, 2*style->padding.y);
    bounds.w = NK_MAX(bounds.w, 2*style->padding.x);
    bounds.w -= 2 * style->padding.x;
    bounds.h -= 2 * style->padding.y;
    if(bounds.h < bounds.w){
        bounds.x += (bounds.w - bounds.h) / 2;
        bounds.w = bounds.h;
    }

    /* make sure the provided values are correct */
    knob_max = NK_MAX(min, max);
    knob_min = NK_MIN(min, max);
    knob_value = NK_CLAMP(knob_min, val, knob_max);
    knob_range = knob_max - knob_min;
    knob_steps = knob_range / step;

    knob_value = nk_knob_behavior(state, in, bounds, knob_min, knob_max, knob_value, step, knob_steps, zero_direction, dead_zone_percent);

    /* draw knob */
    if (style->draw_begin) style->draw_begin(out, style->userdata);
    nk_draw_knob(out, *state, style, &bounds, knob_min, knob_value, knob_max, zero_direction, dead_zone_percent);
    if (style->draw_end) style->draw_end(out, style->userdata);
    return knob_value;
}
NK_API nk_bool
nk_knob_float(struct nk_context *ctx, float min_value, float *value, float max_value,
    float value_step, enum nk_heading zero_direction, float dead_zone_degrees)
{
    struct nk_window *win;
    struct nk_panel *layout;
    struct nk_input *in;
    const struct nk_style *style;

    int ret = 0;
    float old_value;
    struct nk_rect bounds;
    enum nk_widget_layout_states state;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    NK_ASSERT(value);
    NK_ASSERT(NK_BETWEEN(dead_zone_degrees, 0.0f, 360.0f));
    if (!ctx || !ctx->current || !ctx->current->layout || !value)
        return ret;

    win = ctx->current;
    style = &ctx->style;
    layout = win->layout;

    state = nk_widget(&bounds, ctx);
    if (!state) return ret;
    in = (state == NK_WIDGET_DISABLED || layout->flags & NK_WINDOW_ROM) ? 0 : &ctx->input;

    old_value = *value;
    *value = nk_do_knob(&ctx->last_widget_state, &win->buffer, bounds, min_value,
                old_value, max_value, value_step, zero_direction, dead_zone_degrees / 360.0f, &style->knob, in);

    return (old_value > *value || old_value < *value);
}
NK_API nk_bool
nk_knob_int(struct nk_context *ctx, int min, int *val, int max, int step,
    enum nk_heading zero_direction, float dead_zone_degrees)
{
    int ret;
    float value = (float)*val;
    ret = nk_knob_float(ctx, (float)min, &value, (float)max, (float)step, zero_direction, dead_zone_degrees);
    *val =  (int)value;
    return ret;
}




/* ===============================================================
 *
 *                          PROGRESS
 *
 * ===============================================================*/
NK_LIB nk_size
nk_progress_behavior(nk_flags *state, struct nk_input *in,
    struct nk_rect r, struct nk_rect cursor, nk_size max, nk_size value, nk_bool modifiable)
{
    int left_mouse_down = 0;
    int left_mouse_click_in_cursor = 0;

    nk_widget_state_reset(state);
    if (!in || !modifiable) return value;
    left_mouse_down = in && in->mouse.buttons[NK_BUTTON_LEFT].down;
    left_mouse_click_in_cursor = in && nk_input_has_mouse_click_down_in_rect(in,
            NK_BUTTON_LEFT, cursor, nk_true);
    if (nk_input_is_mouse_hovering_rect(in, r))
        *state = NK_WIDGET_STATE_HOVERED;

    if (in && left_mouse_down && left_mouse_click_in_cursor) {
        if (left_mouse_down && left_mouse_click_in_cursor) {
            float ratio = NK_MAX(0, (float)(in->mouse.pos.x - cursor.x)) / (float)cursor.w;
            value = (nk_size)NK_CLAMP(0, (float)max * ratio, (float)max);
            in->mouse.buttons[NK_BUTTON_LEFT].clicked_pos.x = cursor.x + cursor.w/2.0f;
            *state |= NK_WIDGET_STATE_ACTIVE;
        }
    }
    /* set progressbar widget state */
    if (*state & NK_WIDGET_STATE_HOVER && !nk_input_is_mouse_prev_hovering_rect(in, r))
        *state |= NK_WIDGET_STATE_ENTERED;
    else if (nk_input_is_mouse_prev_hovering_rect(in, r))
        *state |= NK_WIDGET_STATE_LEFT;
    return value;
}
NK_LIB void
nk_draw_progress(struct nk_command_buffer *out, nk_flags state,
    const struct nk_style_progress *style, const struct nk_rect *bounds,
    const struct nk_rect *scursor, nk_size value, nk_size max)
{
    const struct nk_style_item *background;
    const struct nk_style_item *cursor;

    NK_UNUSED(max);
    NK_UNUSED(value);

    /* select correct colors/images to draw */
    if (state & NK_WIDGET_STATE_ACTIVED) {
        background = &style->active;
        cursor = &style->cursor_active;
    } else if (state & NK_WIDGET_STATE_HOVER){
        background = &style->hover;
        cursor = &style->cursor_hover;
    } else {
        background = &style->normal;
        cursor = &style->cursor_normal;
    }

    /* draw background */
    switch(background->type) {
        case NK_STYLE_ITEM_IMAGE:
            nk_draw_image(out, *bounds, &background->data.image, nk_rgb_factor(nk_white, style->color_factor));
            break;
        case NK_STYLE_ITEM_NINE_SLICE:
            nk_draw_nine_slice(out, *bounds, &background->data.slice, nk_rgb_factor(nk_white, style->color_factor));
            break;
        case NK_STYLE_ITEM_COLOR:
            nk_fill_rect(out, *bounds, style->rounding, nk_rgb_factor(background->data.color, style->color_factor));
            nk_stroke_rect(out, *bounds, style->rounding, style->border, nk_rgb_factor(style->border_color, style->color_factor));
            break;
    }

    /* draw cursor */
    switch(cursor->type) {
        case NK_STYLE_ITEM_IMAGE:
            nk_draw_image(out, *scursor, &cursor->data.image, nk_rgb_factor(nk_white, style->color_factor));
            break;
        case NK_STYLE_ITEM_NINE_SLICE:
            nk_draw_nine_slice(out, *scursor, &cursor->data.slice, nk_rgb_factor(nk_white, style->color_factor));
            break;
        case NK_STYLE_ITEM_COLOR:
            nk_fill_rect(out, *scursor, style->rounding, nk_rgb_factor(cursor->data.color, style->color_factor));
            nk_stroke_rect(out, *scursor, style->rounding, style->border, nk_rgb_factor(style->border_color, style->color_factor));
            break;
    }
}
NK_LIB nk_size
nk_do_progress(nk_flags *state,
    struct nk_command_buffer *out, struct nk_rect bounds,
    nk_size value, nk_size max, nk_bool modifiable,
    const struct nk_style_progress *style, struct nk_input *in)
{
    float prog_scale;
    nk_size prog_value;
    struct nk_rect cursor;

    NK_ASSERT(style);
    NK_ASSERT(out);
    if (!out || !style) return 0;

    /* calculate progressbar cursor */
    cursor.w = NK_MAX(bounds.w, 2 * style->padding.x + 2 * style->border);
    cursor.h = NK_MAX(bounds.h, 2 * style->padding.y + 2 * style->border);
    cursor = nk_pad_rect(bounds, nk_vec2(style->padding.x + style->border, style->padding.y + style->border));
    prog_scale = (float)value / (float)max;

    /* update progressbar */
    prog_value = NK_MIN(value, max);
    prog_value = nk_progress_behavior(state, in, bounds, cursor,max, prog_value, modifiable);
    cursor.w = cursor.w * prog_scale;

    /* draw progressbar */
    if (style->draw_begin) style->draw_begin(out, style->userdata);
    nk_draw_progress(out, *state, style, &bounds, &cursor, value, max);
    if (style->draw_end) style->draw_end(out, style->userdata);
    return prog_value;
}
NK_API nk_bool
nk_progress(struct nk_context *ctx, nk_size *cur, nk_size max, nk_bool is_modifyable)
{
    struct nk_window *win;
    struct nk_panel *layout;
    const struct nk_style *style;
    struct nk_input *in;

    struct nk_rect bounds;
    enum nk_widget_layout_states state;
    nk_size old_value;

    NK_ASSERT(ctx);
    NK_ASSERT(cur);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout || !cur)
        return 0;

    win = ctx->current;
    style = &ctx->style;
    layout = win->layout;
    state = nk_widget(&bounds, ctx);
    if (!state) return 0;

    in = (state == NK_WIDGET_ROM || state == NK_WIDGET_DISABLED || layout->flags & NK_WINDOW_ROM) ? 0 : &ctx->input;
    old_value = *cur;
    *cur = nk_do_progress(&ctx->last_widget_state, &win->buffer, bounds,
            *cur, max, is_modifyable, &style->progress, in);
    return (*cur != old_value);
}
NK_API nk_size
nk_prog(struct nk_context *ctx, nk_size cur, nk_size max, nk_bool modifyable)
{
    nk_progress(ctx, &cur, max, modifyable);
    return cur;
}





/* ===============================================================
 *
 *                              SCROLLBAR
 *
 * ===============================================================*/
NK_LIB float
nk_scrollbar_behavior(nk_flags *state, struct nk_input *in,
    int has_scrolling, const struct nk_rect *scroll,
    const struct nk_rect *cursor, const struct nk_rect *empty0,
    const struct nk_rect *empty1, float scroll_offset,
    float target, float scroll_step, enum nk_orientation o)
{
    nk_flags ws = 0;
    int left_mouse_down;
    unsigned int left_mouse_clicked;
    int left_mouse_click_in_cursor;
    float scroll_delta;

    nk_widget_state_reset(state);
    if (!in) return scroll_offset;

    left_mouse_down = in->mouse.buttons[NK_BUTTON_LEFT].down;
    left_mouse_clicked = in->mouse.buttons[NK_BUTTON_LEFT].clicked;
    left_mouse_click_in_cursor = nk_input_has_mouse_click_down_in_rect(in,
        NK_BUTTON_LEFT, *cursor, nk_true);
    if (nk_input_is_mouse_hovering_rect(in, *scroll))
        *state = NK_WIDGET_STATE_HOVERED;

    scroll_delta = (o == NK_VERTICAL) ? in->mouse.scroll_delta.y: in->mouse.scroll_delta.x;
    if (left_mouse_down && left_mouse_click_in_cursor && !left_mouse_clicked) {
        /* update cursor by mouse dragging */
        float pixel, delta;
        *state = NK_WIDGET_STATE_ACTIVE;
        if (o == NK_VERTICAL) {
            float cursor_y;
            pixel = in->mouse.delta.y;
            delta = (pixel / scroll->h) * target;
            scroll_offset = NK_CLAMP(0, scroll_offset + delta, target - scroll->h);
            cursor_y = scroll->y + ((scroll_offset/target) * scroll->h);
            in->mouse.buttons[NK_BUTTON_LEFT].clicked_pos.y = cursor_y + cursor->h/2.0f;
        } else {
            float cursor_x;
            pixel = in->mouse.delta.x;
            delta = (pixel / scroll->w) * target;
            scroll_offset = NK_CLAMP(0, scroll_offset + delta, target - scroll->w);
            cursor_x = scroll->x + ((scroll_offset/target) * scroll->w);
            in->mouse.buttons[NK_BUTTON_LEFT].clicked_pos.x = cursor_x + cursor->w/2.0f;
        }
    } else if ((nk_input_is_key_pressed(in, NK_KEY_SCROLL_UP) && o == NK_VERTICAL && has_scrolling)||
            nk_button_behavior(&ws, *empty0, in, NK_BUTTON_DEFAULT)) {
        /* scroll page up by click on empty space or shortcut */
        if (o == NK_VERTICAL)
            scroll_offset = NK_MAX(0, scroll_offset - scroll->h);
        else scroll_offset = NK_MAX(0, scroll_offset - scroll->w);
    } else if ((nk_input_is_key_pressed(in, NK_KEY_SCROLL_DOWN) && o == NK_VERTICAL && has_scrolling) ||
        nk_button_behavior(&ws, *empty1, in, NK_BUTTON_DEFAULT)) {
        /* scroll page down by click on empty space or shortcut */
        if (o == NK_VERTICAL)
            scroll_offset = NK_MIN(scroll_offset + scroll->h, target - scroll->h);
        else scroll_offset = NK_MIN(scroll_offset + scroll->w, target - scroll->w);
    } else if (has_scrolling) {
        if ((scroll_delta < 0 || (scroll_delta > 0))) {
            /* update cursor by mouse scrolling */
            scroll_offset = scroll_offset + scroll_step * (-scroll_delta);
            if (o == NK_VERTICAL)
                scroll_offset = NK_CLAMP(0, scroll_offset, target - scroll->h);
            else scroll_offset = NK_CLAMP(0, scroll_offset, target - scroll->w);
        } else if (nk_input_is_key_pressed(in, NK_KEY_SCROLL_START)) {
            /* update cursor to the beginning  */
            if (o == NK_VERTICAL) scroll_offset = 0;
        } else if (nk_input_is_key_pressed(in, NK_KEY_SCROLL_END)) {
            /* update cursor to the end */
            if (o == NK_VERTICAL) scroll_offset = target - scroll->h;
        }
    }
    if (*state & NK_WIDGET_STATE_HOVER && !nk_input_is_mouse_prev_hovering_rect(in, *scroll))
        *state |= NK_WIDGET_STATE_ENTERED;
    else if (nk_input_is_mouse_prev_hovering_rect(in, *scroll))
        *state |= NK_WIDGET_STATE_LEFT;
    return scroll_offset;
}
NK_LIB void
nk_draw_scrollbar(struct nk_command_buffer *out, nk_flags state,
    const struct nk_style_scrollbar *style, const struct nk_rect *bounds,
    const struct nk_rect *scroll)
{
    const struct nk_style_item *background;
    const struct nk_style_item *cursor;

    /* select correct colors/images to draw */
    if (state & NK_WIDGET_STATE_ACTIVED) {
        background = &style->active;
        cursor = &style->cursor_active;
    } else if (state & NK_WIDGET_STATE_HOVER) {
        background = &style->hover;
        cursor = &style->cursor_hover;
    } else {
        background = &style->normal;
        cursor = &style->cursor_normal;
    }

    /* draw background */
    switch (background->type) {
        case NK_STYLE_ITEM_IMAGE:
            nk_draw_image(out, *bounds, &background->data.image, nk_white);
            break;
        case NK_STYLE_ITEM_NINE_SLICE:
            nk_draw_nine_slice(out, *bounds, &background->data.slice, nk_white);
            break;
        case NK_STYLE_ITEM_COLOR:
            nk_fill_rect(out, *bounds, style->rounding, background->data.color);
            nk_stroke_rect(out, *bounds, style->rounding, style->border, style->border_color);
            break;
    }

    /* draw cursor */
    switch (cursor->type) {
        case NK_STYLE_ITEM_IMAGE:
            nk_draw_image(out, *scroll, &cursor->data.image, nk_white);
            break;
        case NK_STYLE_ITEM_NINE_SLICE:
            nk_draw_nine_slice(out, *scroll, &cursor->data.slice, nk_white);
            break;
        case NK_STYLE_ITEM_COLOR:
            nk_fill_rect(out, *scroll, style->rounding_cursor, cursor->data.color);
            nk_stroke_rect(out, *scroll, style->rounding_cursor, style->border_cursor, style->cursor_border_color);
            break;
    }
}
NK_LIB float
nk_do_scrollbarv(nk_flags *state,
    struct nk_command_buffer *out, struct nk_rect scroll, int has_scrolling,
    float offset, float target, float step, float button_pixel_inc,
    const struct nk_style_scrollbar *style, struct nk_input *in,
    const struct nk_user_font *font)
{
    struct nk_rect empty_north;
    struct nk_rect empty_south;
    struct nk_rect cursor;

    float scroll_step;
    float scroll_offset;
    float scroll_off;
    float scroll_ratio;

    NK_ASSERT(out);
    NK_ASSERT(style);
    NK_ASSERT(state);
    if (!out || !style) return 0;

    scroll.w = NK_MAX(scroll.w, 1);
    scroll.h = NK_MAX(scroll.h, 0);
    if (target <= scroll.h) return 0;

    /* optional scrollbar buttons */
    if (style->show_buttons) {
        nk_flags ws;
        float scroll_h;
        struct nk_rect button;

        button.x = scroll.x;
        button.w = scroll.w;
        button.h = scroll.w;

        scroll_h = NK_MAX(scroll.h - 2 * button.h,0);
        scroll_step = NK_MIN(step, button_pixel_inc);

        /* decrement button */
        button.y = scroll.y;
        if (nk_do_button_symbol(&ws, out, button, style->dec_symbol,
            NK_BUTTON_REPEATER, &style->dec_button, in, font))
            offset = offset - scroll_step;

        /* increment button */
        button.y = scroll.y + scroll.h - button.h;
        if (nk_do_button_symbol(&ws, out, button, style->inc_symbol,
            NK_BUTTON_REPEATER, &style->inc_button, in, font))
            offset = offset + scroll_step;

        scroll.y = scroll.y + button.h;
        scroll.h = scroll_h;
    }

    /* calculate scrollbar constants */
    scroll_step = NK_MIN(step, scroll.h);
    scroll_offset = NK_CLAMP(0, offset, target - scroll.h);
    scroll_ratio = scroll.h / target;
    scroll_off = scroll_offset / target;

    /* calculate scrollbar cursor bounds */
    cursor.h = NK_MAX((scroll_ratio * scroll.h) - (2*style->border + 2*style->padding.y), 0);
    cursor.y = scroll.y + (scroll_off * scroll.h) + style->border + style->padding.y;
    cursor.w = scroll.w - (2 * style->border + 2 * style->padding.x);
    cursor.x = scroll.x + style->border + style->padding.x;

    /* calculate empty space around cursor */
    empty_north.x = scroll.x;
    empty_north.y = scroll.y;
    empty_north.w = scroll.w;
    empty_north.h = NK_MAX(cursor.y - scroll.y, 0);

    empty_south.x = scroll.x;
    empty_south.y = cursor.y + cursor.h;
    empty_south.w = scroll.w;
    empty_south.h = NK_MAX((scroll.y + scroll.h) - (cursor.y + cursor.h), 0);

    /* update scrollbar */
    scroll_offset = nk_scrollbar_behavior(state, in, has_scrolling, &scroll, &cursor,
        &empty_north, &empty_south, scroll_offset, target, scroll_step, NK_VERTICAL);
    scroll_off = scroll_offset / target;
    cursor.y = scroll.y + (scroll_off * scroll.h) + style->border_cursor + style->padding.y;

    /* draw scrollbar */
    if (style->draw_begin) style->draw_begin(out, style->userdata);
    nk_draw_scrollbar(out, *state, style, &scroll, &cursor);
    if (style->draw_end) style->draw_end(out, style->userdata);
    return scroll_offset;
}
NK_LIB float
nk_do_scrollbarh(nk_flags *state,
    struct nk_command_buffer *out, struct nk_rect scroll, int has_scrolling,
    float offset, float target, float step, float button_pixel_inc,
    const struct nk_style_scrollbar *style, struct nk_input *in,
    const struct nk_user_font *font)
{
    struct nk_rect cursor;
    struct nk_rect empty_west;
    struct nk_rect empty_east;

    float scroll_step;
    float scroll_offset;
    float scroll_off;
    float scroll_ratio;

    NK_ASSERT(out);
    NK_ASSERT(style);
    if (!out || !style) return 0;

    /* scrollbar background */
    scroll.h = NK_MAX(scroll.h, 1);
    scroll.w = NK_MAX(scroll.w, 2 * scroll.h);
    if (target <= scroll.w) return 0;

    /* optional scrollbar buttons */
    if (style->show_buttons) {
        nk_flags ws;
        float scroll_w;
        struct nk_rect button;
        button.y = scroll.y;
        button.w = scroll.h;
        button.h = scroll.h;

        scroll_w = scroll.w - 2 * button.w;
        scroll_step = NK_MIN(step, button_pixel_inc);

        /* decrement button */
        button.x = scroll.x;
        if (nk_do_button_symbol(&ws, out, button, style->dec_symbol,
            NK_BUTTON_REPEATER, &style->dec_button, in, font))
            offset = offset - scroll_step;

        /* increment button */
        button.x = scroll.x + scroll.w - button.w;
        if (nk_do_button_symbol(&ws, out, button, style->inc_symbol,
            NK_BUTTON_REPEATER, &style->inc_button, in, font))
            offset = offset + scroll_step;

        scroll.x = scroll.x + button.w;
        scroll.w = scroll_w;
    }

    /* calculate scrollbar constants */
    scroll_step = NK_MIN(step, scroll.w);
    scroll_offset = NK_CLAMP(0, offset, target - scroll.w);
    scroll_ratio = scroll.w / target;
    scroll_off = scroll_offset / target;

    /* calculate cursor bounds */
    cursor.w = (scroll_ratio * scroll.w) - (2*style->border + 2*style->padding.x);
    cursor.x = scroll.x + (scroll_off * scroll.w) + style->border + style->padding.x;
    cursor.h = scroll.h - (2 * style->border + 2 * style->padding.y);
    cursor.y = scroll.y + style->border + style->padding.y;

    /* calculate empty space around cursor */
    empty_west.x = scroll.x;
    empty_west.y = scroll.y;
    empty_west.w = cursor.x - scroll.x;
    empty_west.h = scroll.h;

    empty_east.x = cursor.x + cursor.w;
    empty_east.y = scroll.y;
    empty_east.w = (scroll.x + scroll.w) - (cursor.x + cursor.w);
    empty_east.h = scroll.h;

    /* update scrollbar */
    scroll_offset = nk_scrollbar_behavior(state, in, has_scrolling, &scroll, &cursor,
        &empty_west, &empty_east, scroll_offset, target, scroll_step, NK_HORIZONTAL);
    scroll_off = scroll_offset / target;
    cursor.x = scroll.x + (scroll_off * scroll.w);

    /* draw scrollbar */
    if (style->draw_begin) style->draw_begin(out, style->userdata);
    nk_draw_scrollbar(out, *state, style, &scroll, &cursor);
    if (style->draw_end) style->draw_end(out, style->userdata);
    return scroll_offset;
}





/* ===============================================================
 *
 *                          TEXT EDITOR
 *
 * ===============================================================*/
/* stb_textedit.h - v1.8  - public domain - Sean Barrett */
struct nk_text_find {
   float x,y;    /* position of n'th character */
   float height; /* height of line */
   int first_char, length; /* first char of row, and length */
   int prev_first;  /*_ first char of previous row */
};

struct nk_text_edit_row {
   float x0,x1;
   /* starting x location, end x location (allows for align=right, etc) */
   float baseline_y_delta;
   /* position of baseline relative to previous row's baseline*/
   float ymin,ymax;
   /* height of row above and below baseline */
   int num_chars;
};

/* forward declarations */
NK_INTERN void nk_textedit_makeundo_delete(struct nk_text_edit*, int, int);
NK_INTERN void nk_textedit_makeundo_insert(struct nk_text_edit*, int, int);
NK_INTERN void nk_textedit_makeundo_replace(struct nk_text_edit*, int, int, int);
#define NK_TEXT_HAS_SELECTION(s)   ((s)->select_start != (s)->select_end)

NK_INTERN float
nk_textedit_get_width(const struct nk_text_edit *edit, int line_start, int char_id,
    const struct nk_user_font *font)
{
    int len = 0;
    nk_rune unicode = 0;
    const char *str = nk_str_at_const(&edit->string, line_start + char_id, &unicode, &len);
    return font->width(font->userdata, font->height, str, len);
}
NK_INTERN void
nk_textedit_layout_row(struct nk_text_edit_row *r, struct nk_text_edit *edit,
    int line_start_id, float row_height, const struct nk_user_font *font)
{
    int l;
    int glyphs = 0;
    nk_rune unicode;
    const char *remaining;
    int len = nk_str_len_char(&edit->string);
    const char *end = nk_str_get_const(&edit->string) + len;
    const char *text = nk_str_at_const(&edit->string, line_start_id, &unicode, &l);
    const struct nk_vec2 size = nk_text_calculate_text_bounds(font,
        text, (int)(end - text), row_height, &remaining, 0, &glyphs, NK_STOP_ON_NEW_LINE);

    r->x0 = 0.0f;
    r->x1 = size.x;
    r->baseline_y_delta = size.y;
    r->ymin = 0.0f;
    r->ymax = size.y;
    r->num_chars = glyphs;
}
NK_INTERN int
nk_textedit_locate_coord(struct nk_text_edit *edit, float x, float y,
    const struct nk_user_font *font, float row_height)
{
    struct nk_text_edit_row r;
    int n = edit->string.len;
    float base_y = 0, prev_x;
    int i=0, k;

    r.x0 = r.x1 = 0;
    r.ymin = r.ymax = 0;
    r.num_chars = 0;

    /* search rows to find one that straddles 'y' */
    while (i < n) {
        nk_textedit_layout_row(&r, edit, i, row_height, font);
        if (r.num_chars <= 0)
            return n;

        if (i==0 && y < base_y + r.ymin)
            return 0;

        if (y < base_y + r.ymax)
            break;

        i += r.num_chars;
        base_y += r.baseline_y_delta;
    }

    /* below all text, return 'after' last character */
    if (i >= n)
        return n;

    /* check if it's before the beginning of the line */
    if (x < r.x0)
        return i;

    /* check if it's before the end of the line */
    if (x < r.x1) {
        /* search characters in row for one that straddles 'x' */
        k = i;
        prev_x = r.x0;
        for (i=0; i < r.num_chars; ++i) {
            float w = nk_textedit_get_width(edit, k, i, font);
            if (x < prev_x+w) {
                if (x < prev_x+w/2)
                    return k+i;
                else return k+i+1;
            }
            prev_x += w;
        }
        /* shouldn't happen, but if it does, fall through to end-of-line case */
    }

    /* if the last character is a newline, return that.
     * otherwise return 'after' the last character */
    if (nk_str_rune_at(&edit->string, i+r.num_chars-1) == '\n')
        return i+r.num_chars-1;
    else return i+r.num_chars;
}
NK_LIB void
nk_textedit_click(struct nk_text_edit *state, float x, float y,
    const struct nk_user_font *font, float row_height)
{
    /* API click: on mouse down, move the cursor to the clicked location,
     * and reset the selection */
    state->cursor = nk_textedit_locate_coord(state, x, y, font, row_height);
    state->select_start = state->cursor;
    state->select_end = state->cursor;
    state->has_preferred_x = 0;
}
NK_LIB void
nk_textedit_drag(struct nk_text_edit *state, float x, float y,
    const struct nk_user_font *font, float row_height)
{
    /* API drag: on mouse drag, move the cursor and selection endpoint
     * to the clicked location */
    int p = nk_textedit_locate_coord(state, x, y, font, row_height);
    if (state->select_start == state->select_end)
        state->select_start = state->cursor;
    state->cursor = state->select_end = p;
}
NK_INTERN void
nk_textedit_find_charpos(struct nk_text_find *find, struct nk_text_edit *state,
    int n, int single_line, const struct nk_user_font *font, float row_height)
{
    /* find the x/y location of a character, and remember info about the previous
     * row in case we get a move-up event (for page up, we'll have to rescan) */
    struct nk_text_edit_row r;
    int prev_start = 0;
    int z = state->string.len;
    int i=0, first;

    nk_zero_struct(r);
    if (n == z) {
        /* if it's at the end, then find the last line -- simpler than trying to
        explicitly handle this case in the regular code */
        nk_textedit_layout_row(&r, state, 0, row_height, font);
        if (single_line) {
            find->first_char = 0;
            find->length = z;
        } else {
            while (i < z) {
                prev_start = i;
                i += r.num_chars;
                nk_textedit_layout_row(&r, state, i, row_height, font);
            }

            find->first_char = i;
            find->length = r.num_chars;
        }
        find->x = r.x1;
        find->y = r.ymin;
        find->height = r.ymax - r.ymin;
        find->prev_first = prev_start;
        return;
    }

    /* search rows to find the one that straddles character n */
    find->y = 0;

    for(;;) {
        nk_textedit_layout_row(&r, state, i, row_height, font);
        if (n < i + r.num_chars) break;
        prev_start = i;
        i += r.num_chars;
        find->y += r.baseline_y_delta;
    }

    find->first_char = first = i;
    find->length = r.num_chars;
    find->height = r.ymax - r.ymin;
    find->prev_first = prev_start;

    /* now scan to find xpos */
    find->x = r.x0;
    for (i=0; first+i < n; ++i)
        find->x += nk_textedit_get_width(state, first, i, font);
}
NK_INTERN void
nk_textedit_clamp(struct nk_text_edit *state)
{
    /* make the selection/cursor state valid if client altered the string */
    int n = state->string.len;
    if (NK_TEXT_HAS_SELECTION(state)) {
        if (state->select_start > n) state->select_start = n;
        if (state->select_end   > n) state->select_end = n;
        /* if clamping forced them to be equal, move the cursor to match */
        if (state->select_start == state->select_end)
            state->cursor = state->select_start;
    }
    if (state->cursor > n) state->cursor = n;
}
NK_API void
nk_textedit_delete(struct nk_text_edit *state, int where, int len)
{
    /* delete characters while updating undo */
    nk_textedit_makeundo_delete(state, where, len);
    nk_str_delete_runes(&state->string, where, len);
    state->has_preferred_x = 0;
}
NK_API void
nk_textedit_delete_selection(struct nk_text_edit *state)
{
    /* delete the section */
    nk_textedit_clamp(state);
    if (NK_TEXT_HAS_SELECTION(state)) {
        if (state->select_start < state->select_end) {
            nk_textedit_delete(state, state->select_start,
                state->select_end - state->select_start);
            state->select_end = state->cursor = state->select_start;
        } else {
            nk_textedit_delete(state, state->select_end,
                state->select_start - state->select_end);
            state->select_start = state->cursor = state->select_end;
        }
        state->has_preferred_x = 0;
    }
}
NK_INTERN void
nk_textedit_sortselection(struct nk_text_edit *state)
{
    /* canonicalize the selection so start <= end */
    if (state->select_end < state->select_start) {
        int temp = state->select_end;
        state->select_end = state->select_start;
        state->select_start = temp;
    }
}
NK_INTERN void
nk_textedit_move_to_first(struct nk_text_edit *state)
{
    /* move cursor to first character of selection */
    if (NK_TEXT_HAS_SELECTION(state)) {
        nk_textedit_sortselection(state);
        state->cursor = state->select_start;
        state->select_end = state->select_start;
        state->has_preferred_x = 0;
    }
}
NK_INTERN void
nk_textedit_move_to_last(struct nk_text_edit *state)
{
    /* move cursor to last character of selection */
    if (NK_TEXT_HAS_SELECTION(state)) {
        nk_textedit_sortselection(state);
        nk_textedit_clamp(state);
        state->cursor = state->select_end;
        state->select_start = state->select_end;
        state->has_preferred_x = 0;
    }
}
NK_INTERN int
nk_is_word_boundary( struct nk_text_edit *state, int idx)
{
    int len;
    nk_rune c;
    if (idx <= 0) return 1;
    if (!nk_str_at_rune(&state->string, idx, &c, &len)) return 1;
    return (c == ' ' || c == '\t' ||c == 0x3000 || c == ',' || c == ';' ||
            c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' ||
            c == '|');
}
NK_INTERN int
nk_textedit_move_to_word_previous(struct nk_text_edit *state)
{
   int c = state->cursor - 1;
   while( c >= 0 && !nk_is_word_boundary(state, c))
      --c;

   if( c < 0 )
      c = 0;

   return c;
}
NK_INTERN int
nk_textedit_move_to_word_next(struct nk_text_edit *state)
{
   const int len = state->string.len;
   int c = state->cursor+1;
   while( c < len && !nk_is_word_boundary(state, c))
      ++c;

   if( c > len )
      c = len;

   return c;
}
NK_INTERN void
nk_textedit_prep_selection_at_cursor(struct nk_text_edit *state)
{
    /* update selection and cursor to match each other */
    if (!NK_TEXT_HAS_SELECTION(state))
        state->select_start = state->select_end = state->cursor;
    else state->cursor = state->select_end;
}
NK_API nk_bool
nk_textedit_cut(struct nk_text_edit *state)
{
    /* API cut: delete selection */
    if (state->mode == NK_TEXT_EDIT_MODE_VIEW)
        return 0;
    if (NK_TEXT_HAS_SELECTION(state)) {
        nk_textedit_delete_selection(state); /* implicitly clamps */
        state->has_preferred_x = 0;
        return 1;
    }
   return 0;
}
NK_API nk_bool
nk_textedit_paste(struct nk_text_edit *state, char const *ctext, int len)
{
    /* API paste: replace existing selection with passed-in text */
    int glyphs;
    const char *text = (const char *) ctext;
    if (state->mode == NK_TEXT_EDIT_MODE_VIEW) return 0;

    /* if there's a selection, the paste should delete it */
    nk_textedit_clamp(state);
    nk_textedit_delete_selection(state);

    /* try to insert the characters */
    glyphs = nk_utf_len(ctext, len);
    if (nk_str_insert_text_char(&state->string, state->cursor, text, len)) {
        nk_textedit_makeundo_insert(state, state->cursor, glyphs);
        state->cursor += len;
        state->has_preferred_x = 0;
        return 1;
    }
    /* remove the undo since we didn't actually insert the characters */
    if (state->undo.undo_point)
        --state->undo.undo_point;
    return 0;
}
NK_API void
nk_textedit_text(struct nk_text_edit *state, const char *text, int total_len)
{
    nk_rune unicode;
    int glyph_len;
    int text_len = 0;

    NK_ASSERT(state);
    NK_ASSERT(text);
    if (!text || !total_len || state->mode == NK_TEXT_EDIT_MODE_VIEW) return;

    glyph_len = nk_utf_decode(text, &unicode, total_len);
    while ((text_len < total_len) && glyph_len)
    {
        /* don't insert a backward delete, just process the event */
        if (unicode == 127) goto next;
        /* can't add newline in single-line mode */
        if (unicode == '\n' && state->single_line) goto next;
        /* filter incoming text */
        if (state->filter && !state->filter(state, unicode)) goto next;

        if (!NK_TEXT_HAS_SELECTION(state) &&
            state->cursor < state->string.len)
        {
            if (state->mode == NK_TEXT_EDIT_MODE_REPLACE) {
                nk_textedit_makeundo_replace(state, state->cursor, 1, 1);
                nk_str_delete_runes(&state->string, state->cursor, 1);
            }
            if (nk_str_insert_text_utf8(&state->string, state->cursor,
                                        text+text_len, 1))
            {
                ++state->cursor;
                state->has_preferred_x = 0;
            }
        } else {
            nk_textedit_delete_selection(state); /* implicitly clamps */
            if (nk_str_insert_text_utf8(&state->string, state->cursor,
                                        text+text_len, 1))
            {
                nk_textedit_makeundo_insert(state, state->cursor, 1);
                state->cursor = NK_MIN(state->cursor + 1, state->string.len);
                state->has_preferred_x = 0;
            }
        }
        next:
        text_len += glyph_len;
        glyph_len = nk_utf_decode(text + text_len, &unicode, total_len-text_len);
    }
}
NK_LIB void
nk_textedit_key(struct nk_text_edit *state, enum nk_keys key, int shift_mod,
    const struct nk_user_font *font, float row_height)
{
retry:
    switch (key)
    {
    case NK_KEY_NONE:
    case NK_KEY_CTRL:
    case NK_KEY_ENTER:
    case NK_KEY_SHIFT:
    case NK_KEY_TAB:
    case NK_KEY_COPY:
    case NK_KEY_CUT:
    case NK_KEY_PASTE:
    case NK_KEY_MAX:
    default: break;
    case NK_KEY_TEXT_UNDO:
         nk_textedit_undo(state);
         state->has_preferred_x = 0;
         break;

    case NK_KEY_TEXT_REDO:
        nk_textedit_redo(state);
        state->has_preferred_x = 0;
        break;

    case NK_KEY_TEXT_SELECT_ALL:
        nk_textedit_select_all(state);
        state->has_preferred_x = 0;
        break;

    case NK_KEY_TEXT_INSERT_MODE:
        if (state->mode == NK_TEXT_EDIT_MODE_VIEW)
            state->mode = NK_TEXT_EDIT_MODE_INSERT;
        break;
    case NK_KEY_TEXT_REPLACE_MODE:
        if (state->mode == NK_TEXT_EDIT_MODE_VIEW)
            state->mode = NK_TEXT_EDIT_MODE_REPLACE;
        break;
    case NK_KEY_TEXT_RESET_MODE:
        if (state->mode == NK_TEXT_EDIT_MODE_INSERT ||
            state->mode == NK_TEXT_EDIT_MODE_REPLACE)
            state->mode = NK_TEXT_EDIT_MODE_VIEW;
        break;

    case NK_KEY_LEFT:
        if (shift_mod) {
            nk_textedit_clamp(state);
            nk_textedit_prep_selection_at_cursor(state);
            /* move selection left */
            if (state->select_end > 0)
                --state->select_end;
            state->cursor = state->select_end;
            state->has_preferred_x = 0;
        } else {
            /* if currently there's a selection,
             * move cursor to start of selection */
            if (NK_TEXT_HAS_SELECTION(state))
                nk_textedit_move_to_first(state);
            else if (state->cursor > 0)
               --state->cursor;
            state->has_preferred_x = 0;
        } break;

    case NK_KEY_RIGHT:
        if (shift_mod) {
            nk_textedit_prep_selection_at_cursor(state);
            /* move selection right */
            ++state->select_end;
            nk_textedit_clamp(state);
            state->cursor = state->select_end;
            state->has_preferred_x = 0;
        } else {
            /* if currently there's a selection,
             * move cursor to end of selection */
            if (NK_TEXT_HAS_SELECTION(state))
                nk_textedit_move_to_last(state);
            else ++state->cursor;
            nk_textedit_clamp(state);
            state->has_preferred_x = 0;
        } break;

    case NK_KEY_TEXT_WORD_LEFT:
        if (shift_mod) {
            if( !NK_TEXT_HAS_SELECTION( state ) )
            nk_textedit_prep_selection_at_cursor(state);
            state->cursor = nk_textedit_move_to_word_previous(state);
            state->select_end = state->cursor;
            nk_textedit_clamp(state );
        } else {
            if (NK_TEXT_HAS_SELECTION(state))
                nk_textedit_move_to_first(state);
            else {
                state->cursor = nk_textedit_move_to_word_previous(state);
                nk_textedit_clamp(state );
            }
        } break;

    case NK_KEY_TEXT_WORD_RIGHT:
        if (shift_mod) {
            if( !NK_TEXT_HAS_SELECTION( state ) )
                nk_textedit_prep_selection_at_cursor(state);
            state->cursor = nk_textedit_move_to_word_next(state);
            state->select_end = state->cursor;
            nk_textedit_clamp(state);
        } else {
            if (NK_TEXT_HAS_SELECTION(state))
                nk_textedit_move_to_last(state);
            else {
                state->cursor = nk_textedit_move_to_word_next(state);
                nk_textedit_clamp(state );
            }
        } break;

    case NK_KEY_DOWN: {
        struct nk_text_find find;
        struct nk_text_edit_row row;
        int i, sel = shift_mod;

        if (state->single_line) {
            /* on windows, up&down in single-line behave like left&right */
            key = NK_KEY_RIGHT;
            goto retry;
        }

        if (sel)
            nk_textedit_prep_selection_at_cursor(state);
        else if (NK_TEXT_HAS_SELECTION(state))
            nk_textedit_move_to_last(state);

        /* compute current position of cursor point */
        nk_textedit_clamp(state);
        nk_textedit_find_charpos(&find, state, state->cursor, state->single_line,
            font, row_height);

        /* now find character position down a row */
        if (find.length)
        {
            float x;
            float goal_x = state->has_preferred_x ? state->preferred_x : find.x;
            int start = find.first_char + find.length;

            state->cursor = start;
            nk_textedit_layout_row(&row, state, state->cursor, row_height, font);
            x = row.x0;

            for (i=0; i < row.num_chars && x < row.x1; ++i) {
                float dx = nk_textedit_get_width(state, start, i, font);
                x += dx;
                if (x > goal_x)
                    break;
                ++state->cursor;
            }
            nk_textedit_clamp(state);

            state->has_preferred_x = 1;
            state->preferred_x = goal_x;
            if (sel)
                state->select_end = state->cursor;
        }
    } break;

    case NK_KEY_UP: {
        struct nk_text_find find;
        struct nk_text_edit_row row;
        int i, sel = shift_mod;

        if (state->single_line) {
            /* on windows, up&down become left&right */
            key = NK_KEY_LEFT;
            goto retry;
        }

        if (sel)
            nk_textedit_prep_selection_at_cursor(state);
        else if (NK_TEXT_HAS_SELECTION(state))
            nk_textedit_move_to_first(state);

         /* compute current position of cursor point */
         nk_textedit_clamp(state);
         nk_textedit_find_charpos(&find, state, state->cursor, state->single_line,
                font, row_height);

         /* can only go up if there's a previous row */
         if (find.prev_first != find.first_char) {
            /* now find character position up a row */
            float x;
            float goal_x = state->has_preferred_x ? state->preferred_x : find.x;

            state->cursor = find.prev_first;
            nk_textedit_layout_row(&row, state, state->cursor, row_height, font);
            x = row.x0;

            for (i=0; i < row.num_chars && x < row.x1; ++i) {
                float dx = nk_textedit_get_width(state, find.prev_first, i, font);
                x += dx;
                if (x > goal_x)
                    break;
                ++state->cursor;
            }
            nk_textedit_clamp(state);

            state->has_preferred_x = 1;
            state->preferred_x = goal_x;
            if (sel) state->select_end = state->cursor;
         }
      } break;

    case NK_KEY_DEL:
        if (state->mode == NK_TEXT_EDIT_MODE_VIEW)
            break;
        if (NK_TEXT_HAS_SELECTION(state))
            nk_textedit_delete_selection(state);
        else {
            int n = state->string.len;
            if (state->cursor < n)
                nk_textedit_delete(state, state->cursor, 1);
         }
         state->has_preferred_x = 0;
         break;

    case NK_KEY_BACKSPACE:
        if (state->mode == NK_TEXT_EDIT_MODE_VIEW)
            break;
        if (NK_TEXT_HAS_SELECTION(state))
            nk_textedit_delete_selection(state);
        else {
            nk_textedit_clamp(state);
            if (state->cursor > 0) {
                nk_textedit_delete(state, state->cursor-1, 1);
                --state->cursor;
            }
         }
         state->has_preferred_x = 0;
         break;

    case NK_KEY_TEXT_START:
         if (shift_mod) {
            nk_textedit_prep_selection_at_cursor(state);
            state->cursor = state->select_end = 0;
            state->has_preferred_x = 0;
         } else {
            state->cursor = state->select_start = state->select_end = 0;
            state->has_preferred_x = 0;
         }
         break;

    case NK_KEY_TEXT_END:
         if (shift_mod) {
            nk_textedit_prep_selection_at_cursor(state);
            state->cursor = state->select_end = state->string.len;
            state->has_preferred_x = 0;
         } else {
            state->cursor = state->string.len;
            state->select_start = state->select_end = 0;
            state->has_preferred_x = 0;
         }
         break;

    case NK_KEY_TEXT_LINE_START: {
        if (shift_mod) {
            struct nk_text_find find;
           nk_textedit_clamp(state);
            nk_textedit_prep_selection_at_cursor(state);
            if (state->string.len && state->cursor == state->string.len)
                --state->cursor;
            nk_textedit_find_charpos(&find, state,state->cursor, state->single_line,
                font, row_height);
            state->cursor = state->select_end = find.first_char;
            state->has_preferred_x = 0;
        } else {
            struct nk_text_find find;
            if (state->string.len && state->cursor == state->string.len)
                --state->cursor;
            nk_textedit_clamp(state);
            nk_textedit_move_to_first(state);
            nk_textedit_find_charpos(&find, state, state->cursor, state->single_line,
                font, row_height);
            state->cursor = find.first_char;
            state->has_preferred_x = 0;
        }
      } break;

    case NK_KEY_TEXT_LINE_END: {
        if (shift_mod) {
            struct nk_text_find find;
            nk_textedit_clamp(state);
            nk_textedit_prep_selection_at_cursor(state);
            nk_textedit_find_charpos(&find, state, state->cursor, state->single_line,
                font, row_height);
            state->has_preferred_x = 0;
            state->cursor = find.first_char + find.length;
            if (find.length > 0 && nk_str_rune_at(&state->string, state->cursor-1) == '\n')
                --state->cursor;
            state->select_end = state->cursor;
        } else {
            struct nk_text_find find;
            nk_textedit_clamp(state);
            nk_textedit_move_to_first(state);
            nk_textedit_find_charpos(&find, state, state->cursor, state->single_line,
                font, row_height);

            state->has_preferred_x = 0;
            state->cursor = find.first_char + find.length;
            if (find.length > 0 && nk_str_rune_at(&state->string, state->cursor-1) == '\n')
                --state->cursor;
        }} break;
    }
}
NK_INTERN void
nk_textedit_flush_redo(struct nk_text_undo_state *state)
{
    state->redo_point = NK_TEXTEDIT_UNDOSTATECOUNT;
    state->redo_char_point = NK_TEXTEDIT_UNDOCHARCOUNT;
}
NK_INTERN void
nk_textedit_discard_undo(struct nk_text_undo_state *state)
{
    /* discard the oldest entry in the undo list */
    if (state->undo_point > 0) {
        /* if the 0th undo state has characters, clean those up */
        if (state->undo_rec[0].char_storage >= 0) {
            int n = state->undo_rec[0].insert_length, i;
            /* delete n characters from all other records */
            state->undo_char_point = (short)(state->undo_char_point - n);
            NK_MEMCPY(state->undo_char, state->undo_char + n,
                (nk_size)state->undo_char_point*sizeof(nk_rune));
            for (i=0; i < state->undo_point; ++i) {
                if (state->undo_rec[i].char_storage >= 0)
                state->undo_rec[i].char_storage = (short)
                    (state->undo_rec[i].char_storage - n);
            }
        }
        --state->undo_point;
        NK_MEMCPY(state->undo_rec, state->undo_rec+1,
            (nk_size)((nk_size)state->undo_point * sizeof(state->undo_rec[0])));
    }
}
NK_INTERN void
nk_textedit_discard_redo(struct nk_text_undo_state *state)
{
/*  discard the oldest entry in the redo list--it's bad if this
    ever happens, but because undo & redo have to store the actual
    characters in different cases, the redo character buffer can
    fill up even though the undo buffer didn't */
    nk_size num;
    int k = NK_TEXTEDIT_UNDOSTATECOUNT-1;
    if (state->redo_point <= k) {
        /* if the k'th undo state has characters, clean those up */
        if (state->undo_rec[k].char_storage >= 0) {
            int n = state->undo_rec[k].insert_length, i;
            /* delete n characters from all other records */
            state->redo_char_point = (short)(state->redo_char_point + n);
            num = (nk_size)(NK_TEXTEDIT_UNDOCHARCOUNT - state->redo_char_point);
            NK_MEMCPY(state->undo_char + state->redo_char_point,
                state->undo_char + state->redo_char_point-n, num * sizeof(char));
            for (i = state->redo_point; i < k; ++i) {
                if (state->undo_rec[i].char_storage >= 0) {
                    state->undo_rec[i].char_storage = (short)
                        (state->undo_rec[i].char_storage + n);
                }
            }
        }
        ++state->redo_point;
        num = (nk_size)(NK_TEXTEDIT_UNDOSTATECOUNT - state->redo_point);
        if (num) NK_MEMCPY(state->undo_rec + state->redo_point-1,
            state->undo_rec + state->redo_point, num * sizeof(state->undo_rec[0]));
    }
}
NK_INTERN struct nk_text_undo_record*
nk_textedit_create_undo_record(struct nk_text_undo_state *state, int numchars)
{
    /* any time we create a new undo record, we discard redo*/
    nk_textedit_flush_redo(state);

    /* if we have no free records, we have to make room,
     * by sliding the existing records down */
    if (state->undo_point == NK_TEXTEDIT_UNDOSTATECOUNT)
        nk_textedit_discard_undo(state);

    /* if the characters to store won't possibly fit in the buffer,
     * we can't undo */
    if (numchars > NK_TEXTEDIT_UNDOCHARCOUNT) {
        state->undo_point = 0;
        state->undo_char_point = 0;
        return 0;
    }

    /* if we don't have enough free characters in the buffer,
     * we have to make room */
    while (state->undo_char_point + numchars > NK_TEXTEDIT_UNDOCHARCOUNT)
        nk_textedit_discard_undo(state);
    return &state->undo_rec[state->undo_point++];
}
NK_INTERN nk_rune*
nk_textedit_createundo(struct nk_text_undo_state *state, int pos,
    int insert_len, int delete_len)
{
    struct nk_text_undo_record *r = nk_textedit_create_undo_record(state, insert_len);
    if (r == 0)
        return 0;

    r->where = pos;
    r->insert_length = (short) insert_len;
    r->delete_length = (short) delete_len;

    if (insert_len == 0) {
        r->char_storage = -1;
        return 0;
    } else {
        r->char_storage = state->undo_char_point;
        state->undo_char_point = (short)(state->undo_char_point +  insert_len);
        return &state->undo_char[r->char_storage];
    }
}
NK_API void
nk_textedit_undo(struct nk_text_edit *state)
{
    struct nk_text_undo_state *s = &state->undo;
    struct nk_text_undo_record u, *r;
    if (s->undo_point == 0)
        return;

    /* we need to do two things: apply the undo record, and create a redo record */
    u = s->undo_rec[s->undo_point-1];
    r = &s->undo_rec[s->redo_point-1];
    r->char_storage = -1;

    r->insert_length = u.delete_length;
    r->delete_length = u.insert_length;
    r->where = u.where;

    if (u.delete_length)
    {
       /*   if the undo record says to delete characters, then the redo record will
            need to re-insert the characters that get deleted, so we need to store
            them.
            there are three cases:
                - there's enough room to store the characters
                - characters stored for *redoing* don't leave room for redo
                - characters stored for *undoing* don't leave room for redo
            if the last is true, we have to bail */
        if (s->undo_char_point + u.delete_length >= NK_TEXTEDIT_UNDOCHARCOUNT) {
            /* the undo records take up too much character space; there's no space
            * to store the redo characters */
            r->insert_length = 0;
        } else {
            int i;
            /* there's definitely room to store the characters eventually */
            while (s->undo_char_point + u.delete_length > s->redo_char_point) {
                /* there's currently not enough room, so discard a redo record */
                nk_textedit_discard_redo(s);
                /* should never happen: */
                if (s->redo_point == NK_TEXTEDIT_UNDOSTATECOUNT)
                    return;
            }

            r = &s->undo_rec[s->redo_point-1];
            r->char_storage = (short)(s->redo_char_point - u.delete_length);
            s->redo_char_point = (short)(s->redo_char_point -  u.delete_length);

            /* now save the characters */
            for (i=0; i < u.delete_length; ++i)
                s->undo_char[r->char_storage + i] =
                    nk_str_rune_at(&state->string, u.where + i);
        }
        /* now we can carry out the deletion */
        nk_str_delete_runes(&state->string, u.where, u.delete_length);
    }

    /* check type of recorded action: */
    if (u.insert_length) {
        /* easy case: was a deletion, so we need to insert n characters */
        nk_str_insert_text_runes(&state->string, u.where,
            &s->undo_char[u.char_storage], u.insert_length);
        s->undo_char_point = (short)(s->undo_char_point - u.insert_length);
    }
    state->cursor = (short)(u.where + u.insert_length);

    s->undo_point--;
    s->redo_point--;
}
NK_API void
nk_textedit_redo(struct nk_text_edit *state)
{
    struct nk_text_undo_state *s = &state->undo;
    struct nk_text_undo_record *u, r;
    if (s->redo_point == NK_TEXTEDIT_UNDOSTATECOUNT)
        return;

    /* we need to do two things: apply the redo record, and create an undo record */
    u = &s->undo_rec[s->undo_point];
    r = s->undo_rec[s->redo_point];

    /* we KNOW there must be room for the undo record, because the redo record
    was derived from an undo record */
    u->delete_length = r.insert_length;
    u->insert_length = r.delete_length;
    u->where = r.where;
    u->char_storage = -1;

    if (r.delete_length) {
        /* the redo record requires us to delete characters, so the undo record
        needs to store the characters */
        if (s->undo_char_point + u->insert_length > s->redo_char_point) {
            u->insert_length = 0;
            u->delete_length = 0;
        } else {
            int i;
            u->char_storage = s->undo_char_point;
            s->undo_char_point = (short)(s->undo_char_point + u->insert_length);

            /* now save the characters */
            for (i=0; i < u->insert_length; ++i) {
                s->undo_char[u->char_storage + i] =
                    nk_str_rune_at(&state->string, u->where + i);
            }
        }
        nk_str_delete_runes(&state->string, r.where, r.delete_length);
    }

    if (r.insert_length) {
        /* easy case: need to insert n characters */
        nk_str_insert_text_runes(&state->string, r.where,
            &s->undo_char[r.char_storage], r.insert_length);
    }
    state->cursor = r.where + r.insert_length;

    s->undo_point++;
    s->redo_point++;
}
NK_INTERN void
nk_textedit_makeundo_insert(struct nk_text_edit *state, int where, int length)
{
    nk_textedit_createundo(&state->undo, where, 0, length);
}
NK_INTERN void
nk_textedit_makeundo_delete(struct nk_text_edit *state, int where, int length)
{
    int i;
    nk_rune *p = nk_textedit_createundo(&state->undo, where, length, 0);
    if (p) {
        for (i=0; i < length; ++i)
            p[i] = nk_str_rune_at(&state->string, where+i);
    }
}
NK_INTERN void
nk_textedit_makeundo_replace(struct nk_text_edit *state, int where,
    int old_length, int new_length)
{
    int i;
    nk_rune *p = nk_textedit_createundo(&state->undo, where, old_length, new_length);
    if (p) {
        for (i=0; i < old_length; ++i)
            p[i] = nk_str_rune_at(&state->string, where+i);
    }
}
NK_LIB void
nk_textedit_clear_state(struct nk_text_edit *state, enum nk_text_edit_type type,
    nk_plugin_filter filter)
{
    /* reset the state to default */
   state->undo.undo_point = 0;
   state->undo.undo_char_point = 0;
   state->undo.redo_point = NK_TEXTEDIT_UNDOSTATECOUNT;
   state->undo.redo_char_point = NK_TEXTEDIT_UNDOCHARCOUNT;
   state->select_end = state->select_start = 0;
   state->cursor = 0;
   state->has_preferred_x = 0;
   state->preferred_x = 0;
   state->cursor_at_end_of_line = 0;
   state->initialized = 1;
   state->single_line = (unsigned char)(type == NK_TEXT_EDIT_SINGLE_LINE);
   state->mode = NK_TEXT_EDIT_MODE_VIEW;
   state->filter = filter;
   state->scrollbar = nk_vec2(0,0);
}
NK_API void
nk_textedit_init_fixed(struct nk_text_edit *state, void *memory, nk_size size)
{
    NK_ASSERT(state);
    NK_ASSERT(memory);
    if (!state || !memory || !size) return;
    NK_MEMSET(state, 0, sizeof(struct nk_text_edit));
    nk_textedit_clear_state(state, NK_TEXT_EDIT_SINGLE_LINE, 0);
    nk_str_init_fixed(&state->string, memory, size);
}
NK_API void
nk_textedit_init(struct nk_text_edit *state, const struct nk_allocator *alloc, nk_size size)
{
    NK_ASSERT(state);
    NK_ASSERT(alloc);
    if (!state || !alloc) return;
    NK_MEMSET(state, 0, sizeof(struct nk_text_edit));
    nk_textedit_clear_state(state, NK_TEXT_EDIT_SINGLE_LINE, 0);
    nk_str_init(&state->string, alloc, size);
}
#ifdef NK_INCLUDE_DEFAULT_ALLOCATOR
NK_API void
nk_textedit_init_default(struct nk_text_edit *state)
{
    NK_ASSERT(state);
    if (!state) return;
    NK_MEMSET(state, 0, sizeof(struct nk_text_edit));
    nk_textedit_clear_state(state, NK_TEXT_EDIT_SINGLE_LINE, 0);
    nk_str_init_default(&state->string);
}
#endif
NK_API void
nk_textedit_select_all(struct nk_text_edit *state)
{
    NK_ASSERT(state);
    state->select_start = 0;
    state->select_end = state->string.len;
}
NK_API void
nk_textedit_free(struct nk_text_edit *state)
{
    NK_ASSERT(state);
    if (!state) return;
    nk_str_free(&state->string);
}





/* ===============================================================
 *
 *                          FILTER
 *
 * ===============================================================*/
NK_API nk_bool
nk_filter_default(const struct nk_text_edit *box, nk_rune unicode)
{
    NK_UNUSED(unicode);
    NK_UNUSED(box);
    return nk_true;
}
NK_API nk_bool
nk_filter_ascii(const struct nk_text_edit *box, nk_rune unicode)
{
    NK_UNUSED(box);
    if (unicode > 128) return nk_false;
    else return nk_true;
}
NK_API nk_bool
nk_filter_float(const struct nk_text_edit *box, nk_rune unicode)
{
    NK_UNUSED(box);
    if ((unicode < '0' || unicode > '9') && unicode != '.' && unicode != '-')
        return nk_false;
    else return nk_true;
}
NK_API nk_bool
nk_filter_decimal(const struct nk_text_edit *box, nk_rune unicode)
{
    NK_UNUSED(box);
    if ((unicode < '0' || unicode > '9') && unicode != '-')
        return nk_false;
    else return nk_true;
}
NK_API nk_bool
nk_filter_hex(const struct nk_text_edit *box, nk_rune unicode)
{
    NK_UNUSED(box);
    if ((unicode < '0' || unicode > '9') &&
        (unicode < 'a' || unicode > 'f') &&
        (unicode < 'A' || unicode > 'F'))
        return nk_false;
    else return nk_true;
}
NK_API nk_bool
nk_filter_oct(const struct nk_text_edit *box, nk_rune unicode)
{
    NK_UNUSED(box);
    if (unicode < '0' || unicode > '7')
        return nk_false;
    else return nk_true;
}
NK_API nk_bool
nk_filter_binary(const struct nk_text_edit *box, nk_rune unicode)
{
    NK_UNUSED(box);
    if (unicode != '0' && unicode != '1')
        return nk_false;
    else return nk_true;
}

/* ===============================================================
 *
 *                          EDIT
 *
 * ===============================================================*/
NK_LIB void
nk_edit_draw_text(struct nk_command_buffer *out,
    const struct nk_style_edit *style, float pos_x, float pos_y,
    float x_offset, const char *text, int byte_len, float row_height,
    const struct nk_user_font *font, struct nk_color background,
    struct nk_color foreground, nk_bool is_selected)
{
    NK_ASSERT(out);
    NK_ASSERT(font);
    NK_ASSERT(style);
    if (!text || !byte_len || !out || !style) return;

    {int glyph_len = 0;
    nk_rune unicode = 0;
    int text_len = 0;
    float line_width = 0;
    float glyph_width;
    const char *line = text;
    float line_offset = 0;
    int line_count = 0;

    struct nk_text txt;
    txt.padding = nk_vec2(0,0);
    txt.background = background;
    txt.text = foreground;

    foreground = nk_rgb_factor(foreground, style->color_factor);
    background = nk_rgb_factor(background, style->color_factor);

    glyph_len = nk_utf_decode(text+text_len, &unicode, byte_len-text_len);
    if (!glyph_len) return;
    while ((text_len < byte_len) && glyph_len)
    {
        if (unicode == '\n') {
            /* new line separator so draw previous line */
            struct nk_rect label;
            label.y = pos_y + line_offset;
            label.h = row_height;
            label.w = line_width;
            label.x = pos_x;
            if (!line_count)
                label.x += x_offset;

            if (is_selected) /* selection needs to draw different background color */
                nk_fill_rect(out, label, 0, background);
            nk_widget_text(out, label, line, (int)((text + text_len) - line),
                &txt, NK_TEXT_CENTERED, font);

            text_len++;
            line_count++;
            line_width = 0;
            line = text + text_len;
            line_offset += row_height;
            glyph_len = nk_utf_decode(text + text_len, &unicode, (int)(byte_len-text_len));
            continue;
        }
        if (unicode == '\r') {
            text_len++;
            glyph_len = nk_utf_decode(text + text_len, &unicode, byte_len-text_len);
            continue;
        }
        glyph_width = font->width(font->userdata, font->height, text+text_len, glyph_len);
        line_width += (float)glyph_width;
        text_len += glyph_len;
        glyph_len = nk_utf_decode(text + text_len, &unicode, byte_len-text_len);
        continue;
    }
    if (line_width > 0) {
        /* draw last line */
        struct nk_rect label;
        label.y = pos_y + line_offset;
        label.h = row_height;
        label.w = line_width;
        label.x = pos_x;
        if (!line_count)
            label.x += x_offset;

        if (is_selected)
            nk_fill_rect(out, label, 0, background);
        nk_widget_text(out, label, line, (int)((text + text_len) - line),
            &txt, NK_TEXT_LEFT, font);
    }}
}
NK_LIB nk_flags
nk_do_edit(nk_flags *state, struct nk_command_buffer *out,
    struct nk_rect bounds, nk_flags flags, nk_plugin_filter filter,
    struct nk_text_edit *edit, const struct nk_style_edit *style,
    struct nk_input *in, const struct nk_user_font *font)
{
    struct nk_rect area;
    nk_flags ret = 0;
    float row_height;
    char prev_state = 0;
    char is_hovered = 0;
    char select_all = 0;
    char cursor_follow = 0;
    struct nk_rect old_clip;
    struct nk_rect clip;

    NK_ASSERT(state);
    NK_ASSERT(out);
    NK_ASSERT(style);
    if (!state || !out || !style)
        return ret;

    /* visible text area calculation */
    area.x = bounds.x + style->padding.x + style->border;
    area.y = bounds.y + style->padding.y + style->border;
    area.w = bounds.w - (2.0f * style->padding.x + 2 * style->border);
    area.h = bounds.h - (2.0f * style->padding.y + 2 * style->border);
    if (flags & NK_EDIT_MULTILINE)
        area.w = NK_MAX(0, area.w - style->scrollbar_size.x);
    row_height = (flags & NK_EDIT_MULTILINE)? font->height + style->row_padding: area.h;

    /* calculate clipping rectangle */
    old_clip = out->clip;
    nk_unify(&clip, &old_clip, area.x, area.y, area.x + area.w, area.y + area.h);

    /* update edit state */
    prev_state = (char)edit->active;
    is_hovered = (char)nk_input_is_mouse_hovering_rect(in, bounds);
    if (in && in->mouse.buttons[NK_BUTTON_LEFT].clicked && in->mouse.buttons[NK_BUTTON_LEFT].down) {
        edit->active = NK_INBOX(in->mouse.pos.x, in->mouse.pos.y,
                                bounds.x, bounds.y, bounds.w, bounds.h);
    }

    /* (de)activate text editor */
    if (!prev_state && edit->active) {
        const enum nk_text_edit_type type = (flags & NK_EDIT_MULTILINE) ?
            NK_TEXT_EDIT_MULTI_LINE: NK_TEXT_EDIT_SINGLE_LINE;
        /* keep scroll position when re-activating edit widget */
        struct nk_vec2 oldscrollbar = edit->scrollbar;
        nk_textedit_clear_state(edit, type, filter);
        edit->scrollbar = oldscrollbar;
        if (flags & NK_EDIT_AUTO_SELECT)
            select_all = nk_true;
        if (flags & NK_EDIT_GOTO_END_ON_ACTIVATE) {
            edit->cursor = edit->string.len;
            in = 0;
        }
    } else if (!edit->active) edit->mode = NK_TEXT_EDIT_MODE_VIEW;
    if (flags & NK_EDIT_READ_ONLY)
        edit->mode = NK_TEXT_EDIT_MODE_VIEW;
    else if (flags & NK_EDIT_ALWAYS_INSERT_MODE)
        edit->mode = NK_TEXT_EDIT_MODE_INSERT;

    ret = (edit->active) ? NK_EDIT_ACTIVE: NK_EDIT_INACTIVE;
    if (prev_state != edit->active)
        ret |= (edit->active) ? NK_EDIT_ACTIVATED: NK_EDIT_DEACTIVATED;

    /* handle user input */
    if (edit->active && in)
    {
        int shift_mod = in->keyboard.keys[NK_KEY_SHIFT].down;
        const float mouse_x = (in->mouse.pos.x - area.x) + edit->scrollbar.x;
        const float mouse_y = (in->mouse.pos.y - area.y) + edit->scrollbar.y;

        /* mouse click handler */
        is_hovered = (char)nk_input_is_mouse_hovering_rect(in, area);
        if (select_all) {
            nk_textedit_select_all(edit);
        } else if (is_hovered && in->mouse.buttons[NK_BUTTON_LEFT].down &&
            in->mouse.buttons[NK_BUTTON_LEFT].clicked) {
            nk_textedit_click(edit, mouse_x, mouse_y, font, row_height);
        } else if (in->mouse.buttons[NK_BUTTON_LEFT].down &&
            (in->mouse.delta.x != 0.0f || in->mouse.delta.y != 0.0f)) {
            nk_textedit_drag(edit, mouse_x, mouse_y, font, row_height);
            cursor_follow = nk_true;
        } else if (is_hovered && in->mouse.buttons[NK_BUTTON_RIGHT].clicked &&
            in->mouse.buttons[NK_BUTTON_RIGHT].down) {
            nk_textedit_key(edit, NK_KEY_TEXT_WORD_LEFT, nk_false, font, row_height);
            nk_textedit_key(edit, NK_KEY_TEXT_WORD_RIGHT, nk_true, font, row_height);
            cursor_follow = nk_true;
        }

        {int i; /* keyboard input */
        int old_mode = edit->mode;
        for (i = 0; i < NK_KEY_MAX; ++i) {
            if (i == NK_KEY_ENTER || i == NK_KEY_TAB) continue; /* special case */
            if (nk_input_is_key_pressed(in, (enum nk_keys)i)) {
                nk_textedit_key(edit, (enum nk_keys)i, shift_mod, font, row_height);
                cursor_follow = nk_true;
            }
        }
        if (old_mode != edit->mode) {
            in->keyboard.text_len = 0;
        }}

        /* text input */
        edit->filter = filter;
        if (in->keyboard.text_len) {
            nk_textedit_text(edit, in->keyboard.text, in->keyboard.text_len);
            cursor_follow = nk_true;
            in->keyboard.text_len = 0;
        }

        /* enter key handler */
        if (nk_input_is_key_pressed(in, NK_KEY_ENTER)) {
            cursor_follow = nk_true;
            if (flags & NK_EDIT_CTRL_ENTER_NEWLINE && shift_mod)
                nk_textedit_text(edit, "\n", 1);
            else if (flags & NK_EDIT_SIG_ENTER)
                ret |= NK_EDIT_COMMITED;
            else nk_textedit_text(edit, "\n", 1);
        }

        /* cut & copy handler */
        {int copy= nk_input_is_key_pressed(in, NK_KEY_COPY);
        int cut = nk_input_is_key_pressed(in, NK_KEY_CUT);
        if ((copy || cut) && (flags & NK_EDIT_CLIPBOARD))
        {
            int glyph_len;
            nk_rune unicode;
            const char *text;
            int b = edit->select_start;
            int e = edit->select_end;

            int begin = NK_MIN(b, e);
            int end = NK_MAX(b, e);
            text = nk_str_at_const(&edit->string, begin, &unicode, &glyph_len);
            if (edit->clip.copy)
                edit->clip.copy(edit->clip.userdata, text, end - begin);
            if (cut && !(flags & NK_EDIT_READ_ONLY)){
                nk_textedit_cut(edit);
                cursor_follow = nk_true;
            }
        }}

        /* paste handler */
        {int paste = nk_input_is_key_pressed(in, NK_KEY_PASTE);
        if (paste && (flags & NK_EDIT_CLIPBOARD) && edit->clip.paste) {
            edit->clip.paste(edit->clip.userdata, edit);
            cursor_follow = nk_true;
        }}

        /* tab handler */
        {int tab = nk_input_is_key_pressed(in, NK_KEY_TAB);
        if (tab && (flags & NK_EDIT_ALLOW_TAB)) {
            nk_textedit_text(edit, "    ", 4);
            cursor_follow = nk_true;
        }}
    }

    /* set widget state */
    if (edit->active)
        *state = NK_WIDGET_STATE_ACTIVE;
    else nk_widget_state_reset(state);

    if (is_hovered)
        *state |= NK_WIDGET_STATE_HOVERED;

    /* DRAW EDIT */
    {const char *text = nk_str_get_const(&edit->string);
    int len = nk_str_len_char(&edit->string);

    {/* select background colors/images  */
    const struct nk_style_item *background;
    if (*state & NK_WIDGET_STATE_ACTIVED)
        background = &style->active;
    else if (*state & NK_WIDGET_STATE_HOVER)
        background = &style->hover;
    else background = &style->normal;

    /* draw background frame */
    switch(background->type) {
        case NK_STYLE_ITEM_IMAGE:
            nk_draw_image(out, bounds, &background->data.image, nk_rgb_factor(nk_white, style->color_factor));
            break;
        case NK_STYLE_ITEM_NINE_SLICE:
            nk_draw_nine_slice(out, bounds, &background->data.slice, nk_rgb_factor(nk_white, style->color_factor));
            break;
        case NK_STYLE_ITEM_COLOR:
            nk_fill_rect(out, bounds, style->rounding, nk_rgb_factor(background->data.color, style->color_factor));
            nk_stroke_rect(out, bounds, style->rounding, style->border, nk_rgb_factor(style->border_color, style->color_factor));
            break;
    }}


    area.w = NK_MAX(0, area.w - style->cursor_size);
    if (edit->active)
    {
        int total_lines = 1;
        struct nk_vec2 text_size = nk_vec2(0,0);

        /* text pointer positions */
        const char *cursor_ptr = 0;
        const char *select_begin_ptr = 0;
        const char *select_end_ptr = 0;

        /* 2D pixel positions */
        struct nk_vec2 cursor_pos = nk_vec2(0,0);
        struct nk_vec2 selection_offset_start = nk_vec2(0,0);
        struct nk_vec2 selection_offset_end = nk_vec2(0,0);

        int selection_begin = NK_MIN(edit->select_start, edit->select_end);
        int selection_end = NK_MAX(edit->select_start, edit->select_end);

        /* calculate total line count + total space + cursor/selection position */
        float line_width = 0.0f;
        if (text && len)
        {
            /* utf8 encoding */
            float glyph_width;
            int glyph_len = 0;
            nk_rune unicode = 0;
            int text_len = 0;
            int glyphs = 0;
            int row_begin = 0;

            glyph_len = nk_utf_decode(text, &unicode, len);
            glyph_width = font->width(font->userdata, font->height, text, glyph_len);
            line_width = 0;

            /* iterate all lines */
            while ((text_len < len) && glyph_len)
            {
                /* set cursor 2D position and line */
                if (!cursor_ptr && glyphs == edit->cursor)
                {
                    int glyph_offset;
                    struct nk_vec2 out_offset;
                    struct nk_vec2 row_size;
                    const char *remaining;

                    /* calculate 2d position */
                    cursor_pos.y = (float)(total_lines-1) * row_height;
                    row_size = nk_text_calculate_text_bounds(font, text+row_begin,
                                text_len-row_begin, row_height, &remaining,
                                &out_offset, &glyph_offset, NK_STOP_ON_NEW_LINE);
                    cursor_pos.x = row_size.x;
                    cursor_ptr = text + text_len;
                }

                /* set start selection 2D position and line */
                if (!select_begin_ptr && edit->select_start != edit->select_end &&
                    glyphs == selection_begin)
                {
                    int glyph_offset;
                    struct nk_vec2 out_offset;
                    struct nk_vec2 row_size;
                    const char *remaining;

                    /* calculate 2d position */
                    selection_offset_start.y = (float)(NK_MAX(total_lines-1,0)) * row_height;
                    row_size = nk_text_calculate_text_bounds(font, text+row_begin,
                                text_len-row_begin, row_height, &remaining,
                                &out_offset, &glyph_offset, NK_STOP_ON_NEW_LINE);
                    selection_offset_start.x = row_size.x;
                    select_begin_ptr = text + text_len;
                }

                /* set end selection 2D position and line */
                if (!select_end_ptr && edit->select_start != edit->select_end &&
                    glyphs == selection_end)
                {
                    int glyph_offset;
                    struct nk_vec2 out_offset;
                    struct nk_vec2 row_size;
                    const char *remaining;

                    /* calculate 2d position */
                    selection_offset_end.y = (float)(total_lines-1) * row_height;
                    row_size = nk_text_calculate_text_bounds(font, text+row_begin,
                                text_len-row_begin, row_height, &remaining,
                                &out_offset, &glyph_offset, NK_STOP_ON_NEW_LINE);
                    selection_offset_end.x = row_size.x;
                    select_end_ptr = text + text_len;
                }
                if (unicode == '\n') {
                    text_size.x = NK_MAX(text_size.x, line_width);
                    total_lines++;
                    line_width = 0;
                    text_len++;
                    glyphs++;
                    row_begin = text_len;
                    glyph_len = nk_utf_decode(text + text_len, &unicode, len-text_len);
                    glyph_width = font->width(font->userdata, font->height, text+text_len, glyph_len);
                    continue;
                }

                glyphs++;
                text_len += glyph_len;
                line_width += (float)glyph_width;

                glyph_len = nk_utf_decode(text + text_len, &unicode, len-text_len);
                glyph_width = font->width(font->userdata, font->height,
                    text+text_len, glyph_len);
                continue;
            }
            text_size.y = (float)total_lines * row_height;

            /* handle case when cursor is at end of text buffer */
            if (!cursor_ptr && edit->cursor == edit->string.len) {
                cursor_pos.x = line_width;
                cursor_pos.y = text_size.y - row_height;
            }
        }
        {
            /* scrollbar */
            if (cursor_follow)
            {
                /* update scrollbar to follow cursor */
                if (!(flags & NK_EDIT_NO_HORIZONTAL_SCROLL)) {
                    /* horizontal scroll */
                    const float scroll_increment = area.w * 0.25f;
                    if (cursor_pos.x < edit->scrollbar.x)
                        edit->scrollbar.x = (float)(int)NK_MAX(0.0f, cursor_pos.x - scroll_increment);
                    if (cursor_pos.x >= edit->scrollbar.x + area.w)
                        edit->scrollbar.x = (float)(int)NK_MAX(0.0f, cursor_pos.x - area.w + scroll_increment);
                } else edit->scrollbar.x = 0;

                if (flags & NK_EDIT_MULTILINE) {
                    /* vertical scroll */
                    if (cursor_pos.y < edit->scrollbar.y)
                        edit->scrollbar.y = NK_MAX(0.0f, cursor_pos.y - row_height);
                    if (cursor_pos.y >= edit->scrollbar.y + row_height)
                        edit->scrollbar.y = edit->scrollbar.y + row_height;
                } else edit->scrollbar.y = 0;
            }

            /* scrollbar widget */
            if (flags & NK_EDIT_MULTILINE)
            {
                nk_flags ws;
                struct nk_rect scroll;
                float scroll_target;
                float scroll_offset;
                float scroll_step;
                float scroll_inc;

                scroll = area;
                scroll.x = (bounds.x + bounds.w - style->border) - style->scrollbar_size.x;
                scroll.w = style->scrollbar_size.x;

                scroll_offset = edit->scrollbar.y;
                scroll_step = scroll.h * 0.10f;
                scroll_inc = scroll.h * 0.01f;
                scroll_target = text_size.y;
                edit->scrollbar.y = nk_do_scrollbarv(&ws, out, scroll, 0,
                        scroll_offset, scroll_target, scroll_step, scroll_inc,
                        &style->scrollbar, in, font);
            }
        }

        /* draw text */
        {struct nk_color background_color;
        struct nk_color text_color;
        struct nk_color sel_background_color;
        struct nk_color sel_text_color;
        struct nk_color cursor_color;
        struct nk_color cursor_text_color;
        const struct nk_style_item *background;
        nk_push_scissor(out, clip);

        /* select correct colors to draw */
        if (*state & NK_WIDGET_STATE_ACTIVED) {
            background = &style->active;
            text_color = style->text_active;
            sel_text_color = style->selected_text_hover;
            sel_background_color = style->selected_hover;
            cursor_color = style->cursor_hover;
            cursor_text_color = style->cursor_text_hover;
        } else if (*state & NK_WIDGET_STATE_HOVER) {
            background = &style->hover;
            text_color = style->text_hover;
            sel_text_color = style->selected_text_hover;
            sel_background_color = style->selected_hover;
            cursor_text_color = style->cursor_text_hover;
            cursor_color = style->cursor_hover;
        } else {
            background = &style->normal;
            text_color = style->text_normal;
            sel_text_color = style->selected_text_normal;
            sel_background_color = style->selected_normal;
            cursor_color = style->cursor_normal;
            cursor_text_color = style->cursor_text_normal;
        }
        if (background->type == NK_STYLE_ITEM_IMAGE)
            background_color = nk_rgba(0,0,0,0);
        else
            background_color = background->data.color;

        cursor_color = nk_rgb_factor(cursor_color, style->color_factor);
        cursor_text_color = nk_rgb_factor(cursor_text_color, style->color_factor);

        if (edit->select_start == edit->select_end) {
            /* no selection so just draw the complete text */
            const char *begin = nk_str_get_const(&edit->string);
            int l = nk_str_len_char(&edit->string);
            nk_edit_draw_text(out, style, area.x - edit->scrollbar.x,
                area.y - edit->scrollbar.y, 0, begin, l, row_height, font,
                background_color, text_color, nk_false);
        } else {
            /* edit has selection so draw 1-3 text chunks */
            if (edit->select_start != edit->select_end && selection_begin > 0){
                /* draw unselected text before selection */
                const char *begin = nk_str_get_const(&edit->string);
                NK_ASSERT(select_begin_ptr);
                nk_edit_draw_text(out, style, area.x - edit->scrollbar.x,
                    area.y - edit->scrollbar.y, 0, begin, (int)(select_begin_ptr - begin),
                    row_height, font, background_color, text_color, nk_false);
            }
            if (edit->select_start != edit->select_end) {
                /* draw selected text */
                NK_ASSERT(select_begin_ptr);
                if (!select_end_ptr) {
                    const char *begin = nk_str_get_const(&edit->string);
                    select_end_ptr = begin + nk_str_len_char(&edit->string);
                }
                nk_edit_draw_text(out, style,
                    area.x - edit->scrollbar.x,
                    area.y + selection_offset_start.y - edit->scrollbar.y,
                    selection_offset_start.x,
                    select_begin_ptr, (int)(select_end_ptr - select_begin_ptr),
                    row_height, font, sel_background_color, sel_text_color, nk_true);
            }
            if ((edit->select_start != edit->select_end &&
                selection_end < edit->string.len))
            {
                /* draw unselected text after selected text */
                const char *begin = select_end_ptr;
                const char *end = nk_str_get_const(&edit->string) +
                                    nk_str_len_char(&edit->string);
                NK_ASSERT(select_end_ptr);
                nk_edit_draw_text(out, style,
                    area.x - edit->scrollbar.x,
                    area.y + selection_offset_end.y - edit->scrollbar.y,
                    selection_offset_end.x,
                    begin, (int)(end - begin), row_height, font,
                    background_color, text_color, nk_true);
            }
        }

        /* cursor */
        if (edit->select_start == edit->select_end)
        {
            if (edit->cursor >= nk_str_len(&edit->string) ||
                (cursor_ptr && *cursor_ptr == '\n')) {
                /* draw cursor at end of line */
                struct nk_rect cursor;
                cursor.w = style->cursor_size;
                cursor.h = font->height;
                cursor.x = area.x + cursor_pos.x - edit->scrollbar.x;
                cursor.y = area.y + cursor_pos.y + row_height/2.0f - cursor.h/2.0f;
                cursor.y -= edit->scrollbar.y;
                nk_fill_rect(out, cursor, 0, cursor_color);
            } else {
                /* draw cursor inside text */
                int glyph_len;
                struct nk_rect label;
                struct nk_text txt;

                nk_rune unicode;
                NK_ASSERT(cursor_ptr);
                glyph_len = nk_utf_decode(cursor_ptr, &unicode, 4);

                label.x = area.x + cursor_pos.x - edit->scrollbar.x;
                label.y = area.y + cursor_pos.y - edit->scrollbar.y;
                label.w = font->width(font->userdata, font->height, cursor_ptr, glyph_len);
                label.h = row_height;

                txt.padding = nk_vec2(0,0);
                txt.background = cursor_color;;
                txt.text = cursor_text_color;
                nk_fill_rect(out, label, 0, cursor_color);
                nk_widget_text(out, label, cursor_ptr, glyph_len, &txt, NK_TEXT_LEFT, font);
            }
        }}
    } else {
        /* not active so just draw text */
        int l = nk_str_len_char(&edit->string);
        const char *begin = nk_str_get_const(&edit->string);

        const struct nk_style_item *background;
        struct nk_color background_color;
        struct nk_color text_color;
        nk_push_scissor(out, clip);
        if (*state & NK_WIDGET_STATE_ACTIVED) {
            background = &style->active;
            text_color = style->text_active;
        } else if (*state & NK_WIDGET_STATE_HOVER) {
            background = &style->hover;
            text_color = style->text_hover;
        } else {
            background = &style->normal;
            text_color = style->text_normal;
        }
        if (background->type == NK_STYLE_ITEM_IMAGE)
            background_color = nk_rgba(0,0,0,0);
        else
            background_color = background->data.color;

        background_color = nk_rgb_factor(background_color, style->color_factor);
        text_color = nk_rgb_factor(text_color, style->color_factor);

        nk_edit_draw_text(out, style, area.x - edit->scrollbar.x,
            area.y - edit->scrollbar.y, 0, begin, l, row_height, font,
            background_color, text_color, nk_false);
    }
    nk_push_scissor(out, old_clip);}
    return ret;
}
NK_API void
nk_edit_focus(struct nk_context *ctx, nk_flags flags)
{
    nk_hash hash;
    struct nk_window *win;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    if (!ctx || !ctx->current) return;

    win = ctx->current;
    hash = win->edit.seq;
    win->edit.active = nk_true;
    win->edit.name = hash;
    if (flags & NK_EDIT_ALWAYS_INSERT_MODE)
        win->edit.mode = NK_TEXT_EDIT_MODE_INSERT;
}
NK_API void
nk_edit_unfocus(struct nk_context *ctx)
{
    struct nk_window *win;
    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    if (!ctx || !ctx->current) return;

    win = ctx->current;
    win->edit.active = nk_false;
    win->edit.name = 0;
}
NK_API nk_flags
nk_edit_string(struct nk_context *ctx, nk_flags flags,
    char *memory, int *len, int max, nk_plugin_filter filter)
{
    nk_hash hash;
    nk_flags state;
    struct nk_text_edit *edit;
    struct nk_window *win;

    NK_ASSERT(ctx);
    NK_ASSERT(memory);
    NK_ASSERT(len);
    if (!ctx || !memory || !len)
        return 0;

    filter = (!filter) ? nk_filter_default: filter;
    win = ctx->current;
    hash = win->edit.seq;
    edit = &ctx->text_edit;
    nk_textedit_clear_state(&ctx->text_edit, (flags & NK_EDIT_MULTILINE)?
        NK_TEXT_EDIT_MULTI_LINE: NK_TEXT_EDIT_SINGLE_LINE, filter);

    if (win->edit.active && hash == win->edit.name) {
        if (flags & NK_EDIT_NO_CURSOR)
            edit->cursor = nk_utf_len(memory, *len);
        else edit->cursor = win->edit.cursor;
        if (!(flags & NK_EDIT_SELECTABLE)) {
            edit->select_start = win->edit.cursor;
            edit->select_end = win->edit.cursor;
        } else if(flags & NK_EDIT_AUTO_SELECT) {
            edit->select_start = 0;
            edit->select_end = *len;
        } else {
            edit->select_start = win->edit.sel_start;
            edit->select_end = win->edit.sel_end;
        }
        edit->mode = win->edit.mode;
        edit->scrollbar.x = (float)win->edit.scrollbar.x;
        edit->scrollbar.y = (float)win->edit.scrollbar.y;
        edit->active = nk_true;
    } else edit->active = nk_false;

    max = NK_MAX(1, max);
    *len = NK_MIN(*len, max-1);
    nk_str_init_fixed(&edit->string, memory, (nk_size)max);
    edit->string.buffer.allocated = (nk_size)*len;
    edit->string.len = nk_utf_len(memory, *len);
    state = nk_edit_buffer(ctx, flags, edit, filter);
    *len = (int)edit->string.buffer.allocated;

    if (edit->active) {
        win->edit.cursor = edit->cursor;
        win->edit.sel_start = edit->select_start;
        win->edit.sel_end = edit->select_end;
        win->edit.mode = edit->mode;
        win->edit.scrollbar.x = (nk_uint)edit->scrollbar.x;
        win->edit.scrollbar.y = (nk_uint)edit->scrollbar.y;
    } return state;
}
NK_API nk_flags
nk_edit_buffer(struct nk_context *ctx, nk_flags flags,
    struct nk_text_edit *edit, nk_plugin_filter filter)
{
    struct nk_window *win;
    struct nk_style *style;
    struct nk_input *in;

    enum nk_widget_layout_states state;
    struct nk_rect bounds;

    nk_flags ret_flags = 0;
    unsigned char prev_state;
    nk_hash hash;

    /* make sure correct values */
    NK_ASSERT(ctx);
    NK_ASSERT(edit);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    win = ctx->current;
    style = &ctx->style;
    state = nk_widget(&bounds, ctx);
    if (!state) return state;
    else if (state == NK_WIDGET_DISABLED)
        flags |= NK_EDIT_READ_ONLY;
    in = (win->layout->flags & NK_WINDOW_ROM) ? 0 : &ctx->input;

    /* check if edit is currently hot item */
    hash = win->edit.seq++;
    if (win->edit.active && hash == win->edit.name) {
        if (flags & NK_EDIT_NO_CURSOR)
            edit->cursor = edit->string.len;
        if (!(flags & NK_EDIT_SELECTABLE)) {
            edit->select_start = edit->cursor;
            edit->select_end = edit->cursor;
        }
        if (flags & NK_EDIT_CLIPBOARD)
            edit->clip = ctx->clip;
        edit->active = (unsigned char)win->edit.active;
    } else edit->active = nk_false;
    edit->mode = win->edit.mode;

    filter = (!filter) ? nk_filter_default: filter;
    prev_state = (unsigned char)edit->active;
    in = (flags & NK_EDIT_READ_ONLY) ? 0: in;
    ret_flags = nk_do_edit(&ctx->last_widget_state, &win->buffer, bounds, flags,
                    filter, edit, &style->edit, in, style->font);

    if (ctx->last_widget_state & NK_WIDGET_STATE_HOVER)
        ctx->style.cursor_active = ctx->style.cursors[NK_CURSOR_TEXT];
    if (edit->active && prev_state != edit->active) {
        /* current edit is now hot */
        win->edit.active = nk_true;
        win->edit.name = hash;
    } else if (prev_state && !edit->active) {
        /* current edit is now cold */
        win->edit.active = nk_false;
    } return ret_flags;
}
NK_API nk_flags
nk_edit_string_zero_terminated(struct nk_context *ctx, nk_flags flags,
    char *buffer, int max, nk_plugin_filter filter)
{
    nk_flags result;
    int len = nk_strlen(buffer);
    result = nk_edit_string(ctx, flags, buffer, &len, max, filter);
    buffer[NK_MIN(NK_MAX(max-1,0), len)] = '\0';
    return result;
}





/* ===============================================================
 *
 *                              PROPERTY
 *
 * ===============================================================*/
NK_LIB void
nk_drag_behavior(nk_flags *state, const struct nk_input *in,
    struct nk_rect drag, struct nk_property_variant *variant,
    float inc_per_pixel)
{
    int left_mouse_down = in && in->mouse.buttons[NK_BUTTON_LEFT].down;
    int left_mouse_click_in_cursor = in &&
        nk_input_has_mouse_click_down_in_rect(in, NK_BUTTON_LEFT, drag, nk_true);

    nk_widget_state_reset(state);
    if (nk_input_is_mouse_hovering_rect(in, drag))
        *state = NK_WIDGET_STATE_HOVERED;

    if (left_mouse_down && left_mouse_click_in_cursor) {
        float delta, pixels;
        pixels = in->mouse.delta.x;
        delta = pixels * inc_per_pixel;
        switch (variant->kind) {
        default: break;
        case NK_PROPERTY_INT:
            variant->value.i = variant->value.i + (int)delta;
            variant->value.i = NK_CLAMP(variant->min_value.i, variant->value.i, variant->max_value.i);
            break;
        case NK_PROPERTY_FLOAT:
            variant->value.f = variant->value.f + (float)delta;
            variant->value.f = NK_CLAMP(variant->min_value.f, variant->value.f, variant->max_value.f);
            break;
        case NK_PROPERTY_DOUBLE:
            variant->value.d = variant->value.d + (double)delta;
            variant->value.d = NK_CLAMP(variant->min_value.d, variant->value.d, variant->max_value.d);
            break;
        }
        *state = NK_WIDGET_STATE_ACTIVE;
    }
    if (*state & NK_WIDGET_STATE_HOVER && !nk_input_is_mouse_prev_hovering_rect(in, drag))
        *state |= NK_WIDGET_STATE_ENTERED;
    else if (nk_input_is_mouse_prev_hovering_rect(in, drag))
        *state |= NK_WIDGET_STATE_LEFT;
}
NK_LIB void
nk_property_behavior(nk_flags *ws, const struct nk_input *in,
    struct nk_rect property,  struct nk_rect label, struct nk_rect edit,
    struct nk_rect empty, int *state, struct nk_property_variant *variant,
    float inc_per_pixel)
{
    nk_widget_state_reset(ws);
    if (in && *state == NK_PROPERTY_DEFAULT) {
        if(in->focus_next)
        {
          ((struct nk_input *)in)->focus_next = 0;
          *state = NK_PROPERTY_EDIT;
          *ws = NK_WIDGET_STATE_ACTIVE;
        }
        else if (nk_button_behavior(ws, edit, in, NK_BUTTON_DEFAULT))
            *state = NK_PROPERTY_EDIT;
        else if (nk_input_is_mouse_click_down_in_rect(in, NK_BUTTON_LEFT, label, nk_true))
            *state = NK_PROPERTY_DRAG;
        else if (nk_input_is_mouse_click_down_in_rect(in, NK_BUTTON_LEFT, empty, nk_true))
            *state = NK_PROPERTY_DRAG;
    }
    if (*state == NK_PROPERTY_DRAG) {
        nk_drag_behavior(ws, in, property, variant, inc_per_pixel);
        if (!(*ws & NK_WIDGET_STATE_ACTIVED)) *state = NK_PROPERTY_DEFAULT;
    }
}
NK_LIB void
nk_draw_property(struct nk_command_buffer *out, const struct nk_style_property *style,
    const struct nk_rect *bounds, const struct nk_rect *label, nk_flags state,
    const char *name, int len, const struct nk_user_font *font)
{
    struct nk_text text;
    const struct nk_style_item *background;

    /* select correct background and text color */
    if (state & NK_WIDGET_STATE_ACTIVED) {
        background = &style->active;
        text.text = style->label_active;
    } else if (state & NK_WIDGET_STATE_HOVER) {
        background = &style->hover;
        text.text = style->label_hover;
    } else {
        background = &style->normal;
        text.text = style->label_normal;
    }

    text.text = nk_rgb_factor(text.text, style->color_factor);

    /* draw background */
    switch(background->type) {
        case NK_STYLE_ITEM_IMAGE:
            text.background = nk_rgba(0, 0, 0, 0);
            nk_draw_image(out, *bounds, &background->data.image, nk_rgb_factor(nk_white, style->color_factor));
            break;
        case NK_STYLE_ITEM_NINE_SLICE:
            text.background = nk_rgba(0, 0, 0, 0);
            nk_draw_nine_slice(out, *bounds, &background->data.slice, nk_rgb_factor(nk_white, style->color_factor));
            break;
        case NK_STYLE_ITEM_COLOR:
            text.background = background->data.color;
            nk_fill_rect(out, *bounds, style->rounding, nk_rgb_factor(background->data.color, style->color_factor));
            nk_stroke_rect(out, *bounds, style->rounding, style->border, nk_rgb_factor(background->data.color, style->color_factor));
            break;
    }

    /* draw label */
    text.padding = nk_vec2(0,0);
    if (name && name[0] != '#') {
        nk_widget_text(out, *label, name, len, &text, NK_TEXT_CENTERED, font);
    }
}
NK_INTERN void
nk_property_save(struct nk_property_variant *variant, char *buffer, int len)
{
    buffer[len] = '\0';
    switch (variant->kind) {
    default: break;
    case NK_PROPERTY_INT:
        variant->value.i = nk_strtoi(buffer, 0);
        // variant->value.i = NK_CLAMP(variant->min_value.i, variant->value.i, variant->max_value.i);
        break;
    case NK_PROPERTY_FLOAT:
        nk_string_float_limit(buffer, NK_MAX_FLOAT_PRECISION);
        variant->value.f = nk_strtof(buffer, 0);
        // variant->value.f = NK_CLAMP(variant->min_value.f, variant->value.f, variant->max_value.f);
        break;
    case NK_PROPERTY_DOUBLE:
        nk_string_float_limit(buffer, NK_MAX_FLOAT_PRECISION);
        variant->value.d = nk_strtod(buffer, 0);
        //variant->value.d = NK_CLAMP(variant->min_value.d, variant->value.d, variant->max_value.d);
        break;
    }
}
NK_LIB void
nk_do_property(nk_flags *ws,
    struct nk_command_buffer *out, struct nk_rect property,
    const char *name, struct nk_property_variant *variant,
    float inc_per_pixel, char *buffer, int *len,
    int *state, int *cursor, int *select_begin, int *select_end,
    const struct nk_style_property *style,
    enum nk_property_filter filter, struct nk_input *in,
    const struct nk_user_font *font, struct nk_text_edit *text_edit,
    enum nk_button_behavior behavior)
{
    const nk_plugin_filter filters[] = {
        nk_filter_decimal,
        nk_filter_float
    };
    nk_bool active, old;
    int num_len = 0, name_len = 0;
    char string[NK_MAX_NUMBER_BUFFER];
    float size;

    char *dst = 0;
    int *length;

    struct nk_rect left;
    struct nk_rect right;
    struct nk_rect label;
    struct nk_rect edit;
    struct nk_rect empty;

    /* left decrement button */
    left.h = font->height/2;
    left.w = left.h;
    left.x = property.x + style->border + style->padding.x;
    left.y = property.y + style->border + property.h/2.0f - left.h/2;

    /* text label */
    if (name && name[0] != '#') {
        name_len = nk_strlen(name);
    }
    size = font->width(font->userdata, font->height, name, name_len);
    label.x = left.x + left.w + style->padding.x;
    label.w = (float)size + 2 * style->padding.x;
    label.y = property.y + style->border + style->padding.y;
    label.h = property.h - (2 * style->border + 2 * style->padding.y);

    /* right increment button */
    right.y = left.y;
    right.w = left.w;
    right.h = left.h;
    right.x = property.x + property.w - (right.w + style->padding.x);

    /* edit */
    if (*state == NK_PROPERTY_EDIT) {
        size = font->width(font->userdata, font->height, buffer, *len);
        size += style->edit.cursor_size;
        length = len;
        dst = buffer;
    } else {
        switch (variant->kind) {
        default: break;
        case NK_PROPERTY_INT:
            nk_itoa(string, variant->value.i);
            num_len = nk_strlen(string);
            break;
        case NK_PROPERTY_FLOAT:
            NK_DTOA(string, (double)variant->value.f);
            num_len = nk_string_float_limit(string, NK_MAX_FLOAT_PRECISION);
            break;
        case NK_PROPERTY_DOUBLE:
            NK_DTOA(string, variant->value.d);
            num_len = nk_string_float_limit(string, NK_MAX_FLOAT_PRECISION);
            break;
        }
        size = font->width(font->userdata, font->height, string, num_len);
        dst = string;
        length = &num_len;
    }

    edit.w =  (float)size + 2 * style->padding.x;
    edit.w = NK_MIN(edit.w, right.x - (label.x + label.w));
    edit.x = right.x - (edit.w + style->padding.x);
    edit.y = property.y + style->border;
    edit.h = property.h - (2 * style->border);

    /* empty left space activator */
    empty.w = edit.x - (label.x + label.w);
    empty.x = label.x + label.w;
    empty.y = property.y;
    empty.h = property.h;

    /* update property */
    old = (*state == NK_PROPERTY_EDIT);
    nk_property_behavior(ws, in, property, label, edit, empty, state, variant, inc_per_pixel);

    /* draw property */
    if (style->draw_begin) style->draw_begin(out, style->userdata);
    nk_draw_property(out, style, &property, &label, *ws, name, name_len, font);
    if (style->draw_end) style->draw_end(out, style->userdata);

    /* execute right button  */
    if (nk_do_button_symbol(ws, out, left, style->sym_left, behavior, &style->dec_button, in, font)) {
        switch (variant->kind) {
        default: break;
        case NK_PROPERTY_INT:
            variant->value.i = NK_CLAMP(variant->min_value.i, variant->value.i - variant->step.i, variant->max_value.i); break;
        case NK_PROPERTY_FLOAT:
            variant->value.f = NK_CLAMP(variant->min_value.f, variant->value.f - variant->step.f, variant->max_value.f); break;
        case NK_PROPERTY_DOUBLE:
            variant->value.d = NK_CLAMP(variant->min_value.d, variant->value.d - variant->step.d, variant->max_value.d); break;
        }
    }
    /* execute left button  */
    if (nk_do_button_symbol(ws, out, right, style->sym_right, behavior, &style->inc_button, in, font)) {
        switch (variant->kind) {
        default: break;
        case NK_PROPERTY_INT:
            variant->value.i = NK_CLAMP(variant->min_value.i, variant->value.i + variant->step.i, variant->max_value.i); break;
        case NK_PROPERTY_FLOAT:
            variant->value.f = NK_CLAMP(variant->min_value.f, variant->value.f + variant->step.f, variant->max_value.f); break;
        case NK_PROPERTY_DOUBLE:
            variant->value.d = NK_CLAMP(variant->min_value.d, variant->value.d + variant->step.d, variant->max_value.d); break;
        }
    }
    if (!old && (*state == NK_PROPERTY_EDIT)) {
        /* property has been activated so setup buffer */
        NK_MEMCPY(buffer, dst, (nk_size)*length);
        *select_begin = *cursor = 0;
        *select_end = *len = *length;
        length = len;
        dst = buffer;
        active = 0;
    } else active = (*state == NK_PROPERTY_EDIT);

    /* execute and run text edit field */
    nk_textedit_clear_state(text_edit, NK_TEXT_EDIT_SINGLE_LINE, filters[filter]);
    text_edit->active = (unsigned char)active;
    text_edit->string.len = *length;
    text_edit->cursor = NK_CLAMP(0, *cursor, *length);
    text_edit->select_start = NK_CLAMP(0,*select_begin, *length);
    text_edit->select_end = NK_CLAMP(0,*select_end, *length);
    text_edit->string.buffer.allocated = (nk_size)*length;
    text_edit->string.buffer.memory.size = NK_MAX_NUMBER_BUFFER;
    text_edit->string.buffer.memory.ptr = dst;
    text_edit->string.buffer.size = NK_MAX_NUMBER_BUFFER;
    text_edit->mode = NK_TEXT_EDIT_MODE_INSERT;
    nk_do_edit(ws, out, edit, (int)NK_EDIT_FIELD|(int)NK_EDIT_AUTO_SELECT,
        filters[filter], text_edit, &style->edit, (*state == NK_PROPERTY_EDIT) ? in: 0, font);

    *length = text_edit->string.len;
    *cursor = text_edit->cursor;
    *select_begin = text_edit->select_start;
    *select_end = text_edit->select_end;
    if (text_edit->active && nk_input_is_key_pressed(in, NK_KEY_ENTER))
        text_edit->active = nk_false;

    if (active && !text_edit->active) {
        /* property is now not active so convert edit text to value*/
        *state = NK_PROPERTY_DEFAULT;
        nk_property_save(variant, buffer, *len);
    }
}
NK_API void
nk_property_focus(struct nk_context *ctx)
{ // do this *before* calling property, put focus_next flag on input
  ctx->input.focus_next = 1;
}
NK_API int
nk_property_float_unfocus(struct nk_context *ctx, const char *name, float min, float *val, float max, float step, int keypress)
{ // do this *after* calling property
  struct nk_property_variant variant = nk_property_variant_float(*val, min, max, step);
  if(keypress)
  {
    nk_hash hash = 0;
    /* calculate hash from name */
    if (name[0] == '#') {
      hash = nk_murmur_hash(name, (int)nk_strlen(name), ctx->current->property.seq-1);
      name++; /* special number hash */
    } else hash = nk_murmur_hash(name, (int)nk_strlen(name), 42);
    int hot = ctx->current->property.active &&
      (ctx->current->property.state == NK_PROPERTY_EDIT) &&
      (hash == ctx->current->property.name);
    if (hot)
    {
      ctx->current->property.state = NK_PROPERTY_DEFAULT;
      nk_property_save(&variant, ctx->current->property.buffer, ctx->current->property.length);
      *val = variant.value.f;
      return 1;
    }
  }
  return 0;
}
NK_API int
nk_property_int_unfocus(struct nk_context *ctx, const char *name, int min, int *val, int max, int step, int keypress)
{ // do this *after* calling property
  struct nk_property_variant variant = nk_property_variant_int(*val, min, max, step);
  if(keypress)
  {
    nk_hash hash = 0;
    /* calculate hash from name */
    if (name[0] == '#') {
      hash = nk_murmur_hash(name, (int)nk_strlen(name), ctx->current->property.seq-1);
      name++; /* special number hash */
    } else hash = nk_murmur_hash(name, (int)nk_strlen(name), 42);
    int hot = ctx->current->property.active &&
      (ctx->current->property.state == NK_PROPERTY_EDIT) &&
      (hash == ctx->current->property.name);
    if (hot)
    {
      ctx->current->property.state = NK_PROPERTY_DEFAULT;
      nk_property_save(&variant, ctx->current->property.buffer, ctx->current->property.length);
      *val = variant.value.i;
      return 1;
    }
  }
  return 0;
}

NK_LIB struct nk_property_variant
nk_property_variant_int(int value, int min_value, int max_value, int step)
{
    struct nk_property_variant result;
    result.kind = NK_PROPERTY_INT;
    result.value.i = value;
    result.min_value.i = min_value;
    result.max_value.i = max_value;
    result.step.i = step;
    return result;
}
NK_LIB struct nk_property_variant
nk_property_variant_float(float value, float min_value, float max_value, float step)
{
    struct nk_property_variant result;
    result.kind = NK_PROPERTY_FLOAT;
    result.value.f = value;
    result.min_value.f = min_value;
    result.max_value.f = max_value;
    result.step.f = step;
    return result;
}
NK_LIB struct nk_property_variant
nk_property_variant_double(double value, double min_value, double max_value,
    double step)
{
    struct nk_property_variant result;
    result.kind = NK_PROPERTY_DOUBLE;
    result.value.d = value;
    result.min_value.d = min_value;
    result.max_value.d = max_value;
    result.step.d = step;
    return result;
}
NK_LIB void
nk_property(struct nk_context *ctx, const char *name, struct nk_property_variant *variant,
    float inc_per_pixel, const enum nk_property_filter filter)
{
    struct nk_window *win;
    struct nk_panel *layout;
    struct nk_input *in;
    const struct nk_style *style;

    struct nk_rect bounds;
    enum nk_widget_layout_states s;
    nk_bool hot;

    int *state = 0;
    nk_hash hash = 0;
    char *buffer = 0;
    int *len = 0;
    int *cursor = 0;
    int *select_begin = 0;
    int *select_end = 0;
    int old_state;
    int prev_state;

    char dummy_buffer[NK_MAX_NUMBER_BUFFER];
    int dummy_state = NK_PROPERTY_DEFAULT;
    int dummy_length = 0;
    int dummy_cursor = 0;
    int dummy_select_begin = 0;
    int dummy_select_end = 0;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    win = ctx->current;
    layout = win->layout;
    style = &ctx->style;
    s = nk_widget(&bounds, ctx);
    if (!s) return;

    /* calculate hash from name */
    if (name[0] == '#') {
        hash = nk_murmur_hash(name, (int)nk_strlen(name), win->property.seq++);
        name++; /* special number hash */
    } else hash = nk_murmur_hash(name, (int)nk_strlen(name), 42);

    /* check if property is previously hot */
    if (win->property.prev_state == NK_PROPERTY_EDIT && hash == win->property.prev_name) {
        nk_property_save(variant, win->property.prev_buffer, win->property.prev_length);
        win->property.prev_state = NK_PROPERTY_DEFAULT;
    }

    /* check if property is currently hot item */
    hot = win->property.active && hash == win->property.name;
    if (hot) {
        buffer = win->property.buffer;
        len = &win->property.length;
        cursor = &win->property.cursor;
        state = &win->property.state;
        select_begin = &win->property.select_start;
        select_end = &win->property.select_end;
    } else {
        buffer = dummy_buffer;
        len = &dummy_length;
        cursor = &dummy_cursor;
        state = &dummy_state;
        select_begin =  &dummy_select_begin;
        select_end = &dummy_select_end;
    }

    /* execute property widget */
    old_state = *state;
    prev_state = win->property.state;
    ctx->text_edit.clip = ctx->clip;
    in = ((s == NK_WIDGET_ROM && !win->property.active) ||
        layout->flags & NK_WINDOW_ROM || s == NK_WIDGET_DISABLED) ? 0 : &ctx->input;
    nk_do_property(&ctx->last_widget_state, &win->buffer, bounds, name,
        variant, inc_per_pixel, buffer, len, state, cursor, select_begin,
        select_end, &style->property, filter, in, style->font, &ctx->text_edit,
        ctx->button_behavior);

    if (in && *state != NK_PROPERTY_DEFAULT && !hot) {
        /* another property was active */
        if (win->property.active /* && hash != win->property.name */) {
            win->property.prev_state = prev_state;
            win->property.prev_name = win->property.name;
            win->property.prev_length = win->property.length;
            NK_MEMCPY(win->property.prev_buffer, win->property.buffer, win->property.length);
        }
        /* current property is now hot */
        win->property.active = 1;
        NK_MEMCPY(win->property.buffer, buffer, (nk_size)*len);
        win->property.length = *len;
        win->property.cursor = *cursor;
        win->property.state = *state;
        win->property.name = hash;
        win->property.select_start = *select_begin;
        win->property.select_end = *select_end;
        if (*state == NK_PROPERTY_DRAG) {
            ctx->input.mouse.grab = nk_true;
            ctx->input.mouse.grabbed = nk_true;
        }
    }
    /* check if previously active property is now inactive */
    if (*state == NK_PROPERTY_DEFAULT && old_state != NK_PROPERTY_DEFAULT) {
        if (old_state == NK_PROPERTY_DRAG) {
            ctx->input.mouse.grab = nk_false;
            ctx->input.mouse.grabbed = nk_false;
            ctx->input.mouse.ungrab = nk_true;
        }
        win->property.select_start = 0;
        win->property.select_end = 0;
        win->property.active = 0;
    }
}
NK_API void
nk_property_int(struct nk_context *ctx, const char *name,
    int min, int *val, int max, int step, float inc_per_pixel)
{
    struct nk_property_variant variant;
    NK_ASSERT(ctx);
    NK_ASSERT(name);
    NK_ASSERT(val);

    if (!ctx || !ctx->current || !name || !val) return;
    variant = nk_property_variant_int(*val, min, max, step);
    nk_property(ctx, name, &variant, inc_per_pixel, NK_FILTER_INT);
    *val = variant.value.i;
}
NK_API void
nk_property_float(struct nk_context *ctx, const char *name,
    float min, float *val, float max, float step, float inc_per_pixel)
{
    struct nk_property_variant variant;
    NK_ASSERT(ctx);
    NK_ASSERT(name);
    NK_ASSERT(val);

    if (!ctx || !ctx->current || !name || !val) return;
    variant = nk_property_variant_float(*val, min, max, step);
    nk_property(ctx, name, &variant, inc_per_pixel, NK_FILTER_FLOAT);
    *val = variant.value.f;
}
NK_API void
nk_property_double(struct nk_context *ctx, const char *name,
    double min, double *val, double max, double step, float inc_per_pixel)
{
    struct nk_property_variant variant;
    NK_ASSERT(ctx);
    NK_ASSERT(name);
    NK_ASSERT(val);

    if (!ctx || !ctx->current || !name || !val) return;
    variant = nk_property_variant_double(*val, min, max, step);
    nk_property(ctx, name, &variant, inc_per_pixel, NK_FILTER_FLOAT);
    *val = variant.value.d;
}
NK_API int
nk_propertyi(struct nk_context *ctx, const char *name, int min, int val,
    int max, int step, float inc_per_pixel)
{
    struct nk_property_variant variant;
    NK_ASSERT(ctx);
    NK_ASSERT(name);

    if (!ctx || !ctx->current || !name) return val;
    variant = nk_property_variant_int(val, min, max, step);
    nk_property(ctx, name, &variant, inc_per_pixel, NK_FILTER_INT);
    val = variant.value.i;
    return val;
}
NK_API float
nk_propertyf(struct nk_context *ctx, const char *name, float min,
    float val, float max, float step, float inc_per_pixel)
{
    struct nk_property_variant variant;
    NK_ASSERT(ctx);
    NK_ASSERT(name);

    if (!ctx || !ctx->current || !name) return val;
    variant = nk_property_variant_float(val, min, max, step);
    nk_property(ctx, name, &variant, inc_per_pixel, NK_FILTER_FLOAT);
    val = variant.value.f;
    return val;
}
NK_API double
nk_propertyd(struct nk_context *ctx, const char *name, double min,
    double val, double max, double step, float inc_per_pixel)
{
    struct nk_property_variant variant;
    NK_ASSERT(ctx);
    NK_ASSERT(name);

    if (!ctx || !ctx->current || !name) return val;
    variant = nk_property_variant_double(val, min, max, step);
    nk_property(ctx, name, &variant, inc_per_pixel, NK_FILTER_FLOAT);
    val = variant.value.d;
    return val;
}





/* ==============================================================
 *
 *                          CHART
 *
 * ===============================================================*/
NK_API nk_bool
nk_chart_begin_colored(struct nk_context *ctx, enum nk_chart_type type,
    struct nk_color color, struct nk_color highlight,
    int count, float min_value, float max_value)
{
    struct nk_window *win;
    struct nk_chart *chart;
    const struct nk_style *config;
    const struct nk_style_chart *style;

    const struct nk_style_item *background;
    struct nk_rect bounds = {0, 0, 0, 0};

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);

    if (!ctx || !ctx->current || !ctx->current->layout) return 0;
    if (!nk_widget(&bounds, ctx)) {
        chart = &ctx->current->layout->chart;
        nk_zero(chart, sizeof(*chart));
        return 0;
    }

    win = ctx->current;
    config = &ctx->style;
    chart = &win->layout->chart;
    style = &config->chart;

    /* setup basic generic chart  */
    nk_zero(chart, sizeof(*chart));
    chart->x = bounds.x + style->padding.x;
    chart->y = bounds.y + style->padding.y;
    chart->w = bounds.w - 2 * style->padding.x;
    chart->h = bounds.h - 2 * style->padding.y;
    chart->w = NK_MAX(chart->w, 2 * style->padding.x);
    chart->h = NK_MAX(chart->h, 2 * style->padding.y);

    /* add first slot into chart */
    {struct nk_chart_slot *slot = &chart->slots[chart->slot++];
    slot->type = type;
    slot->count = count;
    slot->color = nk_rgb_factor(color, style->color_factor);
    slot->highlight = highlight;
    slot->min = NK_MIN(min_value, max_value);
    slot->max = NK_MAX(min_value, max_value);
    slot->range = slot->max - slot->min;
    slot->show_markers = style->show_markers;}

    /* draw chart background */
    background = &style->background;

    switch(background->type) {
        case NK_STYLE_ITEM_IMAGE:
            nk_draw_image(&win->buffer, bounds, &background->data.image, nk_rgb_factor(nk_white, style->color_factor));
            break;
        case NK_STYLE_ITEM_NINE_SLICE:
            nk_draw_nine_slice(&win->buffer, bounds, &background->data.slice, nk_rgb_factor(nk_white, style->color_factor));
            break;
        case NK_STYLE_ITEM_COLOR:
            nk_fill_rect(&win->buffer, bounds, style->rounding, nk_rgb_factor(style->border_color, style->color_factor));
            nk_fill_rect(&win->buffer, nk_shrink_rect(bounds, style->border),
                style->rounding, nk_rgb_factor(style->background.data.color, style->color_factor));
            break;
    }
    return 1;
}
NK_API nk_bool
nk_chart_begin(struct nk_context *ctx, const enum nk_chart_type type,
    int count, float min_value, float max_value)
{
    return nk_chart_begin_colored(ctx, type, ctx->style.chart.color,
                ctx->style.chart.selected_color, count, min_value, max_value);
}
NK_API void
nk_chart_add_slot_colored(struct nk_context *ctx, const enum nk_chart_type type,
    struct nk_color color, struct nk_color highlight,
    int count, float min_value, float max_value)
{
    const struct nk_style_chart* style;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    NK_ASSERT(ctx->current->layout->chart.slot < NK_CHART_MAX_SLOT);
    if (!ctx || !ctx->current || !ctx->current->layout) return;
    if (ctx->current->layout->chart.slot >= NK_CHART_MAX_SLOT) return;

    style = &ctx->style.chart;

    /* add another slot into the graph */
    {struct nk_chart *chart = &ctx->current->layout->chart;
    struct nk_chart_slot *slot = &chart->slots[chart->slot++];
    slot->type = type;
    slot->count = count;
    slot->color = nk_rgb_factor(color, style->color_factor);
    slot->highlight = highlight;
    slot->min = NK_MIN(min_value, max_value);
    slot->max = NK_MAX(min_value, max_value);
    slot->range = slot->max - slot->min;
    slot->show_markers = style->show_markers;}
}
NK_API void
nk_chart_add_slot(struct nk_context *ctx, const enum nk_chart_type type,
    int count, float min_value, float max_value)
{
    nk_chart_add_slot_colored(ctx, type, ctx->style.chart.color,
        ctx->style.chart.selected_color, count, min_value, max_value);
}
NK_INTERN nk_flags
nk_chart_push_line(struct nk_context *ctx, struct nk_window *win,
    struct nk_chart *g, float value, int slot)
{
    struct nk_panel *layout = win->layout;
    const struct nk_input *i = ctx->current->widgets_disabled ? 0 : &ctx->input;
    struct nk_command_buffer *out = &win->buffer;

    nk_flags ret = 0;
    struct nk_vec2 cur;
    struct nk_rect bounds;
    struct nk_color color;
    float step;
    float range;
    float ratio;

    NK_ASSERT(slot >= 0 && slot < NK_CHART_MAX_SLOT);
    step = g->w / (float)g->slots[slot].count;
    range = g->slots[slot].max - g->slots[slot].min;
    ratio = (value - g->slots[slot].min) / range;

    if (g->slots[slot].index == 0) {
        /* first data point does not have a connection */
        g->slots[slot].last.x = g->x;
        g->slots[slot].last.y = (g->y + g->h) - ratio * (float)g->h;

        bounds.x = g->slots[slot].last.x - 2;
        bounds.y = g->slots[slot].last.y - 2;
        bounds.w = bounds.h = 4;

        color = g->slots[slot].color;
        if (!(layout->flags & NK_WINDOW_ROM) && i &&
            NK_INBOX(i->mouse.pos.x,i->mouse.pos.y, g->slots[slot].last.x-3, g->slots[slot].last.y-3, 6, 6)){
            ret = nk_input_is_mouse_hovering_rect(i, bounds) ? NK_CHART_HOVERING : 0;
            ret |= (i->mouse.buttons[NK_BUTTON_LEFT].down &&
                i->mouse.buttons[NK_BUTTON_LEFT].clicked) ? NK_CHART_CLICKED: 0;
            color = g->slots[slot].highlight;
        }
        if (g->slots[slot].show_markers) {
            nk_fill_rect(out, bounds, 0, color);
        }
        g->slots[slot].index += 1;
        return ret;
    }

    /* draw a line between the last data point and the new one */
    color = g->slots[slot].color;
    cur.x = g->x + (float)(step * (float)g->slots[slot].index);
    cur.y = (g->y + g->h) - (ratio * (float)g->h);
    nk_stroke_line(out, g->slots[slot].last.x, g->slots[slot].last.y, cur.x, cur.y, 1.0f, color);

    bounds.x = cur.x - 3;
    bounds.y = cur.y - 3;
    bounds.w = bounds.h = 6;

    /* user selection of current data point */
    if (!(layout->flags & NK_WINDOW_ROM)) {
        if (nk_input_is_mouse_hovering_rect(i, bounds)) {
            ret = NK_CHART_HOVERING;
            ret |= (!i->mouse.buttons[NK_BUTTON_LEFT].down &&
                i->mouse.buttons[NK_BUTTON_LEFT].clicked) ? NK_CHART_CLICKED: 0;
            color = g->slots[slot].highlight;
        }
    }
    if (g->slots[slot].show_markers) {
        nk_fill_rect(out, nk_rect(cur.x - 2, cur.y - 2, 4, 4), 0, color);
    }

    /* save current data point position */
    g->slots[slot].last.x = cur.x;
    g->slots[slot].last.y = cur.y;
    g->slots[slot].index  += 1;
    return ret;
}
NK_INTERN nk_flags
nk_chart_push_column(const struct nk_context *ctx, struct nk_window *win,
    struct nk_chart *chart, float value, int slot)
{
    struct nk_command_buffer *out = &win->buffer;
    const struct nk_input *in = ctx->current->widgets_disabled ? 0 : &ctx->input;
    struct nk_panel *layout = win->layout;

    float ratio;
    nk_flags ret = 0;
    struct nk_color color;
    struct nk_rect item = {0,0,0,0};

    NK_ASSERT(slot >= 0 && slot < NK_CHART_MAX_SLOT);
    if (chart->slots[slot].index  >= chart->slots[slot].count)
        return nk_false;
    if (chart->slots[slot].count) {
        float padding = (float)(chart->slots[slot].count-1);
        item.w = (chart->w - padding) / (float)(chart->slots[slot].count);
    }

    /* calculate bounds of current bar chart entry */
    color = chart->slots[slot].color;;
    item.h = chart->h * NK_ABS((value/chart->slots[slot].range));
    if (value >= 0) {
        ratio = (value + NK_ABS(chart->slots[slot].min)) / NK_ABS(chart->slots[slot].range);
        item.y = (chart->y + chart->h) - chart->h * ratio;
    } else {
        ratio = (value - chart->slots[slot].max) / chart->slots[slot].range;
        item.y = chart->y + (chart->h * NK_ABS(ratio)) - item.h;
    }
    item.x = chart->x + ((float)chart->slots[slot].index * item.w);
    item.x = item.x + ((float)chart->slots[slot].index);

    /* user chart bar selection */
    if (!(layout->flags & NK_WINDOW_ROM) && in &&
        NK_INBOX(in->mouse.pos.x,in->mouse.pos.y,item.x,item.y,item.w,item.h)) {
        ret = NK_CHART_HOVERING;
        ret |= (!in->mouse.buttons[NK_BUTTON_LEFT].down &&
                in->mouse.buttons[NK_BUTTON_LEFT].clicked) ? NK_CHART_CLICKED: 0;
        color = chart->slots[slot].highlight;
    }
    nk_fill_rect(out, item, 0, color);
    chart->slots[slot].index += 1;
    return ret;
}
NK_API nk_flags
nk_chart_push_slot(struct nk_context *ctx, float value, int slot)
{
    nk_flags flags;
    struct nk_window *win;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(slot >= 0 && slot < NK_CHART_MAX_SLOT);
    NK_ASSERT(slot < ctx->current->layout->chart.slot);
    if (!ctx || !ctx->current || slot >= NK_CHART_MAX_SLOT) return nk_false;
    if (slot >= ctx->current->layout->chart.slot) return nk_false;

    win = ctx->current;
    if (win->layout->chart.slot < slot) return nk_false;
    switch (win->layout->chart.slots[slot].type) {
    case NK_CHART_LINES:
        flags = nk_chart_push_line(ctx, win, &win->layout->chart, value, slot); break;
    case NK_CHART_COLUMN:
        flags = nk_chart_push_column(ctx, win, &win->layout->chart, value, slot); break;
    default:
    case NK_CHART_MAX:
        flags = 0;
    }
    return flags;
}
NK_API nk_flags
nk_chart_push(struct nk_context *ctx, float value)
{
    return nk_chart_push_slot(ctx, value, 0);
}
NK_API void
nk_chart_end(struct nk_context *ctx)
{
    struct nk_window *win;
    struct nk_chart *chart;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    if (!ctx || !ctx->current)
        return;

    win = ctx->current;
    chart = &win->layout->chart;
    NK_MEMSET(chart, 0, sizeof(*chart));
    return;
}
NK_API void
nk_plot(struct nk_context *ctx, enum nk_chart_type type, const float *values,
    int count, int offset)
{
    int i = 0;
    float min_value;
    float max_value;

    NK_ASSERT(ctx);
    NK_ASSERT(values);
    if (!ctx || !values || !count) return;

    min_value = values[offset];
    max_value = values[offset];
    for (i = 0; i < count; ++i) {
        min_value = NK_MIN(values[i + offset], min_value);
        max_value = NK_MAX(values[i + offset], max_value);
    }

    if (nk_chart_begin(ctx, type, count, min_value, max_value)) {
        for (i = 0; i < count; ++i)
            nk_chart_push(ctx, values[i + offset]);
        nk_chart_end(ctx);
    }
}
NK_API void
nk_plot_function(struct nk_context *ctx, enum nk_chart_type type, void *userdata,
    float(*value_getter)(void* user, int index), int count, int offset)
{
    int i = 0;
    float min_value;
    float max_value;

    NK_ASSERT(ctx);
    NK_ASSERT(value_getter);
    if (!ctx || !value_getter || !count) return;

    max_value = min_value = value_getter(userdata, offset);
    for (i = 0; i < count; ++i) {
        float value = value_getter(userdata, i + offset);
        min_value = NK_MIN(value, min_value);
        max_value = NK_MAX(value, max_value);
    }

    if (nk_chart_begin(ctx, type, count, min_value, max_value)) {
        for (i = 0; i < count; ++i)
            nk_chart_push(ctx, value_getter(userdata, i + offset));
        nk_chart_end(ctx);
    }
}





/* ==============================================================
 *
 *                          COLOR PICKER
 *
 * ===============================================================*/
NK_LIB nk_bool
nk_color_picker_behavior(nk_flags *state,
    const struct nk_rect *bounds, const struct nk_rect *matrix,
    const struct nk_rect *hue_bar, const struct nk_rect *alpha_bar,
    struct nk_colorf *color, const struct nk_input *in)
{
    float hsva[4];
    nk_bool value_changed = 0;
    nk_bool hsv_changed = 0;

    NK_ASSERT(state);
    NK_ASSERT(matrix);
    NK_ASSERT(hue_bar);
    NK_ASSERT(color);

    /* color matrix */
    nk_colorf_hsva_fv(hsva, *color);
    if (nk_button_behavior(state, *matrix, in, NK_BUTTON_REPEATER)) {
        hsva[1] = NK_SATURATE((in->mouse.pos.x - matrix->x) / (matrix->w-1));
        hsva[2] = 1.0f - NK_SATURATE((in->mouse.pos.y - matrix->y) / (matrix->h-1));
        value_changed = hsv_changed = 1;
    }
    /* hue bar */
    if (nk_button_behavior(state, *hue_bar, in, NK_BUTTON_REPEATER)) {
        hsva[0] = NK_SATURATE((in->mouse.pos.y - hue_bar->y) / (hue_bar->h-1));
        value_changed = hsv_changed = 1;
    }
    /* alpha bar */
    if (alpha_bar) {
        if (nk_button_behavior(state, *alpha_bar, in, NK_BUTTON_REPEATER)) {
            hsva[3] = 1.0f - NK_SATURATE((in->mouse.pos.y - alpha_bar->y) / (alpha_bar->h-1));
            value_changed = 1;
        }
    }
    nk_widget_state_reset(state);
    if (hsv_changed) {
        *color = nk_hsva_colorfv(hsva);
        *state = NK_WIDGET_STATE_ACTIVE;
    }
    if (value_changed) {
        color->a = hsva[3];
        *state = NK_WIDGET_STATE_ACTIVE;
    }
    /* set color picker widget state */
    if (nk_input_is_mouse_hovering_rect(in, *bounds))
        *state = NK_WIDGET_STATE_HOVERED;
    if (*state & NK_WIDGET_STATE_HOVER && !nk_input_is_mouse_prev_hovering_rect(in, *bounds))
        *state |= NK_WIDGET_STATE_ENTERED;
    else if ((!in || (in->mouse.buttons[NK_BUTTON_LEFT].down == nk_false)) && nk_input_is_mouse_prev_hovering_rect(in, *bounds))
        *state |= NK_WIDGET_STATE_LEFT;
    return value_changed;
}
NK_LIB void
nk_draw_color_picker(struct nk_command_buffer *o, const struct nk_rect *matrix,
    const struct nk_rect *hue_bar, const struct nk_rect *alpha_bar,
    struct nk_colorf col)
{
    NK_STORAGE const struct nk_color black = {0,0,0,255};
    NK_STORAGE const struct nk_color white = {255, 255, 255, 255};
    NK_STORAGE const struct nk_color black_trans = {0,0,0,0};

    const float crosshair_size = 7.0f;
    struct nk_color temp;
    float hsva[4];
    float line_y;
    int i;

    NK_ASSERT(o);
    NK_ASSERT(matrix);
    NK_ASSERT(hue_bar);

    /* draw hue bar */
    nk_colorf_hsva_fv(hsva, col);
    for (i = 0; i < 6; ++i) {
        NK_GLOBAL const struct nk_color hue_colors[] = {
            {255, 0, 0, 255}, {255,255,0,255}, {0,255,0,255}, {0, 255,255,255},
            {0,0,255,255}, {255, 0, 255, 255}, {255, 0, 0, 255}
        };
        nk_fill_rect_multi_color(o,
            nk_rect(hue_bar->x, hue_bar->y + (float)i * (hue_bar->h/6.0f) + 0.5f,
                hue_bar->w, (hue_bar->h/6.0f) + 0.5f), hue_colors[i], hue_colors[i],
                hue_colors[i+1], hue_colors[i+1]);
    }
    line_y = (float)(int)(hue_bar->y + hsva[0] * matrix->h + 0.5f);
    nk_stroke_line(o, hue_bar->x-1, line_y, hue_bar->x + hue_bar->w + 2,
        line_y, 1, nk_rgb(255,255,255));

    /* draw alpha bar */
    if (alpha_bar) {
        float alpha = NK_SATURATE(col.a);
        line_y = (float)(int)(alpha_bar->y +  (1.0f - alpha) * matrix->h + 0.5f);

        nk_fill_rect_multi_color(o, *alpha_bar, white, white, black, black);
        nk_stroke_line(o, alpha_bar->x-1, line_y, alpha_bar->x + alpha_bar->w + 2,
            line_y, 1, nk_rgb(255,255,255));
    }

    /* draw color matrix */
    temp = nk_hsv_f(hsva[0], 1.0f, 1.0f);
    nk_fill_rect_multi_color(o, *matrix, white, temp, temp, white);
    nk_fill_rect_multi_color(o, *matrix, black_trans, black_trans, black, black);

    /* draw cross-hair */
    {struct nk_vec2 p; float S = hsva[1]; float V = hsva[2];
    p.x = (float)(int)(matrix->x + S * matrix->w);
    p.y = (float)(int)(matrix->y + (1.0f - V) * matrix->h);
    nk_stroke_line(o, p.x - crosshair_size, p.y, p.x-2, p.y, 1.0f, white);
    nk_stroke_line(o, p.x + crosshair_size + 1, p.y, p.x+3, p.y, 1.0f, white);
    nk_stroke_line(o, p.x, p.y + crosshair_size + 1, p.x, p.y+3, 1.0f, white);
    nk_stroke_line(o, p.x, p.y - crosshair_size, p.x, p.y-2, 1.0f, white);}
}
NK_LIB nk_bool
nk_do_color_picker(nk_flags *state,
    struct nk_command_buffer *out, struct nk_colorf *col,
    enum nk_color_format fmt, struct nk_rect bounds,
    struct nk_vec2 padding, const struct nk_input *in,
    const struct nk_user_font *font)
{
    int ret = 0;
    struct nk_rect matrix;
    struct nk_rect hue_bar;
    struct nk_rect alpha_bar;
    float bar_w;

    NK_ASSERT(out);
    NK_ASSERT(col);
    NK_ASSERT(state);
    NK_ASSERT(font);
    if (!out || !col || !state || !font)
        return ret;

    bar_w = font->height;
    bounds.x += padding.x;
    bounds.y += padding.x;
    bounds.w -= 2 * padding.x;
    bounds.h -= 2 * padding.y;

    matrix.x = bounds.x;
    matrix.y = bounds.y;
    matrix.h = bounds.h;
    matrix.w = bounds.w - (3 * padding.x + 2 * bar_w);

    hue_bar.w = bar_w;
    hue_bar.y = bounds.y;
    hue_bar.h = matrix.h;
    hue_bar.x = matrix.x + matrix.w + padding.x;

    alpha_bar.x = hue_bar.x + hue_bar.w + padding.x;
    alpha_bar.y = bounds.y;
    alpha_bar.w = bar_w;
    alpha_bar.h = matrix.h;

    ret = nk_color_picker_behavior(state, &bounds, &matrix, &hue_bar,
        (fmt == NK_RGBA) ? &alpha_bar:0, col, in);
    nk_draw_color_picker(out, &matrix, &hue_bar, (fmt == NK_RGBA) ? &alpha_bar:0, *col);
    return ret;
}
NK_API nk_bool
nk_color_pick(struct nk_context * ctx, struct nk_colorf *color,
    enum nk_color_format fmt)
{
    struct nk_window *win;
    struct nk_panel *layout;
    const struct nk_style *config;
    const struct nk_input *in;

    enum nk_widget_layout_states state;
    struct nk_rect bounds;

    NK_ASSERT(ctx);
    NK_ASSERT(color);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout || !color)
        return 0;

    win = ctx->current;
    config = &ctx->style;
    layout = win->layout;
    state = nk_widget(&bounds, ctx);
    if (!state) return 0;
    in = (state == NK_WIDGET_ROM || state == NK_WIDGET_DISABLED || layout->flags & NK_WINDOW_ROM) ? 0 : &ctx->input;
    return nk_do_color_picker(&ctx->last_widget_state, &win->buffer, color, fmt, bounds,
                nk_vec2(0,0), in, config->font);
}
NK_API struct nk_colorf
nk_color_picker(struct nk_context *ctx, struct nk_colorf color,
    enum nk_color_format fmt)
{
    nk_color_pick(ctx, &color, fmt);
    return color;
}





/* ==============================================================
 *
 *                          COMBO
 *
 * ===============================================================*/
NK_INTERN nk_bool
nk_combo_begin(struct nk_context *ctx, struct nk_window *win,
    struct nk_vec2 size, nk_bool is_clicked, struct nk_rect header)
{
    struct nk_window *popup;
    int is_open = 0;
    int is_active = 0;
    struct nk_rect body;
    nk_hash hash;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    popup = win->popup.win;
    body.x = header.x;
    body.w = size.x;
    body.y = header.y + header.h-ctx->style.window.combo_border;
    body.h = size.y;

    hash = win->popup.combo_count++;
    is_open = (popup) ? nk_true:nk_false;
    is_active = (popup && (win->popup.name == hash) && win->popup.type == NK_PANEL_COMBO);
    if ((is_clicked && is_open && !is_active) || (is_open && !is_active) ||
        (!is_open && !is_active && !is_clicked)) return 0;
    if (!nk_nonblock_begin(ctx, 0, body,
        (is_clicked && is_open)?nk_rect(0,0,0,0):header, NK_PANEL_COMBO)) return 0;

    win->popup.type = NK_PANEL_COMBO;
    win->popup.name = hash;
    return 1;
}
NK_API nk_bool
nk_combo_begin_text(struct nk_context *ctx, const char *selected, int len,
    struct nk_vec2 size)
{
    const struct nk_input *in;
    struct nk_window *win;
    struct nk_style *style;

    enum nk_widget_layout_states s;
    int is_clicked = nk_false;
    struct nk_rect header;
    const struct nk_style_item *background;
    struct nk_text text;

    NK_ASSERT(ctx);
    NK_ASSERT(selected);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout || !selected)
        return 0;

    win = ctx->current;
    style = &ctx->style;
    s = nk_widget(&header, ctx);
    if (s == NK_WIDGET_INVALID)
        return 0;

    in = (win->layout->flags & NK_WINDOW_ROM || s == NK_WIDGET_DISABLED || s == NK_WIDGET_ROM)? 0: &ctx->input;
    if (nk_button_behavior(&ctx->last_widget_state, header, in, NK_BUTTON_DEFAULT))
        is_clicked = nk_true;

    /* draw combo box header background and border */
    if (ctx->last_widget_state & NK_WIDGET_STATE_ACTIVED) {
        background = &style->combo.active;
        text.text = style->combo.label_active;
    } else if (ctx->last_widget_state & NK_WIDGET_STATE_HOVER) {
        background = &style->combo.hover;
        text.text = style->combo.label_hover;
    } else {
        background = &style->combo.normal;
        text.text = style->combo.label_normal;
    }

    text.text = nk_rgb_factor(text.text, style->combo.color_factor);

    switch(background->type) {
        case NK_STYLE_ITEM_IMAGE:
            text.background = nk_rgba(0, 0, 0, 0);
            nk_draw_image(&win->buffer, header, &background->data.image, nk_rgb_factor(nk_white, style->combo.color_factor));
            break;
        case NK_STYLE_ITEM_NINE_SLICE:
            text.background = nk_rgba(0, 0, 0, 0);
            nk_draw_nine_slice(&win->buffer, header, &background->data.slice, nk_rgb_factor(nk_white, style->combo.color_factor));
            break;
        case NK_STYLE_ITEM_COLOR:
            text.background = background->data.color;
            nk_fill_rect(&win->buffer, header, style->combo.rounding, nk_rgb_factor(background->data.color, style->combo.color_factor));
            nk_stroke_rect(&win->buffer, header, style->combo.rounding, style->combo.border, nk_rgb_factor(style->combo.border_color, style->combo.color_factor));
            break;
    }
    {
        /* print currently selected text item */
        struct nk_rect label;
        struct nk_rect button;
        struct nk_rect content;
        int draw_button_symbol;

        enum nk_symbol_type sym;
        if (ctx->last_widget_state & NK_WIDGET_STATE_HOVER)
            sym = style->combo.sym_hover;
        else if (is_clicked)
            sym = style->combo.sym_active;
        else
            sym = style->combo.sym_normal;

        /* represents whether or not the combo's button symbol should be drawn */
        draw_button_symbol = sym != NK_SYMBOL_NONE;

        /* calculate button */
        button.w = header.h - 2 * style->combo.button_padding.y;
        button.x = (header.x + header.w - header.h) - style->combo.button_padding.x;
        button.y = header.y + style->combo.button_padding.y;
        button.h = button.w;

        content.x = button.x + style->combo.button.padding.x;
        content.y = button.y + style->combo.button.padding.y;
        content.w = button.w - 2 * style->combo.button.padding.x;
        content.h = button.h - 2 * style->combo.button.padding.y;

        /* draw selected label */
        text.padding = nk_vec2(0,0);
        label.x = header.x + style->combo.content_padding.x;
        label.y = header.y + style->combo.content_padding.y;
        label.h = header.h - 2 * style->combo.content_padding.y;
        if (draw_button_symbol)
            label.w = button.x - (style->combo.content_padding.x + style->combo.spacing.x) - label.x;
        else
            label.w = header.w - 2 * style->combo.content_padding.x;
        nk_widget_text(&win->buffer, label, selected, len, &text,
            NK_TEXT_LEFT, ctx->style.font);

        /* draw open/close button */
        if (draw_button_symbol)
            nk_draw_button_symbol(&win->buffer, &button, &content, ctx->last_widget_state,
                &ctx->style.combo.button, sym, style->font);
    }
    return nk_combo_begin(ctx, win, size, is_clicked, header);
}
NK_API nk_bool
nk_combo_begin_label(struct nk_context *ctx, const char *selected, struct nk_vec2 size)
{
    return nk_combo_begin_text(ctx, selected, nk_strlen(selected), size);
}
NK_API nk_bool
nk_combo_begin_color(struct nk_context *ctx, struct nk_color color, struct nk_vec2 size)
{
    struct nk_window *win;
    struct nk_style *style;
    const struct nk_input *in;

    struct nk_rect header;
    int is_clicked = nk_false;
    enum nk_widget_layout_states s;
    const struct nk_style_item *background;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    win = ctx->current;
    style = &ctx->style;
    s = nk_widget(&header, ctx);
    if (s == NK_WIDGET_INVALID)
        return 0;

    in = (win->layout->flags & NK_WINDOW_ROM || s == NK_WIDGET_DISABLED || s == NK_WIDGET_ROM)? 0: &ctx->input;
    if (nk_button_behavior(&ctx->last_widget_state, header, in, NK_BUTTON_DEFAULT))
        is_clicked = nk_true;

    /* draw combo box header background and border */
    if (ctx->last_widget_state & NK_WIDGET_STATE_ACTIVED)
        background = &style->combo.active;
    else if (ctx->last_widget_state & NK_WIDGET_STATE_HOVER)
        background = &style->combo.hover;
    else background = &style->combo.normal;

    switch(background->type) {
        case NK_STYLE_ITEM_IMAGE:
            nk_draw_image(&win->buffer, header, &background->data.image, nk_rgb_factor(nk_white, style->combo.color_factor));
            break;
        case NK_STYLE_ITEM_NINE_SLICE:
            nk_draw_nine_slice(&win->buffer, header, &background->data.slice, nk_rgb_factor(nk_white, style->combo.color_factor));
            break;
        case NK_STYLE_ITEM_COLOR:
            nk_fill_rect(&win->buffer, header, style->combo.rounding, nk_rgb_factor(background->data.color, style->combo.color_factor));
            nk_stroke_rect(&win->buffer, header, style->combo.rounding, style->combo.border, nk_rgb_factor(style->combo.border_color, style->combo.color_factor));
            break;
    }
    {
        struct nk_rect content;
        struct nk_rect button;
        struct nk_rect bounds;
        int draw_button_symbol;

        enum nk_symbol_type sym;
        if (ctx->last_widget_state & NK_WIDGET_STATE_HOVER)
            sym = style->combo.sym_hover;
        else if (is_clicked)
            sym = style->combo.sym_active;
        else sym = style->combo.sym_normal;

        /* represents whether or not the combo's button symbol should be drawn */
        draw_button_symbol = sym != NK_SYMBOL_NONE;

        /* calculate button */
        button.w = header.h - 2 * style->combo.button_padding.y;
        button.x = (header.x + header.w - header.h) - style->combo.button_padding.x;
        button.y = header.y + style->combo.button_padding.y;
        button.h = button.w;

        content.x = button.x + style->combo.button.padding.x;
        content.y = button.y + style->combo.button.padding.y;
        content.w = button.w - 2 * style->combo.button.padding.x;
        content.h = button.h - 2 * style->combo.button.padding.y;

        /* draw color */
        bounds.h = header.h - 4 * style->combo.content_padding.y;
        bounds.y = header.y + 2 * style->combo.content_padding.y;
        bounds.x = header.x + 2 * style->combo.content_padding.x;
        if (draw_button_symbol)
            bounds.w = (button.x - (style->combo.content_padding.x + style->combo.spacing.x)) - bounds.x;
        else
            bounds.w = header.w - 4 * style->combo.content_padding.x;
        nk_fill_rect(&win->buffer, bounds, 0, nk_rgb_factor(color, style->combo.color_factor));

        /* draw open/close button */
        if (draw_button_symbol)
            nk_draw_button_symbol(&win->buffer, &button, &content, ctx->last_widget_state,
                &ctx->style.combo.button, sym, style->font);
    }
    return nk_combo_begin(ctx, win, size, is_clicked, header);
}
NK_API nk_bool
nk_combo_begin_symbol(struct nk_context *ctx, enum nk_symbol_type symbol, struct nk_vec2 size)
{
    struct nk_window *win;
    struct nk_style *style;
    const struct nk_input *in;

    struct nk_rect header;
    int is_clicked = nk_false;
    enum nk_widget_layout_states s;
    const struct nk_style_item *background;
    struct nk_color sym_background;
    struct nk_color symbol_color;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    win = ctx->current;
    style = &ctx->style;
    s = nk_widget(&header, ctx);
    if (s == NK_WIDGET_INVALID)
        return 0;

    in = (win->layout->flags & NK_WINDOW_ROM || s == NK_WIDGET_DISABLED || s == NK_WIDGET_ROM)? 0: &ctx->input;
    if (nk_button_behavior(&ctx->last_widget_state, header, in, NK_BUTTON_DEFAULT))
        is_clicked = nk_true;

    /* draw combo box header background and border */
    if (ctx->last_widget_state & NK_WIDGET_STATE_ACTIVED) {
        background = &style->combo.active;
        symbol_color = style->combo.symbol_active;
    } else if (ctx->last_widget_state & NK_WIDGET_STATE_HOVER) {
        background = &style->combo.hover;
        symbol_color = style->combo.symbol_hover;
    } else {
        background = &style->combo.normal;
        symbol_color = style->combo.symbol_hover;
    }

    symbol_color = nk_rgb_factor(symbol_color, style->combo.color_factor);

    switch(background->type) {
        case NK_STYLE_ITEM_IMAGE:
            sym_background = nk_rgba(0, 0, 0, 0);
            nk_draw_image(&win->buffer, header, &background->data.image, nk_rgb_factor(nk_white, style->combo.color_factor));
            break;
        case NK_STYLE_ITEM_NINE_SLICE:
            sym_background = nk_rgba(0, 0, 0, 0);
            nk_draw_nine_slice(&win->buffer, header, &background->data.slice, nk_rgb_factor(nk_white, style->combo.color_factor));
            break;
        case NK_STYLE_ITEM_COLOR:
            sym_background = background->data.color;
            nk_fill_rect(&win->buffer, header, style->combo.rounding, nk_rgb_factor(background->data.color, style->combo.color_factor));
            nk_stroke_rect(&win->buffer, header, style->combo.rounding, style->combo.border, nk_rgb_factor(style->combo.border_color, style->combo.color_factor));
            break;
    }
    {
        struct nk_rect bounds = {0,0,0,0};
        struct nk_rect content;
        struct nk_rect button;

        enum nk_symbol_type sym;
        if (ctx->last_widget_state & NK_WIDGET_STATE_HOVER)
            sym = style->combo.sym_hover;
        else if (is_clicked)
            sym = style->combo.sym_active;
        else sym = style->combo.sym_normal;

        /* calculate button */
        button.w = header.h - 2 * style->combo.button_padding.y;
        button.x = (header.x + header.w - header.h) - style->combo.button_padding.y;
        button.y = header.y + style->combo.button_padding.y;
        button.h = button.w;

        content.x = button.x + style->combo.button.padding.x;
        content.y = button.y + style->combo.button.padding.y;
        content.w = button.w - 2 * style->combo.button.padding.x;
        content.h = button.h - 2 * style->combo.button.padding.y;

        /* draw symbol */
        bounds.h = header.h - 2 * style->combo.content_padding.y;
        bounds.y = header.y + style->combo.content_padding.y;
        bounds.x = header.x + style->combo.content_padding.x;
        bounds.w = (button.x - style->combo.content_padding.y) - bounds.x;
        nk_draw_symbol(&win->buffer, symbol, bounds, sym_background, symbol_color,
            1.0f, style->font);

        /* draw open/close button */
        nk_draw_button_symbol(&win->buffer, &bounds, &content, ctx->last_widget_state,
            &ctx->style.combo.button, sym, style->font);
    }
    return nk_combo_begin(ctx, win, size, is_clicked, header);
}
NK_API nk_bool
nk_combo_begin_symbol_text(struct nk_context *ctx, const char *selected, int len,
    enum nk_symbol_type symbol, struct nk_vec2 size)
{
    struct nk_window *win;
    struct nk_style *style;
    struct nk_input *in;

    struct nk_rect header;
    int is_clicked = nk_false;
    enum nk_widget_layout_states s;
    const struct nk_style_item *background;
    struct nk_color symbol_color;
    struct nk_text text;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    win = ctx->current;
    style = &ctx->style;
    s = nk_widget(&header, ctx);
    if (!s) return 0;

    in = (win->layout->flags & NK_WINDOW_ROM || s == NK_WIDGET_DISABLED || s == NK_WIDGET_ROM)? 0: &ctx->input;
    if (nk_button_behavior(&ctx->last_widget_state, header, in, NK_BUTTON_DEFAULT))
        is_clicked = nk_true;

    /* draw combo box header background and border */
    if (ctx->last_widget_state & NK_WIDGET_STATE_ACTIVED) {
        background = &style->combo.active;
        symbol_color = style->combo.symbol_active;
        text.text = style->combo.label_active;
    } else if (ctx->last_widget_state & NK_WIDGET_STATE_HOVER) {
        background = &style->combo.hover;
        symbol_color = style->combo.symbol_hover;
        text.text = style->combo.label_hover;
    } else {
        background = &style->combo.normal;
        symbol_color = style->combo.symbol_normal;
        text.text = style->combo.label_normal;
    }

    text.text = nk_rgb_factor(text.text, style->combo.color_factor);
    symbol_color = nk_rgb_factor(symbol_color, style->combo.color_factor);

    switch(background->type) {
        case NK_STYLE_ITEM_IMAGE:
            text.background = nk_rgba(0, 0, 0, 0);
            nk_draw_image(&win->buffer, header, &background->data.image, nk_rgb_factor(nk_white, style->combo.color_factor));
            break;
        case NK_STYLE_ITEM_NINE_SLICE:
            text.background = nk_rgba(0, 0, 0, 0);
            nk_draw_nine_slice(&win->buffer, header, &background->data.slice, nk_rgb_factor(nk_white, style->combo.color_factor));
            break;
        case NK_STYLE_ITEM_COLOR:
            text.background = background->data.color;
            nk_fill_rect(&win->buffer, header, style->combo.rounding, nk_rgb_factor(background->data.color, style->combo.color_factor));
            nk_stroke_rect(&win->buffer, header, style->combo.rounding, style->combo.border, nk_rgb_factor(style->combo.border_color, style->combo.color_factor));
            break;
    }
    {
        struct nk_rect content;
        struct nk_rect button;
        struct nk_rect label;
        struct nk_rect image;

        enum nk_symbol_type sym;
        if (ctx->last_widget_state & NK_WIDGET_STATE_HOVER)
            sym = style->combo.sym_hover;
        else if (is_clicked)
            sym = style->combo.sym_active;
        else sym = style->combo.sym_normal;

        /* calculate button */
        button.w = header.h - 2 * style->combo.button_padding.y;
        button.x = (header.x + header.w - header.h) - style->combo.button_padding.x;
        button.y = header.y + style->combo.button_padding.y;
        button.h = button.w;

        content.x = button.x + style->combo.button.padding.x;
        content.y = button.y + style->combo.button.padding.y;
        content.w = button.w - 2 * style->combo.button.padding.x;
        content.h = button.h - 2 * style->combo.button.padding.y;
        nk_draw_button_symbol(&win->buffer, &button, &content, ctx->last_widget_state,
            &ctx->style.combo.button, sym, style->font);

        /* draw symbol */
        image.x = header.x + style->combo.content_padding.x;
        image.y = header.y + style->combo.content_padding.y;
        image.h = header.h - 2 * style->combo.content_padding.y;
        image.w = image.h;
        nk_draw_symbol(&win->buffer, symbol, image, text.background, symbol_color,
            1.0f, style->font);

        /* draw label */
        text.padding = nk_vec2(0,0);
        label.x = image.x + image.w + style->combo.spacing.x + style->combo.content_padding.x;
        label.y = header.y + style->combo.content_padding.y;
        label.w = (button.x - style->combo.content_padding.x) - label.x;
        label.h = header.h - 2 * style->combo.content_padding.y;
        nk_widget_text(&win->buffer, label, selected, len, &text, NK_TEXT_LEFT, style->font);
    }
    return nk_combo_begin(ctx, win, size, is_clicked, header);
}
NK_API nk_bool
nk_combo_begin_image(struct nk_context *ctx, struct nk_image img, struct nk_vec2 size)
{
    struct nk_window *win;
    struct nk_style *style;
    const struct nk_input *in;

    struct nk_rect header;
    int is_clicked = nk_false;
    enum nk_widget_layout_states s;
    const struct nk_style_item *background;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    win = ctx->current;
    style = &ctx->style;
    s = nk_widget(&header, ctx);
    if (s == NK_WIDGET_INVALID)
        return 0;

    in = (win->layout->flags & NK_WINDOW_ROM || s == NK_WIDGET_DISABLED || s == NK_WIDGET_ROM)? 0: &ctx->input;
    if (nk_button_behavior(&ctx->last_widget_state, header, in, NK_BUTTON_DEFAULT))
        is_clicked = nk_true;

    /* draw combo box header background and border */
    if (ctx->last_widget_state & NK_WIDGET_STATE_ACTIVED)
        background = &style->combo.active;
    else if (ctx->last_widget_state & NK_WIDGET_STATE_HOVER)
        background = &style->combo.hover;
    else background = &style->combo.normal;

    switch (background->type) {
        case NK_STYLE_ITEM_IMAGE:
            nk_draw_image(&win->buffer, header, &background->data.image, nk_rgb_factor(nk_white, style->combo.color_factor));
            break;
        case NK_STYLE_ITEM_NINE_SLICE:
            nk_draw_nine_slice(&win->buffer, header, &background->data.slice, nk_rgb_factor(nk_white, style->combo.color_factor));
            break;
        case NK_STYLE_ITEM_COLOR:
            nk_fill_rect(&win->buffer, header, style->combo.rounding, nk_rgb_factor(background->data.color, style->combo.color_factor));
            nk_stroke_rect(&win->buffer, header, style->combo.rounding, style->combo.border, nk_rgb_factor(style->combo.border_color, style->combo.color_factor));
            break;
    }
    {
        struct nk_rect bounds = {0,0,0,0};
        struct nk_rect content;
        struct nk_rect button;
        int draw_button_symbol;

        enum nk_symbol_type sym;
        if (ctx->last_widget_state & NK_WIDGET_STATE_HOVER)
            sym = style->combo.sym_hover;
        else if (is_clicked)
            sym = style->combo.sym_active;
        else sym = style->combo.sym_normal;

        /* represents whether or not the combo's button symbol should be drawn */
        draw_button_symbol = sym != NK_SYMBOL_NONE;

        /* calculate button */
        button.w = header.h - 2 * style->combo.button_padding.y;
        button.x = (header.x + header.w - header.h) - style->combo.button_padding.y;
        button.y = header.y + style->combo.button_padding.y;
        button.h = button.w;

        content.x = button.x + style->combo.button.padding.x;
        content.y = button.y + style->combo.button.padding.y;
        content.w = button.w - 2 * style->combo.button.padding.x;
        content.h = button.h - 2 * style->combo.button.padding.y;

        /* draw image */
        bounds.h = header.h - 2 * style->combo.content_padding.y;
        bounds.y = header.y + style->combo.content_padding.y;
        bounds.x = header.x + style->combo.content_padding.x;
        if (draw_button_symbol)
            bounds.w = (button.x - style->combo.content_padding.y) - bounds.x;
        else
            bounds.w = header.w - 2 * style->combo.content_padding.x;
        nk_draw_image(&win->buffer, bounds, &img, nk_rgb_factor(nk_white, style->combo.color_factor));

        /* draw open/close button */
        if (draw_button_symbol)
            nk_draw_button_symbol(&win->buffer, &bounds, &content, ctx->last_widget_state,
                &ctx->style.combo.button, sym, style->font);
    }
    return nk_combo_begin(ctx, win, size, is_clicked, header);
}
NK_API nk_bool
nk_combo_begin_image_text(struct nk_context *ctx, const char *selected, int len,
    struct nk_image img, struct nk_vec2 size)
{
    struct nk_window *win;
    struct nk_style *style;
    struct nk_input *in;

    struct nk_rect header;
    int is_clicked = nk_false;
    enum nk_widget_layout_states s;
    const struct nk_style_item *background;
    struct nk_text text;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    win = ctx->current;
    style = &ctx->style;
    s = nk_widget(&header, ctx);
    if (!s) return 0;

    in = (win->layout->flags & NK_WINDOW_ROM || s == NK_WIDGET_DISABLED || s == NK_WIDGET_ROM)? 0: &ctx->input;
    if (nk_button_behavior(&ctx->last_widget_state, header, in, NK_BUTTON_DEFAULT))
        is_clicked = nk_true;

    /* draw combo box header background and border */
    if (ctx->last_widget_state & NK_WIDGET_STATE_ACTIVED) {
        background = &style->combo.active;
        text.text = style->combo.label_active;
    } else if (ctx->last_widget_state & NK_WIDGET_STATE_HOVER) {
        background = &style->combo.hover;
        text.text = style->combo.label_hover;
    } else {
        background = &style->combo.normal;
        text.text = style->combo.label_normal;
    }

    text.text = nk_rgb_factor(text.text, style->combo.color_factor);

    switch(background->type) {
        case NK_STYLE_ITEM_IMAGE:
            text.background = nk_rgba(0, 0, 0, 0);
            nk_draw_image(&win->buffer, header, &background->data.image, nk_rgb_factor(nk_white, style->combo.color_factor));
            break;
        case NK_STYLE_ITEM_NINE_SLICE:
            text.background = nk_rgba(0, 0, 0, 0);
            nk_draw_nine_slice(&win->buffer, header, &background->data.slice, nk_rgb_factor(nk_white, style->combo.color_factor));
            break;
        case NK_STYLE_ITEM_COLOR:
            text.background = background->data.color;
            nk_fill_rect(&win->buffer, header, style->combo.rounding, nk_rgb_factor(background->data.color, style->combo.color_factor));
            nk_stroke_rect(&win->buffer, header, style->combo.rounding, style->combo.border, nk_rgb_factor(style->combo.border_color, style->combo.color_factor));
            break;
    }
    {
        struct nk_rect content;
        struct nk_rect button;
        struct nk_rect label;
        struct nk_rect image;
        int draw_button_symbol;

        enum nk_symbol_type sym;
        if (ctx->last_widget_state & NK_WIDGET_STATE_HOVER)
            sym = style->combo.sym_hover;
        else if (is_clicked)
            sym = style->combo.sym_active;
        else sym = style->combo.sym_normal;

        /* represents whether or not the combo's button symbol should be drawn */
        draw_button_symbol = sym != NK_SYMBOL_NONE;

        /* calculate button */
        button.w = header.h - 2 * style->combo.button_padding.y;
        button.x = (header.x + header.w - header.h) - style->combo.button_padding.x;
        button.y = header.y + style->combo.button_padding.y;
        button.h = button.w;

        content.x = button.x + style->combo.button.padding.x;
        content.y = button.y + style->combo.button.padding.y;
        content.w = button.w - 2 * style->combo.button.padding.x;
        content.h = button.h - 2 * style->combo.button.padding.y;
        if (draw_button_symbol)
            nk_draw_button_symbol(&win->buffer, &button, &content, ctx->last_widget_state,
                &ctx->style.combo.button, sym, style->font);

        /* draw image */
        image.x = header.x + style->combo.content_padding.x;
        image.y = header.y + style->combo.content_padding.y;
        image.h = header.h - 2 * style->combo.content_padding.y;
        image.w = image.h;
        nk_draw_image(&win->buffer, image, &img, nk_rgb_factor(nk_white, style->combo.color_factor));

        /* draw label */
        text.padding = nk_vec2(0,0);
        label.x = image.x + image.w + style->combo.spacing.x + style->combo.content_padding.x;
        label.y = header.y + style->combo.content_padding.y;
        label.h = header.h - 2 * style->combo.content_padding.y;
        if (draw_button_symbol)
            label.w = (button.x - style->combo.content_padding.x) - label.x;
        else
            label.w = (header.x + header.w - style->combo.content_padding.x) - label.x;
        nk_widget_text(&win->buffer, label, selected, len, &text, NK_TEXT_LEFT, style->font);
    }
    return nk_combo_begin(ctx, win, size, is_clicked, header);
}
NK_API nk_bool
nk_combo_begin_symbol_label(struct nk_context *ctx,
    const char *selected, enum nk_symbol_type type, struct nk_vec2 size)
{
    return nk_combo_begin_symbol_text(ctx, selected, nk_strlen(selected), type, size);
}
NK_API nk_bool
nk_combo_begin_image_label(struct nk_context *ctx,
    const char *selected, struct nk_image img, struct nk_vec2 size)
{
    return nk_combo_begin_image_text(ctx, selected, nk_strlen(selected), img, size);
}
NK_API nk_bool
nk_combo_item_text(struct nk_context *ctx, const char *text, int len,nk_flags align)
{
    return nk_contextual_item_text(ctx, text, len, align);
}
NK_API nk_bool
nk_combo_item_label(struct nk_context *ctx, const char *label, nk_flags align)
{
    return nk_contextual_item_label(ctx, label, align);
}
NK_API nk_bool
nk_combo_item_image_text(struct nk_context *ctx, struct nk_image img, const char *text,
    int len, nk_flags alignment)
{
    return nk_contextual_item_image_text(ctx, img, text, len, alignment);
}
NK_API nk_bool
nk_combo_item_image_label(struct nk_context *ctx, struct nk_image img,
    const char *text, nk_flags alignment)
{
    return nk_contextual_item_image_label(ctx, img, text, alignment);
}
NK_API nk_bool
nk_combo_item_symbol_text(struct nk_context *ctx, enum nk_symbol_type sym,
    const char *text, int len, nk_flags alignment)
{
    return nk_contextual_item_symbol_text(ctx, sym, text, len, alignment);
}
NK_API nk_bool
nk_combo_item_symbol_label(struct nk_context *ctx, enum nk_symbol_type sym,
    const char *label, nk_flags alignment)
{
    return nk_contextual_item_symbol_label(ctx, sym, label, alignment);
}
NK_API void nk_combo_end(struct nk_context *ctx)
{
    nk_contextual_end(ctx);
}
NK_API void nk_combo_close(struct nk_context *ctx)
{
    nk_contextual_close(ctx);
}
NK_API int
nk_combo(struct nk_context *ctx, const char **items, int count,
    int selected, int item_height, struct nk_vec2 size)
{
    int i = 0;
    int max_height;
    struct nk_vec2 item_spacing;
    struct nk_vec2 window_padding;

    NK_ASSERT(ctx);
    NK_ASSERT(items);
    NK_ASSERT(ctx->current);
    if (!ctx || !items ||!count)
        return selected;

    item_spacing = ctx->style.window.spacing;
    window_padding = nk_panel_get_padding(&ctx->style, ctx->current->layout->type);
    max_height = count * item_height + count * (int)item_spacing.y;
    max_height += (int)item_spacing.y * 2 + (int)window_padding.y * 2;
    size.y = NK_MIN(size.y, (float)max_height);
    if (nk_combo_begin_label(ctx, items[selected], size)) {
        nk_layout_row_dynamic(ctx, (float)item_height, 1);
        for (i = 0; i < count; ++i) {
            if (nk_combo_item_label(ctx, items[i], NK_TEXT_LEFT))
                selected = i;
        }
        nk_combo_end(ctx);
    }
    return selected;
}
NK_API int
nk_combo_separator(struct nk_context *ctx, const char *items_separated_by_separator,
    int separator, int selected, int count, int item_height, struct nk_vec2 size)
{
    int i;
    int max_height;
    struct nk_vec2 item_spacing;
    struct nk_vec2 window_padding;
    const char *current_item;
    const char *iter;
    int length = 0;

    NK_ASSERT(ctx);
    NK_ASSERT(items_separated_by_separator);
    if (!ctx || !items_separated_by_separator)
        return selected;

    /* calculate popup window */
    item_spacing = ctx->style.window.spacing;
    window_padding = nk_panel_get_padding(&ctx->style, ctx->current->layout->type);
    max_height = count * item_height + count * (int)item_spacing.y;
    max_height += (int)item_spacing.y * 2 + (int)window_padding.y * 2;
    size.y = NK_MIN(size.y, (float)max_height);

    /* find selected item */
    current_item = items_separated_by_separator;
    for (i = 0; i < count; ++i) {
        iter = current_item;
        while (*iter && *iter != separator) iter++;
        length = (int)(iter - current_item);
        if (length == 0) break;
        if (i == selected) break;
        current_item = iter + 1;
    }

    if (nk_combo_begin_text(ctx, current_item, length, size)) {
        current_item = items_separated_by_separator;
        nk_layout_row_dynamic(ctx, (float)item_height, 1);
        for (i = 0; i < count; ++i) {
            iter = current_item;
            while (*iter && *iter != separator) iter++;
            length = (int)(iter - current_item);
            if (length == 0) break;
            if (nk_combo_item_text(ctx, current_item, length, NK_TEXT_LEFT))
                selected = i;
            current_item = current_item + length + 1;
        }
        nk_combo_end(ctx);
    }
    return selected;
}
NK_API int
nk_combo_string(struct nk_context *ctx, const char *items_separated_by_zeros,
    int selected, int count, int item_height, struct nk_vec2 size)
{
    return nk_combo_separator(ctx, items_separated_by_zeros, '\0', selected, count, item_height, size);
}
NK_API int
nk_combo_callback(struct nk_context *ctx, void(*item_getter)(void*, int, const char**),
    void *userdata, int selected, int count, int item_height, struct nk_vec2 size)
{
    int i;
    int max_height;
    struct nk_vec2 item_spacing;
    struct nk_vec2 window_padding;
    const char *item;

    NK_ASSERT(ctx);
    NK_ASSERT(item_getter);
    if (!ctx || !item_getter)
        return selected;

    /* calculate popup window */
    item_spacing = ctx->style.window.spacing;
    window_padding = nk_panel_get_padding(&ctx->style, ctx->current->layout->type);
    max_height = count * item_height + count * (int)item_spacing.y;
    max_height += (int)item_spacing.y * 2 + (int)window_padding.y * 2;
    size.y = NK_MIN(size.y, (float)max_height);

    item_getter(userdata, selected, &item);
    if (nk_combo_begin_label(ctx, item, size)) {
        nk_layout_row_dynamic(ctx, (float)item_height, 1);
        for (i = 0; i < count; ++i) {
            item_getter(userdata, i, &item);
            if (nk_combo_item_label(ctx, item, NK_TEXT_LEFT))
                selected = i;
        }
        nk_combo_end(ctx);
    } return selected;
}
NK_API void
nk_combobox(struct nk_context *ctx, const char **items, int count,
    int *selected, int item_height, struct nk_vec2 size)
{
    *selected = nk_combo(ctx, items, count, *selected, item_height, size);
}
NK_API void
nk_combobox_string(struct nk_context *ctx, const char *items_separated_by_zeros,
    int *selected, int count, int item_height, struct nk_vec2 size)
{
    *selected = nk_combo_string(ctx, items_separated_by_zeros, *selected, count, item_height, size);
}
NK_API void
nk_combobox_separator(struct nk_context *ctx, const char *items_separated_by_separator,
    int separator, int *selected, int count, int item_height, struct nk_vec2 size)
{
    *selected = nk_combo_separator(ctx, items_separated_by_separator, separator,
                                    *selected, count, item_height, size);
}
NK_API void
nk_combobox_callback(struct nk_context *ctx,
    void(*item_getter)(void* data, int id, const char **out_text),
    void *userdata, int *selected, int count, int item_height, struct nk_vec2 size)
{
    *selected = nk_combo_callback(ctx, item_getter, userdata,  *selected, count, item_height, size);
}





/* ===============================================================
 *
 *                              TOOLTIP
 *
 * ===============================================================*/
NK_API nk_bool
nk_tooltip_begin(struct nk_context *ctx, float width)
{
    int x,y,w,h;
    struct nk_window *win;
    const struct nk_input *in;
    struct nk_rect bounds;
    int ret;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    /* make sure that no nonblocking popup is currently active */
    win = ctx->current;
    in = &ctx->input;
    if (win->popup.win && ((int)win->popup.type & (int)NK_PANEL_SET_NONBLOCK))
        return 0;

    w = nk_iceilf(width);
    h = nk_iceilf(nk_null_rect.h);
    x = nk_ifloorf(in->mouse.pos.x + 1) - (int)win->layout->clip.x;
    y = nk_ifloorf(in->mouse.pos.y + 1) - (int)win->layout->clip.y;

    bounds.x = (float)x;
    bounds.y = (float)y;
    bounds.w = (float)w;
    bounds.h = (float)h;
    if ((win->layout->clip.x + bounds.x + bounds.w) > (win->bounds.x + win->bounds.w))
        bounds.x = win->bounds.x + win->bounds.w - bounds.w - win->layout->clip.x;

    ret = nk_popup_begin(ctx, NK_POPUP_DYNAMIC,
        "__##Tooltip##__", NK_WINDOW_NO_SCROLLBAR|NK_WINDOW_BORDER, bounds);
    if (ret) win->layout->flags &= ~(nk_flags)NK_WINDOW_ROM;
    win->popup.type = NK_PANEL_TOOLTIP;
    ctx->current->layout->type = NK_PANEL_TOOLTIP;
    return ret;
}

NK_API void
nk_tooltip_end(struct nk_context *ctx)
{
    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    if (!ctx || !ctx->current) return;
    ctx->current->seq--;
    nk_popup_close(ctx);
    nk_popup_end(ctx);
}
NK_API void
nk_tooltip(struct nk_context *ctx, const char *text)
{
    const struct nk_style *style;
    struct nk_vec2 padding;

    int text_len;
    float text_width;
    float text_height;

    NK_ASSERT(ctx);
    NK_ASSERT(ctx->current);
    NK_ASSERT(ctx->current->layout);
    NK_ASSERT(text);
    if (!ctx || !ctx->current || !ctx->current->layout || !text)
        return;

    /* fetch configuration data */
    style = &ctx->style;
    padding = style->window.padding;

    /* calculate size of the text and tooltip */
    text_len = nk_strlen(text);
    text_width = style->font->width(style->font->userdata,
                    style->font->height, text, text_len);
    text_width += (4 * padding.x);
    text_height = (style->font->height + 2 * padding.y);

    /* execute tooltip and fill with text */
    if (nk_tooltip_begin(ctx, (float)text_width)) {
        nk_layout_row_dynamic(ctx, (float)text_height, 1);
        nk_text(ctx, text, text_len, NK_TEXT_LEFT);
        nk_tooltip_end(ctx);
    }
}
#ifdef NK_INCLUDE_STANDARD_VARARGS
NK_API void
nk_tooltipf(struct nk_context *ctx, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    nk_tooltipfv(ctx, fmt, args);
    va_end(args);
}
NK_API void
nk_tooltipfv(struct nk_context *ctx, const char *fmt, va_list args)
{
    char buf[256];
    nk_strfmt(buf, NK_LEN(buf), fmt, args);
    nk_tooltip(ctx, buf);
}
#endif



#endif /* NK_IMPLEMENTATION */

/*
/// ## License
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~none
///    ------------------------------------------------------------------------------
///    This software is available under 2 licenses -- choose whichever you prefer.
///    ------------------------------------------------------------------------------
///    ALTERNATIVE A - MIT License
///    Copyright (c) 2016-2018 Micha Mettke
///    Permission is hereby granted, free of charge, to any person obtaining a copy of
///    this software and associated documentation files (the "Software"), to deal in
///    the Software without restriction, including without limitation the rights to
///    use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
///    of the Software, and to permit persons to whom the Software is furnished to do
///    so, subject to the following conditions:
///    The above copyright notice and this permission notice shall be included in all
///    copies or substantial portions of the Software.
///    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
///    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
///    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
///    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
///    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
///    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
///    SOFTWARE.
///    ------------------------------------------------------------------------------
///    ALTERNATIVE B - Public Domain (www.unlicense.org)
///    This is free and unencumbered software released into the public domain.
///    Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
///    software, either in source code form or as a compiled binary, for any purpose,
///    commercial or non-commercial, and by any means.
///    In jurisdictions that recognize copyright laws, the author or authors of this
///    software dedicate any and all copyright interest in the software to the public
///    domain. We make this dedication for the benefit of the public at large and to
///    the detriment of our heirs and successors. We intend this dedication to be an
///    overt act of relinquishment in perpetuity of all present and future rights to
///    this software under copyright law.
///    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
///    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
///    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
///    AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
///    ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
///    WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
///    ------------------------------------------------------------------------------
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// ## Changelog
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~none
/// [date] ([x.y.z]) - [description]
/// - [date]: date on which the change has been pushed
/// - [x.y.z]: Version string, represented in Semantic Versioning format
///   - [x]: Major version with API and library breaking changes
///   - [y]: Minor version with non-breaking API and library changes
///   - [z]: Patch version with no direct changes to the API
///
/// - 2024/03/07 (4.12.1) - Fix bitwise operations warnings in C++20
/// - 2023/11/26 (4.12.0) - Added an alignment option to checkboxes and radio buttons.
/// - 2023/10/11 (4.11.0) - Added nk_widget_disable_begin() and nk_widget_disable_end()
/// - 2022/12/23 (4.10.6) - Fix incorrect glyph index in nk_font_bake()
/// - 2022/12/17 (4.10.5) - Fix nk_font_bake_pack() using TTC font offset incorrectly
/// - 2022/10/24 (4.10.4) - Fix nk_str_{append,insert}_str_utf8 always returning 0
/// - 2022/09/03 (4.10.3) - Renamed the `null` texture variable to `tex_null`
/// - 2022/08/01 (4.10.2) - Fix Apple Silicon with incorrect NK_SITE_TYPE and NK_POINTER_TYPE
/// - 2022/08/01 (4.10.1) - Fix cursor jumping back to beginning of text when typing more than
///                         nk_edit_xxx limit
/// - 2022/05/27 (4.10.0) - Add nk_input_has_mouse_click_in_button_rect() to fix window move bug
/// - 2022/04/19 (4.9.8)  - Added nk_rule_horizontal() widget
/// - 2022/04/18 (4.9.7)  - Change button behavior when NK_BUTTON_TRIGGER_ON_RELEASE is defined to
///                         only trigger when the mouse position was inside the same button on down
/// - 2022/02/03 (4.9.6)  - Allow overriding the NK_INV_SQRT function, similar to NK_SIN and NK_COS
/// - 2021/12/22 (4.9.5)  - Revert layout bounds not accounting for padding due to regressions
/// - 2021/12/22 (4.9.4)  - Fix checking hovering when window is minimized
/// - 2021/12/22 (4.09.3) - Fix layout bounds not accounting for padding
/// - 2021/12/19 (4.09.2) - Update to stb_rect_pack.h v1.01 and stb_truetype.h v1.26
/// - 2021/12/16 (4.09.1) - Fix the majority of GCC warnings
/// - 2021/10/16 (4.09.0) - Added nk_spacer() widget
/// - 2021/09/22 (4.08.6) - Fix "may be used uninitialized" warnings in nk_widget
/// - 2021/09/22 (4.08.5) - GCC __builtin_offsetof only exists in version 4 and later
/// - 2021/09/15 (4.08.4) - Fix "'num_len' may be used uninitialized" in nk_do_property
/// - 2021/09/15 (4.08.3) - Fix "Templates cannot be declared to have 'C' Linkage"
/// - 2021/09/08 (4.08.2) - Fix warnings in C89 builds
/// - 2021/09/08 (4.08.1) - Use compiler builtins for NK_OFFSETOF when possible
/// - 2021/08/17 (4.08.0) - Implemented 9-slice scaling support for widget styles
/// - 2021/08/16 (4.07.5) - Replace usage of memset in nk_font_atlas_bake with NK_MEMSET
/// - 2021/08/15 (4.07.4) - Fix conversion and sign conversion warnings
/// - 2021/08/08 (4.07.3) - Fix crash when baking merged fonts
/// - 2021/08/08 (4.07.2) - Fix Multiline Edit wrong offset
/// - 2021/03/17 (4.07.1) - Fix warning about unused parameter
/// - 2021/03/17 (4.07.0) - Fix nk_property hover bug
/// - 2021/03/15 (4.06.4) - Change nk_propertyi back to int
/// - 2021/03/15 (4.06.3) - Update documentation for functions that now return nk_bool
/// - 2020/12/19 (4.06.2) - Fix additional C++ style comments which are not allowed in ISO C90.
/// - 2020/10/11 (4.06.1) - Fix C++ style comments which are not allowed in ISO C90.
/// - 2020/10/07 (4.06.0) - Fix nk_combo return type wrongly changed to nk_bool
/// - 2020/09/05 (4.05.0) - Use the nk_font_atlas allocator for stb_truetype memory management.
/// - 2020/09/04 (4.04.1) - Replace every boolean int by nk_bool
/// - 2020/09/04 (4.04.0) - Add nk_bool with NK_INCLUDE_STANDARD_BOOL
/// - 2020/06/13 (4.03.1) - Fix nk_pool allocation sizes.
/// - 2020/06/04 (4.03.0) - Made nk_combo header symbols optional.
/// - 2020/05/27 (4.02.5) - Fix nk_do_edit: Keep scroll position when re-activating edit widget.
/// - 2020/05/09 (4.02.4) - Fix nk_menubar height calculation bug
/// - 2020/05/08 (4.02.3) - Fix missing stdarg.h with NK_INCLUDE_STANDARD_VARARGS
/// - 2020/04/30 (4.02.2) - Fix nk_edit border drawing bug
/// - 2020/04/09 (4.02.1) - Removed unused nk_sqrt function to fix compiler warnings
///                       - Fixed compiler warnings if you bring your own methods for
///                        nk_cos/nk_sin/nk_strtod/nk_memset/nk_memcopy/nk_dtoa
/// - 2020/04/06 (4.01.10) - Fix bug: Do not use pool before checking for NULL
/// - 2020/03/22 (4.01.9) - Fix bug where layout state wasn't restored correctly after
///                        popping a tree.
/// - 2020/03/11 (4.01.8) - Fix bug where padding is subtracted from widget
/// - 2020/03/06 (4.01.7) - Fix bug where width padding was applied twice
/// - 2020/02/06 (4.01.6) - Update stb_truetype.h and stb_rect_pack.h and separate them
/// - 2019/12/10 (4.01.5) - Fix off-by-one error in NK_INTERSECT
/// - 2019/10/09 (4.01.4) - Fix bug for autoscrolling in nk_do_edit
/// - 2019/09/20 (4.01.3) - Fixed a bug wherein combobox cannot be closed by clicking the header
///                        when NK_BUTTON_TRIGGER_ON_RELEASE is defined.
/// - 2019/09/10 (4.01.2) - Fixed the nk_cos function, which deviated significantly.
/// - 2019/09/08 (4.01.1) - Fixed a bug wherein re-baking of fonts caused a segmentation
///                        fault due to dst_font->glyph_count not being zeroed on subsequent
///                        bakes of the same set of fonts.
/// - 2019/06/23 (4.01.0) - Added nk_***_get_scroll and nk_***_set_scroll for groups, windows, and popups.
/// - 2019/06/12 (4.00.3) - Fix panel background drawing bug.
/// - 2018/10/31 (4.00.2) - Added NK_KEYSTATE_BASED_INPUT to "fix" state based backends
///                        like GLFW without breaking key repeat behavior on event based.
/// - 2018/04/01 (4.00.1) - Fixed calling `nk_convert` multiple time per single frame.
/// - 2018/04/01 (4.00.0) - BREAKING CHANGE: nk_draw_list_clear no longer tries to
///                        clear provided buffers. So make sure to either free
///                        or clear each passed buffer after calling nk_convert.
/// - 2018/02/23 (3.00.6) - Fixed slider dragging behavior.
/// - 2018/01/31 (3.00.5) - Fixed overcalculation of cursor data in font baking process.
/// - 2018/01/31 (3.00.4) - Removed name collision with stb_truetype.
/// - 2018/01/28 (3.00.3) - Fixed panel window border drawing bug.
/// - 2018/01/12 (3.00.2) - Added `nk_group_begin_titled` for separated group identifier and title.
/// - 2018/01/07 (3.00.1) - Started to change documentation style.
/// - 2018/01/05 (3.00.0) - BREAKING CHANGE: The previous color picker API was broken
///                        because of conversions between float and byte color representation.
///                        Color pickers now use floating point values to represent
///                        HSV values. To get back the old behavior I added some additional
///                        color conversion functions to cast between nk_color and
///                        nk_colorf.
/// - 2017/12/23 (2.00.7) - Fixed small warning.
/// - 2017/12/23 (2.00.7) - Fixed `nk_edit_buffer` behavior if activated to allow input.
/// - 2017/12/23 (2.00.7) - Fixed modifyable progressbar dragging visuals and input behavior.
/// - 2017/12/04 (2.00.6) - Added formatted string tooltip widget.
/// - 2017/11/18 (2.00.5) - Fixed window becoming hidden with flag `NK_WINDOW_NO_INPUT`.
/// - 2017/11/15 (2.00.4) - Fixed font merging.
/// - 2017/11/07 (2.00.3) - Fixed window size and position modifier functions.
/// - 2017/09/14 (2.00.2) - Fixed `nk_edit_buffer` and `nk_edit_focus` behavior.
/// - 2017/09/14 (2.00.1) - Fixed window closing behavior.
/// - 2017/09/14 (2.00.0) - BREAKING CHANGE: Modifying window position and size functions now
///                        require the name of the window and must happen outside the window
///                        building process (between function call nk_begin and nk_end).
/// - 2017/09/11 (1.40.9) - Fixed window background flag if background window is declared last.
/// - 2017/08/27 (1.40.8) - Fixed `nk_item_is_any_active` for hidden windows.
/// - 2017/08/27 (1.40.7) - Fixed window background flag.
/// - 2017/07/07 (1.40.6) - Fixed missing clipping rect check for hovering/clicked
///                        query for widgets.
/// - 2017/07/07 (1.40.5) - Fixed drawing bug for vertex output for lines and stroked
///                        and filled rectangles.
/// - 2017/07/07 (1.40.4) - Fixed bug in nk_convert trying to add windows that are in
///                        process of being destroyed.
/// - 2017/07/07 (1.40.3) - Fixed table internal bug caused by storing table size in
///                        window instead of directly in table.
/// - 2017/06/30 (1.40.2) - Removed unneeded semicolon in C++ NK_ALIGNOF macro.
/// - 2017/06/30 (1.40.1) - Fixed drawing lines smaller or equal zero.
/// - 2017/06/08 (1.40.0) - Removed the breaking part of last commit. Auto layout now only
///                        comes in effect if you pass in zero was row height argument.
/// - 2017/06/08 (1.40.0) - BREAKING CHANGE: while not directly API breaking it will change
///                        how layouting works. From now there will be an internal minimum
///                        row height derived from font height. If you need a row smaller than
///                        that you can directly set it by `nk_layout_set_min_row_height` and
///                        reset the value back by calling `nk_layout_reset_min_row_height.
/// - 2017/06/08 (1.39.1) - Fixed property text edit handling bug caused by past `nk_widget` fix.
/// - 2017/06/08 (1.39.0) - Added function to retrieve window space without calling a `nk_layout_xxx` function.
/// - 2017/06/06 (1.38.5) - Fixed `nk_convert` return flag for command buffer.
/// - 2017/05/23 (1.38.4) - Fixed activation behavior for widgets partially clipped.
/// - 2017/05/10 (1.38.3) - Fixed wrong min window size mouse scaling over boundaries.
/// - 2017/05/09 (1.38.2) - Fixed vertical scrollbar drawing with not enough space.
/// - 2017/05/09 (1.38.1) - Fixed scaler dragging behavior if window size hits minimum size.
/// - 2017/05/06 (1.38.0) - Added platform double-click support.
/// - 2017/04/20 (1.37.1) - Fixed key repeat found inside glfw demo backends.
/// - 2017/04/20 (1.37.0) - Extended properties with selection and clipboard support.
/// - 2017/04/20 (1.36.2) - Fixed #405 overlapping rows with zero padding and spacing.
/// - 2017/04/09 (1.36.1) - Fixed #403 with another widget float error.
/// - 2017/04/09 (1.36.0) - Added window `NK_WINDOW_NO_INPUT` and `NK_WINDOW_NOT_INTERACTIVE` flags.
/// - 2017/04/09 (1.35.3) - Fixed buffer heap corruption.
/// - 2017/03/25 (1.35.2) - Fixed popup overlapping for `NK_WINDOW_BACKGROUND` windows.
/// - 2017/03/25 (1.35.1) - Fixed windows closing behavior.
/// - 2017/03/18 (1.35.0) - Added horizontal scroll requested in #377.
/// - 2017/03/18 (1.34.3) - Fixed long window header titles.
/// - 2017/03/04 (1.34.2) - Fixed text edit filtering.
/// - 2017/03/04 (1.34.1) - Fixed group closable flag.
/// - 2017/02/25 (1.34.0) - Added custom draw command for better language binding support.
/// - 2017/01/24 (1.33.0) - Added programmatic way to remove edit focus.
/// - 2017/01/24 (1.32.3) - Fixed wrong define for basic type definitions for windows.
/// - 2017/01/21 (1.32.2) - Fixed input capture from hidden or closed windows.
/// - 2017/01/21 (1.32.1) - Fixed slider behavior and drawing.
/// - 2017/01/13 (1.32.0) - Added flag to put scaler into the bottom left corner.
/// - 2017/01/13 (1.31.0) - Added additional row layouting method to combine both
///                        dynamic and static widgets.
/// - 2016/12/31 (1.30.0) - Extended scrollbar offset from 16-bit to 32-bit.
/// - 2016/12/31 (1.29.2) - Fixed closing window bug of minimized windows.
/// - 2016/12/03 (1.29.1) - Fixed wrapped text with no separator and C89 error.
/// - 2016/12/03 (1.29.0) - Changed text wrapping to process words not characters.
/// - 2016/11/22 (1.28.6) - Fixed window minimized closing bug.
/// - 2016/11/19 (1.28.5) - Fixed abstract combo box closing behavior.
/// - 2016/11/19 (1.28.4) - Fixed tooltip flickering.
/// - 2016/11/19 (1.28.3) - Fixed memory leak caused by popup repeated closing.
/// - 2016/11/18 (1.28.2) - Fixed memory leak caused by popup panel allocation.
/// - 2016/11/10 (1.28.1) - Fixed some warnings and C++ error.
/// - 2016/11/10 (1.28.0) - Added additional `nk_button` versions which allows to directly
///                        pass in a style struct to change buttons visual.
/// - 2016/11/10 (1.27.0) - Added additional `nk_tree` versions to support external state
///                        storage. Just like last the `nk_group` commit the main
///                        advantage is that you optionally can minimize nuklears runtime
///                        memory consumption or handle hash collisions.
/// - 2016/11/09 (1.26.0) - Added additional `nk_group` version to support external scrollbar
///                        offset storage. Main advantage is that you can externalize
///                        the memory management for the offset. It could also be helpful
///                        if you have a hash collision in `nk_group_begin` but really
///                        want the name. In addition I added `nk_list_view` which allows
///                        to draw big lists inside a group without actually having to
///                        commit the whole list to nuklear (issue #269).
/// - 2016/10/30 (1.25.1) - Fixed clipping rectangle bug inside `nk_draw_list`.
/// - 2016/10/29 (1.25.0) - Pulled `nk_panel` memory management into nuklear and out of
///                        the hands of the user. From now on users don't have to care
///                        about panels unless they care about some information. If you
///                        still need the panel just call `nk_window_get_panel`.
/// - 2016/10/21 (1.24.0) - Changed widget border drawing to stroked rectangle from filled
///                        rectangle for less overdraw and widget background transparency.
/// - 2016/10/18 (1.23.0) - Added `nk_edit_focus` for manually edit widget focus control.
/// - 2016/09/29 (1.22.7) - Fixed deduction of basic type in non `<stdint.h>` compilation.
/// - 2016/09/29 (1.22.6) - Fixed edit widget UTF-8 text cursor drawing bug.
/// - 2016/09/28 (1.22.5) - Fixed edit widget UTF-8 text appending/inserting/removing.
/// - 2016/09/28 (1.22.4) - Fixed drawing bug inside edit widgets which offset all text
///                        text in every edit widget if one of them is scrolled.
/// - 2016/09/28 (1.22.3) - Fixed small bug in edit widgets if not active. The wrong
///                        text length is passed. It should have been in bytes but
///                        was passed as glyphs.
/// - 2016/09/20 (1.22.2) - Fixed color button size calculation.
/// - 2016/09/20 (1.22.1) - Fixed some `nk_vsnprintf` behavior bugs and removed `<stdio.h>`
///                        again from `NK_INCLUDE_STANDARD_VARARGS`.
/// - 2016/09/18 (1.22.0) - C89 does not support vsnprintf only C99 and newer as well
///                        as C++11 and newer. In addition to use vsnprintf you have
///                        to include <stdio.h>. So just defining `NK_INCLUDE_STD_VAR_ARGS`
///                        is not enough. That behavior is now fixed. By default if
///                        both varargs as well as stdio is selected I try to use
///                        vsnprintf if not possible I will revert to vsprintf. If
///                        varargs but not stdio was defined I will use my own function.
/// - 2016/09/15 (1.21.2) - Fixed panel `close` behavior for deeper panel levels.
/// - 2016/09/15 (1.21.1) - Fixed C++ errors and wrong argument to `nk_panel_get_xxxx`.
/// - 2016/09/13 (1.21.0) - !BREAKING! Fixed nonblocking popup behavior in menu, combo,
///                        and contextual which prevented closing in y-direction if
///                        popup did not reach max height.
///                        In addition the height parameter was changed into vec2
///                        for width and height to have more control over the popup size.
/// - 2016/09/13 (1.20.3) - Cleaned up and extended type selection.
/// - 2016/09/13 (1.20.2) - Fixed slider behavior hopefully for the last time. This time
///                        all calculation are correct so no more hackery.
/// - 2016/09/13 (1.20.1) - Internal change to divide window/panel flags into panel flags and types.
///                        Suprisinly spend years in C and still happened to confuse types
///                        with flags. Probably something to take note.
/// - 2016/09/08 (1.20.0) - Added additional helper function to make it easier to just
///                        take the produced buffers from `nk_convert` and unplug the
///                        iteration process from `nk_context`. So now you can
///                        just use the vertex,element and command buffer + two pointer
///                        inside the command buffer retrieved by calls `nk__draw_begin`
///                        and `nk__draw_end` and macro `nk_draw_foreach_bounded`.
/// - 2016/09/08 (1.19.0) - Added additional asserts to make sure every `nk_xxx_begin` call
///                        for windows, popups, combobox, menu and contextual is guarded by
///                        `if` condition and does not produce false drawing output.
/// - 2016/09/08 (1.18.0) - Changed confusing name for `NK_SYMBOL_RECT_FILLED`, `NK_SYMBOL_RECT`
///                        to hopefully easier to understand `NK_SYMBOL_RECT_FILLED` and
///                        `NK_SYMBOL_RECT_OUTLINE`.
/// - 2016/09/08 (1.17.0) - Changed confusing name for `NK_SYMBOL_CIRLCE_FILLED`, `NK_SYMBOL_CIRCLE`
///                        to hopefully easier to understand `NK_SYMBOL_CIRCLE_FILLED` and
///                        `NK_SYMBOL_CIRCLE_OUTLINE`.
/// - 2016/09/08 (1.16.0) - Added additional checks to select correct types if `NK_INCLUDE_FIXED_TYPES`
///                        is not defined by supporting the biggest compiler GCC, clang and MSVC.
/// - 2016/09/07 (1.15.3) - Fixed `NK_INCLUDE_COMMAND_USERDATA` define to not cause an error.
/// - 2016/09/04 (1.15.2) - Fixed wrong combobox height calculation.
/// - 2016/09/03 (1.15.1) - Fixed gaps inside combo boxes in OpenGL.
/// - 2016/09/02 (1.15.0) - Changed nuklear to not have any default vertex layout and
///                        instead made it user provided. The range of types to convert
///                        to is quite limited at the moment, but I would be more than
///                        happy to accept PRs to add additional.
/// - 2016/08/30 (1.14.2) - Removed unused variables.
/// - 2016/08/30 (1.14.1) - Fixed C++ build errors.
/// - 2016/08/30 (1.14.0) - Removed mouse dragging from SDL demo since it does not work correctly.
/// - 2016/08/30 (1.13.4) - Tweaked some default styling variables.
/// - 2016/08/30 (1.13.3) - Hopefully fixed drawing bug in slider, in general I would
///                        refrain from using slider with a big number of steps.
/// - 2016/08/30 (1.13.2) - Fixed close and minimize button which would fire even if the
///                        window was in Read Only Mode.
/// - 2016/08/30 (1.13.1) - Fixed popup panel padding handling which was previously just
///                        a hack for combo box and menu.
/// - 2016/08/30 (1.13.0) - Removed `NK_WINDOW_DYNAMIC` flag from public API since
///                        it is bugged and causes issues in window selection.
/// - 2016/08/30 (1.12.0) - Removed scaler size. The size of the scaler is now
///                        determined by the scrollbar size.
/// - 2016/08/30 (1.11.2) - Fixed some drawing bugs caused by changes from 1.11.0.
/// - 2016/08/30 (1.11.1) - Fixed overlapping minimized window selection.
/// - 2016/08/30 (1.11.0) - Removed some internal complexity and overly complex code
///                        handling panel padding and panel border.
/// - 2016/08/29 (1.10.0) - Added additional height parameter to `nk_combobox_xxx`.
/// - 2016/08/29 (1.10.0) - Fixed drawing bug in dynamic popups.
/// - 2016/08/29 (1.10.0) - Added experimental mouse scrolling to popups, menus and comboboxes.
/// - 2016/08/26 (1.10.0) - Added window name string prepresentation to account for
///                        hash collisions. Currently limited to `NK_WINDOW_MAX_NAME`
///                        which in term can be redefined if not big enough.
/// - 2016/08/26 (1.10.0) - Added stacks for temporary style/UI changes in code.
/// - 2016/08/25 (1.10.0) - Changed `nk_input_is_key_pressed` and 'nk_input_is_key_released'
///                        to account for key press and release happening in one frame.
/// - 2016/08/25 (1.10.0) - Added additional nk_edit flag to directly jump to the end on activate.
/// - 2016/08/17 (1.09.6) - Removed invalid check for value zero in `nk_propertyx`.
/// - 2016/08/16 (1.09.5) - Fixed ROM mode for deeper levels of popup windows parents.
/// - 2016/08/15 (1.09.4) - Editbox are now still active if enter was pressed with flag
///                        `NK_EDIT_SIG_ENTER`. Main reasoning is to be able to keep
///                        typing after committing.
/// - 2016/08/15 (1.09.4) - Removed redundant code.
/// - 2016/08/15 (1.09.4) - Fixed negative numbers in `nk_strtoi` and remove unused variable.
/// - 2016/08/15 (1.09.3) - Fixed `NK_WINDOW_BACKGROUND` flag behavior to select a background
///                        window only as selected by hovering and not by clicking.
/// - 2016/08/14 (1.09.2) - Fixed a bug in font atlas which caused wrong loading
///                        of glyphs for font with multiple ranges.
/// - 2016/08/12 (1.09.1) - Added additional function to check if window is currently
///                        hidden and therefore not visible.
/// - 2016/08/12 (1.09.1) - nk_window_is_closed now queries the correct flag `NK_WINDOW_CLOSED`
///                        instead of the old flag `NK_WINDOW_HIDDEN`.
/// - 2016/08/09 (1.09.0) - Added additional double version to nk_property and changed
///                        the underlying implementation to not cast to float and instead
///                        work directly on the given values.
/// - 2016/08/09 (1.08.0) - Added additional define to overwrite library internal
///                        floating pointer number to string conversion for additional
///                        precision.
/// - 2016/08/09 (1.08.0) - Added additional define to overwrite library internal
///                        string to floating point number conversion for additional
///                        precision.
/// - 2016/08/08 (1.07.2) - Fixed compiling error without define `NK_INCLUDE_FIXED_TYPE`.
/// - 2016/08/08 (1.07.1) - Fixed possible floating point error inside `nk_widget` leading
///                        to wrong widget width calculation which results in widgets falsely
///                        becoming tagged as not inside window and cannot be accessed.
/// - 2016/08/08 (1.07.0) - Nuklear now differentiates between hiding a window (NK_WINDOW_HIDDEN) and
///                        closing a window (NK_WINDOW_CLOSED). A window can be hidden/shown
///                        by using `nk_window_show` and closed by either clicking the close
///                        icon in a window or by calling `nk_window_close`. Only closed
///                        windows get removed at the end of the frame while hidden windows
///                        remain.
/// - 2016/08/08 (1.06.0) - Added `nk_edit_string_zero_terminated` as a second option to
///                        `nk_edit_string` which takes, edits and outputs a '\0' terminated string.
/// - 2016/08/08 (1.05.4) - Fixed scrollbar auto hiding behavior.
/// - 2016/08/08 (1.05.3) - Fixed wrong panel padding selection in `nk_layout_widget_space`.
/// - 2016/08/07 (1.05.2) - Fixed old bug in dynamic immediate mode layout API, calculating
///                        wrong item spacing and panel width.
/// - 2016/08/07 (1.05.1) - Hopefully finally fixed combobox popup drawing bug.
/// - 2016/08/07 (1.05.0) - Split varargs away from `NK_INCLUDE_STANDARD_IO` into own
///                        define `NK_INCLUDE_STANDARD_VARARGS` to allow more fine
///                        grained controlled over library includes.
/// - 2016/08/06 (1.04.5) - Changed memset calls to `NK_MEMSET`.
/// - 2016/08/04 (1.04.4) - Fixed fast window scaling behavior.
/// - 2016/08/04 (1.04.3) - Fixed window scaling, movement bug which appears if you
///                        move/scale a window and another window is behind it.
///                        If you are fast enough then the window behind gets activated
///                        and the operation is blocked. I now require activating
///                        by hovering only if mouse is not pressed.
/// - 2016/08/04 (1.04.2) - Fixed changing fonts.
/// - 2016/08/03 (1.04.1) - Fixed `NK_WINDOW_BACKGROUND` behavior.
/// - 2016/08/03 (1.04.0) - Added color parameter to `nk_draw_image`.
/// - 2016/08/03 (1.04.0) - Added additional window padding style attributes for
///                        sub windows (combo, menu, ...).
/// - 2016/08/03 (1.04.0) - Added functions to show/hide software cursor.
/// - 2016/08/03 (1.04.0) - Added `NK_WINDOW_BACKGROUND` flag to force a window
///                        to be always in the background of the screen.
/// - 2016/08/03 (1.03.2) - Removed invalid assert macro for NK_RGB color picker.
/// - 2016/08/01 (1.03.1) - Added helper macros into header include guard.
/// - 2016/07/29 (1.03.0) - Moved the window/table pool into the header part to
///                        simplify memory management by removing the need to
///                        allocate the pool.
/// - 2016/07/29 (1.02.0) - Added auto scrollbar hiding window flag which if enabled
///                        will hide the window scrollbar after NK_SCROLLBAR_HIDING_TIMEOUT
///                        seconds without window interaction. To make it work
///                        you have to also set a delta time inside the `nk_context`.
/// - 2016/07/25 (1.01.1) - Fixed small panel and panel border drawing bugs.
/// - 2016/07/15 (1.01.0) - Added software cursor to `nk_style` and `nk_context`.
/// - 2016/07/15 (1.01.0) - Added const correctness to `nk_buffer_push' data argument.
/// - 2016/07/15 (1.01.0) - Removed internal font baking API and simplified
///                        font atlas memory management by converting pointer
///                        arrays for fonts and font configurations to lists.
/// - 2016/07/15 (1.00.0) - Changed button API to use context dependent button
///                        behavior instead of passing it for every function call.
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/// ## Gallery
/// ![Figure [blue]: Feature overview with blue color styling](https://cloud.githubusercontent.com/assets/8057201/13538240/acd96876-e249-11e5-9547-5ac0b19667a0.png)
/// ![Figure [red]: Feature overview with red color styling](https://cloud.githubusercontent.com/assets/8057201/13538243/b04acd4c-e249-11e5-8fd2-ad7744a5b446.png)
/// ![Figure [widgets]: Widget overview](https://cloud.githubusercontent.com/assets/8057201/11282359/3325e3c6-8eff-11e5-86cb-cf02b0596087.png)
/// ![Figure [blackwhite]: Black and white](https://cloud.githubusercontent.com/assets/8057201/11033668/59ab5d04-86e5-11e5-8091-c56f16411565.png)
/// ![Figure [filexp]: File explorer](https://cloud.githubusercontent.com/assets/8057201/10718115/02a9ba08-7b6b-11e5-950f-adacdd637739.png)
/// ![Figure [opengl]: OpenGL Editor](https://cloud.githubusercontent.com/assets/8057201/12779619/2a20d72c-ca69-11e5-95fe-4edecf820d5c.png)
/// ![Figure [nodedit]: Node Editor](https://cloud.githubusercontent.com/assets/8057201/9976995/e81ac04a-5ef7-11e5-872b-acd54fbeee03.gif)
/// ![Figure [skinning]: Using skinning in Nuklear](https://cloud.githubusercontent.com/assets/8057201/15991632/76494854-30b8-11e6-9555-a69840d0d50b.png)
/// ![Figure [bf]: Heavy modified version](https://cloud.githubusercontent.com/assets/8057201/14902576/339926a8-0d9c-11e6-9fee-a8b73af04473.png)
///
/// ## Credits
/// Developed by Micha Mettke and every direct or indirect github contributor. <br /><br />
///
/// Embeds [stb_texedit](https://github.com/nothings/stb/blob/master/stb_textedit.h), [stb_truetype](https://github.com/nothings/stb/blob/master/stb_truetype.h) and [stb_rectpack](https://github.com/nothings/stb/blob/master/stb_rect_pack.h) by Sean Barret (public domain) <br />
/// Uses [stddoc.c](https://github.com/r-lyeh/stddoc.c) from r-lyeh@github.com for documentation generation <br /><br />
/// Embeds ProggyClean.ttf font by Tristan Grimmer (MIT license). <br />
///
/// Big thank you to Omar Cornut (ocornut@github) for his [imgui library](https://github.com/ocornut/imgui) and
/// giving me the inspiration for this library, Casey Muratori for handmade hero
/// and his original immediate mode graphical user interface idea and Sean
/// Barret for his amazing single header libraries which restored my faith
/// in libraries and brought me to create some of my own. Finally Apoorva Joshi
/// for his single header file packer.
*/

