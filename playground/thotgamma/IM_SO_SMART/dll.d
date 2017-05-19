import core.stdc.stdio;


extern (C) void hoge();

extern (C) int dll() {
    printf("dll()\n");
	hoge();
    return 0;
}

shared static this() {
    printf("libdll.so shared static this\n");
}

shared static ~this() {
    printf("libdll.so shared static ~this\n");
}
