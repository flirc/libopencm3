#include <libopencm3/sam/d/port.h>
#include <libopencm3/sam/d/bitfield.h>

/**@{*/

/*---------------------------------------------------------------------------*/
/** @brief Set a Group of Pins Atomic

Set one or more pins of the given GPIO port to 1 in an atomic operation.

@param[in] gpioport Unsigned int32. Port identifier @ref gpio_port_id
@param[in] gpios Unsigned int16. Pin identifiers @ref gpio_pin_id
	     If multiple pins are to be changed, use bitwise OR '|' to separate
	     them.
*/
void gpio_set(uint32_t gpioport, uint32_t gpios)
{
	PORT_OUTSET(gpioport) = gpios;
}

/*---------------------------------------------------------------------------*/
/** @brief Clear a Group of Pins Atomic

Clear one or more pins of the given GPIO port to 0 in an atomic operation.

@param[in] gpioport Unsigned int32. Port identifier @ref gpio_port_id
@param[in] gpios Unsigned int16. Pin identifiers @ref gpio_pin_id
	     If multiple pins are to be changed, use bitwise OR '|' to separate
	     them.
*/
void gpio_clear(uint32_t gpioport, uint32_t gpios)
{
	PORT_OUTCLR(gpioport) = (gpios);
}

/*---------------------------------------------------------------------------*/
/** @brief Read a Group of Pins.

@param[in] gpioport Unsigned int32. Port identifier @ref gpio_port_id
@param[in] gpios Unsigned int16. Pin identifiers @ref gpio_pin_id
	    If multiple pins are to be read, use bitwise OR '|' to separate
	    them.
@return Unsigned int16 value of the pin values. The bit position of the pin
	value returned corresponds to the pin number.
*/
uint32_t gpio_get(uint32_t gpioport, uint32_t gpios)
{
	return !!(gpio_port_read(gpioport) & gpios);
}

/*---------------------------------------------------------------------------*/
/** @brief Toggle a Group of Pins

Toggle one or more pins of the given GPIO port. The toggling is not atomic, but
the non-toggled pins are not affected.

@param[in] gpioport Unsigned int32. Port identifier @ref gpio_port_id
@param[in] gpios Unsigned int16. Pin identifiers @ref gpio_pin_id
	     If multiple pins are to be changed, use bitwise OR '|' to separate
	     them.
*/
void gpio_toggle(uint32_t gpioport, uint32_t gpios)
{
	PORT_OUTTGL(gpioport) = gpios;
}

/*---------------------------------------------------------------------------*/
/** @brief Read from a Port

Read the current value of the given GPIO port. Only the lower 16 bits contain
valid pin data.

@param[in] gpioport Unsigned int32. Port identifier @ref gpio_port_id
@return Unsigned int16. The value held in the specified GPIO port.
*/
uint32_t gpio_port_read(uint32_t gpioport)
{
	return (uint16_t)PORT_IN(gpioport);
}

/*---------------------------------------------------------------------------*/
/** @brief Write to a Port

Write a value to the given GPIO port.

@param[in] gpioport Unsigned int32. Port identifier @ref gpio_port_id
@param[in] data Unsigned int16. The value to be written to the GPIO port.
*/
void gpio_port_write(uint32_t gpioport, uint32_t data)
{
	PORT_OUT(gpioport) = data;
}

/*---------------------------------------------------------------------------*/


const int8_t lsb_nibble_lut[] =
                {-1, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0};

int least_set_bit(uint32_t w) {
        int i, r;

        for (i = 0, r = -1; w; i += 4, w >>= 4) {
                if ((r = lsb_nibble_lut[w & 0xF]) >= 0) {
                        r += i;
                        break;
                }
        }

        return r;
}

/*---------------------------------------------------------------------------*/
/** @brief Configure a pin

Configures a pin as the corresponding special function

@param[in] gpioport Unsigned int32. Port identifier @ref gpio_port_id
@param[in] pin
@param[in] Special Function A-H
*/
void gpio_config_special(uint32_t port, uint32_t pin,
		enum gpio_special_function function)
{
	uint32_t p = least_set_bit(pin);

	if (function == SOC_GPIO_NONE) {
		/* clear pin mux enable pin */
		PORT_PINCFG(port, p) &= ~PMUXEN;
		return;
	}

	if (IS_ODD(p)) {
		PORT_PMUX(port, p/2) |= function<<4;
	} else {
		PORT_PMUX(port, p/2) |= function;
	}

	/* set pin mux enable pin */
	PORT_PINCFG(port, p) |= PMUXEN;
}
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/** @brief Configure a pin as input

Configures a pin as the corresponding special function

@param[in] gpioport Unsigned int32. Port identifier @ref gpio_port_id
@param[in] pin
@param[in] any input flags as defined in port.h
*/
void gpio_config_input(uint32_t port, uint32_t pin, uint32_t inflags)
{
	uint32_t p = least_set_bit(pin);

	/* Enable Input Direction */
	PORT_DIRCLR(port) = pin;

	/* handle flags */
	if (GPIO_IN_FLAG_PULLUP & inflags) {
		/* enable pull-up resistor */
		PORT_PINCFG(port, p) |= GPIO_PINCFG_PULLEN;
		/* activate pull-up */
		PORT_OUTSET(port) = pin;
	} else {
		/* enable pull-up resistor */
		PORT_PINCFG(port, p) &= ~GPIO_PINCFG_PULLEN;
		/* de-activate pullup */
		PORT_OUTCLR(port) = pin;
	}

	/* set pin mux enable pin */
	PORT_PINCFG(port, least_set_bit(pin)) |= GPIO_PINCFG_INEN;
}
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/** @brief Configure a pin as output

Configures a pin as the corresponding special function

@param[in] gpioport Unsigned int32. Port identifier @ref gpio_port_id
@param[in] pin
@param[in] any input flags as defined in port.h
*/
void gpio_config_output(uint32_t port, uint32_t pin, uint32_t outflags)
{
	/* Set the data register before enabling the pin */
	if (GPIO_OUT_FLAG_DEFAULT_HIGH & outflags) {
		PORT_OUTSET(port) = pin;
	} else {
		PORT_OUTCLR(port) = pin;
	}

	/* Enable Output */
	PORT_DIRSET(port) = pin;
}
/*---------------------------------------------------------------------------*/

/**@}*/
