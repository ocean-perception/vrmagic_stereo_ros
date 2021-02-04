/*----------------------------------------------------------------------*/
/**
* @name BL_LIB.H
*       BLOB package
*
* <br>  VM_LIB is subject to the license and usage terms of VRmagic GmbH,
* <br>  which are distributed with this software.
* <br>
* <br>  Header file for the BLOB library.
*
* @version
*       Created:  10.05.2005
* <br>  Modified:
*/
/*----------------------------------------------------------------------*/
/*@{*/

#ifndef _BL_LIB_H     /* [[[ */
#define _BL_LIB_H

#include "cdef.h"       /* common definitions and memory allocation     */

/*............. Version */
#define BL_VERS        101          /* 100 for vers. 1.00   */
#define BL_VERS_STR    "1.01"       /* version string       */

/*--------------------------------------------------------------------------*/
/*                               Error codes                                */
/*--------------------------------------------------------------------------*/
#define BLOB_R_NO_MEMORY       9100 /* Memory allocation error              */
#define BLOB_R_IMG_OVF         9101 /* Image buffer overflow                */
#define BLOB_R_RLCBUF_OVF      9102 /* RLC buffer overflow                  */
#define BLOB_R_BINIMG_ERR      9103 /* Invalid format of binarized image    */
#define BLOB_R_MBUF_OVF        9104 /* Object merge buffer overflow         */
#define BLOB_R_OBJLABEL_ERR    9105 /* Object labelling error               */
#define BLOB_R_ODB_OVF         9106 /* Object database (ODB) buffer overflow*/
#define BLOB_R_INTERNAL_ERR    9107 /* Internal program error               */
#define BLOB_R_INV_OBJ_IDX     9108 /* Invalid object index                 */
#define BLOB_R_TRACE_ERR       9109 /* Contour tracing error                */
#define BLOB_R_CONTBUF_OVF     9110 /* Contour buffer overflow              */
#define BLOB_R_INV_FEATURE     9111 /* Invalid object feature               */
#define BLOB_R_DELETED_OBJ     9112 /* Deleted object                       */
#define BLOB_R_FEATURE_NOCALC  9113 /* Prerequisite feature not calculated  */
#define BLOB_R_INVALID_ARG     9114 /* Invalid input argument               */
#define BLOB_R_DRAW_ERR        9115 /* Feature drawing error                */
#define BLOB_R_NO_ORIENT       9116 /* Insufficient orientation data        */
#define BLOB_R_COPY_PROT       9117 /* Copy protection error                */
#define BLOB_R_INVALID_ODB     9118 /* Invalid ODB buffer                   */
#define BLOB_R_CONTOUR_LIMIT   9119 /* Contour limit exceeded               */
#define BLOB_R_NO_RLC_MEMORY   9120 /* RLC buffer allocation error          */

/*--------------------------------------------------------------------------*/
/*                                 Macros                                   */
/*--------------------------------------------------------------------------*/

/*............. BLOB handle */
#if !defined(_BLOB_HND_DEFINED)
  typedef void * BLOB_HND;
  #define _BLOB_HND_DEFINED
#endif

#if !defined(FALSE) || !defined(TRUE)
  #define FALSE 0
  #define TRUE  1
  typedef int BOOL;
#endif

#ifndef NULL
  #define NULL  ((void *)0)
#endif


/*............. Memory allocation */
#ifndef cam_alloc   /* [[[ */
#ifdef _TI_CAMERA           /* TI camera    */
  #define cam_alloc(size)     vcmalloc(((size)+3)/4)  /* size in bytes  */
  #define cam_alloc_int(size) vcmalloc(size)          /* size in ints   */
  #define cam_free            vcfree
#else                       /* PC           */
  #define cam_alloc(size)     malloc(size)
  #define cam_alloc_int(size) malloc((size)*sizeof(int))
  #define cam_free            free
#endif
#endif      /* ]]] */


#ifndef GETCH
 #ifndef _TI_CAMERA
   #ifndef _MSC_VER
     #define GETCH getch
   #else
     #define GETCH _getch
   #endif
 #else
   #define GETCH rs232rcv
 #endif
#endif

/*............. Object filtering in bl_get_objects() */
/* Bits for the "filt_mask" parameter */
#define BLOB_OBJ_ALL           0  /* include all objects types              */
#define BLOB_OBJ_LEFT     0x0001  /* exclude left boundary objects          */
#define BLOB_OBJ_RIGHT    0x0002  /* exclude right boundary objects         */
#define BLOB_OBJ_TOP      0x0004  /* exclude top boundary objects           */
#define BLOB_OBJ_BOT      0x0008  /* exclude bottom boundary objects        */
#define BLOB_OBJ_INTERNAL 0x0010  /* exclude internal (non-boundary) objects*/

/*............. Object feature drawing in bl_draw_features() */
/* Each bit enables drawing of respective feature */
#define BL_DRAW_CENTER    0x0001  /* draw markers of mass centers           */
#define BL_DRAW_OBJ_NUM   0x0002  /* draw object numbers near mass centers  */
#define BL_DRAW_NONR_RECT 0x0004  /* draw non-rotated enclosing rectangles  */
#define BL_DRAW_CONTOUR   0x0008  /* draw contours                          */
#define BL_DRAW_ELLIPSE   0x0010  /* draw equivalent ellipses               */
#define BL_DRAW_MIN_RECT  0x0020  /* draw min area rotated rectangles       */

/*............. Various constants */
#ifndef ANG_3PI2
  #define ANG_PI   3.141592653589793f     /* PI       */
  #define ANG_PI2  (ANG_PI/2)             /* PI/2     */
  #define ANG_3PI2 (3*ANG_PI/2)           /* 3*PI/2   */
  #define ANG_2PI  (2*ANG_PI)             /* 2*PI     */
#endif

/*--------------------------------------------------------------------------*/
/*                                  Typedefs                                */
/*--------------------------------------------------------------------------*/

/*............. Basic types */
#ifndef BL_TYPES_DEFINED    /* [[[ */
 #define BL_TYPES_DEFINED

 typedef char           BL_CHAR;
 typedef short          BL_INT16;
 typedef int            BL_INT32;

 typedef unsigned char  BL_UCHAR;
 typedef unsigned short BL_UINT16;
 typedef unsigned int   BL_UINT32;
#endif  /* ]]] */

/*............. VC image on PC */
#if !defined(_TI_CAMERA)    /* [[[ - image is not defined on PC   */
  #ifndef IMAGE_DEFINED
    typedef struct
    {
        long st;            /* start byte address (long)    */
        int  dx;            /* horizontal width             */
        int  dy;            /* vertical width               */
        int  pitch;         /* memory pitch                 */
    } image;
    #define IMAGE_DEFINED
  #endif
#endif  /* ]]] */

/*............. ODB parameters */
typedef struct
{
    int       obj_cnt;      /* # of objects in ODB                          */
    BL_UINT32 size;         /* actual size of ODB buffer in bytes           */
    BL_UINT32 max_size;     /* max allocated size of ODB buffer in bytes    */
    int       img_dx;       /* image width in pixels                        */
    int       img_dy;       /* image height in pixels                       */
    int       x0;           /* screen x-coord. of top/left image corner     */
    int       y0;           /* screen y-coord. of top/left image corner     */
} ODB_PAR;

/*............. Object features */
typedef struct
{

/* Basic features calculated during the ODB generation */
                            /* Non-rotated enclosing rectangle:             */
    int     x_min;          /* minimum object x-coordinate                  */
    int     y_min;          /* minimum object y-coordinate                  */
    int     x_max;          /* maximum object x-coordinate                  */
    int     y_max;          /* maximum object y-coordinate                  */

    int     x_cont;         /* x-coord. of beginning contour point          */
    int     y_cont;         /* y-coord. of beginning contour point          */

                            /* Area and mass center:                        */
    BL_UINT32 area;         /* object area in # of pixels                   */
    float   x_cent;         /* x-coord. of mass center                      */
    float   y_cent;         /* y-coord. of mass center                      */
    float   avg_br;         /* average object brightness = sum(pix)/n       */

/* Contours */
    float   cont_len;       /* outer object contour length in pixels (0=NA) */
    int     hole_cnt;       /* # of object holes (>=0)              (-1=NA) */
    float   min_dist;       /* minimum  distance from the mass center to a  */
                            /* contour point                        (-1=NA) */
    float   max_dist;       /* maximum distance from the mass center to a   */
                            /* contour point                        (-1=NA) */

/* Equivalent ellipse (center = mass center) */
    float   major_rad;      /* greater radius (>=0)                 (-1=NA) */
    float   minor_rad;      /* smaller radius (>=0)                 (-1=NA) */
    float   ellip_angle;    /* rotation angle [0,2PI]               (-1=NA) */

/* Minimum area enclosing rectangle */
    BL_UINT32 area_convex;  /* convex hull area in # of pixels       (0=NA) */
    float   rect_dx;        /* width  = greater dimension (>=0)     (-1=NA) */
    float   rect_dy;        /* height = smaller dimension (>=0)     (-1=NA) */
    float   rect_angle;     /* rotation angle [0,2PI]               (-1=NA) */
    float   rect_pts[8];    /* rectangle corner points:                     */
                            /*   (x1,y1) (x2,y2) (x3,y3) (x4,y4)     (0=NA) */

/* Shape features */
    float   compactness;    /* object compactness:                   (0=NA) */
                            /*  1:perfect circle, >1:other shape            */
                            /* Formula = (cont_len*cont_len)/(4*PI*area)    */

    float   circularity;    /* object circularity:                   (0=NA) */
                            /*  1:perfect circle, <1:other shape            */
                            /* Formula = area/(Rmax*Rmax*PI)                */
                            /* Rmax = largest distance from the mass center */
                            /*           to a contour point                 */

    float   anisometry;     /* object anisometry (elongatedness):    (0=NA) */
                            /*  1:perfect circle, >1:other shape            */
                            /* Formula = major_radius/minor_radius          */

    float   bulkiness;      /* object bulkiness:                     (0=NA) */
                            /*  1:perfect circle/ellipse, >1:other shape    */
                            /* Formula = (PI*major_rad*minor_rad)/area      */

    float   convexity;      /* object convexity:                     (0=NA) */
                            /*   1: perfect convex shape                    */
                            /*  <1: blob with holes or with concave shape   */
                            /* Formula = area/area_convex                   */

/* Flags for executed calculation stages */
    int     cont_flg;       /* executed contour stage:   0=no, 1=yes        */
    int     ellip_flg;      /* executed ellipse stage:   0=no, 1=yes        */
    int     rect_flg;       /* executed rectangle stage: 0=no, 1=yes        */

} BL_FEATURE;


/*----------------------------------------------------------------------*/
/*                          Function prototypes                         */
/*----------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/*......................................................................*/
/*................ High-level universal BLOB functions .................*/
/*......................................................................*/

int vm_find_blobs (
    BLOB_HND *blob_hnd,     /* [out] BLOB handle                        */
    VD_IMAGE *vd_img,       /* [i/o] video (gray-level) image           */
    DR_IMAGE *dr_img,       /* [i/o] drawing (overlay) image            */
    int     x,              /* [in] x-coord. of rectangle point         */
    int     y,              /* [in] y-coord. of rectangle point         */
    int     dx,             /* [in] rectangle width in pixels           */
    int     dy,             /* [in] rectangle height in pixels          */
    int     pt_pos,         /* [in] point position: 0=cent, 1=top/left  */
    int     lo_th,          /* [in] lower binarization threshold        */
    int     up_th,          /* [in] upper binarization threshold        */
    int     bgnd_clr,       /* [in] background binarization color       */
    int     obj_clr,        /* [in] object binarization color           */
    int     destr_mode,     /* [in] image destructive mode              */
    int     con_mode,       /* [in] object connection mode              */
    int     min_area,       /* [in] min object area for object filtering*/
    int     max_area,       /* [in] max object area for object filtering*/
    int     filt_mask,      /* [in] object filter mask                  */
    int     odb_size,       /* [in] max handle ODB buf size (0=auto)    */
    int     disp_mode,      /* [in] feature display mode (0=no display) */
    int     dr_mode,        /* [in] drawing mode                        */
    int     dr_clr,         /* [in] drawing color on success            */
    int     err_clr );      /* [in] drawing color on error              */
int vm_blobs (
    BLOB_HND *blob_hnd,     /* [out] BLOB handle                        */
    VD_IMAGE *bin_img,      /* [in] binary image (bgnd color = bgnd_clr)*/
    DR_IMAGE *dr_img,       /* [i/o] drawing (overlay) image            */
    int     x,              /* [in] x-coord. of rectangle point         */
    int     y,              /* [in] y-coord. of rectangle point         */
    int     dx,             /* [in] rectangle width in pixels           */
    int     dy,             /* [in] rectangle height in pixels          */
    int     pt_pos,         /* [in] point position: 0=cent, 1=top/left  */
    int     bgnd_clr,       /* [in] background binarization color       */
    int     con_mode,       /* [in] object connection mode              */
    int     min_area,       /* [in] min object area for object filtering*/
    int     max_area,       /* [in] max object area for object filtering*/
    int     filt_mask,      /* [in] object filter mask                  */
    int     odb_size,       /* [in] max handle ODB buf size (0=auto)    */
    int     disp_mode,      /* [in] feature display mode (0=no display) */
    int     dr_mode,        /* [in] drawing mode                        */
    int     dr_clr,         /* [in] drawing color on success (0=def)    */
    int     err_clr );      /* [in] drawing color on error   (0=def)    */
void vm_blob_close (
    BLOB_HND *blob_hnd );   /* [in/out] BLOB handle                     */
int  vm_get_blob_info (
    BLOB_HND  hnd,          /* [in] BLOB handle                         */
    int      *obj_cnt,      /* [out] number of objects in the handle    */
    int      *odb_size,     /* [out] actual size of ODB buffer in bytes */
    int      *max_odb_size, /* [out] max allocated size of ODB buffer   */
    int      *img_dx,       /* [out] width of initial video image       */
    int      *img_dy );     /* [out] height of initial video image      */
int vm_get_blob_features (
    BLOB_HND  hnd,          /* [in] BLOB handle                         */
    DR_IMAGE *dr_img,       /* [i/o] drawing (overlay) image            */
    unsigned int obj_id,    /* [in] object index                        */
    int     cont_limit,     /* [in] contour length limit (0=don't check)*/
    int     calc_contour,   /* [in] calculate contour features: 0/1=N/Y */
    int     calc_ellipse,   /* [in] calculate ellipse features: 0/1=N/Y */
    int     calc_rect,      /* [in] calculate rect. features:   0/1=N/Y */
    int     disp_mode,      /* [in] feature display mode (0=no display) */
    int     dr_mode,        /* [in] drawing mode                        */
    int     dr_clr,         /* [in] drawing color on success            */
    int     err_clr,        /* [in] drawing color on error              */
    BL_FEATURE *obj_ftr );  /* [out] struct with BLOB features          */
int vm_draw_blob_features (
    BLOB_HND  hnd,          /* [in] BLOB handle                         */
    DR_IMAGE *dr_img,       /* [i/o] drawing (overlay) image            */
    int      *obj_list,     /* [in] object list buffer (NULL:draw all)  */
    int       list_size,    /* [in] obj_list size                       */
    int       dr_clr,       /* [in] drawing color                       */
    int       dr_mode );    /* [in] drawing mode                        */
int vm_get_blob_contour (
    BLOB_HND  hnd,          /* [in] BLOB handle                         */
    DR_IMAGE *dr_img,       /* [i/o] drawing (overlay) image            */
    unsigned int obj_id,    /* [in] object index                        */
    int       cont_limit,   /* [in] contour length limit (0=don't check)*/
    int       dr_mode,      /* [in] drawing mode                        */
    int       dr_clr,       /* [in] drawing color on success            */
    int       err_clr,      /* [in] drawing color on error              */
    int     **xy_buf,       /* [out] buffer with (x,y) coordinates      */
    int      *cont_cnt );   /* [out] number of object contours          */
int vm_get_blob_convex (
    BLOB_HND  hnd,          /* [in] BLOB handle                         */
    DR_IMAGE *dr_img,       /* [i/o] drawing (overlay) image            */
    unsigned int obj_id,    /* [in] object index                        */
    int       cont_limit,   /* [in] contour length limit (0=don't check)*/
    int       dr_mode,      /* [in] drawing mode                        */
    int       dr_clr,       /* [in] drawing color on success            */
    int       err_clr,      /* [in] drawing color on error              */
    int     **cvx_buf,      /* [out] convex point buffer (x,y), (x,y),. */
    int      *cvx_cnt );    /* [out] number of convex points            */
int vm_get_convex_obj (
    BLOB_HND  blob_hnd,     /* [in] BLOB handle                         */
    image    *vd_img,       /* [in] gray-level video image              */
    image    *dr_img,       /* [i/o] drawing (overlay) image            */
    int       obj_id,       /* [in] BLOB object index                   */
    int       pix_fill,     /* [in] pixel value to fill outside convex  */
    int       dr_mode,      /* [in] drawing mode                        */
    int       dr_clr,       /* [in] drawing color on success            */
    int       err_clr,      /* [in] drawing color on error              */
    image    *obj_img,      /* [out] object image                       */
    int      *min_val,      /* [out] minimum object pixel value         */
    int      *max_val );    /* [out] maximum object pixel value         */

/*......................................................................*/
/*........................ Basic BLOB functions ........................*/
/*......................................................................*/

/* Image binarization */
int bl_img_binar (
    image      *img,        /* [in] image in VC format                  */
    image      *img_bin,    /* [out] binarized image in VC format       */
    BL_UINT32   lo_th,      /* [in] lower binarization threshold        */
    BL_UINT32   up_th,      /* [in] upper binarization threshold        */
    int         bgnd_clr,   /* [in] background binarization color       */
    int         fgnd_clr ); /* [in] foreground binarization color       */

/* Object segmentation and ODB generation */
int bl_get_objects (
    image    *imgb,         /* [in] binarized image in VC format        */
    int       bgnd_clr,     /* [in] background color in imgb            */
    char     *odb_buf,      /* [out] object database buffer             */
    BL_UINT32 max_odb_size, /* [in] max size of odb_buf in bytes        */
    int       con_mode,     /* [in] connection mode (0=off, 1:8, 2:4)   */
    BL_UINT32 min_area,     /* [in] min object area                     */
    BL_UINT32 max_area,     /* [in] max object area                     */
    int       filt_mask );  /* [in] object filter mask                  */
int bl_get_objects1 (
    image    *imgb,         /* [in] binary image                        */
    int       obj_clr,      /* [in] object color in imgb                */
    char     *odb_buf,      /* [out] object database buffer             */
    BL_UINT32 max_odb_size, /* [in] max size of odb_buf in bytes        */
    int       con_mode,     /* [in] connection mode (0=off, 1:8, 2:4)   */
    BL_UINT32 min_area,     /* [in] min object area                     */
    BL_UINT32 max_area,     /* [in] max object area                     */
    int       filt_mask );  /* [in] object filter mask                  */
int bl_gen_odb (
    image    *img,          /* [in] gray-level image in VC format       */
    BL_UINT32 lo_th,        /* [in] lower binarization threshold        */
    BL_UINT32 up_th,        /* [in] upper binarization threshold        */
    int       bgnd_clr,     /* [in] background binarization color       */
    int       fgnd_clr,     /* [in] foreground binarization color       */
    int       destructive,  /* [in] destructive mode (0=off, 1:on)      */
    char     *odb_buf,      /* [out] object database buffer             */
    BL_UINT32 max_odb_size, /* [in] max size of odb_buf in bytes        */
    int       con_mode,     /* [in] connection mode (0=off, 1:8, 2:4)   */
    BL_UINT32 min_area,     /* [in] min object area                     */
    BL_UINT32 max_area,     /* [in] max object area                     */
    int       filt_mask );  /* [in] object filter mask                  */

/* Feature calculation */
int bl_get_contours (
    char      *odb_buf,     /* [i/o] object database buffer             */
    BL_UINT32 *obj_list,    /* [in] object list buffer                  */
    BL_UINT32  list_size,   /* [in] obj_list size                       */
    int        calc_mode,   /* [in] contour length calculation mode     */
    int        cont_limit );/* [in] contour limit                       */

int bl_get_ellipses (
    char      *odb_buf,     /* [i/o] object database buffer             */
    BL_UINT32 *obj_list,    /* [in] object list buffer                  */
    BL_UINT32  list_size ); /* [in] obj_list size                       */

int bl_get_rects (
    char      *odb_buf,     /* [i/o] object database buffer             */
    BL_UINT32 *obj_list,    /* [in] object list buffer                  */
    BL_UINT32  list_size ); /* [in] obj_list size                       */

/* Read calculated features */
void bl_read_odb_par (
    char       *odb_buf,    /* [in] object database buffer              */
    ODB_PAR    *odb_par );  /* [out] struct with object features        */

void bl_write_odb_par (
    char       *odb_buf,    /* [in] object database buffer              */
    ODB_PAR    *odb_par );  /* [in] struct with ODB parameters          */

int bl_read_obj_features (
    char       *odb_buf,    /* [in] object database buffer              */
    BL_UINT32   obj_id,     /* [in] object index                        */
    BL_FEATURE *obj_ftr );  /* [out] struct with object features        */

int bl_read_obj_contours (
    char       *odb_buf,    /* [in] object database buffer              */
    BL_UINT32   obj_id,     /* [in] object index                        */
    int       **xy_buf,     /* [out] buffer with (x,y) coordinates      */
    int        *cont_cnt ); /* [out] number of object contours          */

/* Other */
int  bl_approx_odb_size (
    char   *odb_buf );      /* [in] object database buffer              */

void bl_set_max_rlc_size (
    int max_rlc_size );     /* [in] max RLC buffer size (0=auto)        */

/* Drawing functions */
int  bl_draw_features (
    image     *dr_img,      /* [i/o] drawing image in VC format         */
    char      *odb_buf,     /* [i/o] object database buffer             */
    BL_UINT32 *obj_list,    /* [in] object list buffer                  */
    BL_UINT32  list_size,   /* [in] obj_list size                       */
    int        x0,          /* [in] x-coord. of drawing origin point    */
    int        y0,          /* [in] y-coord. of drawing origin point    */
    int        clr,         /* [in] drawing color                       */
    int        dr_mode );   /* [in] drawing mode                        */
int draw_line (
    image  *dr_img,         /* [i/o] drawing image in VC format         */
    int     x1,             /* [in] x-coord. of 1st line point          */
    int     y1,             /* [in] y-coord. of 1st line point          */
    int     x2,             /* [in] x-coord. of 2nd line point          */
    int     y2,             /* [in] y-coord. of 2nd line point          */
    int     clr );          /* [in] drawing color                       */
int draw_box (
    image  *dr_img,         /* [i/o] drawing image in VC format         */
    int     x,              /* [in] x-coord. of top/left box corner     */
    int     y,              /* [in] y-coord. of top/left box corner     */
    int     dx,             /* [in] box width in pixels                 */
    int     dy,             /* [in] box height in pixels                */
    int     clr );          /* [in] drawing color                       */
int draw_marker (
    image  *dr_img,         /* [in] draw image (VC format)              */
    int     x,              /* [in] x-coordinate of marker center       */
    int     y,              /* [in] y-coordinate of marker center       */
    int     size,           /* [in] marker size (from center to end)    */
    int     clr );          /* [in] marker color                        */
int draw_text (
    image  *dr_img,         /* [i/o] drawing image in VC format  */
    char   *str,            /* [in] string                              */
    int     x,              /* [in] x-coord. of top left matrix corner  */
    int     y,              /* [in] y-coord. of top left matrix corner  */
    int     clr,            /* [in] foreground pixel color              */
    int     bgnd,           /* [in] background color                    */
    int     mode );         /* [in] drawing mode                        */
void fill_img (
    image  *img,            /* [out] image                              */
    int     clr );          /* [in] color (fill constant)               */

/* Utilities */
unsigned long ut_get_time ();
void ut_img_copy (
    image  *img1,           /* [in] image                               */
    image  *img2 );         /* [out] image                              */

#ifdef __cplusplus
}
#endif

#endif      /* ]]] _BL_LIB_H */
