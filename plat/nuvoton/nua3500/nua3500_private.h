/*
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef NUA3500_PRIVATE_H
#define NUA3500_PRIVATE_H

#include <plat/arm/common/plat_arm.h>

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

int nua3500_nand_init(void);

#endif /* NUA3500_PRIVATE_H */
