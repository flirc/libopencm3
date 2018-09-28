/* Header will go here
 * For now, enjoy the silence
 */

#include <libopencm3/sam/d/adc.h>
#include <libopencm3/sam/d/bitfield.h>
#include <libopencm3/sam/d/pm.h>
#include <libopencm3/sam/d/nvic.h>
#include <libopencm3/sam/d/nvmctrl.h>

void adc_enable(uint8_t ref, uint8_t comp, uint8_t neg, uint8_t pos)
{
	INSERTBF(PM_APBCMASK_ADC, 1, PM->apbcmask);

	adc_reset();

	while(ADC->status);

	adc_calibration_set(
			GETBF(CALIB_ADC_BIASCAL, *((uint64_t*)0x806020)),
			GETBF(CALIB_ADC_LINEARITY, *((uint64_t*)0x806020))
	);

	adc_neg_set(neg);

	adc_pos_set(pos);

	nvic_enable_irq(NVIC_ADC_IRQ);

	adc_reference_select(ref, comp);
	while(ADC->status);

        ADC->ctrla |= (1<<1);
	while(ADC->status);
}

void adc_disable(void)
{
	ADC->ctrla &= 1;
}

void adc_reset(void)
{
	ADC->ctrla = 1;
}

void adc_reference_select(uint8_t ref, uint8_t comp)
{
	ADC->refctrl = (uint8_t)((comp << 7) | ref);
}

void adc_average_set(uint8_t adj, uint8_t samples)
{
	ADC->avgctrl = (adj << 4) | samples;
}

void adc_sample_length_set(uint8_t len)
{
	ADC->sampctrl = len;
}

void adc_prescale_set(uint8_t prescale)
{
	ADC->ctrlb = (ADC->ctrlb & 0xF8FF) | (prescale << 8);
}

void adc_resolution_set(uint8_t res)
{
	ADC->ctrlb = (ADC->ctrlb & 0xFFCF) | (res << 4);
}

void adc_ctrlb_config(uint8_t corr, uint8_t free, uint8_t adj, uint8_t diff)
{
	ADC->ctrlb = (ADC->ctrlb & 0xFFF0) | (corr << 3) |
			(free << 2) | (adj << 1) | diff;
}

void adc_window_mode_set(uint8_t mode)
{
	ADC->winctrl = mode;
}

void adc_start(void)
{
	ADC->swtrig |= (1 << 1);
}

void adc_flush(void)
{
	ADC->swtrig |= 1;
}

void adc_gain_set(uint8_t gain)
{
	ADC->inputctrl = (ADC->inputctrl & 0xF0FFFFFF) | (gain << 24);
}

void adc_offset_set(uint8_t offset)
{
	ADC->inputctrl = (ADC->inputctrl & 0xFF0FFFFF) | (offset << 20);
}

void adc_scan_set(uint8_t num)
{
	ADC->inputctrl = (ADC->inputctrl & 0xFFF0FFFF) | (num << 16);
}

void adc_neg_set(uint8_t neg)
{
	ADC->inputctrl = (ADC->inputctrl & 0xFFFFE0FF) | (neg << 8);
}

void adc_pos_set(uint8_t pos)
{
	ADC->inputctrl = (ADC->inputctrl & 0xFFFFFFE0) | pos;
}

void adc_event_enable(uint8_t events)
{
	ADC->evctrl = events;
}

void adc_interrupt_clear(uint8_t interrupts)
{
	ADC->intenclr = interrupts;
}

void adc_interrupt_set(uint8_t interrupts)
{
	ADC->intenset = interrupts;
}

int adc_interrupt_flag_get(void)
{
	return ADC->intflag;
}

void adc_interrupt_flag_clear(uint8_t interrupts)
{
	ADC->intflag = interrupts;
}

int adc_syncbusy(void)
{
	return ADC->status;
}

uint16_t adc_result(void)
{
	return ADC->result;
}

void adc_window_lower_threshold_set(uint16_t thresh)
{
	ADC->winlt = thresh;
}

void adc_window_upper_threshold_set(uint16_t thresh)
{
	ADC->winut = thresh;
}

void adc_gain_correction_set(uint16_t corr)
{
	ADC->gaincorr = corr;
}

void adc_offset_correction_set(uint16_t corr)
{
	ADC->offsetcorr = corr;
}

void adc_calibration_set(uint8_t bias, uint8_t linearity)
{
	ADC->calib = (bias << 8) | linearity;
}

void adc_enable_resrdy_interrupt(void)
{
	ADC->intenset |= 1;
}

void adc_enable_overrun_interrupt(void)
{
	ADC->intenset |= 2;
}

void adc_enable_winmon_interrupt(void)
{
	ADC->intenset |= 4;
}

void adc_disable_resrdy_interrupt(void)
{
	ADC->intenclr |= 1;
}
