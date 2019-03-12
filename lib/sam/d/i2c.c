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

#include <libopencm3/sam/d/bitfield.h>
#include <libopencm3/cm3/cortex.h>

#include <libopencm3/sam/d/gclk.h>
#include <libopencm3/sam/d/nvic.h>
#include <libopencm3/sam/d/pm.h>
#include <libopencm3/sam/d/i2c.h>

/* reset i2c */
void i2c_reset(uint8_t i2cx){

	INSERTBF(I2C_CTRLA_SWRST, 1, I2C(i2cx)->ctrla);
	while(GETBF(I2C_CTRLA_SWRST, I2C(i2cx)->ctrla));

}

/* disable i2c */
void i2c_disable(uint8_t i2cx){

	//CTRLA.ENABLE = 0
	INSERTBF(I2C_CTRLA_ENABLE, 0, I2C(i2cx)->ctrla);

}
#if 0
void i2c_init(uint8_t i2cx){

	i2c = I2C(i2cx);
	i2c_disable(i2cx);

}
#endif
/* enable i2c */
void i2c_enable(uint8_t i2cx){

	//CTRLA.ENABLE = 1
	INSERTBF(I2C_CTRLA_ENABLE, 1, I2C(i2cx)->ctrla);
	while(I2C(i2cx)->syncbusy);

}

/* put i2c in master mode and set timeout and baud rate */
void i2c_configure_master(uint8_t i2cx, uint8_t timeout, uint8_t baud){

	//CTRLA.MODE = MASTER
	INSERTBF(I2C_CTRLA_MODE, I2C_MODE_MASTER, I2C(i2cx)->ctrla);

	INSERTBF(I2C_CTRLA_INACTOUT, timeout, I2C(i2cx)->ctrla);
	INSERTBF(I2C_BAUD, baud, I2C(i2cx)->baud);

}

/* put i2c in slave mode and set addressing mode and address mask */
void i2c_configure_slave(uint8_t i2cx, uint8_t address_mode, uint16_t mask){

	//CTRLA.MODE = SLAVE
	INSERTBF(I2C_CTRLA_MODE, I2C_MODE_SLAVE, I2C(i2cx)->ctrla);

	INSERTBF(I2C_CTRLB_AMODE, address_mode, I2C(i2cx)->ctrlb);
	INSERTBF(I2C_ADDR_ADDRMASK, mask, I2C(i2cx)->addr);

}

/* set i2c slave address */
void i2c_set_slave_address(uint8_t i2cx, uint16_t address){

	INSERTBF(I2C_ADDR_ADDR, address, I2C(i2cx)->addr);

}

/**************************** OPTIONAL SETTINGS ***************************/

/* set i2c hold time */
void i2c_set_hold(uint8_t i2cx, uint8_t holdtime){

	//CTRLA.SDAHOLD = holdtime (SDA hold time)
	INSERTBF(I2C_CTRLA_SDAHOLD, holdtime, I2C(i2cx)->ctrla);

}

/* enable or disable i2c smart operation */
void i2c_configure_smart_mode(uint8_t i2cx, uint8_t f){

	//CTRLB.SMEN = 1 for enable smart mode
	INSERTBF(I2C_CTRLB_SMEN, f, I2C(i2cx)->ctrlb);

}

/* enable or disable i2c scl timeout */
void i2c_configure_scl_timeout(uint8_t i2cx, uint8_t f){

	//CTRLA.LOWTOUT = 1 for enable scl timeout
	INSERTBF(I2C_CTRLA_LOWTOUT, f, I2C(i2cx)->ctrla);

}

/* select transfer speed (default Standard Mode (SM)) */
void i2c_set_speed(uint8_t i2cx, uint8_t mode)
{

	INSERTBF(I2C_CTRLA_SPEED, mode, I2C(i2cx)->ctrla);

}

/* set baud rate low */
void i2c_set_baudlow(uint8_t i2cx, uint8_t baudlow)
{

	INSERTBF(I2C_BAUD_BAUDLOW, baudlow, I2C(i2cx)->baud);

}

/* set high speed baud rate and baud rate low */
void i2c_set_hs_baud(uint8_t i2cx, uint8_t baud, uint8_t baudlow)
{

	INSERTBF(I2C_BAUD_HSBAUD, baud, I2C(i2cx)->baud);
	INSERTBF(I2C_BAUD_HSBAUDLOW, baudlow, I2C(i2cx)->baud);

}

/**************************** DATA TRANSMISSION ******************************/

/* send START command followed by slave address */
void i2c_send_slave_address(uint8_t i2cx, uint16_t address){

	//i2c_set_slave_address(i2cx, address);
	INSERTBF(11:0, address, I2C(i2cx)->addr);
	while(I2C(i2cx)->syncbusy);

}

/* read from i2c data register */
uint8_t i2c_read(uint8_t i2cx){

	return GETBF(I2C_DATA, I2C(i2cx)->data);

}

/* write to i2c data register */
void i2c_send(uint8_t i2cx, uint8_t data)
{
	I2C(i2cx)->data = data;
}

/* send STOP command */
void i2c_send_stop(uint8_t i2cx)
{

        INSERTBF(I2C_CTRLB_CMD, 0x3, I2C(i2cx)->ctrlb);

}

/************************** INTERRUPT SETTINGS AND STATUS ****************/

/* enable interrupts (write 1 to all bits to enable) */
void i2c_enable_interrupt(uint8_t i2cx, uint8_t enable)
{

	I2C(i2cx)->intenset = enable;
	//irq_enable(PERIPH_ID_SERCOM0,1);
	//register_interrupt(i2c_handler, PERIPH_ID_SERCOM0);

}

/* disable interrupts (write 1 to all bits to disable) */
void i2c_disable_interrupt(uint8_t i2cx, uint8_t disable){

	I2C(i2cx)->intenclr = disable;

}

static uint8_t wait_for_HS_NACK = 0;
static uint16_t highspeed_addr = 0;
#if 0
/* when SB is set */
void i2c_slave_on_bus(uint8_t i2cx)
{
	in[j] = i2c_read(i2cx);
	while(I2C(i2cx)->syncbusy);

	j++;
	if(j < 2){
		INSERTBF(I2C_CTRLB_CMD, I2C_CMD_READ, I2C(i2cx)->ctrlb);
		while(I2C(i2cx)->syncbusy);
		INSERTBF(I2C_CTRLB_ACKACT, I2C_NACK, I2C(i2cx)->ctrlb);
	}else{
		i2c_send_stop(i2cx);
		while(I2C(i2cx)->syncbusy);
	}
}
#endif
int i2c_master_transmit_handler(uint8_t i2cx, uint8_t buf, int len)
{
	//if no ACK received
	if(GETBF(I2C_STATUS_RXNACK, I2C(i2cx)->status)){

		if(wait_for_HS_NACK){ //to enable highspeed mode
			wait_for_HS_NACK = 0;

			INSERTBF(I2C_CTRLB_ACKACT, I2C_ACK, I2C(i2cx)->ctrlb);
			while(I2C(i2cx)->syncbusy);

			I2C(i2cx)->addr = BF(I2C_ADDR_HS, 1) | 
					BF(11:0, highspeed_addr);
			while(I2C(i2cx)->syncbusy);
			return 0;

		}else{ //otherwise, stop operation
			i2c_send_stop(i2cx);
			while(I2C(i2cx)->syncbusy);
			return 1;
		}

	//if arbitration lost
	}else if(GETBF(I2C_STATUS_ARBLOST, I2C(i2cx)->status)){
		return 2;
	//ACK received
	}else{
		if (len) { /* send another if we want */
			I2C(i2cx)->data = buf;
			while(I2C(i2cx)->syncbusy);
		} else { /* otherwise send stop and halt */
			i2c_send_stop(i2cx);
			while(I2C(i2cx)->syncbusy);
		}
		return 0;
	}
}

void i2c_master_receive_handler(uint8_t i2cx, void *buf, int len)
{
	*((uint8_t*)buf) = i2c_read(i2cx);
	while(I2C(i2cx)->syncbusy);

	if(len){ /* continue transmition if we want */
		INSERTBF(I2C_CTRLB_ACKACT, I2C_ACK, I2C(i2cx)->ctrlb);
		while(I2C(i2cx)->syncbusy);
		INSERTBF(I2C_CTRLB_CMD, I2C_CMD_READ, I2C(i2cx)->ctrlb);
		while(I2C(i2cx)->syncbusy);
	}else{ /* otherwise, send stop and halt */
		i2c_send_stop(i2cx);
		while(I2C(i2cx)->syncbusy);
	}
}

/* addr should not include R/W */
void i2c_start_transmit(uint8_t i2cx, uint16_t addr)
{
	if (GETBF(I2C_CTRLA_SPEED, I2C(i2cx)->ctrla) == I2C_HIGHSPEED_MODE) {
		wait_for_HS_NACK = 1;
		highspeed_addr = (addr<<1);
		i2c_send_slave_address(i2cx, 0x08);
	} else {
		wait_for_HS_NACK = 0;
		i2c_send_slave_address(i2cx, (addr<<1));
	}
}

/* addr should not include R/W */
void i2c_start_receive(uint8_t i2cx, uint16_t addr)
{
	if (GETBF(I2C_CTRLA_SPEED, I2C(i2cx)->ctrla) == I2C_HIGHSPEED_MODE) {
		wait_for_HS_NACK = 1;
		highspeed_addr = ((addr<<1)&1);
		i2c_send_slave_address(i2cx, 0x08);
	} else {
		wait_for_HS_NACK = 0;
		i2c_send_slave_address(i2cx, ((addr<<1)|1));
	}
}

/* reset interrupt flag status */
void i2c_reset_interrupt(uint8_t i2cx, uint8_t reset){

	I2C(i2cx)->intflag = reset;

}

/************************* SETUP FUNCTIONS ********************************/

/* a particular setup for highspeed i2c */
void i2c_setup_highspeed(uint8_t i2cx)
{
	//i2c_init(i2cx);
	i2c_disable(i2cx);

	// set baud rates and configure master mode
	i2c_configure_master(i2cx, I2C_HOLD_450, 0x48);
	i2c_set_baudlow(i2cx, 0x48);
	i2c_set_hs_baud(i2cx, 0x5, 0x10);

	while(I2C(i2cx)->syncbusy);

	// enable standby
	INSERTBF(I2C_CTRLA_RUNSTDBY, 1, I2C(i2cx)->ctrla);

	// set speed to HIGHSPEED
	i2c_set_speed(i2cx, I2C_HIGHSPEED_MODE);

	// enable smart mode
	i2c_configure_smart_mode(i2cx, 1);

	// set clock stretch mode
	INSERTBF(I2C_CTRLA_SCLSM, 1, I2C(i2cx)->ctrla);

	while(I2C(i2cx)->syncbusy);

	nvic_enable_irq(9 + i2cx);

	// enable i2c interrupts
	i2c_enable_interrupt(i2cx, BF(I2C_INTENSET_MB, 1) |
				BF(I2C_INTENSET_SB, 1));

        // enable i2c
        i2c_enable(i2cx);

        // switch bus state to IDLE
        INSERTBF(I2C_STATUS_BUSSTATE, 1, I2C(i2cx)->status);
}

/* A particular setup for standard i2c */
void i2c_setup_standard(uint8_t i2cx)
{
	//i2c_init(i2cx);
	i2c_disable(i2cx);

	// set baud rates and configure master mode
	i2c_configure_master(i2cx, I2C_HOLD_DISABLE, 0x48);
	i2c_set_baudlow(i2cx, 0x48);

	while(I2C(i2cx)->syncbusy);

	// enable standby
	INSERTBF(I2C_CTRLA_RUNSTDBY, 1, I2C(i2cx)->ctrla);

	// enable smart mode
	//i2c_configure_smart_mode(i2cx, 1);

	while(I2C(i2cx)->syncbusy);

	//nvic_enable_irq(NVIC_SERCOM0_IRQ + i2cx);
	nvic_enable_irq(9 + i2cx);

	// enable i2c interrupts
	i2c_enable_interrupt(i2cx, BF(I2C_INTENSET_MB, 1) |
				BF(I2C_INTENSET_SB, 1));

        // enable i2c
        i2c_enable(i2cx);

        // switch bus state to IDLE
        INSERTBF(I2C_STATUS_BUSSTATE, 1, I2C(i2cx)->status);
}

void i2c_setup(uint8_t i2cx, uint8_t mode)
{
	INSERTBF(PM_APBCMASK_PAC2, 1, PM->apbcmask);
	INSERTBF(1 : i2cx + 2, 1, PM->apbcmask);

	if (mode == I2C_HIGHSPEED_MODE)
		i2c_setup_highspeed(i2cx);
	else
		i2c_setup_standard(i2cx);

}

