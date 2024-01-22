#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"
#include "fsl_usart.h"



#define P39_PORT			    0
#define P39_PIN				    11

#define LCD_BL_PORT			    2
#define LCD_BL_PIN				7

volatile uint32_t g_systickCounter;

void SysTick_Handler(void)
{

	if (g_systickCounter != 0U)
    {
        g_systickCounter--;
    }
}


void SysTick_DelayTicks(uint32_t n)
{

    g_systickCounter = n;
    while (g_systickCounter != 0U)
    {
    }
}


int main(void)
{


    gpio_pin_config_t IO_OutputConfig =
										{
											kGPIO_DigitalOutput,
											0,
										};
    SYSCTL1->MCLKPINDIR = SYSCTL1_MCLKPINDIR_MCLKPINDIR_MASK;

    BOARD_InitDebugConsole();

    BOARD_InitPins();

    BOARD_BootClockRUN();

    GPIO_PortInit(GPIO, P39_PORT);
    GPIO_PinInit(GPIO, P39_PORT, P39_PIN, &IO_OutputConfig);

    GPIO_PortInit(GPIO, LCD_BL_PORT);
    GPIO_PinInit(GPIO, LCD_BL_PORT, LCD_BL_PIN, &IO_OutputConfig);


    SystemCoreClockUpdate();

    SysTick_Config(SystemCoreClock / 1000U);


    while (1)
    {

    	GPIO_PortToggle(GPIO, LCD_BL_PORT, 1u << LCD_BL_PIN);

    	GPIO_PortToggle(GPIO, P39_PORT, 1u << P39_PIN);

    	SysTick_DelayTicks(250);


    }
}
