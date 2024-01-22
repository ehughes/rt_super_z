#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_spi.h"
#include "clock_config.h"

#include "fsl_common.h"
#include <stdbool.h>
#include "fsl_gpio.h"


// ST7789 defines
#define ST7789_11_SLPOUT (0x11)
#define ST7789_21_INVON (0x21)
#define ST7789_29_DISPON (0x29)
#define ST7789_2A_CASET (0x2A)
#define ST7789_2B_RASET (0x2B)
#define ST7789_2C_RAMWR (0x2C)
#define ST7789_36_MADCTL (0x36)
#define ST7789_3A_COLMOD (0x3A)
#define ST7789_B2_PORCTRL (0xB2)
#define ST7789_B7_GCTRL (0xB7)
#define ST7789_BB_VCOMS (0xBB)
#define ST7789_C0_LCMCTRL (0xC0)
#define ST7789_C2_VDVVRHEN (0xC2)
#define ST7789_C3_VRHS (0xC3)
#define ST7789_C4_VDVSET (0xC4)
#define ST7789_C6_FRCTR2 (0xC6)
#define ST7789_D0_PWCTRL1 (0xD0)
#define ST7789_E0_PVGAMCTRL (0xE0)
#define ST7789_E1_NVGAMCTRL (0xE1)




#define CS_HIGH   		GPIO->SET[1] = 1<<6;
#define CS_LOW 			GPIO->CLR[1] = 1<<6;

#define RESET_HIGH		   GPIO->SET[0] = 1<<28
#define RESET_LOW		    GPIO->CLR[0] = 1<<28

#define CMD_DATA_HIGH		GPIO->SET[0] = 1<<30;
#define CMD_DATA_LOW		GPIO->CLR[0] = 1<<30;



uint8_t SPI_TX_RX(uint8_t Data)
{
	SPI5->FIFOWR = Data | SPI_FIFOWR_LEN(8-1);
	/* wait for the response*/
	while((SPI5->FIFOSTAT & 1<<6) == 0)
	{
		/*   Wait for something to pop into the Queue*/
	}

	return SPI5->FIFORD;
}



void SPI_sendCommand(uint8_t cmd)
{
	CS_LOW; // select the LCD controller
	CMD_DATA_LOW; // select the command register
	SPI_TX_RX(cmd); // send the command via SPI:
	CS_HIGH; // de-select the LCD controller
}

void SPI_sendData(uint8_t data)
{
	CS_LOW; // select the LCD controller
	CMD_DATA_HIGH; // select the data register
	SPI_TX_RX(data); //send the data via SPI:
	CS_HIGH; // de-select the LCD controller
}





extern void  System__Delay_mS(uint32_t);
//=============================================================
//lcd initial

void delay(uint32_t ms)
{

	System__Delay_mS(ms);
}

void ST7789_Initial(void)
{


	uint32_t srcClock_Hz;

    /* Define the init structure for the output LED pin*/
    gpio_pin_config_t IO_OutputConfig = {
        kGPIO_DigitalOutput,
        0,
    };

    GPIO_PinInit(GPIO, 1, 6, &IO_OutputConfig);

    GPIO_PinInit(GPIO, 0, 28, &IO_OutputConfig);

    //GPIO_PortInit(GPIO, 0);
    GPIO_PinInit(GPIO, 0, 30, &IO_OutputConfig);



	CLOCK_AttachClk(kFFRO_to_FLEXCOMM5);
	spi_master_config_t SPI_Config = {0};


	SPI_MasterGetDefaultConfig(&SPI_Config);

	SPI_Config.sselNum = 0;
	SPI_Config.enableMaster = true;
	SPI_Config.phase = 0;
	SPI_Config.polarity =  0;
	SPI_Config.dataWidth = kSPI_Data8Bits;
	SPI_Config.baudRate_Bps = 48000000;


	SPI_MasterInit(SPI5,&SPI_Config, CLOCK_GetFlexCommClkFreq(5)

	);

	SPI5->FIFOCFG |= 3<<16; /*Flush the Tx & Rx buffers*/

	SPI5->FIFOCFG |= 1; // Enable the fifo







	// perform a hardware reset on the display
		RESET_LOW;
		delay(1);
		RESET_HIGH;
		delay(150);

		SPI_sendCommand(ST7789_11_SLPOUT); // turn off sleep mode
		delay(120); // it is required to wait this much time before issuing another command

		SPI_sendCommand(ST7789_36_MADCTL); // memory data access control
		SPI_sendData(0x08 | 0x80); // set the RGB vs BGR order to match a windows 24-bit BMP

		SPI_sendCommand(ST7789_3A_COLMOD); // interface pixel format
		SPI_sendData(0x06); // 18-bit/pixel

		SPI_sendCommand(ST7789_B2_PORCTRL); // porch control
		SPI_sendData(0x0C);
		SPI_sendData(0x0C);
		SPI_sendData(0x00);
		SPI_sendData(0x33);
		SPI_sendData(0x33);

		SPI_sendCommand(ST7789_B7_GCTRL); // gate control
		SPI_sendData(0x35);

		SPI_sendCommand(ST7789_BB_VCOMS); // VCOM settings
		SPI_sendData(0x2B);

		SPI_sendCommand(ST7789_C0_LCMCTRL); // LCM control
		SPI_sendData(0x2C);

		SPI_sendCommand(ST7789_C2_VDVVRHEN); // VDV and VRH command enable
		SPI_sendData(0x01);
		SPI_sendData(0xFF);

		SPI_sendCommand(ST7789_C3_VRHS); // VRH set
		SPI_sendData(0x20);

		SPI_sendCommand(ST7789_C4_VDVSET); // VDV set
		SPI_sendData(0x20);


		SPI_sendCommand(ST7789_C6_FRCTR2); // frame rate control in normal mode
		SPI_sendData(0x0F); // 60 HZ

		SPI_sendCommand(ST7789_D0_PWCTRL1); // power control 1
		SPI_sendData(0xA4);
		SPI_sendData(0xA1);

		SPI_sendCommand(ST7789_E0_PVGAMCTRL); // positive voltage gamma control
		SPI_sendData(0xD0);
		SPI_sendData(0xCA);
		SPI_sendData(0x0E);
		SPI_sendData(0x08);
		SPI_sendData(0x09);
		SPI_sendData(0x07);
		SPI_sendData(0x2D);
		SPI_sendData(0x3B);
		SPI_sendData(0x3D);
		SPI_sendData(0x34);
		SPI_sendData(0x0A);
		SPI_sendData(0x0A);
		SPI_sendData(0x1B);
		SPI_sendData(0x28);

		SPI_sendCommand(ST7789_E1_NVGAMCTRL); // negative voltage gamma control
		SPI_sendData(0xD0);
		SPI_sendData(0xCA);
		SPI_sendData(0x0F);
		SPI_sendData(0x08);
		SPI_sendData(0x08);
		SPI_sendData(0x07);
		SPI_sendData(0x2E);
		SPI_sendData(0x5C);
		SPI_sendData(0x40);
		SPI_sendData(0x34);
		SPI_sendData(0x09);
		SPI_sendData(0x0B);
		SPI_sendData(0x1B);
		SPI_sendData(0x28);

		SPI_sendCommand(ST7789_21_INVON); // display inversion on

		SPI_sendCommand(ST7789_2A_CASET); // column address set
		SPI_sendData(0x00); // start MSB start = 0
		SPI_sendData(0x00); // start LSB
		SPI_sendData(0x00); // end MSB End = 249
		SPI_sendData(0xEF); // end LSB

		SPI_sendCommand(ST7789_2B_RASET); // row address set
		SPI_sendData(0x00); // start MSB start = 0
		SPI_sendData(0x00); // start LSB
		SPI_sendData(0x01); // end MSB end = 319
		SPI_sendData(0x3F); // end LSB

		SPI_sendCommand(ST7789_29_DISPON); // display on
		delay(1);
}




void Set_LCD_for_write_at_X_Y(uint16_t x, uint16_t y)
{
	SPI_sendCommand(ST7789_2A_CASET); // column address set function
	SPI_sendData(x >> 8);
	SPI_sendData(x & 0x00FF); // the x axis will increment according to the variable passed through
	SPI_sendData(0);
	SPI_sendData(240);

	SPI_sendCommand(ST7789_2B_RASET); // row address set function
	// use 1st quadrant coordinates: 0,0 is lower left, 239,319 is upper right
	y = 319 - y; // this will invert the display direction
	SPI_sendData(y >> 8);
	SPI_sendData(y & 0x00FF); // the y axis will increment according to the variable passed through
	SPI_sendData(0x01);
	SPI_sendData(0x3F);

	SPI_sendCommand(ST7789_2C_RAMWR); // write the current position to RAM
}

void Fill_LCD(uint8_t R, uint8_t G, uint8_t B)
{
	uint32_t
		i;
	Set_LCD_for_write_at_X_Y(0, 319); // set the write position

	// fill display with a given RGB value
	for (i = 0; i < (320UL * 240UL); i++)
	{

		SPI_sendData(B); // blue
		SPI_sendData(G); // green
		SPI_sendData(R); // red
	}
}


#include "frame.h"

extern uint16_t link[];

uint8_t *out_frame;

uint8_t frame_num = 0;
void show_picture(void)
{
	unsigned int i,j;
	unsigned int m=0,n=0;
	uint8_t r,g,b;

	frame_num++;
	if(frame_num>=FRAME_FRAME_COUNT)
		frame_num = 0;

	out_frame = &frame[320*240*frame_num];

	Set_LCD_for_write_at_X_Y(0, 319); // set the write position



	for (i = 0; i < (320UL * 240UL); i++)
	        	{
					r = (out_frame[i]<<2)& 0xC0;
					g = (out_frame[i]<<4) & 0xC0;
					b = out_frame[i]<<6;
/*
					if(r)
						r|= 0x3f;

					if(g)
						g|= 0x3f;

					if(b)
						b|= 0x3f;
						*/
					SPI_sendData(b);
					SPI_sendData(g);
					SPI_sendData(r);
	        	}


        	// fill display with a given RGB value
//        	for (i = 0; i < (320UL * 240UL*3); i++)
  //      	{

    //    		SPI_sendData(link[i]);
      //  	}

    return;
}





