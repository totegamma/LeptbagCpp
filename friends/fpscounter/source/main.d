import core.stdc.stdio;
import std.stdio;
import core.runtime;
import std.conv;

import std.datetime.stopwatch;


import leptbagSDK.leptbaglib;
import dlib.math.vector;
import dlib.math.quaternion;


StopWatch sw;
textbox fpslabel;


//ApplicationInterface----------------------

extern (C) void init(){
	rt_init();

	sw = StopWatch(AutoStart.no);
	sw.start();

	fpslabel = new textbox("fps", 3, 10, 30, 0, 0, 0);

}


extern (C) void tick(){
	sw.stop();

	double usecs = sw.peek.total!"usecs";
	double fps = 1000000.0f/usecs;
	wstring message = to!wstring(to!int(fps)) ~ "fps";
	fpslabel.updateText(message);

	sw.reset();
	sw.start();
}

//------------------------------------------
