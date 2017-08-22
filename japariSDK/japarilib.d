module japariSDK.japarilib;

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
	interface vec3{
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
	interface hingeConstraint_interface{
		void enableMotor(bool flag);
		void setLimit(float lower, float upper);
		void setMaxMotorImpulse(float power);
		void setMotorTarget(float angle, float duration);
		void destroy();
	}

	interface generic6DofConstraint_interface{
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
	hingeConstraint_interface createHingeConstraint(elementNode cubeA, elementNode cubeB, vec3 positionA, vec3 positionB, vec3 axisA, vec3 axisB);
	generic6DofConstraint_interface createGeneric6DofConstraint(elementNode elemA, elementNode elemB, vec3 positionA, vec3 positionB, quat rotation);
}

vec3 toVec3(Vector3f input){
	return createVec3(input.x, input.y, input.z);
}

class hingeConstraint{

	hingeConstraint_interface realHingeConstraint;

	this(elementNode cubeA, elementNode cubeB, Vector3f positionA, Vector3f positionB, Vector3f axis){
		realHingeConstraint = createHingeConstraint(cubeA, cubeB, toVec3(positionA), toVec3(positionB), toVec3(axis), toVec3(axis));
	}

	this(elementNode cubeA, elementNode cubeB, Vector3f positionA, Vector3f positionB, Vector3f axisA, Vector3f axisB){
		realHingeConstraint = createHingeConstraint(cubeA, cubeB, toVec3(positionA), toVec3(positionB), toVec3(axisA), toVec3(axisB));
	}
	void enableMotor(bool flag){
		realHingeConstraint.enableMotor(flag);
	}
	void setLimit(float lower, float upper){
		realHingeConstraint.setLimit(lower, upper);
	}
	void setMaxMotorImpulse(float power){
		realHingeConstraint.setMaxMotorImpulse(power);
	}
	void setMotorTarget(float angle, float duration){
		realHingeConstraint.setMotorTarget(angle, duration);
	}
	void destroy(){
		realHingeConstraint.destroy();
	}
}

