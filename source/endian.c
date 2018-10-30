#include "endian.h"

//https://stackoverflow.com/a/2182184

uint16_t endian_swapShort(uint16_t num)
{
    uint16_t swapped = (num>>8) | (num<<8);
    return swapped;
}

uint32_t endian_swapInt(uint32_t num)
{
    uint32_t swapped = ((num>>24)&0xff) |
                    ((num<<8)&0xff0000) |
                    ((num>>8)&0xff00) |
                    ((num<<24)&0xff000000);
    return swapped;
}
