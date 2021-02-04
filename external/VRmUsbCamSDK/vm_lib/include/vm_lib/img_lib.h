/****** IMG_LIB.H = IMG_LIB header file = 01.2009 ***********************/
/*                                                         */
/* Include file for the image I/O file library IMG_LIB     */
/*  
*
*   VM_LIB is subject to the license and usage terms of VRmagic GmbH,
*   which are distributed with this software.
*/

#ifndef _IMG_LIB_H_LOADED   /* [[[ */
#define _IMG_LIB_H_LOADED

//#if defined(_MSC_VER)
//  #pragma warning(disable: 4103)
//  #pragma pack(1)   /* MS C/C++: disable alignment of structure members */
//#endif

/*----------------------------------------------------------------------*/
/*                          Macros and typedefs                         */
/*----------------------------------------------------------------------*/
//typedef unsigned short USHORT;
//typedef unsigned long  ULONG;

#if !defined(_TI_CAMERA)    /* [[[ - PC */

/*............. VC image on PC */
  #ifndef IMAGE_DEFINED
    typedef struct      /* memory image structure (VC compatible) */
    {
        long st;        /* start byte address of image memory buffer        */
        int  dx;        /* image width in pixels                            */
        int  dy;        /* image height in pixels                           */
        int  pitch;     /* memory pitch = # of bytes between two pixel rows */
    } image;
    #define IMAGE_DEFINED
  #endif

/*............. Portable file operations */
  #define IO_SEEK_SET SEEK_SET  /* Seek from start              */
  #define IO_SEEK_CUR SEEK_CUR  /* Seek from current location   */
  #define IO_SEEK_END SEEK_END  /* Seek from end                */
  #define io_ftell    ftell
  #define io_fseek    fseek

#endif  /* ]]] */

#undef mem_alloc    /* force un-definition */
#undef mem_free

#if defined(_TI_CAMERA)     /* TI camera */
  #define mem_alloc(size)    vcmalloc(((size)+3)/4)   /* size in bytes        */
  #define mem_free           vcfree
#else                       /* PC & other platforms */
  #define mem_alloc(size)    malloc(size)                 /* size in bytes        */
  #define mem_free           free
#endif

/*----------------------------------------------------------------------*/
/*                        Function prototypes                           */
/*----------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

int  read_img_file (                                    /* IMG_LIB.C    */
    char   *img_file,       /* [in] source image file in IMGF format    */
    image  *img );          /* [out] destination image, allocated here  */
int write_img_file (
    char   *img_file,       /* [out] destination file in IMGF format    */
    image  *img,            /* [in] source image                        */
    int     x,              /* [in] x-coord. of upper left image corner */
    int     y );            /* [in] y-coord. of upper left image corner */

#ifdef __cplusplus
}
#endif

#endif /* ]]] - _IMG_LIB_H_LOADED */
