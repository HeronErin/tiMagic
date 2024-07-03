#pragma once
#include <stddef.h>
#include <stdint.h>


enum RecordType{
    Data = 0,
    EndOfFile,
    ExtendedSegmentAddress,
    StartSegmentAddress,
    ExtendedLinearAddress,
    StartLinearAddress
};

typedef struct {
    // Address where the data starts
    uint16_t address;

    // TI wants to be special and uses Extended Segment Address specify what page you are on
    uint16_t declaredPage;

    size_t size;
    uint8_t* data;
} HexChunk;

typedef struct{
    size_t size;
    HexChunk* chunks;
} HexFile;

typedef struct {
    uint8_t count;
    uint16_t address;
    enum RecordType recordType; // uint8_t in file
    uint8_t* data;

    uint8_t sum;
} HexLine;

void freeHexFile(HexFile f);
HexLine* _makeLines(uint8_t* data, size_t size);
HexFile parseTiHex(uint8_t* data, size_t size);

#include "tiHex.c"