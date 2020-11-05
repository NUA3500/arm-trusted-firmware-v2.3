/*
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef NUA3500_PRIVATE_H
#define NUA3500_PRIVATE_H

#include <plat/arm/common/plat_arm.h>

/*****************************************************************************
 * Power On Setting
 *****************************************************************************/
/* PWRON[11:10] */
#define PWRON_BOOT_MSK      (0x00000C00)
#define PWRON_BOOT_USB      (0x00000C00)
#define PWRON_BOOT_SD       (0x00000400)
#define PWRON_BOOT_NAND     (0x00000800)
#define PWRON_BOOT_SPI      (0x00000000)

/* PWRON[13:12] */
#define PWRON_NPAGE_MSK     (0x00003000)
#define PWRON_NPAGE_2K      (0x00000000)
#define PWRON_NPAGE_4K      (0x00001000)
#define PWRON_NPAGE_8K      (0x00002000)

/* PWRON[15:14] */
#define PWRON_MISCCFG_MSK   (0x0000C000)
/* NAND boot for BCH */
#define PWRON_BCH_NONE      (0x00000000)
#define PWRON_BCH_T12       (0x00004000)
#define PWRON_BCH_T24       (0x00008000)
/* SD boot for MFP */
#define PWRON_SD0           (0x00000000)
#define PWRON_SD1           (0x00004000)
#define PWRON_SD_4BIT       (0x00000000)
#define PWRON_SD_8BIT       (0x00008000)
/* SPI boot for mode select */
#define PWRON_SPI_4_NAND    (0x00004000)
#define PWRON_SPI_1_NAND    (0x00000000)
#define PWRON_SPI_4_NOR     (0x0000C000)
#define PWRON_SPI_1_NOR     (0x00008000)
#define PWRON_SPI_NOR       (0x00008000)

/* PWRON[5] */
#define PWRON_DEBUG_MSK     (0x00000010)
/* PWRON[2] */
#define PWRON_WDT_MSK       (0x00000004)


/*******************************************************************************
 * Function and variable prototypes
 ******************************************************************************/

void nua3500_config_setup(void);
void plat_nua3500_init(void);

void nua3500_interconnect_init(void);
void nua3500_interconnect_enable(void);
void nua3500_interconnect_disable(void);
void nua3500_timer_init(void);
void tsp_early_platform_setup(void);
void nua3500_io_setup(void);

void configure_mmu(void);
void nua3500_ddr_init(void);
void nua3500_arch_security_setup(void);

#endif /* NUA3500_PRIVATE_H */
