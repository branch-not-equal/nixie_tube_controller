/*
 */
#ifndef _APP_H_
#define _APP_H_

#include <avr/io.h>
#include "macrodriver.h"

/* ------------------------------------------------
 * MACRO DEFINE
 * ------------------------------------------------ */
#define DEBUG 1
#ifdef DEBUG
#include "dprintf.h"
#endif

#ifdef DEBUG
#define DPRINT(arg) dprintf arg
#define DBITPRINT(arg1, arg2) dbitprint(arg1, arg2)
#else
#define DPRINTT(arg)
#define DBITPRINT(arg1, arg2)
#endif

#endif /* _MACRO_ONLY */
