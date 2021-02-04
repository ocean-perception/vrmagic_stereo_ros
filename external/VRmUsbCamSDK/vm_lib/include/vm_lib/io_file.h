/*----------------------------------------------------------------------*/
/**
* @name IO_FILE.H
*       I/O file library header
*
* <br>  VM_LIB is subject to the license and usage terms of VRmagic GmbH,
* <br>  which are distributed with this software.
* <br>
* <br>  Header for file read/write library functions.
*
* @version
*       Created:  26.10.2005
* <br>  Modified:
*/
/*----------------------------------------------------------------------*/
/*@{*/

#ifndef _IO_FILE_H_LOADED   /* [[[ */
#define _IO_FILE_H_LOADED

#include "cdef.h"

#undef VR_DSP_CAM
#undef VC_CAM
#if   _CAM_TYPE == _CAM_SIM      /* PC Simulator                         */
#elif _CAM_TYPE == _CAM_PC       /* PC                                   */
#elif _CAM_TYPE == _CAM_TI       /* VCxxxx TI-based camera               */
  #define VC_CAM
#elif _CAM_TYPE == _CAM_BF       /* Blackfin uClinux camera              */
#elif _CAM_TYPE == _CAM_ARM      /* ARM Linux camera (i.MX27 IP camera)  */
#elif _CAM_TYPE == _CAM_VR_ARM   /* VRmagic camera - ARM Linux           */
#elif _CAM_TYPE == _CAM_VR_DSP   /* VRmagic camera - DSP                 */
  #define VR_DSP_CAM
#else
  #error io_file.h: Camera type not supported
#endif

//#include "cdef.h"

//#ifndef _TI_CAMERA   /* PC */
#if !defined(VC_CAM) && !defined(VR_DSP_CAM)   /* PC & Linux */
  #include <stdio.h>
//  #include <io.h>
  // #include <fcntl.h>
  // #include <assert.h>
  // #include <time.h>
//  #ifdef _MSC_VER
   #include <sys/types.h>
   #include <sys/stat.h>
//  #endif
#endif


#if defined(VC_CAM)
  #include "io_file1.h"
#endif  /* ]]] */

/*----------------------------------------------------------------------*/
/*                           Macros and typedefs                        */
/*----------------------------------------------------------------------*/

#ifndef CR
  #define CR  0x0d
#endif

#ifndef LF
  #define LF  0x0a
#endif

/*----------------------------------------------------------------------*/
/*                     File I/O function prototypes                     */
/*----------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

FILE *io_file_open (                                    /* IO_FILE.C    */
    char   *file,           /* [in] file name (file path)               */
    char   *mode );         /* [in] file access mode (read/write)       */
int  io_file_close (
    FILE   *fp );           /* [in] file pointer                        */
int  io_file_read (
    FILE   *fp,             /* [in] file pointer                        */
    void   *buf,            /* [out] memory buffer                      */
    int     count );        /* [in] number of bytes to read             */
int io_file_write (
    FILE   *fp,             /* [out] file pointer                       */
    void   *buf,            /* [in] memory buffer                       */
    int     count );        /* [in] number of bytes to write            */
int io_file_getc (
    FILE   *fp );           /* [in] file pointer                        */
int io_file_putc (
    FILE   *fp,             /* [in] file pointer                        */
    int     c );            /* [in] char                                */
int io_file_gets (
    FILE   *fp,             /* [in] file pointer                        */
    char   *str,            /* [out] string buffer                      */
    int     n );            /* [in] max size of str buffer              */
int io_file_puts (
    FILE   *fp,             /* [in] file pointer                        */
    char   *str );          /* [in] string                              */
unsigned int io_file_size (
    char   *path );         /* [in] file name                           */
int io_file_delete (
    char   *path );         /* [in] file name                           */
int io_file_copy (
    char   *path1,          /* [in] file name                           */
    char   *path2 );        /* [out] file name                          */
void io_file_gen_name (
    char   *fname,          /* [in] file name                           */
    int     fdrive,         /* [in] file drive: 0=FD, 1=MD              */
    int     ftype,          /* [in] file type: 0-4=exe,asc,dat,jpg,rlc  */
    char   *ofname );       /* [out] file name                          */

#ifdef __cplusplus
}
#endif

#endif /* ]]] _IO_FILE_H_LOADED */
/*@}*/
