/*----------------------------------------------------------------------*/
/**
* @name IO_SER.H
*       Serial I/O library
*
* <br>  VM_LIB is subject to the license and usage terms of VRmagic GmbH,
* <br>  which are distributed with this software.
* <br>
* <br>  This file contains prototypes of portable (TI camera + PC)
*       functions for serial I/O.
*
* @version
*       Created:   10.10.2005
* <br>  Modified:  29.03.2006:
* <br>            Added support for all types of TI-based cameras
*                 with RS232 and RS422 ports.
*/
/*----------------------------------------------------------------------*/
/*@{*/

#ifndef _IO_SER_H
#define _IO_SER_H

//#if defined(_VM_PC) || defined(_TI_CAMERA)  /* [[[ */

#if defined(_TI_CAMERA)
  #include <vcrt.h>
#elif defined(_MSC_VER)     /* PC */
  #include <windows.h>
#else                       /* other (Linux) */
#endif

/****** IO_SER.H = Portable serial I/O functions = 03.2004 *************/
/*----------------------------------------------------------------------*/
/*                           Macros and typdefs                         */
/*----------------------------------------------------------------------*/
//#define IO_SER_INVALID_HANDLE   NULL

/* Serial port handle */
typedef void * IO_SER_HND;

#if !defined(GETCH) || !defined(KBHIT)  /* [[[ */
  #undef KBHIT
  #undef GETCH
#if defined(_TI_CAMERA)     /* TI camera */
//  typedef FILE * IO_SER_HANDLE;
  #define KBHIT()  (-1 != rbempty())
  #define GETCH    rs232rcv
#elif defined(_MSC_VER)     /* PC */
  #define KBHIT()  _kbhit()
  #define GETCH    _getch
#else                       /* other (Linux) */
//  #define IO_SER_HANDLE  HANDLE
  #define KBHIT()  kbhit()
  #define GETCH    getchar
#endif
#endif  /* ]]] */

#ifndef ONESTOPBIT
  #define ONESTOPBIT          0
  #define ONE5STOPBITS        1
  #define TWOSTOPBITS         2
#endif

#ifndef  NOPARITY
  #define NOPARITY            0
  #define ODDPARITY           1
  #define EVENPARITY          2
  #define MARKPARITY          3
  #define SPACEPARITY         4
#endif

/*................ Serial errors */
/* Keep synchronized with IO_ERR.H, IO_BOX.H, TCP_LIB.H, TCP_IO.H */
#define IO_SER_OPEN_ERROR            2030   /* Serial open error        */
#define IO_SER_GET_STATE_ERROR       2031   /* Serial get state error   */
#define IO_SER_SET_STATE_ERROR       2032   /* Serial set state error   */
#define IO_SER_GET_TIMEOUT_ERROR     2033   /* Serial get timeouts err  */
#define IO_SER_SET_TIMEOUT_ERROR     2034   /* Serial set timeouts err  */
#define IO_SER_DEVICE_NOT_OPEN       2035   /* Serial device not opened */
#define IO_SER_CLOSE_ERROR           2036   /* Serial close error       */
#define IO_SER_FLUSH_ERROR           2037   /* Serial flush error       */
#define IO_SER_PURGE_ERROR           2038   /* Serial purge error       */
#define IO_SER_INVALID_ARG           2039   /* Invalid argument         */
#define IO_SER_READ_ERROR            2040   /* Serial read error        */
#define IO_SER_WRITE_ERROR           2041   /* Serial write error       */
#define IO_SER_WRITE_TIMEOUT_ERROR   2042   /* Serial write timeout     */
#define IO_SER_READ_TIMEOUT_ERROR    2043   /* Serial read timeout      */
#define IO_SER_HANDLE_ERROR          2044   /* Invalid serial handle    */
#define IO_SER_RS422_FIFO_OVF        2045   /* RS422 FIFO overflow      */
#define IO_SER_RTS_ERROR             2046   /* Set RTS error            */
#define IO_SER_DTR_ERROR             2047   /* Set DTR error            */
#define IO_SER_NO_MEMORY             2048   /* Memory allocation error  */

/*----------------------------------------------------------------------*/
/*                         Function prototypes                          */
/*----------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

int io_ser_open (                                       /* IO_SER.C     */
    IO_SER_HND  *shnd,      /* [out] serial port handle                 */
    int          port,      /* [in] COM port: 0-7=COM1-COM8             */
    int          baud_rate, /* [in] baud rate (<0 : def=9600)           */
    int          stop_bits, /* [in] # of stop bits: 0-2=1/1.5/2         */
    int          parity,    /* [in] parity: 0-4=no/odd/even/mark/space  */
    int          word_size, /* [in] word size: 4-8 (<=0 : def=8)        */
    int          bus_mode );/* [in] bus mode: 0=RS232, 1=RS485          */

int  io_ser_close (
    IO_SER_HND  *shnd );    /* [i/o] serial port handle                 */

int io_ser_setbaud (
    IO_SER_HND shnd,        /* [in] serial port handle                  */
    int        baud );      /* [in] baud rate (1200 - 115200)           */

int io_ser_setrts (
    IO_SER_HND shnd,        /* [in] serial port handle                  */
    int        rts );       /* [in] RTS value: 0=low, 1=high            */

int io_ser_setdtr (
    IO_SER_HND shnd,        /* [in] serial port handle                  */
    int        dtr );       /* [in] DTR value: 0=low, 1=high            */

//void io_ser_set_busmode (
//    int    bus_mode );      /* [in] bus mode: 0=RS232, 1=RS485          */

int io_ser_send_byte (
    IO_SER_HND shnd,        /* [in] serial port handle                  */
    unsigned char c );      /* [in] byte to send                        */

int io_ser_recv_byte (
    IO_SER_HND shnd );      /* [in] serial port handle                  */

int io_ser_rbready (
    IO_SER_HND shnd );      /* [in] serial port handle                  */

void io_ser_flush (
    IO_SER_HND shnd,        /* [in] serial port handle                  */
    int        time_out );  /* [in] max time in ms to read bytes        */

void io_ser_encoder_mode (
    int mode );             /* [in] encoder mode                        */

#ifdef __cplusplus
}
#endif

//#endif  /* ]]] - PC and TI camera */

#endif  /* _IO_SER_H */
/*@}*/
