#include <stdio.h>
#include "pin_mux.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_i2s_dma.h"
#include "fsl_codec_common.h"
#include "fsl_codec_adapter.h"
#include <stdbool.h>
#include "fsl_cs42448.h"
#include "i2s__test.h"
#include "eGFX.h"
#include "eGFX__7789.h"
#include "assets/Sprites_16BPP_RGB565.h"
#include  "math.h"
#include "dmic_test.h"

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

extern uint32_t rx_buf_ptr;
int32_t x = 0;
int32_t y = 0;
int16_t capture_buffer[640];

int16_t plot_list_l[320];
int16_t plot_list_r[320];

uint32_t offset = 0;


float hann[320];

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

   GPIO_PinInit(GPIO, BOARD_INITPINS_RED_LED_PORT, BOARD_INITPINS_RED_LED_PIN, &IO_OutputConfig);

   GPIO_PinInit(GPIO, BOARD_INITPINS_GREEN_LED_PORT, BOARD_INITPINS_GREEN_LED_PIN, &IO_OutputConfig);

   SystemCoreClockUpdate();

   SysTick_Config(SystemCoreClock / 1000U);

   i2s__init();

   eGFX_InitDriver(NULL);

   DMIC__init();

   for(int i=0;i<320;i++)
   {
	   hann[i] = 0.5*(1-cos(2.0*3.14159*(float)i/320.0));
   }

   while(1)
   {
	    memcpy(&eGFX_BackBuffer[0].Data[0],Sprite_16BPP_RGB565_bg2.Data,320*240*2);


	  // i2s2__rx_capture(capture_buffer,320);

	   //while(I2S2_Rx_Complete == false)
	   //{

	   //}

	 //   memcpy(&eGFX_BackBuffer[0].Data,Sprite_16BPP_RGB565_bg1.Data,320*240*2);

	    for(int i=0;i<320;i++)
	    {


	    	//y = I2S2_Rx_Buffer[i*2+rx_buf_ptr];
	    	//y = capture_buffer[i*2];
	    	y = (int32_t)(MICBuffer[2][i] * hann[i]);

	    	y=y/(32768/120) + 240/2;
	    	plot_list_l[i] = y;

	    	//y = capture_buffer[i*2+1];
	    	//y=y/(32768/120) + 240/2;
	    //	plot_list_r[i] = y;

	    }

	    for(int i=1;i<320;i++)
	    {
	    	eGFX_DrawLine(&eGFX_BackBuffer[0], plot_list_l[i-1],i-1,
	    								    	plot_list_l[i] ,i, 0x03f<<5|0x1F);

	       	eGFX_DrawLine(&eGFX_BackBuffer[0], plot_list_l[i-1]+1,i-1,
	    	    								    	plot_list_l[i]+1 ,i, 0x03f<<5|0x1F);

	    }

	    eGFX_Dump(&eGFX_BackBuffer[0]);

	    for(int i=1;i<320;i++)
	    {
	    	eGFX_DrawLine(&eGFX_BackBuffer[0], plot_list_l[i-1],i-1,
	    								    	plot_list_l[i] ,i, 0);

	    }


        GPIO_PortToggle(GPIO, BOARD_INITPINS_RED_LED_PORT, 1u << BOARD_INITPINS_RED_LED_PIN);
        GPIO_PortToggle(GPIO, BOARD_INITPINS_GREEN_LED_PORT, 1u << BOARD_INITPINS_GREEN_LED_PIN);
   }
}

