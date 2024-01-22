
zephyr_library_sources(flash_config.c)

#
# Copyright 2020-2022 NXP
#
# SPDX-License-Identifier: Apache-2.0
#

zephyr_library()
zephyr_library_sources(init.c)

if(CONFIG_NXP_IMX_RT6XX_BOOT_HEADER)
  zephyr_compile_definitions(BOOT_HEADER_ENABLE=1)
  zephyr_compile_definitions(BOARD_FLASH_SIZE=CONFIG_FLASH_SIZE*1024)
 #zephyr_library_sources(flash_config.c)
endif()




