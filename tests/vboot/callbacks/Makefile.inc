# SPDX-License-Identifier: GPL-2.0

tests-y += ui_broken-test
tests-y += ui_broken-detachable-test
tests-y += ui_manual-test
tests-y += ui_manual-detachable-test
tests-y += ui_manual-ppkeyboard-test
tests-y += ui_developer-test
tests-y += ui_developer-detachable-test
tests-y += ui_diagnostic-test

ui_broken-test-srcs += tests/vboot/callbacks/ui_broken-test.c
ui_broken-test-srcs += tests/mocks/vb2api.c
ui_broken-test-srcs += tests/mocks/ui_callbacks.c
ui_broken-test-srcs += tests/stubs/boot/payload.c
ui_broken-test-srcs += tests/stubs/vboot/ui/archive.c
ui_broken-test-srcs += tests/stubs/vboot/ui/draw.c
ui_broken-test-srcs += tests/stubs/vboot/ui/layout.c
ui_broken-test-srcs += tests/stubs/vboot/ui/log.c
ui_broken-test-srcs += src/vboot/callbacks/ui.c
ui_broken-test-srcs += src/vboot/ui/loop.c
ui_broken-test-srcs += src/vboot/ui/menu.c
ui_broken-test-srcs += src/vboot/ui/navigation.c
ui_broken-test-srcs += src/vboot/ui/screens.c
ui_broken-test-mocks += vb2api_gbb_get_flags
ui_broken-test-mocks += vb2api_get_locale_id
ui_broken-test-mocks += vb2api_set_locale_id
ui_broken-test-mocks += vb2api_phone_recovery_ui_enabled
ui_broken-test-mocks += vb2api_diagnostic_ui_enabled
ui_broken-test-mocks += vb2api_enable_developer_mode
ui_broken-test-mocks += vb2api_request_diagnostics
ui_broken-test-mocks += vb2api_disable_developer_mode
ui_broken-test-mocks += vb2api_get_dev_default_boot_target

$(call copy-test,ui_broken-test,ui_broken-detachable-test)
ui_broken-detachable-test-config += CONFIG_DETACHABLE=1

ui_manual-test-srcs += tests/vboot/callbacks/ui_manual-test.c
ui_manual-test-srcs += tests/mocks/vb2api.c
ui_manual-test-srcs += tests/mocks/ui_callbacks.c
ui_manual-test-srcs += tests/mocks/vboot_api.c
ui_manual-test-srcs += tests/stubs/boot/payload.c
ui_manual-test-srcs += tests/stubs/vboot/ui/archive.c
ui_manual-test-srcs += tests/stubs/vboot/ui/draw.c
ui_manual-test-srcs += tests/stubs/vboot/ui/layout.c
ui_manual-test-srcs += tests/stubs/vboot/ui/log.c
ui_manual-test-srcs += src/vboot/callbacks/debug.c
ui_manual-test-srcs += src/vboot/ui/loop.c
ui_manual-test-srcs += src/vboot/ui/menu.c
ui_manual-test-srcs += src/vboot/ui/navigation.c
ui_manual-test-srcs += src/vboot/ui/screens.c
ui_manual-test-mocks += ui_get_screen_info
ui_manual-test-mocks += vb2api_gbb_get_flags
ui_manual-test-mocks += vb2api_get_locale_id
ui_manual-test-mocks += vb2api_set_locale_id
ui_manual-test-mocks += vb2api_phone_recovery_ui_enabled
ui_manual-test-mocks += vb2api_diagnostic_ui_enabled
ui_manual-test-mocks += vb2api_enable_developer_mode
ui_manual-test-mocks += vb2api_request_diagnostics

$(call copy-test,ui_manual-test,ui_manual-detachable-test)
ui_manual-detachable-test-config += CONFIG_DETACHABLE=1

$(call copy-test,ui_manual-test,ui_manual-ppkeyboard-test)
ui_manual-ppkeyboard-test-config += CONFIG_PHYSICAL_PRESENCE_KEYBOARD=1

ui_developer-test-srcs += tests/vboot/callbacks/ui_developer-test.c
ui_developer-test-srcs += tests/mocks/ui_callbacks.c
ui_developer-test-srcs += tests/mocks/vboot_api.c
ui_developer-test-srcs += tests/mocks/vb2api.c
ui_developer-test-srcs += tests/stubs/boot/payload.c
ui_developer-test-srcs += tests/stubs/vboot/ui/archive.c
ui_developer-test-srcs += tests/stubs/vboot/ui/draw.c
ui_developer-test-srcs += tests/stubs/vboot/ui/layout.c
ui_developer-test-srcs += tests/stubs/vboot/ui/log.c
ui_developer-test-srcs += src/vboot/callbacks/ui.c
ui_developer-test-srcs += src/vboot/callbacks/debug.c
ui_developer-test-srcs += src/vboot/ui/loop.c
ui_developer-test-srcs += src/vboot/ui/menu.c
ui_developer-test-srcs += src/vboot/ui/navigation.c
ui_developer-test-srcs += src/vboot/ui/screens.c
ui_developer-test-mocks += vb2api_request_diagnostics
ui_developer-test-mocks += vb2api_enable_developer_mode
ui_developer-test-mocks += vb2api_get_locale_id
ui_developer-test-mocks += vb2api_set_locale_id
ui_developer-test-mocks += vb2api_phone_recovery_ui_enabled
ui_developer-test-mocks += vb2api_diagnostic_ui_enabled
ui_developer-test-mocks += vb2api_get_dev_default_boot_target
ui_developer-test-mocks += vb2api_gbb_get_flags
ui_developer-test-mocks += vb2api_disable_developer_mode

$(call copy-test,ui_developer-test,ui_developer-detachable-test)
ui_developer-detachable-test-config += CONFIG_DETACHABLE=1

ui_diagnostic-test-srcs += tests/vboot/callbacks/ui_diagnostic-test.c
ui_diagnostic-test-srcs += tests/mocks/ui_callbacks.c
ui_diagnostic-test-srcs += tests/mocks/vboot_api.c
ui_diagnostic-test-srcs += tests/mocks/vb2api.c
ui_diagnostic-test-srcs += tests/stubs/boot/payload.c
ui_diagnostic-test-srcs += tests/stubs/vboot/ui/archive.c
ui_diagnostic-test-srcs += tests/stubs/vboot/ui/draw.c
ui_diagnostic-test-srcs += tests/stubs/vboot/ui/layout.c
ui_diagnostic-test-srcs += tests/stubs/vboot/ui/log.c
ui_diagnostic-test-srcs += src/vboot/callbacks/ui.c
ui_diagnostic-test-srcs += src/vboot/callbacks/debug.c
ui_diagnostic-test-srcs += src/vboot/ui/loop.c
ui_diagnostic-test-srcs += src/vboot/ui/menu.c
ui_diagnostic-test-srcs += src/vboot/ui/navigation.c
ui_diagnostic-test-srcs += src/vboot/ui/screens.c
ui_diagnostic-test-mocks += vb2api_request_diagnostics
ui_diagnostic-test-mocks += vb2api_enable_developer_mode
ui_diagnostic-test-mocks += vb2api_get_locale_id
ui_diagnostic-test-mocks += vb2api_set_locale_id
ui_diagnostic-test-mocks += vb2api_phone_recovery_ui_enabled
ui_diagnostic-test-mocks += vb2api_diagnostic_ui_enabled
ui_diagnostic-test-mocks += vb2api_get_dev_default_boot_target
ui_diagnostic-test-mocks += vb2api_gbb_get_flags
ui_diagnostic-test-mocks += vb2api_disable_developer_mode
ui_diagnostic-test-mocks += vb2api_use_short_dev_screen_delay