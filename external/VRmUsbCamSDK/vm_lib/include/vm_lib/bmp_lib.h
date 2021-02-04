/****** BMP_LIB.H = BMP_LIB header file = 09.2008 ***********************/
/*
* Include file with typedefs and macros, used by the BMP read/write
* library.
*
* <br>  VM_LIB is subject to the license and usage terms of VRmagic GmbH,
* <br>  which are distributed with this software.
*/

#ifndef _BMP_LIB_H_LOADED   /* [[[ */
#define _BMP_LIB_H_LOADED

#if defined(_MSC_VER)
  #pragma warning(disable: 4103)
  #pragma pack(1)   /* MS C/C++: disable alignment of structure members */
#endif

#include "cdef.h"

/*----------------------------------------------------------------------*/
/*                        BMP file header structures                    */
/*----------------------------------------------------------------------*/
#if !defined(_MSC_VER)      /* [[[ - All platforms except PC: avoid member alignment and gaps */
  typedef struct
  {
          VL_UINT16 bfType;

//          VL_UINT32 bfSize;
          VL_UINT16 bfSize_l;   /* bfSize low 16-bit word */
          VL_UINT16 bfSize_h;   /* bfSize high 16-bit word */

          VL_UINT16 bfReserved1;
          VL_UINT16 bfReserved2;

//          VL_UINT32 bfOffBits;
          VL_UINT16 bfOffBits_l;    /* bfOffBits low 16-bit word */
          VL_UINT16 bfOffBits_h;    /* bfOffBits high 16-bit word */
  } BMP_FILE_HDR;
#else                       /* ]]] [[[ - PC */
  typedef struct
  {
          VL_UINT16 bfType;
          VL_UINT32 bfSize;
          VL_UINT16 bfReserved1;
          VL_UINT16 bfReserved2;
          VL_UINT32 bfOffBits;
  } BMP_FILE_HDR;
#endif  /* ]]] */

typedef struct
{
        VL_UINT32 biSize;
        VL_INT32  biWidth;
        VL_INT32  biHeight;
        VL_UINT16 biPlanes;
        VL_UINT16 biBitCount;
        VL_UINT32 biCompression;
        VL_UINT32 biSizeImage;
        VL_INT32  biXPelsPerMeter;
        VL_INT32  biYPelsPerMeter;
        VL_UINT32 biClrUsed;
        VL_UINT32 biClrImportant;
} BMP_INFO_HDR;

//typedef struct
//{
//        unsigned char rgbBlue;
//        unsigned char rgbGreen;
//        unsigned char rgbRed;
//        unsigned char rgbReserved;
//} RGBQUAD;

////............. My custom BMP header
//typedef struct
//{
//        short Width;
//        short Height;
//        short BitCount;
//} BMP_HEADER;


/*----------------------------------------------------------------------*/
/*                          Macros and typedefs                         */
/*----------------------------------------------------------------------*/

/*............. Portable memory allocation */
#if !defined(mem_alloc)
#if defined(_TI_CAMERA)     /* [[[ - TI camera */
  #define mem_alloc(size)         vcmalloc(((size)+3)/4)  /* in bytes */
  #define mem_free(ptr)           vcfree(ptr)
#else     /* ]]] [[[ - PC */
  #define mem_alloc(size)         malloc(sizeof(char) * (size))
  #define mem_free(ptr)           free(ptr)
#endif  /* ]]] */
#endif

#if !defined(_TI_CAMERA)    /* [[[ - PC */

/*............. Portable file operations */
  #define IO_SEEK_SET SEEK_SET  /* Seek from start              */
  #define IO_SEEK_CUR SEEK_CUR  /* Seek from current location   */
  #define IO_SEEK_END SEEK_END  /* Seek from end                */
  #define io_ftell    ftell
  #define io_fseek    fseek

/*............. VC image on PC */
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

typedef unsigned short USHORT;
typedef unsigned long  ULONG;

/*----------------------------------------------------------------------*/
/*                        Function prototypes                           */
/*----------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

int  read_bmp_file (                                    /* BMP_LIB.C    */
    char  *bmp_file,        /* [in] source BMP file                     */
    image *img );           /* [out] destination image, allocated here  */
int  write_bmp_file (
    char  *bmp_file,        /* [out] destination BMP file               */
    image *img );           /* [in] source image                        */

#ifdef __cplusplus
}
#endif

#endif /* ]]] - _BMP_LIB_H_LOADED */
