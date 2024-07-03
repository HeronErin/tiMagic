#pragma once
#include "tiFile.h"
#include <stdint.h>
#include <stddef.h>
#include "../error.c"
#include <string.h>


Field* findFieldById(LinkFileResult file, const char* id){
    size_t length = file.size;
    size_t index = 0;
    while (length-- && file.fields[index].id){
        if (0 == strcmp(file.fields[index].id, id))
            return &file.fields[index];
        index++;
    }
    return NULL;
}
#include <endian.h>
size_t toUnsigned(Field field){
    switch (field.type) {
        case UCHAR_FIELD:
            return *field.data;
        case USHORT_FIELD:
        case USHORT_CHECKSUM_FIELD:
            return le16toh(*((uint16_t*)field.data));
        case UINT_FIELD:
            return le32toh(*((uint32_t*)field.data));
        default:
            throw_error("Can't make field into unsigned number: \"%s\"", field.id);
    }
}
ptrdiff_t toSigned(Field field){
    return toUnsigned(field);
}

void freeFile(LinkFileResult res){
    free(res.fields);
}

LinkFileResult parseFile(const LinkFileTemplate template, size_t size, uint8_t* data){
    LinkFileResult result;
    result.size = template.size;
    result.fields = calloc(1, sizeof(Field) * template.size);

    size_t index = 0;
    size_t currentField = 0;
    while (currentField < template.size){
        if (index >= size)
            throw_error("Error: unexpected end of file!");

        Field check = template.fields[currentField];
        Field output = check;
        
        switch (check.type) {
            case USHORT_FIELD:
            case USHORT_CHECKSUM_FIELD:
                assert(index + 2 < size);
                output.length = 2;
                output.data = data + index;
                index += 2;
                break;
            case UINT_FIELD:
                assert(index + 4 < size);
                output.length = 4;
                output.data = data + index;
                index += 4;
                break;
            case FLOATING_BYTES_FIELD:
            case FLOATING_STRING_FIELD:
                Field* dependsOn = findFieldById(result, check.dependsOn);
                assert_neq(NULL, dependsOn);
                output.length = toUnsigned(*dependsOn);
                output.data = data + index;
                index += output.length;
                break;
            case FIXED_STRING_FIELD:
            case FIXED_BYTES_FIELD:
                assert(index + check.length < size);
                output.data = data + index;
                index += check.length;
                break;
            case UCHAR_FIELD:
                assert(index + 1 < size);

                output.length = 1;
                output.data = data + index;
                index++;
                break;

            case REQUIRED_BYTES_FIELD:
                assert_neq(NULL, check.byte_input);
                size_t checkSize = strlen(check.byte_input);
                assert(checkSize + index < size);
                if (0 != memcmp(index + data, check.byte_input, checkSize))
                    throw_error("Required bytes not found in file for field: \"%s\"", check.id);
                index+=checkSize;
                
                output.length = checkSize;
                output.data = data + index;
                break;

            default:
                throw_error("Unhandled field type %x", check.type);
        }

        result.fields[currentField] = output;
        if (output.type == FIXED_STRING_FIELD){
            fwrite(output.data, output.length, 1, stdout);
            printf("\n");
        }
        currentField++;

    }
    printf("%lu out of %lu\n", index, size);
    

    return result;

}
