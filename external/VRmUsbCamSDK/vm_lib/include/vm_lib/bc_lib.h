/*----------------------------------------------------------------------*/
/**
* @name BC_LIB.H
*       Header file for barcode library
*
* <br>  VM_LIB is subject to the license and usage terms of VRmagic GmbH,
* <br>  which are distributed with this software.
* <br>
* <br>  This file defines macros and type definitions used by the
*       barcode library. The library supports:
* <br>    Code interleaved 2 of 5
* <br>    Code 39
* <br>    EAN13/UPC-A, EAN8, UPC-E
* <br>    Code-128/EAN128
*
* @version
*       Created: 20.08.2007
* <br>  Modified:
*/
/*----------------------------------------------------------------------*/
/*@{*/

#ifndef _BC_LIB_H     /* [[[ */
#define _BC_LIB_H

#if defined(_TI_CAMERA)
  #include <sysvar.h>
  #include <vcrt.h>
  #include <vclib.h>
  #include <macros.h>
#else   /* PC */
  #include <stdio.h>
  #include <stdlib.h>
/*  #include <malloc.h> */
/*  #include "BackBuffer.h" */
#endif

/*............. Version */
#define BC_VERS       100   /* barcode library version (100 for 1.00)   */
#define BC_VERS_STR "1.00"  /* library version string                   */

/* hardware independent substitute for malloc, free */
#if defined(_TI_CAMERA) || defined(_VM_CAMERA)
  #define MALLOC vcmalloc
  #define FREE   vcfree
#else   /* PC */
  #define MALLOC malloc
  #define FREE   free

  #ifndef IMAGE_DEFINED
    typedef struct          /* image on PC */
    {
        long st;            /* start byte address (long)    */
        int  dx;            /* horizontal width             */
        int  dy;            /* vertical width               */
        int  pitch;         /* memory pitch                 */
    } image;
    #define IMAGE_DEFINED
  #endif
#endif


/* The definition determines the binarization method - VER1 or VER2 */
#define VER2 /* VER1 */

#define MAX_LINE_LENGTH 5000
#define MAX_RESULT_SIZE 100
#define NUMBER_OF_PIXELS 50
#define EDGE_THRESHOLD 15
#define pi 3.141592654
#define ANGLE 10
#define QUIET_ZONE 30
#define A_STEP 10
#define EDGE_NUMBER 10
#define LEFT 0
#define RIGHT 1

#define BC_COPY_PROT_ERR  99

#define DB_ERR1 100
#define DB_ERR2 101
#define DB_ERR3 102
#define DB_ERR4 103

#define RRD_ERR1 200
#define RRD_ERR2 201
#define RRD_ERR3 202
#define RRD_ERR4 203

#define AD_ERR1 300
#define AD_ERR2 301
#define AD_ERR3 302
#define AD_ERR4 303
#define AD_ERR5 304
#define AD_ERR6 305
#define AD_ERR7 306

#define RAD_ERR1 400
#define RAD_ERR2 401
#define RAD_ERR3 402
#define RAD_ERR4 403

#define SB39_ERR1 101
#define SB39_ERR2 102
#define SB39_ERR3 103
#define SB39_ERR4 104
#define SB39_ERR5 105
#define SB39_ERR6 106
#define SB39_ERR7 107

#define EAN128_ERR1  501 /* invalid symbol - bar smaller then 2/11 or larger then 7/11 */
#define EAN128_ERR2  502 /* invalid symbol - bar pattern not in lookup table           */
#define EAN128_ERR3  503 /* invalid symbol - total width of bars wrong for the symbol  */
#define EAN128_ERR4  504 /* invalid start symbol                                       */
#define EAN128_ERR5  505 /* invalid stop symbol                                        */
#define EAN128_ERR6  506 /* missing FNC1 symbol                                        */
#define EAN128_ERR7  507 /* invalid checksum symbol                                    */
#define EAN128_ERR8  508 /* unexpected special function symbol                         */
#define EAN128_ERR9  509 /* faulty data structure - data-type without following data   */
#define EAN128_ERR10 510 /* faulty data structure - unexpected end of code data        */

#define EAN13_ERR1  601 /* invalid symbol - wrong number of bars in the barcode     */
#define EAN13_ERR2  602 /* unused                                                   */
#define EAN13_ERR3  603 /* unused                                                   */
#define EAN13_ERR4  604 /* unexpected end of the scanline - forward                 */
#define EAN13_ERR5  605 /* unexpected end of the scanline - reverse                 */
#define EAN13_ERR6  606 /* some kind of symbol error                                */
#define EAN13_ERR7  607 /* both sides contain font A                                */
#define EAN13_ERR8  608 /* unknown font pattern (EAN13-1st digit or UPC-E checksum) */
#define EAN13_ERR9  609 /* UPC-E symbol error                                       */
#define EAN13_ERR10 610 /* unused                                                   */

#define INTPIXEL    0
#define SUBPIXEL    1

#define EAN128_INTERPRETER_MODE   1

/*=============================================================================*/

typedef struct
{
    short length;
    short pixel[MAX_LINE_LENGTH];
    short histogram[256];
    short histogram_sum;
} DATA;

typedef struct
{
    short length;
    short pixel[MAX_LINE_LENGTH];
} SB_DATA;

typedef struct
{
    short left[18];
    short left_length[18];
    short left_position[18];
    short right[18];
    short right_length[18];
    short right_position[18];
    short sum[18];
} EDGES;

typedef struct
{
    double x;
    double y;
} t_dpoint;

typedef struct
{
    float x;
    float y;
} t_fpoint;

typedef struct
{
    short x[100];
    short y[100];
    short left_size;
    short right_size;
} contour_data;


/*=============================================================================*/

/* Local function prototypes */
void        get_max_coordinates          (image *img,short *max_x,short *max_y);
short       read_pixel                   (image *img,short x,short y);
float       ip_mms                       ();
short       ip_line_cross_byequ          (float *coef1,float *coef2,short *x,short *y);
short       calculate_fine_barcode_angle (short n,t_fpoint* points,short* angle,float* result_b);
void        line_sweep                   (image* img,short* center_x,short* center_y, short angle,short length, short dir, short* result_x,short* result_y);
short       line_sweep_to_max            (image* img,short* center_x,short* center_y,short angle,short dir,SB_DATA* data);
short       edge_detect                  (short threshold,short data[],short length,short result[]);
short       find_barcode_edges           (short quiet_zone,SB_DATA* data,short* length,short* position,short edge_number);
void        read_data_window             (image* img,short* start_x,short* start_y,short threshold,short data[]);
short       left_contour_point           (image* img,short* start_x,short* start_y,short threshold);
short       right_contour_point          (image* img,short* start_x,short* start_y,short threshold);
short       sb                           (image* img,short* center_x,short* center_y,short result_x[4],short result_y[4]);
short       read_subpixel                (image* img,float x,float y);
short       read_raw_data                (image* img,DATA* line,short x[4],short y[4]);
short       read_subpixel_data           (image* img,DATA* line,short x[4],short y[4]);
short       make_histogram               (DATA* line);

#ifdef VER2
short calculate_threshold(short* pointer,short length);
short binarize_data(DATA* line);
#endif

#ifdef VER1
short binarize_data(DATA* line);
#endif


short gen_result_str (
    char  *result_str0,     /* [in] result string 0                     */
    char  *result_str1,     /* [in] result string 1                     */
    char  *result_str2,     /* [in] result string 2                     */
    char  *result_str,      /* [out] result string                      */
    short *result_cnt );    /* [out] # if results chars in result_str   */

/*
//short analyze_data(DATA* line, char* result);
//short reverse_analyze_data(DATA* line, char* result);
//short db39(image* img,short x[4],short y[4],char* result_pointer,short pixel_mode);
//
//short analyze_data_25(DATA* line, char* result);
//short reverse_analyze_data_25(DATA* line, char* result);
//short db25(image* img,short x[4],short y[4],char* result_pointer,short pixel_mode);
//
//short e2E(short e, short p);
//void get_ean128code(short E[5], short* code, short* V);
//short EAN128_data_length(short data_type);
//short analyze_data_EAN128(DATA* line, char* result, short* result_length, short interpreter_mode, short direction);
//short dbEAN128(image* img,short x[4],short y[4],char* result_pointer,short pixel_mode, short interpreter_mode);
//
//short e2E13(short e, short p);
//void get_ean13code(short E[4], short b[3], short p, short* code, short* V);
//short analyze_data_EAN13(DATA* line, char* result, short* result_type);
//short dbEAN13(image* img,short x[4],short y[4],char* result_pointer,short pixel_mode,short* result_type);
*/

/*----------------------------------------------------------------------*/
/*                      Public function prototypes                      */
/*----------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

short search_barcode_pattern(image *img,short center_x,short center_y,short result_x[4],short result_y[4]);

short detect_barcode_39(image *img,short x[4],short y[4],char *result_pointer);
short detect_barcode_25(image *img,short x[4],short y[4],char *result_pointer);
short detect_barcode_EAN128(image *img,short x[4],short y[4],char *result_pointer,short interpreter_mode);
short detect_barcode_EAN13(image *img,short x[4],short y[4],char *result_pointer, short* result_type);

short detect_barcode_CODE128 (
    image *img,             /* [in] barcode pattern image               */
    short  x[4],            /* [in] x-coordinates of 4 pattern corners  */
    short  y[4],            /* [in] y-coordinates of 4 pattern corners  */
    char  *result_str,      /* [out] result buffer                      */
    short *result_cnt );    /* [out] # of chars in result_str[]         */

#ifdef __cplusplus
};
#endif

#endif  /* ]]] - _BC_LIB_H */
/*@}*/
