/*----------------------------------------------------------------------*/
/**
* @name DR_LIB.H
*       Drawing library header
*
* <br>  VM_LIB is subject to the license and usage terms of VRmagic GmbH,
* <br>  which are distributed with this software.
* <br>
* <br>  This file contains type definitions, macros and function prototypes
*       for a platform-independent (PC + TI camera) drawing library. All
*       functions draw into image DR_IMAGE.
*
* @version
*       Created:  24.10.2005
* <br>  Modified:
*/
/*----------------------------------------------------------------------*/
/*@{*/

#ifndef _DR_LIB_H_LOADED        /* [[[ */
#define _DR_LIB_H_LOADED

/* DDD */
#define NO_COPY_RPOT  0     /* 1=disable copy protection */

#include "cdef.h"

/*----------------------------------------------------------------------*/
/*                                Version                               */
/*----------------------------------------------------------------------*/
#define DR_LIB_VERS       100   /* Version 1.00                         */
#define DR_LIB_VERS_STR  "1.00" /* Version string                       */

/*----------------------------------------------------------------------*/
/*                              Error codes                             */
/*----------------------------------------------------------------------*/
/* defined in LIB_ERR.H */

/*----------------------------------------------------------------------*/
/*                        Basic type definitions                        */
/*----------------------------------------------------------------------*/
typedef int  DR_BOX[4];
typedef int  DR_LINE[4];

/*----------------------------------------------------------------------*/
/*                             Drawing macros                           */
/*----------------------------------------------------------------------*/
/*............. Coordinate indexes */
#define DR_X1 0
#define DR_Y1 1
#define DR_X2 2
#define DR_Y2 3

/*............. General drawing modes (enabled by bits) */
//#define DR_MODE_SET              0    /* default SET mode               */
//#define DR_MODE_XOR         0x0001    /* 0: SET mode,    1: XOR mode    */
//#define DR_MODE_BGND_FILL   0x0002    /* 0: no bgnd fill 1: fill bgnd   */
//#define DR_MODE_BIT_MATRIX  0x0004    /* 0: byte matrix, 1: bit matrix  */

/*............. Rotations */
typedef enum
{
    DR_ROT_0 =  0,     /*   0 degrees            */
    DR_ROT_90,         /*  90 degrees            */
    DR_ROT_180,        /* 180 degrees            */
    DR_ROT_270,        /* 270 degrees            */
    DR_ROT_45,         /*  45 degrees            */
    DR_ROT_135,        /* 135 degrees            */
    DR_ROT_225,        /* 225 degrees            */
    DR_ROT_315         /* 315 degrees            */
} DR_ROT_MODES;

/*............. Clipping modes */
/*
* Clipping is globally enabled or disabled by dr_set_clipmode(mode),
* where "mode" has one of the following values:
*   DR_CLIP_OFF : Disable clipping (default)
*   DR_CLIP_ON  : Enable normal clipping (draw inside the clipping box)
*   DR_CLIP_INV : Enable inverse clipping (draw outside the clipping box)
* Clipping is always done inside the drawing image. The current clipping
* box is set by dr_set_clipbox. The box has absolute coordinates (the drawing
* origin is ignored).
*
* WARNING: Currently inverse clipping is not available. In the future it may
* supported in the following functions:
*   dr_dot
*   dr_line
*   dr_box
*/
typedef enum
{
    DR_CLIP_OFF = 0,    /* disable clipping             */
    DR_CLIP_ON          /* enable normal clipping       */
//    DR_CLIP_INV         /* enable inverse clipping :currently N/A */
} DR_CLIP_MODES;

/*............. Line types */
typedef enum
{
    DR_LINE_SOLID = 0,      /* solid line   */
    DR_LINE_DOTTED          /* dotted line  */
} DR_LINE_MODES;

/*............. Text/picture align options */
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

/*............. Convert bit count to byte count */
#define DR_BITS_TO_BYTES(c) (((c)+7)/8)   /* bytes per icon pixel row   */

/*----------------------------------------------------------------------*/
/*                                Colors                                */
/*----------------------------------------------------------------------*/

/*............. Number of available simultaneous colors */
/*
* Available drawing colors: from 0 to DR_COLOR_CNT-1.
* Color 0 is reserved transparent color and can't be changed by
* dr_set_palette() function.
*/
#define DR_COLOR_CNT          67  /* total # of colors for TI camera    */
#define DR_TRANSL_CLR_OFFS    64  /* offset of translucent colors       */

/*............. Default colors */
/* These colors are valid for default palette settings ! */
#define DR_COLOR_NONE            0  /* no color (transparent)           */
#define DR_COLOR_BLUE            1  /* blue                             */
#define DR_COLOR_GREEN           2  /* green                            */
#define DR_COLOR_CYAN            3  /* cyan                             */
#define DR_COLOR_RED             4  /* red                              */
#define DR_COLOR_MAGENTA         5  /* magenta                          */
#define DR_COLOR_YELLOW          6  /* yellow                           */
#define DR_COLOR_GRAY            7  /* gray                             */

#define DR_COLOR_BRIGHT_GRAY     8  /* bright gray                      */
#define DR_COLOR_BRIGHT_BLUE     9  /* bright blue                      */
#define DR_COLOR_BRIGHT_GREEN   10  /* bright green                     */
#define DR_COLOR_BRIGHT_CYAN    11  /* bright cyan                      */
#define DR_COLOR_BRIGHT_RED     12  /* bright red                       */
#define DR_COLOR_BRIGHT_MAGENTA 13  /* bright magenta                   */
#define DR_COLOR_BRIGHT_YELLOW  14  /* bright yellow                    */
#define DR_COLOR_WHITE          15  /* white (high intensity)           */

#define DR_COLOR_BLACK          16  /* black       (fgnd text color)    */
#define DR_COLOR_DARK_GRAY      17  /* dark-gray   (shadow 3-D corner)  */
#define DR_COLOR_DEF_BGND       18  /* middle-gray (windows-like bgnd)  */
#define DR_COLOR_LIGHT_GRAY     19  /* light-gray  (light 3-D corner)   */
#define DR_COLOR_TIT_BGND       20  /* unsel. window title bgnd clr     */
#define DR_COLOR_TIT_SBGND      21  /* sel. window title bgnd clr       */

#define DR_COLOR_TRANSL_BLUE    64  /* translucent blue (TI only)       */
#define DR_COLOR_TRANSL_GREEN   65  /* translucent green (TI only)      */
#define DR_COLOR_TRANSL_RED     66  /* translucent red (TI only)        */

/* One color value reserved for color icons */
#if DR_COLOR_CNT >= 255
  #error  Too great number of colors !
#endif

/*----------------------------------------------------------------------*/
/*                                 Fonts                                */
/*----------------------------------------------------------------------*/
/*
* The drawing library supports bitmap fonts with fixed width and height
* of the character. The default built-in font has character size 8x10
* and contains full ASCII code table 0-255. A character table loaded from
* file may be full or not, but must begin with code 0.
*/

/*............. Font definition structure */
typedef struct
{
    VL_UINT32 font_dx;  /* font width in pixels                         */
    VL_UINT32 font_dy;  /* font height in pixels                        */
    VL_UINT32 font_cnt; /* number of char. icons in font_tab            */
    char     *font_tab; /* font table with char. definition data in     */
                        /* bitmap format (1 byte = 1 icon pixel)        */
} DR_FONT;


/*............. Size of unpacked font table "font_tab" (1 byte/pixel) */
#define DR_FONTTAB_SIZE(font_dx,font_dy,cnt) ((font_dx)*(font_dy)*(cnt))

/*............. Built-in character table, generated by CGEN.C */
#define DR_DEFFONT_DX    8  /* default font dx  */
#define DR_DEFFONT_DY   10  /* default font dy  */
#define DR_DEFFONT_CNT 256  /* default font cnt */

/*............. Size of default font table buffer */
#define DR_DEFFONT_TABSIZE  DR_FONTTAB_SIZE(DR_DEFFONT_DX, DR_DEFFONT_DY, DR_DEFFONT_CNT)

extern unsigned char dr_DefFontTab[];   /* default char generator table */


#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------*/
/*                         Basic drawing functions                      */
/*----------------------------------------------------------------------*/
/*
* All drawing functions draw into an image (typedef DR_IMAGE). The basic
* drawing functions actually set image pixels and do:
*   - clipping inside image and current clipping box
*   - coordinate translation relative to the drawing origin
*   - color encoding for TI camera
* These functions should be compiled with the "-o3" option (best speed
* optimization).
*
* The general (non-basic) drawing functions call the basic functions. They
* should not use coordinate clipping, coordinate translation and color
* conversion.
*/

void dr_line (                                          /* DR_LINE.C    */
    DR_IMAGE *dr_img,       /* [in] drawing image                       */
    int       x1,           /* [in] x-coord. of 1-st line point         */
    int       y1,           /* [in] y-coord. of 1-st line point         */
    int       x2,           /* [in] x-coord. of 2-nd line point         */
    int       y2,           /* [in] y-coord. of 2-nd line point         */
    int       clr );        /* [in] drawing color                       */

void dr_dot_line (                                       /* DR_DLINE.C  */
    DR_IMAGE *dr_img,       /* [in] drawing image                       */
    int       x1,           /* [in] x-coord. of 1-st line point         */
    int       y1,           /* [in] y-coord. of 1-st line point         */
    int       x2,           /* [in] x-coord. of 2-nd line point         */
    int       y2,           /* [in] y-coord. of 2-nd line point         */
    int       clr );        /* [in] drawing color                       */

void dr_mat (                                           /* DR_MAT.C     */
    DR_IMAGE *dr_img,       /* [in] drawing image                       */
    DR_IMAGE *mat,          /* [in] pixel matrix image                  */
    int       x,            /* [in] x-coord. of top left corner         */
    int       y,            /* [in] y-coord. of top left corner         */
    int       clr,          /* [in] foreground pixel color              */
    int       bgnd,         /* [in] background color                    */
    int       type );       /* [in] matrix type mono/clr                */
int  dr_bitmat (
    DR_IMAGE *dr_img,       /* [in] drawing image                       */
    DR_IMAGE *mat,          /* [in] pixel matrix image in "bit" format  */
    int       x,            /* [in] x-coord. of top left 'mat' corner   */
    int       y,            /* [in] y-coord. of top left 'mat' corner   */
    int       dx,           /* [in] bit-matrix width in bits (pixels)   */
    int       clr,          /* [in] foreground pixel color              */
    int       bgnd );       /* [in] background color                    */
void dr_image (
    DR_IMAGE *dr_img,       /* [in] drawing image                       */
    DR_IMAGE *img,          /* [in] image to draw                       */
    int       x,            /* [in] x-coord. of top left corner         */
    int       y,            /* [in] y-coord. of top left corner         */
    int       clr,          /* [in] foreground drawing color            */
    int       bgnd );       /* [in] background drawing color (-1: none) */
void dr_clr_image (
    DR_IMAGE *dr_img,       /* [in] drawing image                       */
    DR_IMAGE *img,          /* [in] image to draw                       */
    int       x,            /* [in] x-coord. of top left corner         */
    int       y );          /* [in] y-coord. of top left corner         */

void dr_dot (                                           /* DR_DOT.C     */
    DR_IMAGE *dr_img,       /* [in] drawing image                       */
    int     x,              /* [in] dot x-coord.                        */
    int     y,              /* [in] dot y-coord.                        */
    int     clr );          /* [in] drawing color                       */

void dr_fill_box_xy (                                   /* DR_FILLB.C   */
    DR_IMAGE *dr_img,       /* [in] drawing image                       */
    int       x1,           /* [in] x-coord. of top/left box corner     */
    int       y1,           /* [in] y-coord. of top/left box corner     */
    int       x2,           /* [in] x-coord. of bot/right box corner    */
    int       y2,           /* [in] y-coord. of bot/right box corner    */
    int       clr );        /* [in] drawing color                       */
void dr_fill_box (
    DR_IMAGE *dr_img,       /* [in] drawing image                       */
    int     x,              /* [in] x-coord. of top/left box corner     */
    int     y,              /* [in] y-coord. of top/left box corner     */
    int     dx,             /* [in] box width in pixels                 */
    int     dy,             /* [in] box height in pixels                */
    int     clr );          /* [in] drawing color                       */

void dr_fill_img (                                      /* DR_FILLI.C   */
    DR_IMAGE *img,          /* [out] drawing image                      */
    int       clr );        /* [in] drawing color                       */
void dr_clear_img (
    DR_IMAGE *img );        /* [out] drawing image                      */

void dr_copy_img (                                      /* DR_COPYI.C   */
    DR_IMAGE *in_img,       /* [in] drawing image                       */
    DR_IMAGE *out_img );    /* [out] drawing image                      */

int  dr_ellip (                                         /* DR_ELLIP.C   */
    DR_IMAGE *dr_img,       /* [in] drawing image                       */
    int     x0,             /* [in] x-coord. of ellipse center          */
    int     y0,             /* [in] y-coord. of ellipse center          */
    int     r1,             /* [in] horizontal ellipse radius           */
    int     r2,             /* [in] vertical ellipse radius             */
    int     clr );          /* [in] drawing color                       */
//int  dr_gen_ellip (
//    int   r1,               /* [in] horizontal ellipse radius           */
//    int   r2,               /* [in] vertical ellipse radius             */
//    int  *xy_buf );         /* [out] buffer with x/y coordinates        */

int  dr_rot_ellip (                                     /* DR_ROTEL.C   */
    DR_IMAGE *dr_img,       /* [in] drawing image                       */
    float   xc,             /* [in] x-coord. of ellipse center          */
    float   yc,             /* [in] y-coord. of ellipse center          */
    float   r_max,          /* [in] greater ellipse radius              */
    float   r_min,          /* [in] smaller ellipse radius              */
    float   ang,            /* [in] rotation angle in radians           */
    int     clr );          /* [in] drawing color                       */
//void dr_dotf (
//    DR_IMAGE *dr_img,       /* [in] drawing image                       */
//    float   fx,             /* [in] dot x-coord. (float)                */
//    float   fy,             /* [in] dot y-coord. (float)                */
//    int     clr );          /* [in] drawing color                       */

/*----------------------------------------------------------------------*/
/*               General (non-basic) drawing functions                  */
/*----------------------------------------------------------------------*/
void dr_set_linetype (                                  /* DR_LIB.C     */
    int   type );           /* [in] line type                           */
int  dr_get_linetype ();
void dr_norm_coord (
    int    *x1,             /* [i/o] 1st coordinate                     */
    int    *x2 );           /* [i/o] 2nd coordinate                     */

int dr_encode_ticlr (                                   /* DR_TICLR.C   */
    int  clr );             /* [in] color                               */
void dr_set_clr_encoding (
    int  mode );            /* [in] TI color encoding: 0=off, 1=on      */
int dr_get_clr_encoding ();

int  dr_init ();                                        /* DR_INIT.C    */
void dr_close ();
void dr_set_palette (
    int     clr,            /* [in] color                               */
    int     r,              /* [in] red intensity                       */
    int     g,              /* [in] green intensity                     */
    int     b );            /* [in] blue intensity                      */

void dr_set_draworg (                                   /* DR_ORG.C     */
    int     x,              /* [in] x-coord. of drawing origin          */
    int     y );            /* [in] y-coord. of drawing origin          */
void dr_get_draworg (
    int    *x,              /* [out] x-coord. of drawing origin         */
    int    *y );            /* [out] y-coord. of drawing origin         */

void dr_set_clipbox (                                   /* DR_CLIP.C    */
    DR_IMAGE *dr_img,       /* [in] drawing image                       */
    int       x1,           /* [in] x-coord. of top/left box corner     */
    int       y1,           /* [in] y-coord. of top/left box corner     */
    int       x2,           /* [in] x-coord. of bot/right box corner    */
    int       y2 );         /* [in] y-coord. of bot/right box corner    */
void dr_get_clipbox (
    DR_IMAGE *dr_img,       /* [in] drawing image                       */
    int      *x1,           /* [out] x-coord. of top/left box corner    */
    int      *y1,           /* [out] y-coord. of top/left box corner    */
    int      *x2,           /* [out] x-coord. of bot/right box corner   */
    int      *y2 );         /* [out] y-coord. of bot/right box corner   */
void dr_set_clipmode (
    int clip_mode );        /* [in] clipping mode: 0=off,1=on, 2=inv    */
int  dr_get_clipmode ();
void dr_clip_point (
    DR_IMAGE *dr_img,       /* [in] drawing image                       */
    int      *x,            /* [i/o] x-coordinate                       */
    int      *y );          /* [i/o] y-coordinate                       */

void dr_box_xy (                                        /* DR_BOX.C     */
    DR_IMAGE *dr_img,       /* [in] drawing image                       */
    int       x1,           /* [in] x-coord. of top/left box corner     */
    int       y1,           /* [in] y-coord. of top/left box corner     */
    int       x2,           /* [in] x-coord. of bot/right box corner    */
    int       y2,           /* [in] y-coord. of bot/right box corner    */
    int       clr );        /* [in] drawing color                       */
void dr_box (
    DR_IMAGE *dr_img,       /* [in] drawing image                       */
    int     x,              /* [in] x-coord. of top/left box corner     */
    int     y,              /* [in] y-coord. of top/left box corner     */
    int     dx,             /* [in] box width in pixels                 */
    int     dy,             /* [in] box height in pixels                */
    int     clr );          /* [in] drawing color                       */

void dr_marker (                                        /* DR_MARKER.C  */
    DR_IMAGE *dr_img,       /* [in] drawing image                       */
    int     x,              /* [in] x-coordinate of marker center       */
    int     y,              /* [in] y-coordinate of marker center       */
    int     type,           /* [in] marker type: 0='+', 1='x', 2=dot    */
    int     size,           /* [in] marker size (from center to end)    */
    int     clr );          /* [in] marker color                        */
void dr_markers (
    DR_IMAGE *dr_img,       /* [in] drawing image                       */
    void   *xy_buf,         /* [in] buffer with x/y marker coordinates  */
    int     xy_type,        /* [in] type of values in xy_buf            */
    int     xy_cnt,         /* [in] number of xy_buf points (x,y)       */
    int     type,           /* [in] marker type: 0='+', 1='x', 2=dot    */
    int     size,           /* [in] marker size (from center to end)    */
    int     clr );          /* [in] marker color                        */
//void dr_xmarker (
//    DR_IMAGE *dr_img,       /* [in] drawing image                       */
//    int     x,              /* [in] x-coordinate of marker center       */
//    int     y,              /* [in] y-coordinate of marker center       */
//    int     size,           /* [in] marker size (from center to end)    */
//    int     clr );          /* [in] marker color                        */

void dr_poly (                                          /* DR_ROTR.C    */
    DR_IMAGE *dr_img,       /* [in] drawing image                       */
    int      *poly,         /* [in] buffer with polygon corner points   */
    int       n,            /* [in] number of polygon corners           */
    int       clr );        /* [in] drawing color                       */
void dr_rot_rect (
    DR_IMAGE *dr_img,       /* [in] drawing image                       */
    int     rect[8],        /* [in] buffer with rectangle points        */
    int     clr );          /* [in] drawing color                       */

void dr_arrow (                                         /* DR_ARROW.C   */
    DR_IMAGE *dr_img,   /* [in] drawing image                           */
    int     x1,         /* [in] x-coord. of 1st arrow point             */
    int     y1,         /* [in] y-coord. of 1st arrow point             */
    int     x2,         /* [in] x-coord. of 2nd arrow point (the tip)   */
    int     y2,         /* [in] y-coord. of 2nd arrow point (the tip)   */
    int     clr );      /* [in] drawing color                           */

int  dr_set_font (                                      /* DR_FONT.C    */
    char   *font_file );    /* [in] font file name                      */
void dr_close_font ();
int  dr_get_font (
    DR_FONT *cur_font );    /* [out] font definition structure          */
int  dr_get_font_size (
    int  *font_dx,          /* [out] font width in pixels               */
    int  *font_dy );        /* [out] font height in pixels              */

int  dr_text (                                          /* DR_TEXT.C    */
    DR_IMAGE *dr_img,       /* [in] drawing image                       */
    char     *str,          /* [in] string                              */
    int       x,            /* [in] x-coord. of top left corner         */
    int       y,            /* [in] y-coord. of top left corner         */
    int       clr,          /* [in] color (0 to TG_COLOR_CNT-1)         */
    int       bgnd );       /* [in] background color                    */

int  dr_txt (                                           /* DR_TXT.C     */
    image  *img,            /* [i/o] drawing image                      */
    char   *str,            /* [in]  0-term. string                     */
    int     x,              /* [in]  x-coord. of top/left text corner   */
    int     y,              /* [in]  y-coord. of top/left text corner   */
    int     size,           /* [in]  text size: 1=8x10, 2=16x20,...     */
    int     clr,            /* [in]  foreground text color              */
    int     bgnd );         /* [in]  background text color              */

void dr_xybuf (                                         /* DR_XYBUF.C   */
    DR_IMAGE *dr_img,       /* [in] drawing image                       */
    int      *xy_buf,       /* [in] buffer with x/y coordinates         */
    int       n,            /* [in] # of points (x/y pairs) in xy_buf   */
    int       clr );        /* [in] pixel color                         */

int  dr_fill_poly (                                     /* DR_FILLP.C   */
    DR_IMAGE *dr_img,       /* [in] drawing image                       */
    int      *pt_buf,       /* [in] x/y buffer with polygon points      */
    int       pt_cnt,       /* [in] # of polygon corner points          */
    int       clr );        /* [in] fill color                          */

void dr_bitmat_to_bytemat (                             /* DR_BM2BT.C   */
    char   *bit_mat,        /* [in] bit matrix                          */
    int     dx,             /* [in] bit-matrix width in pixels          */
    int     dy,             /* [in] bit-matrix height in pixels         */
    char   *byte_mat );     /* [out] byte matrix                        */


#ifdef __cplusplus
}
#endif

#endif /* ]]] - _DR_LIB_H_LOADED */

/*@}*/
