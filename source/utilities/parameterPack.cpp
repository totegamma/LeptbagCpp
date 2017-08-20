#include "parameterPack.hpp"
#include <iostream>

paramWrapper::paramWrapper(std::unique_ptr<univStr> tag, int intValue){
	this->tag = std::move(tag);
	this->data.intValue = intValue;

	contain = INT;
}

paramWrapper::paramWrapper(std::unique_ptr<univStr> tag, float floatValue){
	this->tag = std::move(tag);
	this->data.floatValue = floatValue;

	contain = FLOAT;
}

paramWrapper::paramWrapper(std::unique_ptr<univStr> tag, univStr* stringValue){
	this->tag = std::move(tag);
	this->data.stringValue = stringValue;

	contain = STRING;
}

paramWrapper::paramWrapper(std::unique_ptr<univStr> tag, vec3* vec3Value){
	this->tag = std::move(tag);
	this->data.vec3Value = vec3Value;

	contain = VEC3;
}

paramWrapper::paramWrapper(std::unique_ptr<univStr> tag, quat* quatValue){
	this->tag = std::move(tag);
	this->data.quatValue = quatValue;

	contain = QUAT;
}

paramWrapper::paramWrapper(std::unique_ptr<univStr> tag, vertexManager* modelValue){
	this->tag = std::move(tag);
	this->data.modelValue = modelValue;

	contain = MODEL;
}

//-------------------------------------------------------------------


int paramWrapper::getInt(){
	assert(contain == INT);
	return data.intValue;
}

float paramWrapper::getFloat(){
	assert(contain == FLOAT);
	return data.floatValue;
}

univStr* paramWrapper::getString(){
	assert(contain == STRING);
	return data.stringValue;
}

vec3* paramWrapper::getVec3(){
	assert(contain == VEC3);
	return data.vec3Value;
}

quat* paramWrapper::getQuat(){
	assert(contain == QUAT);
	return data.quatValue;
}

vertexManager* paramWrapper::getModel(){
	assert(contain == MODEL);
	return data.modelValue;
}


//-------------------------------------------------------------------


extern "C"
paramWrapper* createIntParam(univStr *tag, int value){
	return new paramWrapper(std::unique_ptr<univStr>(tag), value);//XXX 未確認
}

extern "C"
paramWrapper* createFloatParam(univStr *tag, float value){
	return new paramWrapper(std::unique_ptr<univStr>(tag), value);//XXX 未確認
}

extern "C"
paramWrapper* createStringParam(univStr *tag, univStr *value){
	return new paramWrapper(std::unique_ptr<univStr>(tag), value);//XXX 未確認
}

extern "C"
paramWrapper* createVec3Param(univStr *tag, vec3 *value){
	return new paramWrapper(std::unique_ptr<univStr>(tag), value);//XXX 未確認
}

extern "C"
paramWrapper* createQuatParam(univStr *tag, quat *value){
	return new paramWrapper(std::unique_ptr<univStr>(tag), value);//XXX 未確認
}

extern "C"
paramWrapper* createModelParam(univStr *tag, vertexManager *value){
	return new paramWrapper(std::unique_ptr<univStr>(tag), value);//XXX 未確認
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
	for(int i = 0; i < length; i++){
		if(paramList[i]->tag->getString() == input){
			return paramList[i];
		}
	}
	std::cout << "404: " << input << std::endl;
	return nullptr;
}


//-------------------------------------------------------------------


extern "C"
parameterPack* createParameterPack(int count, ...){
	va_list arguments;
	va_start(arguments, count);
	return new parameterPack(count, arguments);//XXX 未確認
}


//-------------------------------------------------------------------


paramWrapper* param(std::string tag, int value){
	return new paramWrapper(std::make_unique<univStr>(tag), value);//XXX 未確認
}

paramWrapper* param(std::string tag, float value){
	return new paramWrapper(std::make_unique<univStr>(tag), value);//XXX 未確認
}

paramWrapper* param(std::string tag, std::string value){
	return new paramWrapper(std::make_unique<univStr>(tag), new univStr(value));//XXX 未確認
}

paramWrapper* param(std::string tag, vec3* value){
	return new paramWrapper(std::make_unique<univStr>(tag), value);//XXX 未確認
}

paramWrapper* param(std::string tag, quat* value){
	return new paramWrapper(std::make_unique<univStr>(tag), value);//XXX 未確認
}

paramWrapper* param(std::string tag, vertexManager* value){
	return new paramWrapper(std::make_unique<univStr>(tag), value);//XXX 未確認
}

//-------------------------------------------------------------------

