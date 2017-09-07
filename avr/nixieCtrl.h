/*****************************************************************************/
/**
* @file prtcl.c
*
* This file inprements protocol control functions.
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
#ifndef _NIXIECTRL_CTRL_H_
#define _NIXIECTRL_CTRL_H_


/***************************** Include Files *********************************/
#include "macrodriver.h"
#include "nixieDriver.h"

/************************** Constant Definitions *****************************/


/**************************** Type Definitions *******************************/


/************************** Variable Definitions *****************************/

/**
* Nixie control sample.. 
* Just counting up; ch0->intervally, ch1->randomly.
* @param    None.
* @return   None.
******************************************************************************/
void nixieCtrl(void);

/**
* initialize hardware and software.
******************************************************************************/
void nixieCtrlInitialize(void);


#endif /* _NIXIECTRL_CTRL_H_ */
