#include "parameterPack.hpp"

paramWrapper::paramWrapper(univStr tag, int intValue){
	this->tag = tag;
	this->intValue = intValue;
}

paramWrapper::paramWrapper(univStr tag, float floatValue){
	this->tag = tag;
	this->floatValue = floatValue;
}

paramWrapper::paramWrapper(univStr tag, univStr stringValue){
	this->tag = tag;
	this->stringValue = stringValue;
}

paramWrapper::paramWrapper(univStr tag, vec3 vec3Value){
	this->tag = tag;
	this->vec3Value = vec3Value;
}

paramWrapper::paramWrapper(univStr tag, quat quatValue){
	this->tag = tag;
	this->quatValue = quatValue;
}

//-------------------------------------------------------------------


int paramWrapper::getInt(){
	return intValue;
}


float paramWrapper::getFloat(){
	return floatValue;
}
univStr paramWrapper::getString(){
	return stringValue;
}
vec3 paramWrapper::getVec3(){
	return vec3Value;
}
quat paramWrapper::getQuat(){
	return quatValue;
}


//-------------------------------------------------------------------


extern "C"
paramWrapper* createIntParam(univStr *tag, int value){
	return new paramWrapper(*tag, value);
}

extern "C"
paramWrapper* createFloatParam(univStr *tag, float value){
	return new paramWrapper(*tag, value);
}

extern "C"
paramWrapper* createStringParam(univStr *tag, univStr *value){
	return new paramWrapper(*tag, *value);
}

extern "C"
paramWrapper* createVec3Param(univStr *tag, vec3 *value){
	return new paramWrapper(*tag, *value);
}

extern "C"
paramWrapper* createQuatParam(univStr *tag, quat *value){
	return new paramWrapper(*tag, *value);
}


//-------------------------------------------------------------------


parameterPack::parameterPack(int count, va_list arguments){
	paramList = new paramWrapper*[count];
	length = count;

	for(int i = 0; i < count; i++){
		paramList[i] = va_arg(arguments, paramWrapper*);
	}
	va_end(arguments);
}

parameterPack::~parameterPack(){
	for(int i = 0; i < length; i++){
		delete paramList[i];
	}
	delete[] paramList;
}

paramWrapper* parameterPack::search(std::string input){
	univStr in = makeUnivStr(input);
	for(int i = 0; i < length; i++){
		if(paramList[i]->tag == in){
			return paramList[i];
		}
	}
	return nullptr;
}


//-------------------------------------------------------------------


extern "C"
parameterPack* createParameterPack(int count, ...){
	va_list arguments;
	va_start(arguments, count);
	return new parameterPack(count, arguments);
}


//-------------------------------------------------------------------


paramWrapper* param(std::string tag, int value){
	return new paramWrapper(makeUnivStr(tag), value);
}

paramWrapper* param(std::string tag, float value){
	return new paramWrapper(makeUnivStr(tag), value);
}

paramWrapper* param(std::string tag, std::string value){
	return new paramWrapper(makeUnivStr(tag), makeUnivStr(value));
}

paramWrapper* param(std::string tag, vec3 value){
	return new paramWrapper(makeUnivStr(tag), value);
}

paramWrapper* param(std::string tag, quat value){
	return new paramWrapper(makeUnivStr(tag), value);
}


//-------------------------------------------------------------------






















