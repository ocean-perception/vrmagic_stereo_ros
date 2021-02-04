/*----------------------------------------------------------------------*/
/**
* @name MS_IO.H
*       Mouse TCP I/O header
*
* <br>  VM_LIB is subject to the license and usage terms of VRmagic GmbH,
* <br>  which are distributed with this software.
* <br>
* <br>  This file contains type definitions, macros and function prototypes
*       of mouse data send/receive TCP functions.
*
* @version
*       Created:  01.06.2006
* <br>  Modified:
*/
/*----------------------------------------------------------------------*/
/*@{*/
#ifndef _MS_IO_H_LOADED        /* [[[ */
#define _MS_IO_H_LOADED

/*----------------------------------------------------------------------*/
/*                          Macros and typedefs                         */
/*----------------------------------------------------------------------*/

/*............. Macros */
#define MS_IO_PORT   2011  /* default mouse I/O TCP port  */
#define MS_SEND_CMD   'M'  /* "send mouse data" command   */

/*............. Mouse data block structure */
typedef struct
{
    int   draw_curs;    /* draw cursor flag: 0=off, 1=on    */
    int   x;            /* x-coord. of mouse cursor         */
    int   y;            /* y-coord. of mouse cursor         */
} MS_DATA;


/*----------------------------------------------------------------------*/
/*                           Function prototypes                        */
/*----------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

int  ms_send_open (                                     /* MS_SND.C     */
    int   port );       /* [in] TCP communication port (<=0:def 2011)   */
void ms_send_close ();
int  ms_send (
    MS_DATA  *ms_data,      /* [in] mouse data block                    */
    int      *sent );       /* [out] screen sent flag: 0=no, 1=yes      */

void ms_recv_open (                                     /* MS_RCV.C     */
    int   port,         /* [in] TCP communication port (>0)             */
    char *ip_addr );    /* [in] IP string address "xxx.xxx.xxx.xxx"     */
void ms_recv_close ();
int  ms_recv (
    MS_DATA  *ms_data,      /* [out] mouse data block                   */
    int       time_out );   /* [in] receive time-out in ms (0=def: 1000)*/
int  ms_recv_request (
    int  *state );          /* [out] request state: 0=failed, 1=OK      */
int  ms_recv_response (
    MS_DATA  *ms_data,      /* [out] mouse data block                   */
    int       time_out,     /* [in] receive time-out in ms (0=def: 1000)*/
    int      *state );      /* [out] response state: 0=not ready, 1=ready*/


#ifdef __cplusplus
}
#endif

#endif /* ]]] - _MS_IO_H_LOADED */
/*@}*/
