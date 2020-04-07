/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright 2020 Google Inc.
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
 * Functions to display UI in the firmware screen.
 *
 * Currently libpayload only allows drawing within the canvas, which is the
 * maximal square drawing area located in the center of the screen. For example,
 * in landscape mode, the canvas stretches vertically to the edges of the
 * screen, leaving non-drawing areas on the left and right. In this case, the
 * canvas height will be the same as the screen height, but the canvas width
 * will be less than the screen width.
 *
 * As a result, all the offsets (x and y coordinates) appearing in the drawing
 * functions are relative to the canvas, not the screen.
 */

#ifndef __VBOOT_UI_H__
#define __VBOOT_UI_H__

#include <libpayload.h>
#include <vb2_api.h>

#define _UI_PRINT(fmt, args...) printf("%s: " fmt, __func__, ##args)
#define UI_INFO(...) _UI_PRINT(__VA_ARGS__)
#define UI_WARN(...) _UI_PRINT(__VA_ARGS__)
#define UI_ERROR(...) _UI_PRINT(__VA_ARGS__)

/*
 * This is the base used to specify the size and the coordinate of the image.
 * For example, height = 40 means 4.0% of the canvas height.
 */
#define UI_SCALE				1000

/*
 * UI_BOX_* constants define a large textbox taking up the width of the screen.
 * They are used for
 * (1) a fallback message in the case of a broken screen, and
 * (2) a main UI element for viewing a large body of text.
 */
#define UI_BOX_TEXT_HEIGHT			30
#define UI_BOX_V_MARGIN				275
#define UI_BOX_H_PADDING			20
#define UI_BOX_V_PADDING			15
#define UI_BOX_BORDER_THICKNESS			2
#define UI_BOX_BORDER_RADIUS			6

/* Indicate width or height is automatically set based on the other value */
#define UI_SIZE_AUTO				0

static const struct rgb_color ui_color_bg	= { 0x20, 0x21, 0x24 };
static const struct rgb_color ui_color_fg	= { 0xcc, 0xcc, 0xcc };

struct ui_bitmap {
	const void *data;
	size_t size;
};

struct ui_locale {
	const char *code;	/* Language code */
	int rtl;		/* Whether locale is right-to-left */
};

/******************************************************************************/
/* archive.c */

/*
 * Get locale information.
 *
 * This function will load the locale data on the first call.
 *
 * @return locale info on success, NULL on error or if the locale doesn't exist.
 */
const struct ui_locale *ui_get_locale_info(uint32_t locale);

/*
 * Get generic (locale-independent) bitmap.
 *
 * @param image_name	Image file name.
 * @param bitmap	Bitmap struct to be filled.
 *
 * @return VB2_SUCCESS on success, non-zero on error.
 */
vb2_error_t ui_get_bitmap(const char *image_name, struct ui_bitmap *bitmap);

/*
 * Get locale-dependent bitmap.
 *
 * @param image_name	Image file name.
 * @param locale	Locale.
 * @param bitmap	Bitmap struct to be filled.
 *
 * @return VB2_SUCCESS on success, non-zero on error.
 */
vb2_error_t ui_get_localized_bitmap(const char *image_name, uint32_t locale,
				    struct ui_bitmap *bitmap);

/* Character style. */
enum ui_char_style {
	UI_CHAR_STYLE_DEFAULT = 0,	/* foreground color */
	UI_CHAR_STYLE_DARK = 1,		/* darker color */
};

/*
 * Get character bitmap.
 *
 * @param c		Character.
 * @param style		Style of the character.
 * @param bitmap	Bitmap struct to be filled.
 *
 * @return VB2_SUCCESS on success, non-zero on error.
 */
vb2_error_t ui_get_char_bitmap(const char c, enum ui_char_style style,
			       struct ui_bitmap *bitmap);

/******************************************************************************/
/* draw.c */

/*
 * Get text width.
 *
 * @param text		Text.
 * @param height	Text height.
 * @param style		Style of the text.
 * @param width		Text width to be calculated.
 *
 * @return VB2_SUCCESS on success, non-zero on error.
 */
vb2_error_t ui_get_text_width(const char *text, int32_t height,
			      enum ui_char_style style, int32_t *width);

/*
 * Draw a line of text.
 *
 * @param text		Text to be drawn, which should contain only printable
 *			characters, including spaces, but excluding tabs.
 * @param x		x-coordinate of the top-left corner.
 * @param y		y-coordicate of the top-left corner.
 * @param height	Height of the text.
 * @param flags		Flags passed to draw_bitmap() in libpayload.
 * @param style		Style of the text.
 * @param reverse	Whether to reverse the x-coordinate relative to the
 *			canvas.
 *
 * @return VB2_SUCCESS on success, non-zero on error.
 */
vb2_error_t ui_draw_text(const char *text, int32_t x, int32_t y,
			 int32_t height, uint32_t flags,
			 enum ui_char_style style, int reverse);

/*
 * Draw a box with rounded corners.
 *
 * @param x		x-coordinate of the top-left corner.
 * @param y		y-coordinate of the top-left corner.
 * @param width		Width of the box.
 * @param height	Height of the box.
 * @param rgb		Color of the box.
 * @param thickness	Thickness of the border of the box.
 * @param radius	Radius of the rounded corners.
 *
 * @return VB2_SUCCESS on success, non-zero on error.
 */
vb2_error_t ui_draw_rounded_box(int32_t x, int32_t y,
				int32_t width, int32_t height,
				const struct rgb_color *rgb,
				uint32_t thickness, uint32_t radius);

/******************************************************************************/
/* screens.c */

/* UI state for display. */
struct ui_state {
	enum vb2_screen screen;
	uint32_t locale;
};

struct ui_descriptor {
	/* Screen id */
	enum vb2_screen screen;
	/* Drawing function */
	vb2_error_t (*draw)(const struct ui_state *state);
	/* Fallback message */
	const char *mesg;
};

/*
 * Get UI descriptor of a screen.
 *
 * @param screen	Screen.
 *
 * @return UI descriptor on success, NULL on error.
 */
const struct ui_descriptor *ui_get_descriptor(enum vb2_screen screen);

/******************************************************************************/
/* common.c */

/*
 * Display the UI state on the screen.
 *
 * When part of the screen remains unchanged, screen redrawing should be kept as
 * minimal as possible.
 *
 * @param state		Current UI state.
 * @param prev_state	Previous UI state, or NULL if previous menu drawing was
 *			unsuccessful or there's no previous state.
 *
 * @return VB2_SUCCESS on success, non-zero on error.
 */
vb2_error_t ui_display_screen(struct ui_state *state,
			      const struct ui_state *prev_state);

#endif /* __VBOOT_UI_H__ */