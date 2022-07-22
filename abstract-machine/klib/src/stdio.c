#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)
#define INT_MAX		((int)(~0U >> 1))
#define SIGN	1		/* unsigned/signed, must be 1 */
#define LEFT	2		/* left justified */
#define PLUS	4		/* show plus */
#define SPACE	8		/* space if plus */
#define ZEROPAD	16		/* pad with zero, must be 16 == '0' - ' ' */
#define SMALL	32		/* use lowercase in hex (must be 32 == 0x20) */
#define SPECIAL	64		/* prefix hex with "0x", octal with "0" */

int printf(const char *fmt, ...) {
  panic("Not implemented");
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  panic("Not implemented");
}

int sprintf(char *out, const char *fmt, ...)
{
	va_list ap;
	int i;
	va_start(ap, fmt);
	i = vsnprintf(out, INT_MAX, fmt, ap);
	va_end(ap);
	return i;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

struct printf_spec {
	unsigned int	type:8;		/* format_type enum */
	signed int	field_width:24;	/* width of output field */
	unsigned int	flags:8;	/* flags to number() */
	unsigned int	base:8;		/* number base, 8, 10 or 16 only */
	signed int	precision:16;	/* # of digits/chars */
} __packed;

enum format_type {
	FORMAT_TYPE_NONE, /* Just a string part */
	FORMAT_TYPE_WIDTH,
	FORMAT_TYPE_PRECISION,
	FORMAT_TYPE_CHAR,
	FORMAT_TYPE_STR,
	FORMAT_TYPE_PTR,
	FORMAT_TYPE_PERCENT_CHAR,
	FORMAT_TYPE_INVALID,
	FORMAT_TYPE_LONG_LONG,
	FORMAT_TYPE_ULONG,
	FORMAT_TYPE_LONG,
	FORMAT_TYPE_UBYTE,
	FORMAT_TYPE_BYTE,
	FORMAT_TYPE_USHORT,
	FORMAT_TYPE_SHORT,
	FORMAT_TYPE_UINT,
	FORMAT_TYPE_INT,
	FORMAT_TYPE_SIZE_T,
	FORMAT_TYPE_PTRDIFF
};

int format_decode(const char *fmt, struct printf_spec *spec)
{
	const char *start = fmt;
	char qualifier;

	/* By default */
	spec->type = FORMAT_TYPE_NONE;
	for (; *fmt ; ++fmt) {
		if (*fmt == '%')
			break;
	}
	/* Return the current non-format string */
	if (fmt != start || !*fmt)
		return fmt - start;
	/* Process flags */
	spec->flags = 0;
	while (1) { /* this also skips first '%' */
		bool found = true;
		++fmt;
		switch (*fmt) {
		case '-': spec->flags |= LEFT;    break;
		case '+': spec->flags |= PLUS;    break;
		case ' ': spec->flags |= SPACE;   break;
		case '#': spec->flags |= SPECIAL; break;
		case '0': spec->flags |= ZEROPAD; break;
		default:  found = false;
		}
		if (!found)
			break;
	}

	qualifier = 0;
	if (*fmt == 'h' || *fmt == 'l' ||
	    *fmt == 'z' || *fmt == 't') {
		qualifier = *fmt++;
		if ((qualifier == *fmt)) {
			if (qualifier == 'l') {
				qualifier = 'L';
				++fmt;
			} else if (qualifier == 'h') {
				qualifier = 'H';
				++fmt;
			}
		}
	}
	/* default base */
	spec->base = 10;
	switch (*fmt) {
	case 'c':
		spec->type = FORMAT_TYPE_CHAR;
		return ++fmt - start;
	case 's':
		spec->type = FORMAT_TYPE_STR;
		return ++fmt - start;
	case 'p':
		spec->type = FORMAT_TYPE_PTR;
		return ++fmt - start;
	case '%':
		spec->type = FORMAT_TYPE_PERCENT_CHAR;
		return ++fmt - start;
	/* integer number formats - set up the flags and "break" */
	case 'o':
		spec->base = 8;
		break;
	case 'x':
		spec->flags |= SMALL;
		/* fall through */
	case 'X':
		spec->base = 16;
		break;
	case 'd':
	case 'i':
		spec->flags |= SIGN;
	case 'u':
		break;
	default:
		spec->type = FORMAT_TYPE_INVALID;
		return fmt - start;
	}
	if (qualifier == 'L')
		spec->type = FORMAT_TYPE_LONG_LONG;
	else if (qualifier == 'l') {
		spec->type = FORMAT_TYPE_ULONG + (spec->flags & SIGN);
	} else if (qualifier == 'z') {
		spec->type = FORMAT_TYPE_SIZE_T;
	} else if (qualifier == 't') {
		spec->type = FORMAT_TYPE_PTRDIFF;
	} else if (qualifier == 'H') {
		spec->type = FORMAT_TYPE_UBYTE + (spec->flags & SIGN);
	} else if (qualifier == 'h') {
		spec->type = FORMAT_TYPE_USHORT + (spec->flags & SIGN);
	} else {
		spec->type = FORMAT_TYPE_UINT + (spec->flags & SIGN);
	}
	return ++fmt - start;
}

char *string(char *buf, char *end, const char *s, struct printf_spec spec)
{
	while (1) {
		char c = *s++;
		if (!c)
			break;
		*buf = c;
		++buf;
	}
	return buf;
}

char *number(char *buf, char *end, unsigned long long num,
	     struct printf_spec spec)
{
	char tmp[3 * sizeof(num)];
	char sign;
	int i;
	bool is_zero = num == 0LL;

	sign = 0;
	if ((signed long long)num < 0) {
		sign = '-';
		num = -(signed long long)num;
	}
	
	i = 0;
	if(is_zero)
		tmp[i++]='0';
	
	while(num>0){
		tmp[i++]=(num%10)+'0';
		num=num/10;
	}
	if (sign) {
		*buf = sign;
		++buf;
	}
	while (--i >= 0) {
		*buf = tmp[i];
		++buf;
	}
	return buf;
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap)
{
	unsigned long long num;
	char *str, *end;
	struct printf_spec spec = {0};
	str = out;
	end = out + n;

	while (*fmt) {
		const char *old_fmt = fmt;
		int read = format_decode(fmt, &spec);
		fmt += read;
		switch (spec.type) {
		case FORMAT_TYPE_NONE: {
			int copy = read;
			if (str < end) {
				if (copy > end - str)
					copy = end - str;
				memcpy(str, old_fmt, copy);
			}
			str += read;
			break;
		}
		case FORMAT_TYPE_STR:
			str = string(str, end, va_arg(ap, char *), spec);
			break;
		default:
			switch (spec.type) {
			case FORMAT_TYPE_LONG_LONG:
				num = va_arg(ap, long long);
				break;
			case FORMAT_TYPE_ULONG:
				num = va_arg(ap, unsigned long);
				break;
			case FORMAT_TYPE_LONG:
				num = va_arg(ap, long);
				break;
			case FORMAT_TYPE_UBYTE:
				num = (unsigned char) va_arg(ap, int);
				break;
			case FORMAT_TYPE_BYTE:
				num = (signed char) va_arg(ap, int);
				break;
			case FORMAT_TYPE_USHORT:
				num = (unsigned short) va_arg(ap, int);
				break;
			case FORMAT_TYPE_SHORT:
				num = (short) va_arg(ap, int);
				break;
			case FORMAT_TYPE_INT:
				num = (int) va_arg(ap, int);
				break;
			default:
				num = va_arg(ap, unsigned int);
			}
			str = number(str, end, num, spec);
		}
	}
	
	if (n > 0) {
		if (str < end)
			*str = '\0';
		else
			end[-1] = '\0';
	}
	/* the trailing null byte doesn't count towards the total */
	return str-out; 
}

#endif
