/*
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>

#include <common/debug.h>
#include <drivers/arm/cci.h>
#include <drivers/arm/ccn.h>
#include <drivers/arm/gicv2.h>
#include <drivers/arm/sp804_delay_timer.h>
#include <drivers/generic_delay_timer.h>
#include <lib/mmio.h>
#include <lib/xlat_tables/xlat_tables_compat.h>
#include <plat/arm/common/arm_config.h>
#include <plat/arm/common/plat_arm.h>
#include <plat/common/platform.h>
#include <platform_def.h>
#include <services/spm_mm_partition.h>

#include "nua3500_private.h"

/* Defines for GIC Driver build time selection */
#define NUA3500_GICV2		1
#define NUA3500_GICV3		2

/*******************************************************************************
 * arm_config holds the characteristics of the differences between the three NUA3500
 * platforms (Base, A53_A57 & Foundation). It will be populated during cold boot
 * at each boot stage by the primary before enabling the MMU (to allow
 * interconnect configuration) & used thereafter. Each BL will have its own copy
 * to allow independent operation.
 ******************************************************************************/
arm_config_t arm_config;

#if NUA3500_INTERCONNECT_DRIVER != NUA3500_CCN
#if 0
static const int nua3500_cci400_map[] = {
	PLAT_NUA3500_CCI400_CLUS0_SL_PORT,
	PLAT_NUA3500_CCI400_CLUS1_SL_PORT,
};

static const int nua3500_cci5xx_map[] = {
	PLAT_NUA3500_CCI5XX_CLUS0_SL_PORT,
	PLAT_NUA3500_CCI5XX_CLUS1_SL_PORT,
};
#endif

static unsigned int get_interconnect_master(void)
{
	unsigned int master;
	u_register_t mpidr;

	mpidr = read_mpidr_el1();
	master = ((arm_config.flags & ARM_CONFIG_FVP_SHIFTED_AFF) != 0U) ?
		MPIDR_AFFLVL2_VAL(mpidr) : MPIDR_AFFLVL1_VAL(mpidr);

	assert(master < NUA3500_CLUSTER_COUNT);
	return master;
}
#endif

int console_nua3500_register(uintptr_t baseaddr, uint32_t clock, uint32_t baud, console_t *console);
int console_nua3500_putc(int character, struct console *console);
int console_nua3500_getc(struct console *console);
int console_nua3500_flush(struct console *console);
static console_t nua3500_console;

static console_t nua3500_console = {
	.flags = CONSOLE_FLAG_BOOT | CONSOLE_FLAG_RUNTIME | CONSOLE_FLAG_CRASH | CONSOLE_FLAG_TRANSLATE_CRLF,
	.putc = console_nua3500_putc,
	.getc = console_nua3500_getc,
	.flush = console_nua3500_flush,
};


/*******************************************************************************
 * A single boot loader stack is expected to work on both the Foundation NUA3500
 * models and the two flavours of the Base NUA3500 models (AEMv8 & Cortex). The
 * SYS_ID register provides a mechanism for detecting the differences between
 * these platforms. This information is stored in a per-BL array to allow the
 * code to take the correct path.Per BL platform configuration.
 ******************************************************************************/
void __init nua3500_config_setup(void)
{

	/* Enable UART0 clock */
	outp32((void *)CLK_APBCLK0, inp32((void *)CLK_APBCLK0) | (1 << 12));
	outp32((void *)CLK_CLKSEL2, inp32((void *)CLK_CLKSEL2) & ~(3 << 16));
	/* UART0 multi-function */
	outp32((void *)SYS_GPE_MFPH, (inp32((void *)SYS_GPE_MFPH) & ~0xff000000) | 0x11000000);

	console_nua3500_register(PLAT_ARM_CRASH_UART_BASE, PLAT_ARM_CRASH_UART_CLK_IN_HZ, ARM_CONSOLE_BAUDRATE, &nua3500_console);

	INFO("nua3500 config setup\n");

}


void __init nua3500_interconnect_init(void)
{
}

void nua3500_interconnect_enable(void)
{
	unsigned int master;

	if ((arm_config.flags & (ARM_CONFIG_FVP_HAS_CCI400 |
				 ARM_CONFIG_FVP_HAS_CCI5XX)) != 0U) {
		master = get_interconnect_master();
		cci_enable_snoop_dvm_reqs(master);
	}
}

void nua3500_interconnect_disable(void)
{
	unsigned int master;

	if ((arm_config.flags & (ARM_CONFIG_FVP_HAS_CCI400 |
				 ARM_CONFIG_FVP_HAS_CCI5XX)) != 0U) {
		master = get_interconnect_master();
		cci_disable_snoop_dvm_reqs(master);
	}
}

#if TRUSTED_BOARD_BOOT
int plat_get_mbedtls_heap(void **heap_addr, size_t *heap_size)
{
	assert(heap_addr != NULL);
	assert(heap_size != NULL);

	return arm_get_mbedtls_heap(heap_addr, heap_size);
}
#endif

unsigned int plat_get_syscnt_freq2(void)
{
	return SYS_COUNTER_FREQ_IN_TICKS;
}

void nua3500_timer_init(void)
{
	generic_delay_timer_init();
}

void plat_nua3500_init(void)
{
	/* unlock */
	outp32((void *)SYS_RLKTZS, 0x59);
	outp32((void *)SYS_RLKTZS, 0x16);
	outp32((void *)SYS_RLKTZS, 0x88);

	/* enable SSPCC clock */
	outp32((void *)CLK_APBCLK2, inp32((void *)CLK_APBCLK2) | 0x8);

	/* set SSPCC: Assign NAND/SDH0/SDH1 to TZNS */
	outp32((void *)SSPCC_PSSET1, inp32((void *)SSPCC_PSSET1) | 0x150000);

	/* set SSPCC: Assign QSPI0 to TZNS */
	outp32((void *)SSPCC_PSSET6, inp32((void *)SSPCC_PSSET6) | 0x10000);

	/* set SSPCC: Assign UART0 to TZNS */
	outp32((void *)SSPCC_PSSET7, inp32((void *)SSPCC_PSSET7) | 0x1);

	/* lock */
	outp32((void *)SYS_RLKTZS, 0);
}


