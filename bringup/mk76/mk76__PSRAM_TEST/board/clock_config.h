/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef _CLOCK_CONFIG_H_
#define _CLOCK_CONFIG_H_

#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_SYSOSC_SETTLING_US                        260U  /*!< Board System oscillator settling time in us */
#define BOARD_XTAL32K_CLK_HZ                          32768U  /*!< Board xtal32K frequency in Hz */
#define BOARD_XTAL_SYS_CLK_HZ                      24000000U  /*!< Board xtal frequency in Hz */

/*******************************************************************************
 ************************ BOARD_InitBootClocks function ************************
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @brief This function executes default configuration of clocks.
 *
 */
void BOARD_InitBootClocks(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

/*******************************************************************************
 ********************** Configuration BOARD_BootClockRUN ***********************
 ******************************************************************************/
/*******************************************************************************
 * Definitions for BOARD_BootClockRUN configuration
 ******************************************************************************/
#define BOARD_BOOTCLOCKRUN_CORE_CLOCK             250105263U  /*!< Core clock frequency: 250105263Hz */


/* Clock outputs (values are in Hz): */
#define BOARD_BOOTCLOCKRUN_ACMP_CLOCK                 0UL            /* Clock consumers of ACMP_clock output : CMP */
#define BOARD_BOOTCLOCKRUN_ADC_CLOCK                  0UL            /* Clock consumers of ADC_clock output : LPADC */
#define BOARD_BOOTCLOCKRUN_CLKOUT_CLOCK               0UL            /* Clock consumers of CLKOUT_clock output : N/A */
#define BOARD_BOOTCLOCKRUN_CTIMER0_CLOCK              0UL            /* Clock consumers of CTIMER0_clock output : CTIMER0 */
#define BOARD_BOOTCLOCKRUN_CTIMER1_CLOCK              0UL            /* Clock consumers of CTIMER1_clock output : CTIMER1 */
#define BOARD_BOOTCLOCKRUN_CTIMER2_CLOCK              0UL            /* Clock consumers of CTIMER2_clock output : CTIMER2 */
#define BOARD_BOOTCLOCKRUN_CTIMER3_CLOCK              0UL            /* Clock consumers of CTIMER3_clock output : CTIMER3 */
#define BOARD_BOOTCLOCKRUN_CTIMER4_CLOCK              0UL            /* Clock consumers of CTIMER4_clock output : CTIMER4 */
#define BOARD_BOOTCLOCKRUN_DMIC_CLOCK                 0UL            /* Clock consumers of DMIC_clock output : DMIC0 */
#define BOARD_BOOTCLOCKRUN_DSPRAM_CLOCK               0UL            /* Clock consumers of DSPRAM_clock output : N/A */
#define BOARD_BOOTCLOCKRUN_DSP_CLOCK                  0UL            /* Clock consumers of DSP_clock output : N/A */
#define BOARD_BOOTCLOCKRUN_FLEXSPI_CLOCK              125052631UL    /* Clock consumers of FLEXSPI_clock output : FLEXSPI */
#define BOARD_BOOTCLOCKRUN_FXCOM0_CLOCK               0UL            /* Clock consumers of FXCOM0_clock output : FLEXCOMM0 */
#define BOARD_BOOTCLOCKRUN_FXCOM14_CLOCK              0UL            /* Clock consumers of FXCOM14_clock output : FLEXCOMM14 */
#define BOARD_BOOTCLOCKRUN_FXCOM15_CLOCK              0UL            /* Clock consumers of FXCOM15_clock output : FLEXCOMM15 */
#define BOARD_BOOTCLOCKRUN_FXCOM1_CLOCK               0UL            /* Clock consumers of FXCOM1_clock output : FLEXCOMM1 */
#define BOARD_BOOTCLOCKRUN_FXCOM2_CLOCK               0UL            /* Clock consumers of FXCOM2_clock output : FLEXCOMM2 */
#define BOARD_BOOTCLOCKRUN_FXCOM3_CLOCK               0UL            /* Clock consumers of FXCOM3_clock output : FLEXCOMM3 */
#define BOARD_BOOTCLOCKRUN_FXCOM4_CLOCK               0UL            /* Clock consumers of FXCOM4_clock output : FLEXCOMM4 */
#define BOARD_BOOTCLOCKRUN_FXCOM5_CLOCK               0UL            /* Clock consumers of FXCOM5_clock output : FLEXCOMM5 */
#define BOARD_BOOTCLOCKRUN_FXCOM6_CLOCK               0UL            /* Clock consumers of FXCOM6_clock output : FLEXCOMM6 */
#define BOARD_BOOTCLOCKRUN_FXCOM7_CLOCK               0UL            /* Clock consumers of FXCOM7_clock output : FLEXCOMM7 */
#define BOARD_BOOTCLOCKRUN_I3C_FCLK_CLOCK             0UL            /* Clock consumers of I3C_FCLK_clock output : I3C */
#define BOARD_BOOTCLOCKRUN_I3C_SLOW_TC_CLOCK          0UL            /* Clock consumers of I3C_SLOW_TC_clock output : I3C */
#define BOARD_BOOTCLOCKRUN_I3C_SLOW_CLOCK             0UL            /* Clock consumers of I3C_SLOW_clock output : I3C */
#define BOARD_BOOTCLOCKRUN_LPOSC1M_CLOCK              1000000UL      /* Clock consumers of LPOSC1M_clock output : UTICK0, WWDT0, WWDT1 */
#define BOARD_BOOTCLOCKRUN_MCLK_CLOCK                 0UL            /* Clock consumers of MCLK_clock output : N/A */
#define BOARD_BOOTCLOCKRUN_OSTIMER_CLOCK              1000000UL      /* Clock consumers of OSTIMER_clock output : OSTIMER0, OSTIMER1 */
#define BOARD_BOOTCLOCKRUN_RTC1HZ_CLOCK               0UL            /* Clock consumers of RTC1HZ_clock output : RTC */
#define BOARD_BOOTCLOCKRUN_RTC1KHZ_CLOCK              0UL            /* Clock consumers of RTC1KHZ_clock output : RTC */
#define BOARD_BOOTCLOCKRUN_RTC32K_CLOCK               0UL            /* Clock consumers of RTC32K_clock output : FLEXCOMM0, FLEXCOMM1, FLEXCOMM14, FLEXCOMM15, FLEXCOMM2, FLEXCOMM3, FLEXCOMM4, FLEXCOMM5, FLEXCOMM6, FLEXCOMM7 */
#define BOARD_BOOTCLOCKRUN_SCT_CLOCK                  0UL            /* Clock consumers of SCT_clock output : SCT0 */
#define BOARD_BOOTCLOCKRUN_SDIO0_CLOCK                0UL            /* Clock consumers of SDIO0_clock output : USDHC0 */
#define BOARD_BOOTCLOCKRUN_SDIO1_CLOCK                0UL            /* Clock consumers of SDIO1_clock output : USDHC1 */
#define BOARD_BOOTCLOCKRUN_SYSTICK_CLOCK              0UL            /* Clock consumers of SYSTICK_clock output : N/A */
#define BOARD_BOOTCLOCKRUN_SYSTEM_CLOCK               250105263UL    /* Clock consumers of System_clock output : LPADC, AHB_SECURE_CTRL, CACHE64, CACHE64_POLSEL, CASPER, CLKCTL0, CLKCTL1, CMP, CRC_ENGINE, CTIMER0, CTIMER1, CTIMER2, CTIMER3, CTIMER4, DMA0, DMA1, DMIC0, FLEXCOMM0, FLEXCOMM1, FLEXCOMM14, FLEXCOMM15, FLEXCOMM2, FLEXCOMM3, FLEXCOMM4, FLEXCOMM5, FLEXCOMM6, FLEXCOMM7, FLEXSPI, FREQME, GPIO, HASHCRYPT, I3C, INPUTMUX, IOPCTL, MRT0, MUA, MUB, OCOTP, OSTIMER0, OSTIMER1, OTFAD, PINT, POWERQUAD, PUF, RSTCTL0, RSTCTL1, RTC, SCT0, SECGPIO, SEMA42, SWD, SYSCTL0, SYSCTL1, TRNG, USBHSD, USBHSH, USBPHY, USDHC0, USDHC1, UTICK0, WWDT0, WWDT1 */
#define BOARD_BOOTCLOCKRUN_TRACE_CLOCK                250105263UL    /* Clock consumers of TRACE_clock output : SWD */
#define BOARD_BOOTCLOCKRUN_USBHS_CLOCK                0UL            /* Clock consumers of USBHS_clock output : USBHSD, USBHSH */
#define BOARD_BOOTCLOCKRUN_USBPHY_CLOCK               0UL            /* Clock consumers of USBPHY_clock output : USBHSD, USBHSH, USBPHY */
#define BOARD_BOOTCLOCKRUN_WAKE_32K_CLOCK             31250UL        /* Clock consumers of WAKE_32K_clock output : CMP */

/*******************************************************************************
 * API for BOARD_BootClockRUN configuration
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @brief This function executes configuration of clocks.
 *
 */
void BOARD_BootClockRUN(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _CLOCK_CONFIG_H_ */

