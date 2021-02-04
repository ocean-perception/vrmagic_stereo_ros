/*----------------------------------------------------------------------*/
/**
* @name GUI_LIB.H
*       GUI library header
*
* <br>  VM_LIB is subject to the license and usage terms of VRmagic GmbH,
* <br>  which are distributed with this software.
* <br>
* <br>  This file contains type definitions, macros and function prototypes
*       of GUI functions.
*
* @version
*       Created:  06.05.2008
* <br>  Modified:
*/
/*----------------------------------------------------------------------*/
/*@{*/
#ifndef _GUI_LIB_H_LOADED        /* [[[ */
#define _GUI_LIB_H_LOADED

#include "cdef.h"       /* common definitions and memory allocation     */
#include "dr_lib.h"     /* drawing library                              */
#include "io_mouse.h"   /* mouse events                                 */
//#include "bl_lib.h"     /* BLOB header                                  */

/*----------------------------------------------------------------------*/
/*                                Version                               */
/*----------------------------------------------------------------------*/
#define GUI_LIB_VERS       100   /* Version 1.00                         */
#define GUI_LIB_VERS_STR  "1.00" /* Version string                       */

/*----------------------------------------------------------------------*/
/*                              Error codes                             */
/*----------------------------------------------------------------------*/
/* defined in lib_err.h */

/*----------------------------------------------------------------------*/
/*                           Macros and typedefs                        */
/*----------------------------------------------------------------------*/

/*............. Box definition */
#ifndef VM_BOX_DEFINED    /* [[[ */
  #define VM_BOX_DEFINED
  typedef struct
  {
      int   x;        /* x-coord. of top/left box corner                  */
      int   y;        /* y-coord. of top/left box corner                  */
      int   dx;       /* box width in pixels                              */
      int   dy;       /* box height in pixels                             */
  } VM_BOX;
#endif  /* ]]] */

/*............. Text/picture drawing: align modes */
#ifndef DR_ALIGN_DEFINED    /* [[[ */
  #define DR_ALIGN_DEFINED
  typedef enum
  {
      DR_ALIGN_LEFT = 0,    /* left horizontal alignment                */
      DR_ALIGN_CENTER,      /* center horizontal/vertical alignment     */
      DR_ALIGN_RIGHT,       /* right horizontal alignment               */
      DR_ALIGN_UP,          /* up vertical alignment                    */
      DR_ALIGN_DOWN         /* down vertical alignment                  */
  } DR_ALIGN_MODES;
#endif  /* ]]] */

/*............. GUI frame typess */
#ifndef GUI_FRAME_DEFINED    /* [[[ */
  #define GUI_FRAME_DEFINED
  typedef enum
  {
      GUI_FRAME_DOWN_UP = 0,     /* down & up frame: -\/-    */
      GUI_FRAME_UP_DOWN,         /* up & down frame: -/\-    */
      GUI_FRAME_DOWN,            /* down frame: --\__        */
      GUI_FRAME_UP               /* up frame:   __/--        */
  } GUI_FRAME_TYPES;
#endif  /* ]]] */

/*............. GUI label types */
#ifndef GUI_LABEL_DEFINED    /* [[[ */
  #define GUI_LABEL_DEFINED
  typedef enum
  {
      GUI_LABEL_TXT = 1,    /* text picture                         */
      GUI_LABEL_MONO,       /* monochrome picture (one color)       */
      GUI_LABEL_CLR,        /* multi-color picture                  */
      GUI_LABEL_CLR_TI      /* multi-color picture for TI camera    */
  } GUI_LABEL_TYPES;
#endif  /* ]]] */

///*............. General rotated rectangle */
//typedef struct
//{
//    float   x;      /* x-coord. of rectangle point                      */
//    float   y;      /* y-coord. of rectangle point                      */
//    float   dx;     /* rectangle width in pixels                        */
//    float   dy;     /* rectangle height in pixels                       */
//    float   ang;    /* rotation angle in radians - increases counter-   */
//                    /* clockwise, 0 is at 3 o'clock                     */
//    int     pt_pos; /* rectangle point position:                        */
//                    /*   0 : rectangle center                           */
//                    /*   1 : top left corner                            */
//} VM_RECT;

/*----------------------------------------------------------------------*/
/*                          GUI tool handle                             */
/*----------------------------------------------------------------------*/
/*
* GUI tool execution identifiers:
* ------------------------------
* The purpose of the execution identifiers is to call one GUI tool with
* the same handle in different places (subroutines) with differnent input
* arguments. For proper work the tool execution function should keep the
* internal tool state for each tool instance. This is done by calling the
* tool with different 'exec_id' arguments, which point to respective
* internal states.
*
* When a GUI tool is opened, you should specify the max number of tool
* instances (exec_id's) you are going to use for the created tool handle.
*
* Example:
*   hnd = vm_tool_open(2);
* You can call the tool with the handle 'hnd' and 2 different execution
* identifiers 0 and 1:
*   exec_id = 0;
*   rc = vm_tool(hnd, exec_id,...)
* ................................
*   exec_id = 1;
*   rc = vm_tool(hnd, exec_id,...)
*
* The two tools will behave as if the tools were opened with different
* handles. In this way you can place a GUI tool in a subroutine, which
* displays GUI controls in different program places. The only requirement
* is to use different exec_id's in different subroutine calls.
*
* The max number of tool states (and exec_id's) is defined by MAX_STATE_CNT,
*/

typedef void * GUI_HND;         /* GUI tool handle  */
#define MAX_STATE_CNT   1000    /* max allowed number of tool states    */

/*----------------------------------------------------------------------*/
/*               GUI tool colors (defined in DR_LIB.H):                 */
/*----------------------------------------------------------------------*/
/*
*   DR_COLOR_NONE           =  0 : transparent color (invisible)
*   DR_COLOR_BLUE           =  1 : blue
*   DR_COLOR_GREEN          =  2 : green
*   DR_COLOR_CYAN           =  3 : cyan
*   DR_COLOR_RED            =  4 : red
*   DR_COLOR_MAGENTA        =  5 : magenta
*   DR_COLOR_YELLOW         =  6 : yellow
*   DR_COLOR_GRAY           =  7 : gray
*   DR_COLOR_BRIGHT_GRAY    =  8 : bright gray
*   DR_COLOR_BRIGHT_BLUE    =  9 : bright blue
*   DR_COLOR_BRIGHT_GREEN   = 10 : bright green
*   DR_COLOR_BRIGHT_CYAN    = 11 : bright cyan
*   DR_COLOR_BRIGHT_RED     = 12 : bright red
*   DR_COLOR_BRIGHT_MAGENTA = 13 : bright magenta
*   DR_COLOR_BRIGHT_YELLOW  = 14 : bright yellow
*   DR_COLOR_WHITE          = 15 : white (high intensity white)
*
*   DR_COLOR_BLACK          = 16 : black (default fgnd text color)
*   DR_COLOR_DARK_GRAY      = 17 : dark-gray   (3-D corner shadow)
*   DR_COLOR_DEF_BGND       = 18 : middle-gray (windows-like bgnd)
*   DR_COLOR_LIGHT_GRAY     = 19 : light-gray  (3-D corner light)
*   DR_COLOR_TIT_BGND       = 20 : unsel. window title bgnd clr
*   DR_COLOR_TIT_SBGND      = 21 : sel. window title bgnd clr
*/

/*----------------------------------------------------------------------*/
/*                                  Window                              */
/*----------------------------------------------------------------------*/

/*............. Window flags */
#define VM_WIN_CLOSE_BUT  0x0001  /* enable window close button         */
#define VM_WIN_MOVE       0x0002  /* enable window move operation       */
#define VM_WIN_RESIZE     0x0004  /* enable window resize operation     */

/*............. Window colors */
typedef struct
{
    int   tit_bgnd;         /* unselected title background color        */
    int   tit_fgnd;         /* unselected title foreground color        */
    int   tit_sbgnd;        /* sel. title background color (on focus)   */
    int   tit_sfgnd;        /* sel. title foreground color (on focus)   */
    int   win_bgnd;         /* window backround color (client area clr) */
                            /* (also bgnd color of close button)        */
// default colors used:
//    int   lclr;             /* button/window light corner color         */
//    int   sclr;             /* button/window shadow corner color        */
} VM_WIN_COLORS;


/*............. Window events */
typedef enum
{
    VM_WIN_EVENT_NONE = 0,  /* 0 = no event                             */
    VM_WIN_EVENT_EXIT,      /* 1 = window exit (close button click)     */
    VM_WIN_EVENT_MOVED,     /* 2 = window is moved                      */
    VM_WIN_EVENT_RESIZED,   /* 3 = window is resized                    */
    VM_WIN_EVENT_ON_FOCUS,  /* 4 = window put on focus                  */
    VM_WIN_EVENT_OFF_FOCUS  /* 5 = window removed from focus            */
} VM_WIN_EVENTS;

/*----------------------------------------------------------------------*/
/*                                  Button                              */
/*----------------------------------------------------------------------*/

/*............. Button colors */
typedef struct
{
    int   bgnd;             /* background button color                  */
    int   fgnd;             /* foreground text color                    */
    int   lclr;             /* color of light corner                    */
    int   sclr;             /* color of shadow corner                   */
} VM_BUT_COLORS;

/*............. Button events */
typedef enum
{
    VM_BUT_EVENT_NONE = 0,  /* 0 = no event (button in released state)  */
    VM_BUT_EVENT_PRESS,     /* 1 = button press event                   */
    VM_BUT_EVENT_PRESSED,   /* 2 = button in pressed state              */
    VM_BUT_EVENT_RELEASE    /* 3 = button release event (button click)  */
} VM_BUT_EVENTS;

/*----------------------------------------------------------------------*/
/*                                  Edit                                */
/*----------------------------------------------------------------------*/

/*............. Edit types */
typedef enum
{
  VM_EDIT_TYPE_EDIT = 0,    /* normal key editing                       */
  VM_EDIT_TYPE_PASSWORD     /* enter password (all chars = '*')         */
//  VM_EDIT_TYPE_SHOW,        /* show test in edit box, no edit           */
//  VM_EDIT_TYPE_TEXT         /* show text only, don't display edit box   */
} VM_EDIT_TYPES;

/*............. Edit events */
typedef enum
{
    VM_EDIT_EVENT_NONE = 0,     /* no event                             */
    VM_EDIT_EVENT_ON_FOCUS,     /* put on focus                         */
    VM_EDIT_EVENT_OFF_FOCUS,    /* removed from focus                   */
    VM_EDIT_EVENT_CHANGED       /* edit string changed                  */
} VM_EDIT_EVENTS;

/*............. Edit colors */
typedef struct
{
    int   edit_bgnd;    /* unselected edit-box background color         */
    int   edit_fgnd;    /* unselected edit-box foreground color         */
    int   edit_sbgnd;   /* selected edit-box background color (on focus)*/
    int   edit_sfgnd;   /* selected edit-box foreground color (on focus)*/
    int   lclr;         /* color of light edit-box corner               */
    int   sclr;         /* color of shadow edit-box corner              */
    int   curs_clr;     /* text cursor color                            */
    int   curs_clr1;    /* text cursor color 1 (for cursor blinking)    */
} VM_EDIT_COLORS;

//#define VM_EDIT_MAX_STRLEN  256         /* max length of edit string    */

/*----------------------------------------------------------------------*/
/*                                  Spin                                */
/*----------------------------------------------------------------------*/

/*............. Spin types */
typedef enum
{
    VM_SPIN_TYPE_INT = 0,       /* integer spin                         */
    VM_SPIN_TYPE_INT_EDIT,      /* integer spin with enabled editing    */
    VM_SPIN_TYPE_FLOAT,         /* float spin                           */
    VM_SPIN_TYPE_FLOAT_EDIT,    /* float spin with enabled editing      */
    VM_SPIN_TYPE_TOGGLE         /* toggle                               */
} VM_SPIN_TYPES;

/*............. Spin colors */
typedef struct
{
    int   edit_bgnd;    /* unselected edit-box background color         */
    int   edit_fgnd;    /* unselected edit-box foreground color         */
    int   edit_sbgnd;   /* selected edit-box background color (on focus)*/
    int   edit_sfgnd;   /* selected edit-box foreground color (on focus)*/
    int   but_bgnd;     /* up/down button backround color               */
    int   but_fgnd;     /* up/down button foreground color              */
    int   lclr;         /* color of light button/edit-box corner        */
    int   sclr;         /* color of shadow button/edit-box corner       */
    int   curs_clr;     /* text cursor color                            */
    int   curs_clr1;    /* text cursor color 1 (for cursor blinking)    */
} VM_SPIN_COLORS;

/*............. Spin events */
typedef enum
{
    VM_SPIN_EVENT_NONE = 0,     /* 0 = no event                         */
    VM_SPIN_EVENT_ON_FOCUS,     /* 1 = put on focus                     */
    VM_SPIN_EVENT_OFF_FOCUS,    /* 2 = removed from focus               */
    VM_SPIN_EVENT_CHANGED       /* 3 = spin value/toggle state changed  */
} VM_SPIN_EVENTS;

/*----------------------------------------------------------------------*/
/*                            Radio-button menu                         */
/*----------------------------------------------------------------------*/

#define VM_RADB_MAXCNT       64 /* max number of radio-buton menu items */
#define VM_RADB_MAX_TXTLEN  128 /* max length of text lines in char #   */

/*............. Radio-button types */
typedef enum
{
    VM_RADB_TYPE_VER  = 0,  /* vertical radio-button menu    */
    VM_RADB_TYPE_HOR        /* horizontal radio-button menu  */
} VM_RADB_TYPES;

/*............. Radio-button colors */
typedef struct
{
    int   txt_bgnd;     /* text background color                        */
    int   txt_fgnd;     /* text foreground color                        */
    int   bgnd;         /* radio-button backround color                 */
    int   fgnd;         /* radio-button foreground color                */
    int   lclr;         /* color of light radio-button corner           */
    int   sclr;         /* color of shadow radio-button corner           */
} VM_RADB_COLORS;

/*............. Radio-button events */
typedef enum
{
    VM_RADB_EVENT_NONE = 0,     /* 0 = no event                         */
    VM_RADB_EVENT_ON_FOCUS,     /* 1 = put on focus                     */
    VM_RADB_EVENT_OFF_FOCUS,    /* 2 = removed from focus               */
    VM_RADB_EVENT_CHANGED       /* 3 = radio-button value changed       */
} VM_RADB_EVENTS;

/*----------------------------------------------------------------------*/
/*                               Check-box                              */
/*----------------------------------------------------------------------*/

#define VM_CBOX_MAX_TXTLEN  128 /* max length of text line in char #   */

/*............. Check-box colors */
typedef struct
{
    int   txt_bgnd;     /* text background color                        */
    int   txt_fgnd;     /* text foreground color                        */
    int   bgnd;         /* check-box backround color                    */
    int   fgnd;         /* check-box foreground color                   */
    int   lclr;         /* color of light check-box corner              */
    int   sclr;         /* color of shadow check-box corner             */
} VM_CBOX_COLORS;

/*............. Check-box events */
typedef enum
{
    VM_CBOX_EVENT_NONE = 0,     /* 0 = no event                         */
    VM_CBOX_EVENT_ON_FOCUS,     /* 1 = put on focus                     */
    VM_CBOX_EVENT_OFF_FOCUS,    /* 2 = removed from focus               */
    VM_CBOX_EVENT_CHANGED       /* 3 = check-box value changed          */
} VM_CBOX_EVENTS;

/*----------------------------------------------------------------------*/
/*                             GUI rectangle                            */
/*----------------------------------------------------------------------*/

/*............. Positions of GUI rectangle definition point */
typedef enum
{
    GUI_RECT_PT_CENTER = 0,     /* 0 = at rectangle center              */
    GUI_RECT_PT_TOPLEFT,        /* 1 = at top left corner               */
    GUI_RECT_PT_BOTLEFT         /* 2 = at bottom left corner            */
} GUI_RECT_PT_POS;

/*............. GUI rectangle types */
typedef enum
{
    GUI_RECT_BOX = 0,           /* 0 = box (borders only)               */
    GUI_RECT_BOX_FILL,          /* 1 = box filled with color            */
    GUI_RECT_BOX_VARROW,        /* 2 = box (up vertical arrow)          */
    GUI_RECT_BOX_HARROW,        /* 3 = box (right horizontal arrow)     */
    GUI_RECT_MARKER,            /* 4 = marker                           */
    GUI_RECT_ELLIPSE            /* 5 = ellipse (border only)            */
//    GUI_RECT_ELLIPSE_FILL       /* 6 = ellipse filled with color        */
} GUI_RECT_TYPES;

/*............. GUI rectangle button flags */
/*
* Flags for move/resize/rotate buttons:
* Off  : The button is not displayed, button operation is disabled.
* On   : The button is displayed, button operation is enabled.
* Auto : The button is shown when the mouse cursor enters the
*        rectangle box or the button area. Button operation is enabled.
*/
typedef enum
{
    GUI_RECT_BUT_OFF = 0,       /* 0 = button off                       */
    GUI_RECT_BUT_ON,            /* 1 = button on                        */
    GUI_RECT_BUT_AUTO           /* 2 = auto on                          */
} GUI_RECT_BUT_FLAGS;

/*............. GUI rectangle events */
typedef enum
{
    GUI_RECT_EVENT_NONE = 0,    /* 0 = no event                         */
    GUI_RECT_EVENT_LCLICK,      /* 1 = left click inside rect           */
    GUI_RECT_EVENT_DBLCLICK,    /* 2 = left double click inside rect    */
    GUI_RECT_EVENT_RCLICK,      /* 3 = right click inside rect          */
    GUI_RECT_EVENT_ENTER,       /* 4 = mouse cursor enters rect         */
    GUI_RECT_EVENT_INSIDE,      /* 5 = mouse cursor inside rect         */
    GUI_RECT_EVENT_EXIT,        /* 6 = mouse cursor exits rect          */
    GUI_RECT_EVENT_MOVED,       /* 7 = rectangle moved                  */
    GUI_RECT_EVENT_RESIZED,     /* 8 = rectangle resized                */
    GUI_RECT_EVENT_ROTATED      /* 9 = rectangle rotated                */
} GUI_RECT_EVENTS;

/*----------------------------------------------------------------------*/
/*                       Passive GUI drawing tools                      */
/*----------------------------------------------------------------------*/
/*
* To do:
*   ListBox = display scrolling text in a box like terminals do.
*/
/*............. Text colors */
typedef struct
{
    int   txt_bgnd;     /* text background color           */
    int   txt_fgnd;     /* text foreground color           */
    int   lclr;         /* color of light text box corner   */
    int   sclr;         /* color of shadow text box corner */
} VM_TEXT_COLORS;

/*----------------------------------------------------------------------*/
/*                           Function prototypes                        */
/*----------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

GUI_HND vm_win_open (                                   /* vm_win.c     */
    int   max_exec_ids );   /* [in] max number of exec_id's for handle  */
void vm_win_close (
    GUI_HND  *hnd );        /* [i/o] tool handle                        */
int vm_win (
    GUI_HND   hnd,          /* [i/o] tool handle                        */
    int       exec_id,      /* [in] execution identifier (default=0)    */
    DR_IMAGE *dr_img,       /* [i/o] drawing image                      */
    char     *title,        /* [in] window title string                 */
    VM_BOX   *win_box,      /* [i/o] window box (x,y) (dx,dy)           */
    VM_BOX   *client_box,   /* [out] client box (x,y) (dx,dy)           */
    int       depth,        /* [in] window & close button depth [1,3]   */
    int       win_flags,    /* [in] window flags (close/move/resize)    */
    VM_WIN_COLORS *clrs,    /* [in] window colors                       */
    MS_EVENT *ms_event,     /* [in] mouse event                         */
    int      *gui_event );  /* [out] window event                       */
int vm_win_set_limits (
    GUI_HND   hnd,          /* [i/o] rectangle handle                   */
    int       exec_id,      /* [in] execution identifier (default=0)    */
    int       min_dx,       /* [in] min window width  (<=0: disable)    */
    int       min_dy,       /* [in] min window height (<=0: disable)    */
    int       max_dx,       /* [in] max window width  (<=0: disable)    */
    int       max_dy,       /* [in] max window height (<=0: disable)    */
    VM_BOX   *bound_box );  /* [in] window bounding box                 */
int vm_win_client_box (
    VM_BOX *win_box,        /* [in] window box                          */
    int     win_depth,      /* [in] window depth                        */
    VM_BOX *client_box );   /* [out] client box                         */
int vm_win_set_focus (
    GUI_HND   hnd,          /* [i/o] tool handle                        */
    int       exec_id,      /* [in] execution identifier (default=0)    */
    int       focus );      /* [in] window focus flag: 0=off, 1=on      */
int vm_win_get_focus (
    GUI_HND   hnd,          /* [i/o] tool handle                        */
    int       exec_id,      /* [in] execution identifier (default=0)    */
    int      *focus );      /* [out] window focus flag: 0=off, 1=on     */


GUI_HND vm_but_open (                                   /* vm_but.c     */
    int   max_exec_ids );   /* [in] max number of exec_id's for handle  */
void vm_but_close (
    GUI_HND  *hnd );        /* [i/o] tool handle                        */
int vm_but (
    GUI_HND   hnd,          /* [i/o] tool handle                        */
    int       exec_id,      /* [in] execution identifier (default=0)    */
    DR_IMAGE *dr_img,       /* [i/o] drawing image                      */
    void     *but_label,    /* [in] button label buffer (NULL = none)   */
    int       but_type,     /* [in] button label type: txt/mono/clr/TI  */
    int       hor_align,    /* [in] horizontal text/picture alignment   */
    int       ver_align,    /* [in] vertical text/picture alignment     */
    VM_BOX   *but_box,      /* [in] button box (x,y,dx,dy)              */
    int       but_depth,    /* [in] button depth [1,3]                  */
    VM_BUT_COLORS *but_clrs,/* [in] button colors (NULL=default)        */
    MS_EVENT *ms_event,     /* [in] mouse event                         */
    int      *gui_event );  /* [out] button event                       */

GUI_HND vm_edit_open (                                  /* vm_edit.c    */
    int   max_exec_ids );   /* [in] max number of exec_id's for handle  */
void vm_edit_close (
    GUI_HND  *hnd );        /* [i/o] tool handle                        */
int vm_edit (
    GUI_HND   hnd,          /* [i/o] tool handle                        */
    int       exec_id,      /* [in] execution identifier (default=0)    */
    DR_IMAGE *dr_img,       /* [i/o] drawing image                      */
    int       edit_type,    /* [in] edit type                           */
    char     *edit_str,     /* [i/o] edit string                        */
    int       edit_len,     /* [in] max length of edit string (char #)  */
    VM_BOX   *edit_box,     /* [in] edit box (x,y,dx,dy)                */
    int       edit_depth,   /* [in] edit-box depth [0-3]                */
//    int       hor_align,    /* [in] show/text horiz. alignment:         */
//    int       ver_align,    /* [in] show/text vertical alignment:       */
    VM_EDIT_COLORS *edit_clrs,  /* [in] edit colors                     */
    MS_EVENT *ms_event,     /* [i/o] mouse event                        */
    int      *key,          /* [i/o] key code                           */
    int      *gui_event );  /* [out] edit event                         */

// replaced by vm_draw_text_box:
//int vm_show_text (
//    GUI_HND   hnd,          /* [i/o] tool handle                        */
//    int       exec_id,      /* [in] execution identifier (default=0)    */
//    DR_IMAGE *dr_img,       /* [i/o] drawing image                      */
//    char     *txt,          /* [i/o] edit string                        */
//    VM_BOX   *txt_box,      /* [in] edit box (x,y,dx,dy)                */
//    int       depth,        /* [in] text-box depth [-1,0-3]                */
//    int       hor_align,    /* [in] show/text horiz. alignment:         */
//    int       ver_align,    /* [in] show/text vertical alignment:       */
//    VM_EDIT_COLORS *txt_clrs ); /* [in] text colors                     */

GUI_HND vm_spin_open (                                  /* vm_spin.c    */
    int   max_exec_ids );   /* [in] max number of exec_id's for handle  */
void vm_spin_close (
    GUI_HND  *hnd );        /* [i/o] tool handle                        */
int vm_spin (
    GUI_HND   hnd,          /* [i/o] tool handle                        */
    int       exec_id,      /* [in] execution identifier (default=0)    */
    DR_IMAGE *dr_img,       /* [i/o] drawing image                      */
    int       spin_type,    /* [in] spin type                           */
    void     *spin_val,     /* [i/o] current spin value                 */
    void     *spin_step,    /* [in] spin inc/dec step                   */
    void     *spin_min,     /* [in] min spin value                      */
    void     *spin_max,     /* [in] max spin value                      */
    int       max_edlen,    /* [in] max edit length in edit mode        */
    int       but_dx,       /* [in] up/down button width (0=default)    */
    char    **tog_def,      /* [in] toggle definition buffer            */
    int       tog_cnt,      /* [in] # of toggle states                  */
    int      *tog_state,    /* [i/o] current toggle state               */
    VM_BOX   *spin_box,     /* [in] spin box (x,y,dx,dy)                */
    int       spin_depth,   /* [in] edit-box & up/down button depth     */
    VM_SPIN_COLORS *spin_clrs,  /* [in] spin colors                     */
    MS_EVENT *ms_event,     /* [i/o] mouse event                        */
    int      *key,          /* [i/o] key code                           */
    int      *gui_event );  /* [out] spin event                         */
int vm_int_spin (
    GUI_HND   hnd,          /* [i/o] tool handle                        */
    int       exec_id,      /* [in] execution identifier (default=0)    */
    DR_IMAGE *dr_img,       /* [i/o] drawing image                      */
    int       spin_type,    /* [in] spin type: 0=no edit, 1=edit        */
    int      *spin_val,     /* [i/o] spin value                         */
    int       spin_step,    /* [in] spin inc/dec step                   */
    int       spin_min,     /* [in] min spin value                      */
    int       spin_max,     /* [in] max spin value                      */
    int       max_edlen,    /* [in] max edit length in edit mode        */
    int       but_dx,       /* [in] up/down button width (0=default)    */
    VM_BOX   *spin_box,     /* [in] spin box (x,y,dx,dy)                */
    int       spin_depth,   /* [in] edit-box & up/down button depth     */
    VM_SPIN_COLORS *spin_clrs,  /* [in] spin colors                     */
    MS_EVENT *ms_event,     /* [i/o] mouse event                        */
    int      *key,          /* [i/o] key code                           */
    int      *gui_event );  /* [out] spin event                         */
int vm_float_spin (
    GUI_HND   hnd,          /* [i/o] tool handle                        */
    int       exec_id,      /* [in] execution identifier (default=0)    */
    DR_IMAGE *dr_img,       /* [i/o] drawing image                      */
    int       spin_type,    /* [in] spin type: 0=no edit, 1=edit        */
    double   *spin_val,     /* [i/o] spin value                         */
    double    spin_step,    /* [in] spin inc/dec step                   */
    double    spin_min,     /* [in] min spin value                      */
    double    spin_max,     /* [in] max spin value                      */
    int       max_edlen,    /* [in] max edit length in edit mode        */
    int       but_dx,       /* [in] up/down button width (0=default)    */
    VM_BOX   *spin_box,     /* [in] spin box (x,y,dx,dy)                */
    int       spin_depth,   /* [in] edit-box & up/down button depth     */
    VM_SPIN_COLORS *spin_clrs,  /* [in] spin colors                     */
    MS_EVENT *ms_event,     /* [i/o] mouse event                        */
    int      *key,          /* [i/o] key code                           */
    int      *gui_event );  /* [out] spin event                         */
int vm_toggle (
    GUI_HND   hnd,          /* [i/o] tool handle                        */
    int       exec_id,      /* [in] execution identifier (default=0)    */
    DR_IMAGE *dr_img,       /* [i/o] drawing image                      */
    char    **tog_def,      /* [in] toggle definition buffer            */
    int       tog_cnt,      /* [in] # of toggle states                  */
    int      *tog_state,    /* [i/o] current toggle state               */
    int       but_dx,       /* [in] up/down button width (0=default)    */
    VM_BOX   *spin_box,     /* [in] spin box (x,y,dx,dy)                */
    int       spin_depth,   /* [in] edit-box & up/down button depth     */
    VM_SPIN_COLORS *spin_clrs,  /* [in] spin colors                     */
    MS_EVENT *ms_event,     /* [i/o] mouse event                        */
    int      *key,          /* [i/o] key code                           */
    int      *gui_event );  /* [out] spin event                         */

GUI_HND vm_radb_open (                                  /* vm_radb.c    */
    int   max_exec_ids );   /* [in] max number of exec_id's for handle  */
void vm_radb_close (
    GUI_HND  *hnd );        /* [i/o] tool handle                        */
int vm_radb (
    GUI_HND   hnd,          /* [i/o] tool handle                        */
    int       exec_id,      /* [in] execution identifier (default=0)    */
    DR_IMAGE *dr_img,       /* [i/o] drawing image                      */
    int      *radb_val,     /* [i/o] current radio-button value         */
    int       radb_type,    /* [in] radio-button menu type              */
    int       radb_x,       /* [in] x-coord. of top/left menu corner    */
    int       radb_y,       /* [in] y-coord. of top/left menu corner    */
    char     *radb_texts[], /* [in] radio-button menu texts             */
    int       radb_cnt,     /* [in] # of radio-button menu items (>= 2) */
    int       radb_dist,    /* [i/o] distance in pixels b/n menu items  */
    VM_RADB_COLORS *radb_clrs,  /* [in] radio-button colors             */
    MS_EVENT *ms_event,     /* [i/o] mouse event                        */
    int      *gui_event );  /* [out] radio-button event                 */

GUI_HND vm_cbox_open (                                  /* vm_cbox.c    */
    int   max_exec_ids );   /* [in] max number of exec_id's for handle  */
void vm_cbox_close (
    GUI_HND  *hnd );        /* [i/o] tool handle                        */
int vm_cbox (
    GUI_HND   hnd,          /* [i/o] tool handle                        */
    int       exec_id,      /* [in] execution identifier (default=0)    */
    DR_IMAGE *dr_img,       /* [i/o] drawing image                      */
    int      *cbox_val,     /* [i/o] current check-box value            */
    int       cbox_x,       /* [in] x-coord. of top/left cbox corner    */
    int       cbox_y,       /* [in] y-coord. of top/left cbox corner    */
    char     *cbox_text,    /* [in] check-box text                      */
    VM_CBOX_COLORS *cbox_clrs,  /* [in] check-box colors                */
    MS_EVENT *ms_event,     /* [i/o] mouse event                        */
    int      *gui_event );  /* [out] check-box event                    */

GUI_HND vm_gui_rect_open (                              /* vm_guir.c    */
    int   max_exec_ids );   /* [in] max number of exec_id's for handle  */
void  vm_gui_rect_close (
    GUI_HND  *hnd );        /* [i/o] GUI rectangle handle               */
int vm_gui_rect (
    GUI_HND   hnd,          /* [i/o] rectangle handle                   */
    int       exec_id,      /* [in] execution identifier (default=0)    */
    DR_IMAGE *dr_img,       /* [i/o] drawing image                      */
    int      *x,            /* [i/o] x-coord. of rect. point            */
    int      *y,            /* [i/o] y-coord. of rect. point            */
    int      *dx,           /* [i/o] rectangle width                    */
    int      *dy,           /* [i/o] rectangle height                   */
    float    *ang,          /* [i/o] rotation angle (radians)           */
    int       pt_pos,       /* [in] point position                      */
    int       type,         /* [in] rectangle type                      */
    int       move_but,     /* [in] "move" button flag                  */
    int       resize_but,   /* [in] "resize" button flag                */
    int       rotate_but,   /* [in] "rotate" button flag                */
    int       clr,          /* [in] rectangle color                     */
    MS_EVENT *ms_event,     /* [in] mouse event                         */
    int      *gui_event );  /* [out] GUI rectangle event                */
int vm_gui_rect_set_limits (
    GUI_HND   hnd,          /* [i/o] rectangle handle                   */
    int       exec_id,      /* [in] execution identifier (default=0)    */
    int       min_dx,       /* [in] min GUI rectangle width  (<=0: off) */
    int       min_dy,       /* [in] min GUI rectangle height (<=0: off) */
    int       max_dx,       /* [in] max GUI rectangle width  (<=0: off) */
    int       max_dy,       /* [in] max GUI rectangle height (<=0: off) */
    VM_BOX   *bound_box );  /* [in] GUI rectangle bounding box          */

int vm_draw_text_box (                                  /* vm_draw.c    */
    DR_IMAGE *dr_img,       /* [i/o] drawing image                      */
    char     *txt,          /* [in] string to draw                      */
    VM_BOX   *txt_box,      /* [in] text box (x,y,dx,dy)                */
    int       depth,        /* [in] text-box depth [-1,0-3]             */
    int       hor_align,    /* [in] horizontal text alignment           */
    int       ver_align,    /* [in] vertical text alignment             */
    VM_TEXT_COLORS *txt_clrs ); /* [in] text colors                     */
int vm_draw_gui_frame (
    DR_IMAGE *dr_img,       /* [i/o] drawing image                      */
    char     *txt,          /* [in] frame header string                 */
    VM_BOX   *frame_box,    /* [in] frame enclosing box (x,y,dx,dy)     */
    int       depth,        /* [in] frame depth [0-3]                   */
    int       width,        /* [in] frame width [0-3]                   */
    int       align,        /* [in] header text alignment (-1=default)  */
    int       type,         /* [in] frame type (-1=default)             */
    VM_TEXT_COLORS *frame_clrs ); /* [in] frame colors                  */

#ifdef __cplusplus
}
#endif

#endif /* ]]] - _GUI_LIB_H_LOADED */
/*@}*/
