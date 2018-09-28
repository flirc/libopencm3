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

#ifndef SAMDX_NVMCTRL_H
#define SAMDX_NVMCTRL_H

#include <stdint.h>
#include <libopencm3/sam/d/bitfield.h>

#define FLASH_ADDR					0x00000000
#define USER_ROW_BASE					0x00804000
#define CALBIBRATION_BASE				0x00800000
#define SAMD21_NVMCTRL_BASE				0x41004000

/* Nonvolatile Memory Controller */

/* generic clock controller */
struct samd21_nvmctrl {
	/* 0x0  */ uint16_t ctrla;
	/* 0x2  */ uint8_t reserved1[2];
	/* 0x4  */ uint32_t ctrlb;
	/* 0x8  */ uint32_t param;
	/* 0xC  */ uint8_t intenclr;
	/* 0xD  */ uint8_t reserved2[3];
	/* 0x10 */ uint8_t intenset;
	/* 0x11 */ uint8_t reserved3[3];
	/* 0x14 */ uint8_t intflag;
	/* 0x15 */ uint8_t reserved4[3];
	/* 0x18 */ uint16_t status;
	/* 0x1A */ uint8_t reserved5[2];
	/* 0x1C */ uint32_t addr;
	/* 0x20 */ uint32_t lock;
} __packed __alias;

/* CTRLA - Control A */
#define NVMCTRL_CTRLA_CMD						7 : 0
#	define CTRLA_CMD_ER			0x02
#	define CTRLA_CMD_WP			0x04
#	define CTRLA_CMD_EAR			0x05
#	define CTRLA_CMD_WAP			0x06
#	define CTRLA_CMD_SF			0x0A
#	define CTRLA_CMD_WL			0x0F
#	define CTRLA_CMD_LR			0x40
#	define CTRLA_CMD_UR			0x41
#	define CTRLA_CMD_SPRM			0x42
#	define CTRLA_CMD_CPRM			0x43
#	define CTRLA_CMD_PBC			0x44
#	define CTRLA_CMD_SSB			0x45
#	define CTRLA_CMD_INVALL			0x46
#define NVMCTRL_CTRLA_CMDEX						8 : 8
#	define CTRLA_CMDEX_KEY			0xA5

/* CTRLB - Control B */
#define NVMCTRL_CTRLB_RWS						4 : 1
#define NVMCTRL_CTRLB_MANW						1 : 7
#define NVMCTRL_CTRLB_SLEEPPRM						2 : 8
#	define CTRLB_SLEEPPRM_WAKEONACCESS	0x00
#	define CTRLB_SLEEPPRM_WAKEUPINSTANT	0x01
#	define CTRLB_SLEEPPRM_DISABLED		0x03
#define NVMCTRL_CTRLB_READMODE						2 : 16
#	define CTRLB_READMODE_NO_MISS_PENALTY	0x00
#	define CTRLB_READMODE_LOW_POWER		0x01
#	define CTRLB_READMODE_DETERMINISTIC	0x02
#define NVMCTRL_CTRLB_CACHEDIS						1 : 18
#	define CTRLB_CACHE_DISABLED		0x01
#	define CTRLB_CACHE_ENABLED		0x00

/* PARAM - NVM Parameters */
#define NVMCTRL_PARAM_NVMP						16 : 0
#define NVMCTRL_PARAM_PSZ						3 : 16

/* INTENCLR - Interrupt Enable Clear */
#define NVMCTRL_INTENCLR_READY						1 : 0
#define NVMCTRL_INTENCLR_ERROR						1 : 1

/* INTENSET - Interrupt Enable Set */
#define NVMCTRL_INTENSET_READY						1 : 0
#define NVMCTRL_INTENSET_ERROR						1 : 1

/* INTFLAG - Interrupt Flag Status */
#define NVMCTRL_INTFLAG_READY						1 : 0
#define NVMCTRL_INTFLAG_ERROR						1 : 1

/* STATUS - Status */
#define NVMCTRL_STATUS_PRM						1 : 0
#define NVMCTRL_STATUS_LOAD						1 : 1
#define NVMCTRL_STATUS_PROGE						1 : 2
#define NVMCTRL_STATUS_LOCKE						1 : 3
#define NVMCTRL_STATUS_NVME						1 : 4
#define NVMCTRL_STATUS_SB						1 : 8

#define NVMUSER_ROW_BOOTPROT						3 : 0
#	define BOOTPROT_0			0x07
#	define BOOTPROT_512			0x06
#	define BOOTPROT_1K			0x05
#	define BOOTPROT_2K			0x04
#	define BOOTPROT_4K			0x03
#	define BOOTPROT_8K			0x02
#	define BOOTPROT_16K			0x01
#	define BOOTPROT_32K			0x00

#define CALIB_ADC_LINEARITY						8 : 27
#define CALIB_ADC_BIASCAL						3 : 35
#define CALIB_OSC32K_CAL						7 : 38
#define CALIB_USB_TRANSN						5 : 45
#define CALIB_USB_TRANSP						5 : 50
#define CALIB_USB_TRIM							3 : 55
#define CALIB_DFLL48M_COARSE_CAL					6 : 58
#define CALIB_DFLL48M_FINE_CAL						10 : 64

#define NVMCTRL ((volatile struct samd21_nvmctrl *)SAMD21_NVMCTRL_BASE)
#define CAL     ((volatile uint64_t *)CALBIBRATION_BASE)

void nvmctrl_setwaitstates(int waitstates);
int nvmctrl_init(uint8_t sleep_power_mode, int cache_readmode);
//int nvmctrl_islocked(int addr);
//int nvmctrl_lock_region(int addr);
//int nvmctrl_unlock_region(int addr);
int nvmctrl_erase_row(int addr);
//int nvmctrl_write_page(int addr, uint8_t *buffer);
int nvmctrl_write_row(int addr, uint8_t *buffer);
//int nvmctrl_pr_on(void);
//int nvmctrl_pr_off(void);

//int nvmctrl_get_numpages(void);
//int nvmctrl_get_pagesize(void);
//int nvmctrl_get_rowsize(void);
//int nvmctrl_get_regionsize(void);

#define NVMCTRL_ERR_BUSY		-1
#define NVMCTRL_ERR_BADCMD		-2
#define NVMCTRL_ERR_LOCKED		-3
#define NVMCTRL_ERR_PROGERASE		-4

#define MAX_PAGE_SIZE			64
#define MAX_ROW_SIZE			(MAX_PAGE_SIZE*4)

#endif /* SAMDX_NVMCTRL_H */
