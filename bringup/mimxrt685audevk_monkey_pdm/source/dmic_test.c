#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "MIMXRT685S_cm33.h"
#include "board.h"
#include "pin_mux.h"

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_dma.h"
#include "fsl_dmic.h"
#include "fsl_dmic_dma.h"

#include "dmic_test.h"

#define DMIC_DMA (DMA0)

#define DMIC0_DMA (DMA0)
#define DMIC1_DMA (DMA0)
#define DMIC2_DMA (DMA0)
#define DMIC3_DMA (DMA0)
#define DMIC4_DMA (DMA0)
#define DMIC5_DMA (DMA0)
#define DMIC6_DMA (DMA0)
#define DMIC7_DMA (DMA0)


#define DMIC_DMA_RX_CHANNEL_0 16U
#define DMIC_DMA_RX_CHANNEL_1 17U
#define DMIC_DMA_RX_CHANNEL_2 18U
#define DMIC_DMA_RX_CHANNEL_3 19U
#define DMIC_DMA_RX_CHANNEL_4 20U
#define DMIC_DMA_RX_CHANNEL_5 21U
#define DMIC_DMA_RX_CHANNEL_6 22U
#define DMIC_DMA_RX_CHANNEL_7 23U

#define DMIC_CHANNEL_0 kDMIC_Channel0
#define DMIC_CHANNEL_1 kDMIC_Channel1
#define DMIC_CHANNEL_2 kDMIC_Channel2
#define DMIC_CHANNEL_3 kDMIC_Channel3
#define DMIC_CHANNEL_4 kDMIC_Channel4
#define DMIC_CHANNEL_5 kDMIC_Channel5
#define DMIC_CHANNEL_6 kDMIC_Channel6
#define DMIC_CHANNEL_7 kDMIC_Channel7

static dmic_dma_handle_t s_dmic0DmaHandle;
static dmic_dma_handle_t s_dmic1DmaHandle;
static dmic_dma_handle_t s_dmic2DmaHandle;
static dmic_dma_handle_t s_dmic3DmaHandle;
static dmic_dma_handle_t s_dmic4DmaHandle;
static dmic_dma_handle_t s_dmic5DmaHandle;
static dmic_dma_handle_t s_dmic6DmaHandle;
static dmic_dma_handle_t s_dmic7DmaHandle;


static dma_handle_t s_dmic0RxDmaHandle;
static dma_handle_t s_dmic1RxDmaHandle;
static dma_handle_t s_dmic2RxDmaHandle;
static dma_handle_t s_dmic3RxDmaHandle;
static dma_handle_t s_dmic4RxDmaHandle;
static dma_handle_t s_dmic5RxDmaHandle;
static dma_handle_t s_dmic6RxDmaHandle;
static dma_handle_t s_dmic7RxDmaHandle;

AT_NONCACHEABLE_SECTION_ALIGN(dma_descriptor_t s_dmic0DmaDescriptorPingpong[2], 16);
AT_NONCACHEABLE_SECTION_ALIGN(dma_descriptor_t s_dmic1DmaDescriptorPingpong[2], 16);
AT_NONCACHEABLE_SECTION_ALIGN(dma_descriptor_t s_dmic2DmaDescriptorPingpong[2], 16);
AT_NONCACHEABLE_SECTION_ALIGN(dma_descriptor_t s_dmic3DmaDescriptorPingpong[2], 16);
AT_NONCACHEABLE_SECTION_ALIGN(dma_descriptor_t s_dmic4DmaDescriptorPingpong[2], 16);
AT_NONCACHEABLE_SECTION_ALIGN(dma_descriptor_t s_dmic5DmaDescriptorPingpong[2], 16);
AT_NONCACHEABLE_SECTION_ALIGN(dma_descriptor_t s_dmic6DmaDescriptorPingpong[2], 16);
AT_NONCACHEABLE_SECTION_ALIGN(dma_descriptor_t s_dmic7DmaDescriptorPingpong[2], 16);



static dmic_transfer_t s_dmic0ReceiveXfer[2] =
{
	{
		.data                   = (void *)&MICBuffer[0],
		.dataWidth              = kDMA_Transfer32BitWidth,
		.dataSize               = MIC_CHANNEL_FRAME_SIZE_IN_BYTES,
		.dataAddrInterleaveSize = kDMA_AddressInterleave1xWidth,
		.linkTransfer           = &s_dmic0ReceiveXfer[1],
	},

	{
		.data                   = (void *)&MICBuffer[0][MIC_FRAME_SIZE_SAMPLES],
		.dataWidth              = kDMA_Transfer32BitWidth,
		.dataSize               = MIC_CHANNEL_FRAME_SIZE_IN_BYTES,
		.dataAddrInterleaveSize = kDMA_AddressInterleave1xWidth,
		.linkTransfer           = &s_dmic0ReceiveXfer[0],
	}
};

static dmic_transfer_t s_dmic1ReceiveXfer[2] =
{
	{
		.data                   = (void *)&MICBuffer[1],
		.dataWidth              = kDMA_Transfer32BitWidth,
		.dataSize               = MIC_CHANNEL_FRAME_SIZE_IN_BYTES,
		.dataAddrInterleaveSize = kDMA_AddressInterleave1xWidth,
		.linkTransfer           = &s_dmic1ReceiveXfer[1],
	},

	{
		.data                   = (void *)&MICBuffer[1][MIC_FRAME_SIZE_SAMPLES],
		.dataWidth              = kDMA_Transfer32BitWidth,
		.dataSize               = MIC_CHANNEL_FRAME_SIZE_IN_BYTES,
		.dataAddrInterleaveSize = kDMA_AddressInterleave1xWidth,
		.linkTransfer           = &s_dmic1ReceiveXfer[0],
	}
};

static dmic_transfer_t s_dmic2ReceiveXfer[2] =
{
	{
		.data                   = (void *)&MICBuffer[2],
		.dataWidth              = kDMA_Transfer32BitWidth,
		.dataSize               = MIC_CHANNEL_FRAME_SIZE_IN_BYTES,
		.dataAddrInterleaveSize = kDMA_AddressInterleave1xWidth,
		.linkTransfer           = &s_dmic2ReceiveXfer[1],
	},

	{
		.data                   = (void *)&MICBuffer[2][MIC_FRAME_SIZE_SAMPLES],
		.dataWidth              = kDMA_Transfer32BitWidth,
		.dataSize               = MIC_CHANNEL_FRAME_SIZE_IN_BYTES,
		.dataAddrInterleaveSize = kDMA_AddressInterleave1xWidth,
		.linkTransfer           = &s_dmic2ReceiveXfer[0],
	}
};

static dmic_transfer_t s_dmic3ReceiveXfer[2] =
{
	{
		.data                   = (void *)&MICBuffer[3],
		.dataWidth              = kDMA_Transfer32BitWidth,
		.dataSize               = MIC_CHANNEL_FRAME_SIZE_IN_BYTES,
		.dataAddrInterleaveSize = kDMA_AddressInterleave1xWidth,
		.linkTransfer           = &s_dmic3ReceiveXfer[1],
	},

	{
		.data                   = (void *)&MICBuffer[3][MIC_FRAME_SIZE_SAMPLES],
		.dataWidth              = kDMA_Transfer32BitWidth,
		.dataSize               = MIC_CHANNEL_FRAME_SIZE_IN_BYTES,
		.dataAddrInterleaveSize = kDMA_AddressInterleave1xWidth,
		.linkTransfer           = &s_dmic3ReceiveXfer[0],
	}
};

static dmic_transfer_t s_dmic4ReceiveXfer[2] =
{
	{
		.data                   = (void *)&MICBuffer[4],
		.dataWidth              = kDMA_Transfer32BitWidth,
		.dataSize               = MIC_CHANNEL_FRAME_SIZE_IN_BYTES,
		.dataAddrInterleaveSize = kDMA_AddressInterleave1xWidth,
		.linkTransfer           = &s_dmic4ReceiveXfer[1],
	},

	{
		.data                   = (void *)&MICBuffer[4][MIC_FRAME_SIZE_SAMPLES],
		.dataWidth              = kDMA_Transfer32BitWidth,
		.dataSize               = MIC_CHANNEL_FRAME_SIZE_IN_BYTES,
		.dataAddrInterleaveSize = kDMA_AddressInterleave1xWidth,
		.linkTransfer           = &s_dmic4ReceiveXfer[0],
	}
};

static dmic_transfer_t s_dmic5ReceiveXfer[2] =
{
	{
		.data                   = (void *)&MICBuffer[5],
		.dataWidth              = kDMA_Transfer32BitWidth,
		.dataSize               = MIC_CHANNEL_FRAME_SIZE_IN_BYTES,
		.dataAddrInterleaveSize = kDMA_AddressInterleave1xWidth,
		.linkTransfer           = &s_dmic5ReceiveXfer[1],
	},

	{
		.data                   = (void *)&MICBuffer[5][MIC_FRAME_SIZE_SAMPLES],
		.dataWidth              = kDMA_Transfer32BitWidth,
		.dataSize               = MIC_CHANNEL_FRAME_SIZE_IN_BYTES,
		.dataAddrInterleaveSize = kDMA_AddressInterleave1xWidth,
		.linkTransfer           = &s_dmic5ReceiveXfer[0],
	}
};


static dmic_transfer_t s_dmic6ReceiveXfer[2] =
{
	{
		.data                   = (void *)&MICBuffer[6],
		.dataWidth              = kDMA_Transfer32BitWidth,
		.dataSize               = MIC_CHANNEL_FRAME_SIZE_IN_BYTES,
		.dataAddrInterleaveSize = kDMA_AddressInterleave1xWidth,
		.linkTransfer           = &s_dmic6ReceiveXfer[1],
	},

	{
		.data                   = (void *)&MICBuffer[6][MIC_FRAME_SIZE_SAMPLES],
		.dataWidth              = kDMA_Transfer32BitWidth,
		.dataSize               = MIC_CHANNEL_FRAME_SIZE_IN_BYTES,
		.dataAddrInterleaveSize = kDMA_AddressInterleave1xWidth,
		.linkTransfer           = &s_dmic6ReceiveXfer[0],
	}
};

static dmic_transfer_t s_dmic7ReceiveXfer[2] =
{
	{
		.data                   = (void *)&MICBuffer[7],
		.dataWidth              = kDMA_Transfer32BitWidth,
		.dataSize               = MIC_CHANNEL_FRAME_SIZE_IN_BYTES,
		.dataAddrInterleaveSize = kDMA_AddressInterleave1xWidth,
		.linkTransfer           = &s_dmic7ReceiveXfer[1],
	},

	{
		.data                   = (void *)&MICBuffer[7][MIC_FRAME_SIZE_SAMPLES],
		.dataWidth              = kDMA_Transfer32BitWidth,
		.dataSize               = MIC_CHANNEL_FRAME_SIZE_IN_BYTES,
		.dataAddrInterleaveSize = kDMA_AddressInterleave1xWidth,
		.linkTransfer           = &s_dmic7ReceiveXfer[0],
	}
};


// Mic instance data structure
typedef struct _DMIC_Context {
    int32_t nMicBufferNdx;
    dmic_dma_handle_t dmicDmaHandle;
	dma_handle_t dmicRxDmaHandle;
	uint32_t nMicChannelNdx;
	dmic_transfer_t receiveXfer;
} DMIC_Context;

static dmic_channel_config_t s_dmicChannelConfig = {
	    .divhfclk            = kDMIC_PdmDiv1,
#ifdef SAMPLE_RATE_16K
	    .osr                 = 96U,
#else
	    .osr                 = 48,
#endif
	    .gainshft            = 0,
	    .preac2coef          = kDMIC_CompValueZero,
	    .preac4coef          = kDMIC_CompValueZero,
	    .dc_cut_level        = kDMIC_DcNoRemove,
	    .post_dc_gain_reduce = 0U,
	    .saturate16bit       = 0U, // was 1
	    .sample_rate         = kDMIC_PhyFullSpeed,
		.enableSignExtend    = 1,
};

volatile uint32_t DMIC_Cnt =0 ;

void DmicRxCallback(void *handle, void *userData, bool transferDone, uint32_t intmode)
{
	DMIC_Cnt++;
}


int32_t MICBuffer[MIC_COUNT][MIC_BUFFER_SIZE_SAMPLES];


void DMIC__init(void)
{

    // Init DMA0 for Mics
    DMA_Init(DMA0);

    CLOCK_AttachClk(kAUDIO_PLL_to_DMIC_CLK);                 /* Switch DMIC_CLK to MASTER_CLK */



    CLOCK_SetClkDiv(kCLOCK_DivDmicClk, 4U);         /* Set DMIC0FCLKDIV divider to value 4 */

    DMIC_Init(DMIC0);

    DMIC_Use2fs(DMIC0, false);

	// Configure mic0 channel
	DMA_EnableChannel(DMIC0_DMA, DMIC_DMA_RX_CHANNEL_0);
	DMA_SetChannelPriority(DMIC0_DMA, DMIC_DMA_RX_CHANNEL_0, kDMA_ChannelPriority1);
	DMA_CreateHandle(&s_dmic0RxDmaHandle, DMIC0_DMA, DMIC_DMA_RX_CHANNEL_0);

	DMIC_EnableChannelDma(DMIC0, kDMIC_Channel0, true);
	DMIC_ConfigChannel(DMIC0, kDMIC_Channel0, kDMIC_Left, &s_dmicChannelConfig);
	DMIC_FifoChannel(DMIC0, kDMIC_Channel0, FIFO_DEPTH, true, true);

    DMIC_TransferCreateHandleDMA(DMIC0, &s_dmic0DmaHandle, (dmic_dma_transfer_callback_t)DmicRxCallback, NULL, &s_dmic0RxDmaHandle);
    DMIC_InstallDMADescriptorMemory(&s_dmic0DmaHandle, s_dmic0DmaDescriptorPingpong, 2U);

	// Configure mic1 channel
	DMA_EnableChannel(DMIC1_DMA, DMIC_DMA_RX_CHANNEL_1);
	DMA_SetChannelPriority(DMIC1_DMA, DMIC_DMA_RX_CHANNEL_1, kDMA_ChannelPriority1);
	DMA_CreateHandle(&s_dmic1RxDmaHandle, DMIC1_DMA, DMIC_DMA_RX_CHANNEL_1);

	DMIC_EnableChannelDma(DMIC0, kDMIC_Channel1, true);
	DMIC_ConfigChannel(DMIC0, kDMIC_Channel1, kDMIC_Right, &s_dmicChannelConfig);
	DMIC_FifoChannel(DMIC0, kDMIC_Channel1, FIFO_DEPTH, true, true);

    DMIC_TransferCreateHandleDMA(DMIC0, &s_dmic1DmaHandle, (dmic_dma_transfer_callback_t)NULL, NULL, &s_dmic1RxDmaHandle);
    DMIC_InstallDMADescriptorMemory(&s_dmic1DmaHandle, s_dmic1DmaDescriptorPingpong, 2U);

	// Configure mic2 channel
	DMA_EnableChannel(DMIC2_DMA, DMIC_DMA_RX_CHANNEL_2);
	DMA_SetChannelPriority(DMIC2_DMA, DMIC_DMA_RX_CHANNEL_2, kDMA_ChannelPriority1);
	DMA_CreateHandle(&s_dmic2RxDmaHandle, DMIC2_DMA, DMIC_DMA_RX_CHANNEL_2);

	DMIC_EnableChannelDma(DMIC0, kDMIC_Channel2, true);
	DMIC_ConfigChannel(DMIC0, kDMIC_Channel2, kDMIC_Left, &s_dmicChannelConfig);
	DMIC_FifoChannel(DMIC0, kDMIC_Channel2, FIFO_DEPTH, true, true);

    DMIC_TransferCreateHandleDMA(DMIC0, &s_dmic2DmaHandle, (dmic_dma_transfer_callback_t)NULL, NULL, &s_dmic2RxDmaHandle);
    DMIC_InstallDMADescriptorMemory(&s_dmic2DmaHandle, s_dmic2DmaDescriptorPingpong, 2U);

	// Configure mic3 channel
	DMA_EnableChannel(DMIC3_DMA, DMIC_DMA_RX_CHANNEL_3);
	DMA_SetChannelPriority(DMIC3_DMA, DMIC_DMA_RX_CHANNEL_3, kDMA_ChannelPriority1);
	DMA_CreateHandle(&s_dmic3RxDmaHandle, DMIC3_DMA, DMIC_DMA_RX_CHANNEL_3);

	DMIC_EnableChannelDma(DMIC0, kDMIC_Channel3, true);
	DMIC_ConfigChannel(DMIC0, kDMIC_Channel3, kDMIC_Right, &s_dmicChannelConfig);
	DMIC_FifoChannel(DMIC0, kDMIC_Channel3, FIFO_DEPTH, true, true);

    DMIC_TransferCreateHandleDMA(DMIC0, &s_dmic3DmaHandle, (dmic_dma_transfer_callback_t)NULL, NULL, &s_dmic3RxDmaHandle);
    DMIC_InstallDMADescriptorMemory(&s_dmic3DmaHandle, s_dmic3DmaDescriptorPingpong, 2U);

	// Configure mic4 channel
	DMA_EnableChannel(DMIC4_DMA, DMIC_DMA_RX_CHANNEL_4);
	DMA_SetChannelPriority(DMIC4_DMA, DMIC_DMA_RX_CHANNEL_4, kDMA_ChannelPriority1);
	DMA_CreateHandle(&s_dmic4RxDmaHandle, DMIC4_DMA, DMIC_DMA_RX_CHANNEL_4);

	DMIC_EnableChannelDma(DMIC0, kDMIC_Channel4, true);
	DMIC_ConfigChannel(DMIC0, kDMIC_Channel4, kDMIC_Left, &s_dmicChannelConfig);
	DMIC_FifoChannel(DMIC0, kDMIC_Channel4, FIFO_DEPTH, true, true);

    DMIC_TransferCreateHandleDMA(DMIC0, &s_dmic4DmaHandle, (dmic_dma_transfer_callback_t)NULL, NULL, &s_dmic4RxDmaHandle);
    DMIC_InstallDMADescriptorMemory(&s_dmic4DmaHandle, s_dmic4DmaDescriptorPingpong, 2U);

	// Configure mic5 channel
	DMA_EnableChannel(DMIC5_DMA, DMIC_DMA_RX_CHANNEL_5);
	DMA_SetChannelPriority(DMIC_DMA, DMIC_DMA_RX_CHANNEL_5, kDMA_ChannelPriority1);
	DMA_CreateHandle(&s_dmic5RxDmaHandle, DMIC5_DMA, DMIC_DMA_RX_CHANNEL_5);

	DMIC_EnableChannelDma(DMIC0, kDMIC_Channel5, true);
	DMIC_ConfigChannel(DMIC0, kDMIC_Channel5, kDMIC_Right, &s_dmicChannelConfig);
	DMIC_FifoChannel(DMIC0, kDMIC_Channel5, FIFO_DEPTH, true, true);

    DMIC_TransferCreateHandleDMA(DMIC0, &s_dmic5DmaHandle, (dmic_dma_transfer_callback_t)NULL, NULL, &s_dmic5RxDmaHandle);
    DMIC_InstallDMADescriptorMemory(&s_dmic5DmaHandle, s_dmic5DmaDescriptorPingpong, 2U);


	// Configure mic6 channel
	DMA_EnableChannel(DMIC6_DMA, DMIC_DMA_RX_CHANNEL_6);
	DMA_SetChannelPriority(DMIC6_DMA, DMIC_DMA_RX_CHANNEL_6, kDMA_ChannelPriority1);
	DMA_CreateHandle(&s_dmic6RxDmaHandle, DMIC6_DMA, DMIC_DMA_RX_CHANNEL_6);

	DMIC_EnableChannelDma(DMIC0, kDMIC_Channel6, true);
	DMIC_ConfigChannel(DMIC0, kDMIC_Channel6, kDMIC_Left, &s_dmicChannelConfig);
	DMIC_FifoChannel(DMIC0, kDMIC_Channel6, FIFO_DEPTH, true, true);

    DMIC_TransferCreateHandleDMA(DMIC0, &s_dmic6DmaHandle, (dmic_dma_transfer_callback_t)NULL, NULL, &s_dmic6RxDmaHandle);
    DMIC_InstallDMADescriptorMemory(&s_dmic6DmaHandle, s_dmic6DmaDescriptorPingpong, 2U);



	// Configure mic7 channel
	DMA_EnableChannel(DMIC7_DMA, DMIC_DMA_RX_CHANNEL_7);
	DMA_SetChannelPriority(DMIC7_DMA, DMIC_DMA_RX_CHANNEL_7, kDMA_ChannelPriority1);
	DMA_CreateHandle(&s_dmic7RxDmaHandle, DMIC7_DMA, DMIC_DMA_RX_CHANNEL_7);

	DMIC_EnableChannelDma(DMIC0, kDMIC_Channel7, true);
	DMIC_ConfigChannel(DMIC0, kDMIC_Channel7, kDMIC_Right, &s_dmicChannelConfig);
	DMIC_FifoChannel(DMIC0, kDMIC_Channel7, FIFO_DEPTH, true, true);

    DMIC_TransferCreateHandleDMA(DMIC0, &s_dmic7DmaHandle, (dmic_dma_transfer_callback_t)NULL, NULL, &s_dmic7RxDmaHandle);
    DMIC_InstallDMADescriptorMemory(&s_dmic7DmaHandle, s_dmic7DmaDescriptorPingpong, 2U);

	NVIC_SetPriority(DMA0_IRQn, 3);

    DMIC_EnableChannnel(DMIC0, DMIC_CHANEN_EN_CH0(1));
    DMIC_EnableChannnel(DMIC0, DMIC_CHANEN_EN_CH1(1));
    DMIC_EnableChannnel(DMIC0, DMIC_CHANEN_EN_CH2(1));
    DMIC_EnableChannnel(DMIC0, DMIC_CHANEN_EN_CH3(1));

    DMIC_EnableChannnel(DMIC0, DMIC_CHANEN_EN_CH4(1));
    DMIC_EnableChannnel(DMIC0, DMIC_CHANEN_EN_CH5(1));
    DMIC_EnableChannnel(DMIC0, DMIC_CHANEN_EN_CH6(1));
    DMIC_EnableChannnel(DMIC0, DMIC_CHANEN_EN_CH7(1));

	DMA_DisableChannelInterrupts(DMIC1_DMA, DMIC_DMA_RX_CHANNEL_1);
	DMA_DisableChannelInterrupts(DMIC2_DMA, DMIC_DMA_RX_CHANNEL_2);
	DMA_DisableChannelInterrupts(DMIC3_DMA, DMIC_DMA_RX_CHANNEL_3);
	DMA_DisableChannelInterrupts(DMIC4_DMA, DMIC_DMA_RX_CHANNEL_4);
	DMA_DisableChannelInterrupts(DMIC5_DMA, DMIC_DMA_RX_CHANNEL_5);
	DMA_DisableChannelInterrupts(DMIC6_DMA, DMIC_DMA_RX_CHANNEL_6);
	DMA_DisableChannelInterrupts(DMIC7_DMA, DMIC_DMA_RX_CHANNEL_7);

    DMIC_TransferReceiveDMA(DMIC0, &s_dmic0DmaHandle, s_dmic0ReceiveXfer, kDMIC_Channel0);
    DMIC_TransferReceiveDMA(DMIC0, &s_dmic1DmaHandle, s_dmic1ReceiveXfer, kDMIC_Channel1);
    DMIC_TransferReceiveDMA(DMIC0, &s_dmic2DmaHandle, s_dmic2ReceiveXfer, kDMIC_Channel2);
    DMIC_TransferReceiveDMA(DMIC0, &s_dmic3DmaHandle, s_dmic3ReceiveXfer, kDMIC_Channel3);
    DMIC_TransferReceiveDMA(DMIC0, &s_dmic4DmaHandle, s_dmic4ReceiveXfer, kDMIC_Channel4);
    DMIC_TransferReceiveDMA(DMIC0, &s_dmic5DmaHandle, s_dmic5ReceiveXfer, kDMIC_Channel5);
    DMIC_TransferReceiveDMA(DMIC0, &s_dmic6DmaHandle, s_dmic6ReceiveXfer, kDMIC_Channel6);
    DMIC_TransferReceiveDMA(DMIC0, &s_dmic7DmaHandle, s_dmic7ReceiveXfer, kDMIC_Channel7);



}	// End BOARD_InitDMIC
