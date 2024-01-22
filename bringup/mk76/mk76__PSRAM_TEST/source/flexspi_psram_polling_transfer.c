/*
 * Copyright 2019-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_flexspi.h"
#include "fsl_debug_console.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_common.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_FLEXSPI                    BOARD_FLEXSPI_PSRAM
#define EXAMPLE_FLEXSPI_AMBA_BASE          FlexSPI_AMBA_BASE
#define EXAMPLE_FLEXSPI_PORT               kFLEXSPI_PortA1
#define HYPERRAM_CMD_LUT_SEQ_IDX_READDATA  0
#define HYPERRAM_CMD_LUT_SEQ_IDX_WRITEDATA 1
#define HYPERRAM_CMD_LUT_SEQ_IDX_READREG   2
#define HYPERRAM_CMD_LUT_SEQ_IDX_WRITEREG  3
#define HYPERRAM_CMD_LUT_SEQ_IDX_RESET     4
#define DRAM_SIZE                          0x800000U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint8_t s_hyper_ram_write_buffer[1024];
static uint8_t s_hyper_ram_read_buffer[1024];

/*******************************************************************************
 * Code
 ******************************************************************************/


volatile uint32_t g_systickCounter;

void SysTick_Handler(void)
{
    if (g_systickCounter != 0U)
    {
        g_systickCounter--;
    }
}

void SysTick_DelayTicks(uint32_t n)
{
    g_systickCounter = n;
    while (g_systickCounter != 0U)
    {
    }
}

status_t flexspi_hyper_ram_ipcommand_write_data(FLEXSPI_Type *base, uint32_t address, uint32_t *buffer, uint32_t length)
{
    flexspi_transfer_t flashXfer;
    status_t status;

    /* Write data */
    flashXfer.deviceAddress = address;
    flashXfer.port          = EXAMPLE_FLEXSPI_PORT;
    flashXfer.cmdType       = kFLEXSPI_Write;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = HYPERRAM_CMD_LUT_SEQ_IDX_WRITEDATA;
    flashXfer.data          = buffer;
    flashXfer.dataSize      = length;

    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    return status;
}

status_t flexspi_hyper_ram_ipcommand_read_data(FLEXSPI_Type *base, uint32_t address, uint32_t *buffer, uint32_t length)
{
    flexspi_transfer_t flashXfer;
    status_t status;

    /* Read data */
    flashXfer.deviceAddress = address;
    flashXfer.port          = EXAMPLE_FLEXSPI_PORT;
    flashXfer.cmdType       = kFLEXSPI_Read;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = HYPERRAM_CMD_LUT_SEQ_IDX_READDATA;
    flashXfer.data          = buffer;
    flashXfer.dataSize      = length;

    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    return status;
}

void flexspi_hyper_ram_ahbcommand_write_data(FLEXSPI_Type *base, uint32_t address, uint32_t *buffer, uint32_t length)
{
    uint32_t *startAddr = (uint32_t *)(EXAMPLE_FLEXSPI_AMBA_BASE + address);
    memcpy(startAddr, buffer, length);
}

void flexspi_hyper_ram_ahbcommand_read_data(FLEXSPI_Type *base, uint32_t address, uint32_t *buffer, uint32_t length)
{
    uint32_t *startAddr = (uint32_t *)(EXAMPLE_FLEXSPI_AMBA_BASE + address);
    memcpy(buffer, startAddr, length);
}

int main(void)
{
    uint32_t i  = 0;
    status_t st = kStatus_Success;

    BOARD_InitBootPins();
    BOARD_InitPsRamPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    status_t status = BOARD_InitPsRam();
    if (status != kStatus_Success)
    {
        assert(false);
    }

    PRINTF("FLEXSPI example started!\r\n");

    for (i = 0; i < sizeof(s_hyper_ram_write_buffer); i++)
    {
        s_hyper_ram_write_buffer[i] = i;
    }

    /* IP command write/read, should notice that the start address should be even address and the write address/size
     * should be 1024 aligned.*/

    /*
    for (i = 0; i < DRAM_SIZE; i += 1024)
    {
        st = flexspi_hyper_ram_ipcommand_write_data(EXAMPLE_FLEXSPI, i, (uint32_t *)s_hyper_ram_write_buffer,
                                                    sizeof(s_hyper_ram_write_buffer));

        if (st != kStatus_Success)
        {
            st = kStatus_Fail;
            PRINTF("IP Command Write data Failure at 0x%x!\r\n", i);
        }

        st = flexspi_hyper_ram_ipcommand_read_data(EXAMPLE_FLEXSPI, i, (uint32_t *)s_hyper_ram_read_buffer,
                                                   sizeof(s_hyper_ram_read_buffer));
        if (st != kStatus_Success)
        {
            st = kStatus_Fail;
            PRINTF("IP Command Read data Failure at 0x%x!\r\n", i);
        }

        if (memcmp(s_hyper_ram_read_buffer, s_hyper_ram_write_buffer, sizeof(s_hyper_ram_write_buffer)) != 0)
        {
            PRINTF("IP Command Read/Write data Failure at 0x%x - 0x%x!\r\n", i, i + 1023);
            return -1;
        }
    }

    PRINTF("IP Command Read/Write data succeed at all address range !\r\n");
*/
    /* Need to reset FlexSPI controller between IP/AHB access. */
    FLEXSPI_SoftwareReset(EXAMPLE_FLEXSPI);
    SystemCoreClockUpdate();

    SysTick_Config(SystemCoreClock / 1000U);


#define VT100_RED     "\033[31;40m"
#define VT100_GREEN   "\033[32;40m"
#define VT100_YELLOW  "\033[33;40m"
#define VT100_BLUE    "\033[34;40m"
#define VT100_MAGENTA "\033[35;40m"
#define VT100_CYAN    "\033[36;40m"
#define VT100_WHITE   "\033[37;40m"





   	PRINTF("\033[2J");
	PRINTF("\033[?25l");
   	PRINTF("\033[H");

    memset(s_hyper_ram_read_buffer, 0, sizeof(s_hyper_ram_read_buffer));


    int iteration = 0;
    int ii = 0;
    for(int x=0;x<1;x++)
    {
		for (i = 0; i < DRAM_SIZE; i += 1024)
		{
			iteration++;

			//SysTick_DelayTicks(100);
			//iteration&=0x3;
/*
if(iteration == 0)
{

			PRINTF(VT100_RED"                                    \r\n");
			PRINTF(VT100_RED"             ▓▓▓▓▓▓▓▓▓▓             \r\n");
			PRINTF(VT100_RED"           ▓▓▓▓▓▓▓▓▓▓▓▓▓▓           \r\n");
			PRINTF(VT100_RED"         ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓         \r\n");
			PRINTF(VT100_RED"       ▓▓▓▓░░░░░░▓▓░░░░░░▓▓▓▓       \r\n");
			PRINTF(VT100_RED"   ░░░░▓▓░░░░░░░░░░░░░░░░░░▓▓░░░░   \r\n");
			PRINTF(VT100_RED"   ░░░░▓▓░░  ██░░░░░░  ██░░▓▓░░░░   \r\n");
			PRINTF(VT100_RED"     ░░▓▓░░████░░░░░░████░░▓▓░░     \r\n");
			PRINTF(VT100_RED"       ▓▓░░░░░░░░░░░░░░░░░░▓▓       \r\n");
			PRINTF(VT100_RED"         ▓▓░░░░░░░░░░░░░░▓▓         \r\n");
			PRINTF(VT100_RED"           ▓▓▓▓░░░░░░▓▓▓▓           \r\n");
			PRINTF(VT100_RED"               ▓▓▓▓▓▓        ░░     \r\n");
			PRINTF(VT100_RED"             ▓▓▓▓▓▓▓▓▓▓      ▓▓     \r\n");
			PRINTF(VT100_RED"             ▓▓▓▓▓▓▓▓▓▓    ▓▓▓▓     \r\n");
			PRINTF(VT100_RED"           ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓       \r\n");
			PRINTF(VT100_RED"           ▓▓▓▓░░▓▓░░▓▓▓▓           \r\n");
			PRINTF(VT100_RED"                                    \r\n\r\n");
}
if(iteration == 1)
{

			PRINTF(VT100_RED"                                    \r\n");
			PRINTF(VT100_RED"             ▓▓▓▓▓▓▓▓▓▓             \r\n");
			PRINTF(VT100_RED"           ▓▓▓▓▓▓▓▓▓▓▓▓▓▓           \r\n");
			PRINTF(VT100_RED"         ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓         \r\n");
			PRINTF(VT100_RED"       ▓▓▓▓░░░░░░▓▓░░░░░░▓▓▓▓       \r\n");
			PRINTF(VT100_RED"   ░░░░▓▓░░░░░░░░░░░░░░░░░░▓▓░░░░   \r\n");
			PRINTF(VT100_RED"   ░░░░▓▓░░██  ░░░░░░██  ░░▓▓░░░░   \r\n");
			PRINTF(VT100_RED"     ░░▓▓░░████░░░░░░████░░▓▓░░     \r\n");
			PRINTF(VT100_RED"       ▓▓░░░░░░░░░░░░░░░░░░▓▓       \r\n");
			PRINTF(VT100_RED"         ▓▓░░░░░░░░░░░░░░▓▓         \r\n");
			PRINTF(VT100_RED"           ▓▓▓▓░░░░░░▓▓▓▓           \r\n");
			PRINTF(VT100_RED"               ▓▓▓▓▓▓         ░░     \r\n");
			PRINTF(VT100_RED"             ▓▓▓▓▓▓▓▓▓▓      ▓▓     \r\n");
			PRINTF(VT100_RED"             ▓▓▓▓▓▓▓▓▓▓    ▓▓▓▓     \r\n");
			PRINTF(VT100_RED"           ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓       \r\n");
			PRINTF(VT100_RED"           ▓▓▓▓░░▓▓░░▓▓▓▓           \r\n");
			PRINTF(VT100_RED"                                    \r\n\r\n");
}
 if(iteration == 2)
{

	PRINTF(VT100_RED"                                    \r\n");
	PRINTF(VT100_RED"             ▓▓▓▓▓▓▓▓▓▓             \r\n");
	PRINTF(VT100_RED"           ▓▓▓▓▓▓▓▓▓▓▓▓▓▓           \r\n");
	PRINTF(VT100_RED"         ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓         \r\n");
	PRINTF(VT100_RED"       ▓▓▓▓░░░░░░▓▓░░░░░░▓▓▓▓       \r\n");
	PRINTF(VT100_RED"   ░░░░▓▓░░░░░░░░░░░░░░░░░░▓▓░░░░   \r\n");
	PRINTF(VT100_RED"   ░░░░▓▓░░████░░░░░░████░░▓▓░░░░   \r\n");
	PRINTF(VT100_RED"     ░░▓▓░░██  ░░░░░░██  ░░▓▓░░     \r\n");
	PRINTF(VT100_RED"       ▓▓░░░░░░░░░░░░░░░░░░▓▓       \r\n");
	PRINTF(VT100_RED"         ▓▓░░░░░░░░░░░░░░▓▓         \r\n");
	PRINTF(VT100_RED"           ▓▓▓▓░░░░░░▓▓▓▓           \r\n");
	PRINTF(VT100_RED"               ▓▓▓▓▓▓          ░░     \r\n");
	PRINTF(VT100_RED"             ▓▓▓▓▓▓▓▓▓▓       ▓▓     \r\n");
	PRINTF(VT100_RED"             ▓▓▓▓▓▓▓▓▓▓    ▓▓▓▓     \r\n");
	PRINTF(VT100_RED"           ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓       \r\n");
	PRINTF(VT100_RED"           ▓▓▓▓░░▓▓░░▓▓▓▓           \r\n");
	PRINTF(VT100_RED"                                    \r\n\r\n");
}
 if(iteration == 3)
{
	PRINTF(VT100_RED"                                    \r\n");
	PRINTF(VT100_RED"             ▓▓▓▓▓▓▓▓▓▓             \r\n");
	PRINTF(VT100_RED"           ▓▓▓▓▓▓▓▓▓▓▓▓▓▓           \r\n");
	PRINTF(VT100_RED"         ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓         \r\n");
	PRINTF(VT100_RED"       ▓▓▓▓░░░░░░▓▓░░░░░░▓▓▓▓       \r\n");
	PRINTF(VT100_RED"   ░░░░▓▓░░░░░░░░░░░░░░░░░░▓▓░░░░   \r\n");
	PRINTF(VT100_RED"   ░░░░▓▓░░████░░░░░░████░░▓▓░░░░   \r\n");
	PRINTF(VT100_RED"     ░░▓▓░░  ██░░░░░░  ██░░▓▓░░     \r\n");
	PRINTF(VT100_RED"       ▓▓░░░░░░░░░░░░░░░░░░▓▓       \r\n");
	PRINTF(VT100_RED"         ▓▓░░░░░░░░░░░░░░▓▓         \r\n");
	PRINTF(VT100_RED"           ▓▓▓▓░░░░░░▓▓▓▓           \r\n");
	PRINTF(VT100_RED"               ▓▓▓▓▓▓         ░░     \r\n");
	PRINTF(VT100_RED"             ▓▓▓▓▓▓▓▓▓▓      ▓▓     \r\n");
	PRINTF(VT100_RED"             ▓▓▓▓▓▓▓▓▓▓    ▓▓▓▓     \r\n");
	PRINTF(VT100_RED"           ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓       \r\n");
	PRINTF(VT100_RED"           ▓▓▓▓░░▓▓░░▓▓▓▓           \r\n");
	PRINTF(VT100_RED"                                    \r\n\r\n");
}
*/

			if(iteration%0x2F == 0)
			{
				PRINTF("\033[H");
		    PRINTF(VT100_WHITE"RT685 DDR OCTAL PSRAM STRESS TEST\r\n");
	        PRINTF(VT100_GREEN"------------------------------------------\r\n");
			PRINTF(VT100_GREEN"Address    : "VT100_YELLOW"0x%8x"VT100_GREEN"  Iteration : "VT100_MAGENTA"%i\r\n",i,ii++);
			}
			SysTick_DelayTicks(1);

			for (int j = 0; j < sizeof(s_hyper_ram_write_buffer);j++)
			{
				s_hyper_ram_write_buffer[j] = rand();
			}

			flexspi_hyper_ram_ahbcommand_write_data(EXAMPLE_FLEXSPI, i, (uint32_t *)s_hyper_ram_write_buffer,
													sizeof(s_hyper_ram_write_buffer));
			flexspi_hyper_ram_ahbcommand_read_data(EXAMPLE_FLEXSPI, i, (uint32_t *)s_hyper_ram_read_buffer,
												   sizeof(s_hyper_ram_write_buffer));

			if (memcmp(s_hyper_ram_read_buffer, s_hyper_ram_write_buffer, sizeof(s_hyper_ram_write_buffer)) != 0)
			{
				PRINTF("AHB Command Read/Write data Failure at 0x%x - 0x%x!\r\n", i, i + 1023);
				return -1;
			}
		}
    }
    PRINTF("AHB Command Read/Write data succeed at all address range !\r\n");

    while (1)
    {
    }
}
