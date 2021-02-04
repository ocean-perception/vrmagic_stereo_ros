/****** IP_LIB.H = Image-processing library header = 11.2005 ************/
/*----------------------------------------------------------------------*/
/**
* @name IP_LIB.H
*       Image processing ibrary
*
* <br>  VM_LIB is subject to the license and usage terms of VRmagic GmbH,
* <br>  which are distributed with this software.
* <br>
* <br>  Header file for the image-processing library.
*
* @version
*       Created:  14.11.2005
* <br>  Modified:
*/
/*----------------------------------------------------------------------*/
/*@{*/

#ifndef _IP_LIB_H_LOADED   /* [[[ */
#define _IP_LIB_H_LOADED

#include "cdef.h"   /* common definitions */

/*----------------------------------------------------------------------*/
/*                           Macros and typedefs                        */
/*----------------------------------------------------------------------*/

/*............. 32-bit image */
/*
* Each image element (pixel) is 32-bit integer number.
*/
#ifndef IMAGE32_DEFINED     /* [[[ */
  typedef struct
  {
      int *st;      /* ptr to 32-bit integer image data buffer  */
      int  dx;      /* image width in int's                     */
      int  dy;      /* image height in int's                    */
      int  pitch;   /* image pitch in int's                     */
  } image32;
  #define IMAGE32_DEFINED
#endif  /* ]]] */

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

#ifndef SEG_DEF_DEFINED    /* [[[ - see cl_lib.h */
  #define SEG_DEF_DEFINED
  typedef struct
  {
      int *seg_buf;   /* segment buffer (x1,x2) (x1,x2) ...       */
      int  y_min;     /* minimum y-coordinate (top pixel row)     */
      int  dy;        /* number of segments (x1,x2) in seg_buf    */
                      /* (figure height in pixels)                */
  } SEG_DEF;
#endif  /* ]]] */

/*............. VCLIB-compatible macros for image combination */
#define ip_add2(a, b, c, q) ip_img_combine(a, b, c, (void (*)(int, unsigned char *,unsigned char *,unsigned char *, int))u8_add2f, q)
#define ip_sub2(a, b, c)    ip_img_combine(a, b, c, (void (*)(int, unsigned char *,unsigned char *,unsigned char *, int))u8_sub2f, 0)
#define ip_max2(a, b, c)    ip_img_combine(a, b, c, (void (*)(int, unsigned char *,unsigned char *,unsigned char *, int))u8_max2f, 0)
#define ip_min2(a, b, c)    ip_img_combine(a, b, c, (void (*)(int, unsigned char *,unsigned char *,unsigned char *, int))u8_min2f, 0)
#define ip_and2(a, b, c)    ip_img_combine(a, b, c, (void (*)(int, unsigned char *,unsigned char *,unsigned char *, int))u8_and2f, 0)
#define ip_or2(a, b, c)     ip_img_combine(a, b, c, (void (*)(int, unsigned char *,unsigned char *,unsigned char *, int))u8_or2f,  0)
#define ip_xor2(a, b, c)    ip_img_combine(a, b, c, (void (*)(int, unsigned char *,unsigned char *,unsigned char *, int))u8_xor2f, 0)
#define ip_not2(a, b, c)    ip_img_combine(a, b, c, (void (*)(int, unsigned char *,unsigned char *,unsigned char *, int))u8_not2f, 0)

//#define ip_add2(a, b, c, q) ip_img_combine(a, b, c, (void (*)())u8_add2f, q)
//#define ip_sub2(a, b, c)    ip_img_combine(a, b, c, (void (*)())u8_sub2f, 0)
//#define ip_max2(a, b, c)    ip_img_combine(a, b, c, (void (*)())u8_max2f, 0)
//#define ip_min2(a, b, c)    ip_img_combine(a, b, c, (void (*)())u8_min2f, 0)
//#define ip_and2(a, b, c)    ip_img_combine(a, b, c, (void (*)())u8_and2f, 0)
//#define ip_or2(a, b, c)     ip_img_combine(a, b, c, (void (*)())u8_or2f,  0)
//#define ip_xor2(a, b, c)    ip_img_combine(a, b, c, (void (*)())u8_xor2f, 0)
//#define ip_not2(a, b, c)    ip_img_combine(a, b, c, (void (*)())u8_not2f, 0)



/*----------------------------------------------------------------------*/
/*                           Function prototypes                        */
/*----------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

int ip_alloc_img (                                      /* IP_ALLOC.C   */
    int     dx,             /* [in] image width in pixels               */
    int     dy,             /* [in] image height in pixels              */
    image  *img );          /* [out] allocated image                    */
int ip_alloc_img32 (
    int       dx,           /* [in] image width in pixels               */
    int       dy,           /* [in] image height in pixels              */
    int       pitch,        /* [in] image pitch in pixels (>=dx)        */
    image32  *img );        /* [out] allocated 32-bit image             */
void ip_free_img (
    image  *img );          /* [i/o] image                              */
void ip_free_img32 (
    image32  *img );        /* [i/o] 32-bit image                       */

void ip_img_copy (                                      /* IP_ICOPY.C   */
    VD_IMAGE *in_img,       /* [in] source image                        */
    VD_IMAGE *out_img );    /* [out] destination image                  */
void ip_copy_imgbox (
    image  *in_img,         /* [in] source image                        */
    image  *out_img,        /* [out] destination image                  */
    VM_BOX *box,            /* [in] source/destination image box        */
    int     mode );         /* [in] copy mode                           */
void ip_imgbox_to_img (
    image  *in_img,         /* [in] source image                        */
    image  *out_img,        /* [out] destination image                  */
    VM_BOX *box );          /* [in] source image box                    */
void ip_img_to_imgbox (
    image  *in_img,         /* [in] source image                        */
    image  *out_img,        /* [out] destination image                  */
    VM_BOX *box );          /* [in] destination image box               */
void ip_copy_img (
    VD_IMAGE *in_img,       /* [in] source image                        */
    VD_IMAGE *out_img,      /* [out] destination image                  */
    int       x,            /* [in] x-coord. in destination image       */
    int       y );          /* [in] y-coord. in destination image       */

void ip_img_pyramid (                                   /* IP_PYR.C     */
    VD_IMAGE *in_img,       /* [in] image                               */
    VD_IMAGE *out_img );    /* [out] image                              */

void ip_img_subsample (                                 /* IP_SSAMP.C   */
    VD_IMAGE *in_img,       /* [in] image                               */
    VD_IMAGE *out_img,      /* [out] image                              */
    int       hor_step,     /* [in] horozontal subsampling step         */
    int       ver_step );   /* [in] vertical subsampling step           */

void ip_img_combine (                                   /* IP_ICOMB.C   */
    VD_IMAGE *img_a,        /* [in] image                               */
    VD_IMAGE *img_b,        /* [in] image                               */
    VD_IMAGE *img_c,        /* [out] image                              */
//    void    (*func)(),      /* [in] ptr to basic combination function   */
    void   (*func)(int, unsigned char *,unsigned char *,unsigned char *, int),
    int       q );          /* [in] "func" parameter (shift value)      */

void ip_set_img (                                       /* IP_FILLI.C   */
    VD_IMAGE *img,          /* [out] destination image                  */
    int       val );        /* [in] pixel value to set                  */
void ip_img_set (
    VD_IMAGE *img,          /* [out] destination image                  */
    VD_IMAGE *mask_img,     /* [in] mask image (NULL: no mask)          */
    int       val );        /* [in] pixel value to set                  */
void ip_clear_img (
    VD_IMAGE *img );        /* [out] video image                        */

int ip_truncate_rect (                                  /* IP_TRUNR.C   */
    VD_IMAGE *img,          /* [in] video image                         */
    int    *x,              /* [i/o] x-coord. of top/left rect. corner  */
    int    *y,              /* [i/o] y-coord. of top/left rect. corner  */
    int    *dx,             /* [i/o] rectangle width in pixels          */
    int    *dy );           /* [i/o] rectangle height in pixels         */

int  ip_rect_binar (                                    /* IP_BINAR.C   */
    VD_IMAGE *img,          /* [in/out] video image                     */
    int     x,              /* [in] x-coord. of top/left rect. corner   */
    int     y,              /* [in] y-coord. of top/left rect. corner   */
    int     dx,             /* [in] rectangle width in pixels           */
    int     dy,             /* [in] rectangle height in pixels          */
    int     lo_th,          /* [in] lower binarization threshold        */
    int     up_th,          /* [in] upper binarization threshold        */
    int     bgnd_clr,       /* [in] background pixel value              */
    int     obj_clr );      /* [in] object pixel value                  */
int  ip_img_binar (
    VD_IMAGE *img,          /* [in/out] video image                     */
    int     lo_th,          /* [in] lower binarization threshold        */
    int     up_th,          /* [in] upper binarization threshold        */
    int     bgnd_clr,       /* [in] background pixel value              */
    int     obj_clr );      /* [in] object pixel value                  */

int ip_binv (                                           /* IP_BINV.C    */
    VD_IMAGE *vd_img,       /* [in/out] video image                     */
    SEG_DEF  *seg_def,      /* [in] ROI definition structure            */
    int       nbr_dx,       /* [in] width of neighbouring pixel rect    */
    int       nbr_dy,       /* [in] height of neighbouring pixel rect   */
    int       x_step,       /* [in] horizontal step (1=all)             */
    int       y_step,       /* [in] vertical step (1=all)               */
    unsigned char dark_clr,     /* [in] dark binarization color         */
    unsigned char light_clr );  /* [in] light binarization color        */
int ip_ellip_binv (
    VD_IMAGE *vd_img,       /* [in/out] video image                     */
    int       x0,           /* [in] x-coord. of ellipse center          */
    int       y0,           /* [in] y-coord. of ellipse center          */
    unsigned  int r1,       /* [in] horizontal ellipse radius           */
    unsigned  int r2,       /* [in] vertical ellipse radius             */
    int       nbr_dx,       /* [in] width of neighbouring pixel rect    */
    int       nbr_dy,       /* [in] height of neighbouring pixel rect   */
    int       x_step,       /* [in] horizontal skip step (1=all)        */
    int       y_step,       /* [in] vertical skip step (1=all)          */
    unsigned char dark_clr,     /* [in] dark binarization color         */
    unsigned char light_clr );  /* [in] light binarization color        */
int ip_rect_binv (
    VD_IMAGE *vd_img,       /* [in/out] video image                     */
    int       x,            /* [in] x-coord. of top/left rect corner    */
    int       y,            /* [in] y-coord. of top/left rect corner    */
    int       dx,           /* [in] rectangle width in pixels           */
    int       dy,           /* [in] rectangle height in pixels          */
    int       nbr_dx,       /* [in] width of neighbouring pixel rect    */
    int       nbr_dy,       /* [in] height of neighbouring pixel rect   */
    int       x_step,       /* [in] horizontal skip step (1=all)        */
    int       y_step,       /* [in] vertical skip step (1=all)          */
    unsigned char dark_clr,     /* [in] dark binarization color         */
    unsigned char light_clr );  /* [in] light binarization color        */

int ip_contrast (                                       /* IP_CONTR.C   */
    image  *img );          /* [in] VC image                            */

int ip_img_mean (                                       /* IP_MEAN.C    */
    image  *img );          /* [in] source gray-level image             */

int ip_get_subpixel (                                   /* IP_GSPIX.C   */
    image  *img,            /* [in] image variable                      */
    float   x,              /* [in] pixel floating-point x-coordinate   */
    float   y );            /* [in] pixel floating-point y-coordinate   */

int ip_read_pixel (                                     /* IP_RPIX.C    */
    image  *img,            /* [in] image                               */
    int     x,              /* [in] pixel x-coord                       */
    int     y );            /* [in] pixel y-coord                       */
int ip_read_subpixel (
    image  *img,            /* [in] image                               */
    int     x,              /* [in] scaled integer pixel x-coord (*1000)*/
    int     y );            /* [in] scaled integer pixel y-coord (*1000)*/

int  ip_read_pixel_line (                               /* IP_RPIXL.C   */
    VD_IMAGE *img,          /* [in] gray-level image to read line from  */
    void     *line,         /* [in] line buffer (x1,y1) (x2,y2)         */
    int       arg_type,     /* [in] type of input line buffer           */
    float     pix_step,     /* [in] pixel step used to read line pixels */
    int       sub_pix,      /* [in] sub-pixel mode                      */
    int       res_type,     /* [in] type of result pixel coordinates    */
    int       res_size,     /* [in] size of output buffers in elements  */
    void     *res_xy,       /* [out] buffer with pixel coordinates      */
    int      *res_val,      /* [out] buffer with pixel values [0,255]   */
    int      *res_cnt );    /* [out] number of stored elements          */

int  ip_read_pixel_stripe (                             /* IP_RPIXS.C   */
    VD_IMAGE *vd_img,       /* [in] gray-level image to read line from  */
    DR_IMAGE *dr_img,       /* [in] drawing image (NULL=skip drawing)   */
    void     *line,         /* [in] line buffer (x1,y1) (x2,y2)         */
    int       arg_type,     /* [in] type of input line buffer           */
    int       stripe_wid,   /* [in] stripe width in pixels              */
    int       stripe_lcnt,  /* [in] stripe line count                   */
    float     pix_step,     /* [in] pixel step used to read line pixels */
    int       sub_pix,      /* [in] sub-pixel mode                      */
    int       res_type,     /* [in] type of result pixel coordinates    */
    int       res_size,     /* [in] size of output buffers in elements  */
    void     *res_xy,       /* [out] buffer with pixel coordinates      */
    int      *res_val,      /* [out] buffer with pixel values [0,255]   */
    int      *res_cnt,      /* [out] number of stored elements          */
    int       dr_mode,      /* [in] drawing mode                        */
    int      *dr_clrs );    /* [in] buffer with drawing colors          */

int  ip_read_rect_lines (                               /* IP_RPIXR.C   */
    VD_IMAGE *vd_img,       /* [in] video image with gray-level picture */
    DR_IMAGE *dr_img,       /* [in] drawing image (NULL=skip drawing)   */
    float     rect[8],      /* [in] x/y buffer with rectangle corners   */
    int       line_cnt,     /* [in] number of scan lines                */
//    int      *stripe_ctl,   /* [in] stripe control buffer               */
    int       stripe_wid,   /* [in] stripe width in pixels              */
    int       stripe_lcnt,  /* [in] stripe line count                   */
    float     pix_step,     /* [in] pixel step on each scan line        */
    int       sub_pix,      /* [in] sub-pixel mode                      */
    int       scan_dir,     /* [in] scan direction on each line         */
    int       res_type,     /* [in] type of result pixel coordinates    */
    int       res_size,     /* [in] max size of result buffers          */
    void     *res_xy,       /* [out] buffer with pixel coordinates      */
    int      *res_val,      /* [out] buffer with pixel values           */
    int      *res_cnt_buf,  /* [out] buffer with # of results per line  */
    int      *res_cnt,      /* [out] total number of results            */
    int       dr_mode,      /* [in] drawing mode                        */
    int      *dr_clrs );    /* [in] buffer with drawing colors          */
int  ip_read_pixel_rect (
    VD_IMAGE *vd_img,       /* [in] video image with gray-level picture */
    DR_IMAGE *dr_img,       /* [in] drawing image (NULL=skip drawing)   */
    int       x,            /* [in] x-coord. of rectangle point         */
    int       y,            /* [in] y-coord. of rectangle point         */
    int       dx,           /* [in] rectangle width in pixels           */
    int       dy,           /* [in] rectangle height in pixels          */
    int       pt_pos,       /* [in] point position: 0=cent, 1=top/left  */
    float     ang,          /* [in] rotation angle in radians [0,2PI]   */
    int       line_cnt,     /* [in] number of scan lines                */
//    int      *stripe_ctl,   /* [in] stripe control buffer               */
    int       stripe_wid,   /* [in] stripe width in pixels              */
    int       stripe_lcnt,  /* [in] stripe line count                   */
    float     pix_step,     /* [in] pixel step on each scan line        */
    int       sub_pix,      /* [in] sub-pixel mode                      */
    int       scan_dir,     /* [in] scan direction on each line         */
    int       res_type,     /* [in] type of result pixel coordinates    */
    int       res_size,     /* [in] max size of result buffers          */
    void     *res_xy,       /* [out] buffer with pixel coordinates      */
    int      *res_val,      /* [out] buffer with pixel values           */
    int      *res_cnt_buf,  /* [out] buffer with # of results per line  */
    int      *res_cnt,      /* [out] total number of results            */
    int       dr_mode,      /* [in] drawing mode                        */
    int      *dr_clrs );    /* [in] buffer with drawing colors          */

int ip_sphere_to_plain (                                /* IP_S2P.C     */
    image  *in_img,         /* [in] sphere image                        */
    int     x0,             /* [in] x-coordinate of sphere center       */
    int     y0,             /* [in] y-coordinate of sphere center       */
    int     rad,            /* [in] sphere radius                       */
    int     blank,          /* [in] blank pixel value                   */
    image  *out_img,        /* [out] plain image                        */
    int    *min_val,        /* [out] minimum sphere pixel value         */
    int    *max_val );      /* [out] maximum sphere pixel value         */

int ip_minma (                                          /* IP_MINMA.C   */
    VD_IMAGE *vd_img,       /* [in] video image                         */
    int     x,              /* [in] x-coord. of top/left rect point     */
    int     y,              /* [in] y-coord. of top/left rect point     */
    int     dx,             /* [in] rectangle width in pixels           */
    int     dy,             /* [in] rectangle height in pixels          */
    int     x_step,         /* [in] horizontal step to skip pixels      */
    int     y_step,         /* [in] vertical step to skip pixels        */
    int    *min_val,        /* [out] min pixel brightness (NULL: skip)  */
    int    *max_val,        /* [out] max pixel brightness (NULL: skip)  */
    int    *hist_buf );     /* [out] histogram buffer     (NULL: skip)  */

int  ip_img_scale (                                     /* IP_ISCAL.C   */
    VD_IMAGE *in_img,       /* [in] source image to scale               */
    VD_IMAGE *out_img,      /* [out] destination scaled image           */
    int       dx,           /* [in] width of output scaled image        */
    int       dy,           /* [in] height of output scaled image       */
    int       bw_mode,      /* [in] B&W image mode                      */
    int       bw_clr1,      /* [in] B&W image color 1                   */
    int       bw_clr2,      /* [in] B&W image color 2                   */
    int       calc_mode );  /* [in] calculation mode: 0=int, 1=float    */

int  ip_img_rotate (                                    /* IP_IMROT.C   */
    VD_IMAGE *in_img,       /* [in] source image to rotate              */
    VD_IMAGE *out_img,      /* [out] destination rotated image          */
    float     ang,          /* [in] rotation angle in radians           */
    int       pix_fill,     /* [in] pixel value to fill                 */
    int       bw_mode,      /* [in] B&W image mode                      */
    int       bw_clr1,      /* [in] B&W image color 1                   */
    int       bw_clr2 );    /* [in] B&W image color 2                   */
int  ip_rot_image_size (
    int       dx,           /* [in] width of source image               */
    int       dy,           /* [in] height of source image              */
    float     ang,          /* [in] rotation angle in radians           */
    int      *rot_dx,       /* [out] width of rotated image             */
    int      *rot_dy );     /* [out] height of rotated image            */

int ip_img_bwcorr (                                     /* IP_IMCOR.C   */
    VD_IMAGE *img1,         /* [in] B&W image                           */
    VD_IMAGE *img2,         /* [in] B&W image                           */
    VD_IMAGE *mask_img,     /* [in] mask image (NULL: no mask)          */
    unsigned int *cor,      /* [out] correlation value                  */
    unsigned int *area );   /* [out] image area (max correlation value) */

int ip_line_segm (                                      /* IP_LSEGM.C   */
    float     x1,           /* [in] x-coord. of beginning line point    */
    float     y1,           /* [in] y-coord. of beginning line point    */
    float     x2,           /* [in] x-coord. of end line point          */
    float     y2,           /* [in] y-coord. of end line point          */
    int       pt_cnt,       /* [in]  number of line segment points      */
    int     **res_xy,       /* [out] buffer with pixel coordinates      */
    float    *line_len,     /* [out] line length (NULL: don't return)   */
    float    *seg_len );    /* [out] segment length (NULL: don't return)*/

int ip_rgb_to_hsv_convert (                             /* IP_R2H.C     */
    image  *img,            /* [i/o] image                              */
    int     x,              /* [in] x-coord. of top/left rect. corner   */
    int     y,              /* [in] y-coord. of top/left rect. corner   */
    int     dx,             /* [in] rectangle width in pixels           */
    int     dy,             /* [in] rectangle height in pixels          */
    int     prec,           /* [in] calculation precision               */
    unsigned int hor_step,  /* [in] horizontal step (>=1)               */
    unsigned int ver_step,  /* [in] vertical step (>=1)                 */
    int     mode,           /* [in] conversion mode                     */
    int    *res );          /* [out] result buffer                      */

int ip_edge_det_th (                                    /* IP_EDGET.C   */
    int    *data_buf,   /* [in] source data buffer (pixel values)       */
    int     data_cnt,   /* [in] number of data_buf elements             */
    int     mode,       /* [in] operation mode: bit 0=circular          */
    int     edge_dir,   /* [in] edge direction: 0=L->D, 1=D->L, 2=both  */
    int     edge_type,  /* [in] edge type: 0-3=1st,last,strongest,all   */
    int     edge_th,    /* [in] edge-detection threshold                */
    int     res_type,   /* [in] type of values in res_pos               */
    int     res_size,   /* [in] size of res_pos[] and res_val[]         */
    void   *res_pos,    /* [out] buffer with edge position results      */
    int    *res_val,    /* [out] buffer with edge strength results      */
    int    *res_cnt );  /* [out] number of stored results               */

int ip_edge_det_grad (                                  /* IP_EDGEG.C   */
    int    *data_buf,   /* [in] source data buffer (pixel values)       */
    int     data_cnt,   /* [in] number of data_buf elements             */
    int     mode,       /* [in] operation mode: bit 0=circular          */
    int     edge_dir,   /* [in] edge direction: 0=L->D, 1=D->L, 2=both  */
    int     edge_type,  /* [in] edge type: 0-3=1st,last,strongest,all   */
    int     edge_th,    /* [in] edge-detection gradient threshold       */
    int     grad_size,  /* [in] gradient size                           */
    int     grad_dist,  /* [in] gradient distance                       */
    int     res_type,   /* [in] type of values in res_pos               */
    int     res_size,   /* [in] size of res_pos[] and res_val[]         */
    void   *res_pos,    /* [out] buffer with edge position results      */
    int    *res_val,    /* [out] buffer with edge strength results      */
    int    *res_cnt );  /* [out] number of stored results               */

int ip_calc_edge_coord (                                /* IP_EDGEC.C   */
    void   *pix_xy,     /* [in] source buffer with x/y pixel coordinates*/
    int     xy_type,    /* [in] type of values in pix_xy                */
    int     pix_cnt,    /* [in] number of pix_xy elements (points x,y)  */
    void   *edge_pos,   /* [in] source buffer with edge positions       */
    int     pos_type,   /* [in] type of values in edge_pos              */
    int     edge_cnt,   /* [in] number of edge_pos elements             */
    void   *res_xy,     /* [out] destination buffer with x/y edge coord.*/
    int     res_type ); /* [in] type of result values in res_xy         */

int ip_3x3f (                                           /* ip_3x3f.c    */
    image  *a,              /* [in] source image                        */
    image  *b,              /* [out] destination image                  */
//    void   (*func)() );     /* [in] basic filter function               */
    void   (*func)(unsigned char *,unsigned char *,unsigned char *, unsigned char *, int));

int ip_3x3fm (                                          /* ip_3x3fm.c   */
    image  *a,              /* [in] source image                        */
    image  *b,              /* [out] destination image                  */
//    void   (*func)(),       /* [in] basic filter function               */
    void   (*func)(unsigned char *,unsigned char *,unsigned char *, unsigned char *, int, int),
    int     method );       /* [in] method or other similar argument    */

int ip_3x3(                                             /* ip_3x3.c     */
    image  *a,              /* [in] source image                        */
    image  *b,              /* [out] destination image                  */
    int     c[3][3],        /* [in] 3x3 matrix with filter coefficients */
    int     sh,             /* [in] shift value: <0:right, >0:left      */
    int     mode,           /* [in] operation mode: bit0=abs            */
    int     fill );         /* [in] fill value: -1:leave, 0-255:fill    */

int ip_5x5(                                             /* ip_5x5.c     */
    image  *a,              /* [in] source image                        */
    image  *b,              /* [out] destination image                  */
    int     c[5][5],        /* [in] 5x5 matrix with filter coefficients */
    int     sh,             /* [in] shift value: <0:right, >0:left      */
    int     mode,           /* [in] operation mode: bit0=abs            */
    int     fill );         /* [in] fill value: -1:leave, 0-255:fill    */

int ip_nxn(                                             /* ip_nxn.c     */
    image  *a,              /* [in] source image                        */
    image  *b,              /* [out] destination image                  */
    int    *c,              /* [in] NxN matrix with filter coefficients */
    int     n,              /* [in] filter size N = 3,5,7,9,11          */
    int     sh,             /* [in] shift value: <0:right, >0:left      */
    int     mode,           /* [in] operation mode: bit0=abs            */
    int     fill );         /* [in] fill value: -1:leave, 0-255:fill    */

int ip_sobel (                                          /* ip_sobel.c   */
    VD_IMAGE *a,            /* [in] source image                        */
    VD_IMAGE *b,            /* [out] destination image                  */
    int       algo );       /* [in] algorithm: 0=VC, 1=fast img62x.lib  */

int ip_sobelm (                                         /* ip_sobm.c    */
    VD_IMAGE *a,            /* [in] source image                        */
    VD_IMAGE *b,            /* [out] destination image                  */
    int       algo,         /* [in] algorithm: 0=VC, 1=fast img62x.lib  */
    int      *meanv,        /* [out] mean pixel value in the result img */
    int      *maxv );       /* [out] max pixel value in the result img  */

int ip_eros (                                           /* ip_eros.c    */
    VD_IMAGE *img,          /* [i/o] binary image                      */
    int     x,              /* [in] x-coord. of top/left rect. corner  */
    int     y,              /* [in] y-coord. of top/left rect. corner  */
    int     dx,             /* [in] rectangle width in pixels          */
    int     dy,             /* [in] rectangle height in pixels         */
    unsigned char *sbuf,    /* [in] structuring element buffer         */
    int     ssize,          /* [in] size of structuring element        */
    int     oper );         /* [in] operation (0=erosion, 1=dilation)  */

int  ip_skel (                                          /* ip_skel.c    */
    image  *in_img,         /* [in] source binary (B%W) image           */
    image  *out_img,        /* [out] destination skeleton image         */
    int     min_line_len,   /* [in] minimum line length in pixels, def=3*/
    int     max_iter_cnt,   /* [in] max number of skeleton iterations   */
    int     op_mode,        /* [in] operation mode                      */
    int    *iter_cnt );     /* [out] act # of skeleton generation passes*/
int  ip_skel_pts (
    image  *img,            /* [i/o] skeleton image                     */
    int    *pt_buf,         /* [out] destination point buffer           */
    int     buf_size,       /* [in] max pt_buf size in points           */
    int    *cnt );          /* [out] number of detected points          */

int  ip_bin_aver (                                      /* ip_bina.c    */
    image  *in_img,         /* [in] source gray-level image             */
    image  *out_img,        /* [out] destination binary image           */
    int     th_dx,          /* [in] width of threshold calc. rectangle  */
    int     th_dy,          /* [in] height of threshold calc. rectangle */
    int     perc,           /* [in] dynamic threshold correction in %   */
    int     dark_clr,       /* [in] dark binarization color             */
    int     light_clr );    /* [in] light binarization color            */

int  ip_bin_minmax (                                    /* ip_binm.c    */
    image  *in_img,         /* [in] source gray-level image             */
    image  *out_img,        /* [out] destination binary image           */
    int     th_dx,          /* [in] width of threshold calc. rectangle  */
    int     th_dy,          /* [in] height of threshold calc. rectangle */
    int     perc,           /* [in] dynamic threshold correction in %   */
    int     dark_clr,       /* [in] dark binarization color             */
    int     light_clr );    /* [in] light binarization color            */

int ip_read_img_row (                                   /* ip_rrow.c    */
    image  *img,        /* [in] source image                            */
    int     x,          /* [in] start x-coord. of image row             */
    int     y,          /* [in] start y-coord. of image row             */
    int     cnt,        /* [in] number of pixels to read                */
    int    *buf );      /* [out] 32-bit integer buffer with pixel values*/

int ip_write_img_row (                                  /* ip_wrow.c    */
    image  *img,        /* [out] destination image                      */
    int     x,          /* [in] start x-coord. of image row             */
    int     y,          /* [in] start y-coord. of image row             */
    int     cnt,        /* [in] number of pixels to write               */
    int    *buf,        /* [in] 32-bit integer buffer with pixel values */
    int     sh,         /* [in] shift value: <0:right, >0:left          */
    int     abs_mode ); /* [in] abs operation mode: 0=off, 1=on         */

int ip_read_img_col (                                   /* ip_rcol.c    */
    image  *img,        /* [in] source image                            */
    int     x,          /* [in] start x-coord. of image column          */
    int     y,          /* [in] start y-coord. of image column          */
    int     cnt,        /* [in] number of pixels to read                */
    int    *buf );      /* [out] 32-bit integer buffer with pixel values*/

int ip_edge_2d (                                        /* ip_edge2.c   */
    image  *vd_img,         /* [in] gray-level video image              */
    image  *edg_img,        /* [out] edge image                         */
    int     edge_th,        /* [in] edge-detection threshold            */
    int     grad_size,      /* [in] gradient size, used in method 1     */
    int     grad_dist,      /* [in] gradient distance, used in method 1 */
    int     method,         /* [in] edge-detection method: 0-1=th,grad  */
    int     mode,           /* [in] edge-detection mode: bit0-1=hor,ver */
    int     ld_edge_pix,    /* [in] L->D edge pixel value               */
    int     dl_edge_pix,    /* [in] D->L edge pixel value               */
    int    *edge_cnt );     /* [out] total number detected edges        */

int ip_proj (                                           /* ip_proj.c    */
    image  *img,            /* [in] source image                        */
    int     proj_mode,      /* [in] projection mode: 0=hor, 1=ver       */
    int     bw_mode,        /* [in] B&W operation mode: 0=gray, 1=B&W   */
    int     bgnd_clr,       /* [in] background image color (B&W mode)   */
    int    *res_buf );      /* [out] result projection buffer           */

int ip_contour_dist (                                   /* ip_contd.c   */
    int  *xy_buf1,          /* [in] contour 1 x/y buffer                */
    int   cnt1,             /* [in] # of xy_buf1 points (x,y pairs)     */
    int  *xy_buf2,          /* [in] contour 2 x/y buffer                */
    int   cnt2,             /* [in] # of xy_buf2 points (x,y pairs)     */
    int   scan_reg,         /* [in] scan region (>=1)                   */
    int  *min_dist,         /* [out] min contour distance (NULL=off)    */
    int  *min_id1,          /* [out] min xy_buf1 index    (NULL=off)    */
    int  *min_id2,          /* [out] min xy_buf2 index    (NULL=off)    */
    int  *max_dist,         /* [out] max contour distance (NULL=off)    */
    int  *max_id1,          /* [out] max xy_buf1 index    (NULL=off)    */
    int  *max_id2 );        /* [out] max xy_buf2 index    (NULL=off)    */

void ip_find_obj_rect (                                 /* ip_fobr.c    */
    VD_IMAGE *img,          /* [in] mage                                */
    int       bgnd_clr,     /* [in] background pixel color              */
    VM_BOX   *obj_rect );   /* [out] object enclosing rectangle         */

int ip_perc_threshold (                                 /* IP_PTH.C     */
    VD_IMAGE *vd_img,       /* [i/o] video image                        */
    DR_IMAGE *dr_img,       /* [i/o] drawing image                      */
    int     x,              /* [in] x-coord. of rectangle point         */
    int     y,              /* [in] y-coord. of rectangle point         */
    int     dx,             /* [in] rectangle width in pixels           */
    int     dy,             /* [in] rectangle height in pixels          */
    int     pt_pos,         /* [in] point position: 0=cent, 1=top/left  */
    int     x_step,         /* [in] horizontal step to skip pixels      */
    int     y_step,         /* [in] vertical step to skip pixels        */
    int     perc,           /* [in] percentage value [0,100]            */
    int     mode,           /* [in] tool operation mode                 */
    int     dr_mode,        /* [in] drawing mode                        */
    int     dr_clr,         /* [in] drawing color on success (0=def)    */
    int     err_clr,        /* [in] drawing color on error   (0=def)    */
    float  *intensity,      /* [out] calculated intensity (I)           */
    float  *average,        /* [out] average (mean) pixel brightness (A)*/
    int    *min_val,        /* [out] min pixel brightness               */
    int    *max_val );      /* [out] max pixel brightness               */

int ip_edge (                                           /* IP_EDGE.C    */
    VD_IMAGE *vd_img,       /* [in] video image with gray-level picture */
    DR_IMAGE *dr_img,       /* [i/o] drawing image                      */
    int       x,            /* [in] x-coord. of rectangle point         */
    int       y,            /* [in] y-coord. of rectangle point         */
    int       dx,           /* [in] rectangle width in pixels           */
    int       dy,           /* [in] rectangle height in pixels          */
    int       pt_pos,       /* [in] point position: 0=cent, 1=top/left  */
    float     ang,          /* [in] rotation angle in radians [0,2PI]   */
    int       line_cnt,     /* [in] number of scan lines                */
    int       stripe_wid,   /* [in] stripe width in pixels              */
    int       stripe_lcnt,  /* [in] stripe line count                   */
    float     pix_step,     /* [in] pixel step on each scan line        */
    int       sub_pix,      /* [in] sub-pixel mode                      */
    int       scan_dir,     /* [in] scan direction on each line         */
    int       edge_dir,     /* [in] edge direction: 0-2=L->D,D->L,both  */
    int       edge_type,    /* [in] edge type: 0-3=1st,last,strong,all  */
    int       edge_th,      /* [in] edge-detection threshold            */
    int       grad_size,    /* [in] gradient size, used in method 1     */
    int       grad_dist,    /* [in] gradient distance, used in method 1 */
    int       method,       /* [in] edge-detection method: 0-1=th,grad  */
    int       res_mode,     /* [in] result mode: 0-1=normal,dummy edges */
    int       res_type,     /* [in] type of result edge coordinates     */
    int       res_size,     /* [in] max size of result buffers          */
    void     *res_xy,       /* [out] buffer with detected edge points   */
    int      *res_val,      /* [out] buffer with edge strengths         */
    int      *res_cnt_buf,  /* [out] buffer with # of results per line  */
    int      *res_cnt,      /* [out] total number of results            */
    int       dr_mode,      /* [in] drawing mode                        */
    int       dr_clr,       /* [in] drawing color on success            */
    int       err_clr );    /* [in] drawing color on error              */

#ifdef __cplusplus
}
#endif

#endif /* ]]] _IP_LIB_H_LOADED */
/*@}*/
