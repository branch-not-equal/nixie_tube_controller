/*****************************************************************************/
/**
* @file nixieDriver
*
* This file inprements nixie tube driver functions.
* 
* @note
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who                 Date     Changes
* ----- ----------------- ---------- ------------------------------------------
* 1.00a BNE               2012/04/08 First release
* </pre>
*****************************************************************************/
#ifndef _NIXIEDRIVER_H_
#define _NIXIEDRIVER_H_

#include "app.h"

/** !!! NEED TO BE DEFINED ACCORDING TO YOUR SYSTEM !!! */
#define NIXIEDRIVER_SETPORT(port, val)  (port = val)

/*
 * Port definition
 * All channels are controlled by BCD based value.
 */
#define NIXIEDRIVER_PORT_ANODE       PORTB
#define NIXIEDRIVER_PORT_CATHODE     PORTD

#define NIXIEDRIVER_POUT_ANODE       PORTB
#define NIXIEDRIVER_POUT_CATHODE     PORTD
#define NIXIEDRIVER_PIN_ANODE        PINB
#define NIXIEDRIVER_PIN_CATHODE      PIND

/* BCD 0 ---> 7 ch *//* 74138 A0/A1/A2/G1 pin */
#define NIXIEDRIVER_PIN_ANODE_0     (PB0) // A0
#define NIXIEDRIVER_PIN_ANODE_1     (PB1) // A1
#define NIXIEDRIVER_PIN_ANODE_2     (PB2) // A2
#define NIXIEDRIVER_PIN_ANODE_G     (PB3) // G1 LOW DISABLE
#define NIXIEDRIVER_ANODE_MAX       (8)

/* BCD 0 ---> 9 ch */
#define NIXIEDRIVER_PIN_CATHODE_0   (PD7)
#define NIXIEDRIVER_PIN_CATHODE_1   (PD5)
#define NIXIEDRIVER_PIN_CATHODE_2   (PD6)
#define NIXIEDRIVER_PIN_CATHODE_3   (PD4)


/** nixie tube status */
typedef struct _NIXIE_HANDLE {
    
    u8 channel;
    u8 value;
    u8 valid;
    
} NIXIE_HANDLE;



/** !!! PORT INITIALIZATION MUST BE DONE IN ADVANCE !!! */
bool nixieDriverInitialize(NIXIE_HANDLE* pSts, u8 nSts);

/** Write value in dynamic controling mode. */
bool nixieDriverWrite(u8 channel, u8 value);

/** !!! CALL THIS INTERVALLY IF USE OF DYNAMIC CONTROL. !!! */
void nixieDriverTimerCallback(void);

/** This function is for manually controling anode pins. */
void nixieDriverWriteAnode( u8 channel, u8 on );

/** This function is for manually control cathode pins. */
void nixieDriverWriteCathode(u8 val);

#endif /* _NIXIEDRIVER_H_ */
