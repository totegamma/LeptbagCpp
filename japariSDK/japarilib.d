module japarilib;


extern (C++) {
	interface elementManager{
		elementNode generate(parameterPack input);
	}
	interface elementNode{
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

paramWrapper param(string tag, vec3 value){
	return createVec3Param(mksh(tag), value);
}

paramWrapper param(string tag, quat value){
	return createQuatParam(mksh(tag), value);
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

vec3 addVec(vec3 A, vec3 B){
	return createVec3(A.getx() + B.getx(), A.gety() + B.gety(), A.getz() + B.getz());
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
	hingeConstraint hingeConstraint_create(elementNode cubeA, elementNode cubeB, vec3 positionA, vec3 positionB, vec3 axis);
}
