/*----------------------------------------------------------------------*/
/**
* @name UT_LIB.H
*       Utility library
*
* <br>  VM_LIB is subject to the license and usage terms of VRmagic GmbH,
* <br>  which are distributed with this software.
* <br>
* <br>  Header file for general-purpose utility library.
*
* @version
*       Created: 27.10.2005
* <br>  Modified:
*/
/*----------------------------------------------------------------------*/
/*@{*/

#ifndef _UT_LIB_H_LOADED   /* [[[ */
#define _UT_LIB_H_LOADED

/*----------------------------------------------------------------------*/
/*                              Error codes                             */
/*----------------------------------------------------------------------*/
//#define UT_OK                   0    /* Success                         */
//#define UT_ALLOC_ERROR        601    /* Memory allocation error         */
//#define UT_INTERNAL_ERR       602    /* Internal error (data currupted) */
// #define UT_RESBUF_OVF         603    /* Result buffer overflow          */

/*----------------------------------------------------------------------*/
/*                           Macros and typedefs                        */
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/*                           Function prototypes                        */
/*----------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

void ut_binstr_to_int (                                 /* UT_LIB.C     */
    char  *str,             /* [in] binary string ('0's and '1's)       */
    int    dig_cnt,         /* [in] number of binary digits             */
    int   *ibuf );          /* [out] 32-bit integer buffer              */
void ut_int_to_binstr (
    char  *str,             /* [out] binary string ('0's and '1's)      */
    int    dig_cnt,         /* [in] number of binary digits             */
    int   *ibuf );          /* [in] 32-bit integer buffer               */

char *ut_strtok (                                       /* UT_TOK.C     */
    char *str,              /* [in] string                              */
    const char *brks,       /* [in] brk char string                     */
    char *b );              /* [out] break character (0=end of str)     */


/* String utilities, used to process text configuration files */
void  ut_del_comments (                                 /* STR_UTL.C    */
    char   *str,                /* [i/o] text line                      */
    char    cchar );            /* [in] comment character               */
void  ut_del_blanks (
    char   *str,                /* [i/o] string                         */
    char   *brks,               /* [in] string with break characters    */
    int     mode );             /* [in] upcase mode                     */
int  ut_get_token (
    char   *str,                /* [in] scanned string                  */
    int    *ipos,               /* [i/o] start string position          */
    char   *brks,               /* [in] string with break characters    */
    char   *token,              /* [out] token string                   */
    char   *brk );              /* [out] break character                */
int  ut_search_str (
    char   *str,                /* [in] string                          */
    char  **sbuf,               /* [in] string buffer                   */
    int     n );                /* [in] sbuf size                       */
//int  binstr_to_int (
//    char  *str,             /* [in] binary string ('0's and '1's)      */
//    int    dig_cnt );       /* [in] number of binary digits in str     */
//void int_to_binstr (
//    char  *str,             /* [out] binary string ('0's and '1's)     */
//    int    dig_cnt,         /* [in] number of binary digits            */
//    int    num );           /* [in] integer                            */
//void int16_to_bin (
//    short   num,            /* [in] 16-bit integer                     */
//    char   *str );          /* [out] binary string ('0's and '1's)     */


//unsigned long ut_get_time ();
//void ut_wait (
//    int wait_time );    /* [in] wait time in ms                        */

#ifdef __cplusplus
}
#endif

#endif /* ]]] _UT_LIB_H_LOADED */
/*@}*/
