/*
 * Copyright (c) 2016 Intel Corporation.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <stdio.h>
#include <string.h>
#include <zephyr/shell/shell.h>
#include <zephyr/drivers/spi.h>

#define NUM_4K_SECTORS 32
#define PAGE_SIZE 256
#define REGION__4K_SECTOR (NUM_4K_SECTORS*0x1000)
#define MEM_SIZE (1<<20)



#define CMD_RDID 		0x9F
#define CMD_WREN 		0x06
#define CMD_RDSR1		0x05
#define CMD_ERASE_EXT 	0x21
#define CMD_PP_EXT 		0x12
#define CMD_READ_EXT 	0x13

#define BIT_WIP 0x01

const struct device *spi_device = DEVICE_DT_GET(DT_NODELABEL(arduino_spi));
	
struct spi_config spi_cfg = {
    .frequency = 10000000,         // 1 MHz
    .operation = SPI_OP_MODE_MASTER | SPI_WORD_SET(8),
    .slave = 0,                   // Slave number if applicable
 };

void rdid(const struct device *spi_dev)
{
	uint8_t tx_buffer[4] = {CMD_RDID, 0, 0,0};

	uint8_t rx_buffer[sizeof(tx_buffer)];
	
	const struct spi_buf tx_buf = {
		.buf = tx_buffer,
		.len = sizeof(tx_buffer),
	};
	const struct spi_buf rx_buf = {
		.buf = rx_buffer,
		.len = sizeof(rx_buffer),
	};
	const struct spi_buf_set tx_bufs = {
		.buffers = &tx_buf,
		.count = 1
	};
	const struct spi_buf_set rx_bufs = {
		.buffers = &rx_buf,
		.count = 1
	};

	spi_transceive(spi_dev, &spi_cfg, &tx_bufs, &rx_bufs);

	printf("\r\nDevice RDID : 0x%02x 0x%02x 0x%02x \r\n\r\n", rx_buffer[1], rx_buffer[2], rx_buffer[3]);
}

uint8_t rdsr(const struct device *spi_dev)
{

	uint8_t tx_buffer[2] = {CMD_RDSR1, 0};

	uint8_t rx_buffer[sizeof(tx_buffer)];
	
	const struct spi_buf tx_buf = {
		.buf = tx_buffer,
		.len = sizeof(tx_buffer),
	};
	const struct spi_buf rx_buf = {
		.buf = rx_buffer,
		.len = sizeof(rx_buffer),
	};
	const struct spi_buf_set tx_bufs = {
		.buffers = &tx_buf,
		.count = 1
	};
	const struct spi_buf_set rx_bufs = {
		.buffers = &rx_buf,
		.count = 1
	};

	 spi_transceive(spi_dev, &spi_cfg, &tx_bufs, &rx_bufs);

	 return rx_buffer[1];
}


void wren(const struct device *spi_dev)
{
	uint8_t tx_buffer[1] = {CMD_WREN};
	
	const struct spi_buf tx_buf = {
		.buf = tx_buffer,
		.len = sizeof(tx_buffer),
	};

	const struct spi_buf_set tx_bufs = {
		.buffers = &tx_buf,
		.count = 1
	};

	spi_transceive(spi_dev, &spi_cfg, &tx_bufs, NULL);
}

void erase_sector(const struct device *spi_dev,uint32_t addr)
{
	uint8_t tx_buffer[5] = {CMD_ERASE_EXT,(addr>>24)&0xFF,(addr>>16)&0xFF,(addr>>8)&0xFF,addr&0xFF};


	const struct spi_buf tx_buf = {
		.buf = tx_buffer,
		.len = sizeof(tx_buffer),
	};

	const struct spi_buf_set tx_bufs = {
		.buffers = &tx_buf,
		.count = 1
	};

	wren(spi_dev);
	spi_transceive(spi_dev, &spi_cfg, &tx_bufs, NULL);

	while(rdsr(spi_dev)&BIT_WIP)
	{
		k_sleep(K_MSEC(1));
	}
}


void pp_256(const struct device *spi_dev,uint32_t addr,uint8_t *data)
{
	uint8_t tx_buffer[256+4+1];
	
	tx_buffer[0] = CMD_PP_EXT;
	tx_buffer[1] = (addr>>24)&0xFF;
	tx_buffer[2] = (addr>>16)&0xFF;
	tx_buffer[3] = (addr>>8)&0xFF;
	tx_buffer[4] = (addr)&0xFF;
	
	memcpy(&tx_buffer[5],data,256);

	const struct spi_buf tx_buf = {
		.buf = tx_buffer,
		.len = sizeof(tx_buffer),
	};

	const struct spi_buf_set tx_bufs = {
		.buffers = &tx_buf,
		.count = 1
	};

	wren(spi_dev);
	
	spi_transceive(spi_dev, &spi_cfg, &tx_bufs, NULL);

	while(rdsr(spi_dev)&BIT_WIP)
	{
		k_sleep(K_MSEC(1));
	}
}


void read_256(const struct device *spi_dev,uint32_t addr,uint8_t *data)
{
	uint8_t tx_buffer[256+4+1];
	uint8_t rx_buffer[256+4+1];
	
	tx_buffer[0] = CMD_READ_EXT;
	tx_buffer[1] = (addr>>24)&0xFF;
	tx_buffer[2] = (addr>>16)&0xFF;
	tx_buffer[3] = (addr>>8)&0xFF;
	tx_buffer[4] = (addr)&0xFF;
	
	memcpy(&tx_buffer[5],data,256);

	const struct spi_buf tx_buf = {
		.buf = tx_buffer,
		.len = sizeof(tx_buffer),
	};
	const struct spi_buf rx_buf = {
		.buf = rx_buffer,
		.len = sizeof(rx_buffer),
	};
	const struct spi_buf_set tx_bufs = {
		.buffers = &tx_buf,
		.count = 1
	};
	const struct spi_buf_set rx_bufs = {
		.buffers = &rx_buf,
		.count = 1
	};

	
	spi_transceive(spi_dev, &spi_cfg, &tx_bufs, &rx_bufs);

	memcpy(data,&rx_buffer[5],256);

}


static int rdid_handle(const struct shell *shell, 
                      size_t argc,
                      char **argv)
{
   ARG_UNUSED(argc);
   ARG_UNUSED(argv);

   rdid(spi_device);

   return 0;
}

int flash_rw_test(const struct device *spi_dev)
{
	uint8_t test_page_out[PAGE_SIZE];

	uint8_t test_page_in[PAGE_SIZE];

	uint32_t address=0;
	uint32_t sector_number=0;


	while(address<MEM_SIZE)
	{

		printf("Erasing sector %d at address 0x%08x\r\n",sector_number,address);
		erase_sector(spi_device,address);

		if(address<=REGION__4K_SECTOR)
		{
			address+=0x1000;
		}
		else
		{
			address+=0x1000;
		}
		
		sector_number++;
		
	}

	for(int addr=0;addr<MEM_SIZE;addr+=PAGE_SIZE)
	{
		for(int i=0;i<PAGE_SIZE;i++)
		{
			test_page_out[i] = i;
		}
		
		if(addr%0x1000==0)
			printf("writing/reading random data to flash address 0x%08x : ",addr);

		pp_256(spi_device, addr,test_page_out);

		memset(test_page_in,0,sizeof(test_page_in));

		read_256(spi_device, addr,test_page_in);

		if(memcmp(test_page_out,test_page_in,PAGE_SIZE)!=0)
		{
			printf("ERROR: Data mismatch at address 0x%08x\n",addr);
			return -1;
		}

		if(addr%0x1000==0)
			printf("[ok]\r\n");

	}


	printf("\r\n%d bytes written/read to flash\r\n",MEM_SIZE);
	printf("Flash interface OK\r\n");


	return 0;
}

static int test_flash(const struct shell *shell, 
                      size_t argc,
                      char **argv)
{
   ARG_UNUSED(argc);
   ARG_UNUSED(argv);

   flash_rw_test(spi_device);

   return 0;
}



SHELL_CMD_REGISTER(flash_id, NULL, "Dump the flash RDID response", rdid_handle);

SHELL_CMD_REGISTER(flash_test, NULL, "Test the flash interface", test_flash);


int main(void)
{
printf("\r\n");
printf("                          █                       \r\n");
printf("                          ███                     \r\n");
printf("                          █████                   \r\n");
printf("                          ██████                  \r\n");
printf("                          ██████                  \r\n");
printf("                          ██████                  \r\n");
printf("                          ██████      █           \r\n");
printf("                          ██████    ███████       \r\n");
printf("              ███████     ██████   ████████       \r\n");
printf("              █████████   ██████  ████████        \r\n");
printf("              ███████████  █████ ████████         \r\n");
printf("                         ██   ██████              \r\n");
printf("                      █████                       \r\n");
printf("                   ████████   ███████             \r\n");
printf("                ██████████  ████████████          \r\n");
printf("             ████████████  ██  ████████████       \r\n");
printf("            █████████      ███     ███████████    \r\n");
printf("            ██████        █████       ███████████ \r\n");
printf("           ████           ███████        █████       \r\n");
printf("           █              ████████                \r\n");
printf("                           ██████                 \r\n");
printf("                             ██                    \r\n");

printf("\r\n");         
printf(" ____         __ _      _              _ _\r\n");         
printf("/ ___|  ___  / _| |_   / \\   ___ _   _(_) |_ _   _\r\n"); 
printf("\\___ \\ / _ \\| |_| __| / _ \\ / __| | | | | __| | | |\r\n");
printf(" ___) | (_) |  _| |_ / ___ \\ (__| |_| | | |_| |_| |\r\n");
printf("|____/ \\___/|_|  \\__/_/   \\_\\___|\\__,_|_|\\__|\\__, |\r\n");
printf("                                             |___/ \r\n");
            

if (!device_is_ready(spi_device)) {
		printf("SPI device not ready.\n");
		return 0;
	}

	while(1)
	{  
		k_sleep(K_MSEC(1000));
		
	}
	
	return 0;
}
