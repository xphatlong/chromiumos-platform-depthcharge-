// SPDX-License-Identifier: GPL-2.0

#include <libpayload.h>
#include <pci.h>
#include <pci/pci.h>

#include "base/fw_config.h"
#include "board/brya/include/variant.h"
#include "drivers/bus/i2s/cavs-regs.h"
#include "drivers/bus/i2s/intel_common/max98357a.h"
#include "drivers/bus/usb/intel_tcss.h"
#include "drivers/gpio/alderlake.h"


#define SDMODE_PIN		GPP_A11
#define SDMODE_ENABLE		0
const struct audio_config *variant_probe_audio_config(void)
{
	static struct audio_config config;

	if (fw_config_probe(FW_CONFIG(AUDIO, AUDIO_MAX98357_ALC5682I_I2S))) {
		config = (struct audio_config){
			.bus = {
				.i2s.address		= SSP_I2S2_START_ADDRESS,
				.i2s.enable_gpio = {
							.pad = SDMODE_PIN,
							.active_low = SDMODE_ENABLE
				},
				.i2s.settings		= &max98357a_settings,
			},
			.amp = {
				.type			= AUDIO_GPIO_AMP,
				.gpio.enable_gpio	= SDMODE_PIN,
			},
			.codec = {
				.type			= AUDIO_MAX98357,
			},
		};
	}
	return &config;
}

const struct tcss_map *variant_get_tcss_map(size_t *count)
{
	*count = 0;
	return NULL;
}