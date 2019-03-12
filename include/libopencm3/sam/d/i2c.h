/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2019  Flirc Inc.
 * Written by Ryan Morrison <ryan@flirc.tv>
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

#ifndef SAMDX_I2C_H
#define SAMDX_I2C_H

/* Base address for all SERCOM devices */
#define SAMD21_I2C_BASE			0x42000800

#define I2C0 0x0

/* I2C Slave Registers */
struct samd21_i2c{

	/*0x00*/ uint32_t ctrla;
	/*0x04*/ uint32_t ctrlb;
	/*0x08*/ uint32_t reserved0;
	/*0x0C*/ uint32_t baud;
	/*0x10*/ uint32_t reserved1;
 	/*0x14*/ uint8_t intenclr;
	/*0x15*/ uint8_t reserved2;
	/*0x16*/ uint8_t intenset;
	/*0x17*/ uint8_t reserved3;
	/*0x18*/ uint8_t intflag;
	/*0x19*/ uint8_t reserved4;
	/*0x1A*/ uint16_t status;
	/*0x1C*/ uint32_t syncbusy;
	/*0x20*/ uint32_t reserved5;
	/*0x24*/ uint32_t addr;
	/*0x28*/ uint16_t data;
	/*0x2A*/ uint8_t reserved6[6];
	/*0x30*/ uint8_t dbgctrl;

}__attribute__((packed));

/* CTRLA Register */
#define I2C_CTRLA_SWRST				1 : 0
#define I2C_CTRLA_ENABLE			1 : 1
#define I2C_CTRLA_MODE				3 : 2
#	define I2C_MODE_MASTER		0x05
#	define I2C_MODE_SLAVE		0x04
#define I2C_CTRLA_RUNSTDBY			1 : 7
#define I2C_CTRLA_PINOUT			1 : 16
#define I2C_CTRLA_SDAHOLD			2 : 20
#	define I2C_HOLD_DISABLE		0x0
#	define I2C_HOLD_75		0x1
#	define I2C_HOLD_450		0x2
#	define I2C_HOLD_600		0x3
#define I2C_CTRLA_MEXTTOEN			1 : 22
#define I2C_CTRLA_SEXTTOEN			1 : 23
#define I2C_CTRLA_SPEED				2 : 24
#	define I2C_STANDARD_MODE	0x0
#	define I2C_FAST_MODE		0x1
#	define I2C_HIGHSPEED_MODE	0x2
#define I2C_CTRLA_SCLSM				1 : 27
#define I2C_CTRLA_INACTOUT			2 : 28
#	define I2C_TIMOUT_DIS		0x0
#	define I2C_TIMOUT_55US		0x1
#	define I2C_TIMOUT_105US		0x2
#	define I2C_TIMOUT_205US		0x3
#define I2C_CTRLA_LOWTOUT			1 : 30

/* CTRLB Register */
#define I2C_CTRLB_SMEN				1 : 8
#define I2C_CTRLB_GCMD				1 : 9
#define I2C_CTRLB_QCEN				1 : 9
#define I2C_CTRLB_AACKEN			1 : 10
#define I2C_CTRLB_AMODE				2 : 14
#	define I2C_AMODE_MASK		0x0
#	define I2C_AMODE_2_ADDRS	0x1
#	define I2C_AMODE_RANGE		0x2
#define I2C_CTRLB_CMD				2 : 16
#	define I2C_CMD_NONE		0x0
#	define I2C_CMD_START		0x1
#	define I2C_CMD_READ		0x2
#	define I2C_CMD_STOP		0x3
#define I2C_CTRLB_ACKACT			1 : 18
#	define I2C_ACK			0x0
#	define I2C_NACK			0x1

/* BAUD Register */
#define I2C_BAUD				8 : 0
#define I2C_BAUD_BAUDLOW			8 : 8
#define I2C_BAUD_HSBAUD				8 : 16
#define I2C_BAUD_HSBAUDLOW			8 : 24

/* INTENCLR Register */
#define I2C_INTENCLR_PREC			1 : 0
#define I2C_INTENCLR_MB				1 : 0
#define I2C_INTENCLR_AMATCH			1 : 1
#define I2C_INTENCLR_SB				1 : 1
#define I2C_INTENCLR_DRDY			1 : 2
#define I2C_INTENCLR_ERROR			1 : 7

/* INTENSET Register */
#define I2C_INTENSET_PREC			1 : 0
#define I2C_INTENSET_MB				1 : 0
#define I2C_INTENSET_AMATCH			1 : 1
#define I2C_INTENSET_SB				1 : 1
#define I2C_INTENSET_DRDY			1 : 2
#define I2C_INTENSET_ERROR			1 : 7

/* INTFLAG Register */
#define I2C_INTFLAG_PREC			1 : 0
#define I2C_INTFLAG_MB				1 : 0
#define I2C_INTFLAG_AMATCH			1 : 1
#define I2C_INTFLAG_SB				1 : 1
#define I2C_INTFLAG_DRDY			1 : 2
#define I2C_INTFLAG_ERROR			1 : 7

/* STATUS Register */
#define I2C_STATUS_BUSERR			1 : 0
#define I2C_STATUS_COLL				1 : 1
#define I2C_STATUS_ARBLOST			1 : 1
#define I2C_STATUS_RXNACK			1 : 2
#define I2C_STATUS_DIR				1 : 3
#define I2C_STATUS_SR				1 : 4
#define I2C_STATUS_BUSSTATE			2 : 4
#	define I2C_BUS_UNKNOWN		0x0
#	define I2C_BUS_IDLE		0x1
#	define I2C_BUS_OWNER		0x2
#	define I2C_BUS_BUSY		0x3
#define I2C_STATUS_LOWTOUT			1 : 6
#define I2C_STATUS_CLKHOLD			1 : 7
#define I2C_STATUS_MEXTTOUT			1 : 8
#define I2C_STATUS_SEXTTOUT			1 : 9
#define I2C_STATUS_HS				1 : 10
#define I2C_STATUS_LENERR			1 : 10

/* SYNCBUSY Register */
#define I2C_SYNCBUSY_SWRST			1 : 0
#define I2C_SYNCBUSY_ENABLE			1 : 1
#define I2C_SYNCBUSY_SYSOP			1 : 2

/* ADDR Register */
#define I2C_ADDR_GENCEN				1 : 0
#define I2C_ADDR_ADDR				10 : 1
#define I2C_ADDR_LENEN				1 : 13
#define I2C_ADDR_HS				1 : 14
#define I2C_ADDR_TENBITEN			1 : 15
#define I2C_ADDR_LEN				8 : 16
#define I2C_ADDR_ADDRMASK			10 : 17

/* DATA Register */
#define I2C_DATA				8 : 0

/* DBGCTRL Register */
#define I2C_DBGCTRL_DBGSTOP			1 : 0
#	define I2C_DBGSTOP_MODE_0	0x0
#	define I2C_DBGSTOP_MODE_1	0x1

#define I2C(x) ((volatile struct samd21_i2c*)(SAMD21_I2C_BASE+0x400*x))

void i2c_configure_master(uint8_t i2cx, uint8_t timeout, uint8_t baud);
void i2c_configure_slave(uint8_t i2cx, uint8_t address_mode, uint16_t mask);
void i2c_send_slave_address(uint8_t i2cx, uint16_t address);
void i2c_set_slave_address(uint8_t i2cx, uint16_t address);
void i2c_enable(uint8_t i2cx);
void i2c_send(uint8_t i2cx, uint8_t data);
void i2c_setup(uint8_t i2cx, uint8_t mode);
void i2c_enable_interrupt(uint8_t i2cx, uint8_t enable);
int i2c_master_transmit_handler(uint8_t i2cx, uint8_t buf, int len);
void i2c_master_receive_handler(uint8_t i2cx, void *buf, int len);
void i2c_start_transmit(uint8_t i2cx, uint16_t addr);
void i2c_start_receive(uint8_t i2cx, uint16_t addr);
void i2c_reset_interrupt(uint8_t i2cx, uint8_t reset);

#endif /* SAMDX_I2C_H */

