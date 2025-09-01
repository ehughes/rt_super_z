/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include "fram.h"
#include <zephyr/usb/usb_device.h>

LOG_MODULE_REGISTER(main);


int prev_init()
{
	CLOCK_AttachClk(kFFRO_to_FLEXCOMM14);
	return 0;
}

SYS_INIT(prev_init, PRE_KERNEL_1, 0);

int main(void)
{
	int8_t ret;
	LOG_INF("Hello FRAM bringup! %s\n", CONFIG_BOARD_TARGET);

	ret = fram__init();

		if (ret == 0)
		{
			LOG_INF("FRAM initialization correct\n");
		}
		else
		{
			LOG_ERR("FRAM initialization failed\n");
		}
		
	while(1)
	{
	

		k_sleep(K_MSEC(500));
	}

	return 0;
}
