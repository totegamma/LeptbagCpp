module japariSDK.japarilib;

import dlib.math.vector;
import dlib.math.quaternion;

elementManager getCubeShape(){
	static elementManager instance;
	if(!instance) instance = new elementManager(getCubeshape());
	return instance;
}

elementManager getPlaneShape(){
	static elementManager instance;
	if(!instance) instance = new elementManager(getPlaneshape());
	return instance;
}

extern (C++) {
	interface elementManager_interface{
		elementNode_interface generate(parameterPack input);
	}
	interface elementNode_interface{
		float getXpos();
		float getYpos();
		float getZpos();
		float getBasis(int row, int column);
		float getFriction();
		void setFriction(float coef);
		void destroy();
	}
	interface btRigidBody{
	}

	elementManager_interface getCubeshape();
	elementManager_interface getPlaneshape();
}

class elementManager{

	elementManager_interface realElementManager;

	this(elementManager_interface input){
		realElementManager = input;
	}

	extern(C)
	this(vertexManager vm, btRigidBody function(parameterPack) func){
		realElementManager = createElementManager(vm, func);
	}

	elementNode generate(parameterPack input){
		return new elementNode(realElementManager.generate(input));
	}
}


class elementNode{

	elementNode_interface realElementNode;

	this(elementNode_interface input){
		realElementNode = input;
	}

	Vector3f getPos(){
		return Vector3f(realElementNode.getXpos(), realElementNode.getYpos(), realElementNode.getZpos());
		
	}
	float getBasis(int row, int column){
		return realElementNode.getBasis(row, column);
	}
	float getFriction(){
		return realElementNode.getFriction();
	}
	void setFriction(float coef){
		realElementNode.setFriction(coef);
	}
	void destroy(){
		realElementNode.destroy();
	}

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
	elementManager_interface createElementManager(vertexManager vm, btRigidBody function(parameterPack));
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
	hingeConstraint_interface createHingeConstraint(elementNode_interface cubeA, elementNode_interface cubeB, vec3 positionA, vec3 positionB, vec3 axisA, vec3 axisB);
	generic6DofConstraint_interface createGeneric6DofConstraint(elementNode_interface elemA, elementNode_interface elemB, vec3 positionA, vec3 positionB, quat rotation);
}

vec3 toVec3(Vector3f input){
	return createVec3(input.x, input.y, input.z);
}

class hingeConstraint{

	hingeConstraint_interface realHingeConstraint;

	this(elementNode cubeA, elementNode cubeB, Vector3f positionA, Vector3f positionB, Vector3f axis){
		realHingeConstraint = createHingeConstraint(cubeA.realElementNode, cubeB.realElementNode, toVec3(positionA), toVec3(positionB), toVec3(axis), toVec3(axis));
	}

	this(elementNode cubeA, elementNode cubeB, Vector3f positionA, Vector3f positionB, Vector3f axisA, Vector3f axisB){
		realHingeConstraint = createHingeConstraint(cubeA.realElementNode, cubeB.realElementNode, toVec3(positionA), toVec3(positionB), toVec3(axisA), toVec3(axisB));
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

