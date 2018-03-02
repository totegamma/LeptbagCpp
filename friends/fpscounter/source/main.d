import core.stdc.stdio;
import std.stdio;
import core.runtime;


import leptbagSDK.leptbaglib;
import dlib.math.vector;
import dlib.math.quaternion;



//ApplicationInterface----------------------

extern (C) void init(){
	rt_init();

	textbox newtextbox = new textbox("message", 0, 0, 30, 0, 0, 0);

}


extern (C) void tick(){

}

//------------------------------------------
