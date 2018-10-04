#include "libopencm3/sam/d/eic.h"
#include "libopencm3/sam/d/bitfield.h"

void eic_reset(void)
{
	EIC->ctrl = 1;
}

void eic_enable(uint8_t s)
{
	if (s)
		EIC->ctrl = 2;
	else
		EIC->ctrl = 0;
}

void eic_enable_interrupt(uint32_t pins)
{
	EIC->intenset = pins;
}

void eic_disable_interrupt(uint32_t pins)
{
	EIC->intenclr = pins;
}

int eic_interrupt_flag()
{
	return EIC->intflag;
}

void eic_clr_interrupt(uint32_t pins)
{
	EIC->intflag = pins;
}

void eic_set_config(uint8_t pin, uint8_t filter, uint8_t sense)
{
	uint8_t cblock = pin>>3;
	uint8_t cpin = (pin<<2) - (cblock<<5);

	INSERTBF(3 : cpin, sense, EIC->config[cblock]);
	INSERTBF(1 : (cpin+3), filter, EIC->config[cblock]);
}
