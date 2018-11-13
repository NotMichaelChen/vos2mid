module main;

import std.stdio, std.file, std.path;
import vos, smf, dump;

void main(string[] args) {
    if(args.length < 2) {
        printf("Error: no filename provided");
        printf("Usage: vos2mid [filename]");
        return;
    }

    ubyte[] vosfile = cast(ubyte[]) std.file.read(args[1]);

    vos_t* vos = vos_parser(&vosfile[0], vosfile.length);
    smf_t* smf = vos2smf(vos);

    File fp = File("output.mid", "wb");

    mid2smf(fp, smf);
}