/*----------------------------------------------------------------------*/
/**
* @name OR_LIB.H
*       Object recognition library (ORLIB) header
*
* <br>  VM_LIB is subject to the license and usage terms of VRmagic GmbH,
* <br>  which are distributed with this software.
* <br>
* <br>  This file contains type definitions, macros and prototypes for
*       ORLIB user-interface functions.
*
* @version
*       Created: 02.08.2006
* <br>  Modified:
*/
/*----------------------------------------------------------------------*/

/*@{*/
#ifndef _OR_LIB_H
#define _OR_LIB_H


/****** OR_LIB.H = ORLIB user-interface header = 08.2005 ****************/
/*----------------------------------------------------------------------*/
/*                       Macro and type definitions                     */
/*----------------------------------------------------------------------*/

#define OR_VERS      436    /* library version (100 for 1.00)           */
#define OR_VERS_STR "4.36"  /* library version string                   */

/*............. Image definition */
#ifndef _TI_CAMERA   /* [[[ */
#ifndef IMAGE_DEFINED
#define IMAGE_DEFINED
   typedef struct {
     long st;           /* start address/offset */
     int dx;            /* horizontal line width, in pixels */
     int dy;            /* height, in pixels */
     int pitch;         /* (memory) line pitch, in pixels */
   } image;
#endif
#endif  /* ]]] */

/*............. Diagnostic dump */
#ifdef _TI_CAMERA   /* [[[ - TI camera */
  #define TRACE  printf
#else     /* ]]] [[[ - PC*/
  #if !defined(TRACE)
    #define TRACE  OutputDebugString
  #endif
#endif  /* ]]] */


/*----------------------------------------------------------------------*/
/*                               Error codes                            */
/*----------------------------------------------------------------------*/
#define OR_INV_PAT_RECT     100 /* Invalid pattern rectangle            */
#define OR_INV_EDG_IMG      101 /* Invalid edge image                   */
#define OR_INV_SEARCH_RECT  102 /* Invalid search rectangle             */
#define OR_COPY_PROT_ERR    103 /* Copy protection error                */


/*............. ORLIB handle */
typedef void * OR_HND;

/*............. or_learn(): Learn parameter structure */
typedef struct
{
    int   min_edge_h;       /* min edge height: 7,..,15,..,127              */
                            /*  default = 19                                */
    int   min_edge_g;       /* min edge gradient: 6,..,15,..,min_edge_h     */
                            /*  default = 16                                */
    int   min_chain;        /* min edge contour length: 4,..,24 pixels      */
                            /*  default = 8                                 */
    int   save_edge_image;  /* bool: !=0: save edge image at x0,y0 into     */
                            /* video-image, no further pattern processing   */
                            /*  default = 0                                 */
    int   lrn_mode;         /* learn mode:                                  */
                            /*  0: normal learn mode (edg_img ignored)      */
                            /*  1: learn & generate output chain image in   */
                            /*     edg_img                                  */
                            /*  2: learn, use input edg_img to mask the     */
                            /*     unwanted pattern contours                */
    int   dr_clr;           /* drawing color [0,255]                        */
} LEARN_PAR;

/*............. or_search(): Search parameter structure */
typedef struct
{
    int   min_edge_h;       /* min edge height: 7,..,15,..,127              */
                            /*  default = 19                                */
    int   min_edge_g;       /* min edge gradient, =6,..,15,..,min_edge_h    */
                            /*  default = 16                                */
    int   min_chain;        /* min edge contour length, =4,..,24 pixels     */
                            /*  default = 8                                 */
    int   restr_search;     /* 1 = restricted_searching_mode                */
                            /*  default = 0                                 */
    int   max_pm_items;     /* max allowed num of pattern matching items    */
                            /*  default = 1                                 */
    int   min_rate;         /* min matching rate, =160,..,1023, 1024==100%  */
                            /*  default = 552                               */
    int   contour_width;    /* min edge contour width, =3,5,7,9 pixels      */
                            /*  default = 5                                 */
    int   min_scale;        /* min scale, 512-1024(=100%), keep <=max_scale!*/
                            /*  default = 1024                              */
    int   max_scale;        /* max scale, 1024(=100%)-1536, keep <200% !    */
                            /*  default = 1024                              */
    int   rot_sect_start;   /* rotation sector start, -180..179 deg         */
                            /*  default = -5                                */
    int   rot_sect_width;   /* rotation sector width, 0..360 deg            */
                            /*  default = 360                               */
    int   save_edge_image;  /* bool: 1 = save edge image at x0,y0 into      */
                            /* video-image, no further search processing    */
                            /*  default = 0                                 */
    int   dr_clr;           /* drawing color [0,255]                        */
} SEARCH_PAR;

/*............. or_search(): Result structure */
/* The x/y coordinates are relatiive to the search image */
typedef struct
{
    int   x;                /* x-coord. of contour rotation center      */
    int   y;                /* y-coord. of contour rotation center      */
    int   rate;             /* recognition rate: 1024==100%             */
    int   rot;              /* rotation angle: -180..179 deg            */
    int   scale;            /* scale factor, 1024==100%                 */
    int   corners[8];       /* corner points of rotated rectangle:      */
                            /*  (x0,y0) (x1,y1) (x2,y2) (x3,y3)         */
} OR_RES;

/*
* perc(%) to val_1024 conversion:
*   val = (perc * 1024) / 100;
*/
#define PERC_TO_VAL1024(perc)  (((perc) * 1024) / 100)

/*
* val_1024 to perc(%) conversion:
*   val = (perc * 100) / 1024;
*/
#define VAL1024_TO_PERC(val)  (((val) * 100) / 1024)

/*----------------------------------------------------------------------*/
/*                           Function prototypes                        */
/*----------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

void or_init (
    OR_HND *or_hnd );       /* [out] ORLIB handle                       */

void or_exit (
    OR_HND *or_hnd );       /* [i/o] OR handle                          */

int  or_learn (
    OR_HND     *or_hnd,     /* [i/o] OR library handle                  */
    image      *pat_img,    /* [in] pattern image (video page)          */
    image      *ovl_img,    /* [in] drawing image (overlay page)        */
    image      *edg_img,    /* [i/o] edge-mask image                    */
    int         x,          /* [in] x-coord of top/left pattern corner  */
    int         y,          /* [in] y-coord of top/left pattern corner  */
    int         dx,         /* [in] pattern width in pixels             */
    int         dy,         /* [in] pattern height in pixels            */
    LEARN_PAR  *lpar );     /* [in] learn parameters (see OR_LIB.H)     */

int  or_search (
    OR_HND     *or_hnd,     /* [in] OR library handle                   */
    image      *srh_img,    /* [in] search image (video page)           */
    image      *ovl_img,    /* [in] drawing image (overlay page)        */
    int         x,          /* [in] x-coord of top/left search corner   */
    int         y,          /* [in] y-coord of top/left search corner   */
    int         dx,         /* [in] search area width in pixels         */
    int         dy,         /* [in] search area height in pixels        */
    SEARCH_PAR *spar,       /* [in] search parameters (see OR_LIB.H)    */
    OR_RES     *res_buf,    /* [out] result buffer                      */
    int        *nres );     /* [out] number of detected results         */

int  or_nsearch (
    OR_HND     *or_hnd,     /* [in] OR library handle                   */
    image      *srh_img,    /* [in] search image (video page)           */
    image      *ovl_img,    /* [in] drawing image (overlay page)        */
    int         x,          /* [in] x-coord of top/left search corner   */
    int         y,          /* [in] y-coord of top/left search corner   */
    int         dx,         /* [in] search area width in pixels         */
    int         dy,         /* [in] search area height in pixels        */
    SEARCH_PAR *spar,       /* [in] normalized search parameters        */
    OR_RES     *res_buf,    /* [out] buffer with normalized result      */
    int        *nres );     /* [out] number of detected results         */


/* Drawing functions: */
int or_draw_line (
    image  *dr_img,         /* [i/o] drawing image in VC format         */
    int     x1,             /* [in] x-coord. of 1st line point          */
    int     y1,             /* [in] y-coord. of 1st line point          */
    int     x2,             /* [in] x-coord. of 2nd line point          */
    int     y2,             /* [in] y-coord. of 2nd line point          */
    int     clr );          /* [in] drawing color                       */
int or_draw_marker (
    image  *dr_img,         /* [in] draw image (VC format)              */
    int     x,              /* [in] x-coordinate of marker center       */
    int     y,              /* [in] y-coordinate of marker center       */
    int     size,           /* [in] marker size (from center to end)    */
    int     clr );          /* [in] marker color                        */
int  or_draw_xmarker (
    image  *dr_img,         /* [in] draw image (VC format)              */
    int     x,              /* [in] x-coordinate of marker center       */
    int     y,              /* [in] y-coordinate of marker center       */
    int     size,           /* [in] marker size (from center to end)    */
    int     clr );          /* [in] marker color                        */
int or_draw_box (
    image  *dr_img,         /* [i/o] drawing image in VC format         */
    int     x,              /* [in] x-coord. of top/left box corner     */
    int     y,              /* [in] y-coord. of top/left box corner     */
    int     dx,             /* [in] box width in pixels                 */
    int     dy,             /* [in] box height in pixels                */
    int     clr );          /* [in] drawing color                       */

/* Other */
unsigned long or_get_time ();  /* get system time in ms, grows up       */
void ovl_init (
    int r,                  /* [in] red color component [0,255]         */
    int g,                  /* [in] green color component [0,255]       */
    int b );                /* [in] blue color component [0,255]        */
void ovl_set_palette (
    int     clr,            /* [in] color                               */
    int     r,              /* [in] red intensity                       */
    int     g,              /* [in] green intensity                     */
    int     b );            /* [in] blue intensity                      */

#ifdef __cplusplus
};
#endif

#endif  /* _OR_LIB_H */
/*@}*/
