/*
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef NUA3500_DEF_H
#define NUA3500_DEF_H

#include <lib/utils_def.h>

#ifndef NUA3500_CLUSTER_COUNT
#define NUA3500_CLUSTER_COUNT		1
#endif

#ifndef NUA3500_MAX_CPUS_PER_CLUSTER
#define NUA3500_MAX_CPUS_PER_CLUSTER	2
#endif

#ifndef NUA3500_MAX_PE_PER_CPU
#define NUA3500_MAX_PE_PER_CPU		1
#endif

#define NUA3500_PRIMARY_CPU		0x0

/* Defines for the Interconnect build selection */
#define NUA3500_CCI			1
#define NUA3500_CCN			2

#define MAX_IO_DEVICES			U(4)
#define MAX_IO_HANDLES			U(4)
#define MAX_IO_BLOCK_DEVICES		U(1)
#define MAX_IO_MTD_DEVICES		U(1)

/* Define maximum page size for NAND devices */
#define PLATFORM_MTD_MAX_PAGE_SIZE	U(0x2000)

/*******************************************************************************
 * NUA3500 memory map related constants
 ******************************************************************************/
//#define NUA3500_ROM_BASE		U(0x5FFC0000)
//#define NUA3500_ROM_SIZE		U(0x00020000)

#define NUA3500_SRAM0_BASE		U(0x24000000)
#define NUA3500_SRAM0_SIZE		U(0x00040000)

#define NUA3500_SRAM1_BASE		U(0x28000000)
#define NUA3500_SRAM1_SIZE		U(0x00040000)

/* DDR configuration */
#define NUA3500_DDR_BASE		U(0x80000000)
#define NUA3500_DDR_MAX_SIZE		U(0x10000000)	/* 256MB */

#define NUA3500_BL2_BASE		U(0x28000000)
#define NUA3500_BL2_SIZE		U(0x00010000)

#define NUA3500_BL31_BASE		U(0x28010000)
#define NUA3500_BL31_SIZE		U(0x00020000)

#define NUA3500_BL32_BASE		U(0x8f800000)
#define NUA3500_BL32_SIZE		U(0x00200000)

#define NUA3500_BL33_BASE		U(0x80e00000)
#define NUA3500_BL33_SIZE		U(0x01000000)

#define BL2_BASE			NUA3500_BL2_BASE
#define BL2_LIMIT			(NUA3500_BL2_BASE + NUA3500_BL2_SIZE)

#define DTB_BASE			NUA3500_DTB_BASE
#define DTB_LIMIT			(NUA3500_DTB_BASE + NUA3500_DTB_SIZE)

/*
 * Put BL31 at the bottom of TZC secured DRAM
 */
#define BL31_BASE			NUA3500_BL31_BASE
#define BL31_LIMIT			(NUA3500_BL31_BASE + NUA3500_BL31_SIZE)

#define BL32_BASE			NUA3500_BL32_BASE
#define BL32_LIMIT			(NUA3500_BL32_BASE + NUA3500_BL32_SIZE)

#define BL33_BASE			NUA3500_BL33_BASE
#define BL33_LIMIT			(NUA3500_BL33_BASE + NUA3500_BL33_SIZE)


/*******************************************************************************
 * nua3500 device/io map related constants (used for MMU)
 ******************************************************************************/
#define NUA3500_REG_BASE		U(0x40400000)
#define NUA3500_REG_SIZE		U(0x20000000)

#define NUA3500_DRAM_BASE		U(0x80000000)
#define NUA3500_DRAM_SIZE		U(0x0f800000)

#define NUA3500_DRAM_S_BASE		U(0x8f800000)
#define NUA3500_DRAM_S_SIZE		U(0x00800000)

/* Non-volatile counters */
#define TRUSTED_NVCTR_BASE		UL(0x7fe70000)
#define TFW_NVCTR_BASE			(TRUSTED_NVCTR_BASE + UL(0x0000))
#define TFW_NVCTR_SIZE			UL(4)
#define NTFW_CTR_BASE			(TRUSTED_NVCTR_BASE + UL(0x0004))
#define NTFW_CTR_SIZE			UL(4)

/* Keys */
#define SOC_KEYS_BASE			UL(0x7fe80000)
#define TZ_PUB_KEY_HASH_BASE		(SOC_KEYS_BASE + UL(0x0000))
#define TZ_PUB_KEY_HASH_SIZE		UL(32)
#define HU_KEY_BASE			(SOC_KEYS_BASE + UL(0x0020))
#define HU_KEY_SIZE			UL(16)
#define END_KEY_BASE			(SOC_KEYS_BASE + UL(0x0044))
#define END_KEY_SIZE			UL(32)

/* Constants to distinguish NUA3500 type */
#define HBI_BASE_NUA3500			U(0x020)
#define REV_BASE_NUA3500_V0			U(0x0)
#define REV_BASE_NUA3500_REVC		U(0x2)

#define HBI_FOUNDATION_NUA3500		U(0x010)
#define REV_FOUNDATION_NUA3500_V2_0		U(0x0)
#define REV_FOUNDATION_NUA3500_V2_1		U(0x1)
#define REV_FOUNDATION_NUA3500_v9_1		U(0x2)
#define REV_FOUNDATION_NUA3500_v9_6		U(0x3)

#define ARCH_MODEL			U(0x1)

/* NUA3500 Power controller base address*/
#define PWRC_BASE			UL(0x1c100000)

/*******************************************************************************
 * GIC-400 & interrupt handling related constants
 ******************************************************************************/
/* Base NUA3500 compatible GIC memory map */
#define BASE_GICD_BASE			UL(0x50801000)
#define BASE_GICR_BASE			0       /* no GICR in GIC-400 */
#define BASE_GICC_BASE			UL(0x50802000)
#define BASE_GICH_BASE			UL(0x50804000)
#define BASE_GICV_BASE			UL(0x50806000)

#define NUA3500_IRQ_TZ_WDOG			39	/* wdt0 */
#define NUA3500_IRQ_SEC_SYS_TIMER		79	/* tmr0? */


/*******************************************************************************
 * NUA3500 TZC (TZ400)
 ******************************************************************************/
#define NUA3500_TZC0_BASE		U(0x404E1000)
#define NUA3500_TZC1_BASE		U(0x404E2000)
#define NUA3500_TZC2_BASE		U(0x404E3000)

/* NSAIDs used by devices in TZC filter 0 on NUA3500 */
#define NUA3500_NSAID_TZNS		U(0)
#define NUA3500_NSAID_SUBM		U(1)

#define NUA3500_FILTER_BIT_ALL		U(3)

/*******************************************************************************
 * Memprotect definitions
 ******************************************************************************/
/* PSCI memory protect definitions:
 * This variable is stored in a non-secure flash because some ARM reference
 * platforms do not have secure NVRAM. Real systems that provided MEM_PROTECT
 * support must use a secure NVRAM to store the PSCI MEM_PROTECT definitions.
 */
/*#define PLAT_ARM_MEM_PROT_ADDR		(V2M_FLASH0_BASE + \
					 V2M_FLASH0_SIZE - V2M_FLASH_BLOCK_SIZE)
*/


/*************************************/
#define UMCTL2_BA       U(0x404d0000)
#define SYS_BA          U(0x40460000)
#define CLK_BA          0x40460200

/* sys registers */
#define SYS_PWRONOTP	U(0x40460004)
#define SYS_PWRONPIN	U(0x40460008)
#define SYS_CHIPCFG	U(0x404601F4)

#define SYS_GPE_MFPH	U(0x404600A4)

#define SYS_RLKTZS	U(0x404601A0)
#define SYS_RLKTZNS	U(0x404601A4)

/* clock registers */
#define CLK_APBCLK0	U(0x4046020C)
#define CLK_APBCLK1	U(0x40460210)
#define CLK_APBCLK2	U(0x40460214)
#define CLK_CLKSEL0	U(0x40460218)
#define CLK_CLKSEL1	U(0x4046021C)
#define CLK_CLKSEL2	U(0x40460220)

/* sspcc registers */
#define SSPCC_PSSET1	U(0x404F0004)
#define SSPCC_PSSET6	U(0x404F0018)
#define SSPCC_PSSET7	U(0x404F001C)

#define outp32(addr,data) (*(volatile int*)(addr) = (data))
#define inp32(addr) (*(volatile int*)(addr))

#endif /* NUA3500_DEF_H */
