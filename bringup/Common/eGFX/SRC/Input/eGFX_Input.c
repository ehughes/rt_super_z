#include "eGFX.h"
#include "string.h"

#define BUTTON_STATE_WAIT_FOR_PRESS                             0
#define BUTTON_STATE_WAIT_FOR_WAIT_FOR_PRESS_STABLE             1
#define BUTTON_STATE_WAIT_FOR_RELEASE                           2
#define BUTTON_STATE_WAIT_FOR_STABLE                            3
#define BUTTON_STATE_WAIT_FOR_STABLE_GENERATE_NO_CODES          4



void eGFX_InitButton(eGFX_Button *B,
                uint8_t Port,
                uint8_t Bit,
                uint8_t Polarity,
                uint8_t DebounceTicks,
                char *ButtonID)
{
    if(B!=0)
    {

        B->Port = Port;
        B->Bit = Bit;
        B->Polarity = Polarity;
        B->DebounceTicks = DebounceTicks;
        B->Down = 0;
        B->Up = 0;
        B->HoldTime = 0;
        B->Locked = 0;
        B->State = BUTTON_STATE_WAIT_FOR_PRESS;
        B->DebounceTicker = 0;

        if(ButtonID == (char *)NULL)
            strcpy(ButtonID,"Unknown");
        else
            B->ButtonID = ButtonID;
    }
}
 

uint32_t eGFX_ButtonHAL(eGFX_Button *B)
{
   uint32_t Output;
   uint32_t P;
     
   Output = eGFX_InputHAL_ReadPortPin(B->Port,B->Bit); 

   if(B->Polarity == eGFX_BUTTON_POLARITY_LOW_ACTIVE)
    {
        if(Output > 0)
        {
            P = eGFX_BUTTON_NOT_PRESSED;
        }
        else
        {
            P = eGFX_BUTTON_PRESSED;
        }
    }
    else
    {
        if(Output == 0)
        {
            P = eGFX_BUTTON_NOT_PRESSED;
        }
        else
        {
            P = eGFX_BUTTON_PRESSED;
        }
    }

  return P;

}



// Computes the Debounce algorithm. Must be called periodically
void eGFX_ProcessButton(eGFX_Button *B)
{
    B->Locked = eGFX_TRUE;

    switch(B->State)
    {
        default:
        case BUTTON_STATE_WAIT_FOR_PRESS:
            if(eGFX_ButtonHAL(B) == eGFX_BUTTON_PRESSED)
            {
                B->State = BUTTON_STATE_WAIT_FOR_WAIT_FOR_PRESS_STABLE;
                B->DebounceTicker = 0;
            }

            break;

        case BUTTON_STATE_WAIT_FOR_WAIT_FOR_PRESS_STABLE:
            if(eGFX_ButtonHAL(B) == eGFX_BUTTON_PRESSED)
            {
                B->DebounceTicker++;

                if(B->DebounceTicker > B->DebounceTicks)
                {
                    B->State = BUTTON_STATE_WAIT_FOR_RELEASE;
                    B->HoldTime = B->DebounceTicker;
                    B->Down = eGFX_TRUE;

                }
            }

            else
            {
                B->State = BUTTON_STATE_WAIT_FOR_PRESS;
            }

            break;

        case BUTTON_STATE_WAIT_FOR_RELEASE:
            if(eGFX_ButtonHAL(B) == eGFX_BUTTON_PRESSED)
            {
                if(B->HoldTime<0xFFFF)
                    B->HoldTime++;
            }

            else
            {
                B->State = BUTTON_STATE_WAIT_FOR_STABLE;
                B->DebounceTicker = 0;
            }

            break;

        case BUTTON_STATE_WAIT_FOR_STABLE:
            if(eGFX_ButtonHAL(B) == eGFX_BUTTON_NOT_PRESSED)
            {

                B->DebounceTicker++;

                if(B->DebounceTicker > B->DebounceTicks)
                {
                    B->State = BUTTON_STATE_WAIT_FOR_PRESS;
                    B->Up = eGFX_TRUE;

                }
            }

            else
            {
                B->DebounceTicker = 0;
            }

            break;

        case BUTTON_STATE_WAIT_FOR_STABLE_GENERATE_NO_CODES:

                if(eGFX_ButtonHAL(B) == eGFX_BUTTON_NOT_PRESSED)
                            {
                                B->DebounceTicker++;

                                if(B->DebounceTicker > B->DebounceTicks)
                                {
                                    B->State = BUTTON_STATE_WAIT_FOR_PRESS;
                                }
                            }

                            else
                            {
                                B->DebounceTicker = 0;
                            }

                break;


    }

    B->Locked = eGFX_FALSE;
}

//Indicates that the button is being help down
uint32_t eGFX_ButtonIsActive(eGFX_Button *B)
{
    if(B->State == BUTTON_STATE_WAIT_FOR_RELEASE)
        return eGFX_TRUE;

    else
        return eGFX_FALSE;
}

//Reads the button down flag.  Flag will be cleared after read
uint32_t eGFX_ButtonDown(eGFX_Button *B)
{
    uint8_t P = eGFX_FALSE;

    if(B->Locked == eGFX_FALSE)
    {
        if(B->Down == eGFX_TRUE)
        {
            P = eGFX_TRUE;
            B->Down = eGFX_FALSE;
        }
    }

    else
    {
        P = eGFX_FALSE;
    }

    return P;
}

//Reads the button Up flag.  Flag will be cleared after read
//Returns the number of ticks the button was held for
uint32_t eGFX_ButtonUp(eGFX_Button *B)
{
    uint16_t P = eGFX_FALSE;

    if(B->Locked == eGFX_FALSE)
    {
        if(B->Up == eGFX_TRUE)
        {
            P = B->HoldTime;
            B->Up = eGFX_FALSE;
        }
    }

    else
    {
        P = 0;
    }

    return P;
}


void eGFX_ButtonProgrammaticDown(eGFX_Button *B)
{
    B->State = BUTTON_STATE_WAIT_FOR_RELEASE;
    B->HoldTime = 0;
    B->Down = eGFX_TRUE;

}

void eGFX_ButtonProgrammaticUp(eGFX_Button *B)
{
    B->State = BUTTON_STATE_WAIT_FOR_PRESS;
    B->Up = eGFX_TRUE;
}

//returns the current hold time from the debounce algorithm
uint32_t eGFX_ButtonCurrentHoldTime(eGFX_Button *B)
{
    uint16_t P = eGFX_FALSE;

    if(B->Locked == eGFX_FALSE)
    {
        if(B->State == BUTTON_STATE_WAIT_FOR_RELEASE)
        {
            P = B->HoldTime;
        }
    }

    else
    {
        P = 0;
    }

    return P;
}


void eGFX_Button_ResetState (eGFX_Button *B)
{
        B->State = BUTTON_STATE_WAIT_FOR_STABLE_GENERATE_NO_CODES;
        B->Up = 0;
        B->Down = 0;
        B->HoldTime = 0;
}


const  int32_t EncoderTable[16] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};

void eGFX_InitEncoder(eGFX_Encoder *E,
					uint32_t PhaseA_GPIO_Port,
					uint32_t PhaseA_GPIO_Bit,
					uint32_t PhaseB_GPIO_Port,
					uint32_t PhaseB_GPIO_Bit,
					uint32_t Polarity,
					char * Name
					)
{
	if(E!=NULL)
	{
		E->PhaseA_GPIO_Port = PhaseA_GPIO_Port;
		E->PhaseA_GPIO_Bit = PhaseA_GPIO_Bit;
		E->PhaseB_GPIO_Port = PhaseB_GPIO_Port;
		E->PhaseB_GPIO_Bit = PhaseB_GPIO_Bit;
		E->Polarity = Polarity;

		if(Name != NULL)
		{
			E->Name = Name;
		}
		else
		{
			E->Name = "Unknown";
		}

		E->State = 0;
	}
}

void eGFX_ProcessEncoder(eGFX_Encoder *E)
{
	if(E != NULL)
	{

		E->State = E->State<<2;


		if(E->Polarity == eGFX_BUTTON_POLARITY_HIGH_ACTIVE)
		{
			if(eGFX_InputHAL_ReadPortPin(E->PhaseA_GPIO_Port,E->PhaseA_GPIO_Bit))
					E->State |= 0x2;

				if(eGFX_InputHAL_ReadPortPin(E->PhaseB_GPIO_Port,E->PhaseB_GPIO_Bit))
					E->State |= 0x1;
		}
		else
		{
			if(eGFX_InputHAL_ReadPortPin(E->PhaseA_GPIO_Port,E->PhaseA_GPIO_Bit) == 0)
			{
					E->State |= 0x2;
			}

			if(eGFX_InputHAL_ReadPortPin(E->PhaseB_GPIO_Port,E->PhaseB_GPIO_Bit) == 0)
			{
				E->State |= 0x1;
			}
		}


		E->State &= 0xF;


		E->Count +=	EncoderTable[E->State & 0xF];
	}

}



