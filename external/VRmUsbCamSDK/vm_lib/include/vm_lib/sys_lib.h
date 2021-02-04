/*----------------------------------------------------------------------*/
/**
* @name SYS_LIB.H
*       System library
*
* <br>  VM_LIB is subject to the license and usage terms of VRmagic GmbH,
* <br>  which are distributed with this software.
* <br>
* <br>  Header file for the system library.
*
* @version
*       Created: 24.01.2011
* <br>  Modified:
*/
/*----------------------------------------------------------------------*/
/*@{*/

#ifndef _SYS_LIB_H_LOADED   /* [[[ */
#define _SYS_LIB_H_LOADED

//#include "cdef.h"
//#include "io_mouse.h"

/*----------------------------------------------------------------------*/
/*                           Macros and typedefs                        */
/*----------------------------------------------------------------------*/

///*............. Box definition */
//#ifndef VM_BOX_DEFINED    /* [[[ */
//  #define VM_BOX_DEFINED
//  typedef struct
//  {
//      int   x;        /* x-coord. of top/left box corner    */
//      int   y;        /* y-coord. of top/left box corner    */
//      int   dx;       /* box width in pixels                */
//      int   dy;       /* box height in pixels               */
//  } VM_BOX;
//#endif  /* ]]] */


/*----------------------------------------------------------------------*/
/*                           Function prototypes                        */
/*----------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

unsigned long sys_time ();

void sys_wait (
    int wait_time );        /* [in] wait time in ms                     */

#ifdef __cplusplus
}
#endif

#endif /* ]]] _SYS_LIB_H_LOADED */
/*@}*/
