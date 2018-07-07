#ifndef I__BITFIELD_H__
#define I__BITFIELD_H__

/* Bitfield Macros
 *
 * A bitfield is any range of bits in a word. A bitfield definition
 * needs to specify the SIZE and SHIFT in the following format:
 * 	#define BITFIELD_NAME SIZE:SHIFT
 * Examples (X is don't care, F is the field):
 * 	Register: XXXXXFFX = Bitfield 2:1
 * 	Register: XXXXXFFF = Bitfield 3:0
 * 	Register: XXXXFXXX = Bitfield 1:3
 */

#define _BFSIZE(bf)	(1 ? bf)
#define _BFSHIFT(bf)	(0 ? bf)

#ifndef BF
#define BF(bfdef, bfval)			\
	(((bfval) & ((1 << _BFSIZE(bfdef)) - 1)) << _BFSHIFT(bfdef))
#endif

#ifndef INSERTBF
#define INSERTBF(bfdef, bfval, targetval)	\
	do { \
		(targetval) = ((targetval) \
			& ~(((1 << _BFSIZE(bfdef)) - 1) << _BFSHIFT(bfdef))) \
			| BF(bfdef, bfval); \
	} while (0)
#endif

#ifndef GETBF
#define GETBF(bfdef, val)			\
	((val >> _BFSHIFT(bfdef)) & ((1 << _BFSIZE(bfdef)) - 1))
#endif

#ifndef TESTBF
#define TESTBF(bfdef, testval, val)				\
	(GETBF(bfdef,val) == testval)
#endif

#ifndef _BV
#define _BV(x)		(1 << x )
#endif
/*******************************************************************************
 * clear bit and set bit macros
 * 	We do this a lot, set individual bits and clear individual bits
 */
#define sbi(reg, bit_number)    reg |= _BV(bit_number)
#define cbi(reg, bit_number)    reg &= ~_BV(bit_number)
#define tbi(reg, bit_number)	reg ^= _BV(bit_number)

#ifndef IS_ODD
#define IS_ODD(x)               (x & 0x1)
#endif

#ifndef IS_EVEN
#define IS_EVEN(x)              (!(IS_ODD(x)))
#endif

#ifndef bit_is_set
#define bit_is_set(reg, bit)	(reg && _BV(bit))
#endif

#ifndef bit_is_clr
#define bit_is_clr(reg, bit)	(!(bit_is_set(reg, bit)))
#endif

/******************************************************************************/

/*******************************************************************************
 * Useful Masking macros
 */
#define HIGH_BYTE(x)		(x>>8)
#define LOW_BYTE(x)		(x&0xFF)
/******************************************************************************/

#ifdef __GNUC__

#ifndef __alias
#define __alias		__attribute__((__may_alias__))
#endif

#define likely(x)	__builtin_expect(!!(x), 1)
#define unlikely(x)	__builtin_expect(!!(x), 0)

#ifndef __packed
#define __packed	__attribute__((packed))
#endif

#ifndef __unused
#define __unused	__attribute__((unused))
#endif

#define __constructor	__attribute__((constructor))

#define __must_check	__attribute__((warn_unused_result))

#define __noreturn	__attribute__((noreturn))

#define __printf_check(istr, iarg) \
			__attribute__((format (printf, istr, iarg)))

#define ACCESS_ONCE(x)	(*(volatile typeof(x) *)&(x))

#define __compiler_offsetof(TYPE, MEMBER) \
			__builtin_offsetof(TYPE, MEMBER)

/* END __GNUC__ */
#else

/* No Supported Compiler Found */
#error Compiler Not Supported

#endif

#endif /* I__BITFIELD_H__ */
