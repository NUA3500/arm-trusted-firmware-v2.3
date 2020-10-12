/*
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef PLATFORM_DEF_H
#define PLATFORM_DEF_H

#include <drivers/arm/tzc400.h>
#include <lib/utils_def.h>
#include <plat/arm/board/common/v2m_def.h>
//#include <plat/arm/common/arm_def.h>
#include <plat/arm/common/smccc_def.h>
#include <plat/arm/common/arm_spm_def.h>
#include <plat/common/common_def.h>

#include "../nua3500_def.h"

/**************************************************/
/*
 * The max number of regions like RO(code), coherent and data required by
 * different BL stages which need to be mapped in the MMU.
 */
#define ARM_BL_REGIONS			5

#define MAX_MMAP_REGIONS		(PLAT_ARM_MMAP_ENTRIES +	\
					 ARM_BL_REGIONS)

/*******************************************************************************
 * System counter frequency related constants
 ******************************************************************************/
#define SYS_COUNTER_FREQ_IN_TICKS	12000000
#define SYS_COUNTER_FREQ_IN_MHZ		(SYS_COUNTER_FREQ_IN_TICKS/1000000)

#define ARM_CONSOLE_BAUDRATE		115200

/*
 * Secure Interrupt: based on the standard ARM mapping
 */
#define ARM_IRQ_SEC_PHY_TIMER		U(29)

#define ARM_IRQ_SEC_SGI_0		U(8)
#define ARM_IRQ_SEC_SGI_1		U(9)
#define ARM_IRQ_SEC_SGI_2		U(10)
#define ARM_IRQ_SEC_SGI_3		U(11)
#define ARM_IRQ_SEC_SGI_4		U(12)
#define ARM_IRQ_SEC_SGI_5		U(13)
#define ARM_IRQ_SEC_SGI_6		U(14)
#define ARM_IRQ_SEC_SGI_7		U(15)


/*
 * Define a list of Group 1 Secure and Group 0 interrupt properties as per GICv3
 * terminology. On a GICv2 system or mode, the lists will be merged and treated
 * as Group 0 interrupts.
 */
#define ARM_G1S_IRQ_PROPS(grp) \
	INTR_PROP_DESC(ARM_IRQ_SEC_PHY_TIMER, GIC_HIGHEST_SEC_PRIORITY, (grp), \
			GIC_INTR_CFG_LEVEL), \
	INTR_PROP_DESC(ARM_IRQ_SEC_SGI_1, GIC_HIGHEST_SEC_PRIORITY, (grp), \
			GIC_INTR_CFG_EDGE), \
	INTR_PROP_DESC(ARM_IRQ_SEC_SGI_2, GIC_HIGHEST_SEC_PRIORITY, (grp), \
			GIC_INTR_CFG_EDGE), \
	INTR_PROP_DESC(ARM_IRQ_SEC_SGI_3, GIC_HIGHEST_SEC_PRIORITY, (grp), \
			GIC_INTR_CFG_EDGE), \
	INTR_PROP_DESC(ARM_IRQ_SEC_SGI_4, GIC_HIGHEST_SEC_PRIORITY, (grp), \
			GIC_INTR_CFG_EDGE), \
	INTR_PROP_DESC(ARM_IRQ_SEC_SGI_5, GIC_HIGHEST_SEC_PRIORITY, (grp), \
			GIC_INTR_CFG_EDGE), \
	INTR_PROP_DESC(ARM_IRQ_SEC_SGI_7, GIC_HIGHEST_SEC_PRIORITY, (grp), \
			GIC_INTR_CFG_EDGE)

#define ARM_G0_IRQ_PROPS(grp) \
	INTR_PROP_DESC(ARM_IRQ_SEC_SGI_0, PLAT_SDEI_NORMAL_PRI, (grp), \
			GIC_INTR_CFG_EDGE), \
	INTR_PROP_DESC(ARM_IRQ_SEC_SGI_6, GIC_HIGHEST_SEC_PRIORITY, (grp), \
			GIC_INTR_CFG_EDGE)


/* Special value used to verify platform parameters from BL2 to BL31 */
#define ARM_BL31_PLAT_PARAM_VAL		ULL(0x0f1e2d3c4b5a6978)

#define ARM_CACHE_WRITEBACK_SHIFT	6

/*
 * Macros mapping the MPIDR Affinity levels to ARM Platform Power levels. The
 * power levels have a 1:1 mapping with the MPIDR affinity levels.
 */
#define ARM_PWR_LVL0		MPIDR_AFFLVL0
#define ARM_PWR_LVL1		MPIDR_AFFLVL1
#define ARM_PWR_LVL2		MPIDR_AFFLVL2
#define ARM_PWR_LVL3		MPIDR_AFFLVL3

/*
 *  Macros for local power states in ARM platforms encoded by State-ID field
 *  within the power-state parameter.
 */
/* Local power state for power domains in Run state. */
#define ARM_LOCAL_STATE_RUN	U(0)
/* Local power state for retention. Valid only for CPU power domains */
#define ARM_LOCAL_STATE_RET	U(1)
/* Local power state for OFF/power-down. Valid for CPU and cluster power
   domains */
#define ARM_LOCAL_STATE_OFF	U(2)

#define ARM_DRAM1_BASE			ULL(0x80000000)
#define ARM_DRAM1_SIZE			ULL(0x01000000)
#define ARM_DRAM1_END			(ARM_DRAM1_BASE + ARM_DRAM1_SIZE - 1)

/* The first 4KB of Trusted SRAM are used as shared memory */
#define ARM_TRUSTED_SRAM_BASE		UL(0x28000000)
#define ARM_SHARED_RAM_BASE		ARM_TRUSTED_SRAM_BASE
#define ARM_SHARED_RAM_SIZE		UL(0x00001000)	/* 4 KB */

/* The remaining Trusted SRAM is used to load the BL images */
#define ARM_BL_RAM_BASE			(ARM_SHARED_RAM_BASE +	\
					 ARM_SHARED_RAM_SIZE)
#define ARM_BL_RAM_SIZE			(PLAT_ARM_TRUSTED_SRAM_SIZE -	\
					 ARM_SHARED_RAM_SIZE)

/*
 * Some data must be aligned on the biggest cache line size in the platform.
 * This is known only to the platform as it might have a combination of
 * integrated and external caches.
 */
#define CACHE_WRITEBACK_GRANULE		(U(1) << ARM_CACHE_WRITEBACK_SHIFT)

/*
 * To enable FW_CONFIG to be loaded by BL1, define the corresponding base
 * and limit. Leave enough space of BL2 meminfo.
 */
#define ARM_FW_CONFIG_BASE		(ARM_BL_RAM_BASE + sizeof(meminfo_t))
#define ARM_FW_CONFIG_LIMIT		(ARM_BL_RAM_BASE + PAGE_SIZE)

/*
 * Boot parameters passed from BL2 to BL31/BL32 are stored here
 */
#define ARM_BL2_MEM_DESC_BASE		ARM_FW_CONFIG_LIMIT
#define ARM_BL2_MEM_DESC_LIMIT		(ARM_BL2_MEM_DESC_BASE + (PAGE_SIZE / 2U))


/*
 * If SEPARATE_CODE_AND_RODATA=1 we define a region for each section
 * otherwise one region is defined containing both.
 */
#if SEPARATE_CODE_AND_RODATA
#define ARM_MAP_BL_RO			MAP_REGION_FLAT(			\
						BL_CODE_BASE,			\
						BL_CODE_END - BL_CODE_BASE,	\
						MT_CODE | MT_SECURE),		\
					MAP_REGION_FLAT(			\
						BL_RO_DATA_BASE,		\
						BL_RO_DATA_END			\
							- BL_RO_DATA_BASE,	\
						MT_RO_DATA | MT_SECURE)
#else
#define ARM_MAP_BL_RO			MAP_REGION_FLAT(			\
						BL_CODE_BASE,			\
						BL_CODE_END - BL_CODE_BASE,	\
						MT_CODE | MT_SECURE)
#endif

/* Priority levels for ARM platforms */
#define PLAT_RAS_PRI			0x10
#define PLAT_SDEI_CRITICAL_PRI		0x60
#define PLAT_SDEI_NORMAL_PRI		0x70


/**************************************************/

/* Required platform porting definitions */
#define PLATFORM_MAX_CPUS_PER_CLUSTER	U(2)
#define PLATFORM_CLUSTER_COUNT		U(1)
#define PLATFORM_CLUSTER0_CORE_COUNT	PLATFORM_MAX_CPUS_PER_CLUSTER

#define PLATFORM_CORE_COUNT		PLATFORM_CLUSTER0_CORE_COUNT

#define PLAT_NUM_PWR_DOMAINS		(PLATFORM_CLUSTER_COUNT + \
					PLATFORM_CORE_COUNT)
#define PLAT_MAX_PWR_LVL		MPIDR_AFFLVL1
#define PLAT_MAX_RET_STATE		1
#define PLAT_MAX_OFF_STATE		2

/*
 * Other platform porting definitions are provided by included headers
 */

/*
 * Required ARM standard platform porting definitions
 */
#define PLAT_ARM_CLUSTER_COUNT		NUA3500_CLUSTER_COUNT

#define PLAT_ARM_TRUSTED_SRAM_SIZE	UL(0x00040000)	/* 256 KB */

#define PLAT_ARM_TRUSTED_ROM_BASE	UL(0x00000000)
#define PLAT_ARM_TRUSTED_ROM_SIZE	UL(0x00020000)	/* 128 KB */

#define PLAT_ARM_TRUSTED_DRAM_BASE	UL(0x81000000)
#define PLAT_ARM_TRUSTED_DRAM_SIZE	UL(0x00400000)	/* 1 GB */

#define PLAT_ARM_DRAM2_BASE		ULL(0x80000000)
#define PLAT_ARM_DRAM2_SIZE		UL(0x01000000)

/*
 * Load address of BL33 for this platform port
 */
#define PLAT_ARM_NS_IMAGE_BASE		UL(0x80000000)	/* TBD */


/*
 * PLAT_ARM_MMAP_ENTRIES depends on the number of entries in the
 * plat_arm_mmap array defined for each BL stage.
 */
#if defined(IMAGE_BL31)
# if ENABLE_SPM
#  define PLAT_ARM_MMAP_ENTRIES		9
#  define MAX_XLAT_TABLES		9
#  define PLAT_SP_IMAGE_MMAP_REGIONS	30
#  define PLAT_SP_IMAGE_MAX_XLAT_TABLES	10
# else
#  define PLAT_ARM_MMAP_ENTRIES		8
#  define MAX_XLAT_TABLES		5
# endif
#else
# define PLAT_ARM_MMAP_ENTRIES		12
# define MAX_XLAT_TABLES		6
#endif

/*
 * Since BL31 NOBITS overlays BL2 and BL1-RW, PLAT_ARM_MAX_BL31_SIZE is
 * calculated using the current BL31 PROGBITS debug size plus the sizes of
 * BL2 and BL1-RW
 */
#if ENABLE_SPM && !SPM_MM
#define PLAT_ARM_MAX_BL31_SIZE		UL(0x60000)
#else
#define PLAT_ARM_MAX_BL31_SIZE		UL(0x3B000)
#endif

#ifndef __aarch64__
/*
 * Since BL32 NOBITS overlays BL2 and BL1-RW, PLAT_ARM_MAX_BL32_SIZE is
 * calculated using the current SP_MIN PROGBITS debug size plus the sizes of
 * BL2 and BL1-RW
 */
# define PLAT_ARM_MAX_BL32_SIZE		UL(0x3B000)
#endif

/*
 * Size of cacheable stacks
 */
#if defined(IMAGE_BL2)
# if TRUSTED_BOARD_BOOT
#  define PLATFORM_STACK_SIZE		UL(0x1000)
# else
#  define PLATFORM_STACK_SIZE		UL(0x400)
# endif
#elif defined(IMAGE_BL31)
#  define PLATFORM_STACK_SIZE		UL(0x800)
#elif defined(IMAGE_BL32)
# define PLATFORM_STACK_SIZE		UL(0x440)
#endif


/* Reserve the last block of flash for PSCI MEM PROTECT flag */
//#define PLAT_ARM_FIP_BASE		UL(0x0)	/* TBD */
//#define PLAT_ARM_FIP_MAX_SIZE		UL(0x0)	/* TBD */


#define PLAT_ARM_CRASH_UART_BASE	UL(0x40700000)
#define PLAT_ARM_CRASH_UART_CLK_IN_HZ	UL(24000000)

/* System timer related constants */
#define PLAT_ARM_NSTIMER_FRAME_ID		U(1)

/* Mailbox base address */
#define PLAT_ARM_TRUSTED_MAILBOX_BASE	ARM_TRUSTED_SRAM_BASE


/* TrustZone controller related constants
 */
#define PLAT_ARM_TZC_BASE		UL(0x404E1000)
#define PLAT_ARM_TZC_FILTERS		TZC_400_REGION_ATTR_FILTER_BIT(0)

#define PLAT_ARM_TZC_NS_DEV_ACCESS	(				\
		TZC_REGION_ACCESS_RDWR(NUA3500_NSAID_TZNS)	|	\
		TZC_REGION_ACCESS_RDWR(NUA3500_NSAID_SUBM))

/*
 * GIC related constants to cater for both GICv2 and GICv3 instances of an
 * NUA3500. They could be overriden at runtime in case the NUA3500 implements the legacy
 * VE memory map.
 */
#define PLAT_ARM_GICD_BASE		BASE_GICD_BASE
#define PLAT_ARM_GICR_BASE		BASE_GICR_BASE
#define PLAT_ARM_GICC_BASE		BASE_GICC_BASE

/*
 * Define a list of Group 1 Secure and Group 0 interrupts as per GICv3
 * terminology. On a GICv2 system or mode, the lists will be merged and treated
 * as Group 0 interrupts.
 */
#define PLAT_ARM_G1S_IRQ_PROPS(grp) \
	ARM_G1S_IRQ_PROPS(grp), \
	INTR_PROP_DESC(NUA3500_IRQ_TZ_WDOG, GIC_HIGHEST_SEC_PRIORITY, (grp), \
			GIC_INTR_CFG_LEVEL), \
	INTR_PROP_DESC(NUA3500_IRQ_SEC_SYS_TIMER, GIC_HIGHEST_SEC_PRIORITY, (grp), \
			GIC_INTR_CFG_LEVEL)

#define PLAT_ARM_G0_IRQ_PROPS(grp)	ARM_G0_IRQ_PROPS(grp)

#define PLAT_ARM_PRIVATE_SDEI_EVENTS	ARM_SDEI_PRIVATE_EVENTS
#define PLAT_ARM_SHARED_SDEI_EVENTS	ARM_SDEI_SHARED_EVENTS

#define PLAT_ARM_SP_IMAGE_STACK_BASE	(PLAT_SP_IMAGE_NS_BUF_BASE +	\
					 PLAT_SP_IMAGE_NS_BUF_SIZE)

#define PLAT_SP_PRI			PLAT_RAS_PRI

/*
 * Physical and virtual address space limits for MMU in AARCH64 & AARCH32 modes
 */
#ifdef __aarch64__
#define PLAT_PHY_ADDR_SPACE_SIZE	(1ULL << 36)
#define PLAT_VIRT_ADDR_SPACE_SIZE	(1ULL << 36)
#else
#define PLAT_PHY_ADDR_SPACE_SIZE	(1ULL << 32)
#define PLAT_VIRT_ADDR_SPACE_SIZE	(1ULL << 32)
#endif

#endif /* PLATFORM_DEF_H */
