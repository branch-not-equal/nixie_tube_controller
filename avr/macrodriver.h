/* -----------------------------------------------------
 *
 * FILE	   : macrodriver.h
 * SUMMARY : Application commons.
 * 
 *
 * DATE    : 2008.1.27
 * ----------------------------------------------------- */
#ifndef MACRODRIVER_H
#define MACRODRIVER_H


#define FALSE   0
#define TRUE    1

/* CPU CLOCK 8MHz   */
#define FXX_CPU 8000000


/* ------------------------------------------------
 * TYPE DEFINE
 * ------------------------------------------------ */
typedef unsigned char   u8;
typedef signed   char   s8;    
typedef unsigned int    u16;
typedef unsigned long   u32;
typedef signed   int    s16;
typedef signed   long   s32;
typedef float           f16;
typedef double          f32;

typedef unsigned char   bool;

/* -------------------------------------------------
 * APPLICATION RETURN CODE
 * ------------------------------------------------- */
typedef unsigned char   APPRESULT;

enum APPSTS{
    
    APP_SUCCESS,
    APP_FAILURE,
    APP_ERR_FAIL,
};

#endif
