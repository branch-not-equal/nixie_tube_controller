/* -----------------------------------------------------
 *
 * FILE	   : timer.c
 * SUMMARY : TIMER Configuration
 * REMARKS : TIMER config, initialization
 *
 * DATE    : 2008.1.27
 * ----------------------------------------------------- */
#include "timer.h"

/* -----------------------------------------------------
 * INCLUDE DIRECTIVE
 * ----------------------------------------------------- */
#include <avr/io.h>
#include <avr/interrupt.h>


/* -----------------------------------------------------
 * Variable declaration
 * ----------------------------------------------------- */
static u16 interval1sec         = 0;
static u16 intervalBtn          = 0;
static u16 intervalEnc          = 0;
static u16 intervalNixieAnode   = 0;
static u16 intervalNixieCathode = 0;

/* -----------------------------------------------------
 * Function
 * ----------------------------------------------------- */

/*
 * SUMMARY : INITIALIZE TIMER
 * REMARKS : 
 */
void timer_init(void)
{
    /*
    TCCR1A = 0x00;   TIMER 1 mode
    TCCR1B = 0x01;   TIMER 1 prescaler config
    */
    
    /* TIMER 0 mode：interval timer mode config */
    TCCR1A = 0x00;

    /* clock prescaler config */
    //TCCR1B = _BV(CS01); // | _BV(CS00);     /* fclk/64                      */
    TCCR1B =     0xd;                               /* 2.032msec / 254 clock(0xff)  */
    OCR1A  = 0x08;
}

/* 
 * SUMMARY : START TIMER
 */
void timer_start(void)
{
    /* TIMER COUTNER 0 OVERFLOW INTERRUPTION ENABLE */
    // TIMSK0 = _BV(TOIE0);
    TIMSK1 = 0x02;
}

/*
 * SUMMARY : STOP TIMER
 */
void timer_stop(void)
{
    /* TIMER COUNTER 0 OVERFLOW INTERRUPTION DISABLE    */
    TIMSK0 &= ~_BV(TOIE0);
}

/*
 * TIMER COUNTER 0 OVERFLOW INTERRUPT VECTOR
 */
ISR(TIMER0_OVF_vect)
{
}

/*
 * MSG CONTROL TIMER
 * TIMER COUNTER 1 OVERFLOW INTERRUPT VECTOR
 */
ISR(TIMER1_COMPA_vect)
{
    if (0 == interval1sec){
        msg_post(MSG_1secInterval);
        interval1sec = INTERVAL_1sec;
    }
    if (0 == intervalBtn){
        msg_post(MSG_BTN);
        intervalBtn = INTERVAL_BTN;
    }
    if (0 == intervalEnc){
        msg_post(MSG_ENC);
        intervalEnc = INTERVAL_ENC;
    }
    if (0 == intervalNixieAnode){
        msg_post(MSG_NIXIEANODE);
        intervalNixieAnode = INTERVAL_NIXIEANODE;
    }
    if (0 == intervalNixieCathode){
        msg_post(MSG_NIXIECATHODE);
        intervalNixieCathode = INTERVAL_NIXIECATHODE;
    }
    
    interval1sec--;
    intervalBtn--;
    intervalEnc--;
    intervalNixieAnode--;
    intervalNixieCathode--;
}
