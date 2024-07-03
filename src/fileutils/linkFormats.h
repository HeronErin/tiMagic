#pragma once
#include "tiFile.h"

#define length(array) (sizeof(array)/sizeof(*(array)))
#define ARRAY_LENGTH(array) (sizeof((array))/sizeof((array)[0]))
#define SIZE(array, type) (sizeof(array) / (sizeof(type)))

enum DeviceType{
    DEVICE_TI73 = 0x74,
    DEVICE_TI83p = 0x73,
    DEVICE_TI89 = 0x98,
    DEVICE_TI92 = 0x88
};
enum DataType{
    OS = 0x23,
    APPLICATION = 0x24,
    CERTIFICATE =  0x25,
    LICENSE = 0x3E
};
enum TypeID{
    TYPE_RealNumber = 0x00,
    TYPE_RealList = 0x01,
    TYPE_Matrix = 0x02,
    TYPE_YVariable = 0x03,
    TYPE_String = 0x04,
    TYPE_Program = 0x05,
    TYPE_EditLockedProgram = 0x06,
    TYPE_Picture = 0x07,
    TYPE_GraphicsDatabase = 0x08,
    TYPE_WindowSettings = 0x0B,
    TYPE_ComplexNumber = 0x0C,
    TYPE_ComplexListNumber = 0x0D,
    TYPE_WindowSettings2 = 0x0F,
    TYPE_SavedWindowSettings = 0x10,
    TYPE_TableSetup = 0x11,
    TYPE_Backup = 0x13,
    TYPE_DeleteFlashApplication = 0x14,
    TYPE_ApplicationVariable = 0x15,
    TYPE_GroupVariable = 0x16,
    TYPE_Directory = 0x19,
    TYPE_OS = 0x23,
    TYPE_IdList = 0x26,
    TYPE_GetCertificate = 0x27,
    TYPE_Clock = 0x29
};

const Field FLASH_FILE_FIELDS[] = {
    {REQUIRED_BYTES_FIELD, "Magic Number", NULL, "**TIFL**"},
    {UCHAR_FIELD, "Major Version"},
    {UCHAR_FIELD, "Minor Version"},
    {UCHAR_FIELD, "Flags"},
    {UCHAR_FIELD, "Object type"},

    {FIXED_BYTES_FIELD, "Binary coded date", NULL, NULL, 4},
    
    {UCHAR_FIELD, "Name length"},
    {FLOATING_STRING_FIELD, "Name", "Name length", NULL},

    {FIXED_BYTES_FIELD, "Filler Data", NULL, NULL, 23},

    {UCHAR_FIELD, "Device Type"},
    {UCHAR_FIELD, "Data Type"},

    {FIXED_BYTES_FIELD, "Filler Data2", NULL, NULL, 24},

    {UINT_FIELD, "HexData length"},

    {FLOATING_BYTES_FIELD, "HexData", "HexData length"}
};

const LinkFileTemplate FLASH_FILE = {
    SIZE(FLASH_FILE_FIELDS, Field),
    FLASH_FILE_FIELDS
};

const Field VAR_FILE_FIELDS[] = {
    {REQUIRED_BYTES_FIELD, "Magic Number", NULL, "**TI83F*\xA1\x0A"},
    {UCHAR_FIELD, ""}, // Always null (part of magic number)
    {FIXED_STRING_FIELD, "Comment", NULL, NULL, 42},
    {USHORT_FIELD, "Data length"},

    // Typically a var entry
    {FLOATING_BYTES_FIELD, "Data", "Data length"},

    {USHORT_CHECKSUM_FIELD, "Checksum", "Data"}
};

const Field VAR_FIELD_ENTRY[] = {
    // Always has a value of 11 or 13 (Bh or Dh).
    {FIXED_BYTES_FIELD, "Constant", NULL, NULL, 2},
    {USHORT_FIELD, "Var length"},
    {UCHAR_FIELD, "Var id"},
    {FIXED_STRING_FIELD, "Name", NULL, NULL, 8},
    {UCHAR_FIELD, "Version"},
    {UCHAR_FIELD, "Flag"},
    {USHORT_FIELD, "Var length2"},
    {USHORT_FIELD, "Var length3"},
    {FLOATING_BYTES_FIELD, "VarData", "Var length2"}
};