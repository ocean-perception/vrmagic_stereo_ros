/*----------------------------------------------------------------------*/
/**
* @name IO_MOUSE.H
*       Mouse library header
*
* <br>  VM_LIB is subject to the license and usage terms of VRmagic GmbH,
* <br>  which are distributed with this software.
* <br>
* <br>  This file contains type definitions, macros and function prototypes
*       of the mouse library.
*
* @version
*       Created: 17.01.2006
* <br>  Modified:
*/
/*----------------------------------------------------------------------*/
/*@{*/
#ifndef _IO_MOUSE_H_LOADED        /* [[[ */
#define _IO_MOUSE_H_LOADED

/*----------------------------------------------------------------------*/
/*                              Mouse commands                          */
/*----------------------------------------------------------------------*/

/*............. Mouse event bits */
/* A mouse event may be combination (OR) of several bits */
// #define IO_MOUSE_EVENT_NONE      0x0000 /* none/processed mouse event   */
#define IO_MOUSE_LEFT_PRESS      0x0001 /* left button press event      */
#define IO_MOUSE_LEFT_PRESSED    0x0002 /* left but in pressed state    */
#define IO_MOUSE_LEFT_REL        0x0004 /* left button release event    */
#define IO_MOUSE_LEFT_DBLCLICK   0x0008 /* left button double click     */
#define IO_MOUSE_RIGHT_PRESS     0x0010 /* right button press event     */
#define IO_MOUSE_RIGHT_PRESSED   0x0020 /* right but in pressed state   */
#define IO_MOUSE_RIGHT_REL       0x0040 /* right button release event   */
#define IO_MOUSE_RIGHT_DBLCLICK  0x0080 /* right button double click    */
#define IO_MOUSE_MOVE            0x0100 /* mouse move                   */

/*............. Mouse state bits - keep sync. with MS_DRV.C */
#define IO_MOUSE_BUT_LEFT   0x0001  /* left mouse button pressed        */
#define IO_MOUSE_BUT_RIGHT  0x0002  /* right mouse button pressed       */
//#define IO_MOUSE_STAT_ERROR     -1  /* mouse state error (UART buf ovf) */
//#define IO_MOUSE_OFFLINE        -2  /* no connected mouse               */

/*............. Mouse event structure */
typedef struct
{
    int   event;        /* mouse event (OR of mouse event bits)         */
    int   stat;         /* button state (OR of mouse state bits)        */
    int   x;            /* absolute x-coord. of mouse cursor            */
    int   y;            /* absolute y-coord. of mouse cursor            */

///* Coordinates, relative to window client area */
//        IO_COORD  cx;           /* rel. x-coord. of mouse cursor        */
//        IO_COORD  cy;           /* rel. y-coord. of mouse cursor        */
} MS_EVENT;


///*............. Mouse state structure */
//typedef struct
//{
//    int   stat;     /* mouse button press state             */
//    int   x;        /* abs. x-coord. of mouse cursor        */
//    int   y;        /* abs. y-coord. of mouse cursor        */
//} IO_MOUSE_STATE;

/*----------------------------------------------------------------------*/
/*                           Function prototypes                        */
/*----------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

void io_get_mouse_cmd (                                 /* IO_MOUSE.C   */
    MS_EVENT *ms_event );       /* output mouse event                   */

void io_set_mouse_rect (
    int   x,        /* [in] x-coord. of top/left corner of bound rect   */
    int   y,        /* [in] y-coord. of top/left corner of bound rect   */
    int   dx,       /* [in] width of bound rectangle                    */
    int   dy );     /* [in] height of bound rectangle                   */

void io_set_mouse_pos (
    int   x,                /* [in] x-coord. of mouse cursor            */
    int   y );              /* [in] y-coord. of mouse cursor            */

#ifdef __cplusplus
}
#endif

#endif /* ]]] - _IO_MOUSE_H_LOADED */
/*@}*/
