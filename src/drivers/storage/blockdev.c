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
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include "drivers/storage/blockdev.h"

ListNode fixed_block_devices;
ListNode removable_block_devices;

ListNode block_dev_controllers;

int block_dev_init(void)
{
	BlockDevCtrlr *ctrlr;
	int res = 0;
	list_for_each(ctrlr, block_dev_controllers, list_node)
		if (ctrlr->ops.init)
			res = ctrlr->ops.init(&ctrlr->ops) | res;
	return res;
}

int block_dev_refresh(void)
{
	BlockDevCtrlr *ctrlr;
	int res = 0;
	list_for_each(ctrlr, block_dev_controllers, list_node)
		if (ctrlr->ops.refresh)
			res = ctrlr->ops.refresh(&ctrlr->ops) | res;
	return res;
}
