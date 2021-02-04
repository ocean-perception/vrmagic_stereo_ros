/*----------------------------------------------------------------------*/
/**
* @name VM_LIB.H
*       VM_LIB root library header
*
* <br>  VM_LIB is subject to the license and usage terms of VRmagic GmbH,
* <br>  which are distributed with this software.
* <br>
* <br>  This file includes all VM_LIB library header files.
*
* @version
*       Created: 17.08.2010
* <br>  Modified:
*/
/*----------------------------------------------------------------------*/
/*@{*/
#ifndef _VM_LIB_H_LOADED        /* [[[ */
#define _VM_LIB_H_LOADED

/*----------------------------------------------------------------------*/
/*                            VM_LIB version                            */
/*----------------------------------------------------------------------*/
/* returned by vm_lib_vers() */

/*----------------------------------------------------------------------*/
/*                              Error codes                             */
/*----------------------------------------------------------------------*/
/* defined in lib_err.h */

/*----------------------------------------------------------------------*/
/*                           Macros and typedefs                        */
/*----------------------------------------------------------------------*/

/*............. Tool handles */
//typedef void * GUI_HND;     /* GUI tool handle  */
//typedef void * BLOB_HND;    /* BLOB handle      */


/*----------------------------------------------------------------------*/
/*                     Include sub-library headers                      */
/*----------------------------------------------------------------------*/
#include "cdef.h"
#include "dr_gui.h"
#include "dr_lib.h"
#include "cl_lib.h"
#include "ip_lib.h"
#include "ut_lib.h"
#include "vc_blib.h"
#include "vf_lib.h"
#include "sys_lib.h"
#include "lib_err.h"
#include "dg_lib.h"
#include "gui_lib.h"

#include "io_file.h"
#include "io_key.h"
#include "io_mouse.h"
#include "io_scr.h"
#include "io_ser.h"
#include "ms_drv.h"
#include "ms_io.h"
#include "tcp_lib.h"
#include "tcp_io.h"

#include "bmp_lib.h"
#include "img_lib.h"
#include "bl_lib.h"
#include "bc_lib.h"
#include "ncor.h"
#include "or_lib.h"


/*----------------------------------------------------------------------*/
/*                           Function prototypes                        */
/*----------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

int vm_lib_open (
     int vm_key );          /* [in] license key                         */

void vm_lib_close ();

void vm_lib_vers (
   int* version,            /* [out] basic library version              */
   int* subversion,         /* [out] library sub-version number         */
   int* release,            /* [out] library release number             */
   int* bugfix );           /* [out] library bug-fix number             */
char *vm_lib_vers_str ();

#ifdef __cplusplus
}
#endif

#endif /* ]]] - _VM_LIB_H_LOADED */
/*@}*/
