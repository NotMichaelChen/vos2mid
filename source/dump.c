#include "dump.h"

#include "endian.h"
#include "varbyte.h"

void dump_trk(FILE* fp, track_t* trk);
void dump_tempo_trk(FILE* fp, track_t* trk);
void dump_writeheader(FILE* fp, smf_t* smf);

void dump_smf(FILE* fp, smf_t* smf)
{
    dump_writeheader(fp, smf);

    dump_tempo_trk(fp, &smf->ttempo);

    for(size_t i = 0; i < smf->ntracks; ++i)
        dump_trk(fp, &(smf->tracks[i]));
}

void dump_writeheader(FILE* fp, smf_t* smf)
{
    fputs("MThd", fp);

    uint32_t headerlen = 6;
    headerlen = endian_swapInt(headerlen);
    fwrite(&headerlen, 4, 1, fp);

    uint16_t format = endian_swapShort(smf->format);
    fwrite(&format, 2, 1, fp);

    uint16_t n = endian_swapShort(smf->ntracks);
    fwrite(&n, 2, 1, fp);

    uint16_t division = endian_swapShort(smf->division);
    fwrite(&division, 2, 1, fp);
}

void dump_tempo_trk(FILE* fp, track_t* trk)
{
    fputs("MTrk", fp);
    //Get location of length to return & write later
    size_t lenloc = ftell(fp);
    //Put placeholder length here
    int temp = 0;
    fwrite(&temp, 4, 1, fp);

    size_t lenbegin = ftell(fp);

    size_t prevtime = 0;
    for(size_t i = 0; i < trk->nevents; ++i)
    {
        size_t delta = trk->events[i].tick - prevtime;
        writeVarbyte(fp, delta);
        prevtime = trk->events[i].tick;

        putc(0xFF, fp);
        //TODO: To make more general check for 4 bytes used and write time signature if so (0x58)
        //instead of Set Tempo (0x51)
        putc(0x51, fp);
        putc(0x03, fp);

        unsigned int bigend = endian_swapInt(trk->events[i].data);
        //First byte is now 0, so skip it
        fwrite((char*)(&bigend) + 1, 1, 3, fp);
    }

    putc(0x00, fp);
    putc(0xFF, fp);
    putc(0x2F, fp);
    putc(0x00, fp);

    size_t lenend = ftell(fp);
    fseek(fp, lenloc, SEEK_SET);
    unsigned int len = endian_swapInt(lenend - lenbegin);
    fwrite(&len, sizeof(unsigned int), 1, fp);
    fseek(fp, lenend, SEEK_SET);
}

void dump_trk(FILE* fp, track_t* trk)
{
    fputs("MTrk", fp);
    //Get location of length to return & write later
    size_t lenloc = ftell(fp);
    //Put placeholder length here
    int temp = 0;
    fwrite(&temp, 4, 1, fp);

    size_t lenbegin = ftell(fp);

    size_t prevtime = 0;
    for(size_t i = 0; i < trk->nevents; ++i)
    {
        size_t delta = trk->events[i].tick - prevtime;
        writeVarbyte(fp, delta);
        prevtime = trk->events[i].tick;

        if(trk->events[i].byte[0] >= 0xC0 && trk->events[i].byte[0] <= 0xCF)
            fwrite(trk->events[i].byte, 1, 2, fp);
        else
            fwrite(trk->events[i].byte, 1, 3, fp);
    }

    putc(0x00, fp);
    putc(0xFF, fp);
    putc(0x2F, fp);
    putc(0x00, fp);

    size_t lenend = ftell(fp);
    fseek(fp, lenloc, SEEK_SET);
    unsigned int len = endian_swapInt(lenend - lenbegin);
    fwrite(&len, sizeof(unsigned int), 1, fp);
    fseek(fp, lenend, SEEK_SET);
}
