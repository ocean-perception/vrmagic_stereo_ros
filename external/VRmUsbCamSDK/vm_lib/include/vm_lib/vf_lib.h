/*----------------------------------------------------------------------*/
/**
* @name VF_LIB.H
*       Vector function library header file
*
* <br>  VM_LIB is subject to the license and usage terms of VRmagic GmbH,
* <br>  which are distributed with this software.
* <br>
* <br>  This file contains type definitions, macros and function prototypes
*       for the fast vector library functions, optimized for best speed on
*       the TI camera. The VF_LIB library works on the PC as well.
*
* @version
*       Created: 25.10.2005
* <br>  Modified:
*/
/*----------------------------------------------------------------------*/
/*@{*/

#ifndef _VF_LIB_H     /* [[[ */
#define _VF_LIB_H

#include "cdef.h"

#undef VC_CAM
#undef VR_DSP_CAM
#if   _CAM_TYPE == _CAM_SIM      /* PC Simulator                         */
#elif _CAM_TYPE == _CAM_PC       /* PC stand-alone library               */
#elif _CAM_TYPE == _CAM_TI       /* VCxxxx TI-based camera               */
  #define VC_CAM
#elif _CAM_TYPE == _CAM_BF       /* Blackfin uClinux camera              */
#elif _CAM_TYPE == _CAM_ARM      /* ARM Linux camera (i.MX27 IP camera)  */
#elif _CAM_TYPE == _CAM_VR_ARM   /* VRmagic camera - ARM Linux           */
#elif _CAM_TYPE == _CAM_VR_DSP   /* VRmagic camera - DSP                 */
  #define VR_DSP_CAM
#else
  #error vf_lib.h: Camera type not supported
#endif


/* DDD */
#define DISABLE_VF_LIB_OPTIMIZATION   0   /* 1=disable TI speed optimization  */

/*----------------------------------------------------------------------*/
/*                                 Macros                               */
/*----------------------------------------------------------------------*/

/*............. VF_LIB compilation macros */
#if defined(_VC_CAM) || defined(VR_DSP_CAM)     /* TMS320C6x cameras */
  #define RESTRICT restrict
#else                       /* PC & other platforms */
  #define RESTRICT
#endif

#if DISABLE_VF_LIB_OPTIMIZATION     /* speed opt. disabled */
 #undef  RESTRICT
 #define RESTRICT
#endif

/*----------------------------------------------------------------------*/
/*                                  Typedefs                            */
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/*                           Function prototypes                        */
/*----------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

void i32_copy (                                         /* I32_COPY.C   */
    int *RESTRICT buf,      /* [in] int32 buffer                        */
    int *RESTRICT obuf,     /* [out] int32 buffer                       */
    int  n );               /* [in] # of vector elements                */

void i64_copy (                                         /* I64_COPY.C   */
    VL_INT64 *RESTRICT buf, /* [in] int64 buffer                       */
    VL_INT64 *RESTRICT obuf,/* [out] int64 buffer                      */
    int  n );               /* [in] # of vector elements                */

void i8_bits_to_bytes (                                 /* I8_B2BYT.C   */
    char *RESTRICT bit_buf,     /* [in] bit buffer                      */
    char *RESTRICT byte_buf,    /* [out] byte buffer (bit_cnt bytes)    */
    int   bit_cnt );            /* [in] number of bits in bit_buf       */

void i8_copy (                                          /* I8_COPY.C    */
    char *RESTRICT in_buf,  /* [in] vector                              */
    int   in_inc,           /* [in] "in_buf" address increment          */
    char *RESTRICT out_buf, /* [out] vector                             */
    int   out_inc,          /* [in] "out_buf" address increment         */
    int   n );              /* [in] # of vector elements                */

void i8_copy_clr (                                      /* I8_COPYC.C   */
    char *RESTRICT in_buf,  /* [in] color vector                        */
    char *RESTRICT out_buf, /* [out] color vector                       */
    int   trans_clr,        /* [in] trasparent color                    */
    int   ti_enc,           /* [in] TI color encoding flag              */
    int   n );              /* [in] # of vector elements                */

void i8_fill (                                          /* I8_FILL.C    */
    char *RESTRICT buf,     /* [out] vector                             */
    int   inc,              /* [in] vector address increment in bytes   */
    int   c,                /* [in] constant to fill                    */
    int   n );              /* [in] # of vector elements                */
void i8_set (
    char *RESTRICT a,       /* [i/o] vector A                           */
    int   ainc,             /* [in] vector A address increment in bytes */
    char *RESTRICT m,       /* [in] mask vector M (NULL: don't use)     */
    int   minc,             /* [in] vector M address increment in bytes */
    int   c,                /* [in] constant (byte value to fill)       */
    int   n );             /* [in] number of vector elements            */

void i8_fill_2c (                                       /* I8_FIL2C.C   */
    char *RESTRICT in_buf,  /* [in] mask vector                         */
    char *RESTRICT out_buf, /* [out] vector with color values           */
    int   fgnd,             /* [in] foreground color                    */
    int   bgnd,             /* [in] background color                    */
    int   n );              /* [in] # of vector elements                */

void u8_lookup (                                        /* U8_LOOK.C    */
    unsigned char *RESTRICT buf,    /* [i/o] char buffer                */
    unsigned char *RESTRICT ltab,   /* [in] look-up table               */
    int   n );                      /* [in] # of elements in buf        */

void u8_bin (                                           /* U8_BIN.C     */
    unsigned char *RESTRICT buf,    /* [i/o] vector                     */
    int    lo_th,           /* [in] lower binarization threshold        */
    int    up_th,           /* [in] upper binarization threshold        */
    int    fgnd_clr,        /* [in] foreground color                    */
    int    bgnd_clr,        /* [in] backround color                     */
    int    n );             /* [in] # of vector elements                */

void u8_pyrf (                                          /* U8_PYRF.C     */
    unsigned char *RESTRICT in_row1,  /* [in] image row 1 buffer        */
    unsigned char *RESTRICT in_row2,  /* [in] image row 2 buffer        */
    unsigned char *RESTRICT out_row,  /* [out] image row buffer         */
    int   n );                        /* [in] # of out_row elements     */

unsigned long u8_dotpr (                                /* U8_DOTPR.C   */
    unsigned char *RESTRICT a,  /* [in] vector A                        */
    int            ainc,        /* [in] vector A increment in elements  */
    unsigned char *RESTRICT b,  /* [in] vector B                        */
    int            binc,        /* [in] vector B increment in elements  */
    int            n );         /* [in] number of vector A & B elements */

unsigned long u8_dotpr_xor (                            /* U8_DOTPX.C   */
    unsigned char *RESTRICT a,  /* [in] vector A                        */
    int            ainc,        /* [in] vector A increment in elements  */
    unsigned char *RESTRICT b,  /* [in] vector B                        */
    int            binc,        /* [in] vector B increment in elements  */
    int            n );         /* [in] number of vector A & B elements */
unsigned long u8_dotpr_xorm (
    unsigned char *RESTRICT a,  /* [in] vector A                        */
    int            ainc,        /* [in] vector A increment in elements  */
    unsigned char *RESTRICT b,  /* [in] vector B                        */
    int            binc,        /* [in] vector B increment in elements  */
    unsigned char *RESTRICT m,  /* [in] mask vector M                   */
    int            minc,        /* [in] vector M increment in elements  */
    int            n );         /* [in] number of vector elements       \*/

unsigned long u8_vsum (                                 /* U8_VSUM.C    */
    unsigned char *RESTRICT a,  /* [in] vector A                        */
    int            ainc,        /* [in] vector A increment in elements  */
    int            n );         /* [in] number of vector A elements     */

void u8_histo (                                         /* U8_HISTO.C   */
    unsigned char *RESTRICT buf,    /* [in] U8 buffer                   */
    int            inc,             /* [in] 'buf' element increment     */
    int            n,               /* [in] number of elements in 'buf' */
    unsigned int *RESTRICT hist );  /* [out] histogram buffer           */

void u8_minmax (                                        /* U8_MINMA.C   */
    unsigned char *RESTRICT a,  /* [in] vector A                        */
    int            ainc,        /* [in] vector A increment in elements  */
    int            n,           /* [in] number of vector A elements     */
    int           *minv,        /* [out] min element value, NULL=no calc*/
    int           *maxv );      /* [out] max element value, NULL=no calc*/

void i32_minmax (                                       /* I32_MINM.C   */
    int    *RESTRICT a,         /* [in] vector A                        */
    int     ainc,               /* [in] vector A increment in elements  */
    int     n,                  /* [in] number of vector A elements     */
    int    *minv,               /* [out] min element value, NULL=no calc*/
    int    *maxv );             /* [out] max element value, NULL=no calc*/

void u8_sobel (                                         /* U8_SOBEL.C   */
    unsigned char *row1,    /* [in] image row 1 buffer                  */
    unsigned char *row2,    /* [in] image row 2 buffer                  */
    unsigned char *row3,    /* [in] image row 3 buffer                  */
    unsigned char *rrow,    /* [out] result image row buffer            */
    int            n,       /* [in] number of i/o buffer elements       */
    int            method );/* [in] sobel calculation method            */
void u8_sobelm (
    unsigned char *RESTRICT p1, /* [in] image row 1 buffer              */
    unsigned char *RESTRICT p2, /* [in] image row 2 buffer              */
    unsigned char *RESTRICT p3, /* [in] image row 3 buffer              */
    unsigned char *RESTRICT pr, /* [out] result image row buffer        */
    int            count,       /* [in] number of i/o buffer elements   */
    int           *sum,         /* [out] sum of pixel values in pr      */
    int           *maxv );      /* [out] max pixel value in pr          */

void u8_psum (                                          /* u8_psum.c    */
    unsigned char *RESTRICT a,  /* [in] source vector A                 */
    unsigned int  *RESTRICT r,  /* [out] result vector R                */
    unsigned int  *RESTRICT ra, /* [out] accumulation result vector RA  */
    int   pcnt,                 /* [in] partial sum count               */
    int   n );                  /* [in] number of A and R elements      */

void u8_pminmax (                                       /* u8_pminm.c   */
    unsigned char *RESTRICT a,      /* [in] source vector A             */
    unsigned char *RESTRICT rmin,   /* [out] min result vector RMIN     */
    unsigned char *RESTRICT rmax,   /* [out] max result vector RMAX     */
    unsigned char *RESTRICT rmina,  /* [out] min accum. vector RMINA    */
    unsigned char *RESTRICT rmaxa,  /* [out] max accum. vector RMAXA    */
    int      pcnt,                  /* [in] partial min/max count       */
    int      n );                   /* [in] number of vector elements   */

void u8_vminmax (                                       /* u8_vminm.c   */
    unsigned char *RESTRICT a,      /* [in] source vector A             */
    int            ainc,            /* [in] vector A increment          */
    unsigned char *RESTRICT b,      /* [in] source vector B             */
    int            binc,            /* [in] vector B increment          */
    unsigned char *RESTRICT rmin,   /* [out] result min vector RMIN     */
    int            rmin_inc,        /* [in] vector RMIN increment       */
    unsigned char *RESTRICT rmax,   /* [out] result max vector RMAX     */
    int            rmax_inc,        /* [in] vector RMAX increment       */
    int            n );             /* [in] number of vector elements   */
void u8_vmin (
    unsigned char *RESTRICT a,      /* [in] source vector A             */
    int            ainc,            /* [in] vector A increment          */
    unsigned char *RESTRICT b,      /* [in] source vector B             */
    int            binc,            /* [in] vector B increment          */
    unsigned char *RESTRICT r,      /* [out] result vector R            */
    int            rinc,            /* [in] vector R increment          */
    int            n );             /* [in] number of vector elements   */
void u8_vmax (
    unsigned char *RESTRICT a,      /* [in] source vector A             */
    int            ainc,            /* [in] vector A increment          */
    unsigned char *RESTRICT b,      /* [in] source vector B             */
    int            binc,            /* [in] vector B increment          */
    unsigned char *RESTRICT r,      /* [out] result vector R            */
    int            rinc,            /* [in] vector R increment          */
    int            n );             /* [in] number of vector elements   */

void u32_vadd (                                         /* u32_vadd.c   */
    unsigned int  *RESTRICT a,  /* [in] source vector A                 */
    int            ainc,        /* [in] vector A increment in elements  */
    unsigned int  *RESTRICT b,  /* [in] source vector B                 */
    int            binc,        /* [in] vector B increment in elements  */
    unsigned int  *RESTRICT c,  /* [out] result vector C                */
    int            cinc,        /* [in] vector C increment in elements  */
    int            n );         /* [in] number of vector elements       */

void u32_vsub (                                         /* u32_vsub.c   */
    unsigned int  *RESTRICT a,  /* [in] source vector A                 */
    int            ainc,        /* [in] vector A increment in elements  */
    unsigned int  *RESTRICT b,  /* [in] source vector B                 */
    int            binc,        /* [in] vector B increment in elements  */
    unsigned int  *RESTRICT c,  /* [out] result vector C                */
    int            cinc,        /* [in] vector C increment in elements  */
    int            n );         /* [in] number of vector elements       */

int u8_2i32 (                                           /* u8_2i32.c    */
    unsigned char *RESTRICT buf,    /* [in] buffer with 0/!=0 values    */
    int            n );             /* [in] buffer  size                */

void u8_eros_3x3 (                                      /* u8_eros3.c   */
    unsigned char *buf1,    /* [in] buffer with 1st image row           */
    unsigned char *buf2,    /* [in] buffer with 2nd image row           */
    unsigned char *buf3,    /* [in] buffer with 3rd image row           */
    unsigned char *obuf,    /* [out] buffer with result image row       */
    unsigned int   dx,      /* [in] # of image row pixels (buf sizes)   */
    int            smask,   /* [in] structuring element mask            */
    int            oper );  /* [in] operation (0=erosion, 1=dilation)   */

int  u8_expo_3x3 (                                      /* u8_expo3.c   */
    unsigned char *p1,      /* [in] buffer with 1st image row           */
    unsigned char *p2,      /* [in] buffer with 2nd image row           */
    unsigned char *p3,      /* [in] buffer with 3rd image row           */
    unsigned char *pr,      /* [out] buffer with result image row       */
    int            n );     /* [in] # of buffer elements                */

void u8_3x3 (                                           /* u8_3x3.c     */
    unsigned char *RESTRICT p1, /* [in] image row 1 buffer              */
    unsigned char *RESTRICT p2, /* [in] image row 2 buffer              */
    unsigned char *RESTRICT p3, /* [in] image row 3 buffer              */
    unsigned char *RESTRICT pr, /* [out] result image row buffer        */
    int            count,       /* [in] number of i/o buffer elements   */
    int            c[3][3],     /* [in] 3x3 matrix with filter coeff.   */
    int            sh,          /* [in] shift value: <0:right, >0:left  */
    int            mode,        /* [in] operation mode: bit0=abs        */
    int            fill );      /* [in] fill value: -1:leave, 0-255:fill*/
void u8_3x3i (
    unsigned char *RESTRICT p1, /* [in] image row 1 buffer              */
    unsigned char *RESTRICT p2, /* [in] image row 2 buffer              */
    unsigned char *RESTRICT p3, /* [in] image row 3 buffer              */
    int           *RESTRICT pr, /* [out] result image row buffer        */
    int            count,       /* [in] number of i/o buffer elements   */
    int           *c,           /* [in] 3x3 matrix with filter coeff.   */
    int            fill );      /* [in] fill value: -1:leave, 0-255:fill*/

void u8_5x5 (                                           /* u8_5x5.c     */
    unsigned char *RESTRICT p0, /* [in] source image row 0              */
    unsigned char *RESTRICT p1, /* [in] source image row 1              */
    unsigned char *RESTRICT p2, /* [in] source image row 2              */
    unsigned char *RESTRICT p3, /* [in] source image row 3              */
    unsigned char *RESTRICT p4, /* [in] source image row 4              */
    unsigned char *RESTRICT pr, /* [out] result image row buffer        */
    int            count,       /* [in] number of i/o buffer elements   */
    int            c[5][5],     /* [in] 5x5 matrix with filter coeff.   */
    int            sh,          /* [in] shift value: <0:right, >0:left  */
    int            mode,        /* [in] operation mode: bit0=abs        */
    int            fill );      /* [in] fill value: -1:leave, 0-255:fill*/
void u8_5x5i (
    unsigned char *RESTRICT p0, /* [in] source image row 0              */
    unsigned char *RESTRICT p1, /* [in] source image row 1              */
    unsigned char *RESTRICT p2, /* [in] source image row 2              */
    unsigned char *RESTRICT p3, /* [in] source image row 3              */
    unsigned char *RESTRICT p4, /* [in] source image row 4              */
    int           *RESTRICT pr, /* [out] result image row buffer        */
    int            count,       /* [in] number of i/o buffer elements   */
    int           *c,           /* [in] 5x5 matrix with filter coeff.   */
    int            fill );      /* [in] fill value: -1:leave, 0-255:fill*/

void u8_7x7 (                                           /* u8_7x7.c     */
    unsigned char *RESTRICT p0, /* [in] source image row 0              */
    unsigned char *RESTRICT p1, /* [in] source image row 1              */
    unsigned char *RESTRICT p2, /* [in] source image row 2              */
    unsigned char *RESTRICT p3, /* [in] source image row 3              */
    unsigned char *RESTRICT p4, /* [in] source image row 4              */
    unsigned char *RESTRICT p5, /* [in] source image row 5              */
    unsigned char *RESTRICT p6, /* [in] source image row 6              */
    unsigned char *RESTRICT pr, /* [out] result image row buffer        */
    int            count,       /* [in] number of i/o buffer elements   */
    int            c[7][7],     /* [in] 7x7 matrix with filter coeff.   */
    int            sh,          /* [in] shift value: <0:right, >0:left  */
    int            mode,        /* [in] operation mode: bit0=abs        */
    int            fill );      /* [in] fill value: -1:leave, 0-255:fill*/
void u8_7x7i (
    unsigned char *RESTRICT p0, /* [in] source image row 0              */
    unsigned char *RESTRICT p1, /* [in] source image row 1              */
    unsigned char *RESTRICT p2, /* [in] source image row 2              */
    unsigned char *RESTRICT p3, /* [in] source image row 3              */
    unsigned char *RESTRICT p4, /* [in] source image row 4              */
    unsigned char *RESTRICT p5, /* [in] source image row 5              */
    unsigned char *RESTRICT p6, /* [in] source image row 6              */
    int           *RESTRICT pr, /* [out] result image row buffer        */
    int            count,       /* [in] number of i/o buffer elements   */
    int           *c,           /* [in] 7x7 matrix with filter coeff.   */
    int            fill );      /* [in] fill value: -1:leave, 0-255:fill*/

void u8_9x9 (                                           /* u8_9x9.c     */
    unsigned char *RESTRICT p0, /* [in] source image row 0              */
    unsigned char *RESTRICT p1, /* [in] source image row 1              */
    unsigned char *RESTRICT p2, /* [in] source image row 2              */
    unsigned char *RESTRICT p3, /* [in] source image row 3              */
    unsigned char *RESTRICT p4, /* [in] source image row 4              */
    unsigned char *RESTRICT p5, /* [in] source image row 5              */
    unsigned char *RESTRICT p6, /* [in] source image row 6              */
    unsigned char *RESTRICT p7, /* [in] source image row 7              */
    unsigned char *RESTRICT p8, /* [in] source image row 8              */
    unsigned char *RESTRICT pr, /* [out] result image row buffer        */
    int            count,       /* [in] number of i/o buffer elements   */
    int            c[9][9],     /* [in] 9x9 matrix with filter coeff.   */
    int            sh,          /* [in] shift value: <0:right, >0:left  */
    int            mode,        /* [in] operation mode: bit0=abs        */
    int            fill );      /* [in] fill value: -1:leave, 0-255:fill*/
void u8_9x9i (
    unsigned char *RESTRICT p0, /* [in] source image row 0              */
    unsigned char *RESTRICT p1, /* [in] source image row 1              */
    unsigned char *RESTRICT p2, /* [in] source image row 2              */
    unsigned char *RESTRICT p3, /* [in] source image row 3              */
    unsigned char *RESTRICT p4, /* [in] source image row 4              */
    unsigned char *RESTRICT p5, /* [in] source image row 5              */
    unsigned char *RESTRICT p6, /* [in] source image row 6              */
    unsigned char *RESTRICT p7, /* [in] source image row 7              */
    unsigned char *RESTRICT p8, /* [in] source image row 8              */
    int           *RESTRICT pr, /* [out] result image row buffer        */
    int            count,       /* [in] number of i/o buffer elements   */
    int           *c,           /* [in] 9x9 matrix with filter coeff.   */
    int            fill );      /* [in] fill value: -1:leave, 0-255:fill*/

void u8_11x11 (                                         /* u8_11x11.c   */
    unsigned char *RESTRICT p0, /* [in] source image row 0              */
    unsigned char *RESTRICT p1, /* [in] source image row 1              */
    unsigned char *RESTRICT p2, /* [in] source image row 2              */
    unsigned char *RESTRICT p3, /* [in] source image row 3              */
    unsigned char *RESTRICT p4, /* [in] source image row 4              */
    unsigned char *RESTRICT p5, /* [in] source image row 5              */
    unsigned char *RESTRICT p6, /* [in] source image row 6              */
    unsigned char *RESTRICT p7, /* [in] source image row 7              */
    unsigned char *RESTRICT p8, /* [in] source image row 8              */
    unsigned char *RESTRICT p9, /* [in] source image row 9              */
    unsigned char *RESTRICT pA, /* [in] source image row 10             */
    unsigned char *RESTRICT pr, /* [out] result image row buffer        */
    int            count,       /* [in] number of i/o buffer elements   */
    int            c[11][11],   /* [in] 11x11 matrix with filter coeff. */
    int            sh,          /* [in] shift value: <0:right, >0:left  */
    int            mode,        /* [in] operation mode: bit0=abs        */
    int            fill );      /* [in] fill value: -1:leave, 0-255:fill*/
void u8_11x11i (
    unsigned char *RESTRICT p0, /* [in] source image row 0              */
    unsigned char *RESTRICT p1, /* [in] source image row 1              */
    unsigned char *RESTRICT p2, /* [in] source image row 2              */
    unsigned char *RESTRICT p3, /* [in] source image row 3              */
    unsigned char *RESTRICT p4, /* [in] source image row 4              */
    unsigned char *RESTRICT p5, /* [in] source image row 5              */
    unsigned char *RESTRICT p6, /* [in] source image row 6              */
    unsigned char *RESTRICT p7, /* [in] source image row 7              */
    unsigned char *RESTRICT p8, /* [in] source image row 8              */
    unsigned char *RESTRICT p9, /* [in] source image row 9              */
    unsigned char *RESTRICT pA, /* [in] source image row 10             */
    int           *RESTRICT pr, /* [out] result image row buffer        */
    int            count,       /* [in] number of i/o buffer elements   */
    int           *c,           /* [in] 11x11 matrix with filter coeff. */
    int            fill );      /* [in] fill value: -1:leave, 0-255:fill*/


void mem_cpy (                                          /* mem_cpy.c    */
    void   *dst,            /* [out] destination memory buffer          */
    void   *src,            /* [in] source memory buffer                */
    unsigned int len );     /* [in] # of bytes to copy                  */


#ifdef __cplusplus
}
#endif

#endif      /* ]]] _VF_LIB_H*/
