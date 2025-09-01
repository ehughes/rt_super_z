#include "fram.h"
#include <zephyr/drivers/spi.h>
#include <zephyr/logging/log.h>

#include <stdio.h>
#include <stdlib.h>

#define FRAME_SIZE (8)
#define SPI_OP(frame_size) (SPI_OP_MODE_MASTER | SPI_MODE_CPOL | \
	       SPI_MODE_CPHA | SPI_WORD_SET(frame_size) | SPI_LINES_SINGLE)

struct spi_dt_spec fram_spi = SPI_DT_SPEC_GET(DT_NODELABEL(fram), SPI_OP(FRAME_SIZE), 0);

uint8_t write_en [1] = {FRAM_WREN}; // Write Enable command
uint8_t read_sr [2] = {FRAM_RDSR, 0XFF};

uint8_t write_op [3] = {FRAM_WRITE, 0x00, 0x00};        
uint8_t read_op [3] = {FRAM_READ, 0x00, 0x00};    


LOG_MODULE_REGISTER(fram);

#define FRAM_SIZE             (2048*8)     //2kb
#define FRAM_TEST_CHUNK_SIZE  8 
uint8_t write_data[FRAM_TEST_CHUNK_SIZE] = {0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8}; // Example data to write
uint8_t read_data[FRAM_TEST_CHUNK_SIZE+3] = {0};

int8_t fram__init(void)
{
    if(!spi_is_ready_dt(&fram_spi))
	{
		LOG_ERR("SPI bus is not ready");
		return -1;
	}

    for(int iter=0;iter<256;iter++)
    {
        LOG_INF("test iteration %d\r\n",iter);

        for(int c=0; c<(FRAM_SIZE/FRAM_TEST_CHUNK_SIZE); c+=FRAM_TEST_CHUNK_SIZE)
        {
            
            LOG_INF("Checking %d bytes at address 0x%04x",FRAM_TEST_CHUNK_SIZE, c);

        }
        
        for(int c=0; c<(FRAM_SIZE/FRAM_TEST_CHUNK_SIZE); c+=FRAM_TEST_CHUNK_SIZE)
        {
            
            LOG_INF("Checking %d bytes at address 0x%04x",FRAM_TEST_CHUNK_SIZE,c);

            for(int i=0;i<FRAM_TEST_CHUNK_SIZE;i++)
            {
                write_data[i] = rand();
            }

            fram__spi_write(write_en, sizeof(write_en));

            fram__write(write_data, sizeof(write_data), c); 
            fram__read(read_data, sizeof(write_data), c);

            if(memcmp(write_data, &read_data[3], sizeof(write_data)) != 0)
            {
                LOG_ERR("Data mismatch after write/read operation at address 0x%04x",c);
                return -1;
            }
            k_sleep(K_MSEC(1));
        }
    }


    LOG_INF("FRAM initialized and test passed.");
    return 0;
}

void fram__spi_write(uint8_t * buffer, uint8_t size)
{
	struct spi_buf tx_bufs[1];

	const struct spi_buf_set tx = {
		.buffers = tx_bufs,
		.count = ARRAY_SIZE(tx_bufs)
	};

	tx_bufs[0].buf = buffer;
	tx_bufs[0].len = size;

	spi_write_dt(&fram_spi, &tx);
}


void fram__spi_write_read(uint8_t * tx_buffer, uint8_t * rx_buffer, uint8_t size_tx, uint8_t size_rx)
{
    struct spi_buf tx_bufs[1];
    struct spi_buf rx_bufs[1];

    const struct spi_buf_set tx = {
        .buffers = tx_bufs,
        .count = ARRAY_SIZE(tx_bufs)
    };

    const struct spi_buf_set rx = {
        .buffers = rx_bufs,
        .count = ARRAY_SIZE(rx_bufs)
    };

    tx_bufs[0].buf = tx_buffer;
    tx_bufs[0].len = size_tx;

    rx_bufs[0].buf = rx_buffer;
    rx_bufs[0].len = size_rx;

    spi_transceive_dt(&fram_spi, &tx, &rx);
}


void fram__write(uint8_t * data, uint8_t size, uint16_t start_address)
{
    struct spi_buf tx_bufs[2];

	const struct spi_buf_set tx = {
		.buffers = tx_bufs,
		.count = ARRAY_SIZE(tx_bufs)
	};

    // Prepare write opcode and start address
    memcpy(&write_op[1], &start_address, sizeof(start_address));
    tx_bufs[0].buf = write_op; 
    tx_bufs[0].len = sizeof(write_op);

    // Prepare data to write
	tx_bufs[1].buf = data;
	tx_bufs[1].len = size;

	spi_write_dt(&fram_spi, &tx);   
}


void fram__read(uint8_t * data, uint8_t size, uint16_t start_address)
{
    struct spi_buf tx_bufs[1];
    struct spi_buf rx_bufs[1];

    uint32_t op_size = sizeof(read_op) + size;

    const struct spi_buf_set tx = {
        .buffers = tx_bufs,
        .count = ARRAY_SIZE(tx_bufs)
    };

    const struct spi_buf_set rx = {
        .buffers = rx_bufs,
        .count = ARRAY_SIZE(rx_bufs)
    };

    // Prepare read opcode and start address
    memcpy(&read_op[1], &start_address, sizeof(start_address));
    tx_bufs[0].buf = read_op;
    tx_bufs[0].len = op_size;

    // Prepare buffer for reading data
    rx_bufs[0].buf = data;
    rx_bufs[0].len = op_size;

    spi_transceive_dt(&fram_spi, &tx, &rx);
}
