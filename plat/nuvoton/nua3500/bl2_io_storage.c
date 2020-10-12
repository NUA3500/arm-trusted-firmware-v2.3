/*
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>
#include <string.h>

#include <platform_def.h>

#include <arch_helpers.h>
#include <common/debug.h>
#include <drivers/io/io_block.h>
#include <drivers/io/io_driver.h>
#include <drivers/io/io_dummy.h>
#include <drivers/io/io_mtd.h>
#include <drivers/io/io_storage.h>
#include <drivers/mmc.h>
#include <drivers/partition/partition.h>
#include <drivers/raw_nand.h>
#include <drivers/spi_nand.h>
#include <drivers/spi_nor.h>
#include <lib/mmio.h>
#include <lib/utils.h>

#include "nua3500_private.h"

/* IO devices */
//static const io_dev_connector_t *dummy_dev_con;
static uintptr_t dummy_dev_handle;
//static uintptr_t dummy_dev_spec;

//static uintptr_t image_dev_handle;
//static uintptr_t storage_dev_handle;

#if NUA3500_SDMMC || NUA3500_EMMC
static io_block_spec_t gpt_block_spec = {
	.offset = 0,
	.length = 34 * MMC_BLOCK_SIZE, /* Size of GPT table */
};

static uint32_t block_buffer[MMC_BLOCK_SIZE] __aligned(MMC_BLOCK_SIZE);

static const io_block_dev_spec_t mmc_block_dev_spec = {
	/* It's used as temp buffer in block driver */
	.buffer = {
		.offset = (size_t)&block_buffer,
		.length = MMC_BLOCK_SIZE,
	},
	.ops = {
		.read = mmc_read_blocks,
		.write = NULL,
	},
	.block_size = MMC_BLOCK_SIZE,
};

static const io_dev_connector_t *mmc_dev_con;
#endif /* NUA3500_SDMMC || NUA3500_EMMC */

#if NUA3500_SPI_NOR
static io_mtd_dev_spec_t spi_nor_dev_spec = {
	.ops = {
		.init = spi_nor_init,
		.read = spi_nor_read,
	},
};
#endif

//#if NUA3500_RAW_NAND
#if 0
static io_mtd_dev_spec_t nand_dev_spec = {
	.ops = {
		.init = nand_raw_init,
		.read = nand_read,
	},
};

static const io_dev_connector_t *nand_dev_con;
#endif

#if NUA3500_SPI_NAND
static io_mtd_dev_spec_t spi_nand_dev_spec = {
	.ops = {
		.init = spi_nand_init,
		.read = nand_read,
	},
};
#endif

#if NUA3500_SPI_NAND || NUA3500_SPI_NOR
static const io_dev_connector_t *spi_dev_con;
#endif

static const io_block_spec_t bl2_block_spec = {
	.offset = BL2_BASE,
	.length = NUA3500_BL2_SIZE,
};


static int open_dummy(const uintptr_t spec);

struct plat_io_policy {
	uintptr_t *dev_handle;
	uintptr_t image_spec;
	int (*check)(const uintptr_t spec);
};

static const struct plat_io_policy policies[] = {
	[BL2_IMAGE_ID] = {
		.dev_handle = &dummy_dev_handle,
		.image_spec = (uintptr_t)&bl2_block_spec,
		.check = open_dummy
	},
#if NUA3500_SDMMC || NUA3500_EMMC
	[GPT_IMAGE_ID] = {
		.dev_handle = &storage_dev_handle,
		.image_spec = (uintptr_t)&gpt_block_spec,
	},
#endif
};

static int open_dummy(const uintptr_t spec)
{
	return io_dev_init(dummy_dev_handle, 0);
}


#if NUA3500_SDMMC || NUA3500_EMMC
static void boot_mmc(enum mmc_device_type mmc_dev_type,
		     uint16_t boot_interface_instance)
{
}
#endif /* NUA3500_SDMMC || NUA3500_EMMC */

#if NUA3500_SPI_NOR
static void boot_spi_nor(boot_api_context_t *boot_context)
{
}
#endif /* NUA3500_SPI_NOR */

//#if NUA3500_RAW_NAND
#if 0
static void boot_fmc2_nand(boot_api_context_t *boot_context)
{
}
#endif /* NUA3500_RAW_NAND */

#if NUA3500_SPI_NAND
static void boot_spi_nand(boot_api_context_t *boot_context)
{
}
#endif /* NUA3500_SPI_NAND */

void nua3500_io_setup(void)
{
#if 0
	int io_result __unused;
	boot_api_context_t *boot_context =
		(boot_api_context_t *)stm32mp_get_boot_ctx_address();

	print_boot_device(boot_context);

	if ((boot_context->boot_partition_used_toboot == 1U) ||
	    (boot_context->boot_partition_used_toboot == 2U)) {
		INFO("Boot used partition fsbl%d\n",
		     boot_context->boot_partition_used_toboot);
	}

	io_result = register_io_dev_dummy(&dummy_dev_con);
	assert(io_result == 0);

	io_result = io_dev_open(dummy_dev_con, dummy_dev_spec,
				&dummy_dev_handle);
	assert(io_result == 0);

	switch (boot_context->boot_interface_selected) {
#if NUA3500_SDMMC
	case BOOT_API_CTX_BOOT_INTERFACE_SEL_FLASH_SD:
		dmbsy();
		boot_mmc(MMC_IS_SD, boot_context->boot_interface_instance);
		break;
#endif
#if NUA3500_EMMC
	case BOOT_API_CTX_BOOT_INTERFACE_SEL_FLASH_EMMC:
		dmbsy();
		boot_mmc(MMC_IS_EMMC, boot_context->boot_interface_instance);
		break;
#endif
#if NUA3500_SPI_NOR
	case BOOT_API_CTX_BOOT_INTERFACE_SEL_FLASH_NOR_QSPI:
		dmbsy();
		boot_spi_nor(boot_context);
		break;
#endif
#if NUA3500_RAW_NAND
	case BOOT_API_CTX_BOOT_INTERFACE_SEL_FLASH_NAND_FMC:
		dmbsy();
		boot_fmc2_nand(boot_context);
		break;
#endif
#if NUA3500_SPI_NAND
	case BOOT_API_CTX_BOOT_INTERFACE_SEL_FLASH_NAND_QSPI:
		dmbsy();
		boot_spi_nand(boot_context);
		break;
#endif

	default:
		ERROR("Boot interface %d not supported\n",
		      boot_context->boot_interface_selected);
		break;
	}
#endif
}

/*
 * Return an IO device handle and specification which can be used to access
 * an image. Use this to enforce platform load policy.
 */
int plat_get_image_source(unsigned int image_id, uintptr_t *dev_handle,
			  uintptr_t *image_spec)
{
	int rc;
	const struct plat_io_policy *policy;

	assert(image_id < ARRAY_SIZE(policies));

	policy = &policies[image_id];
	rc = policy->check(policy->image_spec);
	if (rc == 0) {
		*image_spec = policy->image_spec;
		*dev_handle = *(policy->dev_handle);
	}

	return rc;
}

