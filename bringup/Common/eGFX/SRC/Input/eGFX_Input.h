#include "eGFX_InputTypes.h"
#include "stdint.h"

#ifndef _EGFX_INPUT_H_
#define _EGFX_INPUT_H_


void eGFX_InitButton(eGFX_Button *B,
                     uint8_t Port,
                     uint8_t Bit,
                     uint8_t Polarity,
                     uint8_t DebounceTicks,
                     char *ButtonID);


// Computes the Debounce algorithm  Must be done periodically
void eGFX_InputProcess(uint32_t mSecTicks);

void eGFX_ClearAllInputActivity();

//Indicates that the button is being help down
uint32_t eGFX_ButtonIsActive(eGFX_Button *B);

//Reads the button down flag.  Flag will be cleared after read
uint32_t eGFX_ButtonDown(eGFX_Button *B);

//Reads the button Up flag.  Flag will be cleared after read
//Returns the number of ticks the button was held for
uint32_t eGFX_ButtonUp(eGFX_Button *B);

//returns the current hold time from the debounce algorithm
uint32_t eGFX_ButtonCurrentHoldTime(eGFX_Button *B);

//resets the button state and waits for a release (button up but generates no up or down code
void eGFX_Button_ResetState (eGFX_Button *B);

void eGFX_ProcessButton(eGFX_Button *B);

void eGFX_ButtonProgrammaticDown(eGFX_Button *B);

void eGFX_ButtonProgrammaticUp(eGFX_Button *B);

void eGFX_ProcessEncoder(eGFX_Encoder *E);

void eGFX_InitEncoder(eGFX_Encoder *E,
					uint32_t PhaseA_GPIO_Port,
					uint32_t PhaseA_GPIO_Bit,
					uint32_t PhaseB_GPIO_Port,
					uint32_t PhaseB_GPIO_Bit,
					uint32_t Polarity,
					char * Name
					);

#endif
