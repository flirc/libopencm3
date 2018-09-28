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

#include <libopencm3/sam/d/bitfield.h>
#include <libopencm3/cm3/cortex.h>

#include <libopencm3/sam/d/gclk.h>
#include <libopencm3/sam/d/nvic.h>
#include <libopencm3/sam/d/pm.h>
#include <libopencm3/sam/d/uart.h>

void usart_reset(uint8_t usart)
{
	INSERTBF(UART_CTRLA_SWRST, 1, UART(usart)->ctrla);

	while (UART(usart)->syncbusy);
}

void usart_send_blocking(uint8_t usart, uint8_t c)
{
	while (!GETBF(UART_INTFLAG_DRE, UART(usart)->intflag));

	usart_send(usart, c);
}

void usart_send(uint8_t usart, uint8_t c)
{
	UART(usart)->data = c;
}

uint8_t usart_recv(uint8_t usart)
{
	return UART(usart)->data;
}

uint8_t usart_recv_blocking(uint8_t usart)
{
	while (!GETBF(UART_INTFLAG_RXC, UART(usart)->intflag));

	return UART(usart)->data;
}

static int uart_samples_pb(uint8_t usart)
{
	switch (GETBF(UART_CTRLA_SAMPR, UART(usart)->ctrla)) {
	case UART_SAMPR_16_ARITH:
	case UART_SAMPR_16_FRACT:
		return 16;
	case UART_SAMPR_8_ARITH:
	case UART_SAMPR_8_FRACT:
		return 8;
	case UART_SAMPR_3_ARITH:
		return 3;
	default:
		return 0;
	}
}

void usart_set_baudrate(uint8_t usart, uint32_t baud)
{
	uint64_t ratio, scale, baudc = 0;

	if (!usart){
		ratio = ((uart_samples_pb(usart) * (uint64_t)baud) << 32) /
			(get_periph_clk_speed(GCLK_ID_SERCOM0_CORE)*1000);
	}else {
		ratio = ((uart_samples_pb(usart) * (uint64_t)baud) << 32) /
			(get_periph_clk_speed(GCLK_ID_SERCOM1_CORE)*1000);
	}

	scale = ((uint64_t)1 << 32) - ratio;
	baudc = ((1<<16) * scale) >> 32;
	UART(usart)->baud = baudc;
}

void usart_enable_rx_interrupt(uint8_t usart)
{
	/* enable rx complete interrupt */
	INSERTBF(UART_INTENSET_RXC, 1, UART(usart)->intenset);
}

void usart_enable_tx_interrupt(uint8_t usart)
{
	/* kick the transmitter to restart interrupts */
	INSERTBF(UART_INTENSET_DRE, 1, UART(usart)->intenset);
}

void usart_disable_rx_interrupt(uint8_t usart)
{
	/* Disable tx interrupts. Putc will reenable. */
	INSERTBF(UART_INTENCLR_RXC, 1, UART(usart)->intenclr);
}

void usart_disable_tx_interrupt(uint8_t usart)
{
	/* Disable tx interrupts. Putc will reenable. */
	INSERTBF(UART_INTENCLR_DRE, 1, UART(usart)->intenclr);

	/* bug?, need to re-enable rx complete interrupt */
	INSERTBF(UART_INTENSET_RXC, 1, UART(usart)->intenset);
}

void usart_setup(uint8_t usart, uint32_t baud)
{
	INSERTBF(PM_APBCMASK_PAC2, 1, PM->apbcmask);
	INSERTBF(1 : usart + 2, 1, PM->apbcmask);

	usart_reset(usart);

	UART(usart)->ctrla = BF(UART_CTRLA_MODE, UART_MODE_INTCLK) |
		BF(UART_CTRLA_CMODE, UART_CMODE_ASYNC) |
		BF(UART_CTRLA_FORM, UART_FORM_FRAME) |
		BF(UART_CTRLA_RXPO, UART_RXPO_PAD1) |
		BF(UART_CTRLA_TXPO, UART_TXPO_PAD0) |
		BF(UART_CTRLA_SAMPR, UART_SAMPR_8_ARITH) |
		BF(UART_CTRLA_DORD, UART_DORD_LSB);

	UART(usart)->ctrlb = BF(UART_CTRLB_CHSIZE, UART_CHSIZE_8) |
		BF(UART_CTRLB_SBMODE, UART_SBMODE_1);

	usart_set_baudrate(usart, baud);

	/* enable cm level interrupt */
	if (!usart)
		nvic_enable_irq(NVIC_SERCOM0_IRQ);
	else
		nvic_enable_irq(NVIC_SERCOM1_IRQ);
}

void usart_enable(uint8_t usart, uint32_t baud)
{
	if (baud)
		usart_setup(usart, baud);

	INSERTBF(UART_CTRLB_RXEN, 1, UART(usart)->ctrlb);
	while (UART(usart)->syncbusy);

	INSERTBF(UART_CTRLB_TXEN, 1, UART(usart)->ctrlb);
	while (UART(usart)->syncbusy);

	INSERTBF(UART_CTRLA_ENABLE, 1, UART(usart)->ctrla);
	while (UART(usart)->syncbusy);
}

void usart_disable(uint8_t usart)
{
	INSERTBF(UART_CTRLB_RXEN, 0, UART(usart)->ctrlb);
	while (UART(usart)->syncbusy);

	INSERTBF(UART_CTRLB_TXEN, 0, UART(usart)->ctrlb);
	while (UART(usart)->syncbusy);

	INSERTBF(UART_CTRLA_ENABLE, 0, UART(usart)->ctrla);
	while (UART(usart)->syncbusy);
}

void usart_set_pads(uint8_t usart, uint8_t rx, uint8_t tx)
{
	INSERTBF(UART_CTRLA_TXPO, tx, UART(usart)->ctrla);
	INSERTBF(UART_CTRLA_RXPO, rx, UART(usart)->ctrla);
}

void usart_set_chsize(uint8_t usart, uint8_t size)
{
	INSERTBF(UART_CTRLB_CHSIZE, size, UART(usart)->ctrlb);
}

void usart_set_sbmode(uint8_t usart, uint8_t mode)
{
	INSERTBF(UART_CTRLB_SBMODE, mode, UART(usart)->ctrlb);
}

void usart_set_parity(uint8_t usart, uint8_t mode, uint8_t enable)
{
	INSERTBF(UART_CTRLB_PMODE, mode, UART(usart)->ctrlb);
	INSERTBF(UART_CTRLA_FORM, enable, UART(usart)->ctrla);
}
