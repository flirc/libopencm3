/* External Interrupt Controller */

#include <stdint.h>

#ifndef SAMDX_EIC_H
#define SAMDX_EIC_H

#define SAMD21_EIC_BASE				0x40001800

/* generic clock controller */
struct samd21_eic {
	/* 0x0  */ uint8_t ctrl;
	/* 0x1  */ uint8_t status;
	/* 0x2  */ uint8_t nmictrl;
	/* 0x3  */ uint8_t nmiflag;
	/* 0x4  */ uint32_t evctrl;
	/* 0x8  */ uint32_t intenclr;
	/* 0xC  */ uint32_t intenset;
	/* 0x10 */ uint32_t intflag;
	/* 0x14 */ uint32_t wakeup;
	/* 0x18 */ uint32_t config[3];
} __attribute__((packed));

/* CTRL - Control */
#define EIC_CTRL_SWRST							1 : 0
#define EIC_CTRL_ENABLE							1 : 1

/* STATUS */
#define EIC_STATUS_SYNCBUSY						1 : 7

/* NMICTRL - Non-Maskable Interrupt Control */
#define EIC_NMICTRL_NMISENSE						3 : 28
#define EIC_NMICTRL_NMISENSE_SIZE					3
#define EIC_NMICTRL_NMISENSE_SHIFT					0
#	define EIC_NONE				0x00
#	define EIC_RISE				0x01
#	define EIC_FALL				0x02
#	define EIC_BOTH				0x03
#	define EIC_HIGH				0x04
#	define EIC_LOW				0x05
#define EIC_NMICTRL_NMIFILTEN						1 : 31
#define EIC_NMICTRL_NMIFILTEN_SIZE					1
#define EIC_NMICTRL_NMIFILTEN_SHIFT					3

/* NMIFLAG - Non-Maskable Interrupt Flag Status and Clear */
#define EIC_NMIFLAG_NMI							1 : 0

/* EVCTRL - Event Control */
#define EIC_EVCTRL_EXTINTEO0						1 : 0
#define EIC_EVCTRL_EXTINTEO1						1 : 1
#define EIC_EVCTRL_EXTINTEO2						1 : 2
#define EIC_EVCTRL_EXTINTEO3						1 : 3
#define EIC_EVCTRL_EXTINTEO4						1 : 4
#define EIC_EVCTRL_EXTINTEO5						1 : 5
#define EIC_EVCTRL_EXTINTEO6						1 : 6
#define EIC_EVCTRL_EXTINTEO7						1 : 7
#define EIC_EVCTRL_EXTINTEO8						1 : 8
#define EIC_EVCTRL_EXTINTEO9						1 : 9
#define EIC_EVCTRL_EXTINTEO10						1 : 10
#define EIC_EVCTRL_EXTINTEO11						1 : 11
#define EIC_EVCTRL_EXTINTEO12						1 : 12
#define EIC_EVCTRL_EXTINTEO13						1 : 13
#define EIC_EVCTRL_EXTINTEO14						1 : 14
#define EIC_EVCTRL_EXTINTEO15						1 : 15

/* INTENCLR - Interrupt Enable Clear */
#define EIC_INTENCLR_EXTINT0						1 : 0
#define EIC_INTENCLR_EXTINT1						1 : 1
#define EIC_INTENCLR_EXTINT2						1 : 2
#define EIC_INTENCLR_EXTINT3						1 : 3
#define EIC_INTENCLR_EXTINT4						1 : 4
#define EIC_INTENCLR_EXTINT5						1 : 5
#define EIC_INTENCLR_EXTINT6						1 : 6
#define EIC_INTENCLR_EXTINT7						1 : 7
#define EIC_INTENCLR_EXTINT8						1 : 8
#define EIC_INTENCLR_EXTINT9						1 : 9
#define EIC_INTENCLR_EXTINT10						1 : 10
#define EIC_INTENCLR_EXTINT11						1 : 11
#define EIC_INTENCLR_EXTINT12						1 : 12
#define EIC_INTENCLR_EXTINT13						1 : 13
#define EIC_INTENCLR_EXTINT14						1 : 14
#define EIC_INTENCLR_EXTINT15						1 : 15

/* INTENSET - Interrupt Enable Set */
#define EIC_INTENSET_EXTINT0						1 : 0
#define EIC_INTENSET_EXTINT1						1 : 1
#define EIC_INTENSET_EXTINT2						1 : 2
#define EIC_INTENSET_EXTINT3						1 : 3
#define EIC_INTENSET_EXTINT4						1 : 4
#define EIC_INTENSET_EXTINT5						1 : 5
#define EIC_INTENSET_EXTINT6						1 : 6
#define EIC_INTENSET_EXTINT7						1 : 7
#define EIC_INTENSET_EXTINT8						1 : 8
#define EIC_INTENSET_EXTINT9						1 : 9
#define EIC_INTENSET_EXTINT10						1 : 10
#define EIC_INTENSET_EXTINT11						1 : 11
#define EIC_INTENSET_EXTINT12						1 : 12
#define EIC_INTENSET_EXTINT13						1 : 13
#define EIC_INTENSET_EXTINT14						1 : 14
#define EIC_INTENSET_EXTINT15						1 : 15

/* INTFLAG - Interrupt Flag Status and Clear */
#define EIC_INTFLAG_EXTINT0						1 : 0
#define EIC_INTFLAG_EXTINT1						1 : 1
#define EIC_INTFLAG_EXTINT2						1 : 2
#define EIC_INTFLAG_EXTINT3						1 : 3
#define EIC_INTFLAG_EXTINT4						1 : 4
#define EIC_INTFLAG_EXTINT5						1 : 5
#define EIC_INTFLAG_EXTINT6						1 : 6
#define EIC_INTFLAG_EXTINT7						1 : 7
#define EIC_INTFLAG_EXTINT8						1 : 8
#define EIC_INTFLAG_EXTINT9						1 : 9
#define EIC_INTFLAG_EXTINT10						1 : 10
#define EIC_INTFLAG_EXTINT11						1 : 11
#define EIC_INTFLAG_EXTINT12						1 : 12
#define EIC_INTFLAG_EXTINT13						1 : 13
#define EIC_INTFLAG_EXTINT14						1 : 14
#define EIC_INTFLAG_EXTINT15						1 : 15

/* WAKEUP - Wake-Up Enable */
#define EIC_WAKEUP_WAKEUPEN0						1 : 0
#define EIC_WAKEUP_WAKEUPEN1						1 : 1
#define EIC_WAKEUP_WAKEUPEN2						1 : 2
#define EIC_WAKEUP_WAKEUPEN3						1 : 3
#define EIC_WAKEUP_WAKEUPEN4						1 : 4
#define EIC_WAKEUP_WAKEUPEN5						1 : 5
#define EIC_WAKEUP_WAKEUPEN6						1 : 6
#define EIC_WAKEUP_WAKEUPEN7						1 : 7
#define EIC_WAKEUP_WAKEUPEN8						1 : 8
#define EIC_WAKEUP_WAKEUPEN9						1 : 9
#define EIC_WAKEUP_WAKEUPEN10						1 : 10
#define EIC_WAKEUP_WAKEUPEN11						1 : 11
#define EIC_WAKEUP_WAKEUPEN12						1 : 12
#define EIC_WAKEUP_WAKEUPEN13						1 : 13
#define EIC_WAKEUP_WAKEUPEN14						1 : 14
#define EIC_WAKEUP_WAKEUPEN15						1 : 15

/* CONFIGn - Configuration n[0..1] */
#define EIC_CONFIG_SENSEx						3 : 0
#define EIC_CONFIG_FILTEN0						1 : 3
/*
#	define EIC_NONE				0x00
#	define EIC_RISE				0x01
#	define EIC_FALL				0x02
#	define EIC_BOTH				0x03
#	define EIC_HIGH				0x04
#	define EIC_LOW				0x05
*/

#define EIC	((volatile struct samd21_eic *)SAMD21_EIC_BASE)

void eic_reset(void);
void eic_enable(uint8_t s);
void eic_enable_interrupt(uint32_t pins);
void eic_disable_interrupt(uint32_t pins);
int eic_interrupt_flag(void);
void eic_clr_interrupt(uint32_t pins);
void eic_set_config(uint8_t pin, uint8_t filter, uint8_t sense);

#endif /* SAMDX_EIC_H */
