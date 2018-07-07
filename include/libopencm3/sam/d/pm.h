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

#include <stdint.h>
#include <libopencm3/sam/d/bitfield.h>

#ifndef SAMDX_PM_H
#define SAMDX_PM_H

#define SAMD21_PM_BASE				0x40000400

/* Power Manager */

/* generic clock controller */
struct samd21_pm {
	/* 0x0  */ uint8_t ctrl;
	/* 0x1  */ uint8_t sleep;
	/* 0x2  */ uint8_t reserved1[6];
	/* 0x8  */ uint8_t cpusel;
	/* 0x9  */ uint8_t apbasel;
	/* 0xA  */ uint8_t apbbsel;
	/* 0xB  */ uint8_t apbcsel;
	/* 0xC  */ uint8_t reserved2[8];
	/* 0x14 */ uint32_t ahbmask;
	/* 0x18 */ uint32_t apbamask;
	/* 0x1C */ uint32_t apbbmask;
	/* 0x20 */ uint32_t apbcmask;
	/* 0x24 */ uint8_t reserved3[16];
	/* 0x34 */ uint8_t intenclr;
	/* 0x35 */ uint8_t intenset;
	/* 0x36 */ uint8_t intflag;
	/* 0x37 */ uint8_t reserved4;
	/* 0x38 */ uint8_t rcause;
} __attribute__((packed));

/* Generic for APBA/APBB/APBC */
#define PM_DIV1					0x0
#define PM_DIV2					0x1
#define PM_DIV4					0x2
#define PM_DIV8					0x3
#define PM_DIV16				0x4
#define PM_DIV32				0x5
#define PM_DIV64				0x6
#define PM_DIV128				0x7

#define PM_ENABLE				0x1
#define PM_DISABLE				0x0

/* SLEEP - Sleep Mode */
#define PM_SLEEP_IDLE							2 : 0
#       define PM_CPU				0x0
#       define PM_AHB				0x1
#       define PM_APB				0x2

/* CPUSEL - CPU Select */
#define PM_CPUSEL_CPUDIV						3 : 0

/* APBASEL - APBA Clock Select */
#define PM_APBASEL_APBADIV						3 : 0

/* APBBSEL - APBB Clock Select */
#define PM_APBBSEL_APBBDIV						3 : 0

/* APBCSEL - APBC Clock Select */
#define PM_APBCSEL_APBCDIV						3 : 0

/* AHBMASK - AHB Mask */
#define PM_AHBMASK_HPB0							1 : 0
#define PM_AHBMASK_HPB1							1 : 1
#define PM_AHBMASK_HPB2							1 : 2
#define PM_AHBMASK_DSU							1 : 3
#define PM_AHBMASK_NVMCTRL						1 : 4
#define PM_AHBMASK_DMAC							1 : 5
#define PM_AHBMASK_USB							1 : 6

/* APBAMASK - APBA Mask */
#define PM_APBAMASK_PAC0						1 : 0
#define PM_APBAMASK_PM							1 : 1
#define PM_APBAMASK_SYSCTRL						1 : 2
#define PM_APBAMASK_GCLK						1 : 3
#define PM_APBAMASK_WDT							1 : 4
#define PM_APBAMASK_RTC							1 : 5
#define PM_APBAMASK_EIC							1 : 6

/* APBBMASK - APBB Mask */
#define PM_APBBMASK_PAC1						1 : 0
#define PM_APBBMASK_DSU							1 : 1
#define PM_APBBMASK_NVMCTRL						1 : 2
#define PM_APBBMASK_PORT						1 : 3
#define PM_APBBMASK_DMAC						1 : 4
#define PM_APBBMASK_USB							1 : 5

/* APBCMASK - APBC Mask */
#define PM_APBCMASK_PAC2						1 : 0
#define PM_APBCMASK_EVSYS						1 : 1
#define PM_APBCMASK_SERCOM0						1 : 2
#define PM_APBCMASK_SERCOM1						1 : 3
#define PM_APBCMASK_SERCOM2						1 : 4
#define PM_APBCMASK_SERCOM3						1 : 5
#define PM_APBCMASK_SERCOM4						1 : 6
#define PM_APBCMASK_SERCOM5						1 : 7
#define PM_APBCMASK_TCC0						1 : 8
#define PM_APBCMASK_TCC1						1 : 9
#define PM_APBCMASK_TCC2						1 : 10
#define PM_APBCMASK_TC3							1 : 11
#define PM_APBCMASK_TC4							1 : 12
#define PM_APBCMASK_TC5							1 : 13
#define PM_APBCMASK_TC6							1 : 14
#define PM_APBCMASK_TC7							1 : 15
#define PM_APBCMASK_ADC							1 : 16
#define PM_APBCMASK_AC							1 : 17
#define PM_APBCMASK_DAC							1 : 18
#define PM_APBCMASK_I2S							1 : 20

/* INTENCLR - Interrupt Enable Clear */
#define PM_INTENCLR_CKRDY						1 : 0
#define PM_INTENCLR_CFD							1 : 1

/* INTENSET - Interrupt Enable Set */
#define PM_INTENSET_CKRDY						1 : 0
#define PM_INTENSET_CFD							1 : 1

/* INTFLAG - Interrupt Flag Status Clear */
#define PM_INTFLAG_CKRDY						1 : 0
#define PM_INTFLAG_CFD							1 : 1

/* RCAUSE - Reset Cause */
#define PM_RCAUSE_POR							1 : 0
#define PM_RCAUSE_BOD12							1 : 1
#define PM_RCAUSE_BOD33							1 : 2
#define PM_RCAUSE_EXT							1 : 4
#define PM_RCAUSE_WDT							1 : 5
#define PM_RCAUSE_SYST							1 : 6

#define PM ((volatile struct samd21_pm *)SAMD21_PM_BASE)

#endif /* SAMDX_PM_H */
