/*----------------------------------------------------------------------*/
/**
* @name CL_LIB.H
*       Calculation and geometry library
*
* <br>  VM_LIB is subject to the license and usage terms of VRmagic GmbH,
* <br>  which are distributed with this software.
* <br>
* <br>  Header file for calculation and geometry library functions.
*
* @version
*       Created:  27.10.2005
* <br>  Modified:
*/
/*----------------------------------------------------------------------*/
/*@{*/

#ifndef _CL_LIB_H_LOADED   /* [[[ */
#define _CL_LIB_H_LOADED

#include <limits.h>
#include "cdef.h"

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

/*............. Round float to integer */
#ifndef F2I_ROUND
  #define F2I_ROUND(x)  ((int)(((x) >= 0) ? (x) + 0.5f : (x) - 0.5f))
#endif

/*............. Conversion between angles in radians and degrees */
#define RAD_TO_DEG(rad) (float)((rad)*180.0f / ANG_PI)  /* radians -> degrees */
#define DEG_TO_RAD(deg) (float)((deg)*ANG_PI / 180.0f)  /* degrees -> radians */

/*............. Various constants */
#ifndef ANG_3PI2
  #define DBL_PI     3.1415926535897932
  //#define DBL_PI   3.1415926535897932384626433832795
  #define ANG_PI   ((float)DBL_PI)        /* PI       */
  #define ANG_PI2  ((float)(DBL_PI/2))    /* PI/2     */
  #define ANG_3PI2 ((float)(3*DBL_PI/2))  /* 3*PI/2   */
  #define ANG_2PI  ((float)(2*DBL_PI))    /* 2*PI     */
#endif

#define SQRT_2   1.414213562373095   /* sqrt(2)  */

/*............. Segment definition structure */
/*
* This structure is used to define convex figures like rotated rectangles
* and ellipses by pixel line segments. The seg_buf buffer contains the
* beginning and the end x-coordinates of each pixel line segment, which
* belongs to the figure:
*                         seg_buf[2*i]  seg_buf[2*i+1]
*                         ------------  --------------
*    y = y_min          :      x1             x2
*    y = y_min + 1      :      x1             x2
*    . . .   . . . . . . . . . . . . . . . . . . .
*    y = y_min + dy - 1 :      x1             x2
*
* where:
*    x1 = min x-coord. of current segment (CL_SEG_NONE: N/A)
*    x2 = max x-coord. of current segment (CL_SEG_NONE: N/A)
* The size of seg_buf is dy*2 integer words.
*/
#define CL_SEG_NONE  INT_MAX   /* value of uninitialized segment */

#ifndef SEG_DEF_DEFINED    /* [[[ */
  #define SEG_DEF_DEFINED
  typedef struct
  {
      int *seg_buf;   /* segment buffer (x1,x2) (x1,x2) ...       */
      int  y_min;     /* minimum y-coordinate (top pixel row)     */
      int  dy;        /* number of segments (x1,x2) in seg_buf    */
                      /* (figure height in pixels)                */
  } SEG_DEF;
#endif  /* ]]] */

/*............. Fast integer sin/cos generator handle */
typedef void * SC_HND;

/*............. Data types */
typedef enum
{
    CL_CHAR = 0,    /* 8-bit char                                      */
    CL_UCHAR,       /* 8-bit unsigned char                             */
    CL_SHORT,       /* 16-bit integer                                  */
    CL_USHORT,      /* 16-bit unsigned integer                         */
    CL_INT,         /* 32-bit integer                                  */
    CL_UINT,        /* 32-bit unsigned integer                         */
    CL_INT64,       /* 64-bit integer                                  */
    CL_UINT64,      /* 64-bit unsigned integer                         */
    CL_FLOAT,       /* 32-bit float                                    */
    CL_DOUBLE,      /* 64-bit float                                    */
    CL_STR          /* 0-terminated string with arbitrary length       */
} CL_DATA_TYPES;

/*----------------------------------------------------------------------*/
/*                           Function prototypes                        */
/*----------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

float cl_vect_angle (                                   /* cl_vang.c    */
    float   x1,         /* [in] x-coord. of beginning vector point      */
    float   y1,         /* [in] y-coord. of beginning vector point      */
    float   x2,         /* [in] x-coord. of end vector point            */
    float   y2 );       /* [in] y-coord. of end vector point            */

float cl_vect_angle2 (                                  /* cl_vang2.c   */
    float  vec1[4],     /* [in] buffer with 1st vector points           */
    float  vec2[4] );   /* [in] buffer with 2nd vector points           */

void cl_rot_pnt (                                       /* cl_rotp.c    */
    float  *x,          /* [i/o] point x-coordinate                     */
    float  *y,          /* [i/o] point y-coordinate                     */
    float   ang );      /* [in] rotation angle in radians               */

void cl_rot_vect (                                      /* cl_rotv.c    */
    float   x1,         /* [in] x-coord. of 1st vector point            */
    float   y1,         /* [in] y-coord. of 1st vector point            */
    float  *x2,         /* [i/o] x-coord. of 2nd vector point           */
    float  *y2,         /* [i/o] y-coord. of 2nd vector point           */
    float   ang );      /* [in] rotation angle in radians               */

void cl_rot_rect (                                      /* cl_rotr.c    */
    float   rect[8],    /* [i/o] x/y buffer with rect. corners          */
    float   x0,         /* [in] x-coord. of rotation point              */
    float   y0,         /* [in] y-coord. of rotation point              */
    float   ang );      /* [in] rotation angle in radians               */

void cl_rect_corners (                                  /* cl_rectc.c   */
    int     x,              /* [in] x-coord. of rectangle-origin point  */
    int     y,              /* [in] y-coord. of rectangle-origin point  */
    int     dx,             /* [in] rectangle width in pixels           */
    int     dy,             /* [in] rectangle height in pixels          */
    int     pt_pos,         /* [in] point position: 0=cent, 1=top/left  */
    float   ang,            /* [in] rotation angle in radians [0,2PI]   */
    float   rect[8] );      /* [out] x/y buffer with rectangle corners  */

int  cl_round (                                         /* cl_round.c   */
    float   x );        /* [in] float number                            */
int  si_round (
    int   x );          /* [in] scaled*1000 integer number              */
int  si_round_100 (
    int   x );          /* [in] scaled*100 integer number               */

void cl_gen_line_equ (                                  /* cl_glequ.c   */
    float   x1,         /* [in] x-coord. of 1st line point              */
    float   y1,         /* [in] y-coord. of 1st line point              */
    float   x2,         /* [in] x-coord. of 2nd line point              */
    float   y2,         /* [in] y-coord. of 2nd line point              */
    float   coef[3] );  /* [out] buffer with line equation coeff.       */

int  cl_parallel_line (                                 /* cl_parl.c    */
    float   coef[3],    /* [i/o] line equation buffer (A,B,C)           */
    float   x,          /* [in] x-coord. of line definition point       */
    float   y );        /* [in] y-coord. of line definition point       */

int  cl_2pt_perp_line (                                 /* cl_perpl.c   */
    float line1[4],     /* [in] line (x1,y1) (x2,y2)                    */
    float line2[4],     /* [out] prependecular line (xl,yl) (xr,yr)     */
    float dist );       /* [in] distance from (x1,y1) to result points  */

void cl_prolong_line (                                  /* cl_prol.c    */
    int     x1,         /* [in] x-coord. of 1st line point              */
    int     y1,         /* [in] y-coord. of 1st line point              */
    int     x2,         /* [in] x-coord. of 2nd line point              */
    int     y2,         /* [in] y-coord. of 2nd line point              */
    int     dist,       /* [in] prolongation distance                   */
    int    *x,          /* [out] x-coord. of result point               */
    int    *y );        /* [out] y-coord. of result point               */

float cl_dist (                                         /* cl_dist.c    */
    float   x1,         /* [in] x-coordinate of 1st point               */
    float   y1,         /* [in] y-coordinate of 1st point               */
    float   x2,         /* [in] x-coordinate of 2nd point               */
    float   y2 );       /* [in] y-coordinate of 2nd point               */

int  cl_pnt_in_convex (                                 /* cl_chkpt.c   */
    int   x,            /* [in] x-coord. of tested point                */
    int   y,            /* [in] y-coord. of tested point                */
    int  *pbuf,         /* [in] polygon x/y buffer                      */
    int   cnt );        /* [in] # of polygon corners                    */
int  cl_pnt_in_box_xy (
    int   x,            /* [in] x-coord. of tested point                */
    int   y,            /* [in] y-coord. of tested point                */
    int   box_x1,       /* [in] x-coord. of top/left box corner         */
    int   box_y1,       /* [in] y-coord. of top/left box corner         */
    int   box_x2,       /* [in] x-coord. of bot/right box corner        */
    int   box_y2 );     /* [in] y-coord. of bot/right box corner        */
int  cl_pnt_in_box (
    int   x,            /* [in] x-coord. of tested point                */
    int   y,            /* [in] y-coord. of tested point                */
    int   box_x,        /* [in] x-coord. of top/left box corner         */
    int   box_y,        /* [in] y-coord. of top/left box corner         */
    int   dx,           /* [in] box width in pixels                     */
    int   dy );         /* [in] box height in pixels                    */
int  pt_inbox (
    int     x,          /* [in] x-coordinate of tested point            */
    int     y,          /* [in] y-coordinate of tested point            */
    VM_BOX *box );      /* [in] VM_BOX structure x,y,dx,dy              */

float cl_sin (                                          /* cl_sin.c     */
    float   ang );      /* [in] function argument in radians            */

float cl_cos (                                          /* cl_cos.c     */
    float   ang );      /* [in] function argument in radians            */

int cl_check_angle (                                    /* cl_chkan.c   */
    float   ang,        /* [in] angle in radians                        */
    float   eps );      /* [in] allowed angle tolerance                 */

int  cl_line_cross (                                    /* cl_lcros.c   */
    float   coef1[3],   /* [in] equation buffer of 1st line (A,B,C)     */
    float   coef2[3],   /* [in] equation buffer of 2nd line (A,B,C)     */
    float  *x,          /* [out] x-coord. of cross point                */
    float  *y );        /* [out] y-coord. of cross point                */

int cl_gen_line (                                       /* cl_genl.c    */
    int     x1,         /* [in] x-coord. of 1st line point              */
    int     y1,         /* [in] y-coord. of 1st line point              */
    int     x2,         /* [in] x-coord. of 2nd line point              */
    int     y2,         /* [in] y-coord. of 2nd line point              */
    int   **xy_buf );   /* [out] buffer with x/y coordinates            */

int  cl_gen_ellip (                                     /* cl_gene.c    */
    int      x0,        /* [in] x-coord. of ellipse center              */
    int      y0,        /* [in] y-coord. of ellipse center              */
    unsigned int r1,    /* [in] horizontal ellipse radius               */
    unsigned int r2,    /* [in] vertical ellipse radius                 */
    int      quad,      /* [in] # of quadrants to generate              */
    int    **xy_buf );  /* [out] buffer with x/y coordinates            */

int  cl_ellip_quad (                                    /* cl_equad.c   */
    unsigned int r1,    /* [in] horizontal ellipse radius               */
    unsigned int r2,    /* [in] vertical ellipse radius                 */
    int    **xy_buf );  /* [out] buffer with x/y coordinates            */

int  cl_poly_to_seg (                                   /* cl_pol2s.c   */
    int     *pt_buf,    /* [in] convex x/y buffer                       */
    int      pt_cnt,    /* [in] # of convex corners                     */
    SEG_DEF *seg_def ); /* [out] segment definition structure           */

int  cl_ellip_to_seg (                                  /* cl_el2s.c    */
    int      x0,        /* [in] x-coord. of ellipse center              */
    int      y0,        /* [in] y-coord. of ellipse center              */
    unsigned int r1,    /* [in] horizontal ellipse radius               */
    unsigned int r2,    /* [in] vertical ellipse radius                 */
    SEG_DEF *seg_def ); /* [out] segment definition structure           */

void cl_rot_xybuf (                                     /* cl_rotxy.c   */
    int    *xybuf,          /* [i/o] x/y buffer                         */
    int     pt_cnt,         /* [in] number of xybuf points (x,y)        */
    float   x0,             /* [in] x-coord. of rotation point          */
    float   y0,             /* [in] y-coord. of rotation point          */
    float   ang );          /* [in] rotation angle in radians           */

int  cl_rot_ellip (                                     /* cl_rotel.c   */
    float    xcen,          /* [in] x-coord. of ellipse center          */
    float    ycen,          /* [in] y-coord. of ellipse center          */
    float    a,             /* [in] major radius                        */
    float    b,             /* [in] minor radius                        */
    float    phi,           /* [in] rotation angle in radians           */
    SEG_DEF *seg_def );     /* [out] segment definition structure       */

float cl_isqrt (                                        /* cl_isqrt.c   */
    float   x );        /* [in] single-precision function argument      */

int cl_minma (                                          /* cl_minma.c   */
    int  *in_buf,           /* [in] buffer to scan for min/max values   */
    int   in_cnt,           /* [in] number of in_buf elements           */
    int   mode,             /* [in] operation mode                      */
    int   minmax_type,      /* [in] searched min/max type               */
    int   threshold,        /* [in] min/max detection threshold         */
    int  *res_buf,          /* [out] result buffer                      */
    int  *res_cnt );        /* [i/o] result count: [in] = res_buf size  */

/* Fast SIN/COS generator with integer calculations */
SC_HND sc_gen_open (                                    /* SC_GEN.C     */
    int  sc_size,           /* [in] SC table size                       */
    int  sc_fract );        /* [in] SC fraction size, 0=def(10000)      */
void sc_gen_close (
    SC_HND  *hnd );         /* [in/out] SIN/COS generator handle        */
float  sc_rot_ang (
    SC_HND  hnd,            /* [in] SIN/COS generator handle            */
    int     ang_id );       /* [in] rotation angle index                */
int sc_rot_image_size (
    int     dx,             /* [in] width of source image               */
    int     dy,             /* [in] height of source image              */
    float   ang,            /* [in] rotation angle in radians           */
    int    *rot_dx,         /* [out] width of rotated image             */
    int    *rot_dy );       /* [out] height of rotated image            */
int  sc_rot_img (
    SC_HND  hnd,            /* [in] SIN/COS generator handle            */
    image  *in_img,         /* [in] image to rotate                     */
    image  *out_img,        /* [out] rotated image                      */
    int     ang_id,         /* [in] rotation angle index                */
    int     pix_fill,       /* [in] pixel value to fill                 */
    int     bw_mode,        /* [in] B&W image mode: 0=gray, 1=B&W       */
    int     bw_clr1,        /* [in] B&W image color 1                   */
    int     bw_clr2 );      /* [in] B&W image color 2                   */

//int sc_gen_sin (
//    SC_HND  hnd,            /* [in] SIN/COS generator handle            */
//    unsigned int ang_id );  /* [in] angle id in the range [0,sc_size-1] */
//int sc_gen_cos (
//    SC_HND  hnd,            /* [in] SIN/COS generator handle            */
//    unsigned int ang_id );  /* [in] angle id in the range [0,sc_size-1] */
//int sc_rot_vect (
//    SC_HND  hnd,            /* [in] SIN/COS generator handle            */
//    int     x1,             /* [in] x-coord. of 1st vector point        */
//    int     y1,             /* [in] y-coord. of 1st vector point        */
//    int    *x2,             /* [in/out] x-coord. of end vector point    */
//    int    *y2,             /* [in/out] y-coord. of end vector point    */
//    unsigned int ang_id );  /* [in] angle id in the range [0,sc_size-1] */
//int  sc_rot_img (
//    SC_HND  hnd,            /* [in] SIN/COS generator handle            */
//    image  *in_img,         /* [in] image to rotate                     */
//    float   x0,             /* [in] x-coord. of rotation origin point   */
//    float   y0,             /* [in] y-coord. of rotation origin point   */
//    int     ang_id,         /* [in] rotation angle index                */
//    int     bw_mode,        /* [in] B&W image mode                      */
//    int     bgnd_clr,       /* [in] B&W image - background color        */
//    image  *out_img );      /* [out] rotated image                      */

int  cl_line_pt (                                       /* cl_linpt.c   */
    float   line[4],    /* [in] line (x1,y1) (x2,y2)                    */
    float   dist,       /* [in] distance from (x1,y1) to result points  */
    float  *x,          /* [out] x-coord. of result point               */
    float  *y );        /* [out] y-coord. of result point               */

int cl_sort (                                           /* cl_sort.c    */
    void *buf,          /* [i/o] data buffer to sort                    */
    int   el_cnt,       /* [in] # of buf elements                       */
    int   el_size,      /* [in] size of buffer element in bytes         */
    int   data_offs,    /* [in] offset of the data field to sort by     */
    int   data_type,    /* [in] type of data field to sport by          */
    int   sort_dir );   /* [in] sort direction: 0=asc, 1=desc           */

void cl_qsort (                                         /* cl_qsort.c   */
    void  *buf,             /* [i/o] buffer to sort                     */
    int    el_count,        /* [in] # of buffer elements                */
    int    el_size,         /* [in] size of buffer elements in bytes    */
    int (*comp_fp)(const void *, const void *));/* [in] compare func    */
void cl_set_qsort_mode (
    int  mode );            /* [in] qsort mode: 0=C qsort, 1=VM_LIB qsort */


#ifdef __cplusplus
}
#endif

/* Alias name for ip_line_segm from IP_LIB */
#define cl_line_segm ip_line_segm

#endif /* ]]] _CL_LIB_H_LOADED */
/*@}*/
