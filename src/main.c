#include <stdio.h>
#include "fileutils/tiFile.h"
#include "fileutils/linkFormats.h"
#include <stdlib.h>
#include <stdio.h>

int main(){
    FILE* fp = fopen("bins/8xkFiles/helloWorld.8xk", "r");
    fseek(fp, 0L, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    unsigned char* data = malloc(size);
    fread(data, size, 1, fp);
    fclose(fp);


    printf("We got %d\n", size);
    

    return 0;
}