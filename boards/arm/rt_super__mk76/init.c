/*
 * Copyright (c) 2020,  NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/init.h>
#include <zephyr/devicetree.h>
#include <fsl_device_registers.h>

static int rt_super__mk76_init(void)
{

#ifdef CONFIG_REBOOT
	/*
	 * The sys_reboot API calls NVIC_SystemReset. On the RT685, the warm
	 * reset will not complete correctly unless the ROM toggles the
	 * flash reset pin. We can control this behavior using the OTP shadow
	 * register for OPT word BOOT_CFG1
	 *
	 * Set FLEXSPI_RESET_PIN_ENABLE=1, FLEXSPI_RESET_PIN= PIO2_12
	 */
	 OCOTP->OTP_SHADOW[97] = 0x314000;
#endif /* CONFIG_REBOOT */

	return 0;
}

SYS_INIT(rt_super__mk76_init, PRE_KERNEL_1, CONFIG_BOARD_INIT_PRIORITY);
