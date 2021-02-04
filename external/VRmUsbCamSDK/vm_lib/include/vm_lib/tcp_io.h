/*----------------------------------------------------------------------*/
/**
* @name TCP_IO.H
*       Project: Stand-alone TCP I/O tool library
*
* <br>  VM_LIB is subject to the license and usage terms of VRmagic GmbH,
* <br>  which are distributed with this software.
* <br>
* <br> Header file for the stand-alone library with TCP send/receive tools.
* <br> Library files:
* <br>  TCP_SND.C : Send tools (PC & camera, portable code)
* <br>  TCP_RCV.C : Receive tools (PC & camera, portable code)
*
* @version
*       Created: 14.10.2004
* <br>  Modified:
*/
/*----------------------------------------------------------------------*/
/*@{*/

#ifndef _TCP_IO_H_LOADED    /* [[[ */
#define _TCP_IO_H_LOADED

#if defined(_TI_CAMERA)
  #include <vclib.h>        /* 'image' typedef */
#endif

#include "tcp_lib.h"    /* basic TCP I/O functions */

/****** TCP_IO.H = Header file for TCP Send/Recv Tools = 10.2004 ********/

/*----------------------------------------------------------------------*/
/*                                 Macros                               */
/*----------------------------------------------------------------------*/

/*............. Stand-alone mode */
#undef TCP_STAND_ALONE
#if !defined(_VM_CAMERA) && !defined(_VM_PC)
  #define TCP_STAND_ALONE
#endif

#ifdef _TI_CAMERA       /* TI camera    */
  #define PLATFORM "Cam"
#else
  #define PLATFORM "PC"
#endif

/*............. Block types and identifier strings: */

typedef enum                        /* Block types:                     */
{
    TCP_PTLIST_BLOCK_TYPE = 0,      /* point-list block type            */
    TCP_IMAGE_BLOCK_TYPE,           /* image block type                 */
    TCP_RESULT_BLOCK_TYPE,          /* result block type                */
    TCP_STRING_BLOCK_TYPE           /* string block type                */
} TCP_BLOCK_TYPES;

/* Block identifier strings: */
#define TCP_PTLIST_ID   "PtLi"      /* point-list block identifier      */
#define TCP_IMAGE_ID    "ImGe"      /* image block identifier           */
#define TCP_RESULT_ID   "RsLt"      /* result block identifier          */
#define TCP_STRING_ID   "StRg"      /* string block identifier          */

/* Block identifier start bytes: */
#define TCP_PTLIST_START_BYTE  (*(char *)TCP_PTLIST_ID)
#define TCP_IMAGE_START_BYTE   (*(char *)TCP_IMAGE_ID )
#define TCP_RESULT_START_BYTE  (*(char *)TCP_RESULT_ID)
#define TCP_STRING_START_BYTE  (*(char *)TCP_STRING_ID)

/*............. Send/receive tool error codes */

/* General errors */
#ifndef TCP_OK
  #define TCP_OK                      0 /* Success                           */
#endif
#define TCP_NO_MEMORY              2200 /* memory allocation error           */
//#define TCP_CONNECT_FAIL           2201 /* unable to connect to server port  */

/* Send tool errors */
#define TCP_SEND_IMAGE_HDR_ERROR    2210 /* send image - header error         */
#define TCP_SEND_IMAGE_DATA_ERROR   2211 /* send image - data error           */
#define TCP_SEND_IMAGE_REPLY_ERROR  2212 /* send image - reply error          */
#define TCP_SEND_IMAGE_TIMEOUT      2213 /* send image - wait timeout         */

#define TCP_SEND_PTLIST_HDR_ERROR   2214 /* send point-list - header error    */
#define TCP_SEND_PTLIST_DATA_ERROR  2215 /* send point-list - data error      */
#define TCP_SEND_PTLIST_REPLY_ERROR 2216 /* send point-list - reply error     */
#define TCP_SEND_PTLIST_TIMEOUT     2217 /* send point-list - wait timeout    */

#define TCP_SEND_RESULT_HDR_ERROR   2218 /* send result - header error        */
#define TCP_SEND_RESULT_DATA_ERROR  2219 /* send result - data error          */
#define TCP_SEND_RESULT_REPLY_ERROR 2220 /* send result - reply error         */
#define TCP_SEND_RESULT_TIMEOUT     2221 /* send result - wait timeout        */

#define TCP_SEND_STRING_HDR_ERROR   2222 /* send string - header error        */
#define TCP_SEND_STRING_DATA_ERROR  2223 /* send string - data error          */
#define TCP_SEND_STRING_REPLY_ERROR 2224 /* send string - reply error         */
#define TCP_SEND_STRING_TIMEOUT     2225 /* send string - wait timeout        */

/* Background receive (error) codes */
#define BGND_RCV_OFF                2300 /* bgnd recv operation is off        */
#define BGND_RCV_ON                 2301 /* bgnd recv operation in progress   */
#define BGND_RCV_END                2302 /* bgnd recv operation ended OK      */
#define BGND_RCV_HDR_OVF            2303 /* bgnd recv header overflow         */
#define BGND_RCV_NO_MEMORY          2304 /* bgnd recv data buf alloc error    */
#define BGND_RCV_TIMEOUT            2305 /* bgnd recv timeout                 */
#define BGND_RCV_INV_TYPE           2306 /* bgnd recv invalid tool type       */
#define BGND_RCV_SYNC_ERR           2307 /* bgnd recv sync. error             */

/* Receive tool errors */
#define TCP_RECV_IMAGE_NO_DATA      2310 /* recv image - no data              */
#define TCP_RECV_IMAGE_BUF_OVF      2311 /* recv image - buffer overflow      */
#define TCP_RECV_IMAGE_TIMEOUT      2312 /* recv image - timeout              */

#define TCP_RECV_PTLIST_NO_DATA     2313 /* recv point-list - no data         */
#define TCP_RECV_PTLIST_BUF_OVF     2314 /* recv point-list - buffer overflow */
#define TCP_RECV_PTLIST_TIMEOUT     2315 /* recv point-list - timeout         */

#define TCP_RECV_RESULT_NO_DATA     2316 /* recv result - no data             */
#define TCP_RECV_RESULT_BUF_OVF     2317 /* recv result - buffer overflow     */
#define TCP_RECV_RESULT_TIMEOUT     2318 /* recv result - timeout             */

#define TCP_RECV_STRING_NO_DATA     2319 /* recv string - no data             */
#define TCP_RECV_STRING_BUF_OVF     2320 /* recv string - buffer overflow     */
#define TCP_RECV_STRING_TIMEOUT     2321 /* recv string - timeout             */

#define TCP_RECV_ALLOC_ERROR        2322 /* Memory allocation error           */

/*............. Memory allocation */
#ifdef _TI_CAMERA   /* [[[ - Camera */
  #define tcp_alloc(size)     vcmalloc(((size)+3)/4)  /* size in bytes       */
  #define tcp_alloc_int(size) vcmalloc(size)          /* size in 32-bit ints */
  #define tcp_free            vcfree
#else               /* ]]] [[[ - PC */
  #define tcp_alloc(size)     malloc(size)
  #define tcp_alloc_int(size) malloc(sizeof(int) * (size))
  #define tcp_free            free
#endif  /* ]]] */

/*----------------------------------------------------------------------*/
/*                               Typedefs                               */
/*----------------------------------------------------------------------*/

typedef short          TCP_INT16;
typedef unsigned short TCP_UINT16;
typedef int            TCP_INT32;
typedef unsigned int   TCP_UINT32;

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

///*............. TCP image */
///* Keep sync. with IMAGE in VL\H\cdef.h */
//#ifndef PC_IMAGE_DEFINED    /* [[[ */
//  #define PC_IMAGE_DEFINED
//  typedef struct
//  {
//      char *st;               /* ptr to image data buffer                 */
//      int   dx;               /* image width                              */
//      int   dy;               /* image height                             */
//      int   pitch;            /* image pitch                              */
//  } IMAGE;
//#endif  /* ]]] */

/*............. Point */
/* Actual point coordinates = (x + subx/1000, y + suby/1000) */
typedef struct
{
    short x;                /* integer part of X-coordinate             */
    short subx;             /* fractional part (1/1000) of X-coordinate */

    short y;                /* integer part of Y-coordinate             */
    short suby;             /* fractional part (1/1000) of Y-coordinate */
} PTL_POINT;

/*............. Point-list item */
//#define MEM_POINT_LIST_LEN    25000L   // max length of the point-list
//#define MEM_ISVALID_PTLPOS(start, len) ((start) >= 0 && (len) >= 0 && (start) + (len) <= MEM_POINT_LIST_LEN)

#define PTL_PARAM_CNT  8 /* number of float parameters for each item */

typedef struct
{
    PTL_POINT pt;                       /* point                        */
    float     fpar[PTL_PARAM_CNT];      /* Param 0-7                    */
} PTL_ITEM;


/*----------------------------------------------------------------------*/
/*                         Image block format:                          */
/*----------------------------------------------------------------------*/
/*
*       Data       Size            Description
*  -------------  ------  -----------------------------------------------
*  I x x x             4  id    = image block identifier "Ixxx"
*  x x x x             4  type  = image type (0=bitmap, 1=JPEG)
*  x x x x             4  qual  = JPEG quality (if type=1)
*  x x x x             4  wait  = wait for receiver reply (ms), 0=no wait
*  x x x x             4  size  = image data size (dy*pitch for bitmap)
*  x x x x             4  dx    = image width (<= pitch, used if type=0)
*  x x x x             4  dy    = image height (used if type=0)
*  x x x x             4  pitch = image pitch  (used if type=0)
*  x x x . . . x   pitch  Image row 0    |
*  x x x . . . x   pitch  Image row 1    | image data bytes if type=0 (dy rows)
*  . . . . . . .   . . .  . . . . . . .  | or JPEG data it type=1
*  x x x . . . x   pitch  Image row dy-1 |
*  -------------
*      pitch
*
*
* where:
*   id    = 4-byte image block identifier "Ixxx"
*   type  = 4-byte image type:
*             0 : bitmap (size = dy*pitch)
*             1 : JPEG (size = JPEG file size, dx,dy and pitch are ignored)
*                 (currently not supported)
*   qual  = 4-byte JPEG quality [1,100] used when type=1
*   wait  = 4-byte wait flag, which directs the sender to wait for end
*           of the transfer:
*             0 : the sender does not wait for a reply byte from the
*                 receiver
*            >0 : the sender waits "wait" ms for a reply byte from the
*                 receiver, which should be sent after end of transfer
*   size  = 4-byte image size (# of data bytes), equal to:
*             dy*pitch       : for bitmap image type type=0
*             JPEG file size : for JPEG type type=1
*   dx    = 4-byte image width (used for bitmap type type=0)
*   dy    = 4-byte image height (used for bitmap type type=0)
*   pitch = 4-byte image pitch (used for bitmap type type=0, default = 768 for
*           VC2028/VC2038 cameras)
*
* Byte order: little endian (default for TI camera and PC).
* NOTE: Currently type 0 (bitmap) is supported only !
*/

/*............. Image block header */
typedef struct {
    int           id;       /* image block identifier                   */
    int           type;     /* image type:                              */
                            /*   0 : bitmap                             */
                            /*   1 : JPEG                               */
    int           qual;     /* JPEG quality [1,100] (if type = 1)       */
    int           wait;     /* wait reply time:                         */
                            /*   0 : don't wait for end of transfer     */
                            /*  >0 : sender waits "wait" ms for a       */
                            /*       reply byte from the receiver       */
    int           size;     /* number of data bytes after header:       */
                            /*   dy*pitch       : if type = 0           */
                            /*   JPEG file size : if type = 1           */
    int           dx;       /* image width (used if type = 0)           */
    int           dy;       /* image height (used if type = 0)          */
    int           pitch;    /* image pitch (used if type = 0)           */
} TCP_IMAGE_HDR;

/*----------------------------------------------------------------------*/
/*                      Point-list block format:                        */
/*----------------------------------------------------------------------*/
/*
*
*       Data      Size            Description
*  -------------  -----  -------------------------------
*  P x x x           4   id      = point-list block identifier "Pxxx"
*  l l l l           4   ptl_len = number of point-list items
*  w w w w           4   wait    = wait for receiver reply (ms), 0=no wait
*  x x x . . . x    40   Item 0 (Point, Param0,...,Param7)|
*  x x x . . . x    40   Item 1 (Point, Param0,...,Param7)|
*  . . . . . . .  . . .  . . . . . . . . . . . . . . . .  |
*  x x x . . . x    40   Item ptl_len-1                   |
*
* Each item is a PTL_ITEM structure. Should be synchronized with the
* point-list item in vm_mem.h - the MEM_POINT_ITEM structure.
*
* Byte order: little endian (default for TI camera and PC).
*
*/

/*............. Point-list block header */
typedef struct {
    int           id;       /* point-list block identifier              */
    int           ptl_len;  /* point-list length in # of items          */
    int           wait;     /* wait reply time:                         */
                            /*   0 : don't wait for end of transfer     */
                            /*  >0 : sender waits "wait" ms for a       */
                            /*       reply byte from the receiver       */
} TCP_PTLIST_HDR;

/*----------------------------------------------------------------------*/
/*                          Result block format:                        */
/*----------------------------------------------------------------------*/
/*
*       Data        Size                      Description
*  -------------  ---------  ---------------------------------------------
*  R x x x           4       id        = result block identifier "Rxxx"
*  t t t t           4       type      = result type
*  f f f f           4       data_fmt  = data format
*  w w w w           4       wait      = wait for receiver reply (ms), 0=no wait
*  s s s s           4       data_size = # of next data bytes (k*4)
*  x x x . . . x  data_size  data bytes
*
* where:
*   id        = 4-byte result block identifier "Rxxx"
*   type      = 4-byte result type:
*                 0 : float
*                 1 : point PTL_POINT
*                 2 : 16-bit integer TCP_INT16
*                 3 : 32-bit integer TCP_INT32
*   data_fmt  = 4-byte data format:
*                 0 : hex
*                 1 : binary (default)
*                 2 : ASCII
*                 3 : ASCII prompt
*   wait       = 4-byte wait flag, which specifies if the sender should
*                wait for the end of the transfer:
*                  0 : the sender does not wait for a reply byte from the
*                      receiver
*                 >0 : the sender waits "wait" ms for a reply byte from the
*                      receiver, which should be sent after end of transfer
*   data_size  = 4-byte # of data bytes after header (faster I/O on camera
*                if multiple of 4)
*
* Note:       Currently only binary data format is supported !
* Byte order: Little endian (default for TI camera and PC).
*
*/

/*............. Result block header */
typedef struct {
    int      id;            /* result block identifier "Rxxx"           */
    int      type;          /* result type:                             */
                            /*   0 : float                              */
                            /*   1 : point PTL_POINT                    */
                            /*   2 : 16-bit integer TCP_INT16           */
                            /*   3 : 32-bit integer TCP_INT32           */
    int      data_fmt;      /* format of data bytes:                    */
                            /*   0 : hex                                */
                            /*   1 : binary (default)                   */
                            /*   2 : ASCII                              */
                            /*   3 : ASCII prompt                       */
    int      wait;          /* wait reply time:                         */
                            /*   0 : don't wait for end of transfer     */
                            /*  >0 : sender waits "wait" ms for a       */
                            /*       reply byte from the receiver       */
    int      data_size;     /* data size in bytes                       */

} TCP_RESULT_HDR;


/*----------------------------------------------------------------------*/
/*                          String block format:                        */
/*----------------------------------------------------------------------*/
/*
*       Data        Size                      Description
*  -------------  ---------  ---------------------------------------------
*  S x x x           4       id   = string block identifier "Sxxx"
*  w w w w           4       wait = wait for receiver reply (ms), 0=no wait
*  s s s s           4       data_size = # of next data bytes
*  x x x . . . x  data_size  data bytes
*
* where:
*   id        = 4-byte string block identifier "Sxxx"
*   wait      = 4-byte wait flag, which specifies if the sender should
*               wait for the end of the transfer:
*                  0 : the sender does not wait for a reply byte from the
*                      receiver
*                 >0 : the sender waits "wait" ms for a reply byte from the
*                      receiver, which should be sent after end of transfer
*   data_size = 4-byte # of data bytes after header (faster I/O on camera
*               when multiple of 4)
*
* Byte order: Little endian (default for TI camera and PC).
*
*/

/*............. String block header */
typedef struct {
    int      id;            /* string block identifier "Sxxx"           */
    int      wait;          /* wait reply time:                         */
                            /*   0 : don't wait for end of transfer     */
                            /*  >0 : sender waits "wait" ms for a       */
                            /*       reply byte from the receiver       */
    int      data_size;     /* data size in bytes                       */

} TCP_STRING_HDR;

/*----------------------------------------------------------------------*/
/*                          Send/receive binary                         */
/*----------------------------------------------------------------------*/
/*
* Data is sent and received as it is, without any header and protokol.
* See:
*   TCP_SendBinary()
*   TCP_RecvBinary()
*/

/*----------------------------------------------------------------------*/
/*                       I/O tool function prototypes                   */
/*----------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/*............. Send tools */
int TCP_SendImage (                                     /* TCP_SND.C    */
    TCP_SOCK  *sock,            /* [in] TCP socket                      */
    image     *img,             /* [in] image                           */
    int        type,            /* [in] type of image block to send     */
    int        qual,            /* [in] JPEG quality (type=1)           */
    int        wait );          /* [in] wait flag (>0:wait xfer end)    */
int TCP_SendPtlist (
    TCP_SOCK      *sock,        /* [in] TCP socket                      */
    unsigned char *ptl_buf,     /* [in] point-list buffer               */
    int            ptl_len,     /* [in] point-list length (item #)      */
    int            wait );      /* [in] wait flag (>0:wait xfer end)    */
int TCP_SendResult (
    TCP_SOCK   *sock,           /* [in] TCP socket                      */
    void       *res_buf,        /* [in] result buffer                   */
    int         type,           /* [in] result type                     */
    int         data_fmt,       /* [in] data format                     */
    int         wait );         /* [in] wait flag (>0:wait xfer end)    */
int TCP_SendString (
    TCP_SOCK   *sock,           /* [in] TCP socket                      */
    char       *str_buf,        /* [in] string buffer                   */
    int         str_size,       /* [in] string buffer size              */
    int         wait );         /* [in] wait flag (>0:wait xfer end)    */
int TCP_SendRawString (
    TCP_SOCK   *sock,           /* [in] TCP socket                      */
    char       *str_buf,        /* [in] string buffer                   */
    int         str_size );     /* [in] string buffer size              */

#define TCP_SendBinary TCP_SendBlock
/*
* int TCP_SendBinary (
*   TCP_SOCK      *sock,        // [in] socket
*   unsigned char *buf,         // [in] buffer
*   int            cnt );       // [in] # of bytes to send
*
* Return code:
*   >=0            : Number of sent bytes (success)
*   TCP_SEND_ERROR : Socket error (connection broken)
*/

/*............. Receive tools */
int TCP_GetBlockId (                                    /* TCP_RCV.C    */
    TCP_SOCK   *sock );         /* [i/o] TCP socket                     */
void TCP_ResetBlockId (
    TCP_SOCK   *sock );         /* [i/o] TCP socket                     */
int TCP_RecvImage (
    TCP_SOCK  *sock,            /* [in] TCP socket                      */
    image     *img,             /* [i/o] image                          */
    int        img_buf_size,    /* [in] size of "img" buffer "img_buf"  */
    int        wait,            /* [in] wait time (0:wait forever)      */
    int       *type,            /* [out] type of received image         */
    int       *qual );          /* [out] JPEG quality (if type=1)       */
int TCP_RecvImageA (
    TCP_SOCK  *sock,            /* [in] TCP socket                      */
    image     *img,             /* [out] destination image              */
    int        wait,            /* [in] wait time (0:wait forever)      */
    int       *type,            /* [out] type of received image         */
    int       *qual );          /* [out] JPEG quality (if type=1)       */
int TCP_RecvPtlist (
    TCP_SOCK      *sock,        /* [in] TCP socket                      */
    unsigned char *ptl_buf,     /* [out] point-list buffer              */
    int           *ptl_len,     /* [i/o] point-list length (# of items) */
    int            wait );      /* [in] wait time (0:wait forever)      */
int TCP_RecvResult (
    TCP_SOCK      *sock,        /* [in] TCP socket                      */
    void          *res_buf,     /* [out] result buffer                  */
    int           *data_size,   /* [i/o] result buffer size             */
    int           *type,        /* [out] result type                    */
    int            wait );      /* [in] wait time (0:wait forever)      */
int TCP_RecvString (
    TCP_SOCK      *sock,        /* [in] TCP socket                      */
    char          *str_buf,     /* [out] string buffer                  */
    int           *str_size,    /* [i/o] string buffer size             */
    int            wait );      /* [in] wait time (0:wait forever)      */

#ifdef __cplusplus
};
#endif

#define TCP_RecvBinary TCP_RecvBlock
/*
* int TCP_RecvBinary (
*   TCP_SOCK      *sock,        // [in] socket
*   unsigned char *buf,         // [out] buffer
*   int            cnt,         // [in] # of bytes to receive
*   int            wait )       // [in] wait time in ms
*
* Return code:
*   >=0            : Number of received bytes (rc < cnt : timeout error )
*   TCP_RECV_ERROR : Socket error (connection broken)
*/


#endif  /* ]]] - _TCP_IO_H_LOADED*/
/*@}*/
