

     _ram_mpu_ro_region_start = ORIGIN(RAM);



/* Linker section for memory region RAM for TEXT section  */

	SECTION_PROLOGUE(.ram_text_reloc,,)
        {
                __ram_text_reloc_start = .;
                
                KEEP(*flash_clock_setup.c.obj(.rel.text.flexspi_clock_safe_config))

                KEEP(*flash_clock_setup.c.obj(.text.flexspi_clock_safe_config))

                KEEP(*flash_clock_setup.c.obj(.text.flexspi_setup_clock))

                KEEP(*flash_mcux_flexspi_nor.c.obj(.rel.text.flash_flexspi_nor_erase))

                KEEP(*flash_mcux_flexspi_nor.c.obj(.rel.text.flash_flexspi_nor_init))

                KEEP(*flash_mcux_flexspi_nor.c.obj(.rel.text.flash_flexspi_nor_read))

                KEEP(*flash_mcux_flexspi_nor.c.obj(.rel.text.flash_flexspi_nor_wait_bus_busy.isra.0))

                KEEP(*flash_mcux_flexspi_nor.c.obj(.rel.text.flash_flexspi_nor_write))

                KEEP(*flash_mcux_flexspi_nor.c.obj(.rel.text.flash_flexspi_nor_write_enable.isra.0))

                KEEP(*flash_mcux_flexspi_nor.c.obj(.text.flash_flexspi_nor_erase))

                KEEP(*flash_mcux_flexspi_nor.c.obj(.text.flash_flexspi_nor_get_parameters))

                KEEP(*flash_mcux_flexspi_nor.c.obj(.text.flash_flexspi_nor_init))

                KEEP(*flash_mcux_flexspi_nor.c.obj(.text.flash_flexspi_nor_pages_layout))

                KEEP(*flash_mcux_flexspi_nor.c.obj(.text.flash_flexspi_nor_read))

                KEEP(*flash_mcux_flexspi_nor.c.obj(.text.flash_flexspi_nor_wait_bus_busy.isra.0))

                KEEP(*flash_mcux_flexspi_nor.c.obj(.text.flash_flexspi_nor_write))

                KEEP(*flash_mcux_flexspi_nor.c.obj(.text.flash_flexspi_nor_write_enable.isra.0))

                KEEP(*fsl_flexspi.c.obj(.rel.text.FLEXSPI_Deinit))

                KEEP(*fsl_flexspi.c.obj(.rel.text.FLEXSPI_DriverIRQHandler))

                KEEP(*fsl_flexspi.c.obj(.rel.text.FLEXSPI_GetDefaultConfig))

                KEEP(*fsl_flexspi.c.obj(.rel.text.FLEXSPI_Init))

                KEEP(*fsl_flexspi.c.obj(.rel.text.FLEXSPI_ReadBlocking))

                KEEP(*fsl_flexspi.c.obj(.rel.text.FLEXSPI_SetFlashConfig))

                KEEP(*fsl_flexspi.c.obj(.rel.text.FLEXSPI_TransferBlocking))

                KEEP(*fsl_flexspi.c.obj(.rel.text.FLEXSPI_TransferCreateHandle))

                KEEP(*fsl_flexspi.c.obj(.rel.text.FLEXSPI_TransferHandleIRQ))

                KEEP(*fsl_flexspi.c.obj(.rel.text.FLEXSPI_UpdateDllValue))

                KEEP(*fsl_flexspi.c.obj(.rel.text.FLEXSPI_UpdateLUT))

                KEEP(*fsl_flexspi.c.obj(.rel.text.FLEXSPI_UpdateRxSampleClock))

                KEEP(*fsl_flexspi.c.obj(.rel.text.FLEXSPI_WriteBlocking))

                KEEP(*fsl_flexspi.c.obj(.text.FLEXSPI_CheckAndClearError))

                KEEP(*fsl_flexspi.c.obj(.text.FLEXSPI_Deinit))

                KEEP(*fsl_flexspi.c.obj(.text.FLEXSPI_DriverIRQHandler))

                KEEP(*fsl_flexspi.c.obj(.text.FLEXSPI_GetBusIdleStatus))

                KEEP(*fsl_flexspi.c.obj(.text.FLEXSPI_GetDefaultConfig))

                KEEP(*fsl_flexspi.c.obj(.text.FLEXSPI_GetInstance))

                KEEP(*fsl_flexspi.c.obj(.text.FLEXSPI_Init))

                KEEP(*fsl_flexspi.c.obj(.text.FLEXSPI_ReadBlocking))

                KEEP(*fsl_flexspi.c.obj(.text.FLEXSPI_SetFlashConfig))

                KEEP(*fsl_flexspi.c.obj(.text.FLEXSPI_SoftwareReset))

                KEEP(*fsl_flexspi.c.obj(.text.FLEXSPI_TransferAbort))

                KEEP(*fsl_flexspi.c.obj(.text.FLEXSPI_TransferBlocking))

                KEEP(*fsl_flexspi.c.obj(.text.FLEXSPI_TransferCreateHandle))

                KEEP(*fsl_flexspi.c.obj(.text.FLEXSPI_TransferGetCount))

                KEEP(*fsl_flexspi.c.obj(.text.FLEXSPI_TransferHandleIRQ))

                KEEP(*fsl_flexspi.c.obj(.text.FLEXSPI_TransferNonBlocking))

                KEEP(*fsl_flexspi.c.obj(.text.FLEXSPI_UpdateDllValue))

                KEEP(*fsl_flexspi.c.obj(.text.FLEXSPI_UpdateLUT))

                KEEP(*fsl_flexspi.c.obj(.text.FLEXSPI_UpdateRxSampleClock))

                KEEP(*fsl_flexspi.c.obj(.text.FLEXSPI_WriteBlocking))

                KEEP(*memc_mcux_flexspi.c.obj(.rel.text.memc_flexspi_init))

                KEEP(*memc_mcux_flexspi.c.obj(.rel.text.memc_flexspi_set_device_config))

                KEEP(*memc_mcux_flexspi.c.obj(.rel.text.memc_flexspi_transfer))

                KEEP(*memc_mcux_flexspi.c.obj(.rel.text.memc_flexspi_update_lut))

                KEEP(*memc_mcux_flexspi.c.obj(.text.memc_flexspi_get_ahb_address))

                KEEP(*memc_mcux_flexspi.c.obj(.text.memc_flexspi_init))

                KEEP(*memc_mcux_flexspi.c.obj(.text.memc_flexspi_is_running_xip))

                KEEP(*memc_mcux_flexspi.c.obj(.text.memc_flexspi_reset))

                KEEP(*memc_mcux_flexspi.c.obj(.text.memc_flexspi_set_device_config))

                KEEP(*memc_mcux_flexspi.c.obj(.text.memc_flexspi_transfer))

                KEEP(*memc_mcux_flexspi.c.obj(.text.memc_flexspi_update_clock))

                KEEP(*memc_mcux_flexspi.c.obj(.text.memc_flexspi_update_lut))

                KEEP(*memc_mcux_flexspi.c.obj(.text.memc_flexspi_wait_bus_idle))

#if 0
                . = ALIGN(0);
#else
                MPU_ALIGN(__ram_text_reloc_size);
#endif
                __ram_text_reloc_end = .;
	} 
#ifdef CONFIG_XIP
GROUP_DATA_LINK_IN(RAM , ROMABLE_REGION)
#else
GROUP_DATA_LINK_IN(RAM , RAM )
#endif

        __ram_text_reloc_size = __ram_text_reloc_end - __ram_text_reloc_start;

        __ram_text_rom_start = LOADADDR(.ram_text_reloc);


    _ram_mpu_ro_region_end = .;

