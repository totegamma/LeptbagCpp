import core.stdc.stdio;
import std.stdio;
import core.runtime;
import std.random;
import std.math;
import std.algorithm;


import japariSDK.japarilib;

gen6DofInsect insect;

class gen6DofInsect{

	elementNode thorax;
	elementNode legFrontLeft;
	elementNode legFrontRight;
	elementNode legBackLeft;
	elementNode legBackRight;

	generic6DofConstraint gen6Dof_thorax_legFrontLeft;
	generic6DofConstraint gen6Dof_thorax_legFrontRight;
	generic6DofConstraint gen6Dof_thorax_legBackLeft;
	generic6DofConstraint gen6Dof_thorax_legBackRight;


	this(float x, float y, float z) {
		spawn(x, y, z);
	}



	void spawn(float x, float y, float z){

		thorax = getCubeshape().generate(paramWrap(
					param("position", createVec3(x, y, z)),
					param("scale", createVec3(1.0, 0.1, 0.5)),
					param("rotation", createQuat(1, 0, 0, 0)),
					param("mass", 1.0f)));

		legFrontLeft = getCubeshape().generate(paramWrap(
					param("position", createVec3(x+0.8, y-0.4, z-0.6)),
					param("scale", createVec3(0.2, 0.5, 0.1)),
					param("rotation", createQuat(1, 0, 0, 0)),
					param("mass", 0.2f)));

		legFrontRight = getCubeshape().generate(paramWrap(
					param("position", createVec3(x+0.8, y-0.4, z+0.6)),
					param("scale", createVec3(0.2, 0.5, 0.1)),
					param("rotation", createQuat(1, 0, 0, 0)),
					param("mass", 0.2f)));

		legBackLeft = getCubeshape().generate(paramWrap(
					param("position", createVec3(x-0.8, y-0.4, z-0.6)),
					param("scale", createVec3(0.2, 0.5, 0.1)),
					param("rotation", createQuat(1, 0, 0, 0)),
					param("mass", 0.2f)));

		legBackRight = getCubeshape().generate(paramWrap(
					param("position", createVec3(x-0.8, y-0.4, z+0.6)),
					param("scale", createVec3(0.2, 0.5, 0.1)),
					param("rotation", createQuat(1, 0, 0, 0)),
					param("mass", 0.2f)));



		gen6Dof_thorax_legFrontLeft   = generic6DofConstraint_create(thorax , legFrontLeft , createVec3( 0.8, 0, -0.7), createVec3(   0, 0.4, -0.1));
		gen6Dof_thorax_legFrontRight  = generic6DofConstraint_create(thorax , legFrontRight, createVec3( 0.8, 0,  0.7), createVec3(   0, 0.4,  0.1));
		gen6Dof_thorax_legBackLeft	  = generic6DofConstraint_create(thorax , legBackLeft  , createVec3(-0.8, 0, -0.7), createVec3(   0, 0.4, -0.1));
		gen6Dof_thorax_legBackRight	  = generic6DofConstraint_create(thorax , legBackRight , createVec3(-0.8, 0,  0.7), createVec3(   0, 0.4,  0.1));

		// リストにぶち込んでやるぜ的なやーつでこの辺綺麗にしたい。
		// というかデフォルトは全部0なのでは…？
		gen6Dof_thorax_legFrontLeft.setAngularLowerLimit( createVec3(0,  0,  -PI));
		gen6Dof_thorax_legFrontLeft.setAngularUpperLimit( createVec3(0,  0,   PI));
		gen6Dof_thorax_legFrontLeft.setLinearLowerLimit(  createVec3(0,  0,    0));
		gen6Dof_thorax_legFrontLeft.setLinearUpperLimit(  createVec3(0,  0,    0));

		gen6Dof_thorax_legFrontRight.setAngularLowerLimit(createVec3(0,  0,  -PI));
		gen6Dof_thorax_legFrontRight.setAngularUpperLimit(createVec3(0,  0,   PI));
		gen6Dof_thorax_legFrontRight.setLinearLowerLimit( createVec3(0,  0,    0));
		gen6Dof_thorax_legFrontRight.setLinearUpperLimit( createVec3(0,  0,    0));

		gen6Dof_thorax_legBackLeft.setAngularLowerLimit(  createVec3(0,  0,  -PI));
		gen6Dof_thorax_legBackLeft.setAngularUpperLimit(  createVec3(0,  0,   PI));
		gen6Dof_thorax_legBackLeft.setLinearLowerLimit(   createVec3(0,  0,    0));
		gen6Dof_thorax_legBackLeft.setLinearUpperLimit(   createVec3(0,  0,    0));

		gen6Dof_thorax_legBackRight.setAngularLowerLimit( createVec3(0,  0,  -PI));
		gen6Dof_thorax_legBackRight.setAngularUpperLimit( createVec3(0,  0,   PI));
		gen6Dof_thorax_legBackRight.setLinearLowerLimit(  createVec3(0,  0,    0));
		gen6Dof_thorax_legBackRight.setLinearUpperLimit(  createVec3(0,  0,    0));
	}


	void despawn(){
		thorax.destroy();
		legFrontLeft.destroy();
		legFrontRight.destroy();
		legBackLeft.destroy();
		legBackRight.destroy();
		gen6Dof_thorax_legFrontLeft.destroy();
		gen6Dof_thorax_legFrontRight.destroy();
		gen6Dof_thorax_legBackLeft.destroy();
		gen6Dof_thorax_legBackRight.destroy();

	}
}


//ApplicationInterface----------------------

extern (C) void init(){
	rt_init();
	insect = new gen6DofInsect(0, 1, -1);
}


extern (C) void tick(){

}



//------------------------------------------
