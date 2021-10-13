/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _TESTS_VBOOT_COMMON_H
#define _TESTS_VBOOT_COMMON_H

#include <tests/test.h>
#include <mocks/callbacks.h>
#include <vboot_api.h>
#include <vb2_api.h>
#include <vboot/ui.h>

#define ASSERT_VB2_SUCCESS(expr) assert_int_equal((expr), VB2_SUCCESS)

/*
 * Add will_return to VbExIsShutdownRequested. VbExIsShutdownRequested will
 * 1. Return 0 for iterations - 1 times, and then
 * 2. Return 1 once.
 */
#define WILL_SHUTDOWN_IN(iterations) \
	do { \
		if ((iterations) > 1) \
			will_return_count(VbExIsShutdownRequested, 0, \
					  (iterations) - 1); \
		will_return(VbExIsShutdownRequested, 1); \
	} while (0)

/* Add return value to vb2ex_physical_presence_pressed. */
#define WILL_PRESS_PHYSICAL_PRESENCE(pressed) \
	will_return(vb2ex_physical_presence_pressed, (pressed))

#define _EXPECT_BEEP(_msec, _frequency, _expected_time, ...) \
	do { \
		will_return(vb2ex_beep, (_expected_time)); \
		if ((_msec) != MOCK_IGNORE) \
			expect_value(vb2ex_beep, msec, (_msec)); \
		else \
			expect_any(vb2ex_beep, msec); \
		if ((_frequency) != MOCK_IGNORE) \
			expect_value(vb2ex_beep, frequency, (_frequency)); \
		else \
			expect_any(vb2ex_beep, frequency); \
	} while (0)

/*
 * Expect a vb2ex_beep call with specified duration and frequency at the
 * specified time. The expected time in vb2ex_beep is checked using
 * ASSERT_TIME_RANGE. This macro supports variable length of parameters, for
 * example:
 *
 * EXPECT_BEEP(100, 200): Expect a 200Hz beep with 100ms duration
 * EXPECT_BEEP(100): Expect a beep with 100ms duration
 * EXPECT_BEEP(): Expect a beep with any duration and frequency
 */
#define EXPECT_BEEP(...) \
	_EXPECT_BEEP(__VA_ARGS__, MOCK_IGNORE, MOCK_IGNORE, MOCK_IGNORE)

/*
 * Time window for ASSERT_TIME_RANGE macro.
 * This value is used to deal with some random internal delays. For example,
 * using either > or >= can affect the time when the function is being called.
 * FUZZ_MS and ASSERT_TIME_RANGE can be used in order to avoid some hard-coded
 * delay value in tests.
 */
#define FUZZ_MS (3 * UI_KEY_DELAY_MS)

/*
 * Check if the value is no less than the expected time, but smaller than
 * (expected + FUZZ_MS).
 */
#define ASSERT_TIME_RANGE(value, expected) \
	do { \
		intmax_t _local_expected = (expected); \
		assert_in_range((value), \
				_local_expected, \
				_local_expected + FUZZ_MS - 1); \
	} while (0)

#endif /* _TESTS_VBOOT_COMMON_H */