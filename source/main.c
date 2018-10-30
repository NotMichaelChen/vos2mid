#include <stdio.h>
#include <stdlib.h>

#include "vos.h"
#include "smf.h"
#include "dump.h"

int main(int argc, char **argv)
{
    if(argc < 2) {
        printf("Error: no filename provided\n");
        printf("Usage: vos2mid filename");
        return 0;
    }

    FILE* fp = fopen(argv[1], "rb");
    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);  //same as rewind(f);

    uint8_t *string = malloc(fsize);
    fread(string, 1, fsize, fp);
    fclose(fp);

    vos_t* vos = vos_parser(string, fsize);

    smf_t* smf = vos2smf(vos);

//    for(int i = 0; i < smf->ttempo.nevents; i++) {
//        printf("%d %d %d %d %d\n", smf->ttempo.events[i].tick, smf->ttempo.events[i].byte[0], smf->ttempo.events[i].byte[1], smf->ttempo.events[i].byte[2], smf->ttempo.events[i].byte[3]);
//    }

//    int num = 2;
//    for(int i = 0; i < smf->tracks[num].nevents; i++) {
//        printf("%d %d %d %d %d\n", smf->tracks[num].events[i].tick, smf->tracks[num].events[i].byte[0], smf->tracks[num].events[i].byte[1], smf->tracks[num].events[i].byte[2], smf->tracks[num].events[i].byte[3]);
//    }

    FILE* op = fopen("output.mid", "wb");

    dump_smf(op, smf);

    return 0;
}
