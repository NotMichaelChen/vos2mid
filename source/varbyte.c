#include "varbyte.h"

#include <stdlib.h>
#include <stdint.h>

void writeVarbyte(FILE* fp, size_t num)
{
    uint8_t bytestream[10] = {0};

    uint8_t index = 0;
    while(1) {
        bytestream[index] = (num & 127) + 128;
        if(num < 128)
            break;
        num >>= 7;
        ++index;
    }

    bytestream[0] -= 128;

    for(int i = index; i >= 0; i--) {
        fputc(bytestream[i], fp);
    }
}
