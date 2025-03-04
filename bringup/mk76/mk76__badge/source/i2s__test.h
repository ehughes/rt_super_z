#include "stdint.h"
#include "stdbool.h"

#ifndef _I2S1_TEST_H
#define _I2S1_TEST_H


void i2s__init();

#define AUDIO_SAMPLE_RATE 48000
#define AUDIO_BLOCK_SIZE 48
#define DOUBLE_BUFFER_CNT 2

#define SAMPLE_TYPE int16_t

#define I2S_TX_CHANNEL_COUNT 4
#define I2S_FRAME_SIZE_SAMPLES			 (I2S_TX_CHANNEL_COUNT * AUDIO_BLOCK_SIZE)
#define I2S_TX_BUFFER_SIZE_SAMPLES 		 (I2S_FRAME_SIZE_SAMPLES * DOUBLE_BUFFER_CNT)
#define I2S_TX_SIZE_BYTES 				 (I2S_FRAME_SIZE_SAMPLES * sizeof(SAMPLE_TYPE))


#define AUDIO_RX_BLOCK_SIZE  			 320
#define I2S_RX_CHANNEL_COUNT			 2
#define I2S_RX_FRAME_SIZE_SAMPLES		 (I2S_RX_CHANNEL_COUNT * AUDIO_RX_BLOCK_SIZE)
#define I2S_RX_BUFFER_SIZE_SAMPLES 		 (I2S_RX_FRAME_SIZE_SAMPLES * DOUBLE_BUFFER_CNT)
#define I2S_RX_SIZE_BYTES 				 (I2S_RX_FRAME_SIZE_SAMPLES * sizeof(SAMPLE_TYPE))


#define BIG_BUFFER_SAMPLES				  512
#define BIG_BUFFER_BYTES				  (BIG_BUFFER_SAMPLES	*sizeof(SAMPLE_TYPE))

extern  int16_t I2S2_Rx_Buffer[I2S_RX_BUFFER_SIZE_SAMPLES];
extern  volatile bool I2S2_Rx_Complete;

void i2s2__rx_capture(int16_t *buffer,uint32_t num_samples);

#endif
