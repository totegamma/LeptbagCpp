import core.stdc.stdio;
import std.stdio;
import core.runtime;
import std.random;
import std.math;
import std.algorithm;

//Library-----------------------------------

extern (C++) {
	interface vec3{
	}
	interface quat{
	}
}

extern (C) {
	vec3 createVec3(float x, float y, float z);
	quat createQuat(float w, float x, float y, float z);
}

extern (C++) {
	interface cubeshapeObject{
		void destroy();
		float getXpos();
		float getYpos();
		float getZpos();
	}


}

extern (C) {
	cubeshapeObject cubeshape_create(vec3 position, vec3 size, quat rotation, float weight);
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
	hingeConstraint hingeConstraint_create(cubeshapeObject cubeA, cubeshapeObject cubeB, vec3 positionA, vec3 positionB, vec3 axis);
}

//------------------------------------------





//ApplicationInterface----------------------

extern (C) void init(){
	rt_init();

	cubeshape_create(createVec3( 50.25, 0.25,      0), createVec3(0.25, 0.25,   50), createQuat(1, 0, 0, 0),  100);
	cubeshape_create(createVec3(-50.25, 0.25,      0), createVec3(0.25, 0.25,   50), createQuat(1, 0, 0, 0),  100);
	cubeshape_create(createVec3(     0, 0.25,  50.25), createVec3(  50, 0.25, 0.25), createQuat(1, 0, 0, 0),  100);
	cubeshape_create(createVec3(     0, 0.25, -50.25), createVec3(  50, 0.25, 0.25), createQuat(1, 0, 0, 0),  100);

	cubeshape_create(createVec3( 50.25, 0.25,  50.25), createVec3(0.25, 0.25, 0.25), createQuat(1, 0, 0, 0),  100);
	cubeshape_create(createVec3( 50.25, 0.25, -50.25), createVec3(0.25, 0.25, 0.25), createQuat(1, 0, 0, 0),  100);
	cubeshape_create(createVec3(-50.25, 0.25,  50.25), createVec3(0.25, 0.25, 0.25), createQuat(1, 0, 0, 0),  100);
	cubeshape_create(createVec3(-50.25, 0.25, -50.25), createVec3(0.25, 0.25, 0.25), createQuat(1, 0, 0, 0),  100);

}


extern (C) void tick(){
}



//------------------------------------------
