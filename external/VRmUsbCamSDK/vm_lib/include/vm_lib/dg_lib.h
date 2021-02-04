/*----------------------------------------------------------------------*/
/**
* @name DG_LIB.H
*       Diagnostic and dump library
*
* <br>  VM_LIB is subject to the license and usage terms of VRmagic GmbH,
* <br>  which are distributed with this software.
* <br>
* <br>  Header file for the diagnostic and dump library.
*
* @version
*       Created:  08.12.2005
* <br>  Modified:
*/
/*----------------------------------------------------------------------*/
/*@{*/

#ifndef _DG_LIB_H_LOADED   /* [[[ */
#define _DG_LIB_H_LOADED

#include "cdef.h"

/*----------------------------------------------------------------------*/
/*                           Macros and typedefs                        */
/*----------------------------------------------------------------------*/

/*............. DG_ASSERT macro */
/*
* Print message when assertion fails:
*   "DG_ASSERT: <expr> <file>(<line>)"
*
* Arguments:
*   expr - [in] expression that evaluates to false
*   file - [in] source file name
*   line - [in] source file line number
*/

/*----------------------------------------------------------------------*/
#if _CAM_TYPE == _CAM_SIM   /* [[[ - PC Simulator                       */
/*----------------------------------------------------------------------*/
void dg_trace(char *message);
void dg_trace1(char *message, int n1);
void dg_trace2(char *message, int n1, int n2);
void dg_trace3(char *message, int n1, int n2, int n3);
void dg_trace4(char *message, int n1, int n2, int n3, int n4);

#ifdef _DEBUG   /* [[[ */
  #ifndef TRACE
    #define TRACE(msg)                  dg_trace(msg)
    #define TRACE1(msg, n1)             dg_trace1(msg, n1)
    #define TRACE2(msg, n1, n2)         dg_trace2(msg, n1, n2)
    #define TRACE3(msg, n1, n2, n3)     dg_trace3(msg, n1, n2, n3)
    #define TRACE4(msg, n1, n2, n3, n4) dg_trace4(msg, n1, n2, n3, n4)
  #endif

  void dg_assert(char* expr, char* file, unsigned int line);
  #ifndef DG_ASSERT
    #define DG_ASSERT(exp) (void)( (exp) || (dg_assert(#exp, __FILE__, __LINE__), 0) )
  #endif

#else  /* ]]] [[[ */
  #ifndef TRACE
    #define TRACE(msg)                  ((void)0)
    #define TRACE1(msg, n1)             ((void)0)
    #define TRACE2(msg, n1, n2)         ((void)0)
    #define TRACE3(msg, n1, n2, n3)     ((void)0)
    #define TRACE4(msg, n1, n2, n3, n4) ((void)0)
  #endif

  #ifndef DG_ASSERT
    #define DG_ASSERT(exp)    ((void)0)
  #endif
#endif  /* ]]] */

/*----------------------------------------------------------------------*/
#else                       /* ]]] [[[ - all other platforms            */
/*----------------------------------------------------------------------*/
#ifndef TRACE
  #define TRACE     printf
  #define TRACE1    printf
  #define TRACE2    printf
  #define TRACE3    printf
  #define TRACE4    printf
#endif

#ifdef _DEBUG
  #define DG_ASSERT(exp) (void)((exp) || (dg_assert(#exp,__FILE__,__LINE__),0))
#else
  #define DG_ASSERT(exp) ((void)0)
#endif

#endif  /* ]]] */


/*----------------------------------------------------------------------*/
/*                           Function prototypes                        */
/*----------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

int  dg_open_dumpfile (                                 /* DG_LIB.C     */
    char   *dfile );    /* [in] dump file name                          */
void dg_close_dumpfile ();
void dg_dump_str (
    char   *str );      /* [in] string to dump                          */
void  dg_dump_buf2str (
    void *buf,          /* [in] buffer to dump                          */
    int   buf_type,     /* [in] type of buffer elements                 */
    int   n,            /* [in] # of buffer elements                    */
    int   elm_size,     /* [in] element size (# of fields per element)  */
    char *str );        /* [out] string                                 */
void  dg_dump_buf (
    void *buf,          /* [in] buffer to dump                          */
    int   buf_type,     /* [in] type of buffer elements                 */
    int   n,            /* [in] # of buffer elements                    */
    int   elm_size,     /* [in] element size (# of fields per element)  */
    int   line_size,    /* [in] # of printed elements per line          */
    int   mode );       /* [in] print mode                              */
void dg_dump_intbuf (
    int  *buf,          /* [in] integer buffer                          */
    int   n,            /* [in] number of elements to dump              */
    int   line_size );  /* [in] # of printed elements per line          */
void dg_dump_xybuf (
    int  *buf,          /* [in] integer buffer: (x,y) (x,y) ...         */
    int   n,            /* [in] number of points (x,y) in buf           */
    int   line_size );  /* [in] # of printed elements per line          */
int  dg_dump_img (
    DR_IMAGE *img,      /* [in] image                                   */
    int       x0,       /* [in] x-coord. of top/left rect corner        */
    int       y0,       /* [in] y-coord. of top/left rect corner        */
    int       dx,       /* [in] rectangle width                         */
    int       dy );     /* [in] rectangle height                        */
int  dg_create_img (
    DR_IMAGE *img,      /* [out] image                                  */
    int       dx,       /* [in] image width                             */
    int       dy,       /* [in] image height                            */
    int       pitch );  /* [in] image pitch                             */
void dg_close_img (
    DR_IMAGE *img );    /* [i/o] image                                  */
void dg_fill_img (
    DR_IMAGE *img,      /* [out] image                                  */
    int       val );    /* [in] value to fill                           */
int  dg_dump_xybuf_img (
    int  *xybuf,        /* [in] integer buffer: (x,y) (x,y) ...         */
    int   n );          /* [in] number of points (x,y) in buf           */

void dg_assert (                                        /* DG_ASSER.C   */
    char   *expr,           /* [in] expression that evaluates to false  */
    char   *file,           /* [in] source file name                    */
    unsigned int line );    /* [in] source file line number             */

void dg_disp_str (                                      /* DG_DISPS.C   */
    char  *str );       /* [in] string to display                       */

#ifdef __cplusplus
}
#endif

#endif /* ]]] _DG_LIB_H_LOADED */
/*@}*/
