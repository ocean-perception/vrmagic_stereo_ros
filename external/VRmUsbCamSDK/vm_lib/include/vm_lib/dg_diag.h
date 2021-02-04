/*----------------------------------------------------------------------*/
/**
* @name DG_DIAG.H
*       Diagnostic header file
*
* <br>  VM_LIB is subject to the license and usage terms of VRmagic GmbH,
* <br>  which are distributed with this software.
* <br>
* <br>  This header file contains macros, typedefs and function
* <br>  prototypes, used for run-time debugging, diagnostic image
* <br>  drawing and trace/dump operations.
*
* @version
*       Created:  04.11.2009
* <br>  Modified:
*/
/*----------------------------------------------------------------------*/
/*@{*/

#ifndef _DG_DIAG_H_LOADED   /* [[[ */
#define _DG_DIAG_H_LOADED

#include "cdef.h"

/*----------------------------------------------------------------------*/
/*                          Macros and typedefs                         */
/*----------------------------------------------------------------------*/

/*............. Macros */
#define FUNC_NAME_LEN  128  /* max length of function name  */
#define DUMP_NAME_LEN  256  /* max length of dump file name  */

/*............. General diag structure */
typedef struct {

/* Overlay image diag drawing */
    int     dr_mode;        /* overlay diagnostic drawing mode -        */
                            /* context dependent                        */
                            /* (see respective function descriptions):  */
                            /*   0 : diag drawing off                   */
                            /* !=0 : diag drawing on                    */
    image   dr_img;         /* diagnostic drawing image:                */
                            /*   st=0: disable diag drawing             */
    int     dr_clr;         /* foreground drawing color (0=default)     */
    int     dr_bgnd;        /* background drawing color (0=default)     */
    int     dr_x;           /* top/left x-coord. in dr_img              */
    int     dr_y;           /* top/left y-coord. in dr_img              */

/* Gray-level video image diag drawing */
    int     vd_mode;        /* video diagnostic mode, context dependent  */
                            /* (see respective function descriptions):  */
                            /*   0 : video diag drawing off             */
                            /* !=0 : video diag drawing on              */
    image   vd_img;         /* diagnostic gray-level video image:       */
                            /*   st=0: disable diag drawing             */
    int     vd_x;           /* top/left x-coord. in vd_img              */
    int     vd_y;           /* top/left y-coord. in vd_img              */

/* General dump to terminal/file */
    int     dump_mode;      /* diagnostic dump mode:                    */
                            /*  bit 0 = dump diag info on terminal      */
                            /*  bit 1 = append diag info to dump file:  */
                            /*          needs valid file name in        */
                            /*          dump_file                       */

    char    dump_file[DUMP_NAME_LEN];
                            /* dump file name                           */

/* Function in diag mode */
    char    func_name[FUNC_NAME_LEN];
                            /* function name, which should work in diag */
                            /* mode:                                    */
                            /*   func_name="" : ignore                  */
} DG_DIAG;


/*----------------------------------------------------------------------*/
/*                          Function prototypes                         */
/*----------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

void dg_set_draw_params (
    char   *func_name,      /* [in] function name (NULL=don't set)                */
    int     dr_mode,        /* [in] drawing mode                                  */
    image  *dr_img,         /* [in] drawing image (NULL=don't set)                */
    int     dr_clr,         /* [in] foreground drawing color (0=def,-1=don't set) */
    int     dr_bgnd,        /* [in] background drawing color (0=def,-1=don't set) */
    int     dr_x,           /* [in] top/left x-coord. in dr_img (-1=don't set)    */
    int     dr_y );         /* [in] top/left y-coord. in dr_img (-1=don't set)    */
void dg_set_video_params (
    char   *func_name,      /* [in] function name (NULL=don't set)      */
    int     vd_mode,        /* [in] video drawing mode                  */
    image  *vd_img,         /* [in] video image (NULL=don't set)        */
    int     vd_x,           /* [in] top/left x-coord. in vd_img         */
    int     vd_y );         /* [in] top/left y-coord. in vd_img         */
char *dg_get_draw_params (
    int    *dr_mode,        /* [out] drawing mode (NULL=don't return)   */
    image  *dr_img,         /* [out] drawing image (NULL=don't return)  */
    int    *dr_clr,         /* [out] fgnd draw color (NULL=don't return)*/
    int    *dr_bgnd,        /* [out] bgnd draw color (NULL=don't return)*/
    int    *dr_x,           /* [out] top/left x-coord. (NULL=don't ret) */
    int    *dr_y );         /* [out] top/left y-coord. (NULL=don't ret) */
char *dg_get_video_params (
    int    *vd_mode,        /* [out] video mode (NULL=don't return)     */
    image  *vd_img,         /* [out] video image (NULL=don't return)    */
    int    *vd_x,           /* [out] top/left x-coord. (NULL=don't ret) */
    int    *vd_y );         /* [out] top/left y-coord. (NULL=don't ret) */

#ifdef __cplusplus
}
#endif

#endif /* ]]] _DG_DIAG_H_LOADED */
/*@}*/
