/*
 * This file is part of the libopencm3 project and was heavily borrowed from
 * printf BSD
 *
 * Integration by Flirc Inc. Jason Kotzin 2018 <jasonkotzin@gmail.com
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

#ifndef _PRINTF_H
#define _PRINTF_H

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#ifndef _STDIO_H
typedef void *FILE;
#endif

extern int (*_getc)(void);
extern void (*_putc)(char);

#define stdin  int (*_getc)(void)
#define stdout void (*_putc)(char)

#ifndef EOF
#define EOF -1
#endif

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
int  vsnprintf(char *buf, size_t size, const char *fmt, va_list ap);

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
int  snprintf(char *buf, size_t size, const char *fmt, ...);

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
int sprintf(char *buf, const char *fmt, ...);

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
int __attribute__((format(__printf__, 1, 0))) vprintf(const char *fmt,
                                                      va_list ap);

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
int __attribute__((format(__printf__, 1, 2))) printf(const char *fmt, ...);

/**
 * dfprintf() is wrapper for printf() which first checks if the debug flag(s)
 *            presented is within the mask of requested output flags.  If not,
 *            nothing is printed.
 *
 * @param [in]  mask - Flag(s) which will be matched against the debug_flags
 *                     global.
 * @param [in]  fmt  - A string describing the format of the output.  This
 *                     format string is compatible with that of printf().
 * @param [in]  ...  - A variable list of arguments.
 *
 * @return      None.
 */
void __attribute__((format(__printf__, 2, 3))) dprintf(uint32_t mask,
                                                       const char *fmt, ...);

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
void err(int rc, const char *fmt, ...);

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
void warn(const char *fmt, ...);

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
int  fprintf(FILE *fp, const char *fmt, ...);

/**
 * puts() is a stdio compatible function which operates on a string
 *        buffer, sending output to the serial console.  A newline is
 *        automatically appended to the output.
 *
 * @param [in]  str - The string to output.
 *
 * @return      0 = Success.
 * @return      1 = Failure.
 */
int  puts(const char *str);

#endif /* _PRINTF_H */
