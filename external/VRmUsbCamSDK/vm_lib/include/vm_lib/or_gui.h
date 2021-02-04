/*----------------------------------------------------------------------*/
/**
* @name OR_GUI.H
*       OR GUI module header
*
* <br>  VM_LIB is subject to the license and usage terms of VRmagic GmbH,
* <br>  which are distributed with this software.
* <br>
* <br>  This file contains GUI type definitions, macros and function
*       prototypes used by the object recognition GUI module.
*
* @version
*       Created: 14.05.2008
* <br>  Modified:
*/
/*----------------------------------------------------------------------*/
/*@{*/
#ifndef _OR_GUI_H_LOADED        /* [[[ */
#define _OR_GUI_H_LOADED

#include "gui_lib.h"

/*----------------------------------------------------------------------*/
/*                           Macros and typedefs                        */
/*----------------------------------------------------------------------*/
#define OR_GUI_HANDLE_ERR     1000  /* OR GUI handle error              */
#define OR_GUI_INV_DRAW_IMG   1001  /* Invalid drawing image            */
#define OR_GUI_DEF_MISMATCH   1002  /* Definition mismatch              */

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

/*............. Supported capture screens */
typedef enum
{
    CAPT_640_480 = 0,     /* 640 x 480 center screen area             */
    CAPT_640_480_SUBSAMP, /* 640 x 480 subsampled whole screen        */
    CAPT_320_240,         /* 320 x 240 center screen area             */
    CAPT_320_240_SUBSAMP, /* 320 x 240 subsampled whole screen        */
    CAPT_CNT              /* number of capture screen types           */
} GUI_CAPT_TYPES;

/*............. GUI events */
typedef enum
{
    OR_GUI_EVENT_NONE = 0,  /* no event                                 */
    OR_GUI_EVENT_TEACH,     /* "Teach" button click                     */
    OR_GUI_EVENT_RUN,       /* "Run" button click                       */
    OR_GUI_EVENT_STOP,      /* "Stop" button click                      */
    OR_GUI_EVENT_TRIG,      /* "Trig" button click                      */
    OR_GUI_EVENT_EXIT,      /* "Exit" button click                      */
    OR_GUI_EVENT_SHUTTER,   /* Shutter value changed                    */
    OR_GUI_EVENT_SCREEN,    /* Screen type changed                      */
    OR_GUI_EVENT_PAT        /* Pattern rectangle changed                */
} OR_GUI_EVENTS;

/*............. GUI control structure */
/* Control parameters, passed by the GUI module */
typedef struct
{
    int capt_type;      /* capture screen type - see GUI_CAPT_TYPES     */

    int capt_dx;        /* capture screen width - see capt_type         */
    int capt_dy;        /* capture screen height - see capt_type        */

    int disp_dx;        /* display screen width - depends on capt_type  */
    int disp_dy;        /* display screen height - depends on capt_type */

    int gui_event;      /* GUI event - see OR_GUI_EVENTS                */

    int run_state;      /* run state: 0=idle, 1=running ("Run" click)   */
    int run_mode;       /* run mode: 0=free run, 1=triggered            */
    int search_mode;    /* search mode:                                 */
                        /*  0 = complete                                */
                        /*  1 = until 1st good                          */
                        /*  2 = show edges in run mode (teach disabled) */
                        /*      no further search processing            */
    int live_display;   /* display live picture: 0=no, 1=yes            */

    VM_BOX pat_box;     /* pattern-window picture box                   */
    VM_BOX scr_box;     /* screen-window picture box                    */

    VM_BOX pat_rect;    /* pattern rectangle relative to screen image   */
    VM_BOX srh_rect;    /* search rectangle relative to screen image    */

    int shutter;        /* shutter value (us) [10, 30000000]            */
    int edge_contr;     /* edge contrast          [7-128]               */
    int edge_grad;      /* edge gradient          [6-128]               */
    int min_chain_len;  /* min chain length       [4-24]                */
    int min_rate;       /* min recognition rate % [15-100]              */
    int contour_width;  /* contour width          [3,5,7,9]             */
    int min_scale;      /* min scale %            [50-100]              */
    int max_scale;      /* max scale %            [100-150]             */
    int start_ang;      /* start rotation angle   [0-359]               */
    int ang_range;      /* rotation angle range   [0-360]               */

    int teach_flg;      /* taught (learned) OR pattern: 0=no, 1=yes     */
} OR_GUI;

/*----------------------------------------------------------------------*/
/*                           Function prototypes                        */
/*----------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

GUI_HND or_gui_open (
    int capt_type );    /* [in] capture screen type (see GUI_CAPT_TYPES)*/

void or_gui_close (
    GUI_HND  *or_gui_hnd );     /* [i/o] GUI module handle              */

int or_gui_exec (
    GUI_HND   or_gui_hnd,   /* [i/o] GUI module handle                  */
    image    *dr_img,       /* [in] drawing image                       */
    image    *vd_img,       /* [in] video image                         */
    MS_EVENT *ms_event,     /* [in] mouse event (see io_mouse.h)        */
    OR_GUI   *or_gui );     /* [out] OR GUI control parameters          */

int or_gui_get_params (
    GUI_HND   or_gui_hnd,   /* [i/o] GUI module handle                  */
    OR_GUI   *or_gui );     /* [out] OR GUI control parameters          */

int or_gui_set_img (
    GUI_HND   or_gui_hnd,   /* [i/o] GUI module handle                  */
    image    *capt_img,     /* [in] capture image (NULL: don't set)     */
    image    *pat_img,      /* [in] pattern image (NULL: don't set)     */
    image    *edg_img );    /* [in] pattern edge image (NULL: don't set)*/

//int or_gui_teach_flg (
//    GUI_HND   or_gui_hnd,   /* [in] GUI module handle                   */
//    int      *teach_flg );  /* [out] teach fla                          */

void or_imgbox_to_img (
    image  *in_img,         /* [in] source image                        */
    image  *out_img,        /* [out] destination image                  */
    VM_BOX *box );          /* [in] source image box                    */

void or_img_to_imgbox (
    image  *in_img,         /* [in] source image                        */
    image  *out_img,        /* [out] destination image                  */
    VM_BOX *box );          /* [in] destination image box               */

int or_gui_set_results (
    GUI_HND or_gui_hnd,     /* [in] GUI module handle                   */
    int res_rate,           /* [in] recognition rate (-1: bad result)   */
    int res_rot,            /* [in] rotation angle [0,360] deg          */
    int res_scale,          /* [in] scale factor [50,150] %             */
    int res_corners[8],     /* [in] rectangle corners                   */
    int res_time,           /* [in] search time                         */
    int good_cnt,           /* [in] counter for good patterns           */
    int bad_cnt );          /* [in] counter for bad patterns            */

#ifdef __cplusplus
}
#endif

#endif /* ]]] - _OR_GUI_H_LOADED */
/*@}*/
