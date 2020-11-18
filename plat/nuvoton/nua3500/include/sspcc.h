/*
 * Copyright (c) 2020, Nuvoton Technology Corp. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __SSPCC_H__
#define __SSPCC_H__

#define PDMA0_TZS	0	16	0
#define PDMA0_TZNS	0	16	1
#define PDMA1_TZS	0	18	0
#define PDMA1_TZNS	0	18	1
#define PDMA2_TZNS	0	20	1
#define PDMA2_SUBM	0	20	3
#define PDMA3_TZNS	0	22	1
#define PDMA3_SUBM	0	22	3

#define EBI_TZNS	4	0	1
#define EBI_SUBM	4	0	3
#define SDH0_TZS	4	16	0
#define SDH0_TZNS	4	16	1
#define SDH1_TZS	4	18	0
#define SDH1_TZNS	4	18	1
#define NAND_TZS	4	20	0
#define NAND_TZNS	4	20	1

#define HSUSBD_TZS	8	0	0
#define HSUSBD_TZNS	8	0	1
#define MCAN0_TZNS	8	24	1
#define MCAN0_SUBM	8	24	3
#define MCAN1_TZNS	8	28	1
#define MCAN1_SUBM	8	28	3

#define CRYPTO_TZS	12	0	0
#define CRYPTO_TZNS	12	0	1
#define MCAN2_TZNS	12	24	1
#define MCAN2_SUBM	12	24	3
#define MCAN3_TZNS	12	28	1
#define MCAN3_SUBM	12	28	3

#define ADC0_TZNS	16	4	1
#define ADC0_SUBM	16	4	3
#define EADC_TZNS	16	6	1
#define EADC_SUBM	16	6	3
#define WDTWWDT1_TZS	16	8	0
#define WDTWWDT1_TZNS	16	8	1
#define I2S0_TZNS	16	16	1
#define I2S0_SUBM	16	16	3
#define I2S1_TZNS	16	18	1
#define I2S1_SUBM	16	18	3
#define KPI_TZNS	16	20	1
#define KPI_SUBM	16	20	3
#define DDRPHY_TZS	16	24	0
#define DDRPHY_TZNS	16	24	1
#define MCTL_TZS	16	26	0
#define MCTL_TZNS	16	26	1

#define TIMER01_TZS	20	0	0
#define TIMER01_TZNS	20	0	1
#define TIMER23_TZNS	20	2	1
#define TIMER23_SUBM	20	2	3
#define TIMER45_TZNS	20	4	1
#define TIMER45_SUBM	20	4	3
#define TIMER67_TZNS	20	6	1
#define TIMER67_SUBM	20	6	3
#define TIMER89_TZNS	20	8	1
#define TIMER89_SUBM	20	8	3
#define TIMER1011_TZNS	20	10	1
#define TIMER1011_SUBM	20	10	3
#define EPWM0_TZNS	20	16	1
#define EPWM0_SUBM	20	16	3
#define EPWM1_TZNS	20	18	1
#define EPWM1_SUBM	20	18	3
#define EPWM2_TZNS	20	20	1
#define EPWM3_SUBM	20	20	3

#define SPI0_TZNS	24	0	1
#define SPI0_SUBM	24	0	3
#define SPI1_TZNS	24	2	1
#define SPI1_SUBM	24	2	3
#define SPI2_TZNS	24	4	1
#define SPI2_SUBM	24	4	3
#define SPI3_TZNS	24	6	1
#define SPI3_SUBM	24	6	3
#define QSPI0_TZS	24	16	0
#define QSPI0_TZNS	24	16	1
#define QSPI1_TZNS	24	18	1
#define QSPI1_SUBM	24	18	3

#define UART0_TZS	28	0	0
#define UART0_TZNS	28	0	1
#define UART1_TZNS	28	2	1
#define UART1_SUBM	28	2	3
#define UART2_TZNS	28	4	1
#define UART2_SUBM	28	4	3
#define UART3_TZNS	28	6	1
#define UART3_SUBM	28	6	3
#define UART4_TZNS	28	8	1
#define UART4_SUBM	28	8	3
#define UART5_TZNS	28	10	1
#define UART5_SUBM	28	10	3
#define UART6_TZNS	28	12	1
#define UART6_SUBM	28	12	3
#define UART7_TZNS	28	14	1
#define UART7_SUBM	28	14	3
#define UART8_TZNS	28	16	1
#define UART8_SUBM	28	16	3
#define UART9_TZNS	28	18	1
#define UART9_SUBM	28	18	3
#define UART10_TZNS	28	20	1
#define UART10_SUBM	28	20	3
#define UART11_TZNS	28	22	1
#define UART11_SUBM	28	22	3
#define UART12_TZNS	28	24	1
#define UART12_SUBM	28	24	3
#define UART13_TZNS	28	26	1
#define UART13_SUBM	28	26	3
#define UART14_TZNS	28	28	1
#define UART14_SUBM	28	28	3
#define UART15_TZNS	28	30	1
#define UART15_SUBM	28	30	3

#define I2C0_TZS	32	0	0
#define I2C0_TZNS	32	0	1
#define I2C1_TZNS	32	2	1
#define I2C1_SUBM	32	2	3
#define I2C2_TZNS	32	4	1
#define I2C2_SUBM	32	4	3
#define I2C3_TZNS	32	6	1
#define I2C3_SUBM	32	6	3
#define I2C4_TZNS	32	8	1
#define I2C4_SUBM	32	8	3
#define I2C5_TZNS	32	10	1
#define I2C5_SUBM	32	10	3
#define UART16_TZNS	32	16	1
#define UART16_SUBM	32	16	3

#define SC0_TZNS	36	0	1
#define SC0_SUBM	36	0	3
#define SC1_TZNS	36	2	1
#define SC1_SUBM	36	2	3

#define QEI0_TZNS	44	0	1
#define QEI0_SUBM	44	0	3
#define QEI1_TZNS	44	2	1
#define QEI1_SUBM	44	2	3
#define QEI2_TZNS	44	4	1
#define QEI2_SUBM	44	4	3
#define ECAP0_TZNS	44	8	1
#define ECAP0_SUBM	44	8	3
#define ECAP1_TZNS	44	10	1
#define ECAP1_SUBM	44	10	3
#define ECAP2_TZNS	44	12	1
#define ECAP2_SUBM	44	12	3
#define TRNG_TZS	44	18	0
#define TRNG_TZNS	44	18	1

#endif // __SSPCC_H__
