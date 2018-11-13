module varbyte;

import std.stdio;

void writeVarbyte(File fp, size_t num)
{
    ubyte[] bytestream;

    while(1) {
        bytestream ~= (num & 127) + 128;
        if(num < 128)
            break;
        num >>= 7;
    }

    bytestream[0] -= 128;

    fp.rawWrite(bytestream);
}