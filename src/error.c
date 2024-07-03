#pragma once
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void __attribute__((noreturn)) __throw_error(const char *file, int line, const char *format, ...)  {
    va_list args;

    fprintf(stderr, "Error at %s:%d: ", file, line);
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fprintf(stderr, "\n");
    exit(1);
}

#define throw_error(...) __throw_error(__FILE__, __LINE__, __VA_ARGS__)
#define assert(XXXX) if (!(XXXX)) throw_error("Assertion failed!")
#define assert_eq(XXXX, YYYYY) if ( (XXXX) != (YYYYY)) throw_error("Assertion failed! Left (%x) side != right side (%x)!", (XXXX), (YYYYY))
#define assert_neq(XXXX, YYYYY) if ( (XXXX) == (YYYYY)) throw_error("Assertion failed! Left (%x) side == right side (%x)!", (XXXX), (YYYYY))