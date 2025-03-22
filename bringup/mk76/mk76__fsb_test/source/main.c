#include <stdio.h>
#include "pin_mux.h"
#include "board.h"
#include "fsl_debug_console.h"
#include <stdbool.h>


volatile uint32_t g_systickCounter;

void SysTick_Handler(void)
{

	if (g_systickCounter != 0U)
    {
        g_systickCounter--;
    }
}

void System__Delay_mS(uint32_t n)
{
    g_systickCounter = n;
    while (g_systickCounter != 0U)
    {
    }
}




int main(void)
{

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    gpio_pin_config_t IO_OutputConfig =
 										{
 											kGPIO_DigitalOutput,
 											0,
 										};

   GPIO_PortInit(GPIO, 0);

   GPIO_PortInit(GPIO, 1);

   GPIO_PortInit(GPIO, 2);

   GPIO_PinInit(GPIO, BOARD_INITPINS_GREEN_LED_PORT, BOARD_INITPINS_GREEN_LED_PIN, &IO_OutputConfig);

   SystemCoreClockUpdate();

   SysTick_Config(SystemCoreClock / 1000U);

   while(1)
   {

	   System__Delay_mS(500);


       GPIO_PortToggle(GPIO, BOARD_INITPINS_GREEN_LED_PORT, 1u << BOARD_INITPINS_GREEN_LED_PIN);

   }
}

