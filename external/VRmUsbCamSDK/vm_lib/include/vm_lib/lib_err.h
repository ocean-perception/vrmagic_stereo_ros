/*----------------------------------------------------------------------*/
/**
* @name LIB_ERR.H
*       VM_LIB error codes
* 
* <br>  VM_LIB is subject to the license and usage terms of VRmagic GmbH,
* <br>  which are distributed with this software.
* <br>
* <br>  This file contains error codes, returned by VM_LIB.
*
* @version
*       Created:   17.01.2006
* <br>  Modified:
*/
/*----------------------------------------------------------------------*/
/*@{*/

#ifndef _LIB_ERR_H
#define _LIB_ERR_H

/*----------------------------------------------------------------------*/
/*                             System errors                            */
/*----------------------------------------------------------------------*/
#define SYS_OK                    0 /* Success                          */
#define SYS_ALLOC_ERROR         150 /* Memory allocation error          */
#define SYS_EXIT_EVENT          151 /* System exit event                */
#define SYS_CAM_INIT_ERR        152 /* Camera initialization error      */
#define SYS_SCREEN_ERR          153 /* Screen size/pitch error          */
                                    /* (BFIN camera not initialized)    */
#define SYS_TPICT_ERR           154 /* Take picture error               */
#define SYS_TPICT_TIMEOUT       155 /* BFIN take picture timeout error  */
#define SYS_IMGOVF_ERR          156 /* Image overflow error             */
#define SYS_INV_IMAGE           157 /* Invalid image                    */

/*----------------------------------------------------------------------*/
/*                         High-level tool errors                       */
/*----------------------------------------------------------------------*/
#define VM_OK                     0 /* Success                          */

#define VM_ALLOC_ERROR          200 /* Memory allocation error          */
#define VM_INV_IMAGE            201 /* Invalid input image              */
#define VM_AREA_OUTSIDE         202 /* Rectangle area outside image     */
#define VM_BLOB_INV_HANDLE      203 /* BLOB handle error                */
#define VM_BLOB_INV_ODB_BUFFER  204 /* BLOB invalid ODB buffer          */
#define VM_MFIND_ARG_ERROR      205 /* "Mark finder" argument error     */

#define VM_GUI_RECT_INV_HANDLE  206 /* GUI rectangle handle error       */
#define VM_GUI_RECT_INV_EXEC_ID 207 /* Invalid GUI rectangle exec id.   */
#define VM_GUI_RESIZE_ERROR     208 /* GUI rectangle resize error       */

#define VM_WIN_INV_HANDLE       209 /* Window handle error              */
#define VM_WIN_INV_EXEC_ID      210 /* Invalid window exec id.          */
#define VM_WIN_SIZE_ERR         211 /* Window size error (too small)    */

#define VM_BUT_INV_HANDLE       212 /* Button handle error              */
#define VM_BUT_INV_EXEC_ID      213 /* Invalid button exec id.          */

#define VM_SPIN_INV_HANDLE      214 /* Spin handle error                */
#define VM_SPIN_INV_EXEC_ID     215 /* Invalid spin exec id.            */
#define VM_SPIN_INV_ARG         216 /* Invalid spin argument(s)         */
#define VM_SPIN_TOGGLE_ERR      217 /* Spin toggle error                */

#define VM_EDIT_INV_HANDLE      218 /* Edit handle error                */
#define VM_EDIT_INV_EXEC_ID     219 /* Invalid edit exec id.            */
#define VM_EDIT_INV_STATE       220 /* Invalid edit state               */
#define VM_EDIT_INV_EDIT_LEN    221 /* Invalid edit box length in char #*/
#define VM_EDIT_INV_ARG         222 /* Invalid edit argument(s)         */

#define VM_RADB_INV_HANDLE      223 /* Radio-button handle error        */
#define VM_RADB_INV_EXEC_ID     224 /* Invalid radio-button exec id.    */
#define VM_RADB_INV_ARG         225 /* Invalid radio-button argument(s) */

#define VM_CBOX_INV_HANDLE      226 /* Check-box handle error           */
#define VM_CBOX_INV_EXEC_ID     227 /* Invalid check-box exec id.       */
#define VM_CBOX_INV_ARG         228 /* Invalid check-box argument(s)    */

#define VM_DRAW_INV_ARG         229 /* Invalid drawing argument(s)      */
#define VM_INV_ARG              230 /* Invalid argument                 */
#define VM_INTERNAL_ERR         231 /* Internal error                   */
#define VM_RESBUF_OVF           232 /* Result buffer overflow           */

//#define VM_EDIT_STR_OVF         230 /* Too long edit string (>256)      */

/*----------------------------------------------------------------------*/
/*                          Drawing library errors                      */
/*----------------------------------------------------------------------*/
#define DR_OK                     0 /* Success                          */
#define DR_ALLOC_ERROR          300 /* Memory allocation error          */
#define DR_INV_ARG              301 /* Invalid function argument(s)     */
#define DR_INV_IMAGE            302 /* Invalid image                    */
#define DR_INIT_ERROR           303 /* Draw library init error          */
#define DR_FONT_FILE_ERR        304 /* Font file error                  */
#define DR_INVALID_FONT         305 /* Invalid font or not installed    */
#define DR_COPY_PROT_ERR        306 /* Copy protection error            */
//#define DR_INTERNAL_ERR         306  /* Internal error (data currupted) */
//#define DR_RESBUF_OVF           307  /* Result buffer overflow          */

/*----------------------------------------------------------------------*/
/*                 Image processing library errors                      */
/*----------------------------------------------------------------------*/
#define IP_ALLOC_ERROR          350 /* Memory allocation error          */
#define IP_INV_ARG              351 /* Invalid function argument(s)     */
#define IP_INV_IMAGE            352 /* Invalid image                    */
#define IP_RESBUF_OVF           353 /* Result buffer overflow           */
#define IP_AREA_OUTSIDE         354 /* Rectangle outside image          */
#define IP_INTERNAL_ERR         355 /* Internal error                   */
#define IP_INV_PLATFORM         356 /* Invalid PC/camera platform       */
#define IP_INV_BUFFER           357 /* Invalid data buffer              */
#define IP_NO_EDGES             358 /* No detected edges                */
#define IP_INV_RLC              359 /* Invalid RLC buffer               */
#define IP_RLC_OVF              360 /* RLC buffer overflow              */
#define IP_RLC_MISMATCH         361 /* Mismatching RLC dimensions       */
#define IP_RLC_CORR_ERR         362 /* RLC correlation error            */
#define IP_INV_FONT             363 /* Invalid font buffer              */
#define IP_INV_FONT_CODE        364 /* Invalid font code                */
#define IP_INV_FONT_ICON        365 /* Invalid font char icon           */
#define IP_FONTBUF_OVF          366 /* Font data buffer overflow        */
#define IP_CHARBUF_OVF          367 /* Font char buffer overflow        */
#define IP_FONT_FILE_ERR        368 /* Font file error                  */
#define IP_INV_FILT             369 /* Invalid filter type              */
#define IP_INV_COEF             370 /* Invalid filter coef. matrix      */
#define IP_BAD_CONTRAST         371 /* Bad image contrast               */
#define IP_IMG_MISMATCH         372 /* Mismatching images               */
#define IP_OPEN_ERR             373 /* Open error                       */
#define IP_INV_RES              374 /* Invalid result                   */
#define IP_INV_GRID             375 /* Invalid grid                     */
#define IP_INV_GT_HND           376 /* Invalid GT library handle        */
#define IP_NO_TEACH_GT          377 /* GT library not taught            */
#define IP_COPY_PROT_ERR        378 /* Copy protection error            */
#define IP_FILE_ERR             379 /* File read/write error            */

/*----------------------------------------------------------------------*/
/*                     Calculation library errors                       */
/*----------------------------------------------------------------------*/
#define CL_ALLOC_ERROR          400 /* Memory allocation error          */
#define CL_INV_ARG              401 /* Invalid input argument(s)        */
#define CL_INV_RES              402 /* Invalid result                   */
#define CL_RESBUF_OVF           403 /* Result buffer overflow           */
#define CL_INTERNAL_ERR         404 /* Internal error                   */
#define CL_COPY_PROT_ERR        405 /* Copy protection error            */

/*----------------------------------------------------------------------*/
/*                           I/O library errors                         */
/*----------------------------------------------------------------------*/
#define IO_ALLOC_ERROR          450 /* Memory allocation error          */
#define IO_SCR_ERROR            451 /* I/O screen error (invalid images)*/
#define IO_MS_ERROR             452 /* Mouse data TCP I/O error         */
#define IO_INV_ARG              453 /* Invalid input argument           */
#define IO_INV_IMAGE            454 /* Invalid image                    */
#define IO_RES_OVF              455 /* Result buffer overflow           */
#define IO_TCP_ERROR            456 /* I/O TCP error                    */
#define IO_REQ_ERROR            457 /* I/O request error                */


// defined in io_ser.h:
//#define IO_SER_OPEN_ERROR            2030   /* Serial open error        */
//#define IO_SER_GET_STATE_ERROR       2031   /* Serial get state error   */
//#define IO_SER_SET_STATE_ERROR       2032   /* Serial set state error   */
//#define IO_SER_GET_TIMEOUT_ERROR     2033   /* Serial get timeouts err  */
//#define IO_SER_SET_TIMEOUT_ERROR     2034   /* Serial set timeouts err  */
//#define IO_SER_DEVICE_NOT_OPEN       2035   /* Serial device not opened */
//#define IO_SER_CLOSE_ERROR           2036   /* Serial close error       */
//#define IO_SER_FLUSH_ERROR           2037   /* Serial flush error       */
//#define IO_SER_PURGE_ERROR           2038   /* Serial purge error       */
//#define IO_SER_INVALID_ARG           2039   /* Serial invalid argument  */
//#define IO_SER_READ_ERROR            2040   /* Serial read error        */
//#define IO_SER_WRITE_ERROR           2041   /* Serial write error       */
//#define IO_SER_WRITE_TIMEOUT_ERROR   2042   /* Serial write timeout     */
//#define IO_SER_READ_TIMEOUT_ERROR    2043   /* Serial read timeout      */
//#define IO_SER_HANDLE_ERROR          2044   /* Serial invalid handle    */
//#define IO_SER_RS422_FIFO_OVF        2045   /* RS422 FIFO overflow      */
//#define IO_SER_RTS_ERROR             2046   /* Set RTS error            */
//#define IO_SER_DTR_ERROR             2047   /* Set DTR error            */
//#define IO_SER_NO_MEMORY             2048   /* Memory allocation error  */



#endif  /* _LIB_ERR_H */
/*@}*/
