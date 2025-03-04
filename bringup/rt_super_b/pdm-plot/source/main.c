#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"
#include "eGFX_Driver_ST7789_240_240_BD.h"
#include "eGFX.h"
#include "fsl_usart.h"
#include "Sprites_16BPP_RGB565.h"
#include "dmic_test.h"
#include "math.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_BOARD_TEST_LED_PORT 0
#define APP_BOARD_TEST_LED_PIN  11
#define APP_SW_PORT             BOARD_SW2_GPIO_PORT
#define APP_SW_PIN              BOARD_SW2_GPIO_PIN

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint32_t g_systickCounter;

/*******************************************************************************
 * Code
 ******************************************************************************/
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


float hann[240];
int16_t plot_list_l[240];
int32_t x = 0;
int32_t y = 0;

int main(void)
{
	eGFX_ImagePlane *Next;

   uint32_t port_state = 0;


    /* Define the init structure for the output LED pin*/
    gpio_pin_config_t led_config = {
        kGPIO_DigitalOutput,
        0,
    };

    BOARD_InitDebugConsole();


    /* Board pin, clock, debug console init */
    BOARD_InitPins();

    BOARD_BootClockRUN();

    /* Init output LED GPIO. */
    GPIO_PortInit(GPIO, 0);
    GPIO_PortInit(GPIO, 1);
    GPIO_PortInit(GPIO, 2);
    GPIO_PinInit(GPIO, APP_BOARD_TEST_LED_PORT, APP_BOARD_TEST_LED_PIN, &led_config);
    GPIO_PinInit(GPIO, 0, 5, &led_config);
    SystemCoreClockUpdate();

    SysTick_Config(SystemCoreClock / 1000U);

    SysTick_DelayTicks(1000);


    eGFX_InitDriver();

    Next = (eGFX_ImagePlane *)&Sprite_16BPP_RGB565_pdm_monkey;

    DMIC__init();

    for(int i=0;i<240;i++)
    {
 	   hann[i] = 0.5*(1-cos(2.0*3.14159*(float)i/240.0));
    }

    while (1)
    {

    	GPIO_PortToggle(GPIO, APP_BOARD_TEST_LED_PORT, 1u << APP_BOARD_TEST_LED_PIN);
    	GPIO_PortToggle(GPIO, 0, 1u << 5);


    	eGFX_Blit(&eGFX_BackBuffer[0],
            	          0,
            	          0,
            	          Next);


        for(int i=0;i<240;i++)
    	    {


    	    	y = (int32_t)(MICBuffer[0][i]);

    	    	y=y/(32768/120) + 240/2;
    	    	plot_list_l[i] = y;

    	    }

    	    for(int i=1;i<320;i++)
    	    {
    	    	eGFX_DrawLine(&eGFX_BackBuffer[0],i-1, plot_list_l[i-1],
    	    								      i,plot_list_l[i], 0x03f<<5|0x1F);

    	       	eGFX_DrawLine(&eGFX_BackBuffer[0], i-1,plot_list_l[i-1]+1,
    	    	    			    			i,plot_list_l[i]+1,  0x03f<<5|0x1F);

    	    }


       	eGFX_Dump(&eGFX_BackBuffer[0]);

    }
}
