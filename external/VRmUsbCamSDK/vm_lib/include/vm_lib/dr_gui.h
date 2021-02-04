/*----------------------------------------------------------------------*/
/**
* @name DR_GUI.H
*       3-D drawing library header
*
* <br>  VM_LIB is subject to the license and usage terms of VRmagic GmbH,
* <br>  which are distributed with this software.
* <br>
* <br>  This file contains type definitions, macros and prototypes for 3-D
*       GUI drawing functions.
*
* @version
*       Created:  01.11.2005
* <br>  Modified:
*/
/*----------------------------------------------------------------------*/
/*@{*/

#ifndef _DR_GUI_H_LOADED        /* [[[ */
#define _DR_GUI_H_LOADED

#include "cdef.h"


/*----------------------------------------------------------------------*/
/*                       General macros and typedefs                    */
/*----------------------------------------------------------------------*/

/*............. 3-D box type (bit flags) */
#define GUI_BOX_DOWN  0x0001    /* specifies "up" or "down" 3D GUI box:     */
                                /* 0 = light top/left corner:               */
                                /*     protruding box like unpressed button */
                                /* 1 = dark (shadow) top/left corner:       */
                                /*     concave box like pressed button      */
#define GUI_BOX_BUT   0x0002    /* specifies normal or button GUI box:      */
                                /* 0 = normal box                           */
                                /* 1 = buton box (used for pressed button)  */

/*............. Editing modes */
#define DR_INSERT_OFF    0              /* overwrite editing mode */
#define DR_INSERT_ON     1              /* insert editing mode    */

/*............. Text cursor shapes */
#define DR_CURSOR_LINE   DR_INSERT_OFF  /* underline cursor (insert off) */
#define DR_CURSOR_BLOCK  DR_INSERT_ON   /* block cursor     (insert on)  */

/*............. Mouse cursor types */
typedef enum
{
    MS_CURSOR_ARROW = 0,     /* normal arrow corsor */
    MS_CURSOR_RESIZE         /* resize corsor */
} MS_CURSOR_TYPES;

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

/*............. Edit state structure */
#define MAX_EDIT_LEN  256   /* max length of edit string */
//#define DEF_EDIT_LEN    8   /* default length of edit box in chars */
typedef struct
{
    int edit_curs_pos;  /* cursor position (-1: no cursor)              */
    int insert_mode;    /* insert mode (cursor type):                   */
                        /*   DR_INSERT_OFF = overwrite mode             */
                        /*   DR_INSERT_ON  = insert mode                */
    int edit_start_flg; /* start editing flag (1: 1st key entered)      */
    int edit_str_beg;   /* offset of beg. string char in edit box       */

    int edit_box_len;   /* length of edit box in # of chars             */
    int max_edlen;      /* max # of chars to enter/edit (0=default)     */
    int change_flg;     /* flag for changed edit string                 */

    char edit_str[MAX_EDIT_LEN+1];      /* current edit string          */
    char edit_str_old[MAX_EDIT_LEN+1];  /* saved edit string            */

} GUI_EDIT_STATE;

/*----------------------------------------------------------------------*/
/*                                Button                                */
/*----------------------------------------------------------------------*/

/*............. Button state (OR of bits) */
#define DR_BUT_PRESS    0x0001  /* pressed button   */
#define DR_BUT_SEL      0x0002  /* selected button  */

/*----------------------------------------------------------------------*/
/*                          Function prototypes                         */
/*----------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

void dr_gui_box_xy (                                    /* DR_GBOX.C    */
    DR_IMAGE *dr_img,       /* [in] drawing image                       */
    int       box_x1,       /* [in] x-coord. of top/left box corner     */
    int       box_y1,       /* [in] y-coord. of top/left box corner     */
    int       box_x2,       /* [in] x-coord. of bot/right box corner    */
    int       box_y2,       /* [in] y-coord. of bot/right box corner    */
    int       type,         /* [in] box type                            */
    int       lclr,         /* [in] color of light box corner           */
    int       sclr,         /* [in] color of shadow box corner          */
    int       depth );      /* [in] box depth                           */
void dr_gui_box (
    DR_IMAGE *dr_img,       /* [in] drawing image                       */
    int       x,            /* [in] x-coord. of top/left box corner     */
    int       y,            /* [in] y-coord. of top/left box corner     */
    int       dx,           /* [in] box width in pixels                 */
    int       dy,           /* [in] box height in pixels                */
    int       type,         /* [in] box type                            */
    int       lclr,         /* [in] color of light box corner           */
    int       sclr,         /* [in] color of shadow box corner          */
    int       depth );      /* [in] box depth                           */

int  dr_gui_label (                                     /* DR_PBOX.C    */
    DR_IMAGE *dr_img,       /* [in] drawing image                       */
    int       x1,           /* [in] x-coord. of top/left box corner     */
    int       y1,           /* [in] y-coord. of top/left box corner     */
    int       x2,           /* [in] x-coord. of bot/right box corner    */
    int       y2,           /* [in] y-coord. of bot/right box corner    */
    void     *label_buf,    /* [in] picture buffer                      */
    int       label_type,   /* [in] picture type (text, mono/clr icon)  */
    int       hor_align,    /* [in] horizontal text/picture alignment   */
    int       ver_align,    /* [in] vertical text/picture alignment     */
    int       fgnd,         /* [in] text/mono icon color                */
    int       bgnd );       /* [in] background color                    */
int  dr_pict_box (
    DR_IMAGE *dr_img,       /* [in] drawing image                       */
    int       box_x1,       /* [in] x-coord. of top/left box corner     */
    int       box_y1,       /* [in] y-coord. of top/left box corner     */
    int       box_x2,       /* [in] x-coord. of bot/right box corner    */
    int       box_y2,       /* [in] y-coord. of bot/right box corner    */
    void     *pict_buf,     /* [in] picture buffer                      */
    int       pict_type,    /* [in] picture type (text, mono/clr icon)  */
    int       x,            /* [in] text/pict x-coord.                  */
    int       y,            /* [in] text/pict y-coord.                  */
    int       fgnd,         /* [in] text/mono icon color                */
    int       bgnd );       /* [in] background color                    */
int  dr_get_pict_size (
    void     *pict_buf,     /* [in] picture buffer                      */
    int       pict_type,    /* [in] picture type (text, mono/clr icon)  */
    int      *pict_dx,      /* [out] picture width                      */
    int      *pict_dy );    /* [out] picture height                     */
void dr_align_pict (
    int   box_dx,           /* [in] width of enclosing box              */
    int   box_dy,           /* [in] height of enclosing box             */
    int   pict_dx,          /* [in] picture width                       */
    int   pict_dy,          /* [in] picture height                      */
    int   hor_align,        /* [in] horizontal alignment                */
    int   ver_align,        /* [in] vertical alignment                  */
    int  *x,                /* [out] x-coord. of top/left pict corner   */
    int  *y );              /* [out] y-coord. of top/left pict corner   */

void dr_mouse_cursor (                                  /* DR_MSCUR.C   */
    DR_IMAGE *dr_img,       /* [in] drawing image                       */
    int       x,            /* [in] x-coord. of cursor hot spot         */
    int       y );          /* [in] x-coord. of cursor hot spot         */
void dr_set_mouse_cursor_type (
    int   type );           /* [in] mouse cursor type                   */

int  dr_button (                                        /* DR_BUT.C     */
    DR_IMAGE *dr_img,   /* [in] drawing image                           */
    void *but_label,    /* [in] button label buffer (NULL = none)       */
    int   but_type,     /* [in] button type                             */
    int   but_x,        /* [in] x-coord. of top/left button corner      */
    int   but_y,        /* [in] y-coord. of top/left button corner      */
    int   but_dx,       /* [in] button width in pixels (0=auto)         */
    int   but_dy,       /* [in] button height in pixels (0=auto)        */
    int  *but_clrs,     /* [in] button colors: fclr,bclr,lclr,sclr      */
    int   but_depth,    /* [in] button depth (1 or more)                */
    int   hor_align,    /* [in] button horizontal label alignment       */
    int   ver_align,    /* [in] button vertical label alignment         */
    int   but_state );  /* [in] button state (OR of bits)               */

int  dr_mrr_button (                                    /* DR_GUIR.C    */
    DR_IMAGE *dr_img,       /* [in] drawing image                       */
    int       x,            /* [in] x-coord. of center button point     */
    int       y,            /* [in] x-coord. of center button point     */
    unsigned  int type,     /* [in] button type: 0=mov, 1=res, 2=rot    */
    int       state );      /* [in] button state: 0=rel, 1=pr           */
void dr_mrr_button_size (
    int  *but_dx,           /* [out] width of move/resize/rot buttons   */
    int  *but_dy );         /* [out] height of move/resize/rot buttons  */

int  dr_edit_box (                                      /* DR_EDIT.C    */
    DR_IMAGE *dr_img,       /* [in] drawing image                       */
    char     *txt,          /* [in] 0-term text to draw (NULL=none)     */
    int       x,            /* [in] x-coord. of top/left edit box corner*/
    int       y,            /* [in] y-coord. of top/left edit box corner*/
    int       dx,           /* [in] edit box width in pixels            */
    int       dy,           /* [in] edit box height in pixels           */
    int       depth,        /* [in] box depth (0=2D box)                */
    int      *colors,       /* [in] buffer with edit box colors         */
    int       curs_pos,     /* [in] text cursor position (-1: no curs)  */
    int       curs_typ,     /* [in] text cursor type (underline/block)  */
    int       curs_clr );   /* [in] text cursor color                   */

int  gui_edit_init (                                    /* GUI_EDIT.C   */
    GUI_EDIT_STATE *edit_state, /* [out] edit state structure           */
    int     curs_pos,           /* [in] cursor position                 */
    int     insert_mode,        /* [in] insert mode = off/on            */
    int     edit_box_len,       /* [in] edit box length in char #       */
    int     max_edlen,          /* [in] max # of chars to enter/edit    */
    char   *edit_str );         /* [in] string to edit                  */
void gui_edit_get_parms (
    GUI_EDIT_STATE *edit_state, /* [in] edit state structure            */
    char          **edit_str,   /* [out] ptr to current edit string     */
    int            *curs_pos,   /* [out] cursor position (-1=none)      */
    int            *curs_type );/* [out] cursor type (overwrite/insert) */
char *gui_edit_get_str (
    GUI_EDIT_STATE *edit_state );/* [in] edit state structure           */
void gui_edit_set_str (
    GUI_EDIT_STATE *edit_state,  /* [in] edit state structure           */
    char           *str );       /* [in] edit string to set             */
int  gui_edit_cmd_proc (
    GUI_EDIT_STATE *edit_state, /* [i/o] edit state structure           */
    int     edit_key,           /* [in] key code                        */
    int    *cursor_pos,         /* [out] current cursor position        */
    int    *cursor_type,        /* [out] current cursor type (ins/ovr)  */
    char  **str );              /* [out] current string to display      */


#ifdef __cplusplus
}
#endif

#endif /* ]]] - _DR_GUI_H_LOADED */
/*@}*/

