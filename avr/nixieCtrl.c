/*****************************************************************************/
/**
* @file nixieCtrl.c
*
* This file inprements nixie tube control functions.
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
*****************************************************************************/

/***************************** Include Files *********************************/
#include "nixieCtrl.h"
#include <stdlib.h>

/************************** Constant Definitions *****************************/

#define DIGIT_MAX 9

NIXIE_HANDLE nixieStatus[] = {
    {0, 0, TRUE},
    {1, 0, TRUE},
    {2, 0, TRUE},
    {3, 0, FALSE},
    {4, 0, FALSE},
    {5, 0, FALSE},
    {6, 0, FALSE},    
    {7, 0, FALSE}
};


/**
 * Parameter on control single nixie tube.
 */
typedef struct {
    u8  channel;
    u16 value;
    u16 count;

} ctrlParam;



/************************** Function Prototypes ******************************/
static int  oneOverFNoiseUnit(int min, int max);
static void blinkIntervally(ctrlParam* param, u16 countmax);
static void blinkOneOverFNoise(ctrlParam* param, u16 fmin, u16 fmax);

/************************** Variable Definitions *****************************/

/**
 * [Interval Function!!] Nixie control sample.. 
 * Just counting up; ch0->intervally, ch1->randomly.
 * @param    None.
 * @return   None.
 */
void nixieCtrl()
{

    static ctrlParam ch0 = {0, 0, 10};
    static ctrlParam ch1 = {1, 0, 10};
    static ctrlParam ch2 = {2, 0, 10};
#if 0
    /* static u16 value_0 = 0; */
    /* static u16 value_1 = 0; */
    /* static u16 count_0 = 10; */
    /* static u16 count_1 = 10; */
    
    if (ch0.count == 0){
        
        nixieDriverWrite(ch0.channel, ch0.value++);
        ch0.value = 9 < ch0.value ? 0 : ch0.value;
        ch0.count = 10; /* Assuming that this func is called 100 msec interval,
                         counts up channel 0 by 1sec */
/*         DPRINT(("ch0.value %d\r", ch0.value)); */

    }
    if (ch1.count == 0){
        
        nixieDriverWrite(ch1.channel, ch1.value++);
        ch1.value = 9 < ch1.value ? 0 : ch1.value;
        ch1.count = oneOverFNoiseUnit(2, 50); // interval 200msec ~ 5000msec
        ch1.count = 100 < ch1.count ? 100 : ch1.count;

/*         DPRINT(("ch1.value %d\r", ch1.value)); */
    }
    if (ch2.count == 0){
        
        nixieDriverWrite(ch2.channel, ch2.value++);
        ch2.value = 9 < ch2.value ? 0 : ch2.value;
        ch2.count = oneOverFNoiseUnit(2, 100); // interval 200msec ~ 5000msec
        ch2.count = 100 < ch2.count ? 100 : ch2.count;

/*         DPRINT(("ch1.value %d\r", ch1.value)); */
    }

    ch0.count--;
    ch1.count--;
    ch2.count--;
#else
    //blinkIntervally(&ch0, 10);
    blinkOneOverFNoise(&ch0, 2, 50);
    blinkOneOverFNoise(&ch1, 2, 50);
    blinkOneOverFNoise(&ch2, 2, 50);
#endif
}


/**
 * initialize hardware and software.
 */
void nixieCtrlInitialize(void)
{
    /* nixie driver port initialize */
    //// port output configuration
    DDRB |= (1 << NIXIEDRIVER_PIN_ANODE_0) | 
        (1 << NIXIEDRIVER_PIN_ANODE_1) | 
        (1 << NIXIEDRIVER_PIN_ANODE_2) |
        (1 << NIXIEDRIVER_PIN_ANODE_G);
    DDRD |= (1 << NIXIEDRIVER_PIN_CATHODE_0) | 
        (1 << NIXIEDRIVER_PIN_CATHODE_1) |
        (1 << NIXIEDRIVER_PIN_CATHODE_2) |
        (1 << NIXIEDRIVER_PIN_CATHODE_3) ;

    if(!nixieDriverInitialize(nixieStatus, sizeof(nixieStatus)/sizeof(NIXIE_HANDLE))){
        DPRINT(("!!! FAILED TO INITIALIZE NIXIE DRIVER....!!!\r"));
    }
}



/**
 * get 1/f random value by **** method.
 */
static int oneOverFNoiseUnit(int min, int max)
{
#ifndef FLOAT_CHAOS
    
    static int x = 0;

    if (x < max/2) {
        x = x + 2 * x * x ;
    } else {
        x = x - 2 * (max - x) * (max - x);
        }
    // Install random because x tends to fall into spot near 1.0 and 0.0
    if ((x <= min + max/100) || ((max-max/100) < x)) {
        x = rand() % (max-min) + min;
    }

    return x;
    
#else // use of float value.
    
    static float x   = 0.0;
    
    if (x < 0.5) {
        x = x + 2 * x * x ;
    } else {
        x = x - 2 * (1.0 - x) * (1.0 - x);
    }
    // Install random because x tends to fall into spot near 1.0 and 0.0
    if ((x < 0.005) || (0.999 < x)) {
        x = (float)(rand() % 256 / 256);
    }

    x *= max;
    
    return (x < min) ? min : (int)x;
#endif
}


/**
 * Assuming that this func is called 100 msec interval,
 *                        counts up channel 0 by 1sec
 */
static void blinkIntervally(ctrlParam* param, u16 countmax)
{
    if (param->count == 0){
        
        nixieDriverWrite(param->channel, param->value++);
        param->value = DIGIT_MAX < param->value ? 0 : param->value;
        param->count = countmax; 
/*         DPRINT(("param->value %d\r", param->value)); */

    }
    param->count--;
}


static void blinkOneOverFNoise(ctrlParam* param, u16 fmin, u16 fmax)
{

    if (param->count == 0){
        
        nixieDriverWrite(param->channel, param->value++);
        param->value = DIGIT_MAX < param->value ? 0 : param->value;
        param->count = oneOverFNoiseUnit(fmin, fmax); // interval fmin*100msec ~ fmax*100msec
        param->count = fmax < param->count ? fmax : param->count;

/*         DPRINT(("param->value %d\r", param->value)); */
    }
    param->count--;
}    
