#include "eGFX.h"
#include "pin_mux.h"
#include "fsl_common.h"
#include "fsl_clock.h"
#include "fsl_spi.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"

#include "eGFX_Driver_ST7789_240_240_BD.h"


/*x
 * Note:
 *
 * This was adapted from the Buy Display example code
 *
 */


void ST7789V_Initial(void);
void Write_Cmd_Data(unsigned char);
void Write_Cmd(unsigned char);
void Write_Data(unsigned char DH,unsigned char DL);
void delayms(unsigned int tt);
void Write_Data_U16(unsigned int y);
void LCD_SetPos(unsigned int Xstart,unsigned int Ystart,unsigned int Xend,unsigned int Yend);
void ClearScreen(unsigned int bColor);

#define DIRECTION 0    //0  vertical screen      2 is a horizontal screen

#define ST7789V_CS_HIGH GPIO->SET[BOARD_INITPINS_SPI_SSEL_PORT] = 1<<BOARD_INITPINS_SPI_SSEL_PIN;
#define ST7789V_CS_LOW	GPIO->CLR[BOARD_INITPINS_SPI_SSEL_PORT] = 1<<BOARD_INITPINS_SPI_SSEL_PIN;


#define ST7789V_DC_HIGH GPIO->SET[BOARD_INITPINS_LCD_CMD_DATA_PORT] = 1<<BOARD_INITPINS_LCD_CMD_DATA_PIN;
#define ST7789V_DC_LOW  GPIO->CLR[BOARD_INITPINS_LCD_CMD_DATA_PORT] = 1<<BOARD_INITPINS_LCD_CMD_DATA_PIN;

#define ST7789V_SCL_HIGH// GPIO->SET[BOARD_INITPINS_SPI_SCK_PORT] = 1<<BOARD_INITPINS_SPI_SCK_PIN
#define ST7789V_SCL_LOW // GPIO->CLR[BOARD_INITPINS_SPI_SCK_PORT] = 1<<BOARD_INITPINS_SPI_SCK_PIN

#define ST7789V_SDI_HIGH// GPIO->SET[BOARD_INITPINS_SPI_MOSI_PORT] = 1<<BOARD_INITPINS_SPI_MOSI_PIN
#define ST7789V_SDI_LOW //GPIO->CLR[BOARD_INITPINS_SPI_MOSI_PORT] = 1<<BOARD_INITPINS_SPI_MOSI_PIN

#define ST7789V_RESET_HIGH GPIO->SET[BOARD_INITPINS_LCD_RST_PORT] = 1<<BOARD_INITPINS_LCD_RST_PIN
#define ST7789V_RESET_LOW  GPIO->CLR[BOARD_INITPINS_LCD_RST_PORT] = 1<<BOARD_INITPINS_LCD_RST_PIN


#define LCD_BACKLIGHT_ON	GPIO->SET[BOARD_INITPINS_LCD_BL_PORT] = 1<<BOARD_INITPINS_LCD_BL_PIN
#define LCD_BACKLIGHT_OFF	GPIO->CLR[BOARD_INITPINS_LCD_BL_PORT] = 1<<BOARD_INITPINS_LCD_BL_PIN



eGFX_ImagePlane eGFX_BackBuffer[eGFX_NUM_BACKBUFFERS];

uint8_t BackBufferStore[eGFX_CALCULATE_16BPP_IMAGE_STORAGE_SPACE_SIZE(eGFX_PHYSICAL_SCREEN_SIZE_X,eGFX_PHYSICAL_SCREEN_SIZE_Y)];

void eGFX_InitDriver()
{

    /* Define the init structure for the output LED pin*/
    gpio_pin_config_t IO_OutputConfig = {
        kGPIO_DigitalOutput,
        0,
    };

    GPIO_PortInit(GPIO, BOARD_INITPINS_SPI_SSEL_PORT);
    GPIO_PinInit(GPIO, BOARD_INITPINS_SPI_SSEL_PORT, BOARD_INITPINS_SPI_SSEL_PIN, &IO_OutputConfig);

    GPIO_PortInit(GPIO, BOARD_INITPINS_LCD_RST_PORT);
    GPIO_PinInit(GPIO, BOARD_INITPINS_LCD_RST_PORT, BOARD_INITPINS_LCD_RST_PIN, &IO_OutputConfig);

    GPIO_PortInit(GPIO, BOARD_INITPINS_LCD_BL_PORT);
    GPIO_PinInit(GPIO, BOARD_INITPINS_LCD_BL_PORT, BOARD_INITPINS_LCD_BL_PIN, &IO_OutputConfig);

    GPIO_PortInit(GPIO, BOARD_INITPINS_LCD_CMD_DATA_PORT);
    GPIO_PinInit(GPIO, BOARD_INITPINS_LCD_CMD_DATA_PORT, BOARD_INITPINS_LCD_CMD_DATA_PIN, &IO_OutputConfig);


    ST7789V_DC_HIGH GPIO->SET[BOARD_INITPINS_LCD_CMD_DATA_PORT] = 1<<BOARD_INITPINS_LCD_CMD_DATA_PIN;
    ST7789V_DC_LOW  GPIO->CLR[BOARD_INITPINS_LCD_CMD_DATA_PORT] = 1<<BOARD_INITPINS_LCD_CMD_DATA_PIN;



    eGFX_ImagePlaneInit(&eGFX_BackBuffer[0],
                        BackBufferStore,
                        eGFX_PHYSICAL_SCREEN_SIZE_X,
                        eGFX_PHYSICAL_SCREEN_SIZE_Y,
                        eGFX_IMAGE_PLANE_16BPP_BGR565);


	spi_master_config_t SPI_Config = {0};



	CLOCK_AttachClk(kFFRO_to_FLEXCOMM14);


	ST7789V_RESET_HIGH;

	SPI_MasterGetDefaultConfig(&SPI_Config);

	SPI_Config.sselNum = 0;
	SPI_Config.enableMaster = true;
	SPI_Config.phase = 0;
	SPI_Config.polarity =  0;
	SPI_Config.dataWidth = kSPI_Data8Bits;
	SPI_Config.baudRate_Bps = 48000000;



	SPI_MasterInit(SPI14,&SPI_Config, CLOCK_GetFlexCommClkFreq(14)

	);

	SPI14->FIFOCFG |= 3<<16; /*Flush the Tx & Rx buffers*/

	SPI14->FIFOCFG |= 1; // Enable the fifo

    ST7789V_Initial();

  	LCD_BACKLIGHT_ON;

}


void eGFX_WaitForV_Sync()
{
    
}



void eGFX_Dump(eGFX_ImagePlane *Image)
{
   LCD_SetPos(0,0,239,239);

   ST7789V_CS_LOW;
   ST7789V_DC_HIGH;

   //ToDo  Make this better!
   //
   //This is a minimal implementation to get the screen to work.  Need to reimplement with DMA
   //
   //Blast out (Tx only) the SPI Data.
   //
   //Optimizations ToDo
   //
   //Only need to write the upper 16 SPI config bits once
   //Move to DMA

	for(int i=0;i<(240*240);i++)
	{
		/* wait for the response*/
		while((SPI14->FIFOSTAT & 1<<5) == 0)
		{
		}

		(SPI14->FIFOWR) = ((uint16_t *)Image->Data)[i] | SPI_FIFOWR_LEN(16-1) | (1<<SPI_FIFOWR_RXIGNORE_SHIFT);
	}

   	ST7789V_CS_HIGH;

}

void eGFX_SetBacklight(uint8_t BacklightValue)
{
	if(BacklightValue)
		LCD_BACKLIGHT_ON;
	else
		LCD_BACKLIGHT_OFF;

}


//Synchronously TX/RX a Byte
uint8_t SPI_TX_RX(uint8_t Data)
{
	SPI14->FIFOWR = Data | SPI_FIFOWR_LEN(8-1);
	/* wait for the response*/
	while((SPI14->FIFOSTAT & 1<<6) == 0)
	{
		/*   Wait for something to pop into the Queue*/
	}

	return SPI14->FIFORD;
}


//===============================================================
//write parameter
void  Write_Cmd_Data (unsigned char CMDP)
{

    ST7789V_CS_LOW;

    ST7789V_DC_HIGH;

  	SPI_TX_RX(CMDP);

   	ST7789V_CS_HIGH;

}


//=============================================================
//write command

void Write_Cmd(unsigned char CMD)
{

	ST7789V_DC_LOW;
    ST7789V_CS_LOW;


   	SPI_TX_RX(CMD);

   	ST7789V_CS_HIGH;
}


//===================================================================
//write data byte

void Write_Data(unsigned char DH,unsigned char DL)
{
    ST7789V_DC_HIGH;
	ST7789V_CS_LOW;



	SPI_TX_RX(DH);

	SPI_TX_RX(DL);

   	ST7789V_CS_HIGH;

}



//============================================================
//delay
void delayms(unsigned int count)
{
    int i,j;
    for(i=0;i<count;i++)
       {
	     for(j=0;j<10000;j++);
       }
}

//=============================================================
//LCD Initial

void ST7789V_Initial(void)
{

	delayms(5);
	ST7789V_RESET_LOW;
	delayms(100);
	ST7789V_RESET_HIGH;
	delayms(220);

	 //************* Start Initial Sequence **********//
	Write_Cmd(0x36);

	Write_Cmd_Data(0x00);

	if(DIRECTION==0)Write_Cmd_Data(0x00);
	else if(DIRECTION==2)Write_Cmd_Data(0x70);

	Write_Cmd(0x3A);
	Write_Cmd_Data(0x05);

	Write_Cmd(0xB2);
	Write_Cmd_Data(0x0C);
	Write_Cmd_Data(0x0C);
	Write_Cmd_Data(0x00);
	Write_Cmd_Data(0x33);
	Write_Cmd_Data(0x33);

	Write_Cmd(0xB7);
	Write_Cmd_Data(0x35);

	Write_Cmd(0xBB);
	Write_Cmd_Data(0x19);

	Write_Cmd(0xC0);
	Write_Cmd_Data(0x2C);

	Write_Cmd(0xC2);
	Write_Cmd_Data(0x01);

	Write_Cmd(0xC3);
	Write_Cmd_Data(0x12);

	Write_Cmd(0xC4);
	Write_Cmd_Data(0x20);

	Write_Cmd(0xC6);
	Write_Cmd_Data(0x0F);

	Write_Cmd(0xD0);
	Write_Cmd_Data(0xA4);
	Write_Cmd_Data(0xA1);

	Write_Cmd(0xE0);
	Write_Cmd_Data(0xD0);
	Write_Cmd_Data(0x04);
	Write_Cmd_Data(0x0D);
	Write_Cmd_Data(0x11);
	Write_Cmd_Data(0x13);
	Write_Cmd_Data(0x2B);
	Write_Cmd_Data(0x3F);
	Write_Cmd_Data(0x54);
	Write_Cmd_Data(0x4C);
	Write_Cmd_Data(0x18);
	Write_Cmd_Data(0x0D);
	Write_Cmd_Data(0x0B);
	Write_Cmd_Data(0x1F);
	Write_Cmd_Data(0x23);

	Write_Cmd(0xE1);
	Write_Cmd_Data(0xD0);
	Write_Cmd_Data(0x04);
	Write_Cmd_Data(0x0C);
	Write_Cmd_Data(0x11);
	Write_Cmd_Data(0x13);
	Write_Cmd_Data(0x2C);
	Write_Cmd_Data(0x3F);
	Write_Cmd_Data(0x44);
	Write_Cmd_Data(0x51);
	Write_Cmd_Data(0x2F);
	Write_Cmd_Data(0x1F);
	Write_Cmd_Data(0x1F);
	Write_Cmd_Data(0x20);
	Write_Cmd_Data(0x23);

	Write_Cmd(0x21);

	Write_Cmd(0x11);
	delayms(120);

	Write_Cmd(0x29);


}


//===============================================================
 void LCD_SetPos(unsigned int Xstart,unsigned int Ystart,unsigned int Xend,unsigned int Yend)
{
	Write_Cmd(0x2a);
	Write_Cmd_Data(Xstart>>8);
	Write_Cmd_Data(Xstart);
 	Write_Cmd_Data(Xend>>8);
	Write_Cmd_Data(Xend);

	Write_Cmd(0x2b);
	Write_Cmd_Data(Ystart>>8);
	Write_Cmd_Data(Ystart);
	Write_Cmd_Data(Yend>>8);
	Write_Cmd_Data(Yend);

  	Write_Cmd(0x2c);//LCD_WriteCMD(GRAMWR);
}




