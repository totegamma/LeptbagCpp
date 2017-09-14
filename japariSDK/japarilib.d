module japariSDK.japarilib;
import std.stdio;

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

// -------- 目次 --------
//vec3
//quat
//vertex
//vertexManager
//univStr
//paramWrapper
//parameterPack
//elementManager
//elementNode
//hingeConstraint
//generic6DofConstraint
// ----------------------


//========[vec3]========
extern (C++){
	interface vec3_interface{
		float getx();
		float gety();
		float getz();
		void destroy();
	}
}

extern (C) vec3_interface createVec3(float x, float y, float z);

class vec3{
	vec3_interface entity;
	bool exported;

	this(float x, float y, float z){
		entity = createVec3(x, y, z);
		exported = false;
	}

	float getx(){
		return entity.getx();
	}

	float gety(){
		return entity.gety();
	}

	float getz(){
		return entity.getz();
	}
}

vec3 toVec3(Vector3f input){
	return createVec3(input.x, input.y, input.z);
}


//========[quat]========
extern (C++){
	interface quat_interface{
		float getw();
		float getx();
		float gety();
		float getz();
		void destroy();
	}
}

extern (C) quat_interface createQuat(float w, float x, float y, float z);

class quat{
	quat_interface entity;
	bool exported;

	this(float w, float x, float y, float z){
		entity = createQuat(w, x, y, z);
		exported = false;
	}

	float getw(){
		return entity.getw();
	}

	float getx(){
		return entity.getx();
	}

	float gety(){
		return entity.gety();
	}

	float getz(){
		return entity.getz();
	}
}


//========[vertex]========
extern (C++){
	interface vertex_interface{
		void destroy();
	}
}
extern (C) vertex_interface createVertex(float coordinate_x, float coordinate_y, float coordinate_z, float normal_x, float normal_y, float normal_z, float color_r, float color_g, float color_b);


class vertex{
	vertex_interface entity;
	bool exported;

	this(float coordinate_x, float coordinate_y, float coordinate_z, float normal_x, float normal_y, float normal_z, float color_r, float color_g, float color_b){
		entity = createVertex(coordinate_x, coordinate_y, coordinate_z, normal_x, normal_y, normal_Z, color_r, color_g, color_b);
		exported = false;
	}


}


//========[vertexManager]========
extern (C++){
	interface vertexManager_interface{
		void addVertex(vertex input);
		void destroy();
	}
}

extern (C) vertexManager_interface createVertexManager();

class vertexManager{
	vertexManager_interface entity;
	bool exported;

	this(){
		entity = createVertexManager();
		exported = false;
	}

	void addVertex(vertex input){
		vertex.exported = true;
		entity.addVertex(input.entity);
	}
}


//========[univStr]========
extern (C++){
	interface univStr_interface{
		void destroy();
	}
}
extern (C) univStr_interface createUnivStr(char* str, int length);

class univStr{
	univStr_interface entity;
	bool exported;

	this(char* str, int length){
		entity = createUnivStr(str, length);
		exported = false;
	}

	this(string input){
		char* cstr = &input.dup[0];
		return createUnivStr(cstr, cast(int)input.length);
	}
}



//========[paramWrapper]========
extern (C++){
	interface paramWrapper_interface{
		void destroy();
	}
}

extern (C){
	paramWrapper createIntParam(univStr tag, int value);
	paramWrapper createFloatParam(univStr tag, float value);
	paramWrapper createStringParam(univStr tag, univStr value);
	paramWrapper createVec3Param(univStr tag, vec3 value);
	paramWrapper createQuatParam(univStr tag, quat value);
	paramWrapper createModelParam(univStr tag, vertexManager value);
}

class paramWrapper{
	paramWrapper_interface entity;
	bool exported;

	this(string tag, int value){
		univStr tag_tmp = univStr(tag);
		entity = createIntParam(tag_tmp, value);
		tag_tmp.exported = true;
		exported = false;
	}

	this(string tag, float value){
		univStr tag_tmp = univStr(tag);
		entity = createFloatParam(tag_tmp, value);
		tag_tmp.exported = true;
		exported = false;
	}

	this(string tag, string value){
		univStr tag_tmp = univStr(tag);
		univStr value_tmp = univStr(value);
		entity = createStringParam(tag_tmp, value_tmp);
		tag_tmp.exported = true;
		value_tmp.exported = true;
		exported = false;
	}

	this(string tag, Vector3f value){
		univStr tag_tmp = univStr(tag);
		entity = createVec3Param(tag_tmp, value);
		tag_tmp.exported = true;
		exported = false;
	}

	this(string tag, Quaternionf value){
		univStr tag_tmp = univStr(tag);
		entity = createQuatParam(tag_tmp, value);
		tag_tmp.exported = true;
		exported = false;
	}

	this(string tag, vertexManager value){
		univStr tag_tmp = univStr(tag);
		entity = createModelParam(tag_tmp, value);
		tag_tmp.exported = true;
		exported = false;
	}
}



//========[parameterPack]========
extern (C++){
	interface parameterPack_interface{
		void destroy();
	}
}

void makeAllExported()(){
}

void makeAllExported(paramWrapper...)(paramWrapper first, paramWrapper args){
	first.exported = false;
	makeAllExported(args);
}

extern (C) parameterPack_interface createParameterPack(int count, ...);

class parameterPack{
	parameterPack_interface entity;
	bool exported;

	this(paramWrapper...)(paramWrapper args){
		makeAllexported(args);
		entity = createParameterPack(args.length, args);
		exported = false;
	}
}


//========[elementManager]========
extern (C++){
	interface elementManager_interface{
		elementNode_interface generate(parameterPack input);
		void destroy();
	}
}
extern (C) elementManager_interface createElementManager(vertexManager vm, btRigidBody function(parameterPack));

class elementManager{

	elementManager_interface entity;
	bool exported = false;

	this(elementManager_interface input){
		realElementManager = input;
	}

	extern(C)
	this(vertexManager vm, btRigidBody function(parameterPack) func){
		realElementManager = createElementManager(vm, func);
	}

	elementNode generate(parameterPack input){
		input.exported = true;
		return new elementNode(realElementManager.generate(input));
	}
}


//========[elementNode]========
extern (C++){
	interface elementNode_interface{
		float getXpos();
		float getYpos();
		float getZpos();
		float getBasis(int row, int column);
		float getFriction();
		void setFriction(float coef);
		void destroy();
	}
}

class elementNode{

	elementNode_interface entity;
	bool exported;

	this(elementNode_interface input){
		entity = input;
		exported = false;
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

//========[hingeConstraint]========

extern(C++){
	interface hingeConstraint_interface{
		void enableMotor(bool flag);
		void setLimit(float lower, float upper);
		void setMaxMotorImpulse(float power);
		void setMotorTarget(float angle, float duration);
		void destroy();
	}
}

extern (C) hingeConstraint_interface createHingeConstraint(elementNode_interface cubeA, elementNode_interface cubeB, vec3 positionA, vec3 positionB, vec3 axisA, vec3 axisB);

class hingeConstraint{

	hingeConstraint_interface realHingeConstraint;

	this(elementNode cubeA, elementNode cubeB, Vector3f positionA, Vector3f positionB, Vector3f axis){
		positionA.exported = true;
		positionB.exported = true;
		axis.exported = true;
		realHingeConstraint = createHingeConstraint(cubeA.realElementNode, cubeB.realElementNode, toVec3(positionA), toVec3(positionB), toVec3(axis), toVec3(axis));
	}

	this(elementNode cubeA, elementNode cubeB, Vector3f positionA, Vector3f positionB, Vector3f axisA, Vector3f axisB){
		positionA.exported = true;
		positionB.exported = true;
		axisA.exported = true;
		axisB.exported = true;
		realHingeConstraint = createHingeConstraint(cubeA.realElementNode, cubeB.realElementNode, toVec3(positionA), toVec3(positionB), toVec3(axisA), toVec3(axisB));
	}
	
	~this(){
		realHingeConstraint.destroy();
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


//========[generic6DofConstraint]========

extern (C++) {
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

extern (C) generic6DofConstraint_interface createGeneric6DofConstraint(elementNode_interface elemA, elementNode_interface elemB, vec3 positionA, vec3 positionB, quat rotation);


//-------------------------------------------------------------------------------------------------------

extern (C++) {
	interface btRigidBody{
	}
	elementManager_interface getCubeshape();
	elementManager_interface getPlaneshape();
}

extern (C){
	btRigidBody createBoxBody(parameterPack input);
	btRigidBody createPlaneBody(parameterPack input);
	btRigidBody createConvexHullShapeBody(parameterPack input);
}

