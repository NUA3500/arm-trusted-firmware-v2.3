/*
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>
#include <errno.h>

#include <arch_helpers.h>
#include <common/debug.h>
#include <lib/extensions/spe.h>
#include <lib/mmio.h>
#include <lib/psci/psci.h>
#include <plat/arm/common/arm_config.h>
#include <plat/arm/common/plat_arm.h>
#include <plat/common/platform.h>
#include <platform_def.h>
#include <drivers/arm/gicv2.h>
#include <platform_def.h>

#include <common/interrupt_props.h>
#include <lib/utils.h>
#include "ma35d1_private.h"

/* Macros to read the rk power domain state */
#define MA35D1_CORE_PWR_STATE(state) \
    ((state)->pwr_domain_state[MPIDR_AFFLVL0])
#define MA35D1_CLUSTER_PWR_STATE(state) \
    ((state)->pwr_domain_state[MPIDR_AFFLVL1])
#define MA35D1_SYSTEM_PWR_STATE(state) \
((state)->pwr_domain_state[PLAT_MAX_PWR_LVL])

static uintptr_t ma35d1_sec_entrypoint;

#define SYS_BASE 0x40460000
#define PMUCR 0x30
#define DDRCQCSR 0x34
#define PMUIEN 0x38
#define PMUSTS 0x3C
#define MISCFCR 0x70
#define CA35WRBADR1 0x40
#define CA35WRBPAR1 0x44
#define CA35WRBADR2 0x48
#define CA35WRBPAR2 0x4C

#define CLK_BASE 0x40460200
#define PWRCTL 0x0
#define STATUS 0x50

static __inline void ma35d1_UnlockReg(void)
{
	do {
		mmio_write_32(0x404601A0, 0x59UL);
		mmio_write_32(0x404601A0, 0x16UL);
		mmio_write_32(0x404601A0, 0x88UL);
	} while (mmio_read_32(0x404601A0) == 0UL);

}


static __inline void ma35d1_LockReg(void)
{
	mmio_write_32(0x404601A0, 0);
}

// Q ch
void ma35d1_ddr_hw_pd(void)
{
	int DDR_QCH_BPPORT0;
	int DDR_QCH_BPPORT1;
	int DDR_QCH_BPPORT2;
	int DDR_QCH_BPPORT3;
	int DDR_QCH_BPPORT4;
	int DDR_QCH_BPPORT5;
	int DDR_QCH_BPPORT6;
	int DDR_QCH_BPPORT7;

	//disable DDR CG bypass
	mmio_write_32(SYS_BASE + MISCFCR,
					mmio_read_32(SYS_BASE + MISCFCR) & ~(1 << 23));

	//[11:8]= pg chain time period for voltage stable
	mmio_write_32(SYS_BASE + PMUCR,
					mmio_read_32(SYS_BASE + PMUCR) & 0xfffff0ff);

	//[15:12]= pg chain timeout
	mmio_write_32(SYS_BASE + PMUCR,
				(mmio_read_32(SYS_BASE + PMUCR) & 0xfffff0ff) | (0x2 << 12));

	//[31:24]=DDR time out & delay
	mmio_write_32(SYS_BASE + DDRCQCSR,
					mmio_read_32(SYS_BASE + DDRCQCSR) & 0x00ffffff);

	//[16]=DDRCQBYPAS, disable ddrc qch bypass
	mmio_write_32(SYS_BASE + DDRCQCSR,
					mmio_read_32(SYS_BASE + DDRCQCSR) & ~(0x1 << 16));

//------------------------------------------------------------------------
	//[7:0]=AXIQBYPAS,
	DDR_QCH_BPPORT0 = 1;
	DDR_QCH_BPPORT1 = 1;
	DDR_QCH_BPPORT2 = 1;
	DDR_QCH_BPPORT3 = 1;
	DDR_QCH_BPPORT4 = 1;
	DDR_QCH_BPPORT5 = 1;
	DDR_QCH_BPPORT6 = 1;
	DDR_QCH_BPPORT7 = 1;

	if ((mmio_read_32((0x404d0000 + 0x490)) & 0x1) & 
		((mmio_read_32((0x40460000 + 0x204)) >> 4) & 0x1))
		DDR_QCH_BPPORT0 = 0;	//A35
	if ((mmio_read_32(0x404d0000 + 0x540) & 0x1) &
		((mmio_read_32((0x40460000 + 0x204)) >> 24) & 0x1))
		DDR_QCH_BPPORT1 = 0;	//GFX
	if ((mmio_read_32((0x404d0000 + 0x5f0)) & 0x1) &
		((mmio_read_32((0x40460000 + 0x204)) >> 26) & 0x1))
		DDR_QCH_BPPORT2 = 0;	//DC
	if ((mmio_read_32((0x404d0000 + 0x6a0)) & 0x1) &
		((mmio_read_32((0x40460000 + 0x204)) >> 25) & 0x1))
		DDR_QCH_BPPORT3 = 0;	//VC8000
	if ((mmio_read_32((0x404d0000 + 0x750)) & 0x1) &
		(((mmio_read_32((0x40460000 + 0x204)) >> 27) |
		(mmio_read_32((0x40460000 + 0x200)) >> 28)) & 0x1))
		DDR_QCH_BPPORT4 = 0;	//GMAC
	if ((mmio_read_32((0x404d0000 + 0x800)) & 0x1) &
		(((mmio_read_32((0x40460000 + 0x204)) >> 29) |
		(mmio_read_32((0x40460000 + 0x200)) >> 30)) & 0x1))
		DDR_QCH_BPPORT5 = 0;	//CCAP
	if ((mmio_read_32((0x404d0000 + 0x8b0)) & 0x1) &
		((mmio_read_32((0x40460000 + 0x204)) >> 5) & 0x1))
		DDR_QCH_BPPORT6 = 0;	//system

	mmio_write_32(SYS_BASE + DDRCQCSR,
			(mmio_read_32(SYS_BASE + DDRCQCSR) & ~0xFF) |
			(DDR_QCH_BPPORT0 << 0) |
			(DDR_QCH_BPPORT1 << 1) |
			(DDR_QCH_BPPORT2 << 2) |
			(DDR_QCH_BPPORT3 << 3) |
			(DDR_QCH_BPPORT4 << 4) |
			(DDR_QCH_BPPORT5 << 5) |
			(DDR_QCH_BPPORT6 << 6) |
			(DDR_QCH_BPPORT7 << 7));//disable ddr 8 ports qch bypass

	//L2 auto-flush
	mmio_write_32(SYS_BASE + PMUCR,
			mmio_read_32(SYS_BASE + PMUCR) & ~(1 << 4));
}

void ma35d1_deep_power_down(void)
{
	ma35d1_UnlockReg();

	ma35d1_ddr_hw_pd();

	//[0]=pg_eanble, Enable clock gating
	mmio_write_32(SYS_BASE + PMUCR,
			mmio_read_32(SYS_BASE + PMUCR) | (1 << 0));

	//[16]=pd_eanble, Enable PD
	mmio_write_32(SYS_BASE + PMUCR,
			mmio_read_32(SYS_BASE + PMUCR) | (1 << 16));

	//[16]=PMUIEN, Enable PD
	mmio_write_32(SYS_BASE + PMUIEN,
			mmio_read_32(SYS_BASE + PMUIEN) | (1 << 0) | (1 << 8));

	ma35d1_LockReg();
}

void ma35d1_normal_power_down(void)
{
	ma35d1_UnlockReg();

	mmio_write_32(CLK_BASE + PWRCTL,
			mmio_read_32(CLK_BASE + PWRCTL) | (1 << 21));

	ma35d1_ddr_hw_pd();

	// Disable L2 flush by PMU
	mmio_write_32(SYS_BASE + PMUCR,
			mmio_read_32(SYS_BASE + PMUCR) | (1 << 4));

	//[0]=pg_eanble, Disable clock gating
	mmio_write_32(SYS_BASE + PMUCR,
			mmio_read_32(SYS_BASE + PMUCR) & ~(1 << 0));

	//[16]=pd_eanble, Enable Power down
	mmio_write_32(SYS_BASE + PMUCR,
			mmio_read_32(SYS_BASE + PMUCR) | (1 << 16));

	//[16]=PMUIEN,
	mmio_write_32(SYS_BASE + PMUIEN,
			mmio_read_32(SYS_BASE + PMUIEN) | (1 << 0) | (1 << 8));
	ma35d1_LockReg();
}


static void ma35d1_cpu_standby(plat_local_state_t cpu_state)
{

	u_register_t scr;

	assert(cpu_state == PLAT_MAX_RET_STATE);

	scr = read_scr_el3();
	/* Enable PhysicalIRQ bit for NS world to wake the CPU */
	write_scr_el3(scr | SCR_IRQ_BIT);
	isb();
	dsb();
	wfi();

	/*
	 * Restore SCR to the original value, synchronisation of scr_el3 is
	 * done by eret while el3_exit to save some execution cycles.
	 */
	write_scr_el3(scr);
}

static int ma35d1_pwr_domain_on(u_register_t mpidr)
{
	uint32_t loop;

	for (loop = 0; loop < 100; loop++)
	{
		mmio_write_32(SYS_BASE + CA35WRBADR2, ma35d1_sec_entrypoint);
		sev();
	}

	return PSCI_E_SUCCESS;
}

static void ma35d1_pwr_domain_off(const psci_power_state_t *target_state)
{

}

static void ma35d1_pwr_domain_suspend(const psci_power_state_t *target_state)
{
	unsigned int reg;

	if (MA35D1_CORE_PWR_STATE(target_state) != PLAT_MAX_OFF_STATE)
		return;

	disable_mmu_el3();

	if(mmio_read_32(SYS_BASE+DDRCQCSR)&0x0002FF00) {
		reg =mmio_read_32(SYS_BASE + DDRCQCSR);
		mmio_write_32(SYS_BASE + DDRCQCSR,reg);
	}

	if (MA35D1_SYSTEM_PWR_STATE(target_state) == PLAT_MAX_OFF_STATE) {
		mmio_write_32(0x2803fd04, 0);
		mmio_write_32(SYS_BASE + CA35WRBADR1, ma35d1_sec_entrypoint);
		mmio_write_32(SYS_BASE + CA35WRBPAR1, 0x7761726D);
		ma35d1_deep_power_down();
	} else {

		ma35d1_normal_power_down();
	}

}

static void ma35d1_pwr_domain_on_finish(const psci_power_state_t *target_state)
{
	plat_arm_gic_init();

	/* Enable the gic cpu interface */
	gicv2_cpuif_enable();
	gicv2_pcpu_distif_init();
}

int ma35d1_validate_ns_entrypoint(uintptr_t ns_entrypoint)
{
	return PSCI_E_SUCCESS;
}


static void ma35d1_pwr_domain_suspend_finish(const
			psci_power_state_t * target_state)
{
	/* Clear poer down flag */
	mmio_write_32(SYS_BASE + PMUSTS, (1 << 8));

	/* Clear Core 1 Warm-boot */
	mmio_write_32(SYS_BASE + CA35WRBPAR1, 0);

	plat_arm_gic_init();

	/* Enable the gic cpu interface */
	gicv2_cpuif_enable();
	gicv2_pcpu_distif_init();

	/* Disable the Non secure interrupt to wake the CPU */
	write_scr_el3(read_scr_el3() & ~(SCR_IRQ_BIT | SCR_FIQ_BIT));

	isb();
	/* dsb is good practice before using wfi to enter low power states */
	dsb();
}

/*******************************************************************************
 * MA35D1 handlers to shutdown/reboot the system
 ******************************************************************************/
static void __dead2 ma35d1_system_off(void)
{
	while (1)
		;
}


static void __dead2 ma35d1_system_reset(void)
{
	ma35d1_UnlockReg();
	mmio_write_32(SYS_IPRST0, 0x1);
	mmio_write_32(SYS_IPRST0, 0x0);

	while (1)
		;
}


static int ma35d1_validate_power_state(unsigned int power_state,
					psci_power_state_t * req_state)
{
	int pstate = psci_get_pstate_type(power_state);
	int pwr_lvl = psci_get_pstate_pwrlvl(power_state);
	int i;

	assert(req_state);

	if (pwr_lvl > PLAT_MAX_PWR_LVL)
		return PSCI_E_INVALID_PARAMS;

	/* Sanity check the requested state */
	if (pstate == PSTATE_TYPE_STANDBY) {
		/*
		 * It's probably to enter standby only on power level 0
		 * ignore any other power level.
		 */
		if (pwr_lvl != MPIDR_AFFLVL0)
			return PSCI_E_INVALID_PARAMS;

		req_state->pwr_domain_state[MPIDR_AFFLVL0] = PLAT_MAX_RET_STATE;
	} else {

		for (i = MPIDR_AFFLVL0; i <= pwr_lvl; i++)
			req_state->pwr_domain_state[i] = PLAT_MAX_OFF_STATE;

		for (i = (pwr_lvl); i <= PLAT_MAX_PWR_LVL; i++)
			req_state->pwr_domain_state[i] = PLAT_MAX_RET_STATE;
	}

	/* We expect the 'state id' to be zero */
	if (psci_get_pstate_id(power_state))
		return PSCI_E_INVALID_PARAMS;

	return PSCI_E_SUCCESS;
}


void ma35d1_get_sys_suspend_power_state(psci_power_state_t
        *req_state)
{
	int i;

	for (i = MPIDR_AFFLVL0; i <= PLAT_MAX_PWR_LVL; i++)
		req_state->pwr_domain_state[i] = PLAT_MAX_OFF_STATE;
}


void __dead2 ma35d1_pwr_domain_pwr_down_wfi(const
						psci_power_state_t * target_state)										
{
	u_register_t scr;

	scr = read_scr_el3();

	/* Enable the Non secure interrupt to wake the CPU */
	write_scr_el3(scr | SCR_IRQ_BIT | SCR_FIQ_BIT);
	isb();
	/* dsb is good practice before using wfi to enter low power states */
	dsb();

	if (MA35D1_SYSTEM_PWR_STATE(target_state) == PLAT_MAX_OFF_STATE)
		while(1)
			wfi();
	else {

		while (1) {
			wfi();
			((void (*)(void))ma35d1_sec_entrypoint) ();
		}
	}
}

plat_psci_ops_t plat_arm_psci_pm_ops = {
	.cpu_standby = ma35d1_cpu_standby,
	.pwr_domain_on = ma35d1_pwr_domain_on,
	.pwr_domain_off = ma35d1_pwr_domain_off,
	.validate_ns_entrypoint = ma35d1_validate_ns_entrypoint,
	.pwr_domain_suspend = ma35d1_pwr_domain_suspend,
	.pwr_domain_on_finish = ma35d1_pwr_domain_on_finish,
	.pwr_domain_suspend_finish = ma35d1_pwr_domain_suspend_finish,
	.system_off = ma35d1_system_off,
	.system_reset = ma35d1_system_reset,
	.validate_power_state = ma35d1_validate_power_state,
	.pwr_domain_pwr_down_wfi = ma35d1_pwr_domain_pwr_down_wfi,
	.get_sys_suspend_power_state = ma35d1_get_sys_suspend_power_state,
};

/*******************************************************************************
 * Export the platform specific power ops.
 ******************************************************************************/
int plat_setup_psci_ops(uintptr_t sec_entrypoint,
			const plat_psci_ops_t ** psci_ops)
{
	ma35d1_sec_entrypoint = sec_entrypoint;
	*psci_ops = &plat_arm_psci_pm_ops;

	mmio_write_32(CLK_BASE, mmio_read_32(CLK_BASE) | (1 << 9));//ICE DB
	return 0;
}
