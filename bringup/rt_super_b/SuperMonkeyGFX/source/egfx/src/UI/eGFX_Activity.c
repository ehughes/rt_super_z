#include "eGFX.h"

eGFX_ActivityRecord MyActivities[eGFX_MAX_SYSTEM_ACTIVITIES];


static  uint32_t        eGFX_ActivityStackPointer = 0;
static  uint32_t    	eGFX_ActivityStack[eGFX_ACTIVITY_STACK_DEPTH];
static  uint32_t        eGFX_CurrentActivityID = eGFX_ACTIVITY_ID_HOME;

uint32_t eGFX_GetCurrentActivity()
{
        return eGFX_CurrentActivityID;
}

void eGFX_ActivityStackReset()
{
        eGFX_ActivityStackPointer = 0;
}

void eGFX_InitActivities()
{
        eGFX_ActivityStackReset();
        eGFX_CurrentActivityID = eGFX_ACTIVITY_ID_HOME;
}

int32_t eGFX_ActivitySwitch(uint32_t NewActivityID,uint32_t MessageId,void *Message)
{
        if(NewActivityID >= eGFX_MAX_SYSTEM_ACTIVITIES)
        {
                NewActivityID = eGFX_ACTIVITY_ID_HOME;
                return eGFX_ACTIVITY_INVALID;
        }

        //Reset the stack!
         eGFX_ActivityStackReset();

        //Finish the current activity
        if(MyActivities[eGFX_CurrentActivityID].Exit != NULL_FUNC_PTR)
                MyActivities[eGFX_CurrentActivityID].Exit(eGFX_ACTIVITY_MSG_ID_SWITCH,(void *)NULL_FUNC_PTR);

        //Switch to the new activity
        eGFX_CurrentActivityID = NewActivityID;

        if(MyActivities[eGFX_CurrentActivityID].Enter != NULL_FUNC_PTR)
                        MyActivities[eGFX_CurrentActivityID].Enter(MessageId,Message);

        eGFX_ClearAllInputActivity();

        return eGFX_ACTIVITY_OK;
}

int32_t eGFX_ActivityPush(uint32_t NewActivityID,uint32_t MessageId,void *Message)
{

        if(NewActivityID >= eGFX_MAX_SYSTEM_ACTIVITIES)
        {
                return eGFX_ACTIVITY_INVALID;
        }

        //see if the stack is full
        if(eGFX_ActivityStackPointer >= eGFX_ACTIVITY_STACK_DEPTH-1)
        {
                return eGFX_ACTIVITY_STACK_FULL;
        }

        //Finish the current activity and tell it it will be pushed
        if(MyActivities[eGFX_CurrentActivityID].Exit != NULL_FUNC_PTR)
                MyActivities[eGFX_CurrentActivityID].Exit(eGFX_ACTIVITY_MSG_ID_PUSH,(void *)NULL_FUNC_PTR);

        //push the current activity on the stack
        eGFX_ActivityStack[eGFX_ActivityStackPointer] = eGFX_CurrentActivityID;
        eGFX_ActivityStackPointer++;

        //switch to the new activity
        eGFX_CurrentActivityID = NewActivityID;

        //send the push arguments to the new activity
        if(MyActivities[eGFX_CurrentActivityID].Enter != NULL_FUNC_PTR)
                 MyActivities[eGFX_CurrentActivityID].Enter(MessageId,Message);

        eGFX_ClearAllInputActivity();


        return eGFX_ACTIVITY_OK;

}

int32_t eGFX_ActivityPop(uint32_t MessageId,void *Message)
{

        //See if the stack is empty
        if(eGFX_ActivityStackPointer == 0)
        {
                //Finish the current activity and tell it will be switched
                if(MyActivities[eGFX_CurrentActivityID].Exit != NULL_FUNC_PTR)
                        MyActivities[eGFX_CurrentActivityID].Exit(eGFX_ACTIVITY_MSG_ID_SWITCH,(void *)NULL_FUNC_PTR);

                //Switch to the home screen
                eGFX_CurrentActivityID = eGFX_ACTIVITY_ID_HOME;

                if(MyActivities[eGFX_CurrentActivityID].Enter != NULL_FUNC_PTR)
                        MyActivities[eGFX_CurrentActivityID].Enter(eGFX_ACTIVITY_MSG_ID_RESET,(void *)NULL_FUNC_PTR);

                return eGFX_ACTIVITY_STACK_EMPTY;
        }

        //Exit the current activity.  We need to flag it that we are doing a pop (which is really an acknowledge)

        if(MyActivities[eGFX_CurrentActivityID].Exit != NULL_FUNC_PTR)
            MyActivities[eGFX_CurrentActivityID].Exit(eGFX_ACTIVITY_MSG_ID_POP,(void *)NULL_FUNC_PTR);

        //Get the activity off the stack
        eGFX_ActivityStackPointer--;

        eGFX_CurrentActivityID = eGFX_ActivityStack[eGFX_ActivityStackPointer];

        //Enter the new activity with the parameters returned from the old
        if(MyActivities[eGFX_CurrentActivityID].Enter != NULL_FUNC_PTR)
                MyActivities[eGFX_CurrentActivityID].Enter(MessageId,Message);

        eGFX_ClearAllInputActivity();

        return eGFX_ACTIVITY_OK;

}



void eGFX_RegisterActivity(	uint32_t ActivityID,
								 char * ActivityName,
								 void (*Enter)(uint32_t MessageID, void *Message),
								 void (*Process)(),
								 void (*Exit)(uint32_t MessageID, void *Message)
							)
{
	if(ActivityID<eGFX_MAX_SYSTEM_ACTIVITIES)
	{
		MyActivities[ActivityID].ActivityName = ActivityName;
		MyActivities[ActivityID].Enter = Enter;
		MyActivities[ActivityID].Process = Process;
		MyActivities[ActivityID].Exit = Exit;
	}
}



void eGFX_ProcessActivities()
{
    if(MyActivities[eGFX_CurrentActivityID].Process != NULL_FUNC_PTR)
    {
         MyActivities[eGFX_CurrentActivityID].Process();
    }
}

