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

#ifndef SAMDX_GCLK_H
#define SAMDX_GCLK_H

#include <stdint.h>
#include <libopencm3/sam/d/bitfield.h>

#define SAMD21_GCLK_BASE			0x40000C00

/* Generic Clock Controller */

struct gclk_hw {
	uint8_t gclk0;
	uint8_t gclk0_div;
	uint8_t gclk1;
	uint16_t gclk1_div;
	uint8_t gclk2;
	uint8_t gclk2_div;
	uint8_t gclk3;
	uint8_t gclk3_div;
	uint8_t gclk4;
	uint8_t gclk4_div;
	uint8_t gclk5;
	uint8_t gclk5_div;
	uint8_t gclk6;
	uint8_t gclk6_div;
	uint8_t gclk7;
	uint8_t gclk7_div;
};

/* generic clock controller */
struct samd21_gclk {
	/* 0x0 */ uint8_t ctrl;
	/* 0x1 */ uint8_t status;
	/* 0x2 */ uint8_t clkctrl_id; /* need 8 bit access */
	/* 0x3 */ uint8_t clkctrl;
	/* 0x4 */ uint32_t genctrl;
	/* 0x8 */ uint32_t gendiv;
} __packed __alias;

/* Control Bitfields */
#define GCLK_CTRL_SWRST							1 : 0

/* Status Bitfields */
#define GCLK_STATUS_SYNCBUSY						1 : 7

/* Clock Control Bitfields */
#define GCLK_CLKCTRL_ID							6 : 0
#	define GCLK_ID_DFLL48M			0x00
#	define GCLK_ID_FDPLL96M			0x01
#	define GCLK_ID_FDPLL96M_32khz		0x02
#	define GCLK_ID_WDT			0x03
#	define GCLK_ID_RTC			0x04
#	define GCLK_ID_EIC			0x05
#	define GCLK_ID_USB			0x06
#	define GCLK_ID_EVSYS_CHANNEL_0		0x07
#	define GCLK_ID_EVSYS_CHANNEL_1		0x08
#	define GCLK_ID_EVSYS_CHANNEL_2		0x09
#	define GCLK_ID_EVSYS_CHANNEL_3		0x0a
#	define GCLK_ID_EVSYS_CHANNEL_4		0x0b
#	define GCLK_ID_EVSYS_CHANNEL_5		0x0c
#	define GCLK_ID_EVSYS_CHANNEL_6		0x0d
#	define GCLK_ID_EVSYS_CHANNEL_7		0x0e
#	define GCLK_ID_EVSYS_CHANNEL_8		0x0f
#	define GCLK_ID_EVSYS_CHANNEL_9		0x10
#	define GCLK_ID_EVSYS_CHANNEL_10		0x11
#	define GCLK_ID_EVSYS_CHANNEL_11		0x12
#	define GCLK_ID_SERCOMX_SLOW		0x13
#	define GCLK_ID_SERCOM0_CORE		0x14
#	define GCLK_ID_SERCOM1_CORE		0x15
#	define GCLK_ID_SERCOM2_CORE		0x16
#	define GCLK_ID_SERCOM3_CORE		0x17
#	define GCLK_ID_SERCOM4_CORE		0x18
#	define GCLK_ID_SERCOM5_CORE		0x19
#	define GCLK_ID_TCC0			0x1A
#	define GCLK_ID_TCC1			0x1A
#	define GCLK_ID_TCC2			0x1B
#	define GCLK_ID_TC3			0x1B
#	define GCLK_ID_TC4			0x1C
#	define GCLK_ID_TC5			0x1C
#	define GCLK_ID_TC6			0x1D
#	define GCLK_ID_TC7			0x1D
#	define GCLK_ID_ADC			0x1E
#	define GCLK_ID_AC_DIG			0x1F
#	define GCLK_ID_AC_ANA			0x20
#	define GCLK_ID_DAC			0x21
#	define GCLK_ID_PTC			0x22
#	define GCLK_ID_I2S_0			0x23
#	define GCLK_ID_I2S_1			0x24
#define GCLK_CLKCTRL_GEN						4 : 0
#	define GCLK0				0x00
#	define GCLK1				0x01
#	define GCLK2				0x02
#	define GCLK3				0x03
#	define GCLK4				0x04
#	define GCLK5				0x05
#	define GCLK6				0x06
#	define GCLK7				0x07
#	define GCLK8				0x08
#define GCLK_CLKCTRL_CLKEN						1 : 6
#define GCLK_CLKCTRL_WRTLOCK						1 : 7

/* Clock Generator Control Bitfields */
#define GCLK_GENCTRL_ID							4 : 0
#define GCLK_GENCTRL_SRC						5 : 8
#	define SRC_XOSC				0x00
#	define SRC_GCLKIN			0x01
#	define SRC_GCLKGEN1			0x02
#	define SRC_OSCULP32K			0x03
#	define SRC_OSC32K			0x04
#	define SRC_XOSC32K			0x05
#	define SRC_OSC8M			0x06
#	define SRC_DFLL48M			0x07
#	define SRC_FDPLL96M			0x08
#define GCLK_GENCTRL_GENEN						1 : 16
/*
#	define GCLK0				0x00
#	define GCLK1				0x01
#	define GCLK2				0x02
#	define GCLK3				0x03
#	define GCLK4				0x04
#	define GCLK5				0x05
#	define GCLK6				0x06
#	define GCLK7				0x07
#	define GCLK8				0x08
*/
#define GCLK_GENCTRL_IDC						1 : 17
#define GCLK_GENCTRL_OOV						1 : 18
#define GCLK_GENCTRL_OE							1 : 19
#define GCLK_GENCTRL_DIVSEL						1 : 20
#define GCLK_GENCTRL_RUNSTDBY						1 : 21

/* Clock Generator Division */
#define GCLK_GENDIV_ID							4 : 0
#define GCLK_GENDIV_DIV							16 : 8
#	define GENDIV_DIV_DIV2			0x00 /* divide clock by 2 */
#	define GENDIV_DIV_DIV4			0x01 /* divide clock by 4 */
#	define GENDIV_DIV_DIV8			0x02 /* divide clock by 8 */
#	define GENDIV_DIV_DIV16			0x03 /* divide clock by 16 */
#	define GENDIV_DIV_DIV32			0x04 /* divide clock by 32 */
#	define GENDIV_DIV_DIV64			0x05 /* divide clock by 64 */
#	define GENDIV_DIV_DIV128		0x06 /* divide clock by 128 */
#	define GENDIV_DIV_DIV256		0x07 /* divide clock by 256 */
#	define GENDIV_DIV_DIV512		0x08 /* divide clock by 512 */
#	define GENDIV_DIV_DIV1K			0x09 /* divide clock by 1024 */
#	define GENDIV_DIV_DIV2K			0x0A /* divide clock by 2048 */
#	define GENDIV_DIV_DIV4K			0x0B /* divide clock by 4096 */
#	define GENDIV_DIV_DIV8K			0x0C /* divide clock by 8192 */
#	define GENDIV_DIV_DIV16K		0x0D /* divide clock by 16384 */
#	define GENDIV_DIV_DIV32K		0x0E /* divide clock by 32768 */
#	define GENDIV_DIV_DIV64K		0x0F /* divide clock by 65536 */
/* convert the GCLK_GENDIV value into a divisor number */
#define GCLK_DIV_VAL(x)				(2 << x)

enum system_osc32k_startup {
	/** Wait three clock cycles */
	SYSTEM_OSC32K_STARTUP_3,
	/** Wait four clock cycles */
	SYSTEM_OSC32K_STARTUP_4,
	/** Wait six clock cycles */
	SYSTEM_OSC32K_STARTUP_6,
	/** Wait ten clock cycles */
	SYSTEM_OSC32K_STARTUP_10,
	/** Wait 18 clock cycles */
	SYSTEM_OSC32K_STARTUP_18,
	/** Wait 34 clock cycles */
	SYSTEM_OSC32K_STARTUP_34,
	/** Wait 66 clock cycles */
	SYSTEM_OSC32K_STARTUP_66,
	/** Wait 130 clock cycles */
	SYSTEM_OSC32K_STARTUP_130,
};

#define GCLK	((volatile struct samd21_gclk *)SAMD21_GCLK_BASE)

/**
 * system_clk_init() initialize the global and systeml clocks. Currently
 * defaults to 48Mhz, but will be more dynamic in the future.
 *
 * This function requires no arguments.
 *
 * @return      None.
 */
void system_clk_init(void);

/**
 * set_gclk_source() Will connect a system clock to a global glock. The global
 * clocks are connected to peripherals. See gclk.h for all the available clocks.
 * The clock must be setup first, which is currently done in system_clk_init.
 *
 * @param  	gclk_id   - global clock id
 * @param  	source    - the clock source to connect to gclk_id
 *
 * @return      None.
 */
void set_gclk_source(uint8_t gclk_id, uint8_t source);

/**
 * set_gclk_div() Will set the divider for the global clock specified. Use
 * GCLK and divider definitions in gclk.h.
 *
 * @param  	gclk_id   - global clock id
 * @param  	div       - the divider
 *
 * @return      None.
 */
void set_gclk_div(uint8_t gclk_id, uint16_t div);

/**
 * set_periph_clk() Will connect a global clock to a peripheral and enable
 * that global clock. The peripheral must be enabled independently of this
 * clock.
 *
 * @param  	gclk_id    - global clock id
 * @param  	peripheral - peripheral
 *
 * @return      None.
 */
void set_periph_clk(uint8_t gclk_id, uint8_t peripheral);

/**
 * periph_clk_en() Enable the peripheral clock.
 *
 * @param  	peripheral - the peripheral to enable
 * @param  	enable	   - 1:Enable 0:Disable
 *
 * @return      None.
 */
void periph_clk_en(uint8_t peripheral, uint8_t enable);

/**
 * setup_dfll() Sets up the 48Mhz clock in open loop mode. Needed for USB
 * and will retrieve the calibration information from the factory set ROM.
 *
 * This function requires no arguments.
 *
 * @return      None.
 */
void setup_dfll(void);

/**
 * get_gclk_source() gets the clock source that is powering the gclk id
 *
 * @param	gclk_id - gclk you want to get the source for
 *
 * @return      gclk_source.
 */
uint8_t get_gclk_source(uint8_t gclk_id);

/**
 * get_periph_clk_gclk_id() gets the gclk id associated with the peripheral
 *
 * @param	peripheral - the peripheral clock id
 *
 * @return      gclock id
 */
uint8_t get_periph_clk_gclk_id(uint8_t peripheral);

/**
 * get_periph_clk_speed() gets the clock speed of the peripheral
 *
 * @param	peripheral - the peripheral clock id
 *
 * @return      gclock id
 */
uint32_t get_periph_clk_speed(uint8_t peripheral);

/**
 * gclk_init() sets up the clock with a bunch of settings
 *
 * @param	peripheral - the peripheral clock id
 *
 * @return      gclock id
 */
int gclk_init(struct gclk_hw *hw);

#endif /* SAMDX_GCLK_H */
