#include "libopencm3/sam/d/tc.h"
#include "libopencm3/sam/d/bitfield.h"

void tc_reset(uint8_t tc)
{
	TC(tc)->ctrla = 1;
}

void tc_enable(uint8_t tc, uint8_t e)
{
	if (e)
		TC(tc)->ctrla |= 2;
	else
		TC(tc)->ctrla &= ~2;
}

void tc_config_ctrla(uint8_t tc, uint8_t s, uint16_t flags)
{
	(void) s;

	TC(tc)->ctrla = flags;
}

void tc_config_ctrlb(uint8_t tc, uint8_t s, uint8_t flags)
{
	if (s)
		TC(tc)->ctrlbset = flags;
	else
		TC(tc)->ctrlbclr = flags;
}

void tc_config_ctrlc(uint8_t tc, uint8_t s, uint8_t flags)
{
	(void) s;

	TC(tc)->ctrlc = flags;
}

int tc_get_count(uint8_t tc)
{
	uint8_t size = GETBF(TC_CTRLA_MODE, TC(tc)->ctrla);

	if (size == 0)
		return TC(tc)->count;
	if (size == 1)
		return TC8(tc)->count;

	return TC32(tc)->count;
}

/* Period is only appliciable to 8bit counters. This function returns
 * -1 if the counter is of any other size.
 */
int tc_set_period(uint8_t tc, uint8_t per)
{
	uint8_t size = GETBF(TC_CTRLA_MODE, TC(tc)->ctrla);

	if (size == 1){
		TC8(tc)->per = per;
		return 0;
	}

	return -1;
}

void tc_set_cc(uint8_t tc, uint8_t cc, uint32_t val)
{
	uint8_t size = GETBF(TC_CTRLA_MODE, TC(tc)->ctrla);

	if (size == 0)
		TC(tc)->cc[cc] = val;
	else if (size == 1)
		TC8(tc)->cc[cc] = val;
	else
		TC32(tc)->cc[cc] = val;
}

void tc_enable_interrupt(uint8_t tc, uint8_t flags)
{
	TC(tc)->intenset = flags;
}

void tc_disable_interrupt(uint8_t tc, uint8_t flags)
{
	TC(tc)->intenclr = flags;
}

int tc_interrupt_flag(uint8_t tc)
{
	return TC(tc)->intflag;
}

void tc_clr_interrupt(uint8_t tc, uint8_t flags)
{
	TC(tc)->intflag = flags;
}
