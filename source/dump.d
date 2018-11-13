module dump;

import std.stdio, std.bitmanip, std.algorithm;
import smf, varbyte;

void mid2smf(File fp, smf_t* smf)
{
    writeHeader(fp, smf);

    dump_tempo_trk(fp, &smf.ttempo);

    for(size_t i = 0; i < smf.ntracks; ++i)
        dump_trk(fp, &(smf.tracks[i]));
}

private void writeHeader(File fp, smf_t* smf)
{
    fp.write("MThd");

    immutable uint headerlen = 6;
    fp.rawWrite(std.bitmanip.nativeToBigEndian(headerlen));

    immutable ushort format = smf.format;
    fp.rawWrite(std.bitmanip.nativeToBigEndian(format));

    immutable ushort n = cast(ushort) smf.ntracks;
    fp.rawWrite(std.bitmanip.nativeToBigEndian(n));

    immutable ushort division = smf.division;
    fp.rawWrite(std.bitmanip.nativeToBigEndian(division));
}

private void dump_tempo_trk(File fp, track_t* trk)
{
    fp.write("MTrk");
    
    //Get location of length to return & write later
    ulong lenloc = fp.tell();
    //Put placeholder length here
    fp.rawWrite!ubyte([0, 0, 0, 0]);

    ulong lenbegin = fp.tell();

    size_t prevtime = 0;
    for(size_t i; i < trk.nevents; i++)
    {
        size_t delta = trk.events[i].tick - prevtime;
        writeVarbyte(fp, delta);
        prevtime = trk.events[i].tick;

        fp.rawWrite!ubyte([0xFF]);
        //TODO: To make more general check for 4 bytes used and write time signature if so (0x58)
        //instead of Set Tempo (0x51)
        fp.rawWrite!ubyte([0x51, 0x03]);
        //First byte is now 0, so skip it
        fp.rawWrite!ubyte(std.bitmanip.nativeToBigEndian(trk.events[i].data)[1..$]);

    }

    fp.rawWrite!ubyte([0x00, 0xFF, 0x2F, 0x00]);

    ulong lenend = fp.tell();
    fp.seek(lenloc);
    fp.rawWrite!ubyte(std.bitmanip.nativeToBigEndian(cast(uint) (lenend - lenbegin)));
    fp.seek(lenend);
}

void dump_trk(File fp, track_t* trk)
{
    fp.write("MTrk");
    
    //Get location of length to return & write later
    ulong lenloc = fp.tell();
    //Put placeholder length here
    fp.rawWrite!ubyte([0, 0, 0, 0]);

    ulong lenbegin = fp.tell();

    size_t prevtime = 0;
    for(size_t i; i < trk.nevents; i++)
    {
        size_t delta = trk.events[i].tick - prevtime;
        writeVarbyte(fp, delta);
        prevtime = trk.events[i].tick;

        if(trk.events[i].bytes[0] >= 0xC0 && trk.events[i].bytes[0] <= 0xCF)
            fp.rawWrite!ubyte(trk.events[i].bytes[0..2]);
        else
            fp.rawWrite!ubyte(trk.events[i].bytes[0..3]);
    }

    fp.rawWrite!ubyte([0x00, 0xFF, 0x2F, 0x00]);

    ulong lenend = fp.tell();
    fp.seek(lenloc);
    fp.rawWrite!ubyte(std.bitmanip.nativeToBigEndian(cast(uint) (lenend - lenbegin)));
    fp.seek(lenend);
}