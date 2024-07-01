#pragma once
#include "tiFile.h"



const Field FLASH_FILE[] = {
    {REQUIRED_BYTES_FIELD, "Magic Number", NULL, "**TIFL**"},
    {UCHAR_FIELD, "Major Version"}
};