#ifndef FRAM_H
#define FRAM_H

#include <zephyr/kernel.h>


#define FRAM_WREN   0x06
#define FRAM_RDSR   0x05
#define FRAM_WRSR   0x01
#define FRAM_READ   0x03
#define FRAM_WRITE  0x02

void fram__spi_read(uint8_t * buffer, uint8_t size);
void fram__spi_write(uint8_t * buffer, uint8_t size);
void fram__spi_write_read(uint8_t * tx_buffer, uint8_t * rx_buffer, uint8_t size_tx, uint8_t size_rx);

void fram__write(uint8_t * data, uint8_t size, uint16_t start_address);
void fram__read(uint8_t * data, uint8_t size, uint16_t start_address);


int8_t fram__init(void);

#endif