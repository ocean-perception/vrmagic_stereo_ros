/*----------------------------------------------------------------------*/
/**
* @name IO_KEY.H
*       Keyboard input header.
*
* <br>  VM_LIB is subject to the license and usage terms of VRmagic GmbH,
* <br>  which are distributed with this software.
* <br>
* <br>  Header file with macros for extended key codes.
* <br>  This file contains type definitions, macros and function prototypes
*       of keyboard I/O functions.
*
* @version
*       Created:  24.04.2008
* <br>  Modified:
*/
/*----------------------------------------------------------------------*/
/*@{*/

#ifndef _IO_KEY_H_LOADED   /* [[[ */
#define _IO_KEY_H_LOADED

/*----------------------------------------------------------------------*/
/*                          Extended key codes                          */
/*----------------------------------------------------------------------*/
#define KEY_BACKSPACE   0x0008          /* Backspace                    */
#define KEY_TAB         0x0009          /* Tab                          */
#define KEY_LF          0x000A          /* Line feed                    */
#define KEY_ENTER       0x000D          /* Enter (carriage return)      */
#define KEY_ESC         0x001B          /* Esc                          */
#define KEY_CURS_UP     0x4800          /* Cursor Up                    */
#define KEY_CURS_DOWN   0x5000          /* Cursor Down                  */
#define KEY_CURS_RIGHT  0x4D00          /* Cursor Right                 */
#define KEY_CURS_LEFT   0x4B00          /* Cursor Left                  */
#define KEY_INS         0x5200          /* Ins                          */
#define KEY_DEL         0x5300          /* Del                          */
#define KEY_HOME        0x4700          /* Home                         */
#define KEY_END         0x4F00          /* End                          */
#define KEY_F1          0x3B00          /* F1                           */
#define KEY_F2          0x3C00          /* F2                           */
#define KEY_F3          0x3D00          /* F3                           */
#define KEY_F4          0x3E00          /* F4                           */
#define KEY_F5          0x3F00          /* F5                           */
#define KEY_F6          0x4000          /* F6                           */
#define KEY_F7          0x4100          /* F7                           */
#define KEY_F8          0x4200          /* F8                           */
#define KEY_F9          0x4300          /* F9                           */
#define KEY_F10         0x4400          /* F10                          */

/*----------------------------------------------------------------------*/
/*                           Function prototypes                        */
/*----------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

int  io_get_key ();                                     /* IO_KEY.C    */
int  io_get_char ();
int  io_put_char (
    int  c );               /* [in] char to write                       */

#ifdef __cplusplus
}
#endif


#endif /* ]]] _IO_KEY_H_LOADED */
/*@}*/
