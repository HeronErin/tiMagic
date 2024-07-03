#pragma once
#include <stdint.h>
#include <stdlib.h>

enum FieldType {
    UCHAR_FIELD,            // 0
    USHORT_FIELD,           // 1
    USHORT_CHECKSUM_FIELD,  // 2

    UINT_FIELD,             // 3

    FIXED_STRING_FIELD,     // 4
    FLOATING_STRING_FIELD,  // 5

    FIXED_BYTES_FIELD,      // 6
    FLOATING_BYTES_FIELD,   // 7

    GREEDY_BYTES_FIELD,     // 8
    REQUIRED_BYTES_FIELD,   // 9
};


typedef struct {
    enum FieldType type;
    const char* id;
    const char* dependsOn;
    const char* byte_input;
    size_t length;
    uint8_t* data;
} Field;


typedef struct {
    const size_t size;
    const Field* fields;
} LinkFileTemplate;


typedef struct {
    size_t size;
    Field* fields;
} LinkFileResult;


void freeFile(LinkFileResult res);

Field* findFieldById(LinkFileResult file, const char* id);
size_t toUnsigned(Field field);
LinkFileResult parseFile(const LinkFileTemplate template, size_t size, uint8_t* data);

#include "tiFile.c"