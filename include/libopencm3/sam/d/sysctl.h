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

#ifndef SAMDX_SYSCTL_H
#define SAMDX_SYSCTL_H

#include <stdint.h>

#define SAMD21_SYSCTL_BASE			0x40000800

/* System Controller */

/* generic clock controller */
struct samd21_sysctl {
	/* 0x0  */ uint32_t intenclr;
	/* 0x4  */ uint32_t intenset;
	/* 0x8  */ uint32_t intflag;
	/* 0xC  */ uint32_t pclksr;
	/* 0x10 */ uint16_t xosc;
	/* 0x12 */ uint8_t reserved1;
	/* 0x13 */ uint8_t reserved2;
	/* 0x14 */ uint16_t xosc32k;
	/* 0x16 */ uint8_t reserved3;
	/* 0x17 */ uint8_t reserved4;
	/* 0x18 */ uint32_t osc32k;
	/* 0x1C */ uint8_t osculp32k;
	/* 0x1D */ uint8_t reserved5;
	/* 0x1E */ uint8_t reserved6;
	/* 0x1F */ uint8_t reserved7;
	/* 0x20 */ uint32_t osc8m;
	/* 0x24 */ uint16_t dfllctrl;
	/* 0x26 */ uint8_t reserved8;
	/* 0x27 */ uint8_t reserved9;
	/* 0x28 */ uint32_t dfllval;
	/* 0x2C */ uint32_t dfllmul;
	/* 0x30 */ uint8_t dfllsync;
	/* 0x31 */ uint8_t reserved10[3];
	/* 0x34 */ uint32_t bod33;
	/* 0x38 */ uint8_t reserved11[8];
	/* 0x40 */ uint32_t vref;
	/* 0x44 */ uint8_t dpllctrla;
	/* 0x45 */ uint8_t reserved12[3];
	/* 0x48 */ uint32_t dpllratio;
	/* 0x4C */ uint32_t dpllctrlb;
	/* 0x50 */ uint8_t dpllstatus;

} __packed __alias;

/* INTENCLR - Interrupt Enable Clear */
#define SYSCTL_INTENCLR_XOSCRDY						1 : 0
#define SYSCTL_INTENCLR_XOSC32KRDY					1 : 1
#define SYSCTL_INTENCLR_OSC32KRDY					1 : 2
#define SYSCTL_INTENCLR_OSC8MRDY					1 : 3
#define SYSCTL_INTENCLR_DFLLRDY						1 : 4
#define SYSCTL_INTENCLR_DFLLOOB						1 : 5
#define SYSCTL_INTENCLR_DFLLLCKF					1 : 6
#define SYSCTL_INTENCLR_DFLLLCKC					1 : 7
#define SYSCTL_INTENCLR_DFLLRCS						1 : 8
#define SYSCTL_INTENCLR_BOD33RDY					1 : 9
#define SYSCTL_INTENCLR_BOD33DET					1 : 10
#define SYSCTL_INTENCLR_B33SRDY						1 : 11
#define SYSCTL_INTENCLR_DPLLLCKR					1 : 15
#define SYSCTL_INTENCLR_DPLLLCKF					1 : 16
#define SYSCTL_INTENCLR_DPLLLTO						1 : 17

/* INTENSET - Interrupt Enable Set */
#define SYSCTL_INTENSET_XOSCRDY						1 : 0
#define SYSCTL_INTENSET_XOSC32KRDY					1 : 1
#define SYSCTL_INTENSET_OSC32KRDY					1 : 2
#define SYSCTL_INTENSET_OSC8MRDY					1 : 3
#define SYSCTL_INTENSET_DFLLRDY						1 : 4
#define SYSCTL_INTENSET_DFLLOOB						1 : 5
#define SYSCTL_INTENSET_DFLLLCKF					1 : 6
#define SYSCTL_INTENSET_DFLLLCKC					1 : 7
#define SYSCTL_INTENSET_DFLLRCS						1 : 8
#define SYSCTL_INTENSET_BOD33RDY					1 : 9
#define SYSCTL_INTENSET_BOD33DET					1 : 10
#define SYSCTL_INTENSET_B33SRDY						1 : 11
#define SYSCTL_INTENSET_DPLLLCKR					1 : 15
#define SYSCTL_INTENSET_DPLLLCKF					1 : 16
#define SYSCTL_INTENSET_DPLLLTO						1 : 17

/* INTFLAG - Interrupt Flag Status and Clear */
#define SYSCTL_INTFLAG_XOSCRDY						1 : 0
#define SYSCTL_INTFLAG_XOSC32KRDY					1 : 1
#define SYSCTL_INTFLAG_OSC32KRDY					1 : 2
#define SYSCTL_INTFLAG_OSC8MRDY						1 : 3
#define SYSCTL_INTFLAG_DFLLRDY						1 : 4
#define SYSCTL_INTFLAG_DFLLOOB						1 : 5
#define SYSCTL_INTFLAG_DFLLLCKF						1 : 6
#define SYSCTL_INTFLAG_DFLLLCKC						1 : 7
#define SYSCTL_INTFLAG_DFLLRCS						1 : 8
#define SYSCTL_INTFLAG_BOD33RDY						1 : 9
#define SYSCTL_INTFLAG_BOD33DET						1 : 10
#define SYSCTL_INTFLAG_B33SRDY						1 : 11
#define SYSCTL_INTFLAG_DPLLLCKR						1 : 15
#define SYSCTL_INTFLAG_DPLLLCKF						1 : 16
#define SYSCTL_INTFLAG_DPLLLTO						1 : 17

/* PCLKSR - Interrupt Flag Status and Clear */
#define SYSCTL_PCLKSR_XOSCRDY						1 : 0
#define SYSCTL_PCLKSR_XOSC32KRDY					1 : 1
#define SYSCTL_PCLKSR_OSC32KRDY						1 : 2
#define SYSCTL_PCLKSR_OSC8MRDY						1 : 3
#define SYSCTL_PCLKSR_DFLLRDY						1 : 4
#define SYSCTL_PCLKSR_DFLLOOB						1 : 5
#define SYSCTL_PCLKSR_DFLLLCKF						1 : 6
#define SYSCTL_PCLKSR_DFLLLCKC						1 : 7
#define SYSCTL_PCLKSR_DFLLRCS						1 : 8
#define SYSCTL_PCLKSR_BOD33RDY						1 : 9
#define SYSCTL_PCLKSR_BOD33DET						1 : 10
#define SYSCTL_PCLKSR_B33SRDY						1 : 11
#define SYSCTL_PCLKSR_DPLLLCKR						1 : 15
#define SYSCTL_PCLKSR_DPLLLCKF						1 : 16
#define SYSCTL_PCLKSR_DPLLLTO						1 : 17

/* XOSC - External Multipurpose Oscillator Control */
#define SYSCTL_XOSC_ENABLE						1 : 1
#define SYSCTL_XOSC_XTALEN						1 : 2
#define SYSCTL_XOSC_RUNSTDBY						1 : 6
#define SYSCTL_XOSC_ONDEMAND						1 : 7
#define SYSCTL_XOSC_GAIN						3 : 8
#define SYSCTL_XOSC_AMPGC						1 : 11
#define SYSCTL_XOSC_STARTUP						4 : 12

/* XOSC32K - 32kHz Crystal Oscillator (XOSC32K) Control */
#define SYSCTL_XOSC32K_ENABLE						1 : 1
#define SYSCTL_XOSC32K_XTALEN						1 : 2
#define SYSCTL_XOSC32K_EN32K						1 : 3
#define SYSCTL_XOSC32K_EN1K						1 : 4
#define SYSCTL_XOSC32K_AAMPEN						1 : 5
#define SYSCTL_XOSC32K_RUNSTDBY						1 : 6
#define SYSCTL_XOSC32K_ONDEMAND						1 : 7
#define SYSCTL_XOSC32K_STARTUP						3 : 8
#define SYSCTL_XOSC32K_WRTLOCK						1 : 12

/* OSC32K - 32kHz Internal Oscillator (OSC32K) Control */
#define SYSCTL_OSC32K_ENABLE						1 : 1
#define SYSCTL_OSC32K_EN32K						1 : 2
#define SYSCTL_OSC32K_EN1K						1 : 3
#define SYSCTL_OSC32K_RUNSTDBY						1 : 6
#define SYSCTL_OSC32K_ONDEMAND						1 : 7
#define SYSCTL_OSC32K_STARTUP						3 : 8
#define SYSCTL_OSC32K_WRTLOCK						1 : 12
#define SYSCTL_OSC32K_CALIB						7 : 16

/* OSCULP32K - 32kHz Ultra Low Power Internal Oscillator (OSCULP32K) Control */
#define SYSCTL_OSCULP32K_CALIB						5 : 0
#define SYSCTL_OSCULP32K_WRTLOCK					1 : 7

/* OSC8M - 8MHz Internal Oscillator Control */
#define SYSCTL_OSC8M_ENABLE						1 : 1
#define SYSCTL_OSC8M_RUNSTDBY						1 : 6
#define SYSCTL_OSC8M_ONDEMAND						1 : 7
#define SYSCTL_OSC8M_PRESC						2 : 8
#define SYSCTL_OSC8M_CALIB						12 : 16
#define SYSCTL_OSC8M_FRANGE						2 : 30

/* DFLLCTRL - DFLL48M Control */
#define SYSCTL_DFLLCTRL_ENABLE						1 : 1
#define SYSCTL_DFLLCTRL_MODE						1 : 2
#	define DFLLCTRL_MODE_CLOSED		1
#	define DFLLCTRL_MODE_OPEN		0
#define SYSCTL_DFLLCTRL_STABLE						1 : 3
#       define DFLLCTRL_STABLE_TRACK            0x00
#	define DFLLCTRL_STABLE_AFTER		0x01
#define SYSCTL_DFLLCTRL_LLAW						1 : 4
#	define DFLLCTRL_LLAW_KEEP		0x00
#	define DFLLCTRL_LLAW_LOST		0x01
#define SYSCTL_DFLLCTRL_USBCRM						1 : 5
#	define DFLLCTRL_USBCRM_DISABLE		0x00
#	define DFLLCTRL_USBCRM_ENABLE		0x01
#define SYSCTL_DFLLCTRL_RUNSTDBY					1 : 6
#define SYSCTL_DFLLCTRL_ONDEMAND					1 : 7
#define SYSCTL_DFLLCTRL_CCDIS						1 : 8
#	define DFLLCTRL_CCDIS_ENABLE		0x00
#	define DFLLCTRL_CCDIS_DISABLE		0x01
#define SYSCTL_DFLLCTRL_QLDIS						1 : 9
#	define DFLLCTRL_QLDIS_ENABLE		0x00
#	define DFLLCTRL_QLDIS_DISABLE		0x01
#define SYSCTL_DFLLCTRL_BPLCKC						1 : 10
#	define DFLLCTRL_BPLCKC_DISABLE		0x00
#	define DFLLCTRL_BPLCKC_ENABLE		0x01
#define SYSCTL_DFLLCTRL_WAITLOCK					1 : 11

/* DFLL open loop mode configuration */
#define CONF_CLOCK_DFLL_COARSE_VALUE            (0x1f / 4)
#define CONF_CLOCK_DFLL_FINE_VALUE              (0xff / 4)
#define NVMCTRL_OTP4                  		(0x00806020U)
#define NVM_DFLL_COARSE_POS    			58
#define NVM_DFLL_COARSE_SIZE   			6
#define NVM_DFLL_FINE_POS      			64
#define NVM_DFLL_FINE_SIZE     			10


/* DFLLVAL - DFLL48M Value */
#define SYSCTL_DFLLVAL_FINE						10 : 0
#define SYSCTL_DFLLVAL_COARSE						6 : 10
#define SYSCTL_DFLLVAL_DIFF						16 : 16

/* DFLLMUL - DFLL48M Multiplier */
#define SYSCTL_DFLLMUL_MUL						16 : 0
#	define DFLLMUL_MUL_DEFAULT		48000
#define SYSCTL_DFLLMUL_FSTEP						10 : 16
#define SYSCTL_DFLLMUL_CSTEP						6 : 26

/* DFLLSYNC - DFLL48M Synchronization */
#define SYSCTL_DFLLSYNC_READREQ						1 : 7

#define FUSES_OSC32K_CAL_ADDR       (NVMCTRL_OTP4 + 4)
#define FUSES_OSC32K_CAL_Pos        6
#define FUSES_OSC32K_CAL_Msk        (0x7Ful << FUSES_OSC32K_CAL_Pos)
#define FUSES_OSC32K_CAL(value)     \
	((FUSES_OSC32K_CAL_Msk & ((value) << FUSES_OSC32K_CAL_Pos)))

/* BOD33 - 3.3V Brown-Out Detector Control */
#define SYSCTL_BOD33_ENABLE						1 : 1
#define SYSCTL_BOD33_HYST						1 : 2
#define SYSCTL_BOD33_ACTION						2 : 3
#define SYSCTL_BOD33_RUNSTDBY						1 : 6
#define SYSCTL_BOD33_MODE						1 : 8
#define SYSCTL_BOD33_CEN						1 : 9
#define SYSCTL_BOD33_PSEL						4 : 12
#define SYSCTL_BOD33_LEVEL						6 : 16

/* VREF - Voltage References System Control */
#define SYSCTL_VREF_TSEN						1 : 1
#define SYSCTL_VREF_BGOUTEN						1 : 2
#define SYSCTL_VREF_CALIB						11 : 16

/* DPLLCTRLA - DPLL Control A */
#define SYSCTL_DPLLCTRLA_ENABLE						1 : 1
#define SYSCTL_DPLLCTRLA_RUNSTDBY					1 : 6
#define SYSCTL_DPLLCTRLA_ONDEMAND					1 : 7

/* DPLLRATIO - DPLL Ratio Control */
#define SYSCTL_DPLLRATIO_LDR						12 : 0
#define SYSCTL_DPLLRATIO_LDRFRAC					4 : 16

/* DPLLCTRLB - DPLL Control B */
#define SYSCTL_DPLLCTRLB_FILTER						2 : 0
#define SYSCTL_DPLLCTRLB_LPEN						1 : 2
#define SYSCTL_DPLLCTRLB_WUF						1 : 3
#define SYSCTL_DPLLCTRLB_REFCLK						2 : 4
#define SYSCTL_DPLLCTRLB_LTIME						3 : 8
#define SYSCTL_DPLLCTRLB_LBYPASS					1 : 12
#define SYSCTL_DPLLCTRLB_DIV						11 : 16

/* DPLLSTATUS - DPLL Status */
#define SYSCTL_DPLLSTATUS_LOCK						1 : 0
#define SYSCTL_DPLLSTATUS_CLKRDY					1 : 1
#define SYSCTL_DPLLSTATUS_ENABLE					1 : 2
#define SYSCTL_DPLLSTATUS_DIV						1 : 3

#define SYSCTL	((volatile struct samd21_sysctl *)SAMD21_SYSCTL_BASE)

#endif /* SAMDX_SYSCTL_H */
