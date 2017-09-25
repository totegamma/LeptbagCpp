import core.stdc.stdio;
import std.stdio;
import core.runtime;
import std.random;
import std.math;
import std.algorithm;

import japariSDK.japarilib;



//ApplicationInterface----------------------

extern (C) void init(){
	rt_init();

	getPlaneshape().generate(paramWrap(
										param("position",createVec3(0, 0, 0)),
										param("scale",    createVec3(1, 1, 1)),
										param("face",    createVec3(0, 1, 0)),
										param("rotation",createQuat(1, 0, 0, 0)),
										param("mass", 0)));




}


extern (C) void tick(){
}



//------------------------------------------
