#include <stdint.h>
#include <stdio.h>
#include "fileutils/tiFile.h"
#include "fileutils/linkFormats.h"
#include <stdlib.h>
#include <stdio.h>
#include "error.c"
#include "hexUtils.c"

int main(){
    FILE* fp = fopen("bins/8xkFiles/helloWorld.8xk", "r");
    fseek(fp, 0L, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    unsigned char* data = malloc(size);
    fread(data, size, 1, fp);
    fclose(fp);

    uint8_t x[] = {0x59, 2, 0};

    PtrLenPair pair = hexToBin("FEEDBEEF", 4);


    printf("We got %d\n", pair.length);
    printf("%x %x %x %x\n", pair.data[0], pair.data[1], pair.data[2], pair.data[3]);

    return 0;
}