import core.stdc.stdio;
import std.stdio;
import core.runtime;
import std.random;
import std.math;
import std.algorithm;


import japariSDK.japarilib;
import dlib.math.vector;
import dlib.math.quaternion;



//ApplicationInterface----------------------

extern (C) void init(){
	rt_init();

	getPlaneShape().generate(paramWrap(
									param("position", Vector3f(0, 0, 0)),
									param("scale",    Vector3f(1, 1, 1)),
									param("face",     Vector3f(0, 1, 0)),
									param("rotation", Quaternionf(0, 0, 0, 1)),
									param("mass",     0.0f)));


}


extern (C) void tick(){

}

//------------------------------------------
