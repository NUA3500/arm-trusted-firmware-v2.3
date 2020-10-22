#
# Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

# Default cluster count for NUA3500
NUA3500_CLUSTER_COUNT	:= 2
# Default number of CPUs per cluster on NUA3500
NUA3500_MAX_CPUS_PER_CLUSTER	:= 4
# Default number of threads per CPU on NUA3500
NUA3500_MAX_PE_PER_CPU	:= 1

BL2_AT_EL3		:=	1
USE_COHERENT_MEM	:=	0
#ENABLE_PIE		:=	1

# Pass NUA3500_CLUSTER_COUNT to the build system.
$(eval $(call add_define,NUA3500_CLUSTER_COUNT))
# Pass NUA3500_MAX_CPUS_PER_CLUSTER to the build system.
$(eval $(call add_define,NUA3500_MAX_CPUS_PER_CLUSTER))
# Pass NUA3500_MAX_PE_PER_CPU to the build system.
$(eval $(call add_define,NUA3500_MAX_PE_PER_CPU))
$(eval $(call add_define,NUA3500_INTERCONNECT_DRIVER))

# dump the state on crash console
CRASH_REPORTING		:=	1
$(eval $(call add_define,CRASH_REPORTING))

# enable assert() for release/debug builds
ENABLE_ASSERTIONS	:=	1
PLAT_LOG_LEVEL_ASSERT	:=	50
$(eval $(call add_define,PLAT_LOG_LEVEL_ASSERT))

# Build option to add debugfs support
USE_DEBUGFS		:= 1
DEBUG			:= 1
LOG_LEVEL		:= 40

# Enable dynamic memory mapping
PLAT_XLAT_TABLES_DYNAMIC :=	1
$(eval $(call assert_boolean,PLAT_XLAT_TABLES_DYNAMIC))
$(eval $(call add_define,PLAT_XLAT_TABLES_DYNAMIC))


# Boot devices
NUA3500_EMMC		?=	0
NUA3500_SDMMC		?=	0
NUA3500_RAW_NAND	?=	1
NUA3500_SPI_NAND	?=	0
NUA3500_SPI_NOR		?=	0

ifeq ($(filter 1,${NUA3500_EMMC} ${NUA3500_SDMMC} ${NUA3500_RAW_NAND} \
	${NUA3500_SPI_NAND} ${NUA3500_SPI_NOR}),)
$(error "No boot device driver is enabled")
endif

$(eval $(call assert_boolean,NUA3500_EMMC))
$(eval $(call assert_boolean,NUA3500_SDMMC))
$(eval $(call assert_boolean,NUA3500_RAW_NAND))
$(eval $(call assert_boolean,NUA3500_SPI_NAND))
$(eval $(call assert_boolean,NUA3500_SPI_NOR))
$(eval $(call add_define,NUA3500_EMMC))
$(eval $(call add_define,NUA3500_SDMMC))
$(eval $(call add_define,NUA3500_RAW_NAND))
$(eval $(call add_define,NUA3500_SPI_NAND))
$(eval $(call add_define,NUA3500_SPI_NOR))


PLAT_INCLUDES		:=	-Iplat/nuvoton/nua3500/include		\
				-Iinclude/plat/arm/common/aarch64

# Device tree
#DTB_FILE_NAME		?=	nua3500.dtb
#FDT_SOURCES		:=	$(addprefix plat/nuvoton/nua3500/fdts/,	${PLAT}_tb_fw_config.dts)
DTC_FLAGS		+=	-Wno-unit_address_vs_reg

# Add `libfdt` and Arm common helpers required for Dynamic Config
include lib/libfdt/libfdt.mk
# Firmware Configuration Framework sources
include lib/fconf/fconf.mk

include lib/debugfs/debugfs.mk

# Include GICv2 driver files
NUA3500_GIC_SOURCES :=		drivers/arm/gic/common/gic_common.c	\
				drivers/arm/gic/v2/gicv2_main.c		\
				drivers/arm/gic/v2/gicv2_helpers.c	\
				plat/common/plat_gicv2.c		\
				plat/arm/common/arm_gicv2.c		\

NUA3500_SECURITY_SOURCES :=	plat/nuvoton/nua3500/nua3500_security.c		\

PLAT_BL_COMMON_SOURCES	:=	common/fdt_wrappers.c				\
				plat/nuvoton/nua3500/drivers/nua3500_console.S	\
				plat/arm/common/arm_common.c			\
				plat/nuvoton/nua3500/nua3500_common.c		\
				plat/nuvoton/nua3500/drivers/nua3500_ddr.c	\
				plat/nuvoton/nua3500/nua3500_private.c		\
				plat/nuvoton/nua3500/drivers/nua3500_crypto.c	\
				plat/nuvoton/nua3500/drivers/tsi_cmd.c	        \

include lib/xlat_tables_v2/xlat_tables.mk
PLAT_BL_COMMON_SOURCES	+=	${XLAT_TABLES_LIB_SRCS}

PLAT_BL_COMMON_SOURCES	+=	lib/cpus/aarch64/cortex_a35.S			\
				lib/cpus/${ARCH}/aem_generic.S			\
				drivers/arm/cci/cci.c				\

PLAT_BL_COMMON_SOURCES	+=	drivers/arm/tzc/tzc400.c			\
				drivers/delay_timer/delay_timer.c		\
				drivers/delay_timer/generic_delay_timer.c	\
				plat/nuvoton/nua3500/aarch64/nua3500_helpers.S	\

BL2_SOURCES		+=	drivers/io/io_block.c				\
				drivers/io/io_dummy.c				\
				drivers/io/io_mtd.c				\
				drivers/io/io_storage.c				\
				plat/arm/common/arm_bl2_el3_setup.c		\
				plat/nuvoton/nua3500/nua3500_bl2_setup.c	\
				plat/nuvoton/nua3500/nua3500_io_storage.c	\
				plat/nuvoton/nua3500/nua3500_trusted_boot.c	\
				plat/nuvoton/nua3500/nua3500_bl2_el3_setup.c	\
				plat/nuvoton/nua3500/nua3500_bl2_mem_params_desc.c	\
				plat/nuvoton/nua3500/nua3500_image_load.c		\
				drivers/io/io_semihosting.c			\
				lib/optee/optee_utils.c				\
				lib/utils/mem_region.c				\
				lib/semihosting/semihosting.c			\
				lib/semihosting/${ARCH}/semihosting_call.S	\
				${NUA3500_SECURITY_SOURCES}

ifneq ($(filter 1,${NUA3500_EMMC} ${NUA3500_SDMMC}),)
BL2_SOURCES		+=	drivers/mmc/mmc.c					\
				drivers/partition/gpt.c					\
				drivers/partition/partition.c
endif

ifeq (${NUA3500_RAW_NAND},1)
$(eval $(call add_define_val,NAND_ONFI_DETECT,1))
BL2_SOURCES		+=	drivers/mtd/nand/raw_nand.c				\
#				plat/nuvoton/nua3500/drivers/nua3500_nand.c		\

endif

ifeq (${NUA3500_SPI_NAND},1)
BL2_SOURCES		+=	drivers/mtd/nand/spi_nand.c
endif

ifeq (${NUA3500_SPI_NOR},1)
BL2_SOURCES		+=	drivers/mtd/nor/spi_nor.c
endif

ifneq ($(filter 1,${NUA3500_SPI_NAND} ${NUA3500_SPI_NOR}),)
BL2_SOURCES		+=	drivers/mtd/spi-mem/spi_mem.c
endif

ifneq ($(filter 1,${NUA3500_RAW_NAND} ${NUA3500_SPI_NAND}),)
BL2_SOURCES		+=	drivers/mtd/nand/core.c
endif


BL2_SOURCES		+=	common/desc_image_load.c

BL2_SOURCES		+=	plat/arm/common/arm_dyn_cfg.c		\
				plat/arm/common/arm_dyn_cfg_helpers.c	\


BL31_SOURCES		+=	drivers/arm/smmu/smmu_v3.c			\
				drivers/cfi/v2m/v2m_flash.c			\
				lib/utils/mem_region.c				\
				plat/nuvoton/nua3500/nua3500_bl31_setup.c	\
				plat/nuvoton/nua3500/nua3500_pm.c		\
				plat/nuvoton/nua3500/nua3500_topology.c		\
				${NUA3500_GIC_SOURCES}				\
				${NUA3500_SECURITY_SOURCES}			\

BL31_SOURCES		+=	plat/common/plat_psci_common.c

include services/spd/opteed/opteed.mk

override BL1_SOURCES =



