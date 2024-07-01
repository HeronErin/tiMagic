#pragma once
#include "tiFile.c"
#include <stdint.h>
#include <stdlib.h>

enum FieldType {
    UCHAR_FIELD,
    USHORT_FIELD,
    USHORT_CHECKSUM_FIELD,

    UINT_FIELD,

    FIXED_STRING_FIELD,
    FLOATING_STRING_FIELD,

    FIXED_BYTES_FIELD,
    FLOATING_BYTES_FIELD,

    GREEDY_BYTES_FIELD,
    REQUIRED_BYTES_FIELD,
};


typedef struct {
    enum FieldType type;
    const char* id;
    const char* deps;
    const char* byte_input;
    size_t len;
    const char* data;
} Field;


