/*
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>
#include <libfdt.h>
#include <platform_def.h>
#include <lib/xlat_tables/xlat_tables_v2.h>

#include "nua3500_private.h"

#define MAP_SEC_SYSRAM	MAP_REGION_FLAT(NUA3500_SRAM1_BASE, \
					NUA3500_SRAM1_SIZE, \
					MT_MEMORY | \
					MT_RW | \
					MT_SECURE )

#define MAP_DEVICE1	MAP_REGION_FLAT(NUA3500_REG_BASE, \
					NUA3500_REG_SIZE, \
					MT_DEVICE | \
					MT_RW | \
					MT_SECURE)

#define MAP_DEVICE2	MAP_REGION_FLAT(NUA3500_DRAM_BASE, \
					NUA3500_DRAM_SIZE, \
					MT_MEMORY | \
					MT_RW | \
					MT_NS)

#define MAP_DEVICE3	MAP_REGION_FLAT(NUA3500_DRAM_S_BASE, \
					NUA3500_DRAM_S_SIZE, \
					MT_MEMORY | \
					MT_RW | \
					MT_SECURE)

const mmap_region_t nua3500_mmap[] = {
	MAP_SEC_SYSRAM,
	MAP_DEVICE1,
	MAP_DEVICE2,
	MAP_DEVICE3,
	{0}
};


void configure_mmu(void)
{
	mmap_add(nua3500_mmap);
	init_xlat_tables();

	enable_mmu_el3(0);
}


