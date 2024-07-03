#pragma once
#include "tiHex.h"
#include "stdint.h"
#include <stdbool.h>
#include <stdint.h>
#include "../error.c"
#include "../hexUtils.c"

#include "stdio.h"

void freeHexFile(HexFile f){
    for (size_t i = 0; i < f.size; i++){
        free(f.chunks[i].data);
    }

    free(f.chunks);
}

HexLine* _makeLines(uint8_t* data, size_t size){
    size_t arrSize = 128;
    HexLine* lines = malloc(arrSize * sizeof(HexLine));

    size_t lineIndex = 0;
    size_t index = 0;

    HexLine currentLine;
    PtrLenPair hexData;

    uint8_t sumInFile;

    while (index < size){
        // Dynamically expand array
        if (arrSize <= lineIndex){
            arrSize *= 2;
            lines = realloc(lines, arrSize * sizeof(HexLine));
        }

        // Seek to ':' and ignore all else
        while (index < size && data[index++] != ':') {}
        if (index >= size) break;
        
        currentLine.count = hexByteToBin(data, &index);
        
        currentLine.address = hexByteToBin(data, &index) << 8;
        currentLine.address |= hexByteToBin(data, &index);
        
        currentLine.recordType = hexByteToBin(data, &index);

        currentLine.sum = currentLine.count 
                            + (currentLine.address >> 8) 
                            + (currentLine.address & 0xFF)
                            + currentLine.recordType;
        hexData = hexToBin(data + index, currentLine.count * 2);
        currentLine.data = hexData.data;
        index += currentLine.count * 2;

        sumInFile = hexByteToBin(data, &index);

        while (hexData.length--)
            currentLine.sum += *(hexData.data++);
        currentLine.sum = -currentLine.sum;
        

        if (sumInFile != currentLine.sum)
            throw_error("Checksum not valid on line %lu in the intellhex! This file is likely corrupt! (File: %x, Calculated: %x)\n", lineIndex, sumInFile, currentLine.sum);
        

        lines[lineIndex++] = currentLine;
    }

    // Signify end
    lines[lineIndex++].recordType = 0xFF;
    return lines;
}

HexFile parseTiHex(uint8_t* data, size_t size){
    HexFile ret;

    size_t chunksAmount = 128;
    ret.chunks = malloc(chunksAmount * sizeof(HexChunk));

    HexChunk current;
    size_t chunkIndex = 0;

    HexLine* lines = _makeLines(data, size);
    size_t lineIndex = 0; 
    bool isFirstDataline = true;

    current.address = 0;
    current.size = 0;
    current.data = NULL;

    while (lines[lineIndex].recordType != 0xFF){
        if (chunksAmount <= chunkIndex){
            chunksAmount *= 2;
            ret.chunks = realloc(ret.chunks, chunksAmount * sizeof(HexChunk));
        }

        switch (lines[lineIndex].recordType){
            case ExtendedSegmentAddress:
                if (!isFirstDataline) {
                    ret.chunks[chunkIndex++] = current;
                    current.address = 0;
                    current.size = 0;
                    current.data = NULL;
                    isFirstDataline = true;
                }
                current.declaredPage = *(ushort*)lines[lineIndex].data;
                break;
            case Data:
               if (isFirstDataline) {
                    current.address = lines[lineIndex].address;
                    isFirstDataline = false;
                }
                // Not the best approach, but it works.
                size_t oldSize = current.size;
                current.data = realloc(current.data, current.size+=lines[lineIndex].count);
                memcpy(current.data + oldSize, lines[lineIndex].data, lines[lineIndex].count);
                free(lines[lineIndex].data);
                break;
            case EndOfFile:
                if (!isFirstDataline){
                    ret.chunks[chunkIndex++] = current;
                    current.address = 0;
                    current.size = 0;
                    current.data = NULL;
                    isFirstDataline = true;
                }
                break;
            default:
                throw_error("Unsupported record type %lu!\n", lines[lineIndex].recordType);
        }
        lineIndex++;
    }
    if (!isFirstDataline)
        ret.chunks[chunkIndex++] = current;
    ret.size = chunkIndex;

    free(lines);
    return ret;
}