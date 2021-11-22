/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <sys/printk.h>
#include <logging/log.h>

LOG_MODULE_REGISTER(main);


void main(void)
{

//	LOG_INF("The RT685 SuperMonkey is running.");


	while(1)
	{
		k_sleep(K_MSEC(1000));
	}
}
