/*----------------------------------------------------------------------*/
/**
* @name VC_BLIB.H
*       VCLIB compatible vector function library header
*
* <br>  VM_LIB is subject to the license and usage terms of VRmagic GmbH,
* <br>  which are distributed with this software.
* <br>
* <br>  This file contains type definitions, macros and function prototypes
*       of basic VCLIB-compatible vector functions.
*
* @version
*       Created:  21.11.2005
* <br>  Modified:
*/
/*----------------------------------------------------------------------*/
/*@{*/

#ifndef _VC_BLIB_H     /* [[[ */
#define _VC_BLIB_H

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
  #error vc_blib.h: Camera type not supported
#endif

/* DDD */
#define DISABLE_VC_BLIB_OPTIMIZATION   0   /* 1=disable TI speed optimization  */

/*----------------------------------------------------------------------*/
/*                                 Macros                               */
/*----------------------------------------------------------------------*/

/*............. VC_BLIB compilation macros */
#if defined(_VC_CAM) || defined(VR_DSP_CAM)     /* TMS320C6x cameras */
  #define RESTRICT restrict
#else                       /* PC & other platforms */
  #define RESTRICT
#endif

#if DISABLE_VC_BLIB_OPTIMIZATION     /* speed opt. disabled */
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

void u8_add2f(                                          /* VC_BLIB.C    */
    int  count,                 /* [in] # of elements                  */
    unsigned char *RESTRICT p1, /* [in] buffer 1                       */
    unsigned char *RESTRICT p2, /* [in] buffer 2                       */
    unsigned char *RESTRICT pr, /* [out] buffer                        */
    int   sh );                 /* [in] shift value: >=0:left, <0:right*/
void u8_sub2f (
    int  n,                     /* [in] # of elements                  */
    unsigned char *RESTRICT p1, /* [in] buffer 1                       */
    unsigned char *RESTRICT p2, /* [in] buffer 2                       */
    unsigned char *RESTRICT pr);/* [out] buffer                        */
void u8_max2f (
    int  n,                     /* [in] # of elements                  */
    unsigned char *RESTRICT p1, /* [in] buffer 1                       */
    unsigned char *RESTRICT p2, /* [in] buffer 2                       */
    unsigned char *RESTRICT pr);/* [out] buffer                        */
void u8_min2f (
    int  n,                     /* [in] # of elements                  */
    unsigned char *RESTRICT p1, /* [in] buffer 1                       */
    unsigned char *RESTRICT p2, /* [in] buffer 2                       */
    unsigned char *RESTRICT pr);/* [out] buffer                        */
void u8_and2f (
    int  n,                     /* [in] # of elements                  */
    unsigned char *RESTRICT p1, /* [in] buffer 1                       */
    unsigned char *RESTRICT p2, /* [in] buffer 2                       */
    unsigned char *RESTRICT pr);/* [out] buffer                        */
void u8_or2f (
    int  n,                     /* [in] # of elements                  */
    unsigned char *RESTRICT p1, /* [in] buffer 1                       */
    unsigned char *RESTRICT p2, /* [in] buffer 2                       */
    unsigned char *RESTRICT pr);/* [out] buffer                        */
void u8_xor2f (
    int  n,                     /* [in] # of elements                  */
    unsigned char *RESTRICT p1, /* [in] buffer 1                       */
    unsigned char *RESTRICT p2, /* [in] buffer 2                       */
    unsigned char *RESTRICT pr);/* [out] buffer                        */
void u8_not2f (
    int  n,                     /* [in] # of elements                  */
    unsigned char *RESTRICT p1, /* [in] buffer 1                       */
    unsigned char *p2,          /* [in] buffer 2 (not used)            */
    unsigned char *RESTRICT pr);/* [out] buffer                        */

#ifdef __cplusplus
}
#endif

#endif      /* ]]] _VC_BLIB_H*/
