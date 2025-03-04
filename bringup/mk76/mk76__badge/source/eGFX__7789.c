#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_spi.h"
#include "clock_config.h"

#include "fsl_common.h"
#include <stdbool.h>
#include "fsl_gpio.h"


#include "eGFX__7789.h"

#include "eGFX.h"

uint8_t eGFX_FrameBuffer[eGFX_NUM_BACKBUFFERS][eGFX_CALCULATE_16BPP_IMAGE_STORAGE_SPACE_SIZE(eGFX_PHYSICAL_SCREEN_SIZE_X, eGFX_PHYSICAL_SCREEN_SIZE_Y)];
eGFX_ImagePlane eGFX_BackBuffer[eGFX_NUM_BACKBUFFERS];

extern eGFX_ImagePlane eGFX_BackBuffer[eGFX_NUM_BACKBUFFERS];

#define CS_HIGH   		GPIO->SET[1] = 1<<6;
#define CS_LOW 			GPIO->CLR[1] = 1<<6;

#define RESET_HIGH		   GPIO->SET[1] = 1<<9
#define RESET_LOW		    GPIO->CLR[1] = 1<<9

#define CMD_DATA_HIGH		GPIO->SET[0] = 1<<26;
#define CMD_DATA_LOW		GPIO->CLR[0] = 1<<26;


void ST7789_Initial(void);
void Write_Cmd_Data(unsigned char x);
void Write_Cmd(unsigned char DL);
void Write_Data(unsigned char DH,unsigned char DL);
void  Write_Data_U16(unsigned int y);
static void LCD_SetPos(unsigned char x0,unsigned char x1,unsigned  int y0,unsigned int y1);

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

void  Write_Cmd_Data (unsigned char CMDP)
{
	unsigned char i;

	CS_LOW;
	CMD_DATA_HIGH;
	SPI_TX_RX(CMDP);
	CS_HIGH;
}


void  Write_Data_U16(unsigned int y)
{
	unsigned char m,n;
	m=y>>8;
	n=y;
	Write_Data(m,n);

}
//=============================================================
//write command

void Write_Cmd(unsigned char CMD)
{
	unsigned char i;
 	 
	CS_LOW;
	CMD_DATA_LOW;

	SPI_TX_RX(CMD);

	CS_HIGH;

}


void Write_Data(unsigned char DH,unsigned char DL)
{
	unsigned char i;
 	 
	CS_LOW;
	CMD_DATA_HIGH;

	SPI_TX_RX(DH);

	SPI_TX_RX(DL);

   	CS_HIGH;

}


extern void  System__Delay_mS(uint32_t);

void ST7789_Initial(void)
{
	uint32_t srcClock_Hz;

    gpio_pin_config_t IO_OutputConfig =
    {
        kGPIO_DigitalOutput,
        0,
    };

    GPIO_PinInit(GPIO, 1, 6, &IO_OutputConfig);

    GPIO_PinInit(GPIO, 1, 9, &IO_OutputConfig);

    GPIO_PinInit(GPIO, 0, 26, &IO_OutputConfig);

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

//-----------------------------------ST7789V reset sequence------------------------------------// 
	RESET_HIGH;
	System__Delay_mS(10);
	RESET_LOW;
	System__Delay_mS(10);
	RESET_HIGH;
	System__Delay_mS(120);

 	Write_Cmd(0x36); 
	Write_Cmd_Data(0x00); 
 

    Write_Cmd(0x3A);
	Write_Cmd_Data(0x05);

 	Write_Cmd(0xB2);
	Write_Cmd_Data(0x0C);
	Write_Cmd_Data(0x0C);
	Write_Cmd_Data(0x00);
	Write_Cmd_Data(0x33);
	Write_Cmd_Data(0x33); 

 	Write_Cmd(0xB7); 
	Write_Cmd_Data(0x44);  

 	Write_Cmd(0xBB);
	Write_Cmd_Data(0x24);

 	Write_Cmd(0xC0);
	Write_Cmd_Data(0x2C);

 	Write_Cmd(0xC2);
	Write_Cmd_Data(0x01);

 	Write_Cmd(0xC3);
	Write_Cmd_Data(0x13);   

 	Write_Cmd(0xC4);
	Write_Cmd_Data(0x20);  

 	Write_Cmd(0xC6); 
	Write_Cmd_Data(0x0F);    

 	Write_Cmd(0xD0); 
	Write_Cmd_Data(0xA4);
	Write_Cmd_Data(0xA1);

  	Write_Cmd(0xD6); 
	Write_Cmd_Data(0xA1);

 	Write_Cmd(0xE0);
	Write_Cmd_Data(0xF0);
	Write_Cmd_Data(0x00);
	Write_Cmd_Data(0x04);
	Write_Cmd_Data(0x04);
	Write_Cmd_Data(0x04);
	Write_Cmd_Data(0x05);
	Write_Cmd_Data(0x29);
	Write_Cmd_Data(0x33);
	Write_Cmd_Data(0x3E);
	Write_Cmd_Data(0x38);
	Write_Cmd_Data(0x12);
	Write_Cmd_Data(0x12);
	Write_Cmd_Data(0x28);
	Write_Cmd_Data(0x30);

 	Write_Cmd(0xE1);
	Write_Cmd_Data(0xF0);
	Write_Cmd_Data(0x07);
	Write_Cmd_Data(0x0A);
	Write_Cmd_Data(0x0D);
	Write_Cmd_Data(0x0B);
	Write_Cmd_Data(0x07);
	Write_Cmd_Data(0x28);
	Write_Cmd_Data(0x33);
	Write_Cmd_Data(0x3E);
	Write_Cmd_Data(0x36);
	Write_Cmd_Data(0x14);
	Write_Cmd_Data(0x14);
	Write_Cmd_Data(0x29);
	Write_Cmd_Data(0x32);

  	Write_Cmd(0x21); 

 	Write_Cmd(0x11); 	
	System__Delay_mS(120);

	Write_Cmd(0x29);

}

 
static void LCD_SetPos(unsigned char x0,unsigned char x1,unsigned int y0,unsigned int y1)
{
	unsigned char YSH,YSL,YEH,YEL;

	YSH=y0>>8;
	YSL=y0;

	YEH=y1>>8;
	YEL=y1;

 	Write_Cmd(0x2A);
	Write_Cmd_Data (0x00);
	Write_Cmd_Data (x0);
	Write_Cmd_Data (0x00);
	Write_Cmd_Data (x1);
	Write_Cmd(0x2B);
	Write_Cmd_Data (YSH);
	Write_Cmd_Data (YSL);
	Write_Cmd_Data (YEH);
	Write_Cmd_Data (YEL);
	Write_Cmd(0x2C);//LCD_WriteCMD(GRAMWR);
}

void eGFX_InitDriver(eGFX_VSyncCallback_t VS)
{

	for (int i = 0; i < eGFX_NUM_BACKBUFFERS; i++)
	{

		eGFX_ImagePlaneInit(&eGFX_BackBuffer[i],
			&eGFX_FrameBuffer[i][0],
			eGFX_PHYSICAL_SCREEN_SIZE_X,
			eGFX_PHYSICAL_SCREEN_SIZE_Y,
			eGFX_DISPLAY_DRIVER_IMAGE_PLANE_TYPE);

	}

	memset(eGFX_FrameBuffer, 0, sizeof(eGFX_FrameBuffer));

	ST7789_Initial();
}

void eGFX_Dump(eGFX_ImagePlane *Image)
{
	LCD_SetPos(0,239,0,319); //320x240

	CS_LOW;
	CMD_DATA_HIGH;

	//Change to DMA
	for(int i=0;i<(320*240*2);i+=2)
	{

		SPI_TX_RX(Image->Data[i+1]);
		SPI_TX_RX(Image->Data[i]);
	}

	CS_HIGH;
	return;
}








