/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2016 Karl Palsson <karlp@tweak.net.au>
 * Copyright (C) 2018 Jason Kotzin <jasonkotzin@gmail.com>
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

#pragma once

#include <libopencm3/cm3/common.h>
#include <libopencm3/sam/d/memorymap.h>
#include <libopencm3/sam/d/bitfield.h>

/* --- Convenience macros ------------------------------------------------ */

#define PORTA				(PORT_BASE + 0)
#define PORTB				(PORT_BASE + 0x80)
#define PORTC				(PORT_BASE + 0x80*2)

/* --- PORT registers ----------------------------------------------------- */

/* Direction register */
#define PORT_DIR(port)			MMIO32((port) + 0x0000)

/* Direction clear register */
#define PORT_DIRCLR(port)		MMIO32((port) + 0x0004)

/* Direction set register */
#define PORT_DIRSET(port)		MMIO32((port) + 0x0008)

/* Direction toggle register */
#define PORT_DIRTGL(port)		MMIO32((port) + 0x000c)

/* output register */
#define PORT_OUT(port)			MMIO32((port) + 0x0010)

/* output clear register */
#define PORT_OUTCLR(port)		MMIO32((port) + 0x0014)

/* output set register */
#define PORT_OUTSET(port)		MMIO32((port) + 0x0018)

/* output toggle register */
#define PORT_OUTTGL(port)		MMIO32((port) + 0x001c)

/* input register */
#define PORT_IN(port)			MMIO32((port) + 0x0020)

/* Control register */
#define PORT_CTRL(port)			MMIO32((port) + 0x0024)

/* Write configuration register */
#define PORT_WRCONFIG(port)		MMIO32((port) + 0x0028)

/* Peripheral multiplexing registers */
#define PORT_PMUX(port, n)		MMIO8((port) + 0x0030 + (n))

/* Pin configuration registers */
#define PORT_PINCFG(port, n)		MMIO8((port) + 0x0040 + (n))

/* --- GPIO Flags ------------------------------------------------------ */


/* Input Flags */
#define GPIO_IN_FLAG_PULLUP		(1 << 0)
#define GPIO_IN_FLAG_PULLDOWN		(0 << 1)
#define GPIO_OUT_FLAG_DEFAULT_HIGH	(1 << 0)

#define GPIO0				(1 << 0)
#define GPIO1				(1 << 1)
#define GPIO2				(1 << 2)
#define GPIO3				(1 << 3)
#define GPIO4				(1 << 4)
#define GPIO5				(1 << 5)
#define GPIO6				(1 << 6)
#define GPIO7				(1 << 7)
#define GPIO8				(1 << 8)
#define GPIO9				(1 << 9)
#define GPIO10				(1 << 10)
#define GPIO11				(1 << 11)
#define GPIO12				(1 << 12)
#define GPIO13				(1 << 13)
#define GPIO14				(1 << 14)
#define GPIO15				(1 << 15)
#define GPIO16				(1 << 16)
#define GPIO17				(1 << 17)
#define GPIO18				(1 << 18)
#define GPIO19				(1 << 19)
#define GPIO20				(1 << 20)
#define GPIO21				(1 << 21)
#define GPIO22				(1 << 22)
#define GPIO23				(1 << 23)
#define GPIO24				(1 << 24)
#define GPIO25				(1 << 25)
#define GPIO26				(1 << 26)
#define GPIO27				(1 << 27)
#define GPIO28				(1 << 28)
#define GPIO29				(1 << 29)
#define GPIO30				(1 << 30)
#define GPIO31				(1 << 31)
#define GPIO32				(1 << 32)
#define GPIO_ALL			0xFFFFFFFF

enum gpio_special_function {
	SOC_GPIO_PERIPH_A = 0,
	SOC_GPIO_PERIPH_B,
	SOC_GPIO_PERIPH_C,
	SOC_GPIO_PERIPH_D,
	SOC_GPIO_PERIPH_E,
	SOC_GPIO_PERIPH_F,
	SOC_GPIO_PERIPH_G,
	SOC_GPIO_PERIPH_H,
	SOC_GPIO_NONE,
};

/**
 * SOC_GPIO_PERIPH_A - EIC
 * SOC_GPIO_PERIPH_B - ADC/PTC
 * SOC_GPIO_PERIPH_C - SERCOM
 * SOC_GPIO_PERIPH_D - SERCOM_ALT
 * SOC_GPIO_PERIPH_E - TCC
 * SOC_GPIO_PERIPH_F - TC
 * SOC_GPIO_PERIPH_G - COM
 * SOC_GPIO_PERIPH_H - AC/GLCLK
 */

/* PMUXn - Peripheral Multiplexing */
#define GPIO_PMUX_PMUXE_MASK						(0xF << 0)
#define GPIO_PMUX_PMUXO_MASK						(0xF << 4)
#define PMUXEN				0x1

/* PINCFGn - Pin Configuration */
#define GPIO_PINCFG_PMUXEN						(1 << 0)
#define GPIO_PINCFG_INEN						(1 << 1)
#define GPIO_PINCFG_PULLEN						(1 << 2)
#define GPIO_PINCFG_DRVSTR						(1 << 6)

void gpio_set(uint32_t gpioport, uint32_t gpios);
void  gpio_clear(uint32_t gpioport, uint32_t gpios);
uint32_t gpio_get(uint32_t gpioport, uint32_t gpios);
void gpio_toggle(uint32_t gpioport, uint32_t gpios);
uint32_t gpio_port_read(uint32_t gpioport);
void gpio_port_write(uint32_t gpioport, uint32_t data);
void gpio_config_special(uint32_t port, uint32_t pin, enum gpio_special_function function);
void gpio_config_input(uint32_t port, uint32_t pin, uint32_t inflags);
void gpio_config_output(uint32_t port, uint32_t pin, uint32_t outflags);
