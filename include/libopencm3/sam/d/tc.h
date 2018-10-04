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

#ifndef SAMDX_TC_H
#define SAMDX_TC_H

#define SAMD21_TC_BASE				0x42002000

/* generic clock controller */
struct samd21_tc_8b {
	/* 0x00 */ uint16_t ctrla;
	/* 0x02 */ uint16_t readreq;
	/* 0x04 */ uint8_t ctrlbclr;
	/* 0x05 */ uint8_t ctrlbset;
	/* 0x06 */ uint8_t ctrlc;
	/* 0x07 */ uint8_t reserved0;
	/* 0x08 */ uint8_t dbgctrl;
	/* 0x09 */ uint8_t reserved1;
	/* 0x0A */ uint16_t evctrl;
	/* 0x0C */ uint8_t intenclr;
	/* 0x0D */ uint8_t intenset;
	/* 0x0E */ uint8_t intflag;
	/* 0x0F */ uint8_t status;
	/* 0x10 */ uint8_t count;
	/* 0x11 */ uint8_t reserved2[3];
	/* 0x14 */ uint8_t per;
	/* 0x15 */ uint8_t reserved3[3];
	/* 0x18 */ uint8_t cc[2];
	/* 0x1A */ uint8_t reserved4[6];
} __attribute__((packed));

struct samd21_tc_16b {
	/* 0x00 */ uint16_t ctrla;
	/* 0x02 */ uint16_t readreq;
	/* 0x04 */ uint8_t ctrlbclr;
	/* 0x05 */ uint8_t ctrlbset;
	/* 0x06 */ uint8_t ctrlc;
	/* 0x07 */ uint8_t reserved0;
	/* 0x08 */ uint8_t dbgctrl;
	/* 0x09 */ uint8_t reserved1;
	/* 0x0A */ uint16_t evctrl;
	/* 0x0C */ uint8_t intenclr;
	/* 0x0D */ uint8_t intenset;
	/* 0x0E */ uint8_t intflag;
	/* 0x0F */ uint8_t status;
	/* 0x10 */ uint16_t count;
	/* 0x12 */ uint8_t reserved2[6];
	/* 0x18 */ uint16_t cc[2];
	/* 0x1C */ uint8_t reserved3[4];
} __attribute__((packed));

struct samd21_tc_32b {
	/* 0x00 */ uint16_t ctrla;
	/* 0x02 */ uint16_t readreq;
	/* 0x04 */ uint8_t ctrlbclr;
	/* 0x05 */ uint8_t ctrlbset;
	/* 0x06 */ uint8_t ctrlc;
	/* 0x07 */ uint8_t reserved0;
	/* 0x08 */ uint8_t dbgctrl;
	/* 0x09 */ uint8_t reserved1;
	/* 0x0A */ uint16_t evctrl;
	/* 0x0C */ uint8_t intenclr;
	/* 0x0D */ uint8_t intenset;
	/* 0x0E */ uint8_t intflag;
	/* 0x0F */ uint8_t status;
	/* 0x10 */ uint32_t count;
	/* 0x14 */ uint8_t reserved2[4];
	/* 0x18 */ uint32_t cc[2];
} __attribute__((packed));

/* Control A */
#define TC_CTRLA_SWRST                                                  1 : 0
#define TC_CTRLA_ENABLE                                                 1 : 1
#define TC_CTRLA_MODE                                                   2 : 2
#       define TC_CTRLA_MODE_16                         0x0
#       define TC_CTRLA_MODE_8                          0x1
#       define TC_CTRLA_MODE_32                         0x2
#define TC_CTRLA_WAVEGEN                                                2 : 5
#       define TC_WAVEGEN_NFRQ                          0x0
#       define TC_WAVEGEN_MFRQ                          0x1
#       define TC_WAVEGEN_NPWM                          0x2
#       define TC_WAVEGEN_MPWM                          0x3
#define TC_CTRLA_PRESCALER                                              3 : 8
#       define TC_PRESCALER_DIV1                        0x0
#       define TC_PRESCALER_DIV2                        0x1
#       define TC_PRESCALER_DIV4                        0x2
#       define TC_PRESCALER_DIV8                        0x3
#       define TC_PRESCALER_DIV16                       0x4
#       define TC_PRESCALER_DIV64                       0x5
#       define TC_PRESCALER_DIV256                      0x6
#       define TC_PRESCALER_DIV1024                     0x7
#define TC_CTRLA_RUNSTDBY                                               1 : 11
#define TC_CTRLA_PRESCSYNC                                              2 : 12
#       define TC_PRESCSYNC_GCLK                        0x0
#       define TC_PRESCSYNC_PRESC                       0x1
#       define TC_PRESCSYNC_RESYNC                      0x2

#define TC_READREQ_ADDR                                                 5 : 0
#define TC_READREQ_RCONT                                                1 : 14
#define TC_READREQ_RREQ                                                 1 : 15

#define TC_CTRLBCLR_DIR                                                 1 : 0
#define TC_CTRLBCLR_ONESHOT                                             1 : 2
#define TC_CTRLBCLR_CMD                                                 2 : 6
#       define TC_CMD_NONE                              0x0
#       define TC_CMD_RETRIGGER                         0x1
#       define TC_CMD_STOP                              0x2

#define TC_CTRLBSET_DIR                                                 1 : 0
#define TC_CTRLBSET_ONESHOT                                             1 : 2
#define TC_CTRLBSET_CMD                                                 2 : 6

#define TC_CTRLC_INVEN0                                                 1 : 0
#define TC_CTRLC_INVEN1                                                 1 : 1
#define TC_CTRLC_CPTEN0                                                 1 : 4
#define TC_CTRLC_CPTEN1                                                 1 : 5

#define TC_DBGCTRL_DBGRUN                                               1 : 0

#define TC_EVCTRL_EVACT                                                 3 : 0
#define TC_EVCTRL_TCINV                                                 1 : 4
#define TC_EVCTRL_TCEI                                                  1 : 5
#define TC_EVCTRL_OVFEO                                                 1 : 8
#define TC_EVCTRL_MCEO0                                                 1 : 12
#define TC_EVCTRL_MCEO1                                                 1 : 13

#define TC_INTENCLR_OVF                                                 1 : 0
#define TC_INTENCLR_ERR                                                 1 : 1
#define TC_INTENCLR_SYNCRDY                                             1 : 3
#define TC_INTENCLR_MC0                                                 1 : 4
#define TC_INTENCLR_MC1                                                 1 : 5

#define TC_INTENSET_OVF                                                 1 : 0
#define TC_INTENSET_ERR                                                 1 : 1
#define TC_INTENSET_SYNCRDY                                             1 : 3
#define TC_INTENSET_MC0                                                 1 : 4
#define TC_INTENSET_MC1                                                 1 : 5

#define TC_INTFLAG_OVF                                                  1 : 0
#define TC_INTFLAG_ERR                                                  1 : 1
#define TC_INTFLAG_SYNCRDY                                              1 : 3
#define TC_INTFLAG_MC0                                                  1 : 4
#define TC_INTFLAG_MC1                                                  1 : 5

#define TC_STATUS_STOP                                                  1 : 3
#define TC_STATUS_SLAVE                                                 1 : 4
#define TC_STATUS_SYNCBUSY                                              1 : 7

#define TC8(x) ((volatile struct samd21_tc_8b *)(SAMD21_TC_BASE+0x400*x))
#define TC16(x) ((volatile struct samd21_tc_16b *)(SAMD21_TC_BASE+0x400*x))
#define TC32(x) ((volatile struct samd21_tc_32b *)(SAMD21_TC_BASE+0x400*x))

#define TC(x) ((volatile struct samd21_tc_16b *)(SAMD21_TC_BASE+0x400*x))

void tc_reset(uint8_t tc);
void tc_enable(uint8_t tc, uint8_t e);
void tc_config_ctrla(uint8_t tc, uint8_t s, uint16_t flags);
void tc_config_ctrlb(uint8_t tc, uint8_t s, uint8_t flags);
void tc_config_ctrlc(uint8_t tc, uint8_t s, uint8_t flags);
int tc_get_count(uint8_t tc);
int tc_set_period(uint8_t tc, uint8_t per);
void tc_set_cc(uint8_t tc, uint8_t cc, uint32_t val);
void tc_enable_interrupt(uint8_t tc, uint8_t flags);
void tc_disable_interrupt(uint8_t tc, uint8_t flags);
int tc_interrupt_flag(uint8_t tc);
void tc_clr_interrupt(uint8_t tc, uint8_t flags);

#endif /* SAMDX_TC_H */
