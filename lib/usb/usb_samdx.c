/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2018  Flirc Inc.
 * Written by Gareth McMullin <gareth@blacksphere.co.nz>
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

#include <string.h>
#include <libopencm3/sam/d/bitfield.h>
#include <libopencm3/sam/d/usb.h>
#include <libopencm3/usb/usbd.h>
#include "usb_private.h"

static usbd_device *samd21_usbd_init(void);
static void samd21_set_address(usbd_device *usbd_dev, uint8_t addr);
static void samd21_ep_setup(usbd_device *usbd_dev, uint8_t addr,
			       uint8_t type, uint16_t max_size,
			       void (*callback) (usbd_device *usbd_dev,
						 uint8_t ep));
static void samd21_endpoints_reset(usbd_device *usbd_dev);
static void samd21_ep_stall_set(usbd_device *usbd_dev, uint8_t addr,
				   uint8_t stall);
static uint8_t samd21_ep_stall_get(usbd_device *usbd_dev, uint8_t addr);
static void samd21_ep_nak_set(usbd_device *usbd_dev, uint8_t addr,
				 uint8_t nak);
static uint16_t samd21_ep_write_packet(usbd_device *usbd_dev, uint8_t addr,
					  const void *buf, uint16_t len);
static uint16_t samd21_ep_read_packet(usbd_device *usbd_dev, uint8_t addr,
					 void *buf, uint16_t len);
static void samd21_poll(usbd_device *usbd_dev);
static void samd21_disconnect(usbd_device *usbd_dev, bool disconnected);

static uint8_t force_nak[8];
static struct _usbd_device usbd_dev;

static struct samd21_usb_desc samd21_ep_desc[8][2];

static uint8_t endpoint_ram[512];

const struct _usbd_driver samd21_usb_driver = {
	.init = samd21_usbd_init,
	.set_address = samd21_set_address,
	.ep_setup = samd21_ep_setup,
	.ep_reset = samd21_endpoints_reset,
	.ep_stall_set = samd21_ep_stall_set,
	.ep_stall_get = samd21_ep_stall_get,
	.ep_nak_set = samd21_ep_nak_set,
	.ep_write_packet = samd21_ep_write_packet,
	.ep_read_packet = samd21_ep_read_packet,
	.poll = samd21_poll,
	.disconnect = samd21_disconnect,
};

/** Initialize the USB device controller hardware of the STM32. */
static usbd_device *samd21_usbd_init(void)
{
	/* Set to device mode */
	INSERTBF(USB_CTRLA_MODE, 0, USB->ctrla);
	/* Enable USB peripheral */
	INSERTBF(USB_CTRLA_ENABLE, 1, USB->ctrla);

	USB->descadd = (uint32_t)&samd21_ep_desc;

	/* Enable interrupts used here. */
	USB->intenset = BF(USB_INTENSET_SUSPEND, 1) |
	                BF(USB_INTENSET_SOF, 1) |
	                BF(USB_INTENSET_EORST, 1) |
	                BF(USB_INTENSET_WAKEUP, 1);

	return &usbd_dev;
}

static void samd21_set_address(usbd_device *dev, uint8_t addr)
{
	(void)dev;
	/* Set device address and enable. */
	INSERTBF(USB_DADD_DADD, addr, USB->dadd);
	INSERTBF(USB_DADD_ADDEN, 1, USB->dadd);
}

static void
samd21_ep_setup(usbd_device *dev, uint8_t addr, uint8_t type, uint16_t max_size,
               void (*callback) (usbd_device *usbd_dev, uint8_t ep))
{
	uint8_t dir = addr & 0x80;
	addr &= 0x7f;
	uint8_t size = 0;
	switch (max_size) {
		case 8: size = 0; break;
		case 16: size = 1; break;
		case 32: size = 2; break;
		case 64: size = 3; break;
		case 128: size = 3; break;
		default:
			asm("bkpt"); /* Bug in program! */
	}

	if (dir || (type == USB_ENDPOINT_ATTR_CONTROL)) { /* IN endpoint */
		if (callback) {
			dev->user_callback_ctr[addr][USB_TRANSACTION_IN] =
			    (void *)callback;
		}

		/* Setup DMA descriptor */
		samd21_ep_desc[addr][1].addr = &endpoint_ram[dev->pm_top];
		samd21_ep_desc[addr][1].pcksize = BF(USB_EP_PCKSIZE_SIZE, size);
		dev->pm_top += max_size;

		/* Reset DATA toggle */
		USB->ep[addr].statusclr = BF(USB_EPSTATUS_DTGLIN, 1);

		/* Enable interrupt */
		USB->ep[addr].intenset = BF(USB_EPINTFLAG_TRCPT1, 1);

		/* Configure endpoint type */
		USB->ep[addr].cfg = (USB->ep[addr].cfg & 0xF) |
		                    ((type+1) << 4);

	}

	if (dir == 0) { /* OUT endpoint */
		if (callback) {
			dev->user_callback_ctr[addr][USB_TRANSACTION_OUT] =
			    (void *)callback;
		}

		/* Setup DMA descriptor */
		samd21_ep_desc[addr][0].addr = &endpoint_ram[dev->pm_top];
		samd21_ep_desc[addr][0].pcksize = BF(USB_EP_PCKSIZE_SIZE, size) |
		                                  BF(USB_EP_PCKSIZE_MULTI_PACKET_SIZE, max_size);
		dev->pm_top += max_size;

		/* Reset DATA toggle */
		USB->ep[addr].statusclr = BF(USB_EPSTATUS_DTGLOUT, 1);

		/* Enable interrupt */
		USB->ep[addr].intenset = BF(USB_EPINTFLAG_TRCPT0, 1);

		/* Configure endpoint type */
		USB->ep[addr].cfg = (USB->ep[addr].cfg & 0xF0) |
		                    (type+1);
	}

	if (type == USB_ENDPOINT_ATTR_CONTROL)
		USB->ep[addr].intenset = BF(USB_EPINTFLAG_RXSTP, 1);

}

static void samd21_endpoints_reset(usbd_device *dev)
{
	int i;
	for (i = 1; i < 8; i++)
		USB->ep[i].cfg = 0;

	dev->pm_top = (2 * dev->desc->bMaxPacketSize0);
}

static void samd21_ep_stall_set(usbd_device *dev, uint8_t addr, uint8_t stall)
{
	(void)dev;

	if ((addr & 0x80) || (addr == 0)) {
		addr &= 0x7F;

		if (stall) {
			USB->ep[addr].statusset = BF(USB_EPSTATUS_STALLRQ1, 1);
		} else {
			/* Reset to DATA0 if clearing stall condition. */
			USB->ep[addr].statusclr = BF(USB_EPSTATUS_STALLRQ1, 1) |
			                          BF(USB_EPSTATUS_DTGLIN, 1);
		}
	}
	if ((addr & 0x80) == 0) {
		if (stall) {
			USB->ep[addr].statusset = BF(USB_EPSTATUS_STALLRQ0, 1);
		} else {
			/* Reset to DATA0 if clearing stall condition. */
			USB->ep[addr].statusclr = BF(USB_EPSTATUS_STALLRQ0, 1) |
			                          BF(USB_EPSTATUS_DTGLOUT, 1);
		}
	}
}

static uint8_t samd21_ep_stall_get(usbd_device *dev, uint8_t addr)
{
	(void)dev;

	if (addr & 0x80) {
		return USB->ep[addr].status & BF(USB_EPSTATUS_STALLRQ1, 1);
	} else {
		return USB->ep[addr].status & BF(USB_EPSTATUS_STALLRQ0, 1);
	}

	return 0;
}

static void samd21_ep_nak_set(usbd_device *dev, uint8_t addr, uint8_t nak)
{
	(void)dev;
	/* It does not make sence to force NAK on IN endpoints. */
	if (addr & 0x80) {
		return;
	}

	force_nak[addr] = nak;

	if (nak) {
		USB->ep[addr].statusset = BF(USB_EPSTATUS_BK0RDY, 1);
	} else {
		USB->ep[addr].statusclr = BF(USB_EPSTATUS_BK0RDY, 1);
	}
}

static uint16_t
samd21_ep_write_packet(usbd_device *dev,
                       uint8_t addr, const void *buf, uint16_t len)
{
	(void)dev;
	addr &= 0x7F;

	if (GETBF(USB_EPSTATUS_BK1RDY, USB->ep[addr].status))
		return 0;

	memcpy(samd21_ep_desc[addr][1].addr, buf, len);
	INSERTBF(USB_EP_PCKSIZE_MULTI_PACKET_SIZE, 0, samd21_ep_desc[addr][1].pcksize);
	INSERTBF(USB_EP_PCKSIZE_BYTE_COUNT, len, samd21_ep_desc[addr][1].pcksize);
	INSERTBF(USB_EPSTATUS_BK1RDY, 1, USB->ep[addr].statusset);

	return len;
}

static uint16_t
samd21_ep_read_packet(usbd_device *dev, uint8_t addr, void *buf, uint16_t len)
{
	(void)dev;
	if (!GETBF(USB_EPSTATUS_BK0RDY, USB->ep[addr].status))
		return 0;

	len = MIN(GETBF(USB_EP_PCKSIZE_BYTE_COUNT, samd21_ep_desc[addr][0].pcksize), len);
	INSERTBF(USB_EP_PCKSIZE_BYTE_COUNT, 0, samd21_ep_desc[addr][0].pcksize);

	memcpy(buf, samd21_ep_desc[addr][0].addr, len);
	if (!force_nak[addr])
		INSERTBF(USB_EPSTATUS_BK0RDY, 1, USB->ep[addr].statusclr);

	return len;
}

static void samd21_poll(usbd_device *dev)
{
	/* Read interrupt register */
	uint16_t intflag = USB->intflag;
	int i;

	/* Handle reset */
	if (GETBF(USB_INTFLAG_EORST, intflag)) {
		_usbd_reset(dev);
		USB->intflag = BF(USB_INTFLAG_EORST, 1);
	}
	/* Handle transfer complete */
	for (i = 0; i < 8; i++) {
		if (USB->ep[i].intflag == 0)
			continue;

		if (GETBF(USB_EPINTFLAG_RXSTP, USB->ep[i].intflag)) {
			/* Handle SETUP transaction */
			if (dev->user_callback_ctr[i][USB_TRANSACTION_SETUP])
				dev->user_callback_ctr[i][USB_TRANSACTION_SETUP] (dev, i);
			USB->ep[i].intflag = BF(USB_EPINTFLAG_RXSTP, 1);
		}
		if (GETBF(USB_EPINTFLAG_TRCPT1, USB->ep[i].intflag)) {
			/* Handle IN transaction */
			if (dev->user_callback_ctr[i][USB_TRANSACTION_IN])
				dev->user_callback_ctr[i][USB_TRANSACTION_IN] (dev, i);
			USB->ep[i].intflag = BF(USB_EPINTFLAG_TRCPT1, 1);
		}
		if (GETBF(USB_EPINTFLAG_TRCPT0, USB->ep[i].intflag)) {
			/* Handle OUT transaction */
			if (dev->user_callback_ctr[i][USB_TRANSACTION_OUT])
				dev->user_callback_ctr[i][USB_TRANSACTION_OUT] (dev, i);
			/* Is BK0RDY cleared by hardware? */
			USB->ep[i].intflag = BF(USB_EPINTFLAG_TRCPT0, 1);
		}
	}
	/* Handle suspend / wakeup */
	if (GETBF(USB_INTFLAG_SUSPEND, intflag)) {
		if (dev->user_callback_suspend)
			dev->user_callback_suspend();
		USB->intflag = BF(USB_INTFLAG_SUSPEND, 1);
	}
	if (GETBF(USB_INTFLAG_WAKEUP, intflag)) {
		if (dev->user_callback_resume)
			dev->user_callback_resume();
		USB->intflag = BF(USB_INTFLAG_WAKEUP, 1);
	}
	/* Handle start-of-frame */
	if (GETBF(USB_INTFLAG_SOF, intflag)) {
		if (dev->user_callback_sof)
			dev->user_callback_sof();
		USB->intflag = BF(USB_INTFLAG_SOF, 1);
	}
}

static void samd21_disconnect(usbd_device *dev, bool disconnected)
{
	(void)dev;
	if (disconnected) {
		INSERTBF(USB_CTRLB_DETACH, 1, USB->ctrlb);
	} else {
		INSERTBF(USB_CTRLB_DETACH, 0, USB->ctrlb);
	}
}
