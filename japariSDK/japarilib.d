module japarilib;

import dlib.math.vector;
import dlib.math.quaternion;

extern (C++) {
	interface elementManager{
		elementNode generate(parameterPack input);
	}
	interface elementNode{
		float getXpos();
		float getYpos();
		float getZpos();
		float getBasis(int row, int column);
		float getFriction();
		float setFriction(float coef);
		void destroy();
	}
	interface btRigidBody{
	}

	elementManager getCubeshape();
	elementManager getPlaneshape();
}

extern (C){
	btRigidBody createBoxBody(parameterPack input);
	btRigidBody createPlaneBody(parameterPack input);
	btRigidBody createConvexHullShapeBody(parameterPack input);
}

extern (C++){
	interface vec3_interface{
		float getx();
		float gety();
		float getz();
	}
	interface quat{
		float getw();
		float getx();
		float gety();
		float getz();
	}
	interface vertex{
	}
	interface vertexManager{
		void addVertex(vertex input);
	}
	interface univStr{
	}
	interface paramWrapper{
	}
	interface parameterPack{
	}
}

class vec3 : vec3_interface{

	this(){
	}
	extern (C++){
		float getx();
		float gety();
		float getz();
	}

}

extern (C) {
	vec3 createVec3(float x, float y, float z);
	quat createQuat(float w, float x, float y, float z);
	parameterPack createParameterPack(int count, ...);
	univStr createUnivStr(char* str, int length);
	vertex createVertex(float coordinate_x, float coordinate_y, float coordinate_z, float normal_x, float normal_y, float normal_z, float color_r, float color_g, float color_b);
	vertexManager createVertexManager();
	elementManager createElementManager(vertexManager vm, btRigidBody function(parameterPack));
}

extern (C){
	paramWrapper createIntParam(univStr tag, int value);
	paramWrapper createFloatParam(univStr tag, float value);
	paramWrapper createStringParam(univStr tag, univStr value);
	paramWrapper createVec3Param(univStr tag, vec3 value);
	paramWrapper createQuatParam(univStr tag, quat value);
	paramWrapper createModelParam(univStr tag, vertexManager value);
}

paramWrapper param(string tag, int value){
	return createIntParam(mksh(tag), value);
}

paramWrapper param(string tag, float value){
	return createFloatParam(mksh(tag), value);
}

paramWrapper param(string tag, string value){
	return createStringParam(mksh(tag), mksh(value));
}

paramWrapper param(string tag, Vector3f value){
	return createVec3Param(mksh(tag), createVec3(value.x, value.y, value.z));
}

paramWrapper param(string tag, Quaternionf value){
	return createQuatParam(mksh(tag), createQuat(value.w, value.x, value.y, value.z));
}

paramWrapper param(string tag, vertexManager value){
	return createModelParam(mksh(tag), value);
}



univStr mksh(string input){
	char* cstr = &input.dup[0];
	return createUnivStr(cstr, cast(int)input.length);
}

parameterPack paramWrap(ARGS...)(ARGS args){
	return createParameterPack(args.length, args);
}


extern (C++) {
	interface hingeConstraint{
		void enableMotor(bool flag);
		void setLimit(float lower, float upper);
		void setMaxMotorImpulse(float power);
		void setMotorTarget(float angle, float duration);
		void destroy();
	}

	interface generic6DofConstraint{
		float getAngle(int index);
		void setAngularLimit(vec3 lower, vec3 upper);
		void setLinearLimit(vec3 lower, vec3 upper);
		void setRotationalMotor(int index);
		void setLinearMotor(int index);
		void setMaxRotationalMotorForce(int index, float force);
		void setMaxLinearMotorForce(vec3 force);
		void setRotationalTargetVelocity(vec3 velocity);
		void setLinearTargetVelocity(vec3 velocity);
		void destroy();
	}
}

extern (C) {
	hingeConstraint hingeConstraint_create(elementNode cubeA, elementNode cubeB, vec3 positionA, vec3 positionB, vec3 axisA, vec3 axisB);
	generic6DofConstraint generic6DofConstraint_create(elementNode elemA, elementNode elemB, vec3 positionA, vec3 positionB, quat rotation);
}
