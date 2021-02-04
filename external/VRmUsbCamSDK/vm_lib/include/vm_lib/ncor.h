/*----------------------------------------------------------------------*/
/**
* @name NCOR.H
*       Normalized correlation on PC
*
* <br>  VM_LIB is subject to the license and usage terms of VRmagic GmbH,
* <br>  which are distributed with this software.
* <br>
* <br>  This file defines macros and type definitions for NCOR module
*       ported to PC.
*
* @version
*       Created:  09.12.2003
* <br>  Modified: 02.01.2007
* <br>            Vers 3.10 : Added "exact" ncor() argument.
*/
/*----------------------------------------------------------------------*/
/*@{*/

#ifndef _NCOR_H     /* [[[ */
#define _NCOR_H

//#include <stdio.h>
#ifdef _TI_CAMERA
  #include <vclib.h>    /* image definition on TI camera */
#endif

/****** NCOR.H = Macros and typedefs for NCOR library = 01.2007 *********/

/*............. Version */
#define NC_VERS  311        /* NCOR library version (310 for 3.10)      */
                            /* 3.11: PK: pc_image -> VC image           */
                            /* 3.10: PK: exact odd/even result coord    */
                            /* 3.05: JB: larger image & pattern         */
#define NC_VERS_STR "3.11"  /* library version string                   */

/* Memory specifier, disabled for non-ADSP code */
#define PM

#ifndef _TI_CAMERA
  #define  pstr      printf
  #define  print     printf
  #define  sprint    sprintf
  #define  rs232rcv  getch
#endif

/* PC compatible image - included for compatibility with P&F sensor correlation */
typedef struct
{
    char *stp;      /* start byte address of image data as pointer  */
    int   dx;       /* image width                                  */
    int   dy;       /* image height                                 */
    int   pitch;    /* image pitch >= dx                            */
} pc_image;

/* VC camera compatible image */
#ifndef _TI_CAMERA
  #ifndef IMAGE_DEFINED
    #define IMAGE_DEFINED
    typedef struct
    {
        long st;    /* start byte address of image data as long     */
        int  dx;    /* image width                                  */
        int  dy;    /* image height                                 */
        int  pitch; /* image pitch >= dx                            */
    } image;
  #endif
#endif

/*............. Memory allocation on PC */
#ifndef _TI_CAMERA
  #define nc_alloc(size)     malloc(size)                 /* size in bytes        */
  #define nc_alloc_w16(size) malloc(sizeof(short)*(size)) /* size in 16-bit words */
  #define nc_alloc_w32(size) malloc(sizeof(int)*(size))   /* size in 32-bit words */
  #define nc_free            free

/*............. Memory allocation on TI camera */
#else
  #define nc_alloc(size)     vcmalloc(((size)+3)/4)   /* size in bytes        */
  #define nc_alloc_w16(size) vcmalloc(((size)+1)/2)   /* size in 16-bit words */
  #define nc_alloc_w32(size) vcmalloc(size)           /* size in 32-bit words */
  #define nc_free            vcfree
#endif

//#ifdef _TI_CAMERA
//  #define KBHIT()  (-1 != rbempty())
//  #define GETCH()  rs232rcv()
//#else
//  #define KBHIT()  _kbhit()
//  #define GETCH()  _getch()
//#endif


/*----------------------------------------------------------------------*/
/*                           Function prototypes                        */
/*----------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/* Main normalized correlation function */
int  ncor (
    image     *pat_img,     /* input pattern image (VC format)          */
    image     *srh_img,     /* input search image (VC format)           */
    short      mode,        /* input search mode:                       */
                            /*  -1=fast, 0=normal, 1=fine               */
    short      exact,       /* input exact searching mode:              */
                            /*  0=even only(fast), 1=odd/even results   */
    short      th,          /* input rate threshold (512-1023)          */
    short     *rate,        /* output matching rate                     */
    short     *res_x,       /* output x-coord of found pattern          */
    short     *res_y );     /* output y-coord of found pattern          */

/* Get system time in ms */
unsigned long nc_get_time ();

void nc_config (
    int fast_pmf,       /* [in] max # to search in fast mode   (def=20) */
    int norm_pmf,       /* [in] max # to search in normal mode (def=10) */
    int fine_pmf,       /* [in] max # to search in fine mode   (def=5)  */
    int blur_filt,      /* [in] blur filter flag:  0,1=off,on(def)      */
    int sharp_filt );   /* [in] sharpen filter flg: 0,1=off(def),on     */
void nc_get_config (
    int *fast_pmf,      /* [out] max # to search in fast mode           */
    int *norm_pmf,      /* [out] max # to search in normal mode         */
    int *fine_pmf,      /* [out] max # to search in fine mode           */
    int *blur_filt,     /* [out] blur filter flag: 0,1=off,on(def)      */
    int *sharp_filt );  /* [out] sharpen filter flg: 0,1=off(def),on    */

#ifdef __cplusplus
};
#endif

#endif  /* ]]] - _NCOR_H */
/*@}*/
