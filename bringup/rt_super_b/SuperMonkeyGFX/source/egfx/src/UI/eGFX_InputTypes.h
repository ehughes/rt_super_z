#include "stdint.h"

#ifndef eGFX_INPUT_TYPES
#define eGFX_INPUT_TYPES

#ifndef TRUE
 // #define TRUE 1
#endif

#ifndef FALSE
//  #define FALSE 0
#endif

#define eGFX_BUTTON_POLARITY_LOW_ACTIVE                          0
#define eGFX_BUTTON_POLARITY_HIGH_ACTIVE                         1

#define eGFX_BUTTON_PRESSED                                      1
#define eGFX_BUTTON_NOT_PRESSED                                  0



typedef struct
{
    volatile uint8_t Down;      //Flag to indicate button was pressed down
    volatile uint8_t Up;        //Flag to indicate button was release

    uint8_t Port;       //GPIO port of the button
    uint8_t Bit;        //GPIO bit of the button

    volatile uint16_t   HoldTime;       //Number of ticks the butt was pressed for
    int16_t   DebounceTicks; //Number of ticks for the debounce time

    volatile uint16_t   DebounceTicker; //used internally for the debouncer alogirhtm

    volatile uint8_t State;             //State of the debounce algorithm

    uint8_t   Polarity; //is the button active high (>0) or active low (0)

    volatile uint8_t    Locked;   //Flag to indicate the struct is beign processed bu the Processbutton() function

    char *ButtonID;

} eGFX_Button;


typedef struct
{
    volatile int32_t Count;

    uint8_t PhaseA_GPIO_Port;
    uint8_t PhaseA_GPIO_Bit;

    uint8_t PhaseB_GPIO_Port;
    uint8_t PhaseB_GPIO_Bit;

    char *Name;

    uint32_t State;

    uint8_t   Polarity; //is the IO active high (>0) or active low (0)

} eGFX_Encoder;



#endif
