/****** TCP_LIB.H = Include file for TCP_LIB functions = 10.2004 ********/
/*
*
*  VM_LIB is subject to the license and usage terms of VRmagic GmbH,
*  which are distributed with this software.
*
* Header file for basic TCP library functions - common for PC (Windows),
* TI camera and LINUX (CANCamBF uClinux camera). Include this header in
* yor code for TCP communication.
*
* Link programs with the library wsock32.lib on PC.
*
* ----------------------------------------------------------------------
*            Example for connection as server (one data socket):
* ----------------------------------------------------------------------
* #include "tcp_lib.h"
* #define CONN_TIME_OUT ...     // connection wait time in ms
*
*   TCP_SOCK ListenSock;        // listen socket
*   TCP_SOCK DataSock;          // data socket
*   unsigned long time;
*   int      rc;
*   . . . . . . . . . . .
*   // TCP initialization:
*   TCP_Init();
*   TCP_DataSockReset(&DataSock,1);
*   TCP_DataSockReset(&ListenSock,1);
*
*   TCP_Listen(&ListenSock,2000);
*   . . . . . . . . . . . . . . .
*
*   // Wait for connection with remote client:
*   time = get_time();
*   for(;;)
*   {
*     TCP_AcceptConnection(&ListenSock,&DataSock,1);
*     if(TCP_GetConnectionState(&DataSock)) break;
*
*     // Code for connection timeout (if needed):
*     if(get_time() - time >= CONN_TIME_OUT)
*     {
*       . . . . . . . .     // connection time out
*     }
*   }
*   . . . . . . . . . . .
*
*   // Send and receive data in loop...
*   for(;;)
*   {
*     // Accept new or broken connection:
*     rc = TCP_AcceptConnection(&ListenSock,&DataSock,1);
*     if(rc != TCP_OK)
*     {
*       . . . . . . .   // connection broken
*     }
*     . . . . . . . . . . . . . .
*     // Call send and receive functions:
*     TCP_SendByte(&DataSock,byte);
*     TCP_RecvByte(&DataSock);
*     TCP_WaitByte(&DataSock,wait);
*     TCP_SendBlock(&DataSock,buf,cnt);
*     TCP_RecvBlock(&DataSock,buf,cnt,wait);
*   }
*
*   // Terminate TCP connection:
*   TCP_Disconnect(&ListenSock);
*   TCP_Disconnect(&DataSock);
*   TCP_Close();
*
* ----------------------------------------------------------------------
*            Example for connection as client (one data socket):
* ----------------------------------------------------------------------
* #include "tcp_lib.h"
* #define CONN_TIME_OUT ...             // connection wait time in ms
*
*   TCP_SOCK DataSock;                  // data socket
*   char    *ip_addr = "xxx.xxx.x.xx";  // server's IP address
*   int      rc;
*   . . . . . . . . . . .
*   // TCP initialization:
*   TCP_Init();
*   TCP_DataSockReset(&DataSock,1);
*   TCP_Connect(&DataSock,ip_addr,2000,CONN_TIME_OUT);
*   . . . . . . . . . . . . . . . . .
*
*   // Send/receive loop:
*   for(;;)
*   {
*     . . . . . . . . . . . . . .
*     // Call send and receive functions:
*     TCP_SendByte(&DataSock,byte);
*     TCP_RecvByte(&DataSock);
*     TCP_WaitByte(&DataSock,wait);
*     TCP_SendBlock(&DataSock,buf,cnt);
*     TCP_RecvBlock(&DataSock,buf,cnt,wait);
*     . . . . . . . . . . . . . .
*   }
*
*   // Terminate TCP connection:
*   TCP_Disconnect(&DataSock);
*   TCP_Close();
*
*
* NOTE: Checks of function return codes are omitted for clarity, but
*       should be present in the real code !
*/
/*----------------------------------------------------------------------*/

#ifndef _TCP_LIB_H_LOADED    /* [[[ */
#define _TCP_LIB_H_LOADED

#ifdef _TI_CAMERA       /* TI camera   */
  /* #include <sysvar.h>*/
  /* #include <vcrt.h>*/
#else                   /* PC & other platforms */
  /* #include "stdafx.h"*/
  /* #include "camsocket.h"*/
  /* #include <winsock2.h>*/
  #if defined(_MSC_VER)     /* PC Windows */
    #include <winsock.h>
  #else                     /* LINUX */
//    #include <errno.h>
//    #include <sys/time.h>
//    #include <time.h>
//    #include <sys/select.h>
//    #include <sys/signal.h>
//    #include <sys/socket.h>
//    #include <arpa/inet.h>
//    #include <string.h>
  #endif
#endif

/*----------------------------------------------------------------------*/
/*                                 Macros                               */
/*----------------------------------------------------------------------*/
/*
 * WinSock 2 extension -- manifest constants for shutdown()
 */
#ifndef SD_RECEIVE
  #define SD_RECEIVE      0x00
#endif
#ifndef SD_SEND
  #define SD_SEND         0x01
#endif
#ifndef SD_BOTH
  #define SD_BOTH         0x02
#endif

#ifndef CR
  #define CR   0x0D       // carriage return
#endif

#ifndef LF
  #define LF   0x0A       // line feed
#endif

#ifndef BS
  #define BS   0x08       // backspace
#endif

/*............. Default TCP/IP server port on camera */
#define CAM_SERVER_PORT   2000    /* waiting at port for any connection */

/*............. Timeouts in ms */
#ifdef _TI_CAMERA   /* [[[ - TI camera  */
  #define TCP_WAITBYTE_TIMEOUT  300
  #define TCP_RECVBLOCK_TIMEOUT 800
#else               /* ]]] [[[ - PC     */
  #define TCP_WAITBYTE_TIMEOUT  300
  #define TCP_RECVBLOCK_TIMEOUT 100   /* 3000 */
  #define TCP_READ_ECHO_TIMEOUT 300   /* wait time for camera echo (50)   */
#endif  /* ]]] */

// #define TCP_CONNECT_TIMEOUT   800   /* 500 */
// #define TCP_SENDBYTE_TIMEOUT    2

/*............. Error codes */
#define TCP_OK                          0

/* Camera errors: */
#define TCP_STREAM_BIND_ERROR       -2101

/* PC errors: */
#define TCP_MFCINIT_ERROR           -2102
#define TCP_AFXSOCKETINIT_ERROR     -2103

/* Common errors: */
#define TCP_SOCKET_CREATE_ERROR     -2104
#define TCP_LISTEN_FAIL_ERROR       -2105
#define TCP_ACCEPT_FAIL_ERROR       -2106
#define TCP_NO_CONNECTION_ERROR     -2107
#define TCP_INVALID_IP_ERROR        -2108
#define TCP_CONNECT_FAIL_ERROR      -2109
#define TCP_NO_DATA_SOCKETS_ERROR   -2110
#define TCP_SEND_BYTE_ERROR         -2111
#define TCP_RECV_BYTE_ERROR         -2112
#define TCP_SEND_ERROR              -2113
#define TCP_RECV_ERROR              -2114
#define TCP_SEND_BLOCK_ERROR        -2115
#define TCP_RECV_BLOCK_ERROR        -2116
#define TCP_GETSOCKNAME_ERROR       -2117
#define TCP_GETPEERNAME_ERROR       -2118
#define TCP_COPY_PROT_ERROR         -2130   /* copy protection error   */
#define TCP_SETSOCKOPT_ERROR        -2131   /* set socket option error */
#define TCP_CONNECT_TIMEOUT_ERROR   -2132   /* connect timeout error   */
#define TCP_GETSOCKOPT_ERROR        -2133   /* get socket option error */

/*----------------------------------------------------------------------*/
/*                     Background receive operations                    */
/*----------------------------------------------------------------------*/
/*
* The receive functions TCP_Recv... can work in background mode. The basic
* idea is to receive tool data in background mode in a way, which does not
* decrease the system performance.
*
* Once a tool has received a valid header block (i.e. the transfer is started)
* the tool waits no more than "wait" ms and exits even when all tool's data
* has not arrived yet. The tool exits with OK in the next call(s) when
* all data is been received. During this transfer all other receive tools
* read tool's data in background mode - they just fill the tool's receive
* buffer. The total wait time must not exceed a given timeout value, set
* by TCP_SetRecvMode() for each receive tool (see recv_timeout[]).
*
* The background receive transfer is controlled by the "bgnd_rcv" handle
* in the TCP_SOCK socket structure (see TCP_LIB.H).
*
* For a given socket one background transfer (for one tool) can be in progress
* at a time.
*/

/*............. Type of current background receive operation (recv_type) */
typedef enum
{
    BGND_RCV_NONE = 0,  /* no background receive operation                */
    BGND_RCV_PTLIST,    /* point-list bgnd receive operation in progress  */
    BGND_RCV_IMAGE,     /* image bgnd receive operation in progress       */
    BGND_RCV_RESULT,    /* result bgnd receive operation in progress      */
    BGND_RCV_STRING,    /* string bgnd receive operation in progress      */
    BGND_RCV_OPCNT      /* # of bgnd receive operation types              */
} BGND_RCV_TYPES;

///*............. Background receive modes (OR of bits) */
///* TCP_SetRecvMode(mode) and sock->bgnd_mode */
//#define BGND_RCV_PTLIST_ENB  0x01   /* point-list background receive mode   */
//#define BGND_RCV_IMAGE_ENB   0x02   /* image background receive mode        */
//#define BGND_RCV_RESULT_ENB  0x04   /* result background receive mode       */
//#define BGND_RCV_STRING_ENB  0x08   /* string background receive mode       */


/*............. Max header size in bytes */
/* keep greater than all header sizes, defined in TCP_IO.H  */
#define RECV_HDR_SIZE  (12*sizeof(int))

/*............. Background receive control structure (handle) */
/*
* A background receive operation for a given tool is enabled by a non-zero
* value in the respective recv_timeout[] element.
*
* The current state of the background receive operation is specified by:
*   recv_start != 0 : bgnd receive operation in progress (recv_data != NULL)
*               = 0 : bgnd receive operation terminated (recv_data = NULL)
*
*/
typedef struct
{
    int     recv_start;     /* type of started receive operation:           */
                            /*   BGND_RCV_OFF     : bgnd receive is off     */
                            /*   BGND_RCV_PTLIST  : point-list bgnd rcv     */
                            /*   BGND_RCV_IMAGE   : image bgnd rcv          */
                            /*   BGND_RCV_RESULT  : result bgnd rcv         */
                            /*   BGND_RCV_STRING  : string bgnd rcv         */
    int     recv_hdr[RECV_HDR_SIZE/sizeof(int)];
                            /* current receive block header buffer with     */
                            /* format specified by 'recv_start':            */
                            /*   TCP_IMAGE_HDR    : point-list header       */
                            /*   TCP_PTLIST_HDR   : image header            */
                            /*   TCP_RESULT_HDR   : result header           */
                            /*   TCP_STRING_HDR   : string header           */
    int     recv_hdr_size;  /* block header buffer size                     */
    char   *recv_data_buf;  /* ptr to current block data buffer (NULL=none) */
    int     recv_data_size; /* size of recv_data_buf buffer = # of bytes to */
                            /* get (equal to the data size in hdr)          */
    int     recv_data_cnt;  /* # of received data bytes [0 to data_size]    */
    int     recv_time;      /* current accumulated receive time in ms       */
                            /* (recv_time <= recv_timeout[i])               */
    int     recv_timeout[BGND_RCV_OPCNT];
                            /* enables for tool bgnd receive operations &   */
                            /* timeouts for each receive operation in ms    */
                            /* (max transfer time before timeout error)     */
    int     recv_mode;      /* special bgnd receive mode in TCP_Recv... functions:   */
                            /*  0 = normal: wait for block ID                        */
                            /*  1 = special: don't wait when block ID is not present */
} BGND_RCV;

/*----------------------------------------------------------------------*/
/*                                TCP socket                            */
/*----------------------------------------------------------------------*/
typedef struct
{
    unsigned int ip_addr;           /* IP addr of remote peer           */
    unsigned int remote_port;       /* port of remote peer              */
    unsigned int port;              /* local port                       */

#ifdef _TI_CAMERA
    unsigned int socket;            /* camera socket                    */
#else
  #if defined(_MSC_VER)     /* PC Windows */
    SOCKET socket;                  /* PC socket                        */
  #else                     /* LINUX */
    unsigned int socket;            /* LINUX socket                     */
  #endif
#endif

    unsigned int connection_exists; /* flag for existing connection     */

/* TCP I/O tools: */
    int     block_id;               /* block identifier                 */
    int     block_type;             /* block type: -1:N/A(consumed)     */

/* Background receive control */
    BGND_RCV bgnd_rcv;              /* background receive handle        */

} TCP_SOCK;


/*----------------------------------------------------------------------*/
/*                          Function prototypes                         */
/*----------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/*............. General TCP functions - common for PC and camera */
/*
* TCP_LIB.C    : camera TCP library
* TCP_LIB.CPP  : PC TCP library
*/
/* Control functions */
int TCP_Init ();
void TCP_Close ();
void TCP_DataSockReset (
    TCP_SOCK *data_sock,        /* input/output data socket buffer      */
    int       data_sock_cnt );  /* input # of data sockets              */
int TCP_Listen (
    TCP_SOCK *list_sock,        // input/output listen socket
    int       port );           // input port
int TCP_Connect (
    TCP_SOCK   *Sock,           // input/output data socket
    char       *IP_Addr,        // input string with IP address (NULL=default)
    int         Port,           // connect port
    int         wait_time );    // input wait time in ms
void TCP_Disconnect (
    TCP_SOCK *sock );           // input socket
int TCP_AcceptConnection (
    TCP_SOCK *listen_sock,      /* input/output listen socket           */
    TCP_SOCK *data_sock,        /* input/output data socket buffer      */
    int       data_sock_cnt );  /* input # of data sockets              */
int TCP_GetConnectionState (
    TCP_SOCK *sock );           // input data socket

/* Data transfer functions */
int TCP_SendByte (
    TCP_SOCK   *sock,           // input data socket
    int         byt );          // input byte to send
int TCP_ByteReady (
    TCP_SOCK   *sock );         /* [in] socket                          */
int TCP_RecvByte (
    TCP_SOCK   *sock );         // input data socket
int TCP_WaitByte (
    TCP_SOCK   *sock,           // input data socket
    int         wait );         // input wait time in ms
int TCP_SendBlock (
    TCP_SOCK      *sock,        // input data socket
    unsigned char *buf,         // input buffer
    int            cnt );       // input # of bytes to send
int TCP_RecvBlock (
    TCP_SOCK      *sock,        // input data socket
    unsigned char *buf,         // output buffer
    int            cnt,         // input # of bytes to receive
    int            wait );      // input wait time in ms
int TCP_RecvBlock1 (
    TCP_SOCK      *sock,        /* input socket                         */
    unsigned char *buf,         /* output buffer                        */
    int            cnt,         /* input # of bytes to receive          */
    int            wait );      /* input wait time in ms                */
void TCP_SetBufSize (
    int tb_size,            /* [in] transmit buffer size in bytes       */
    int rb_size );          /* [in] receive buffer size in bytes        */
void TCP_GetBufSize (
//    int *tb_size,           /* [out] transmit buffer size in bytes      */
//    int *rb_size );         /* [out] receive buffer size in bytes       */
    unsigned int *tb_size,  /* [out] transmit buffer size in bytes      */
    unsigned int *rb_size );/* [out] receive buffer size in bytes       */

#ifdef _TI_CAMERA
int TCP_RecvBlock2 (
    TCP_SOCK      *sock,        /* input socket                         */
    unsigned char *buf,         /* output buffer                        */
    int            cnt,         /* input # of bytes to receive          */
    int            wait );      /* input wait time in ms                */
#endif

/*............. Camera specific TCP functions */
#ifdef _TI_CAMERA   /* [[[ - TI camera */
int TCP_SendBlockNoWait (
    TCP_SOCK      *sock,        // input data socket
    unsigned char *buf,         // input buffer
    int            cnt );       // input # of bytes to send

/* Missing VCRT prototypes */
unsigned int shutdown (unsigned int Sock, unsigned int flag);
//uint_32 shutdown (uint_32 socket, uint_32 method);

void time_delay(int);   /* enables other tasks to run */

/*............. PC specific TCP functions */
#else   /* ]]] [[[ - PC    */
int TCP_SendCmd (
    TCP_SOCK   *sock,           // input socket
    char       *cmd,            // input command string
    int         mode,           // input mode (1: read command echo)
    unsigned int wait );        // input wait time
void TCP_ReadEcho (
    TCP_SOCK    *sock,          // input socket
    int          dump,          // input dump flag
    unsigned int wait );        // input wait time
#endif  /* ]]] */

#ifdef __cplusplus
};
#endif

/*............. System time */
#ifdef _TI_CAMERA   /* [[[ - TI camera */
  unsigned long get_time ();
#else               /* ]]] [[[ - PC & other platforms   */
  #if defined(_MSC_VER)     /* PC Windows */
    #define get_time GetTickCount
  #else                     /* LINUX */
    unsigned long get_time();
  #endif
#endif  /* ]]] */


/*----------------------------------------------------------------------*/
/*                      Background receive functions                    */
/*----------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

void bgnd_rcv_reset (                                   /* BGND_RES.C   */
    TCP_SOCK   *sock );     /* input/output TCP socket                  */
void bgnd_rcv_close (
    TCP_SOCK   *sock );     /* input/output TCP socket                  */

void TCP_SetRecvMode (                                  /* BGND_RCV.C   */
    TCP_SOCK   *sock,       /* input/output TCP socket                  */
    int         type,       /* input type of receive operation          */
    int         mode,       /* input receive mode                       */
    int         timeout );  /* input total timeout for                  */
int  TCP_GetRecvMode (
    TCP_SOCK   *sock,       /* input TCP socket                         */
    int         type );     /* input type of receive tool               */
void TCP_SetSpecialMode (
    TCP_SOCK   *sock,       /* input/output TCP socket                  */
    int         type,       /* input type of receive operation          */
    int         mode );     /* input special mode                       */
int  TCP_GetSpecialMode (
    TCP_SOCK   *sock,       /* input TCP socket                         */
    int         type );     /* input type of receive tool               */
int  TCP_GetRecvState (
    TCP_SOCK   *sock );     /* input TCP socket                         */
int bgnd_rcv_start (
    TCP_SOCK   *sock,       /* input/output TCP socket                  */
    int         type,       /* input type of bgnd receive operation     */
    void       *hdr );      /* input block header                       */
int bgnd_rcv (
    TCP_SOCK   *sock,       /* input/output TCP socket                  */
    int         type,       /* input type of calling receive tool       */
    int         wait,       /* input max wait time                      */
    void       *hdr,        /* output block header                      */
    char       *data_buf ); /* output block data buffer                 */
int TCP_BgndRecvKeep (
    TCP_SOCK   *sock );     /* input/output TCP socket                  */

#ifdef __cplusplus
};
#endif

#endif  /* ]]] - _TCP_LIB_H_LOADED */
