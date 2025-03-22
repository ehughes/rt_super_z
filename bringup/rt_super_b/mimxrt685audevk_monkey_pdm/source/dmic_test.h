
#ifndef DMIC_TEST_H_
#define DMIC_TEST_H_

void DMIC__init(void);

#define AUDIO_BLOCK_SIZE						320
#define DOUBLE_BUFFER_CNT						2
#define MIC_SAMPLE_SIZE_IN_BYTES                (4)
#define MIC_FRAME_SIZE_SAMPLES                  (AUDIO_BLOCK_SIZE)
#define MIC_BUFFER_SIZE_SAMPLES                 (MIC_FRAME_SIZE_SAMPLES * DOUBLE_BUFFER_CNT)
#define MIC_CHANNEL_FRAME_SIZE_IN_BYTES         (AUDIO_BLOCK_SIZE * MIC_SAMPLE_SIZE_IN_BYTES)


#define FIFO_DEPTH (15U)


#define MIC_COUNT                               8
extern int32_t MICBuffer[MIC_COUNT][MIC_BUFFER_SIZE_SAMPLES];

#endif /* DMIC_TEST_H_ */
