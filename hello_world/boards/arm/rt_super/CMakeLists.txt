#
# Copyright (c) 2020, NXP
#
# SPDX-License-Identifier: Apache-2.0
#

zephyr_library()

zephyr_library_sources(pinmux.c)

zephyr_compile_definitions_ifdef(CONFIG_NXP_IMX_RT6XX_BOOT_HEADER BOOT_HEADER_ENABLE=1)

zephyr_library_sources(flash_config.c)

