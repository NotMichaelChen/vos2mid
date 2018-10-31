module main;

import std.stdio, std.file;
import vos, smf;

void main(string[] args) {
    writeln("Hello World!");

    ubyte[] vosfile = cast(ubyte[]) std.file.read("./bin/6653.vos");

    vos_t* vos = vos_parser(&vosfile[0], vosfile.length);

    smf_t* smf = vos2smf(vos);

    writeln(smf.status);
    writeln(smf.format);
    writeln(smf.division);
    writeln(smf.ttempo.nevents);
    writeln(smf.ntracks);
}