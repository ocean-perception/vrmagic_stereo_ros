/*----------------------------------------------------------------------*/
/**
* @name IO_SCR.H
*       I/O screen header
*
* <br>  VM_LIB is subject to the license and usage terms of VRmagic GmbH,
* <br>  which are distributed with this software.
* <br>
* <br>  This file contains type definitions, macros and function prototypes
*       for screen I/O.
*
* @version
*       Created:  10.04.2008
* <br>  Modified:
*/
/*----------------------------------------------------------------------*/
/*@{*/
#ifndef _IO_SCR_H_LOADED        /* [[[ */
#define _IO_SCR_H_LOADED

#include "cdef.h"
#include "tcp_lib.h"

/*----------------------------------------------------------------------*/
/*                            Macros and typdefs                        */
/*----------------------------------------------------------------------*/
/*
* Top-level virtual screen (VS) client commands (usually PC):
*   S = get old (slower) virtual screen
*   ? = get server configuration (old or new)
*       The VS server replies with VS_CFG block.
*   F = get new (faster) virtual screen. Must be followed by VS_REQ block,
*       sent from PC to camera.
*       The VS server replies with TCP image block, similar to the 'S' command.
*
* The new virtual screen is used for faster screen send operations when
* the virtual screen is in ZOOM mode:
*   - display whole zoom-ed camera screen when the screen is to big to fit in
*     the Windows desktop. The camera sends subsampled screen with ratio
*     x_zoom, y_zoom
*   - display part (area) of the camera screen 1:1 (1/4, 1/9, ... of whole screen)
*/
#define SCR_SEND_CMD  'S'   /* old "send screen" cmd    */
#define VS_GET_CFG    '?'   /* get VS configuration     */
#define VS_GET_SCR    'F'   /* get new faster VS        */

/* Fast virtual screen config packet */
typedef struct
{
    int vers;       /* VS server version                */
    int scr_dx;     /* width of camera screen           */
    int scr_dy;     /* height of camera screen          */
    int param[10];  /* spare                            */
} VS_CFG;

/*
* Fast virtual screen: request commands: PC -> CAM
* All commands receive merged VD + OVL image,
*/
typedef enum
{
    VS_REQ_GET_SCR = 0, /* get old full merged screen 1:1 (VD+OVL)      */
                        /* currently enables top-level 'S' cmd          */
    VS_REQ_GET_SUBS,    /* get full subsampled screen (merged VD+OVL)   */
    VS_REQ_GET_PANE,    /* get screen pane = screen area defined by     */
                        /* pane number. The screen is divided into      */
                        /* x_zoom * y_zoom equal parts, marked as shown */
                        /* below:                                       */
                        /* x_zoom,y_zoom=2:   0   1                     */
                        /*                    2   3                     */
                        /* x_zoom,y_zoom=3:   0   1   2                 */
                        /*                    3   4   5                 */
                        /*                    6   7   8                 */
    VS_REQ_GET_AREA     /* get screen area 1:1, defined by x,y,dx,dy    */
} VS_REQ_CMDS;


/* Fast virtual screen request packet */
typedef struct
{
    int cmd;        /* command - see VS_REQ_CMDS                        */

/* Zoom params, used when cmd = VS_REQ_GET_SUBS */
    int x_zoom;     /* x-zoom (subsample) ratio : scr_dx/x_zoom         */
    int y_zoom;     /* y-zoom (subsample) ratio : scr_dy/y_zoom         */

/* Screen pane number, used when cmd = VS_REQ_GET_PANE */
    int ipane;      /* screen pane number [0, x_zoom*y_zoom-1]          */

/* Screen area pos/size, used when cmd = VS_REQ_GET_AREA */
    int x;          /* x-coord. of top/left area corner                 */
    int y;          /* y-coord. of top/left area corner                 */
    int dx;         /* area dx                                          */
    int dy;         /* area dy                                          */

/* screen dimensions of PC simulator */
    int scr_dx;     /* screen dx                                        */
    int scr_dy;     /* screen dy                                        */

    int param[10];  /* spare                                            */
} VS_REQ;

/*----------------------------------------------------------------------*/
/*                           Function prototypes                        */
/*----------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

int  io_scr_init (                                      /* IO_SCR.C     */
    int   port );       /* [in] TCP connection port (<=0:default 2010)  */

void io_scr_close ();

int  io_scr_send (
    image  *vd_img,         /* [in] video (gray-level) image            */
    image  *dr_img,         /* [in] drawing (overlay) image             */
    int     scr_dx,         /* [in] screen width (<=0: default)         */
    int     scr_dy,         /* [in] screen height (<=0: default)        */
    int     wait,           /* [in] wait reply time in ms (0=no wait)   */
    int    *sent );         /* [out] screen sent flag: 0=no, 1=yes      */

void io_scr_get_sockets (
    TCP_SOCK **lsock,       /* [out] ptr to screen listen socket        */
    TCP_SOCK **dsock );     /* [out] ptr to screen data socket          */

#ifdef __cplusplus
}
#endif

#endif /* ]]] - _IO_SCR_H_LOADED */
/*@}*/
