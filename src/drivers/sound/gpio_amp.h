/*
 * gpio_amp.h -- Audio driver for GPIO based amplifier
 *
 * Copyright (C) 2015 Google Inc.
 * Copyright (C) 2015 Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
 * GNU General Public License for more details.
 */

#ifndef __DRIVERS_SOUND_GPIO_AMP_H__
#define __DRIVERS_SOUND_GPIO_AMP_H__

#include "drivers/gpio/gpio.h"
#include "drivers/sound/route.h"

typedef struct
{
	SoundRouteComponent component;
	GpioOps *sdmode_gpio;

} GpioAmpCodec;

GpioAmpCodec *new_gpio_amp_codec(GpioOps *ops);

#endif /* __DRIVERS_SOUND_GPIO_AMP_H__ */
