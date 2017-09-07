/**
* @file main.c
*
* This file imnprements main functions, application entry point.
* 
* @note
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who                 Date     Changes
* ----- ----------------- -------- ------------------------------------------
* 1.00a bne 08/20/08 First release
* </pre>
*/

/*---------------------------------------------------------------------------*/
/* INCLUDE FILES */
/*---------------------------------------------------------------------------*/
#include <avr/interrupt.h>

	/* User specified include files*/
#include "app.h"
#include "nixieDriver.h"
#include "nixieCtrl.h"
#include "timer.h"
#include "msg.h"
#include "uart.h"

/*---------------------------------------------------------------------------*/
/* MACRO/TYPE DEFINES */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* FUNCTION PROTOTYPES */
/*---------------------------------------------------------------------------*/
static void initialize(void);

/*---------------------------------------------------------------------------*/
/* VARIABLE DEFINITIONS */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* PUBLIC FUNCTION */
/*---------------------------------------------------------------------------*/

/**
* Application entry point.
* @param    None.
* @return   None.
*/
int	main(void)
{
    u8 msg;

    /* initialize hardware and software */
    initialize();
    
    /* Enable interrupt */
    sei();
    timer_start();

    DPRINT(("-----------------------------\r"));
    DPRINT(("START\r"));
    DPRINT(("-----------------------------\r"));

//    return 0;

    /* select initial state. */
    //msg_post(MSG_NIXIEANODE);

    /* task loop */
    for(;;){
        
        /* no message */
        if (msg_get(&msg) == MSG_NOMSG){
            continue;
        }

        /* analyze message */
        switch (msg)
        {
        case MSG_INVALID:
            break;
            
        case MSG_1secInterval:
/*             DPRINT(("1sec\r")); */
            break;
            
            // callback for dynamic blinking control.
        case MSG_NIXIEANODE:
/*             DPRINT(("ANODE CALLBACK\r")); */
            nixieDriverTimerCallback();
            break;
            
        case MSG_NIXIECATHODE:
            /* Nixie control sample.. 
               Just counting up; ch0->intervally, ch1->randomly. */
            nixieCtrl();
            break;
            
        case MSG_BTN: // not implemented.
            //if (encBtnInterval() == TRUE){}
            break;
            
        case MSG_ENC: // not implemented.
            // if (encInterval() == TRUE){
            break;
        default:
            break;
        }
    }
}


/**
* initialize hardware and software.
*/
static void initialize(void)
{
    USART_Init(MYUBRR);
    //encInitialize();
    nixieCtrlInitialize();

    msg_init();
    
    DPRINT(("MMMSG OK\r"));
    DPRINT(("NIXIE OK\r"));    
    timer_init();
    DPRINT(("TIMER OK\r"));
}

