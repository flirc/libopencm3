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

#ifndef SAMDX_UART_H
#define SAMDX_UART_H

#define SAMD21_UART_BASE				0x42000800

/* UART */

/* generic clock controller */
struct samd21_uart {
	/* 0x0  */ uint32_t ctrla;
	/* 0x1  */ uint32_t ctrlb;
	/* 0x2  */ uint8_t reserved1[4];
	/* 0xC  */ uint16_t baud;
	/* 0xE  */ uint8_t rxpl;
	/* 0xF  */ uint8_t reserved2[5];
	/* 0x14 */ uint8_t intenclr;
	/* 0x15 */ uint8_t reserved3;
	/* 0x16 */ uint8_t intenset;
	/* 0x17 */ uint8_t reserved4;
	/* 0x18 */ uint8_t intflag;
	/* 0x19 */ uint8_t reserved5;
	/* 0x1A */ uint16_t status;
	/* 0x1C */ uint32_t syncbusy;
	/* 0x20 */ uint8_t reserved6[8];
	/* 0x28 */ uint16_t data;
	/* 0x2A */ uint8_t reserved7[6];
	/* 0x30 */ uint8_t dbgctrl;
} __attribute__((packed));

/* Control A */
#define UART_CTRLA_SWRST						1 : 0
#define UART_CTRLA_ENABLE						1 : 1
#define UART_CTRLA_MODE							3 : 2
#	define UART_MODE_EXTCLK			0x0
#	define UART_MODE_INTCLK			0x1
#define UART_CTRLA_RUNSTDBY						1 : 7
#define UART_CTRLA_IBON							1 : 8
#define UART_CTRLA_SAMPR						3 : 13
#	define UART_SAMPR_16_ARITH		0x0
#	define UART_SAMPR_16_FRACT		0x1
#	define UART_SAMPR_8_ARITH		0x2
#	define UART_SAMPR_8_FRACT		0x3
#	define UART_SAMPR_3_ARITH		0x4
#define UART_CTRLA_TXPO							2 : 16
/* Transmit Data Pinout */
#	define UART_TXPO_PAD0			0x0
#	define UART_TXPO_PAD1			0x1
#define UART_CTRLA_RXPO							2 : 20
/* Receive Data Pinout */
#	define UART_RXPO_PAD0			0x0
#	define UART_RXPO_PAD1			0x1
#	define UART_RXPO_PAD2			0x2
#	define UART_RXPO_PAD3			0x3
#define UART_CTRLA_SAMPA						2 : 22
/* 16x Over-sampling (CTRLA.SAMPR=0 or 1) */
#	define UART_SAMPA_7_8_9			0x00
#	define UART_SAMPA_9_10_11		0x01
#	define UART_SAMPA_11_12_13		0x02
#	define UART_SAMPA_13_14_15		0x03
/* 8x Over-sampling (CTRLA.SAMPR=2 or 3) */
#	define UART_SAMPA_3_4_5			0x00
#	define UART_SAMPA_4_5_6			0x01
#	define UART_SAMPA_5_6_7			0x02
#	define UART_SAMPA_6_7_8			0x03
#define UART_CTRLA_FORM							4 : 24
#	define UART_FORM_FRAME			0x00
#	define UART_FORM_FRAME_PAR		0x01
#	define UART_FORM_BREAK			0x04
#	define UART_FORM_BREAK_PAR		0x05
#define UART_CTRLA_CMODE						1 : 28
#	define UART_CMODE_ASYNC			0x0
#	define UART_CMODE_SYNC			0x1
#define UART_CTRLA_CPOL							1 : 29
#	define UART_CPOL_RISE			0x00
#	define UART_CPOL_FALL			0x01
#define UART_CTRLA_DORD							1 : 30
#	define UART_DORD_MSB			0x00
#	define UART_DORD_LSB			0x01

/* Control B */
#define UART_CTRLB_CHSIZE						3 : 0
#	define UART_CHSIZE_8			0x00
#	define UART_CHSIZE_9			0x01
#	define UART_CHSIZE_5			0x05
#	define UART_CHSIZE_6			0x06
#	define UART_CHSIZE_7			0x07
#define UART_CTRLB_SBMODE						1 : 6
#	define UART_SBMODE_1			0x00
#	define UART_SBMODE_2			0x01
#define UART_CTRLB_COLDEN						1 : 8
#define UART_CTRLB_SFDE							1 : 9
#define UART_CTRLB_ENC							1 : 10
#define UART_CTRLB_PMODE						1 : 13
#define UART_CTRLB_TXEN							1 : 16
#define UART_CTRLB_RXEN							1 : 17

/* Baud */
#define UART_BAUD_BAUD							13 : 0
#define UART_BAUD_FP							3 : 13

/* RXPL */
#define UART_RXPL							8 : 0

/* Interrupt Enable Clear */
#define UART_INTENCLR_DRE						1 : 0
#define UART_INTENCLR_TXC						1 : 1
#define UART_INTENCLR_RXC						1 : 2
#define UART_INTENCLR_RXS						1 : 3
#define UART_INTENCLR_CTSIC						1 : 4
#define UART_INTENCLR_RXBRK						1 : 5
#define UART_INTENCLR_ERROR						1 : 7

/* Interrupt Enable Set */
#define UART_INTENSET_DRE						1 : 0
#define UART_INTENSET_TXC						1 : 1
#define UART_INTENSET_RXC						1 : 2
#define UART_INTENSET_RXS						1 : 3
#define UART_INTENSET_CTSIC						1 : 4
#define UART_INTENSET_RXBRK						1 : 5
#define UART_INTENSET_ERROR						1 : 7

/* Interrupt Flag Status and Clear */
#define UART_INTFLAG_DRE						1 : 0
#define UART_INTFLAG_TXC						1 : 1
#define UART_INTFLAG_RXC						1 : 2
#define UART_INTFLAG_RXS						1 : 3
#define UART_INTFLAG_CTSIC						1 : 4
#define UART_INTFLAG_RXBRK						1 : 5
#define UART_INTFLAG_ERROR						1 : 7

/* Status */
#define UART_STATUS_PERR						1 : 0
#define UART_STATUS_FERR						1 : 1
#define UART_STATUS_BUFOVF						1 : 2
#define UART_STATUS_CTS							1 : 3
#define UART_STATUS_ISF							1 : 4
#define UART_STATUS_COLL						1 : 5

/* Syncbusy Register */
#define UART_SYNCBUSY_SWRST						1 : 0
#define UART_SYNCBUSY_ENABLE						1 : 1
#define UART_SYNCBUSY_CTRLB						1 : 2

/* Data */
#define UART_DATA							9 : 0

/* Debug Control */
#define UART_DBGCTRL_DBGSTOP						1 : 0

#define UART(x) ((volatile struct samd21_uart *)(SAMD21_UART_BASE+0x400*x))

void usart_reset(uint8_t usart);
void usart_set_baudrate(uint8_t usart, uint32_t baud);

void usart_enable_rx_interrupt(uint8_t usart);
void usart_disable_rx_interrupt(uint8_t usart);

void usart_enable_tx_interrupt(uint8_t usart);
void usart_disable_tx_interrupt(uint8_t usart);

void usart_setup(uint8_t usart, uint32_t baud);
void usart_enable(uint8_t usart, uint32_t baud);
void usart_disable(uint8_t usart);

void usart_send_blocking(uint8_t usart, uint8_t c);
void usart_send(uint8_t usart, uint8_t c);

uint8_t usart_recv(uint8_t usart);
uint8_t usart_recv_blocking(uint8_t usart);

void usart_set_chsize(uint8_t usart, uint8_t size);
void usart_set_sbmode(uint8_t usart, uint8_t mode);
void usart_set_parity(uint8_t usart, uint8_t mode, uint8_t enable);

void usart_set_pads(uint8_t usart, uint8_t rx, uint8_t tx);

#endif /* SAMDX_UART_H */
