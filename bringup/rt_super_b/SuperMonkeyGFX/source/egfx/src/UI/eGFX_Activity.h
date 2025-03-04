/*
 * Actitivy.h
 *
 *  Created on: Jun 30, 2014
 *      Author: ehughes
 */

#include "stdint.h"

#ifndef eGFX_ACTITIVY_H_
#define eGFX_ACTITIVY_H_


#define eGFX_ACTIVITY_MSG_ID_CUSTOM_BASE		0x1000000

#define eGFX_ACTIVITY_ID_HOME		     0x0			//Home is always zero!


#define	eGFX_ACTIVITY_MSG_ID_RESET	0xFFFFFFFF
#define	eGFX_ACTIVITY_MSG_ID_NO_MSG	0xFFFFFFFE
#define	eGFX_ACTIVITY_MSG_ID_POP	0xFFFFFFFD
#define	eGFX_ACTIVITY_MSG_ID_PUSH	0xFFFFFFFC	//Not really needed
#define	eGFX_ACTIVITY_MSG_ID_SWITCH	0xFFFFFFFB
#define	eGFX_ACTIVITY_MSG_ID_STRING	0xFFFFFFFA  //This is for sending a string to an activity


#define eGFX_ACTIVITY_STACK_FULL		-1
#define eGFX_ACTIVITY_STACK_EMPTY		-2
#define eGFX_ACTIVITY_INVALID			-3
#define eGFX_ACTIVITY_OK				 0


/*
   Struct: ActivityRecord

	This structure is used to keep track of activity.   A system will have an array of these records
	that is statically allocated for the GUI/menu system



   Fields:


      Enter - A function pointer to the activity's Enter routine
       	   	  This is called every time a new activity is entered.  A generic int and pointer are used to init the activity.
 	 	 	  This is called an activity message. A global database of these messages will be kept in Activity.h
			  If the MessageID is ACTIVITY_MSG_ID_RESET, then that is considered a full "reset" of the activity
			  If the MessageID is ACTIVITY_MSG_ID_NO_MSG,  then then there is no message

			  There is no return value for this function pointer

	  Process - A function pointer to the Activity's process routine.    Here the Activity must do all of it's functions (draw, check buttons)

				There is no return value for this function pointer



	 Exit - A function pointer to the Activity's exit routine.  This routine will be called as a result of an ActivityPop or and ActivitySwitch.
	 	 	caller may populate the arguments with a message of pop, switch, etc.


   See Also:

      <ActivitySwitch> <ActivityPush> <ActivityPop>
*/


typedef struct
{
    char * ActivityName;

    void (*Enter)(uint32_t MessageID, void *Message);

	void (*Process)();

	void (*Exit)(uint32_t MessageID, void *Message);

}eGFX_ActivityRecord;


/*
   Function: ActivitySwitch

	This function will do a full switch to another Activity

	Important!

	-The new activity will have the message  arguments sent through it's "Enter" routine
	-The Activity stack will be *blown away*, reseting the stack pointer to zero
	-The current activity's exit routine will be called with the message ID of ACTIVITY_MSG_ID_SWITCH
	-If the new activity ID is greater than the number of actual activities,   the HOME activity will be used.
   Parameters:

      NewActivityID  -  The new Activity ID to switch to
      MessageId - Message ID of the message to send the new activity
      Message - Pointer to the message data to send to the new activity

   Returns:

      ACTIVITY_OK				 on success.
      ACTIVITY_INVALID 			 if NewActivityID is invalid

   See Also:

      <ActivitySwitch>
*/

int32_t eGFX_ActivitySwitch(uint32_t NewActivityID,uint32_t MessageId,void *Message);



/*
   Function: ActivityPush

	This function will push the current activity to the activity *stack*.
	This is used for make an easy way for one to navigate back to previous activities
	The caller will select the new activity as well as the message

	Important!

	-The new activity will have this message sent through it's "Enter" routine
	-When the new activity "pops", a message will be sent to the caller "Enter message"
	-If the Activity Stack is full, call will be ignored
	-If the Activity is invalid,  the call will be ignored

   Parameters:

      NewActivityID  -  The new Activity ID to switch to
      MessageId - Message ID of the message to send the new activity
      Message - Pointer to the message data to send to the new activity

   Returns:

      ACTIVITY_OK				 on success.
      ACTIVITY_STACK_FULL	  	 if stack is full
      ACTIVITY_INVALID 			 if NewActivityID is invalid

   See Also:

      <ActivitySwitch>
*/

int32_t eGFX_ActivityPush(uint32_t NewActivityID,uint32_t MessageId,void *Message);

/*
   Function: ActivityPop

	This function will pop the current activity to the activity *stack*.
	This is used for make an easy way for one to navigate back to previous activities


	Important!

	-The current acitivity's exit routine will be called before the pop.  The exit "out" message will be send to the Activity is popped from the stack
	-If  the stack is *empty*,   an ActivitySwitch to the "HOME" activity will be called.

   Parameters:

	 A message ID and pointer.   These will get passed to the activity getting off the stack.

   Returns:

      ACTIVITY_OK				 on success.
      ACTIVITY_STACK_EMPTY 	  	 if stack is empty
      ACTIVITY_INVALID 			 if NewActivityID is invalid

   See Also:

      <ActivityPush>
*/
int32_t eGFX_ActivityPop(uint32_t MessageId,void *Message);


/*
   Function: InitActivities

	Call at bootup.  initializes the Activity system

   Parameters:

	Nothing

   Returns:

	Nothing

   See Also:

      <ProcessActivities>
*/
void eGFX_InitActivities();

/*
   Function: eGFX_ProcessActivities

	Call this at UI update rate

   Parameters:

	Nothing

   Returns:

	Nothing

   See Also:

      <InitActivities>
*/
void eGFX_ProcessActivities();

uint32_t eGFX_GetCurrentActivity();

void eGFX_RegisterActivity(	uint32_t ActivityID,
								 char * ActivityName,
								 void (*Enter)(uint32_t MessageID, void *Message),
								 void (*Process)(),
								 void (*Exit)(uint32_t MessageID, void *Message)
							);

#ifndef NULL_FUNC_PTR
	#define NULL_FUNC_PTR	((void *)0)
#endif

#endif /* ACTITIVY_H_ */
