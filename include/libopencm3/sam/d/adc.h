/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2013 Gareth McMullin <gareth@blacksphere.co.nz>
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

#ifndef LIBOPENCM3_SAMD_ADC_H
#define LIBOPENCM3_SAMD_ADC_H

#include <libopencm3/cm3/common.h>
#include <libopencm3/sam/d/memorymap.h>

#define SAMD21_ADC_BASE			(0x42004000U)

struct samd21_adc{
	/* 0x00 */ uint8_t ctrla;
	/* 0x01 */ uint8_t refctrl;
	/* 0x02 */ uint8_t avgctrl;
	/* 0x03 */ uint8_t sampctrl;
	/* 0x04 */ uint16_t ctrlb;
	/* 0x06 */ uint16_t reserved1;
	/* 0x08 */ uint8_t winctrl;
	/* 0x09 */ uint8_t reserved2[3];
	/* 0x0C */ uint8_t swtrig;
	/* 0x0D */ uint8_t reserved3[3];
	/* 0x10 */ uint32_t inputctrl;
	/* 0x14 */ uint8_t evctrl;
	/* 0x15 */ uint8_t reserved4;
	/* 0x16 */ uint8_t intenclr;
	/* 0x17 */ uint8_t intenset;
	/* 0x18 */ uint8_t intflag;
	/* 0x19 */ uint8_t status;
	/* 0x1A */ uint16_t result;
	/* 0x1C */ uint16_t winlt;
	/* 0x1E */ uint16_t reserved5;
	/* 0x20 */ uint16_t winut;
	/* 0x22 */ uint16_t reserved6;
	/* 0x24 */ uint16_t gaincorr;
	/* 0x26 */ uint16_t offsetcorr;
	/* 0x28 */ uint16_t calib;
	/* 0x2A */ uint8_t dbgctrl;
} __attribute__((packed));

#define ADC ((volatile struct samd21_adc*)(SAMD21_ADC_BASE))

/* CTRLA */
#define ADC_CTRLA_SWRST				1 : 0
#define ADC_CTRLA_ENABLE			1 : 1
#define ADC_CTRLA_RUNSTDBY			1 : 2

/* Reference Control (REFCTRL) */
#define ADC_REFCTRL_REFSEL			4 : 0
	#define ADC_REFCTRL_INT1V		0x0
	#define ADC_REFCTRL_INTVCC0		0x1
	#define ADC_REFCTRL_INTVCC1		0x2
	#define ADC_REFCTRL_VREFA		0x3
	#define ADC_REFCTRL_VREFB		0x4
#define ADC_REFCTRLREFCOMP			1 : 7

/* Average Control (AVGCTRL) */
#define ADC_AVGCTRL_SAMPLENUM			4 : 0
	#define ADC_AVGCTRL_1SAMPLE		0x0
	#define ADC_AVGCTRL_2SAMPLE		0x1
	#define ADC_AVGCTRL_4SAMPLE		0x2
	#define ADC_AVGCTRL_8SAMPLE		0x3
	#define ADC_AVGCTRL_16SAMPLE		0x4
	#define ADC_AVGCTRL_32SAMPLE		0x5
	#define ADC_AVGCTRL_64SAMPLE		0x6
	#define ADC_AVGCTRL_128SAMPLE		0x7
	#define ADC_AVGCTRL_256SAMPLE		0x8
	#define ADC_AVGCTRL_512SAMPLE		0x9
	#define ADC_AVGCTRL_1024SAMPLE		0xA
#define ADC_AVGCTRL_ADJRES			3 : 4

/* Sampling Time Control (SAMPCTRL) */
#define ADC_SAMPCTRL_SAMPLEN			6 : 0

/* CTRLB */
#define ADC_CTRLB_DIFFMODE			1 : 0
#define ADC_CTRLB_LEFTADJ			1 : 1
#define ADC_CTRLB_FREERUN			1 : 2
#define ADC_CTRLB_CORREN			1 : 3
#define ADC_CTRLB_RESSEL			2 : 4
	#define ADC_CTRLB_12BIT			0x0
	#define ADC_CTRLB_16BIT			0x1
	#define ADC_CTRLB_10BIT			0x2
	#define ADC_CTRLB_8BIT			0x3
#define ADC_CTRLB_PRESCALER			3 : 8
	#define ADC_CTRLB_DIV4			0x0
	#define ADC_CTRLB_DIV8			0x1
	#define ADC_CTRLB_DIV16			0x2
	#define ADC_CTRLB_DIV32			0x3
	#define ADC_CTRLB_DIV64			0x4
	#define ADC_CTRLB_DIV128		0x5
	#define ADC_CTRLB_DIV256		0x6
	#define ADC_CTRLB_DIV512		0x7

/* Window Monitor Control (WINCTRL) */
#define ADC_WINCTRL_WINMODE			3 : 0
	#define ADC_WINCTRL_DISABLE		0x0
	#define ADC_WINCTRL_MODE1		0x1
	#define ADC_WINCTRL_MODE2		0x2
	#define ADC_WINCTRL_MODE3		0x3
	#define ADC_WINCTRL_MODE4		0x4

/* Software Trigger (SWTRIG) */
#define ADC_SWTRIG_FLUSH			1 : 0
#define ADC_SWTRIG_START			1 : 1

/* Input Control (INPUTCTRL) */
#define ADC_INPUTCTRL_MUXPOS			5 : 0
	#define ADC_INPUTCTRL_PIN0		0x00
	#define ADC_INPUTCTRL_PIN1		0x01
	#define ADC_INPUTCTRL_PIN2		0x02
	#define ADC_INPUTCTRL_PIN3		0x03
	#define ADC_INPUTCTRL_PIN4		0x04
	#define ADC_INPUTCTRL_PIN5		0x05
	#define ADC_INPUTCTRL_PIN6		0x06
	#define ADC_INPUTCTRL_PIN7		0x07
	#define ADC_INPUTCTRL_PIN8		0x08
	#define ADC_INPUTCTRL_PIN9		0x09
	#define ADC_INPUTCTRL_PIN10		0x0A
	#define ADC_INPUTCTRL_PIN11		0x0B
	#define ADC_INPUTCTRL_PIN12		0x0C
	#define ADC_INPUTCTRL_PIN13		0x0D
	#define ADC_INPUTCTRL_PIN14		0x0E
	#define ADC_INPUTCTRL_PIN15		0x0F
	#define ADC_INPUTCTRL_PIN16		0x10
	#define ADC_INPUTCTRL_PIN17		0x11
	#define ADC_INPUTCTRL_PIN18		0x12
	#define ADC_INPUTCTRL_PIN19		0x13
	#define ADC_INPUTCTRL_TEMP		0x18
	#define ADC_INPUTCTRL_BANDGAP		0x19
	#define ADC_INPUTCTRL_SCALEDCOREVCC	0x1A
	#define ADC_INPUTCTRL_SCALEDIOVCC	0x1B
	#define ADC_INPUTCTRL_DAC		0x1C
#define ADC_INPUTCTRL_MUXNEG			5 : 8
	#define ADC_INPUTCTRL_GND		0x18
	#define ADC_INPUTCTRL_IOGND		0x19
#define ADC_INPUTCTRL_INPUTSCAN			4 : 16
#define ADC_INPUTCTRL_INPUTOFFSET		4 : 20
#define ADC_INPUTCTRL_GAIN			4 : 24
	#define ADC_INPUTCTRL_1X		0x0
	#define ADC_INPUTCTRL_2X		0x1
	#define ADC_INPUTCTRL_4X		0x2
	#define ADC_INPUTCTRL_8X		0x3
	#define ADC_INPUTCTRL_16X		0x4
	#define ADC_INPUTCTRL_DIV2		0xF

/* Event Control (EVCTRL) */
#define ADC_EVCTRL_STARTEI			1 : 0
#define ADC_EVCTRL_SYNCEI			1 : 1
#define ADC_EVCTRL_RESRDYEO			1 : 4
#define ADC_EVCTRL_WINMONEO			1 : 5

/* Interrupt Enable Clear (INTENCLR) */
#define ADC_INTENCLR_RESRDY			1 : 0
#define ADC_INTENCLR_OVERRUN			1 : 1
#define ADC_INTENCLR_WINMON			1 : 2
#define ADC_INTENCLR_SYNCRDY			1 : 3

/* Interrupt Enable Set (INTENSET) */
#define ADC_INTENSET_RESRDY			1 : 0
#define ADC_INTENSET_OVERRUN			1 : 1
#define ADC_INTENSET_WINMON			1 : 2
#define ADC_INTENSET_SYNCRDY			1 : 3

/* Interrupt Flag Status and Clear (INTFLAG) */
#define ADC_INTFLAG_RESRDY			1 : 0
#define ADC_INTFLAG_OVERRUN			1 : 1
#define ADC_INTFLAG_WINMON			1 : 2
#define ADC_INTFLAG_SYNCRDY			1 : 3

/* Status (STATUS) */
#define ADC_STATUS_SYNCBUSY			1 : 7

/* Result (RESULT) */
#define ADC_RESULT				16 : 0

/* Window Monitor Lower Threshold (WINLT) */
#define ADC_WINLT				16 : 0

/* Window Monitor Upper Threshold (WINUT) */
#define ADC_WINUT				16 : 0

/* Gain Correction (GAINCORR) */
#define ADC_GAINCORR				12 : 0

/* Offset Correction (OFFSETCORR) */
#define ADC_OFFSETCORR				12 : 0

/* Calibration (CALIB) */
#define ADC_CALIB_LINEARITY_CAL			8 : 0
#define ADC_CALIB_BIAS_CAL			3 : 8

/* Debug Control (DBGCTRL) */
#define ADC_DBGCTRL_DBGRUN			1 : 0

void adc_enable(uint8_t ref, uint8_t comp, uint8_t neg, uint8_t pos);
void adc_disable(void);
void adc_reset(void);
void adc_reference_select(uint8_t ref, uint8_t comp);
void adc_average_set(uint8_t adj, uint8_t samples);
void adc_sample_length_set(uint8_t len);
void adc_prescale_set(uint8_t prescale);
void adc_resolution_set(uint8_t res);
void adc_ctrlb_config(uint8_t corr, uint8_t free, uint8_t adj, uint8_t diff);
void adc_window_mode_set(uint8_t mode);
void adc_start(void);
void adc_flush(void);
void adc_gain_set(uint8_t gain);
void adc_offset_set(uint8_t offset);
void adc_scan_set(uint8_t num);
void adc_neg_set(uint8_t neg);
void adc_pos_set(uint8_t pos);
void adc_event_enable(uint8_t events);
void adc_interrupt_clear(uint8_t interrupts);
void adc_interrupt_set(uint8_t interrupts);
int adc_interrupt_flag_get(void);
void adc_interrupt_flag_clear(uint8_t interrupts);
int adc_syncbusy(void);
uint16_t adc_result(void);
void adc_window_lower_threshold_set(uint16_t thresh);
void adc_window_upper_threshold_set(uint16_t thresh);
void adc_gain_correction_set(uint16_t corr);
void adc_offset_correction_set(uint16_t corr);
void adc_calibration_set(uint8_t bias, uint8_t linearity);
void adc_enable_resrdy_interrupt(void);
void adc_enable_overrun_interrupt(void);
void adc_enable_winmon_interrupt(void);
void adc_disable_resrdy_interrupt(void);

#endif

