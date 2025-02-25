/*
 * Copyright (C) 2021-2022, STMicroelectronics - All Rights Reserved
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef STM32MP1_FIP_DEF_H
#define STM32MP1_FIP_DEF_H

#if STM32MP15_OPTEE_RSV_SHM
#define STM32MP_DDR_S_SIZE		U(0x01E00000)	/* 30 MB */
#define STM32MP_DDR_SHMEM_SIZE		U(0x00200000)	/* 2 MB */
#else
#define STM32MP_DDR_S_SIZE		U(0x02000000)	/* 32 MB */
#define STM32MP_DDR_SHMEM_SIZE		U(0)		/* empty */
#endif

#if TRUSTED_BOARD_BOOT && !STM32MP_USE_EXTERNAL_HEAP
#if STM32MP15
#define STM32MP_BL2_RO_SIZE		U(0x00014000)	/* 80 KB */
#define STM32MP_BL2_SIZE		U(0x0001B000)	/* 108 KB for BL2 */
#endif /* STM32MP15 */
#else /* TRUSTED_BOARD_BOOT && !STM32MP_USE_EXTERNAL_HEAP */
#if STM32MP13
#if BL2_IN_XIP_MEM
#define STM32MP_BL2_RO_SIZE		U(0x00015000)	/* 84 KB */
#define STM32MP_BL2_SIZE		U(0x00017000)	/* 92 KB for BL2 */
#else
/* STM32MP_BL2_RO_SIZE not used if !BL2_IN_XIP_MEM */
#define STM32MP_BL2_SIZE		U(0x0001B000)	/* 108KB for BL2 */
					/* with 20KB for DTB, SYSRAM is full */
#endif
#endif /* STM32MP13 */
#if STM32MP15
#define STM32MP_BL2_RO_SIZE		U(0x00011000)	/* 68 KB */
#define STM32MP_BL2_SIZE		U(0x00016000)	/* 88 KB for BL2 */
#endif /* STM32MP15 */
#endif /* TRUSTED_BOARD_BOOT && !STM32MP_USE_EXTERNAL_HEAP */

#if STM32MP13
#if TRUSTED_BOARD_BOOT
#define STM32MP_BL2_DTB_SIZE		U(0x00005000)	/* 20 KB for DTB */
#else /* TRUSTED_BOARD_BOOT */
#define STM32MP_BL2_DTB_SIZE		U(0x00004000)	/* 16 KB for DTB */
#endif /* TRUSTED_BOARD_BOOT */
#endif /* STM32MP13 */
#if STM32MP15
#define STM32MP_BL2_DTB_SIZE		U(0x00007000)	/* 28 KB for DTB */
#endif /* STM32MP15 */
#define STM32MP_BL32_SIZE		U(0x0001B000)	/* 108 KB for BL32 */
#define STM32MP_BL32_DTB_SIZE		U(0x00005000)	/* 20 KB for DTB */
#define STM32MP_FW_CONFIG_MAX_SIZE	PAGE_SIZE	/* 4 KB for FCONF DTB */
#define STM32MP_HW_CONFIG_MAX_SIZE	U(0x40000)	/* 256 KB for HW config DTB */

#if STM32MP13
#define STM32MP_BL2_BASE		(STM32MP_BL2_DTB_BASE + \
					 STM32MP_BL2_DTB_SIZE)
#endif /* STM32MP13 */
#if STM32MP15
#define STM32MP_BL2_BASE		(STM32MP_SEC_SYSRAM_BASE + \
					 STM32MP_SEC_SYSRAM_SIZE - \
					 STM32MP_BL2_SIZE)
#endif /* STM32MP15 */

#define STM32MP_BL2_RO_BASE		STM32MP_BL2_BASE

#define STM32MP_BL2_RW_BASE		(STM32MP_BL2_RO_BASE + \
					 STM32MP_BL2_RO_SIZE)

#if STM32MP13
#define STM32MP_BL2_RW_SIZE		(STM32MP_SYSRAM_BASE + \
					 STM32MP_SYSRAM_SIZE - \
					 STM32MP_BL2_RW_BASE)

#define STM32MP_BL2_DTB_BASE		STM32MP_SEC_SYSRAM_BASE
#endif /* STM32MP13 */
#if STM32MP15
#define STM32MP_BL2_RW_SIZE		(STM32MP_SEC_SYSRAM_BASE + \
					 STM32MP_SEC_SYSRAM_SIZE - \
					 STM32MP_BL2_RW_BASE)

#define STM32MP_BL2_DTB_BASE		(STM32MP_BL2_BASE - \
					 STM32MP_BL2_DTB_SIZE)
#endif /* STM32MP15 */

#define STM32MP_BL32_DTB_BASE		STM32MP_SYSRAM_BASE

#define STM32MP_BL32_BASE		(STM32MP_BL32_DTB_BASE + \
					 STM32MP_BL32_DTB_SIZE)


#if defined(IMAGE_BL2)
#define STM32MP_DTB_SIZE		STM32MP_BL2_DTB_SIZE
#define STM32MP_DTB_BASE		STM32MP_BL2_DTB_BASE
#endif
#if defined(IMAGE_BL32)
#define STM32MP_DTB_SIZE		STM32MP_BL32_DTB_SIZE
#define STM32MP_DTB_BASE		STM32MP_BL32_DTB_BASE
#endif

#ifdef AARCH32_SP_OPTEE
#define STM32MP_OPTEE_BASE		STM32MP_SEC_SYSRAM_BASE

#define STM32MP_OPTEE_SIZE		(STM32MP_BL2_DTB_BASE -  \
					 STM32MP_OPTEE_BASE)
#endif

#if STM32MP13
#define STM32MP_FW_CONFIG_BASE		SRAM3_BASE
#endif /* STM32MP13 */
#if STM32MP15
#define STM32MP_FW_CONFIG_BASE		(STM32MP_SYSRAM_BASE + \
					 STM32MP_SYSRAM_SIZE - \
					 PAGE_SIZE)
#endif /* STM32MP15 */
#define STM32MP_HW_CONFIG_BASE		(STM32MP_BL33_BASE + \
					STM32MP_BL33_MAX_SIZE)

/*
 * MAX_MMAP_REGIONS is usually:
 * BL stm32mp1_mmap size + mmap regions in *_plat_arch_setup
 */
#if defined(IMAGE_BL32)
#define MAX_MMAP_REGIONS		10
#endif

/*******************************************************************************
 * STM32MP1 RAW partition offset for devices without GPT
 ******************************************************************************/
#define STM32MP_EMMC_BOOT_FIP_OFFSET	U(0x00040000)

/*******************************************************************************
 * STM32MP1 RAW partition offset for MTD devices
 ******************************************************************************/
#if PSA_FWU_SUPPORT
#define STM32MP_NOR_METADATA1_OFFSET	U(0x00080000)
#define STM32MP_NOR_METADATA2_OFFSET	U(0x000C0000)
#define STM32MP_NOR_FIP_A_OFFSET	U(0x00100000)
#define STM32MP_NOR_FIP_A_GUID		(const struct efi_guid)EFI_GUID(0x4fd84c93,  \
					0x54ef, 0x463f, 0xa7, 0xef, 0xae, 0x25, 0xff,\
					0x88, 0x70, 0x87)

#define STM32MP_NOR_FIP_B_OFFSET	U(0x00500000)
#define STM32MP_NOR_FIP_B_GUID		(const struct efi_guid)EFI_GUID(0x09c54952,  \
					0xd5bf, 0x45af, 0xac, 0xee, 0x33, 0x53, 0x03,\
					0x76, 0x6f, 0xb3)

#ifndef STM32MP_NOR_FIP_OFFSET
#define STM32MP_NOR_FIP_OFFSET		U(0x00100000)
#endif

#else /* PSA_FWU_SUPPORT */
#ifndef STM32MP_NOR_FIP_OFFSET
#define STM32MP_NOR_FIP_OFFSET		U(0x00080000)
#endif
#ifndef STM32MP_NAND_FIP_OFFSET
#define STM32MP_NAND_FIP_OFFSET		U(0x00200000)
#endif
#endif /* PSA_FWU_SUPPORT */

#endif /* STM32MP1_FIP_DEF_H */
