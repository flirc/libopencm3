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

#ifndef SAMDX_USB_H
#define SAMDX_USB_H

#include <stdint.h>

#define SAMD21_USB_BASE				0x41005000

/* USB_SOC */

struct samd21_usb {
	/* 0x00 C */ uint8_t ctrla;
	/* 0x01 C */ uint8_t _reserved0;
	/* 0x02 C */ uint8_t syncbusy;
	/* 0x03 C */ uint8_t qosctrl;
	/* 0x04 D */ uint32_t _reserved1;
	/* 0x08 D */ uint16_t ctrlb;
	/* 0x0A D */ uint8_t dadd;
	/* 0x0B D */ uint8_t _reserved2;
	/* 0x0C D */ uint8_t status;
	/* 0x0D C */ uint8_t fsmstatus;
	/* 0x0E D */ uint16_t _reserved3;
	/* 0x10 D */ uint16_t fnum;
	/* 0x12 D */ uint8_t _reserved4;
	/* 0x13 ? */ uint8_t _undocumented;
	/* 0x14 D */ uint16_t intenclr;
	/* 0x16 D */ uint16_t _reserved5;
	/* 0x18 D */ uint16_t intenset;
	/* 0x1A D */ uint16_t _reserved6;
	/* 0x1C D */ uint16_t intflag;
	/* 0x1E D */ uint16_t _reserved7;
	/* 0x20 D */ uint16_t epintsmry;
	/* 0x22 D */ uint16_t _reserved8;

	/* 0x24 C */ uint32_t descadd;
	/* 0x28 C */ uint16_t padcal;

	/* 0x2A to 0xFF */ uint8_t _padding[0xD6];

	/* 0x100 */
	struct {
		/* 0x1n0 D */ uint8_t cfg;
		/* 0x1n1 D */ uint8_t _reserved0;
		/* 0x1n2 D */ uint8_t _reserved1;
		/* 0x1n3 D */ uint8_t _reserved2;
		/* 0x1n4 D */ uint8_t statusclr;
		/* 0x1n5 D */ uint8_t statusset;
		/* 0x1n6 D */ uint8_t status;
		/* 0x1n7 D */ uint8_t intflag;
		/* 0x1n8 D */ uint8_t intenclr;
		/* 0x1n9 D */ uint8_t intenset;
		/* 0x1nA D */ uint16_t _reserved3;
		/* 0x1nC ? */ uint32_t _undocumented;
		/* 0x11x ? */ uint32_t padding[4];
	} __attribute((packed)) ep[];
} __attribute__((packed));

struct samd21_usb_desc {
	void * addr;
	uint32_t pcksize;
	uint32_t extreg;
	uint16_t status_bk;
	uint16_t reserved;
} __attribute__((packed));

#define USB ((volatile struct samd21_usb *)SAMD21_USB_BASE)

#define USB_CTRLA_SWRST		1 : 0
#define USB_CTRLA_ENABLE	1 : 1
#define USB_CTRLA_RUNSTBY	1 : 2
#define USB_CTRLA_MODE		1 : 7

#define USB_SYNCBUSY_SWRST	1 : 0
#define USB_SYNCBUSY_ENABLE	1 : 1

#define USB_QOSCTRL_CQOS	2 : 0
#define USB_QOSCTRL_DQOS	2 : 2

#define USB_PADCAL_TRANSP	5 : 0
#define USB_PADCAL_TRANSN	5 : 6
#define USB_PADCAL_TRIM		3 : 12

#define USB_CTRLB_DETACH	1 : 0
#define USB_CTRLB_UPRSM		1 : 1
#define USB_CTRLB_SPDCONF	2 : 2
#    define USB_SPDCONF_FS            0x00
#    define USB_SPDCONF_LS            0x01
#define USB_CTRLB_NREPLY	1 : 4
#define USB_CTRLB_GNAK		1 : 9
#define USB_CTRLB_LPMHD		2 : 10

#define USB_DADD_DADD		7 : 0
#define USB_DADD_ADDEN		1 : 7

#define USB_INTENCLR_SUSPEND	1 : 0
#define USB_INTENCLR_SOF	1 : 2
#define USB_INTENCLR_EORST	1 : 3
#define USB_INTENCLR_WAKEUP	1 : 4
#define USB_INTENCLR_EORSM	1 : 5
#define USB_INTENCLR_UPRSM	1 : 6
#define USB_INTENCLR_RAMACER	1 : 7
#define USB_INTENCLR_LPMNYET	1 : 8
#define USB_INTENCLR_LPMSUSP	1 : 9

#define USB_INTENSET_SUSPEND	1 : 0
#define USB_INTENSET_SOF	1 : 2
#define USB_INTENSET_EORST	1 : 3
#define USB_INTENSET_WAKEUP	1 : 4
#define USB_INTENSET_EORSM	1 : 5
#define USB_INTENSET_UPRSM	1 : 6
#define USB_INTENSET_RAMACER	1 : 7
#define USB_INTENSET_LPMNYET	1 : 8
#define USB_INTENSET_LPMSUSP	1 : 9

#define USB_INTFLAG_SUSPEND	1 : 0
#define USB_INTFLAG_SOF		1 : 2
#define USB_INTFLAG_EORST	1 : 3
#define USB_INTFLAG_WAKEUP	1 : 4
#define USB_INTFLAG_EORSM	1 : 5
#define USB_INTFLAG_UPRSM	1 : 6
#define USB_INTFLAG_RAMACER	1 : 7
#define USB_INTFLAG_LPMNYET	1 : 8
#define USB_INTFLAG_LPMSUSP	1 : 9

#define USB_EPSTATUS_DTGLOUT	1 : 0
#define USB_EPSTATUS_DTGLIN	1 : 1
#define USB_EPSTATUS_CURBK	1 : 2
#define USB_EPSTATUS_STALLRQ0	1 : 4
#define USB_EPSTATUS_STALLRQ1	1 : 5
#define USB_EPSTATUS_BK0RDY	1 : 6
#define USB_EPSTATUS_BK1RDY	1 : 7

#define USB_EPINTFLAG_TRCPT0	1 : 0
#define USB_EPINTFLAG_TRCPT1	1 : 1
#define USB_EPINTFLAG_TRFAIL0	1 : 2
#define USB_EPINTFLAG_TRFAIL1	1 : 3
#define USB_EPINTFLAG_RXSTP	1 : 4
#define USB_EPINTFLAG_STALL0	1 : 5
#define USB_EPINTFLAG_STALL1	1 : 5

#define USB_EP_PCKSIZE_BYTE_COUNT		14 : 0
#define USB_EP_PCKSIZE_MULTI_PACKET_SIZE	14 : 14
#define USB_EP_PCKSIZE_SIZE			 3 : 28
#define USB_EP_PCKSIZE_AUTO_ZLP			 1 : 31

#endif /* SAMDX_USB_H */
