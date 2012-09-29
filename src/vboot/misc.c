/*
 * Copyright (c) 2012 The Chromium OS Authors.
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
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <libpayload.h>
#include <lzma.h>
#include <vboot_api.h>

#include "base/gpio.h"

uint32_t VbExIsShutdownRequested(void)
{
	gpio_t lidsw, pwrsw;

	if (gpio_fetch(GPIO_LIDSW, &lidsw) || gpio_fetch(GPIO_PWRSW, &pwrsw)) {
		printf("Failed to fetch lid or power switch GPIO.\n");
		return 1;
	}

	if (!lidsw.value) {
		printf("Lid is closed.\n");
		return 1;
	}
	if (pwrsw.value) {
		printf("Power key pressed.\n");
		return 1;
	}

	return 0;
}

VbError_t VbExDecompress(void *inbuf, uint32_t in_size,
			 uint32_t compression_type,
			 void *outbuf, uint32_t *out_size)
{
	switch (compression_type) {
	case COMPRESS_NONE:
		memcpy(outbuf, inbuf, in_size);
		*out_size = in_size;
		break;
	case COMPRESS_EFIv1:
		printf("EFIv1 compression not supported.\n");
		return VBERROR_UNKNOWN;
	case COMPRESS_LZMA1:
		*out_size = ulzma(inbuf, outbuf);
		if (!*out_size) {
			printf("Error doing LZMA decompression.\n");
			return VBERROR_UNKNOWN;
		}
		break;
	default:
		printf("Unrecognized compression type %d.\n",
		       compression_type);
		return VBERROR_INVALID_PARAMETER;
	}
	return VBERROR_SUCCESS;
}
