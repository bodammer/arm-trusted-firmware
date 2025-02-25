/*
 * Copyright (c) 2020-2023, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>

#include <libfdt.h>
#include <tc_plat.h>

#include <common/bl_common.h>
#include <common/debug.h>
#include <drivers/arm/css/css_mhu_doorbell.h>
#include <drivers/arm/css/scmi.h>
#include <drivers/arm/sbsa.h>
#include <lib/fconf/fconf.h>
#include <lib/fconf/fconf_dyn_cfg_getter.h>
#include <plat/arm/common/plat_arm.h>
#include <plat/common/platform.h>

static scmi_channel_plat_info_t tc_scmi_plat_info[] = {
	{
		.scmi_mbx_mem = CSS_SCMI_PAYLOAD_BASE,
		.db_reg_addr = PLAT_CSS_MHU_BASE + SENDER_REG_SET(0),
		.db_preserve_mask = 0xfffffffe,
		.db_modify_mask = 0x1,
		.ring_doorbell = &mhuv2_ring_doorbell,
	}
};

void bl31_platform_setup(void)
{
	tc_bl31_common_platform_setup();
}

scmi_channel_plat_info_t *plat_css_get_scmi_info(unsigned int channel_id)
{

	return &tc_scmi_plat_info[channel_id];

}

void bl31_early_platform_setup2(u_register_t arg0, u_register_t arg1,
				u_register_t arg2, u_register_t arg3)
{
	arm_bl31_early_platform_setup((void *)arg0, arg1, arg2, (void *)arg3);

	/* Fill the properties struct with the info from the config dtb */
	fconf_populate("FW_CONFIG", arg1);
}

void tc_bl31_common_platform_setup(void)
{
	arm_bl31_platform_setup();

#if defined(PLATFORM_TEST_NV_COUNTERS) || defined(PLATFORM_TEST_TFM_TESTSUITE)
#ifdef PLATFORM_TEST_NV_COUNTERS
	nv_counter_test();
#elif PLATFORM_TEST_TFM_TESTSUITE
	run_platform_tests();
#endif
	/* Suspend booting */
	plat_error_handler(-1);
#endif
}

const plat_psci_ops_t *plat_arm_psci_override_pm_ops(plat_psci_ops_t *ops)
{
	return css_scmi_override_pm_ops(ops);
}

void __init bl31_plat_arch_setup(void)
{
	arm_bl31_plat_arch_setup();

	/* HW_CONFIG was also loaded by BL2 */
	const struct dyn_cfg_dtb_info_t *hw_config_info;

	hw_config_info = FCONF_GET_PROPERTY(dyn_cfg, dtb, HW_CONFIG_ID);
	assert(hw_config_info != NULL);

	fconf_populate("HW_CONFIG", hw_config_info->config_addr);
}

#if defined(SPD_spmd) && (SPMC_AT_EL3 == 0)
void tc_bl31_plat_runtime_setup(void)
{
	arm_bl31_plat_runtime_setup();

	/* Start secure watchdog timer. */
	plat_arm_secure_wdt_start();
}

void bl31_plat_runtime_setup(void)
{
	tc_bl31_plat_runtime_setup();
}

/*
 * Platform handler for Group0 secure interrupt.
 */
int plat_spmd_handle_group0_interrupt(uint32_t intid)
{
	/* Trusted Watchdog timer is the only source of Group0 interrupt now. */
	if (intid == SBSA_SECURE_WDOG_INTID) {
		INFO("Watchdog restarted\n");
		/* Refresh the timer. */
		plat_arm_secure_wdt_refresh();

		/* Deactivate the corresponding interrupt. */
		plat_ic_end_of_interrupt(intid);
		return 0;
	}

	return -1;
}
#endif /*defined(SPD_spmd) && (SPMC_AT_EL3 == 0)*/
