import core.stdc.stdio;
import std.stdio;
import core.runtime;
import std.random;
import std.math;
import std.algorithm;

extern (C++){
	interface stringHolder{
	}
	interface argWrapper{
	}
	interface parameterPack{
	}
}

extern (C){
	argWrapper createIntArg(stringHolder tag, int value);
	argWrapper createFloatArg(stringHolder tag, float value);
	argWrapper createStringArg(stringHolder tag, stringHolder value);
}

extern (C) void hoge(parameterPack i);
extern (C) stringHolder makeStringHolder(char* str, int length);
extern (C) parameterPack createParameterPack(int count, ...);


argWrapper arg(string tag, int value){
	return createIntArg(mksh(tag), value);
}

argWrapper arg(string tag, float value){
	return createFloatArg(mksh(tag), value);
}

argWrapper arg(string tag, string value){
	return createStringArg(mksh(tag), mksh(value));
}


stringHolder mksh(string input){
	char* cstr = &input.dup[0];
	return makeStringHolder(cstr, cast(int)input.length);
}

parameterPack argWrap(ARGS...)(ARGS args){
	return createParameterPack(args.length, args);
}

extern (C) int dll() {
	rt_init();
	printf("dll()\n");

	hoge(argWrap(arg("nekochan", 101), arg("hogepiyo", 101.101), arg("nyannyan", "nya~~n")));
	return 0;
}

shared static this() {
	printf("libdll.so shared static this\n");
}

shared static ~this() {
	printf("libdll.so shared static ~this\n");
}
