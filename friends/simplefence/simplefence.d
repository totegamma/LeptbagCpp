import core.stdc.stdio;
import std.stdio;
import core.runtime;
import std.random;
import std.math;
import std.algorithm;

//Library-----------------------------------

extern (C++) {
	interface cubeshapeObject{
		void destroy();
		float getXpos();
		float getYpos();
		float getZpos();
	}

	cubeshapeObject cubeshape_create(float x, float y, float z, float w, float h, float d, float qw, float qx, float qy, float qz, float g);

}

extern (C++) {
	interface hingeConstraint{
		void enableMotor(bool flag);
		void setLimit(float lower, float upper);
		void setMaxMotorImpulse(float power);
		void setMotorTarget(float angle, float duration);
		void destroy();	
	}
}

extern (C) {
	hingeConstraint hingeConstraint_create(cubeshapeObject cubeA, cubeshapeObject cubeB, float ax, float ay, float az, float bx, float by, float bz, float vx, float vy, float vz);
}

//------------------------------------------





//ApplicationInterface----------------------

extern (C) void init(){
	rt_init();

	cubeshape_create(  50.25, 0.25,      0,    0.5, 0.5, 100,    1, 0, 0, 0,    100);
	cubeshape_create( -50.25, 0.25,      0,    0.5, 0.5, 100,    1, 0, 0, 0,    100);
	cubeshape_create(      0, 0.25,  50.25,    100, 0.5, 0.5,    1, 0, 0, 0,    100);
	cubeshape_create(      0, 0.25, -50.25,    100, 0.5, 0.5,    1, 0, 0, 0,    100);

	cubeshape_create(  50.25, 0.25,  50.25,    0.5, 0.5, 0.5,    1, 0, 0, 0,    100);
	cubeshape_create(  50.25, 0.25, -50.25,    0.5, 0.5, 0.5,    1, 0, 0, 0,    100);
	cubeshape_create( -50.25, 0.25,  50.25,    0.5, 0.5, 0.5,    1, 0, 0, 0,    100);
	cubeshape_create( -50.25, 0.25, -50.25,    0.5, 0.5, 0.5,    1, 0, 0, 0,    100);

}


extern (C) void tick(){
}



//------------------------------------------
