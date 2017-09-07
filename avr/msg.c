/*
 * FILE NAME : msg.c
 * 
 * SUMMARY   : Message Cycled Queue Lib.
 *
 * REMARKS   : Overflowed Message is ignored.
 *
 */


/*------------------------------------------------------------------------------
 * INCLUDE FILES
 *------------------------------------------------------------------------------*/
#include "msg.h"


/*------------------------------------------------------------------------------
 * MACRO DEFINE
 *------------------------------------------------------------------------------*/
#define MSG_CUE_SIZE    256


/*------------------------------------------------------------------------------
 * Struct Declaration
 *------------------------------------------------------------------------------*/
typedef struct _T_MSG{
    
    u8 index_post;
    u8 index_get;
    u8 msg[MSG_CUE_SIZE];

} T_MSG;


/*------------------------------------------------------------------------------
 * Variable Declaration
 *------------------------------------------------------------------------------*/
static T_MSG t_msg;					/* message control info. */

/*------------------------------------------------------------------------------
 * FUNCTION DEFINE
 *------------------------------------------------------------------------------*/

/**
 * SUMMARY：	get message
 *
 * ARGUMENTS：	p_msg				［出力］Pointer to message.
 *
 * RETURN：		APP_OK			valid message
 *				APP_ERR_FAIL	invalid message(includes no message)
 *
 * REMARKS：	Must be called in interrupt enabled state.
 *
 */
APPRESULT msg_init(void)
{
    u16 i = 0;
    
    t_msg.index_post = 0;
    t_msg.index_get  = 0;
    for (i=0; i<MSG_CUE_SIZE; i++){
        t_msg.msg[i] = MSG_INVALID;
    }
    
	return MSG_OK;    
}

/**
 * SUMMARY：	get message
 *
 * ARGUMENTS：	p_msg				［出力］Pointer to message.
 *
 * RETURN：		APP_OK			valid message
 *				APP_ERR_FAIL	invalid message(includes no message)
 *
 * REMARKS：	Must be called in interrupt enabled state.
 *
 */
APPRESULT msg_get(u8 *p_msg){

	T_MSG *p = &t_msg;

	/* no msg ? */
	if(p->index_get == p->index_post){
		return MSG_NOMSG;
	}

	/* interrupt disabled here */

	/* get msg */
	*p_msg = p->msg[p->index_get];

	/* update pointer */
	p->index_get++;
	if(p->index_get >= sizeof(p->msg)){
		p->index_get = 0;
	}

	/* enable interrupt */

	return MSG_OK;
}

/**
 * SUMMARY：	post message
 *
 * ARGUMENTS：	msg	［INPUT］post message
 *
 * RETURN：		APP_OK	OK
 *
 * REMARKS：	Must be called on interrupt disabled state.
 *
 */
APPRESULT msg_post(u8 msg){

	T_MSG *p = &t_msg;
	u8 temp_index = p->index_post;

	/* Any rooms in msg queue ? */
	temp_index++;
	if(temp_index >= sizeof(p->msg)){
		temp_index = 0;
	}

	if(temp_index != p->index_get){

		/* set msg if ok.. */
		p->msg[p->index_post] = msg;

		/* update pointer */
		p->index_post = temp_index;
	}
    else {
        
        /* MSG CUE OVERFLOW OCCURED */
        return MSG_OVERFLW;
    }

	return MSG_OK;
}

