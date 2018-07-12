/*
 * Copyright (c) 1993
 *      The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *      @(#)printf.c    8.1 (Berkeley) 6/11/93
 */

/*
 * Scaled down version of printf(3).
 */

#include <stdint.h>
#include <sys/types.h>
#include <libopencm3/ushell/printf.h>

/* Compatibility types */
#define INTMAX_T      int32_t  // Signed largest integer
#define UINTMAX_T     uint32_t // Unsigned largest integer
#define PTRDIFF_T     int32_t  // Signed difference between two pointers


/* Output formatting flags */
#define LF_LONG       0x0001   // Value is a 32-bit long integer
#define LF_LLONG      0x0002   // Value is a 64-bit long long integer
#define LF_ALT        0x0004   // Prefix octal by "0" and hex by "0x"
#define LF_SPACE      0x0008   // If the value is positive, pad sign with ' '
#define LF_LJUST      0x0010   // left justify (padding on right side)
#define LF_SIGN       0x0020   // If the value is positive, print '+' for sign
#define LF_ZEROPAD    0x0040   // Zero-pad (on the left) to the specified width
#define LF_NEGATIVE   0x0080   // Value is negative
#define LF_UPPERCASE  0x0100   // Uppercase hex (A-F) instead of (a-f)

/* Output buffer structure */
typedef struct {
    char *buf_cur;
    char *buf_end;
} buf_t;

void put(int8_t ch, buf_t *desc);
/**
 * put() sends the specified character either to a buffer to the console.
 *
 * @param [in]  ch   - The character to output.
 * @param [out] desc - A pointer to the buffer structure describing the
 *                     storage buffer for the output character.  If this
 *                     pointer is NULL, then output should instead be
 *                     directed to the serial console.  The buffer
 *                     structure contains a pointer to the current position
 *                     in the buffer where the next character is to be
 *                     stored.  If this pointer reaches the end of buffer
 *                     pointer, no additional characters will be stored in
 *                     the buffer.  This allows the calling function to
 *                     insert a NIL ('\0') at the tail of the output string.
 *
 * @return      None.
 *
 * @see         kprintn() and kdoprnt().
 */
void
put(int8_t ch, buf_t *desc)
{
    if (desc == NULL) {
        _putc((uint8_t) ch);
    } else if (desc->buf_cur < desc->buf_end) {
        *(desc->buf_cur)++ = (char) ch;
    }
}

/**
 * kprintn() converts a binary number to ASCII in the specified base.
 *           Common bases are 2, 8, 10, and 16.  Bases up to 36 are supported.
 *
 * This function is an enhanced port of the NetBSD kernel function of the
 * same name.
 *
 * @param [out] desc  - Output buffer descriptor.  If NULL, output will be
 *                      directed to the serial console.
 * @param [in]  value - Binary value to be converted to ASCII.
 * @param [in]  base  - Base in which the converted value is to be expressed.
 *                      Common bases are 2, 8, 10, and 16.  The minimum base
 *                      value supported is 2, and the maximum is 36.
 * @param [in]  flags - Output formatting flags.  The following are available:
 *                          LF_ALT       - alternate representation (octal
 *                                         prefixed by 0 and hex prefixed by 0x)
 *                          LF_LJUST     - output should be left justified in
 *                                         the specified field.  The default is
 *                                         to right justify output.
 *                          LF_NEGATIVE  - output value is negative.
 *                          LF_SIGN      - output should always include the
 *                                         sign of the value ("+" or "-").
 *                          LF_SPACE     - output should have a space in the
 *                                         position where a negative sign would
 *                                         otherwise be placed.
 *                          LF_UPPERCASE - uppercase hexadecimal (A-F)
 *                          LF_ZEROPAD   - output should be zero padded (on the
 *                                         left) to the specified width.
 * @param [in]  width - The width of the output field in digits.  If zero or
 *                      less, the output field will not be padded.
 *
 * @return      None.
 *
 * @see         kdoprnt().
 */
static void
kprintn(buf_t *desc, UINTMAX_T value, uint32_t base, int flags, int width)
{
    /* hold a INTMAX_T in base 8 */
    char *p, buf[(sizeof (INTMAX_T) * 8 / 3) + 1 + 2 /* LF_ALT + LF_SIGN */];
    char *q;
    char  hex_a = 'a' - 10;
    char  lpad;

    if (flags & LF_UPPERCASE)
        hex_a = 'A' - 10;  // Use uppercase hex
    if (base < 2)
        base = 16;

    p = buf;
    do {
        uint32_t digit = value % base;
        *p++ = (char) (digit + ((digit <= 9) ? '0' : hex_a));
    } while ((value /= base) != 0);

    q = p;
    if (flags & LF_ALT && *(p - 1) != '0') {
        if (base == 8) {
            *p++ = '0';
        } else if (base == 16) {
            *p++ = 'x';
            *p++ = '0';
        }
    }
    if (flags & LF_NEGATIVE)
        *p++ = '-';
    else if (flags & LF_SIGN)
        *p++ = '+';
    else if (flags & LF_SPACE)
        *p++ = ' ';

    if (flags & LF_ZEROPAD)
        lpad = '0';
    else
        lpad = ' ';
    width -= p - buf;
    if ((flags & LF_LJUST) == 0)
        while (width-- > 0)
            put(lpad, desc);
    if ((flags & LF_LJUST) == 0)
        while (p > q)
            put(*--p, desc);
#if 0
    /*
     * XXX: Could not find a use case for the following, but left
     *      the code here just in case there is a zero-pad format which
     *      is not currently being tested.
     */
    if ((flags & (LF_ZEROPAD | LF_LJUST)) == LF_ZEROPAD) {
        while (width-- > 0)
            put('0', desc);
    }
#endif
    do {
        put(*--p, desc);
    } while (p > buf);

    if ((flags & (LF_ZEROPAD | LF_LJUST)) == LF_LJUST)
        while (width-- > 0)
            put(' ', desc);
}


/**
 * kdoprnt() processes a format string and a variable argument list to
 *           generate formatted output in provided buffer.
 *
 * This function is an enhanced port of the NetBSD kernel function of the
 * same name.
 *
 * @param [out] desc  - Output buffer descriptor.  If NULL, output will be
 *                      directed to the serial console.
 * @param [in]  fmt   - A string describing the format of the output.
 *                      This format string is implemented to be as close
 *                      as possible to that of the UNIX stdio printf()
 *                      function.
 * @param [in]  ap    - A pointer to a variable list of arguments.
 *
 * @return      None.
 *
 * @see         kprintn().
 */
static void
kdoprnt(buf_t *desc, const char *fmt, va_list const ap)
{
    char      *p;
    int       ch;
    UINTMAX_T ul;
    int       flags;
    int       width;
    int       mwidth;
    char     *q;

    for (;;) {
        while ((ch = *fmt++) != '%') {
            if (ch == '\0')
                return;
            put(ch, desc);
        }
        flags  = 0;
        width  = 0;
        mwidth = 2048;  // Limit max string output
reswitch:
        switch (ch = *fmt++) {
            case '#':
                flags |= LF_ALT;
                goto reswitch;
            case ' ':
                flags |= LF_SPACE;
                goto reswitch;
            case '-':
                flags |= LF_LJUST;
                goto reswitch;
            case '+':
                flags |= LF_SIGN;
                goto reswitch;
            case '*':
                width = va_arg(ap, int);
                if (width < 0) {
                    flags |= LF_LJUST;
                    width = -width;
                }
                goto reswitch;
            case '.':
                ch = *fmt++;
                if (ch == '*') {
                    mwidth = va_arg(ap, int);
                } else {
                    mwidth = 0;
                    for (mwidth = 0; ch >= '0' && ch <= '9'; ch = *fmt++)
                        mwidth = (mwidth * 10) + (ch - '0');
                    fmt--;
                }
                goto reswitch;
            case '0':
                flags |= LF_ZEROPAD;
                goto reswitch;
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                for (;;) {
                    width *= 10;
                    width += ch - '0';
                    ch = *fmt;
                    if ((unsigned)ch - '0' > 9)
                        break;
                    ++fmt;
                }
                goto reswitch;
            case 'l':
                if (*fmt == 'l') {
                    ++fmt;
                    flags |= LF_LLONG;
                } else {
                    flags |= LF_LONG;
                }
                goto reswitch;
            case 't':
                if (sizeof (PTRDIFF_T) == sizeof (long))
                    flags |= LF_LONG;
                goto reswitch;
            case 'z':
                if (sizeof (ssize_t) == sizeof (long))
                    flags |= LF_LONG;
                goto reswitch;
            case 'c':
                ch = va_arg(ap, int);
                --width;
                if ((flags & LF_LJUST) == 0)
                    while (width-- > 0)
                        put(' ', desc);
                put(ch & 0xFF, desc);
                if ((flags & (LF_ZEROPAD | LF_LJUST)) == LF_LJUST)
                    while (width-- > 0)
                        put(' ', desc);
                break;
            case 's':
                p = va_arg(ap, char *);
                for (q = p; *q != '\0'; ++q)
                    continue;
                width -= q - p;
                if ((flags & LF_LJUST) == 0)
                    while (width-- > 0)
                        put(' ', desc);
                while ((ch = (unsigned char)*p++)) {
                    if (mwidth-- == 0)
                        break;
                    put(ch, desc);
                }
                if ((flags & (LF_ZEROPAD | LF_LJUST)) == LF_LJUST)
                    while (width-- > 0)
                        put(' ', desc);
                break;
            case 'd':
                ul = (flags & LF_LLONG) ? va_arg(ap, int32_t) :
                     (flags & LF_LONG)  ? va_arg(ap, long)    :
                                          va_arg(ap, int);
                if ((INTMAX_T)ul < 0) {
                    ul = (UINTMAX_T) -(INTMAX_T)ul;
                    flags |= LF_NEGATIVE;
                }
                kprintn(desc, ul, 10, flags, width);
                break;
            case 'o':
                ul = (flags & LF_LLONG) ? va_arg(ap, uint64_t)      :
                     (flags & LF_LONG)  ? va_arg(ap, unsigned long) :
                                          va_arg(ap, unsigned int);
                kprintn(desc, ul, 8, flags, width);
                break;
            case 'u':
                ul = (flags & LF_LLONG) ? va_arg(ap, uint64_t)      :
                     (flags & LF_LONG)  ? va_arg(ap, unsigned long) :
                                          va_arg(ap, unsigned int);
                kprintn(desc, ul, 10, flags, width);
                break;
            case 'p':
                flags |= (LF_LONG | LF_ALT);
                goto do_hex;
            case 'X':
                flags |= LF_UPPERCASE;
                goto do_hex;
            case 'x':
do_hex:
                ul = (flags & LF_LLONG) ? va_arg(ap, uint64_t)      :
                     (flags & LF_LONG)  ? va_arg(ap, unsigned long) :
                                          va_arg(ap, unsigned int);
                kprintn(desc, ul, 16, flags, width);
                break;
            default:
                if (ch == '\0')
                    return;
                put(ch, desc);
                break;
        }
    }
}

/**
 * vsnprintf() is a stdio compatible function which operates on a buffer, size
 *             format string, and variable argument list.  Output is directed
 *             to the specified buffer.
 *
 * @param [out] buf   - A pointer to the buffer where output is to be stored.
 * @param [in]  size  - The length of the buffer where output is to be stored.
 * @param [in]  fmt   - A string describing the format of the output.  This
 *                      format string is compatible with that of printf().
 * @param [in]  ap    - A pointer to a variable list of arguments.
 *
 * @return      The number of bytes written to the buffer.  Note that this
 *              function differs from the stdio vsnprintf() in that it will
 *              never return a count greater than the specified length of the
 *              buffer.  If the output string would exceed the buffer length,
 *              it is truncated but always terminated by a NIL ('\0') character.
 */
int
vsnprintf(char *buf, size_t size, const char *fmt, va_list ap)
{
    if (buf == NULL) {
        kdoprnt(NULL, fmt, ap);
        return (0);
    } else {
        buf_t desc;
        desc.buf_cur = buf;
        desc.buf_end = buf + size - 1;

        kdoprnt(&desc, fmt, ap);
        *desc.buf_cur = '\0';  // terminate the string
        return (desc.buf_cur - buf);
    }
}

/**
 * snprintf() is a stdio compatible function which operates on a buffer, size,
 *            format string, and variable argument list.  Output is directed
 *            to the specified buffer.
 *
 * @param [out] buf   - A pointer to the buffer where output is to be stored.
 * @param [in]  size  - The length of the buffer where output is to be stored.
 * @param [in]  fmt   - A string describing the format of the output.  This
 *                      format string is compatible with that of printf().
 * @param [in]  ...   - A variable list of arguments.
 *
 * @return      The number of bytes written to the buffer.  Note that this
 *              function differs from the stdio snprintf() in that it will
 *              never return a count greater than the specified length of
 *              the buffer.  If the output string would exceed the buffer
 *              length, it is truncated but always terminated by a NIL ('\0')
 *              character.
 */
int
snprintf(char *buf, size_t size, const char *fmt, ...)
{
    va_list ap;
    int     rc;

    va_start(ap, fmt);
    rc = vsnprintf(buf, size, fmt, ap);
    va_end(ap);

    return (rc);
}

/**
 * sprintf() is a stdio compatible function which operates on a buffer,
 *           format string, and variable argument list.  Output is directed
 *           to the specified buffer.  A maximum of 80 characters may be
 *           written to the buffer.  Use snprintf() to write larger buffers.
 *
 * @param [out] buf   - A pointer to the buffer where output is to be stored.
 * @param [in]  fmt   - A string describing the format of the output.  This
 *                      format string is compatible with that of printf().
 * @param [in]  ...   - A variable list of arguments.
 *
 * @return      The number of bytes written to the buffer.  Note that this
 *              function differs from the stdio sprintf() in that it will
 *              never return a count greater than 80 bytes.  If the output
 *              string would exceed the buffer length, it is truncated but
 *              always terminated by a NIL ('\0') character.
 *
 * @see         snprintf();
 */
int __attribute__((format(__printf__, 2, 3)))
sprintf(char *buf, const char *fmt, ...)
{
    va_list ap;
    int     rc;
    size_t  size = 80;

    va_start(ap, fmt);
    rc = vsnprintf(buf, size, fmt, ap);
    va_end(ap);

    return (rc);
}

/**
 * vprintf() is a stdio compatible function which operates on a format
 *           string and variable argument list.  Output is directed to
 *           the serial console.
 *
 * @param [in]  fmt   - A string describing the format of the output.  This
 *                      format string is compatible with that of printf().
 * @param [in]  ap    - A pointer to a variable list of arguments.
 *
 * @return      Always zero.
 */
int __attribute__((format(__printf__, 1, 0)))
vprintf(const char *fmt, va_list ap)
{
    kdoprnt(NULL, fmt, ap);
    return (0);
}

/**
 * printf() is a stdio compatible function which operates on a format
 *          string and variable argument list.  Output is directed to
 *          the serial console.
 *
 * @param [in]  fmt - A string describing the format of the output.  This
 *                    format string is compatible with that of printf().
 * @param [in]  ... - A variable list of arguments.
 *
 * @return      Always zero.
 */
int __attribute__((format(__printf__, 1, 2)))
printf(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    (void) vprintf(fmt, args);
    va_end(args);
    return (0);
}

/**
 * dprintf() is wrapper for printf() which first checks if the debug flag(s)
 *           presented is within the mask of requested output flags.  If not,
 *           nothing is printed.
 *
 * @param [in]  mask - Flag(s) which will be matched against the debug_flags
 *                     global.
 * @param [in]  fmt  - A string describing the format of the output.  This
 *                     format string is compatible with that of printf().
 * @param [in]  ...  - A variable list of arguments.
 *
 * @return      None.
 */
/*
void __attribute__((format(__printf__, 2, 3)))
dprintf(uint32_t mask, const char *fmt, ...)
{
    va_list args;

    if (debug_flags & mask) {
        va_start(args, fmt);
        (void) vprintf(fmt, args);
        va_end(args);
    }
}
*/

/**
 * fprintf() is a stdio compatible function which operates on a format
 *           string and variable argument list.  Output is directed to
 *           the serial console (not to a file).
 *
 * @param [in]  fp  - Ignored.
 * @param [in]  fmt - A string describing the format of the output.  This
 *                    format string is compatible with that of printf().
 * @param [in]  ... - A variable list of arguments.
 *
 * @return      Always zero.
 */
int __attribute__((format(__printf__, 2, 3)))
fprintf(FILE *fp, const char *fmt, ...)
{
    (void) fp;
    va_list args;

    va_start(args, fmt);
    (void) vprintf(fmt, args);
    va_end(args);
    return (0);
}

/**
 * warn() is a stdio compatible function which operates on a format
 *        string and variable argument list.  Output is directed to
 *        the serial console.  Note that this function also implements
 *        warnx() in Magneto.
 *
 * @param [in]  fmt - A string describing the format of the output.  This
 *                    format string is compatible with that of printf().
 * @param [in]  ... - A variable list of arguments.
 *
 * @return      None.
 */
void __attribute__((format(__printf__, 1, 2)))
warn(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    (void) vprintf(fmt, args);
    va_end(args);
    _putc('\n');
}

/**
 * err() is a stdio compatible function which operates on a format
 *       string and variable argument list.  Output is directed to
 *       the serial console.  Note that this function also implements
 *       errx() in Magneto.
 *
 * @param [in]  rc  - Ignored.
 * @param [in]  fmt - A string describing the format of the output.  This
 *                    format string is compatible with that of printf().
 * @param [in]  ... - A variable list of arguments.
 *
 * @return      None.
 */
void __attribute__((format(__printf__, 2, 3)))
err(int rc, const char *fmt, ...)
{
    (void) rc;
    va_list args;

    va_start(args, fmt);
    (void) vprintf(fmt, args);
    va_end(args);
    _putc('\n');
    puts("Should panic here.");
}

//int _putc(uint8_t ch)
//{
	//return _putc(ch);
//}


#ifdef UART_TEST
void uart_testprintf_str(void) {
    static char shortstr[] = "Hi, Z.";

    printf("zero-padded string:\t\"%010s\"\n", shortstr);
    printf("left-adjusted Z string:\t\"%-010s\"\n", shortstr);
    printf("space-padded string:\t\"%10s\"\n", shortstr);
    printf("left-adjusted S string:\t\"%-10s\"\n", shortstr);
    printf("null string:\t\"%s\"\n", (char *)NULL);
    //printf("decimal negative:\t\"%d\"\n", -2345);

    /*
    printf("octal negative:\t\"%o\"\n", -2345);
    printf("hex negative:\t\"%x\"\n", -2345);
    printf("long decimal number:\t\"%ld\"\n", -123456);
    printf("long octal negative:\t\"%lo\"\n", -2345L);
    printf("long unsigned decimal number:\t\"%lu\"\n", -123456);
    printf("zero-padded LDN:\t\"%010ld\"\n", -123456);
    printf("left-adjusted ZLDN:\t\"%-010ld\"\n", -123456);
    printf("space-padded LDN:\t\"%10ld\"\n", -123456);
    printf("left-adjusted SLDN:\t\"%-10ld\"\n", -123456);

    printf("zero-padded string:\t\"%010s\"\n", shortstr);
    printf("left-adjusted Z string:\t\"%-010s\"\n", shortstr);
    printf("space-padded string:\t\"%10s\"\n", shortstr);
    printf("left-adjusted S string:\t\"%-10s\"\n", shortstr);
    printf("null string:\t\"%s\"\n", (char *)NULL);
    #if 0
    printf("limited string:\t\"%.22s\"\n", longstr);
    #endif

    */
}

void uart_testprintf_num(void) {
    printf("e-style >= 1:\t\"%e\"\n", 12.34);
    printf("e-style >= .1:\t\"%e\"\n", 0.1234);
    printf("e-style < .1:\t\"%e\"\n", 0.001234);
    printf("e-style big:\t\"%.60e\"\n", 1e20);
    printf ("e-style == .1:\t\"%e\"\n", 0.1);
    printf("f-style >= 1:\t\"%f\"\n", 12.34);
    printf("f-style >= .1:\t\"%f\"\n", 0.1234);
    printf("f-style < .1:\t\"%f\"\n", 0.001234);
    printf("g-style >= 1:\t\"%g\"\n", 12.34);
    printf("g-style >= .1:\t\"%g\"\n", 0.1234);
    printf("g-style < .1:\t\"%g\"\n", 0.001234);
    printf("g-style big:\t\"%.60g\"\n", 1e20);

    printf (" %6.5lf\n", .099999999860301614);
    printf (" %6.5lf\n", .1);
    printf ("x%5.4lfx\n", .5);

    printf ("%#03x\n", 1);

    #define FORMAT "|%12.4f|%12.4e|%12.4g|\n"
    printf (FORMAT, 0.0, 0.0, 0.0);
    printf (FORMAT, 1.0, 1.0, 1.0);
    printf (FORMAT, -1.0, -1.0, -1.0);
    printf (FORMAT, 100.0, 100.0, 100.0);
    printf (FORMAT, 1000.0, 1000.0, 1000.0);
    printf (FORMAT, 10000.0, 10000.0, 10000.0);
    printf (FORMAT, 12345.0, 12345.0, 12345.0);
    printf (FORMAT, 100000.0, 100000.0, 100000.0);
    printf (FORMAT, 123456.0, 123456.0, 123456.0);
    #undef  FORMAT

    printf ("%e should be 1.234568e+06\n", 1234567.8);
    printf ("%f should be 1234567.800000\n", 1234567.8);
    printf ("%g should be 1.23457e+06\n", 1234567.8);
    printf ("%g should be 123.456\n", 123.456);
    printf ("%g should be 1e+06\n", 1000000.0);
    printf ("%g should be 10\n", 10.0);
    printf ("%g should be 0.02\n", 0.02);
}
#endif
// #endif /* DO_PRINTF_TEST */
