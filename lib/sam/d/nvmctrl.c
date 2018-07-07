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
/**@{*/

#include <stddef.h>

#include <libopencm3/sam/d/bitfield.h>
#include <libopencm3/sam/d/nvmctrl.h>
#include <libopencm3/sam/d/pm.h>

/* For Writing Flash */ 
#define NVM_MEM		((volatile uint16_t *)FLASH_ADDR)

/* Global state */
static uint16_t nvm_page_size;		/* Bytes per page */
static uint16_t nvm_num_pages;		/* Number of pages */

static inline int nvmctrl_isready(void)
{
	return GETBF(NVMCTRL_INTFLAG_READY, NVMCTRL->intflag);
}

static inline void nvmctrl_clearerrors(void)
{
	/* Clear all error flags */
	NVMCTRL->status |= BF(NVMCTRL_STATUS_PROGE, 1) |
					   BF(NVMCTRL_STATUS_LOCKE, 1) |
					   BF(NVMCTRL_STATUS_NVME, 1) |
					   BF(NVMCTRL_STATUS_LOAD, 1);
	NVMCTRL->intflag |= BF(NVMCTRL_INTFLAG_ERROR, 1);
}

static inline int nvmctrl_geterror(void)
{
	if (GETBF(NVMCTRL_STATUS_PROGE, NVMCTRL->status)) {
		return NVMCTRL_ERR_BADCMD;
	} else if (GETBF(NVMCTRL_STATUS_LOCKE, NVMCTRL->status)) {
		return NVMCTRL_ERR_LOCKED;
	} else if (GETBF(NVMCTRL_STATUS_NVME, NVMCTRL->status)) {
		return NVMCTRL_ERR_PROGERASE;
	} else {
		return 0;
	}
}

static inline int nvmctrl_cmd(int cmd)
{
	uint32_t saved_ctrlb = NVMCTRL->ctrlb;
	/* Shut off cache before any flash operation */
	INSERTBF(NVMCTRL_CTRLB_CACHEDIS, 1, NVMCTRL->ctrlb);

	NVMCTRL->ctrla = BF(NVMCTRL_CTRLA_CMD, cmd) |
		BF(NVMCTRL_CTRLA_CMDEX, CTRLA_CMDEX_KEY);
	while (!nvmctrl_isready()) {}

	NVMCTRL->ctrlb = saved_ctrlb;
	return nvmctrl_geterror();
}

static inline int nvmctrl_generic_cmd(int cmd)
{
	if (!nvmctrl_isready()) return NVMCTRL_ERR_BUSY;

	nvmctrl_clearerrors();

	return nvmctrl_cmd(cmd);
}

static inline int nvmctrl_generic_cmd_addr(int cmd, int addr)
{
	if (!nvmctrl_isready()) return NVMCTRL_ERR_BUSY;

	nvmctrl_clearerrors();

	NVMCTRL->addr = addr >> 1;
	return nvmctrl_cmd(cmd);
}

void nvmctrl_setwaitstates(int wait_states)
{
	INSERTBF(NVMCTRL_CTRLB_RWS, wait_states, NVMCTRL->ctrlb);
}

int nvmctrl_init(uint8_t sleep_power_mode, int cache_readmode)
{
	int wait_states;

	/* Make sure the peripheral clocks are enabled */
	INSERTBF(PM_AHBMASK_NVMCTRL, 1, PM->ahbmask);
	INSERTBF(PM_APBBMASK_NVMCTRL, 1, PM->apbbmask);

	nvmctrl_clearerrors();

	/* Is it busy? */
	if (!nvmctrl_isready()) return NVMCTRL_ERR_BUSY;

	wait_states = GETBF(NVMCTRL_CTRLB_RWS, NVMCTRL->ctrlb);

	/* Set up CTRLB, manual write is always on */
	NVMCTRL->ctrlb = BF(NVMCTRL_CTRLB_SLEEPPRM, sleep_power_mode) |
					 BF(NVMCTRL_CTRLB_MANW, 1) |
					 BF(NVMCTRL_CTRLB_RWS, wait_states) |
					 BF(NVMCTRL_CTRLB_CACHEDIS, cache_readmode);

	/* Retrieve flash parameters for this device */
	nvm_page_size = (8 << GETBF(NVMCTRL_PARAM_PSZ, NVMCTRL->param));
	nvm_num_pages = GETBF(NVMCTRL_PARAM_NVMP, NVMCTRL->param);

	return 0;
}

int nvmctrl_lock_device(void)
{
	return nvmctrl_generic_cmd(CTRLA_CMD_SSB);
}

int nvmctrl_islocked(int addr)
{
	int region;
	addr &= ~(nvm_page_size - 1);
	region = addr / nvm_page_size / (nvm_num_pages / 16);
	return ~(NVMCTRL->lock >> region) & 1;
}

int nvmctrl_lock_region(int addr)
{
	return nvmctrl_generic_cmd_addr(CTRLA_CMD_LR, addr);
}

int nvmctrl_unlock_region(int addr)
{
	return nvmctrl_generic_cmd_addr(CTRLA_CMD_UR, addr);
}

int nvmctrl_erase_row(int addr)
{
	addr &= ~((nvm_page_size * 4) - 1);
	return nvmctrl_generic_cmd_addr(CTRLA_CMD_ER, addr);
}

int nvmctrl_write_page(int addr, uint8_t *buffer)
{
	uint16_t data, i;
	addr &= ~(nvm_page_size - 1);
	addr /= 2;
	if (!nvmctrl_isready()) return NVMCTRL_ERR_BUSY;

	/* Clear page buffer */
	nvmctrl_cmd(CTRLA_CMD_PBC);

	nvmctrl_clearerrors();

	/* Copy data 16 bits at a time */
	for (i = 0; i < nvm_page_size; i += 2) {
		data = buffer[i] | (buffer[i + 1] << 8);
		NVM_MEM[addr++] = data;
	}

	return nvmctrl_cmd(CTRLA_CMD_WP);
}

int nvmctrl_read(uint32_t addr, uint8_t *buf, size_t size)
{
	size_t i;

	/* Copy data 16 bits at a time */
	for (i = 0; i < size; i++) {
		buf[i] = *(uint8_t *)(addr+i);
	}

	return 0;
}

int nvmctrl_write_row(int addr, uint8_t *buffer)
{
	int i, ret;
	uint32_t offset = 0;
	addr &= ~((nvm_page_size * 4) - 1);
	for (i = 0; i < 4; i++) {
		ret = nvmctrl_write_page(addr + offset, buffer + offset);
		if (ret < 0)
			return ret;
		offset += nvm_page_size;
	}
	return 0;
}

int nvmctrl_write_aux_page(int addr, uint8_t *buffer)
{
	uint16_t data, i;
	addr &= ~(nvm_page_size - 1);
	addr /= 2;
	if (!nvmctrl_isready()) return NVMCTRL_ERR_BUSY;

	/* Clear page buffer */
	nvmctrl_cmd(CTRLA_CMD_PBC);

	nvmctrl_clearerrors();

	/* Copy data 16 bits at a time */
	for (i = 0; i < nvm_page_size; i += 2) {
		data = buffer[i] | (buffer[i + 1] << 8);
		NVM_MEM[addr++] = data;
	}

	return nvmctrl_cmd(CTRLA_CMD_WAP);
}

int nvmctrl_erase_aux_row(void)
{
	int addr = USER_ROW_BASE;
	return nvmctrl_generic_cmd_addr(CTRLA_CMD_EAR, addr);
}

int nvmctrl_write_aux_row(uint8_t *buffer)
{
	int addr = USER_ROW_BASE;
	int i, ret;
	uint32_t offset = 0;
	addr &= ~((nvm_page_size * 4) - 1);
	for (i = 0; i < 4; i++) {
		ret = nvmctrl_write_aux_page(addr + offset, buffer + offset);
		if (ret < 0)
			return ret;
		offset += nvm_page_size;
	}
	return 0;
}

/* Power reduction mode control commands */
int nvmctrl_pr_on(void)
{
	return nvmctrl_generic_cmd(CTRLA_CMD_SPRM);
}

int nvmctrl_pr_off(void)
{
	return nvmctrl_generic_cmd(CTRLA_CMD_CPRM);
}

uint32_t nvmctrl_get_numpages(void)
{
	return nvm_num_pages;
}

/* Minimum writeable size, bytes */
uint32_t nvmctrl_get_pagesize(void)
{
	return nvm_page_size;
}

/* Minimum erasable size, bytes */
uint32_t nvmctrl_get_rowsize(void)
{
	return nvm_page_size * 4;
}

/* Minimum lockable size, bytes */
uint32_t nvmctrl_get_regionsize(void)
{
	return nvm_num_pages * 4;
}
