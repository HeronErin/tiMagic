#pragma once
#include "tiFile.h"
#include <stdint.h>
#include <stddef.h>
#include "../error.c"
#include <string.h>



LinkFileResult parseFile(const LinkFileTemplate template, size_t size, uint8_t* data){
    LinkFileResult result;
    result.size = template.size;
    result.fields = malloc(template.size * sizeof(Field));

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
                    throw_error("Required bytes not found in file for feild: \"%s\"", check.id);
                index+=checkSize;
                
                output.length = checkSize;
                output.data = data + index;
                break;

            default:
                throw_error("Unhandled field type %x", check.type);
        }

        result.fields[currentField] = output;
        printf("Field of type %x of length %x\n", output.type, output.length);
        currentField++;

    }
    

    return result;

}
