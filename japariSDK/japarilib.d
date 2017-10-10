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

struct vec3{
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

	~this(){
		if(exported == false){
			entity.destroy();
		}
	}
}

vec3_interface toVec3(Vector3f input){
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

struct quat{
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

	~this(){
		if(exported == false){
			entity.destroy();
		}
	}
}

quat_interface toQuat(Quaternionf input){
	return createQuat(input.w, input.x, input.y, input.z);
}


//========[vertex]========
/*
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
		entity = createVertex(coordinate_x, coordinate_y, coordinate_z, normal_x, normal_y, normal_z, color_r, color_g, color_b);
		exported = false;
	}


	~this(){
		if(exported == false){
			entity.destroy();
		}
	}

}
*/


//========[vertexManager]========
extern (C++){
	interface vertexManager_interface{
		void addVertex(float coordinate_x, float coordinate_y, float coordinate_z, float normal_x, float normal_y, float normal_z, float color_r, float color_g, float color_b);
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

	void addVertex(float coordinate_x, float coordinate_y, float coordinate_z, float normal_x, float normal_y, float normal_z, float color_r, float color_g, float color_b){
		entity.addVertex(coordinate_x, coordinate_y, coordinate_z, normal_x, normal_y, normal_z, color_r, color_g, color_b);
	}
}


//========[univStr]========
extern (C++){
	interface univStr_interface{
		void destroy();
	}
}
extern (C) univStr_interface createUnivStr(char* str, int length);

struct univStr{
	univStr_interface entity;
	bool exported;

	this(char* str, int length){
		entity = createUnivStr(str, length);
		exported = false;
	}

	this(string input){
		char* cstr = &input.dup[0];
		entity = createUnivStr(cstr, cast(int)input.length);
		exported = false;
	}

	~this(){
		if(exported == false){
			entity.destroy();
		}
	}
}

univStr_interface str2unis(string input){
	char* cstr = &input.dup[0];
	return createUnivStr(cstr, cast(int)input.length);
}



//========[paramWrapper]========
extern (C++){
	interface paramWrapper{
	}
}

extern (C){
	paramWrapper createIntParam(univStr_interface tag, int value);
	paramWrapper createFloatParam(univStr_interface tag, float value);
	paramWrapper createStringParam(univStr_interface tag, univStr_interface value);
	paramWrapper createVec3Param(univStr_interface tag, vec3_interface value);
	paramWrapper createQuatParam(univStr_interface tag, quat_interface value);
	paramWrapper createModelParam(univStr_interface tag, vertexManager_interface value);
}



paramWrapper param(string tag, int value){
	return createIntParam(str2unis(tag), value);
}

paramWrapper param(string tag, float value){
	return createFloatParam(str2unis(tag), value);
}

paramWrapper param(string tag, string value){
	return createStringParam(str2unis(tag), str2unis(value));
}

paramWrapper param(string tag, Vector3f value){
	return createVec3Param(str2unis(tag), toVec3(value));
}

paramWrapper param(string tag, Quaternionf value){
	return createQuatParam(str2unis(tag), toQuat(value));
}

paramWrapper param(string tag, vertexManager value){
	return createModelParam(str2unis(tag), value.entity);
}



//========[parameterPack]========
extern (C++){
	interface parameterPack_interface{
		void destroy();
	}
}


extern (C) parameterPack_interface createParameterPack(int count, ...);

struct parameterPack{
	parameterPack_interface entity;
	bool exported;

	this(T...)(T args){
		entity = createParameterPack(args.length, args);
		exported = false;
	}

	~this(){
		if(exported == false){
			entity.destroy();
		}
	}
}


//========[elementManager]========
extern (C++){
	interface elementManager_interface{
		elementNode_interface generate(parameterPack_interface input);
		void destroy();
	}
}
extern (C) elementManager_interface createElementManager(vertexManager_interface vm, btRigidBody function(parameterPack));

class elementManager{

	elementManager_interface entity;

	this(elementManager_interface input){
		entity = input;
	}

	extern(C)
	this(vertexManager vm, btRigidBody function(parameterPack) func){
		entity = createElementManager(vm.entity, func);
	}

	elementNode generate(parameterPack input){
		input.exported = true;
		return new elementNode(entity.generate(input.entity));
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
		return Vector3f(entity.getXpos(), entity.getYpos(), entity.getZpos());
	}
	float getBasis(int row, int column){
		return entity.getBasis(row, column);
	}
	float getFriction(){
		return entity.getFriction();
	}
	void setFriction(float coef){
		entity.setFriction(coef);
	}
	void destroy(){
		entity.destroy();
		exported = true;
		entity = null;
	}

	~this(){
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

extern (C) hingeConstraint_interface createHingeConstraint(elementNode_interface cubeA, elementNode_interface cubeB, vec3_interface positionA, vec3_interface positionB, vec3_interface axisA, vec3_interface axisB);

class hingeConstraint{

	hingeConstraint_interface entity;
	bool exported;

	this(elementNode cubeA, elementNode cubeB, Vector3f positionA, Vector3f positionB, Vector3f axis){
		entity = createHingeConstraint(cubeA.entity, cubeB.entity, toVec3(positionA), toVec3(positionB), toVec3(axis), toVec3(axis));
		exported = false;
	}

	this(elementNode cubeA, elementNode cubeB, Vector3f positionA, Vector3f positionB, Vector3f axisA, Vector3f axisB){
		entity = createHingeConstraint(cubeA.entity, cubeB.entity, toVec3(positionA), toVec3(positionB), toVec3(axisA), toVec3(axisB));
		exported = false;
	}
	

	void enableMotor(bool flag){
		entity.enableMotor(flag);
	}
	void setLimit(float lower, float upper){
		entity.setLimit(lower, upper);
	}
	void setMaxMotorImpulse(float power){
		entity.setMaxMotorImpulse(power);
	}
	void setMotorTarget(float angle, float duration){
		entity.setMotorTarget(angle, duration);
	}
	void destroy(){
		entity.destroy();
		exported = true;
		entity = null;
	}

	~this(){
		if(exported == false){
			entity.destroy();
		}
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

