#pragma once
#include "tiFile.h"

#define length(array) (sizeof(array)/sizeof(*(array)))
#define ARRAY_LENGTH(array) (sizeof((array))/sizeof((array)[0]))
#define SIZE(array, type) (sizeof(array) / (sizeof(type)))


const Field FLASH_FILE_FIELDS[] = {
    {REQUIRED_BYTES_FIELD, "Magic Number", NULL, "**TIFL**"},
    {UCHAR_FIELD, "Major Version"},
    {UCHAR_FIELD, "Minor Version"},
    {UCHAR_FIELD, "Flags"},
    {UCHAR_FIELD, "Object type"},

    {FIXED_BYTES_FIELD, "Binary coded date", NULL, NULL, 4},
    
    {UCHAR_FIELD, "Name length"},
    {FIXED_STRING_FIELD, "Name", NULL, NULL, 8},

    {FIXED_BYTES_FIELD, "Filler Data", NULL, NULL, 23},

    {UCHAR_FIELD, "Device Type"},
    {UCHAR_FIELD, "Data Type"},

    {FIXED_BYTES_FIELD, "Filler Data2", NULL, NULL, 24},

    {UINT_FIELD, "HexData length"}


};

const LinkFileTemplate FLASH_FILE = {
    SIZE(FLASH_FILE_FIELDS, Field),
    FLASH_FILE_FIELDS
};