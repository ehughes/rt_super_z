#include <i2s__test.h>
#include "fsl_i2s.h"
#include "fsl_i2s_dma.h"


#define I2S1_TEST__TX_DMA (DMA1)
#define I2S1_TEST__TX__DMA_CHANNEL (3)
#define I2S1_TEST__TX (I2S1)

#define I2S3_TEST__TX_DMA (DMA1)
#define I2S3_TEST__TX__DMA_CHANNEL (7)
#define I2S3_TEST__TX (I2S3)


#define I2S_TEST__RX_DMA (DMA1)
#define I2S_TEST__RX__DMA_CHANNEL (4)
#define I2S_TEST__RX (I2S2)


#define TDM_DATA_START_POSITION 0

#define TDM_SYNC_MODE 		kI2S_ModeDspWsShort
#define WS_POL				true
// MCLK 12.288 MHz
// 48 KHz * 2 channels * 16-bits per channel = 1.536 MHz
// Clock divider = 12.288 / 1.536 = 8

#define I2S_TEST__CLOCK_DIVIDER (12288000 / I2S_TX_CHANNEL_COUNT / AUDIO_SAMPLE_RATE / 16)
// needs to be zero for slave mode, but sdk subtracts 1
#define I2S_CLOCK_DIVIDER_RX 1

volatile int16_t I2S1_Tx_Buffer[I2S_TX_BUFFER_SIZE_SAMPLES];

volatile int16_t I2S3_Tx_Buffer[I2S_TX_BUFFER_SIZE_SAMPLES];

int16_t I2S2_Rx_Buffer[I2S_RX_BUFFER_SIZE_SAMPLES];

volatile int16_t I2S2_Rx_Buffer__Big[BIG_BUFFER_SAMPLES];

volatile uint32_t I2S3_Tx__PingPongIdx;

volatile uint32_t I2S1_Tx__PingPongIdx;

static dma_handle_t I2S3_Tx__Handle;
static dma_handle_t I2S1_Tx__Handle;
static dma_handle_t I2S2_Rx__Handle;

volatile bool I2S2_Rx_Complete;

__attribute__((aligned(16))) dma_descriptor_t I2S1_Tx_DMADesc[2] = {0};
__attribute__((aligned(16))) dma_descriptor_t I2S3_Tx_DMADesc[2] = {0};
__attribute__((aligned(16))) dma_descriptor_t I2S2_Rx_DMADesc[2] = {0};

static void I2S_TxEnableDMA(I2S_Type *base, bool enable)
{
    if (enable)
    {
        base->FIFOCFG |= I2S_FIFOCFG_DMATX_MASK;
    }
    else
    {
        base->FIFOCFG &= (~I2S_FIFOCFG_DMATX_MASK);
        base->FIFOCFG |= I2S_FIFOCFG_EMPTYTX_MASK;
    }
}


static void I2S_RxEnableDMA(I2S_Type *base, bool enable)
{
    if (enable)
    {
        base->FIFOCFG |= I2S_FIFOCFG_DMARX_MASK;
    }
    else
    {
        base->FIFOCFG &= (~I2S_FIFOCFG_DMARX_MASK);
        base->FIFOCFG |= I2S_FIFOCFG_EMPTYRX_MASK;
    }
}

static int out = 0;
static int tx1_cnt = 0;
static int tx3_cnt = 0;
static int rx_cnt = 0;
static int out1 = 0;

uint32_t idx;

const int16_t sine_wave[] = {
    0,
    6392,
    12539,
    18204,
    23169,
    27244,
    30272,
    32137,
    32767,
    32137,
    30272,
    27244,
    23169,
    18204,
    12539,
    6392,
    0,
    -6392,
    -12539,
    -18204,
    -23169,
    -27244,
    -30272,
    -32137,
    -32767,
    -32137,
    -30272,
    -27244,
    -23169,
    -18204,
    -12539,
    -6392
};

void I2S1_TX_Callback(void *handle, void *userData, bool transferDone, uint32_t intmode)
{
	tx1_cnt++;

	for(int i=0;i<I2S_FRAME_SIZE_SAMPLES/4;i++)
	{

			I2S1_Tx_Buffer[i*4 +I2S1_Tx__PingPongIdx + 0] = sine_wave[idx];
			I2S1_Tx_Buffer[i*4 +I2S1_Tx__PingPongIdx + 1] = sine_wave[idx]>>2;
			I2S1_Tx_Buffer[i*4 +I2S1_Tx__PingPongIdx + 2] = sine_wave[idx]>>4;
			I2S1_Tx_Buffer[i*4 +I2S1_Tx__PingPongIdx + 3] = sine_wave[idx]>>6;

			idx = idx+1;

			if(idx>=(sizeof(sine_wave)/sizeof(int16_t)))
			{
				idx = 0;
			}
		out1++;

	}

	if(I2S1_Tx__PingPongIdx == 0)
    	I2S1_Tx__PingPongIdx = I2S_FRAME_SIZE_SAMPLES;
    else
    	I2S1_Tx__PingPongIdx = 0;

}

void I2S3_TX_Callback(void *handle, void *userData, bool transferDone, uint32_t intmode)
{
	tx1_cnt++;
	for(int i=0;i<I2S_FRAME_SIZE_SAMPLES;i++)
	{
		I2S3_Tx_Buffer[i+I2S3_Tx__PingPongIdx] = out;
		out++;
	}

	if(I2S3_Tx__PingPongIdx == 0)
    	I2S3_Tx__PingPongIdx = I2S_FRAME_SIZE_SAMPLES;
    else
    	I2S3_Tx__PingPongIdx = 0;

}


uint32_t rx_buf_ptr = 0;

void I2S2_RX_Callback(void *handle, void *userData, bool transferDone, uint32_t intmode)
{

	I2S2_Rx_Complete = true;

/*	if(rx_buf_ptr!=I2S_RX_FRAME_SIZE_SAMPLES)
		rx_buf_ptr = I2S_RX_FRAME_SIZE_SAMPLES;
	else
		rx_buf_ptr = 0;

	rx_cnt++;
*/
	DMA_AbortTransfer(&I2S2_Rx__Handle);

	I2S_Disable(I2S_TEST__RX);
}

void i2s1__tx_init()
{
	 i2s_config_t s_TxConfig;

	 dma_transfer_config_t I2S_Tx_TransferConfig;

	 memset((void *)I2S1_Tx_Buffer, 0x00, sizeof(I2S1_Tx_Buffer));


	 for(int i=0;i<I2S_TX_BUFFER_SIZE_SAMPLES;i++)
	 {
		 I2S1_Tx_Buffer[i] = i*10;
	 }

	 I2S_TxGetDefaultConfig(&s_TxConfig);


    s_TxConfig.divider     = I2S_TEST__CLOCK_DIVIDER;
    s_TxConfig.mode        = TDM_SYNC_MODE;
    s_TxConfig.wsPol       = WS_POL;
    s_TxConfig.dataLength  = 16U;
    s_TxConfig.frameLength = 16 * 4U;
    s_TxConfig.masterSlave = kI2S_MasterSlaveNormalMaster;
	s_TxConfig.rightLow = false;



    s_TxConfig.position    = TDM_DATA_START_POSITION;

	I2S_TxInit(I2S1_TEST__TX, &s_TxConfig);

    I2S_EnableSecondaryChannel(I2S1_TEST__TX, kI2S_SecondaryChannel1, false, 32 + TDM_DATA_START_POSITION);

	I2S_TxEnableDMA(I2S1_TEST__TX, true);

	DMA_CreateHandle(&I2S1_Tx__Handle, I2S1_TEST__TX_DMA, I2S1_TEST__TX__DMA_CHANNEL);

	DMA_PrepareTransfer(&I2S_Tx_TransferConfig,
							(void *)I2S1_Tx_Buffer,
							(void *)&I2S1_TEST__TX->FIFOWR,
							sizeof(int32_t),
							I2S_TX_SIZE_BYTES,
							kDMA_MemoryToPeripheral,
							&I2S1_Tx_DMADesc);


	I2S_Tx_TransferConfig.xfercfg.intA = true;
	I2S_Tx_TransferConfig.xfercfg.intB = false;


	DMA_CreateDescriptor(&I2S1_Tx_DMADesc[1], &I2S_Tx_TransferConfig.xfercfg,	(void *)&I2S1_Tx_Buffer[0], (void *)&I2S1_TEST__TX->FIFOWR, &I2S1_Tx_DMADesc[0]);
	DMA_CreateDescriptor(&I2S1_Tx_DMADesc[0], &I2S_Tx_TransferConfig.xfercfg, 	(void *)&I2S1_Tx_Buffer[I2S_FRAME_SIZE_SAMPLES], (void *)&I2S1_TEST__TX->FIFOWR, &I2S1_Tx_DMADesc[1]);

	DMA_SubmitTransfer(&I2S1_Tx__Handle, &I2S_Tx_TransferConfig);

	DMA_EnableChannel(I2S1_TEST__TX_DMA, I2S1_TEST__TX__DMA_CHANNEL);

	DMA_SetCallback(&I2S1_Tx__Handle, (dma_callback)I2S1_TX_Callback, NULL);

	DMA_SetChannelPriority(I2S1_TEST__TX_DMA, I2S1_TEST__TX__DMA_CHANNEL, kDMA_ChannelPriority3);

	DMA_StartTransfer(&I2S1_Tx__Handle);

	I2S_Enable(I2S1_TEST__TX);

}	// End BOARD_InitCODEC

static void i2s3__tx_init()
{
	 i2s_config_t s_TxConfig;

	 dma_transfer_config_t I2S_Tx_TransferConfig;

	 memset((void *)I2S3_Tx_Buffer, 0x00, sizeof(I2S3_Tx_Buffer));


	 for(int i=0;i<I2S_TX_BUFFER_SIZE_SAMPLES;i++)
	 {
		 I2S3_Tx_Buffer[i] = i*10;
	 }

	 I2S_TxGetDefaultConfig(&s_TxConfig);


    s_TxConfig.divider     = I2S_TEST__CLOCK_DIVIDER;
    s_TxConfig.mode        = TDM_SYNC_MODE;
    s_TxConfig.wsPol       = WS_POL;
    s_TxConfig.dataLength  = 16U;
    s_TxConfig.frameLength = 16 * 4U;
    s_TxConfig.masterSlave = kI2S_MasterSlaveNormalMaster;

    s_TxConfig.position    = TDM_DATA_START_POSITION;

	I2S_TxInit(I2S3_TEST__TX, &s_TxConfig);


    I2S_EnableSecondaryChannel(I2S3_TEST__TX, kI2S_SecondaryChannel1, false, 32 + TDM_DATA_START_POSITION);

	I2S_TxEnableDMA(I2S3_TEST__TX, true);

	DMA_CreateHandle(&I2S3_Tx__Handle, I2S3_TEST__TX_DMA, I2S3_TEST__TX__DMA_CHANNEL);

	DMA_PrepareTransfer(&I2S_Tx_TransferConfig,
							(void *)I2S3_Tx_Buffer,
							(void *)&I2S3_TEST__TX->FIFOWR,
							sizeof(int32_t),
							I2S_TX_SIZE_BYTES,
							kDMA_MemoryToPeripheral,
							&I2S3_Tx_DMADesc);


	I2S_Tx_TransferConfig.xfercfg.intA = true;
	I2S_Tx_TransferConfig.xfercfg.intB = false;


	DMA_CreateDescriptor(&I2S3_Tx_DMADesc[1], &I2S_Tx_TransferConfig.xfercfg,	(void *)&I2S3_Tx_Buffer[0], (void *)&I2S3_TEST__TX->FIFOWR, &I2S3_Tx_DMADesc[0]);
	DMA_CreateDescriptor(&I2S3_Tx_DMADesc[0], &I2S_Tx_TransferConfig.xfercfg, 	(void *)&I2S3_Tx_Buffer[I2S_FRAME_SIZE_SAMPLES], (void *)&I2S3_TEST__TX->FIFOWR, &I2S3_Tx_DMADesc[1]);

	DMA_SubmitTransfer(&I2S3_Tx__Handle, &I2S_Tx_TransferConfig);

	DMA_EnableChannel(I2S3_TEST__TX_DMA, I2S3_TEST__TX__DMA_CHANNEL);

	DMA_SetCallback(&I2S3_Tx__Handle, (dma_callback)I2S3_TX_Callback, NULL);

	DMA_SetChannelPriority(I2S3_TEST__TX_DMA, I2S3_TEST__TX__DMA_CHANNEL, kDMA_ChannelPriority3);

	DMA_StartTransfer(&I2S3_Tx__Handle);

	I2S_Enable(I2S3_TEST__TX);

}	// End BOARD_InitCODEC

static void i2s2__rx_init__big_buff()
{
	 i2s_config_t s_RxConfig;

	 dma_transfer_config_t I2S_Rx_TransferConfig;

	 memset((void *)I2S2_Rx_Buffer__Big, 0x00, sizeof(I2S2_Rx_Buffer__Big));


	 for(int i=0;i<I2S_TX_BUFFER_SIZE_SAMPLES;i++)
	 {
		 I2S2_Rx_Buffer__Big[i] = i*10;
	 }

	 I2S_RxGetDefaultConfig(&s_RxConfig);


    s_RxConfig.divider     = 1;
    s_RxConfig.mode        = TDM_SYNC_MODE;
    s_RxConfig.wsPol       = WS_POL;
    s_RxConfig.dataLength  = 16U;
    s_RxConfig.frameLength = 16 * 4U;
    s_RxConfig.position    = TDM_DATA_START_POSITION;
    s_RxConfig.masterSlave = kI2S_MasterSlaveNormalSlave;
    s_RxConfig.rightLow = false;


	I2S_RxInit(I2S_TEST__RX, &s_RxConfig);

    I2S_EnableSecondaryChannel(I2S_TEST__RX, kI2S_SecondaryChannel1, false, 32 + TDM_DATA_START_POSITION);

	I2S_RxEnableDMA(I2S_TEST__RX, true);

	DMA_CreateHandle(&I2S2_Rx__Handle, I2S_TEST__RX_DMA, I2S_TEST__RX__DMA_CHANNEL);

	DMA_PrepareTransfer(&I2S_Rx_TransferConfig,
							(void *)&I2S_TEST__RX->FIFORD,
							(void *)I2S2_Rx_Buffer,

							sizeof(int32_t),
							BIG_BUFFER_BYTES,
							kDMA_PeripheralToMemory,
							&I2S2_Rx_DMADesc);


	I2S_Rx_TransferConfig.xfercfg.intA = true;
	I2S_Rx_TransferConfig.xfercfg.intB = false;


	DMA_CreateDescriptor(&I2S2_Rx_DMADesc[0], &I2S_Rx_TransferConfig.xfercfg,(void *)&I2S_TEST__RX->FIFORD,	(void *)&I2S2_Rx_Buffer__Big[0],  0);

	DMA_SubmitTransfer(&I2S2_Rx__Handle, &I2S_Rx_TransferConfig);

	DMA_EnableChannel(I2S_TEST__RX_DMA, I2S_TEST__RX__DMA_CHANNEL);

	DMA_SetCallback(&I2S2_Rx__Handle, (dma_callback)I2S2_RX_Callback, NULL);

	DMA_SetChannelPriority(I2S_TEST__RX_DMA, I2S_TEST__RX__DMA_CHANNEL, kDMA_ChannelPriority4);

	DMA_StartTransfer(&I2S2_Rx__Handle);


	I2S_Enable(I2S_TEST__RX);

}	// End BOARD_InitCODEC

static void i2s2__rx_init__tdm()
{
	 i2s_config_t s_RxConfig;

	 dma_transfer_config_t I2S_Rx_TransferConfig;

	 memset((void *)I2S2_Rx_Buffer, 0x00, sizeof(I2S2_Rx_Buffer));


	 for(int i=0;i<I2S_TX_BUFFER_SIZE_SAMPLES;i++)
	 {
		 I2S2_Rx_Buffer[i] = i*10;
	 }

	 I2S_RxGetDefaultConfig(&s_RxConfig);


    s_RxConfig.divider     = 1;
    s_RxConfig.mode        = TDM_SYNC_MODE;
    s_RxConfig.wsPol       = WS_POL;
    s_RxConfig.dataLength  = 16U;
    s_RxConfig.frameLength = 16 * 4U;
    s_RxConfig.position    = TDM_DATA_START_POSITION;
    s_RxConfig.masterSlave = kI2S_MasterSlaveNormalSlave;
    s_RxConfig.rightLow = true;

	I2S_RxInit(I2S_TEST__RX, &s_RxConfig);

   // I2S_EnableSecondaryChannel(I2S_TEST__RX, kI2S_SecondaryChannel1, false, 32 + TDM_DATA_START_POSITION);

	I2S_RxEnableDMA(I2S_TEST__RX, true);

	DMA_CreateHandle(&I2S2_Rx__Handle, I2S_TEST__RX_DMA, I2S_TEST__RX__DMA_CHANNEL);

	DMA_PrepareTransfer(&I2S_Rx_TransferConfig,
							(void *)&I2S_TEST__RX->FIFORD,
							(void *)I2S2_Rx_Buffer,

							sizeof(int32_t),
							I2S_TX_SIZE_BYTES,
							kDMA_PeripheralToMemory,
							&I2S2_Rx_DMADesc);


	I2S_Rx_TransferConfig.xfercfg.intA = true;
	I2S_Rx_TransferConfig.xfercfg.intB = false;


	DMA_CreateDescriptor(&I2S2_Rx_DMADesc[1], &I2S_Rx_TransferConfig.xfercfg,(void *)&I2S_TEST__RX->FIFORD,	(void *)&I2S2_Rx_Buffer[0],  &I2S2_Rx_DMADesc[0]);
	DMA_CreateDescriptor(&I2S2_Rx_DMADesc[0], &I2S_Rx_TransferConfig.xfercfg, (void *)&I2S_TEST__RX->FIFORD,	(void *)&I2S2_Rx_Buffer[I2S_FRAME_SIZE_SAMPLES],  &I2S2_Rx_DMADesc[1]);

	DMA_SubmitTransfer(&I2S2_Rx__Handle, &I2S_Rx_TransferConfig);

	DMA_EnableChannel(I2S_TEST__RX_DMA, I2S_TEST__RX__DMA_CHANNEL);

	DMA_SetCallback(&I2S2_Rx__Handle, (dma_callback)I2S2_RX_Callback, NULL);

	DMA_SetChannelPriority(I2S_TEST__RX_DMA, I2S_TEST__RX__DMA_CHANNEL, kDMA_ChannelPriority4);

	DMA_StartTransfer(&I2S2_Rx__Handle);


	I2S_Enable(I2S_TEST__RX);

}	// End BOARD_InitCODEC


void i2s2__rx_init()
{
	 i2s_config_t s_RxConfig;

	 dma_transfer_config_t I2S_Rx_TransferConfig;

	 memset((void *)I2S2_Rx_Buffer, 0x0100, sizeof(I2S2_Rx_Buffer));


	 for(int i=0;i<I2S_TX_BUFFER_SIZE_SAMPLES;i++)
	 {
		 I2S2_Rx_Buffer[i] = i*123;
	 }

	 I2S_RxGetDefaultConfig(&s_RxConfig);


    s_RxConfig.divider     = 1;
    s_RxConfig.mode        = kI2S_ModeI2sClassic;
    s_RxConfig.wsPol       = 0;
    s_RxConfig.dataLength  = 16U;
    s_RxConfig.frameLength = 16 * 2U;
    s_RxConfig.position    = 0;
    s_RxConfig.masterSlave = kI2S_MasterSlaveNormalSlave;
    s_RxConfig.rightLow = true;

	I2S_RxInit(I2S_TEST__RX, &s_RxConfig);

	//I2S_Enable(I2S_TEST__RX);

}	// End BOARD_InitCODEC


void i2s2__rx_capture(int16_t *buffer,uint32_t num_samples)
{
	 i2s_config_t s_RxConfig;

	 dma_transfer_config_t I2S_Rx_TransferConfig;



	I2S_RxEnableDMA(I2S_TEST__RX, true);

	DMA_CreateHandle(&I2S2_Rx__Handle, I2S_TEST__RX_DMA, I2S_TEST__RX__DMA_CHANNEL);

	DMA_PrepareTransfer(&I2S_Rx_TransferConfig,
							(void *)&I2S_TEST__RX->FIFORD,
							(void *)buffer ,

							sizeof(int32_t),
							num_samples*sizeof(int16_t)*2,
							kDMA_PeripheralToMemory,
							&I2S2_Rx_DMADesc);


	I2S_Rx_TransferConfig.xfercfg.intA = true;
	I2S_Rx_TransferConfig.xfercfg.intB = false;

	DMA_CreateDescriptor(&I2S2_Rx_DMADesc[0], &I2S_Rx_TransferConfig.xfercfg, (void *)&I2S_TEST__RX->FIFORD,	(void *)buffer ,  0);

	DMA_SubmitTransfer(&I2S2_Rx__Handle, &I2S_Rx_TransferConfig);

	DMA_EnableChannel(I2S_TEST__RX_DMA, I2S_TEST__RX__DMA_CHANNEL);

	DMA_SetCallback(&I2S2_Rx__Handle, (dma_callback)I2S2_RX_Callback, NULL);

	DMA_SetChannelPriority(I2S_TEST__RX_DMA, I2S_TEST__RX__DMA_CHANNEL, kDMA_ChannelPriority4);

	I2S2_Rx_Complete = false;

	DMA_StartTransfer(&I2S2_Rx__Handle);

	I2S_Enable(I2S_TEST__RX);


}	// End BOARD_InitCODEC


void i2s__init()
{
    CLOCK_EnableClock(kCLOCK_InputMux);

    /* Clear MUA reset before run DSP core */
    RESET_PeripheralReset(kMU_RST_SHIFT_RSTn);

#if 0
   /* attach main clock to I3C */
    CLOCK_AttachClk(kMAIN_CLK_to_I3C_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivI3cClk, 20);
#endif


    DMA_Init(DMA1);

    CLOCK_AttachClk(kAUDIO_PLL_to_FLEXCOMM1);
    CLOCK_AttachClk(kAUDIO_PLL_to_FLEXCOMM2);
    CLOCK_AttachClk(kAUDIO_PLL_to_FLEXCOMM3);
    CLOCK_AttachClk(kAUDIO_PLL_to_FLEXCOMM4);

    CLOCK_AttachClk(kAUDIO_PLL_to_MCLK_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivMclkClk, 1);

    SYSCTL1->MCLKPINDIR = SYSCTL1_MCLKPINDIR_MCLKPINDIR_MASK;

    SYSCTL1->MCLKPINDIR = 1;

    i2s1__tx_init();
    //i2s2__rx_init__big_buff();
    i2s2__rx_init();
}

void i2s__crunch()
{



}


