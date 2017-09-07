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
* ----- ----------------- -------- ------------------------------------------
* 1.00a bne               12/04/12 First release
* </pre>
*/

/*---------------------------------------------------------------------------*/
/* INCLUDE FILES */
/*---------------------------------------------------------------------------*/
#include "nixieDriver.h"

/*---------------------------------------------------------------------------*/
/* MACRO/TYPE DEFINES */
/*---------------------------------------------------------------------------*/

/**
   ANODE : BCD Buffer - TLP227G
 */
#define PORT_ANODE_MASK              \
    ((1 << NIXIEDRIVER_PIN_ANODE_G)| \
     (1 << NIXIEDRIVER_PIN_ANODE_2)|      \
     (1 << NIXIEDRIVER_PIN_ANODE_1)|      \
     (1 << NIXIEDRIVER_PIN_ANODE_0))
/**
   CATHODE : 74141
 */
#define PORT_CATHODE_MASK                       \
    ((1 << NIXIEDRIVER_PIN_CATHODE_3)|          \
     (1 << NIXIEDRIVER_PIN_CATHODE_2)|          \
     (1 << NIXIEDRIVER_PIN_CATHODE_1)|          \
     (1 << NIXIEDRIVER_PIN_CATHODE_0))


/*---------------------------------------------------------------------------*/
/* VARIABLE DEFINITIONS */
/*---------------------------------------------------------------------------*/
/**
 * tube status
 * each status indicates tube status.
 */
static NIXIE_HANDLE*  pNixieStatus;
static u8             nNixieStatus;

/*---------------------------------------------------------------------------*/
/* FUNCTION PROTOTYPES */
/*---------------------------------------------------------------------------*/

static void nixieDriverDynamicCtrl( u8 on );

/*---------------------------------------------------------------------------*/
/* PUBLIC FUNCTION */
/*---------------------------------------------------------------------------*/

/**
* all pins are controlled as BCD value.
* @param    None.
* @return   None.
*/
bool nixieDriverInitialize(NIXIE_HANDLE* pSts, u8 nSts)
{
    int i = 0;
    
    if (nSts <= 0 || NIXIEDRIVER_ANODE_MAX < nSts){
        return FALSE;
    }
    if (!pSts){
        return FALSE;
    }
    
    // port configuration
    pNixieStatus = pSts;
    nNixieStatus = nSts;

    // initialize port
    for (i = 0; i < nNixieStatus; i++){
        if (pSts[i].valid == TRUE){
            nixieDriverWriteAnode(pSts[i].channel, FALSE);
        }
    }
    nixieDriverWriteCathode(0);

    return TRUE;
}


/**
* To be callbacked interval... control anode on/off
* @param    None.
* @return   None.
*/
void nixieDriverTimerCallback(void)
{
    static u8 on = 0x00;
    
    on ^= 0x01;
    nixieDriverDynamicCtrl(on);
}

/**
* 
* @param    None.
* @return   None.
*/
bool nixieDriverWrite(u8 channel, u8 value)
{
    if (nNixieStatus <= channel){
        return FALSE;
    }
    else {
        pNixieStatus[channel].value = value;
        return TRUE;
    }
}

/**
* control anode on/off, logic negative
* @param    None.
* @return   None.
*/
void nixieDriverWriteAnode( u8 channel, u8 on )
{
    if (on){
        u8 data =
            (1 << NIXIEDRIVER_PIN_ANODE_G) |
            (((channel & 0x01) >> 0) << NIXIEDRIVER_PIN_ANODE_0) |
            (((channel & 0x02) >> 1) << NIXIEDRIVER_PIN_ANODE_1) |
            (((channel & 0x04) >> 2) << NIXIEDRIVER_PIN_ANODE_2);

        NIXIEDRIVER_SETPORT(
             NIXIEDRIVER_POUT_ANODE,
            (NIXIEDRIVER_POUT_ANODE & (~PORT_ANODE_MASK)) | data );
    }
    else {
        // disable 74138 
        NIXIEDRIVER_SETPORT(
            NIXIEDRIVER_POUT_ANODE,
            NIXIEDRIVER_POUT_ANODE & (~(1 << NIXIEDRIVER_PIN_ANODE_G)));
    }

    return;
}

/**
* To be callbacked interval... control anode on/off
* @param    None.
* @return   None.
*/
void nixieDriverWriteCathode(u8 val)
{
    u8 data = 0;

    if (0 <= val && val <= 9){
        data =
            (((val & 0x01) >> 0) << NIXIEDRIVER_PIN_CATHODE_0) |
            (((val & 0x02) >> 1) << NIXIEDRIVER_PIN_CATHODE_1) |
            (((val & 0x04) >> 2) << NIXIEDRIVER_PIN_CATHODE_2) |
            (((val & 0x08) >> 3) << NIXIEDRIVER_PIN_CATHODE_3);
    }
    else {
        // data = ????writing none to cathode (does 74151 has that option ???)
        
    }

    //DPRINT(("CATHODE %x POUT %x (%x)\r", val, data, NIXIEDRIVER_POUT_CATHODE));    
    
    NIXIEDRIVER_SETPORT(
         NIXIEDRIVER_POUT_CATHODE,
        (NIXIEDRIVER_POUT_CATHODE & ~PORT_CATHODE_MASK) | data);
}

/*---------------------------------------------------------------------------*/
/* STATIC FUNCTION */
/*---------------------------------------------------------------------------*/

/**
* To be callbacked interval... control anode on/off
* @param    None.
* @return   None.
*/
static void nixieDriverDynamicCtrl( u8 on )
{
    static u16 index = 0;

/*     DPRINT(("INIT %d %d ", on, index)); */

    if (on == FALSE){
        
        //nixieDriverWriteCathode(0x00);
        nixieDriverWriteAnode(pNixieStatus[index].channel, FALSE);
    }
    else {

/*         DPRINT(("pSts[%d] %d %d %d ", */
/*                 index, */
/*                 pNixieStatus[index].channel, */
/*                 pNixieStatus[index].value, */
/*                 pNixieStatus[index].valid)); */
        
        // cathode write
        nixieDriverWriteCathode(pNixieStatus[index].value);
        // anode on
        nixieDriverWriteAnode(pNixieStatus[index].channel, TRUE);
        
        while (++index != nNixieStatus){
            if (pNixieStatus[index].valid){
                break;
            }
        }
        index = (nNixieStatus <= index) ? 0 : index;
    }
    
/*     DPRINT(("\r")); */
}


