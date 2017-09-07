/* -----------------------------------------------------
 *
 * FILE	   : timer.h
 * SUMMARY : Time handling.
 * REMARKS : Timer initialize, start, stop control.
 *           MPU dependent.
 *
 * DATE    : 2008.1.27
 * ----------------------------------------------------- */
#ifndef TIMER_H
#define TIMER_H

/* -----------------------------------------------------
 * INCLUDE DIRECTIVE
 * ----------------------------------------------------- */
#include "macrodriver.h"
#include "msg.h"

/* -----------------------------------------------------
 * Macro/Enum define
 * ----------------------------------------------------- */
enum TIMER_INTERVAL{
    INTERVAL_1sec         = 1000, /* 1000msec interval */
    INTERVAL_ENC          = 5,
    INTERVAL_BTN          = 10,
/*     INTERVAL_NIXIEANODE   = 5000, */
/*     INTERVAL_NIXIECATHODE = 5000 */
    INTERVAL_NIXIEANODE   = 5,
    INTERVAL_NIXIECATHODE = 100,
};


/* -----------------------------------------------------
 * Function
 * ----------------------------------------------------- */

/*
 * SUMMARY : INITIALIZE TIMER
 * REMARKS : 2.032msec INTERVAL TIMER
 */
void timer_init(void);

/* 
 * SUMMARY : START TIMER
 */
void timer_start(void);

/*
 * SUMMARY : STOP TIMER
 */
void timer_stop(void);


#endif /* TIMER_H */
