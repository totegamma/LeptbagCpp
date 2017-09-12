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

	getCubeshape().generate(paramWrap(
				param("position", createVec3(50.25, 0.25, 0)),
				param("scale", createVec3(0.25, 0.25, 50)),
				param("rotation", createQuat(1, 0, 0, 0)),
				param("mass", 100)));

	getCubeshape().generate(paramWrap(
				param("position", createVec3(-50.25, 0.25, 0)),
				param("scale", createVec3(0.25, 0.25, 50)),
				param("rotation", createQuat(1, 0, 0, 0)),
				param("mass", 100)));

	getCubeshape().generate(paramWrap(
				param("position", createVec3(0, 0.25, 50.25)),
				param("scale", createVec3(50, 0.25, 0.25)),
				param("rotation", createQuat(1, 0, 0, 0)),
				param("mass", 100)));

	getCubeshape().generate(paramWrap(
				param("position", createVec3(0, 0.25, -50.25)),
				param("scale", createVec3(50, 0.25, 0.25)),
				param("rotation", createQuat(1, 0, 0, 0)),
				param("mass", 100)));

	getCubeshape().generate(paramWrap(
				param("position", createVec3(50.25, 0.25, 50.25)),
				param("scale", createVec3(0.25, 0.25, 0.25)),
				param("rotation", createQuat(1, 0, 0, 0)),
				param("mass", 100)));

	getCubeshape().generate(paramWrap(
				param("position", createVec3(50.25, 0.25, -50.25)),
				param("scale", createVec3(0.25, 0.25, 0.25)),
				param("rotation", createQuat(1, 0, 0, 0)),
				param("mass", 100)));

	getCubeshape().generate(paramWrap(
				param("position", createVec3(-50.25, 0.25, 50.25)),
				param("scale", createVec3(0.25, 0.25, 0.25)),
				param("rotation", createQuat(1, 0, 0, 0)),
				param("mass", 100)));

	getCubeshape().generate(paramWrap(
				param("position", createVec3(-50.25, 0.25, -50.25)),
				param("scale", createVec3(0.25, 0.25, 0.25)),
				param("rotation", createQuat(1, 0, 0, 0)),
				param("mass", 100)));

}


extern (C) void tick(){
}



//------------------------------------------
