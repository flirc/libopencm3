/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2018  Flirc Inc.
 * Written by Jason Kotzin <jasonkotzin@gmail.com>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef I__USHELL_PRIV_H__
#define I__USHELL_PRIV_H__

#define ESC		0x1B
#define UP_ARROW	0x5B

/* ASCII control character; for example, CTRL('C') = ^C */
#define CTRL(c) ((c) - '@')

/* State of input escape code */
static enum {
	ESC_OUTSIDE,   /* Not in escape code */
	ESC_START,     /* Got ESC */
	ESC_BAD,       /* Bad escape sequence */
	ESC_BRACKET,   /* Got ESC [ */
	ESC_BRACKET_1, /* Got ESC [ 1 */
	ESC_BRACKET_3, /* Got ESC [ 3 */
	ESC_O,         /* Got ESC O */
} esc_state;

/* Extended key code values, from multi-byte escape sequences */
enum extended_key_code {
	KEY_UP_ARROW = 0x100,
	KEY_DOWN_ARROW,
	KEY_RIGHT_ARROW,
	KEY_LEFT_ARROW,
	KEY_END,
	KEY_HOME,
	KEY_DEL
};

#ifdef CONSOLE_HISTORY_ENABLE
void load_history(int idx);
void save_history(void);
#endif

#endif /* I__SHELL_PRIV_H__ */
