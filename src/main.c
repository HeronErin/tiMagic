#include <stdint.h>
#include <stdio.h>
#include "fileutils/tiFile.h"
#include "fileutils/linkFormats.h"
#include "fileutils/tiHex.h"
#include <stdlib.h>
#include <stdio.h>
#include "error.c"
#include "hexUtils.c"

int main(){
    FILE* fp = fopen("bins/ti84plus_2.55.8xu", "r");
    fseek(fp, 0L, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    unsigned char* data = malloc(size);
    fread(data, size, 1, fp);
    fclose(fp);


    LinkFileResult f = parseFile(FLASH_FILE, size, data);
    Field* fi = findFieldById(f, "HexData");

    HexFile file = parseTiHex(fi->data, fi->length);
    printf("%lu:\n", file.size);
    printf("- %lu\n", file.chunks[0].declaredPage);
    printf("- %lu\n", file.chunks[1].declaredPage);
    printf("- %lu\n", file.chunks[2].declaredPage);
    printf("- %lu\n", file.chunks[3].declaredPage);



    freeFile(f);
    freeHexFile(file);

    return 0;
}