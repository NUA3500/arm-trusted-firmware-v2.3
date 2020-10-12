/*
 * Copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>

#include <common/debug.h>
#include <drivers/io/io_block.h>
#include <drivers/io/io_driver.h>
#include <drivers/io/io_dummy.h>
#include <drivers/io/io_memmap.h>

#include <drivers/io/io_mtd.h>
#include <drivers/io/io_storage.h>
#include <drivers/io/io_semihosting.h>
#include <drivers/mmc.h>
#include <drivers/partition/partition.h>
#include <drivers/raw_nand.h>
#include <drivers/spi_nand.h>
#include <drivers/spi_nor.h>
#include <lib/semihosting.h>
#include <plat/arm/common/plat_arm.h>
#include <plat/common/common_def.h>

#include <tools_share/firmware_image_package.h>

#include "nua3500_private.h"

/* Semihosting filenames */
#define BL2_IMAGE_NAME			"bl2.bin"
#define BL31_IMAGE_NAME			"bl31.bin"
#define BL32_IMAGE_NAME			"bl32.bin"
#define BL33_IMAGE_NAME			"bl33.bin"
#define TB_FW_CONFIG_NAME		"nua3500_tb_fw_config.dtb"
#define HW_CONFIG_NAME			"hw_config.dtb"

#if TRUSTED_BOARD_BOOT
#define TRUSTED_BOOT_FW_CERT_NAME	"tb_fw.crt"
#define TRUSTED_KEY_CERT_NAME		"trusted_key.crt"
#define SOC_FW_KEY_CERT_NAME		"soc_fw_key.crt"
#define TOS_FW_KEY_CERT_NAME		"tos_fw_key.crt"
#define NT_FW_KEY_CERT_NAME		"nt_fw_key.crt"
#define SOC_FW_CONTENT_CERT_NAME	"soc_fw_content.crt"
#define TOS_FW_CONTENT_CERT_NAME	"tos_fw_content.crt"
#define NT_FW_CONTENT_CERT_NAME		"nt_fw_content.crt"
#endif /* TRUSTED_BOARD_BOOT */

/* IO devices */

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

#define OPTEE_HEADER_IMAGE_NAME		"teeh"
#define OPTEE_PAGED_IMAGE_NAME		"teed"
#define OPTEE_PAGER_IMAGE_NAME		"teex"
#define OPTEE_HEADER_BINARY_TYPE	U(0x20)
#define OPTEE_PAGER_BINARY_TYPE		U(0x21)
#define OPTEE_PAGED_BINARY_TYPE		U(0x22)

struct nua3500_part_info {
	char name[8];
	uint32_t binary_type;
	uintptr_t part_offset;
	uint32_t bkp_offset;
};

/* dummy */
static const io_dev_connector_t *dummy_dev_con;
static uintptr_t dummy_dev_handle;
static uintptr_t dummy_dev_spec;

//static uintptr_t image_dev_handle;
//static uintptr_t storage_dev_handle;

static const io_block_spec_t bl2_block_spec = {
	.offset = BL2_BASE,
	.length = NUA3500_BL2_SIZE,
};

static const io_block_spec_t bl31_block_spec = {
	.offset = NUA3500_BL31_BASE,
	.length = NUA3500_BL31_SIZE,
};

#if 0
static const io_block_spec_t bl32_block_spec = {
	.offset = NUA3500_BL32_BASE,
	.length = NUA3500_BL32_SIZE,
};
#else
static const struct nua3500_part_info optee_header_partition_spec = {
	.name = OPTEE_HEADER_IMAGE_NAME,
	.binary_type = OPTEE_HEADER_BINARY_TYPE,
};

static const struct nua3500_part_info optee_pager_partition_spec = {
	.name = OPTEE_PAGER_IMAGE_NAME,
	.binary_type = OPTEE_PAGER_BINARY_TYPE,
};

static const struct nua3500_part_info optee_paged_partition_spec = {
	.name = OPTEE_PAGED_IMAGE_NAME,
	.binary_type = OPTEE_PAGED_BINARY_TYPE,
};
#endif

static const io_block_spec_t bl33_block_spec = {
	.offset = NUA3500_BL33_BASE,
	.length = NUA3500_BL33_SIZE,
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
	[BL31_IMAGE_ID] = {
		.dev_handle = &dummy_dev_handle,
		.image_spec = (uintptr_t)&bl31_block_spec,
		.check = open_dummy
	},
#if 0
	[BL32_IMAGE_ID] = {
		.dev_handle = &dummy_dev_handle,
		.image_spec = (uintptr_t)&bl32_block_spec,
		.check = open_dummy
	},
#else
	[BL32_IMAGE_ID] = {
		.dev_handle = &dummy_dev_handle,
		.image_spec = (uintptr_t)&optee_header_partition_spec,
		.check = open_dummy
	},
	[BL32_EXTRA1_IMAGE_ID] = {
		.dev_handle = &dummy_dev_handle,
		.image_spec = (uintptr_t)&optee_pager_partition_spec,
		.check = open_dummy
	},
	[BL32_EXTRA2_IMAGE_ID] = {
		.dev_handle = &dummy_dev_handle,
		.image_spec = (uintptr_t)&optee_paged_partition_spec,
		.check = open_dummy
	},
#endif
	[BL33_IMAGE_ID] = {
		.dev_handle = &dummy_dev_handle,
		.image_spec = (uintptr_t)&bl33_block_spec,
		.check = open_dummy
	},
};

static int open_dummy(const uintptr_t spec)
{
	return io_dev_init(dummy_dev_handle, 0);
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
#endif /* STM32MP_SPI_NOR */

#if 0
#if NUA3500_RAW_NAND
static void boot_nand(void)
{
	int io_result __unused;

	io_result = nua3500_nand_init();
	assert(io_result == 0);

	/* Register the IO device on this platform */
	io_result = register_io_dev_mtd(&nand_dev_con);
	assert(io_result == 0);

	/* Open connections to device */
	io_result = io_dev_open(nand_dev_con, (uintptr_t)&nand_dev_spec,
				&storage_dev_handle);
	assert(io_result == 0);

}
#endif /* NUA3500_RAW_NAND */
#endif

#if NUA3500_SPI_NAND
static void boot_spi_nand(boot_api_context_t *boot_context)
{
}
#endif /* NUA3500_SPI_NAND */


void nua3500_io_setup(void)
{
	int result __unused;

	/* dummy */
	result = register_io_dev_dummy(&dummy_dev_con);
	assert(result == 0);

	result = io_dev_open(dummy_dev_con, dummy_dev_spec,
				&dummy_dev_handle);
	assert(result == 0);

#if 0
	/* select boot device */
	dmbsy();
#if NUA3500_SDMMC
	INFO("Using SDMMC\n");
	boot_mmc(MMC_IS_SD, boot_context->boot_interface_instance);
#endif
#if NUA3500_EMMC
	INFO("Using EMMC\n");
	boot_mmc(MMC_IS_EMMC, boot_context->boot_interface_instance);
#endif
#if NUA3500_SPI_NOR
	INFO("Using SPI NOR\n");
	boot_spi_nor(boot_context);
#endif
#if NUA3500_RAW_NAND
	INFO("Using NAND\n");
	boot_nand();
#endif
#if NUA3500_SPI_NAND
	INFO("Using SPI NAND\n");
	boot_spi_nand(boot_context);
#endif
#endif

}





