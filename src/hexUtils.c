#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// mapping of ASCII characters to hex values
const uint8_t hexToBinMap[] =
{
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, // 01234567
    0x08, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 89:;<=>?
    0x00, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x00, // @ABCDEFG
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // HIJKLMNO
};
const uint8_t binToHexMap[16] = "0123456789ABCDEF";

uint8_t* binToHex(uint8_t* ptr, size_t length){
    uint8_t* ret = malloc(length * 2 + 1);
    ret[length * 2] = 0;

    uint8_t* itr = ret;
    while (length--){
        *(itr++) = binToHexMap[*ptr & 0xF0 >> 4];
        *(itr++) = binToHexMap[*ptr & 0x0F];
        ptr++;
    }

    return ret;
}
typedef struct{
    size_t length;
    uint8_t* data;
} PtrLenPair;

PtrLenPair hexToBin(uint8_t* ptr, size_t length){
    PtrLenPair ret;
    uint8_t* toData;
    uint8_t idx0, idx1;

    length >>= 1;

    ret.length = length;
    ret.data = malloc(ret.length);

    toData = ret.data;
    
    while (length--){
        idx0 = (*(ptr++) & 0x1F) ^ 0x10;
        idx1 = (*(ptr++) & 0x1F) ^ 0x10;

        *(toData++) = (uint8_t)(hexToBinMap[idx0] << 4) | hexToBinMap[idx1];
    }
    return ret;
}

PtrLenPair hexStrToBin(uint8_t* ptr){
    return hexToBin(ptr, strlen(ptr));
}