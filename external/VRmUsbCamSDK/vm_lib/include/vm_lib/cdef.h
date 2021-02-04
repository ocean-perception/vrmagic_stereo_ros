/*----------------------------------------------------------------------*/
/**
* @name CDEF.H
*       Common macro definitions and typedefs
*
* <br>  VM_LIB is subject to the license and usage terms of VRmagic GmbH,
* <br>  which are distributed with this software.
* <br>
* <br>  Header file for common VM_LIB library system-dependent macros
* <br>  and typedefs.
*
* @version
*       Created:   13.12.2005
* <br>  Modified:
*/
/*----------------------------------------------------------------------*/
/*@{*/

#ifndef _CDEF_H_LOADED   /* [[[ */
#define _CDEF_H_LOADED

//#if defined(_DEBUG) && defined(_VM_PC) && !defined(__cplusplus)
//   #define _CRTDBG_MAP_ALLOC
//   #include <crtdbg.h>
//#endif

//#ifdef _MSC_VER
//#pragma warning(disable: 4244 4761)
//#endif

/*----------------------------------------------------------------------*/
/*                      Supported hardare platforms                     */
/*----------------------------------------------------------------------*/
/*
* Set macro _CAM_TYPE for different hardware platforms.
* One of the following macros must be defined by a compiler's command
* line switch -D<macro>:
*   _VM_PC        : PC Simulator
*   _MSC_VER      : MS C/C++ compiler (passed automatically, no need to set)
*   _TI_CAMERA    : VCxxxx TI-based camera
*   _BFIN         : Blackfin uClinux camera
*   _ARM          : ARM Embedded Linux camera
*   _VR_ARM       : VRmagic camera - ARM
*   _VR_DSP       : VRmagic camera - DSP
*   _VR_LINUX_X86 : VRmagic linux x86 lib
*/

/*............. Camera type macros: */
#define _CAM_SIM      0     /* PC Simulator                                 */
#define _CAM_PC       1     /* PC library                                   */
#define _CAM_TI       2     /* VCxxxx TI-based camera                       */
#define _CAM_BF       3     /* Blackfin uClinux camera                      */
#define _CAM_ARM      4     /* ARM Embedded Linux camera (i.MX27 IP camera) */
#define _CAM_VR_ARM   5     /* VRmagic camera - ARM Debian Linux            */
#define _CAM_VR_DSP   6     /* VRmagic camera - DSP                         */

/*............. Set _CAM_TYPE = camera type */
#if defined(_MSC_VER) && defined(_VM_PC)
  #define _CAM_TYPE  _CAM_SIM     /* PC Simulator                       */
#elif defined(_MSC_VER)
  #define _CAM_TYPE  _CAM_PC      /* PC - stand-alone library           */
#elif defined(_TI_CAMERA)
  #define _CAM_TYPE  _CAM_TI      /* VCxxx TI-based camera              */
#elif defined(_BFIN)
  #define _CAM_TYPE  _CAM_BF      /* Blackfin uClinux camera            */
#elif defined(_ARM)
  #define _CAM_TYPE  _CAM_ARM     /* ARM Embedded Linux camera          */
#elif defined(_VR_ARM)
  #define _CAM_TYPE  _CAM_VR_ARM  /* VRmagic camera - ARM Debian Linux  */
#elif defined(_VR_DSP)
  #define _CAM_TYPE  _CAM_VR_DSP  /* VRmagic camera - DSP               */
#elif defined(_VR_LINUX_X86)
  #define _CAM_TYPE  _CAM_VR_ARM
#else
  #error cdef.h: Camera type not supported
#endif


/*----------------------------------------------------------------------*/
/*                               Include files                          */
/*----------------------------------------------------------------------*/
#if _CAM_TYPE == _CAM_TI    /* TI camera */
//  #include <register.h>
  #include <sysvar.h>
  #include <vcrt.h>       /* memory allocation */
  #include <vclib.h>
#elif _CAM_TYPE == _CAM_BF  /* Blackfin uClinux camera CANCamBF */
  #include <stdlib.h>
  #include <stdio.h>
  #include "cancambf.h"
#else                       /* PC */
  #include <stdlib.h>
  #include <stdio.h>
#endif


/*----------------------------------------------------------------------*/
/*                        Basic type definitions                        */
/*----------------------------------------------------------------------*/

/*............. _CAM_SIM or _CAM_PC - PC : MS C/C++ */
#if defined(_MSC_VER)
  typedef char                  VL_INT8;
  typedef unsigned char         VL_UINT8;
  typedef short                 VL_INT16;
  typedef unsigned short        VL_UINT16;
  typedef long                  VL_INT32;
  typedef unsigned long         VL_UINT32;

  typedef _int64                VL_INT64;
  typedef unsigned _int64       VL_UINT64;

#elif _CAM_TYPE == _CAM_TI    /* TI camera */
  typedef char                  VL_INT8;
  typedef unsigned char         VL_UINT8;
  typedef short                 VL_INT16;
  typedef unsigned short        VL_UINT16;
  typedef int                   VL_INT32;
  typedef unsigned int          VL_UINT32;

  typedef long long             VL_INT64;
  typedef unsigned long long    VL_UINT64;

#elif _CAM_TYPE == _CAM_BF  /* Blackfin uClinux camera */
  typedef char                  VL_INT8;
  typedef unsigned char         VL_UINT8;
  typedef short                 VL_INT16;
  typedef unsigned short        VL_UINT16;
  typedef int                   VL_INT32;
  typedef unsigned int          VL_UINT32;

  typedef long long             VL_INT64;
  typedef unsigned long long    VL_UINT64;

#elif _CAM_TYPE == _CAM_ARM /* ARM Linux camera */
  typedef char                  VL_INT8;
  typedef unsigned char         VL_UINT8;
  typedef short                 VL_INT16;
  typedef unsigned short        VL_UINT16;
  typedef int                   VL_INT32;
  typedef unsigned int          VL_UINT32;

  typedef long long             VL_INT64;
  typedef unsigned long long    VL_UINT64;

#elif _CAM_TYPE == _CAM_VR_ARM  /* VRmagic camera - ARM */
  typedef char                  VL_INT8;
  typedef unsigned char         VL_UINT8;
  typedef short                 VL_INT16;
  typedef unsigned short        VL_UINT16;
  typedef int                   VL_INT32;
  typedef unsigned int          VL_UINT32;

  typedef long long             VL_INT64;
  typedef unsigned long long    VL_UINT64;

#elif _CAM_TYPE == _CAM_VR_DSP  /* VRmagic camera - DSP */
  typedef char                  VL_INT8;
  typedef unsigned char         VL_UINT8;
  typedef short                 VL_INT16;
  typedef unsigned short        VL_UINT16;
  typedef int                   VL_INT32;
  typedef unsigned int          VL_UINT32;

  typedef long long             VL_INT64;
  typedef unsigned long long    VL_UINT64;

#else
  #error cdef.h: Camera type not supported
#endif


/*----------------------------------------------------------------------*/
/*                         Image type definitions                       */
/*----------------------------------------------------------------------*/

/*............. VC image on PC */
/* 'image' is not defined on all platforms except VCxxxx TI camera  */
#if _CAM_TYPE != _CAM_TI    /* [[[ - no VCxxxx TI-based camera */
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

///*............. TCP image */
///* Keep sync. with IMAGE in VL\TCP\tcp_io.h */
//#ifndef PC_IMAGE_DEFINED    /* [[[ */
//  #define PC_IMAGE_DEFINED
//  typedef struct
//  {
//      char   *st;         /* start byte address of image buffer   */
//      int     dx;         /* image width in pixels                */
//      int     dy;         /* image height in pixels               */
//      int     pitch;      /* image pitch in bytes                 */
//  } IMAGE;
//#endif  /* ]]] */

/*............. Video and drawing images */
typedef image VD_IMAGE; /* gray-level (video-page) image : 1 pixel = 1 byte */
typedef image DR_IMAGE; /* drawing (overlay-page) image  : 1 pixel = 1 byte */

///*............. Gray-level (video-page) image */
////typedef IMAGE VD_IMAGE; /* gray-level (video-page) image : 1 pixel = 1 byte */
////typedef IMAGE DR_IMAGE; /* drawing (overlay-page) image  : 1 pixel = 1 byte */
//typedef struct
//{
//    char   *st;         /* start byte address of image buffer   */
//    int     dx;         /* image width in pixels                */
//    int     dy;         /* image height in pixels               */
//    int     pitch;      /* image pitch in bytes                 */
//} VD_IMAGE;
//
///*............. Drawing (overlay-page) image : 1 pixel = 1 byte */
//typedef struct
//{
//    char   *st;         /* start byte address of image buffer   */
//    int     dx;         /* image width in pixels                */
//    int     dy;         /* image height in pixels               */
//    int     pitch;      /* image pitch                          */
//} DR_IMAGE;

/*----------------------------------------------------------------------*/
/*                       Memory allocation macros                       */
/*----------------------------------------------------------------------*/
#undef mem_alloc    /* force un-definition */
#undef mem_free

#if _CAM_TYPE == _CAM_TI    /* TI camera */
  #define mem_alloc(size)    vcmalloc(((size)+3)/4)   /* size in bytes        */
  #define mem_alloc_16(size) vcmalloc(((size)+1)/2)   /* size in 16-bit words */
  #define mem_alloc_32(size) vcmalloc(size)           /* size in 32-bit words */
  #define mem_free           vcfree
#else                       /* PC & other platforms */
  #define mem_alloc(size)    malloc(size)                 /* size in bytes        */
  #define mem_alloc_16(size) malloc(sizeof(short)*(size)) /* size in 16-bit words */
  #define mem_alloc_32(size) malloc(sizeof(int)*(size))   /* size in 32-bit words */
  #define mem_free           free
#endif

/*----------------------------------------------------------------------*/
/*                           Calculation macros                         */
/*----------------------------------------------------------------------*/

/*............. C run-time library functions */
#if _CAM_TYPE == _CAM_TI    /* TI camera : fast single-precision functions */
  #define SQRT  sqrtf
  #define SIN   sinf
  #define COS   cosf
  #define ATAN  atanf
  #define ATAN2 atan2f
  #define FABS  fabsf
#else                       /* PC & other - double precision math functions */
  #define SQRT  sqrt
  #define SIN   sin
  #define COS   cos
  #define ATAN  atan
  #define ATAN2 atan2
  #define FABS  fabs
#endif


/*----------------------------------------------------------------------*/
/*                             General macros                           */
/*----------------------------------------------------------------------*/
#ifndef VL_NULL
  #define VL_NULL ((void *)0)
#endif

#ifndef VL_MAX
  #define VL_MAX(a,b)  (((a) > (b)) ? (a) : (b))
#endif

#ifndef VL_MIN
  #define VL_MIN(a,b)  (((a) < (b)) ? (a) : (b))
#endif

#ifndef VL_SIGN
  #define VL_SIGN(x)   ((x) > 0 ? 1 : ((x) == 0 ? 0 : (-1)))
#endif

//#if defined(_MSC_VER) && !defined(_INC_MATH)   /* MSC: abs is defined in math.h(154) */
//#if !defined(_MSC_VER)       /* MSC: abs is defined in math.h(154) */
#ifndef VL_ABS
  #define VL_ABS(x)    ((x) >= 0 ? (x) : -(x))
#endif
//#endif

#ifndef VL_ROUND
  #define VL_ROUND(x)  ((int)(((x) >= 0) ? (x) + 0.5f : (x) - 0.5f))
#endif

#if !defined(VL_FALSE) || !defined(VL_TRUE)
  #define VL_FALSE 0
  #define VL_TRUE  1
  typedef int VL_BOOL;
#endif

#endif /* ]]] _CDEF_H_LOADED */
/*@}*/
