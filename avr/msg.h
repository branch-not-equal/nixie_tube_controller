/*
 * FILE NAME : msg.h
 * 
 * SUMMARY   : Message Cycled Queue Lib. HEADER FILE
 *
 * REMARKS   : Define Cue Size in "msg.c"
 *
 */
#ifndef MSG_H
#define MSG_H

#include "macrodriver.h"

/* MSG IDENTIFIER       */
enum {
    MSG_1secInterval,
    MSG_BTN,
    MSG_ENC,
    MSG_NIXIEANODE,
    MSG_NIXIECATHODE,
    MSG_INVALID
};

/* RETURN PARAMETERS     */
enum {
    MSG_OK,             /* OK                   */
    MSG_NOMSG,          /* NO MESSAGE       */
    MSG_OVERFLW,        /* MSG CUE OVERFLOW   */
};

/* INITIALIZE MSG	*/
APPRESULT msg_init(void);

/* POST MSG	*/
APPRESULT msg_post(u8 msg);

/* GET MSG	*/
APPRESULT msg_get(u8* p_msg);

#endif	/* MSG_H*/
