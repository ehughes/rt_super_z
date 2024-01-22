#include "eGFX.h"

#include "board.h"

#include "eGFX_InputDriver_MM.h"

eGFX_Button MyButtons[eGFX_NUM_BUTTONS];

eGFX_Encoder MyEncoders[eGFX_NUM_ENCODERS];


void eGFX_InputInit()
{
	/*
	 eGFX_InitButton( &MyButtons[WHEEL_BUTTON],
			 	 	 MAIN_ENCODER_BTN_GPIO_PORT,
					 MAIN_ENCODER_BTN_GPIO_PIN	,
					eGFX_BUTTON_POLARITY_LOW_ACTIVE,
					eGFX_BUTTON_DEFAULT_DEBOUNCE_TICKS,
	                "WHEEL_BUTTON");
*/

}


uint32_t eGFX_InputHAL_ReadPortPin(uint8_t Port, uint8_t Pin)
{
     //  return (uint32_t)Chip_GPIO_ReadPortBit(LPC_GPIO_PORT,Port,Pin);

	return 0;
}



void eGFX_InputProcess(uint32_t mSecTicks)
{
        for(int i=0;i<eGFX_NUM_BUTTONS;i++)
        {
                eGFX_ProcessButton(&MyButtons[i]);
        }
/*
        for(int i=0;i<eGFX_NUM_ENCODERS;i++)
        {
                eGFX_ProcessEncoder(&MyEncoders[i]);
        }
        */
}

void eGFX_ClearAllInputActivity()
{
        for(int i=0;i<eGFX_NUM_BUTTONS;i++)
        {
                eGFX_Button_ResetState(&MyButtons[i]);
        }

}
