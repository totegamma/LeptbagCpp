module japarilib;


extern (C++) {
	interface elementManager{
		elementNode generate(parameterPack input);
	}
	interface elementNode{
	}

	elementManager getCubeshape();
	elementManager getPlaneshape();
}

extern (C++){
	interface vec3{
	}
	interface quat{
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
}

extern (C){
	paramWrapper createIntParam(univStr tag, int value);
	paramWrapper createFloatParam(univStr tag, float value);
	paramWrapper createStringParam(univStr tag, univStr value);
	paramWrapper createVec3Param(univStr tag, vec3 value);
	paramWrapper createQuatParam(univStr tag, quat value);
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



univStr mksh(string input){
	char* cstr = &input.dup[0];
	return createUnivStr(cstr, cast(int)input.length);
}

parameterPack paramWrap(ARGS...)(ARGS args){
	return createParameterPack(args.length, args);
}
