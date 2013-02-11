/*
 * Copyright 2013 Google Inc.
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

#include "config.h"
#include "drivers/flash/flash.h"

void *flash_read(uint32_t offset, uint32_t size)
{
	const uint32_t flash_size = CONFIG_DRIVER_FLASH_MEMMAPPED_SIZE;
	const uint32_t flash_base = CONFIG_DRIVER_FLASH_MEMMAPPED_BASE;

	if (offset > flash_size || offset + size > flash_size) {
		printf("Out of bounds flash access.\n");
		return NULL;
	}

	return (void *)(uintptr_t)(offset + flash_base);
}
