
#include "Input/eGFX_InputTypes.h"


#ifndef _eGFX_INPUT_DRIVER_TC_H
#define _eGFX_INPUT_DRIVER__TC_H


/*
    An input driver must expose a macro with the number of buttons  
*/

#define eGFX_NUM_BUTTONS					1

#define eGFX_BUTTON_DEFAULT_DEBOUNCE_TICKS	2
/*
    An input driver must expose an array of eGFX_Button Objects
*/

extern eGFX_Button MyButtons[eGFX_NUM_BUTTONS];


#define eGFX_NUM_ENCODERS					    1

extern eGFX_Encoder MyEncoders[eGFX_NUM_ENCODERS];

/*
    eGFX_InputInit() must init any related input hardware and setup the button objects
*/
void eGFX_InputInit();


/*
    eGFX_InputHAL_ReadPortPin() should read a physical port/pin.   The physical meaning
    of port/pin is system dependent.    They are stored with each button object and should
    translate to a port and pin on a microcontroller.
*/

uint32_t eGFX_InputHAL_ReadPortPin(uint8_t Port, uint8_t Pin);

/*
 * This function should be used to handle an continuous process.
 * At a minimum it should loop through all the input objects and process events
 */
void eGFX_InputProcess(uint32_t mSecTicks);


/*
 * This function should be used clear all input processing state
 */

void eGFX_ClearAllInputActivity();


#define WHEEL_BUTTON	0

#define WHEEL_ENCODER	0


#endif

