#include <device.h>
#include <toolchain.h>

/* 1 : /soc/peripheral@50000000/clkctl@21000:
 * Direct Dependencies:
 *   - (/soc/peripheral@50000000)
 * Supported:
 *   - (/soc/peripheral@50000000/ctimer@28000)
 *   - (/soc/peripheral@50000000/ctimer@29000)
 *   - (/soc/peripheral@50000000/ctimer@2a000)
 *   - (/soc/peripheral@50000000/ctimer@2b000)
 *   - (/soc/peripheral@50000000/ctimer@2c000)
 *   - /soc/peripheral@50000000/flexcomm@106000
 *   - (/soc/peripheral@50000000/flexcomm@107000)
 *   - (/soc/peripheral@50000000/flexcomm@108000)
 *   - (/soc/peripheral@50000000/flexcomm@109000)
 *   - (/soc/peripheral@50000000/flexcomm@122000)
 *   - (/soc/peripheral@50000000/flexcomm@123000)
 *   - (/soc/peripheral@50000000/flexcomm@124000)
 *   - (/soc/peripheral@50000000/flexcomm@125000)
 *   - (/soc/peripheral@50000000/spi@126000)
 *   - (/soc/peripheral@50000000/usdhc@136000)
 *   - (/soc/peripheral@50000000/usdhc@137000)
 */
const device_handle_t __aligned(2) __attribute__((__section__(".__device_handles_pass2")))
__devicehdl_DT_N_S_soc_S_peripheral_50000000_S_clkctl_21000[] = { DEVICE_HANDLE_SEP, DEVICE_HANDLE_SEP, 3, DEVICE_HANDLE_ENDS };

/* 2 : /soc/peripheral@50000000/clkctl@1000:
 * Direct Dependencies:
 *   - (/soc/peripheral@50000000)
 */
const device_handle_t __aligned(2) __attribute__((__section__(".__device_handles_pass2")))
__devicehdl_DT_N_S_soc_S_peripheral_50000000_S_clkctl_1000[] = { DEVICE_HANDLE_SEP, DEVICE_HANDLE_SEP, DEVICE_HANDLE_ENDS };

/* 3 : /soc/peripheral@50000000/flexcomm@106000:
 * Direct Dependencies:
 *   - (/soc/interrupt-controller@e000e100)
 *   - (/soc/peripheral@50000000)
 *   - /soc/peripheral@50000000/clkctl@21000
 */
const device_handle_t __aligned(2) __attribute__((__section__(".__device_handles_pass2")))
__devicehdl_DT_N_S_soc_S_peripheral_50000000_S_flexcomm_106000[] = { 1, DEVICE_HANDLE_SEP, DEVICE_HANDLE_SEP, DEVICE_HANDLE_ENDS };

/* 4 : /soc/peripheral@50000000/gpio@2:
 * Direct Dependencies:
 *   - (/soc/peripheral@50000000)
 * Supported:
 *   - (/soc/peripheral@50000000/usdhc@136000)
 */
const device_handle_t __aligned(2) __attribute__((__section__(".__device_handles_pass2")))
__devicehdl_DT_N_S_soc_S_peripheral_50000000_S_gpio_2[] = { DEVICE_HANDLE_SEP, DEVICE_HANDLE_SEP, DEVICE_HANDLE_ENDS };

/* 5 : /soc/peripheral@50000000/gpio@1:
 * Direct Dependencies:
 *   - (/soc/interrupt-controller@e000e100)
 *   - (/soc/peripheral@50000000)
 * Supported:
 *   - (/gpio_keys/button_0)
 */
const device_handle_t __aligned(2) __attribute__((__section__(".__device_handles_pass2")))
__devicehdl_DT_N_S_soc_S_peripheral_50000000_S_gpio_1[] = { DEVICE_HANDLE_SEP, DEVICE_HANDLE_SEP, DEVICE_HANDLE_ENDS };

/* 6 : /soc/peripheral@50000000/gpio@0:
 * Direct Dependencies:
 *   - (/soc/interrupt-controller@e000e100)
 *   - (/soc/peripheral@50000000)
 * Supported:
 *   - (/gpio_keys/button_1)
 *   - (/leds/led_1)
 *   - (/leds/led_2)
 *   - (/leds/led_3)
 */
const device_handle_t __aligned(2) __attribute__((__section__(".__device_handles_pass2")))
__devicehdl_DT_N_S_soc_S_peripheral_50000000_S_gpio_0[] = { DEVICE_HANDLE_SEP, DEVICE_HANDLE_SEP, DEVICE_HANDLE_ENDS };
