/*
 * Copyright 2012 Google Inc.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but without any warranty; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <image/fmap.h>
#include <libpayload.h>
#include <lzma.h>
#include <vb2_api.h>
#include <vboot_api.h>

#include "base/vpd_util.h"
#include "drivers/flash/flash.h"
#include "vboot/nvdata.h"
#include "vboot/secdata_tpm.h"
#include "vboot/util/commonparams.h"
#include "vboot/util/flag.h"

uint32_t VbExIsShutdownRequested(void)
{
	int lidsw = flag_fetch(FLAG_LIDSW);
	int pwrsw = 0;
	uint32_t shutdown_request = 0;

	// Power button is always handled as a keyboard key on detachables.
	if (!CONFIG(DETACHABLE))
		pwrsw = flag_fetch(FLAG_PWRSW);

	if (lidsw < 0 || pwrsw < 0) {
		// There isn't any way to return an error, so just hang.
		printf("Failed to fetch lid or power switch flag.\n");
		halt();
	}

	if (!lidsw) {
		printf("Lid is closed.\n");
		shutdown_request |= VB_SHUTDOWN_REQUEST_LID_CLOSED;
	}
	if (pwrsw) {
		printf("Power key pressed.\n");
		shutdown_request |= VB_SHUTDOWN_REQUEST_POWER_BUTTON;
	}

	return shutdown_request;
}

vb2_error_t VbExSetVendorData(const char *vendor_data_value)
{
	FmapArea vpd_area_descriptor;
	uint8_t *ro_vpd;
	uint32_t size, offset;

	printf("%s: Setting %s to '%s'\n", __func__,
	       CONFIG_VENDOR_DATA_KEY, vendor_data_value);
	if (strnlen(vendor_data_value, CONFIG_VENDOR_DATA_LENGTH + 1) !=
		CONFIG_VENDOR_DATA_LENGTH)
		return VB2_ERROR_INVALID_PARAMETER;

	if (fmap_find_area("RO_VPD", &vpd_area_descriptor)) {
		printf("%s: failed to find RO_VPD area\n", __func__);
		return VB2_ERROR_EX_SET_VENDOR_DATA;
	}

	/* Read RO VPD from flash */
	ro_vpd = flash_read(vpd_area_descriptor.offset,
			    vpd_area_descriptor.size);

	if (!ro_vpd) {
		printf("%s: failed to read RO_VPD area\n", __func__);
		return VB2_ERROR_EX_SET_VENDOR_DATA;
	}

	/* Find the vendor data value */
	if (!vpd_find(CONFIG_VENDOR_DATA_KEY, ro_vpd, &offset, &size) ||
	    size != CONFIG_VENDOR_DATA_LENGTH)
		return VB2_ERROR_EX_SET_VENDOR_DATA;

	if (flash_write_status(0x00)) {
		printf("%s: failed to disable wp\n", __func__);
		return VB2_ERROR_EX_SET_VENDOR_DATA;
	}

	/* Set vendor data to new value */
	if (flash_rewrite(vpd_area_descriptor.offset + offset,
			  CONFIG_VENDOR_DATA_LENGTH, vendor_data_value) !=
			  CONFIG_VENDOR_DATA_LENGTH) {
		printf("%s: failed to rewrite RO_VPD area\n", __func__);
		return VB2_ERROR_EX_SET_VENDOR_DATA;
	}

	/* Clear GBB flags */
	if (gbb_clear_flags()) {
		printf("%s: failed to clear GBB flags\n", __func__);
		return VB2_ERROR_EX_SET_VENDOR_DATA;
	}

	if (flash_set_wp_enabled()) {
		printf("%s: failed to enable wp\n", __func__);
		return VB2_ERROR_EX_SET_VENDOR_DATA;
	}

	return VB2_SUCCESS;
}

vb2_error_t vb2ex_read_resource(struct vb2_context *ctx,
			enum vb2_resource_index index,
			uint32_t offset,
			void *buf,
			uint32_t size)
{
	FmapArea area;
	void *data;

	if (index != VB2_RES_GBB)
		return VB2_ERROR_EX_READ_RESOURCE_INDEX;

	if (fmap_find_area("GBB", &area)) {
		printf("%s: couldn't find GBB region\n", __func__);
		return VB2_ERROR_EX_READ_RESOURCE_INDEX;
	}

	if ((offset + size) > area.size) {
		printf("%s: offset outside of GBB region\n", __func__);
		return VB2_ERROR_EX_READ_RESOURCE_SIZE;
	}

	data = flash_read(area.offset + offset, size);
	if (!data) {
		printf("%s: failed to read from GBB region\n", __func__);
		return VB2_ERROR_EX_READ_RESOURCE_INDEX;
	}

	memcpy(buf, data, size);

	return VB2_SUCCESS;
}

vb2_error_t vb2ex_commit_data(struct vb2_context *ctx)
{
	vb2_error_t rv = VB2_SUCCESS;
	vb2_error_t nvdata_rv;
	uint32_t tpm_rv;

	/* Write secdata spaces.  Firmware never needs to write secdata_fwmp. */

	tpm_rv = secdata_kernel_write(ctx);
	if (tpm_rv) {
		printf("%s: write secdata_kernel returned %#x\n",
		       __func__, tpm_rv);
		if (!(ctx->flags & VB2_CONTEXT_RECOVERY_MODE)) {
			vb2api_fail(ctx, VB2_RECOVERY_RW_TPM_W_ERROR, rv);
			rv = VB2_ERROR_SECDATA_KERNEL_WRITE;
		}
	}

	tpm_rv = secdata_firmware_write(ctx);
	if (tpm_rv) {
		printf("%s: write secdata_firmware returned %#x\n",
		       __func__, tpm_rv);
		if (!(ctx->flags & VB2_CONTEXT_RECOVERY_MODE)) {
			vb2api_fail(ctx, VB2_RECOVERY_RW_TPM_W_ERROR, rv);
			rv = VB2_ERROR_SECDATA_FIRMWARE_WRITE;
		}
	}

	nvdata_rv = nvdata_write(ctx);
	if (nvdata_rv) {
		printf("%s: write nvdata returned %#x\n",
		       __func__, nvdata_rv);
		/*
		 * We can't write to nvdata, so it's impossible to trigger
		 * recovery mode.  Skip calling vb2api_fail and just die.
		 */
		if (!(ctx->flags & VB2_CONTEXT_RECOVERY_MODE))
			die("can't write recovery reason to nvdata");
		/* If we *are* in recovery mode, ignore any error and return. */
		return VB2_SUCCESS;
	}

	return rv;
}
