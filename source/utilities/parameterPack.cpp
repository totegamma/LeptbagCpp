#include "parameterPack.hpp"
#include <iostream>
#include <stdarg.h>

parameterPackException::parameterPackException() {
}

parameterPackException::parameterPackException(std::string key) {
	std::cout << "Exception: key not found(" << key << ")" << std::endl;
}

paramWrapper::paramWrapper(std::unique_ptr<std::string> tag, int intValue){
	this->tag = std::move(tag);
	this->data.intValue = intValue;

	contain = INT;
}

paramWrapper::paramWrapper(std::unique_ptr<std::string> tag, float floatValue){
	this->tag = std::move(tag);
	this->data.floatValue = floatValue;

	contain = FLOAT;
}

paramWrapper::paramWrapper(std::unique_ptr<std::string> tag, std::string* stringValue){
	this->tag = std::move(tag);
	this->data.stringValue = stringValue;

	contain = STRING;
}

paramWrapper::paramWrapper(std::unique_ptr<std::string> tag, Eigen::Vector3f* vec3Value){
	this->tag = std::move(tag);
	this->data.vec3Value = vec3Value;

	contain = VEC3;
}

paramWrapper::paramWrapper(std::unique_ptr<std::string> tag, Eigen::Quaternionf* quatValue){
	this->tag = std::move(tag);
	this->data.quatValue = quatValue;

	contain = QUAT;
}

paramWrapper::paramWrapper(std::unique_ptr<std::string> tag, vertexManager* modelValue){
	this->tag = std::move(tag);
	this->data.modelValue = modelValue;

	contain = MODEL;
}

paramWrapper::paramWrapper(std::unique_ptr<std::string> tag, elementManager* emValue){
	this->tag = std::move(tag);
	this->data.emValue = emValue;

	contain = EM;
}

paramWrapper::~paramWrapper(){

	switch(contain){
		case INT:
			//do nothing.
			break;
		case FLOAT:
			//do nothing.
			break;
		case STRING:
			delete data.stringValue;
			break;
		case VEC3:
			delete data.vec3Value;
			break;
		case QUAT:
			delete data.quatValue;
			break;
		case MODEL:
			//do nothing.
			break;
		case EM:
			//do nothing.
			break;
	}

}

void paramWrapper::destroy(){
	delete this;
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

std::string* paramWrapper::getString(){
	assert(contain == STRING);
	return data.stringValue;
}

Eigen::Vector3f* paramWrapper::getVec3(){
	assert(contain == VEC3);
	return data.vec3Value;
}

Eigen::Quaternionf* paramWrapper::getQuat(){
	assert(contain == QUAT);
	return data.quatValue;
}

vertexManager* paramWrapper::getModel(){
	assert(contain == MODEL);
	return data.modelValue;
}

elementManager* paramWrapper::getEm(){
	assert(contain == EM);
	return data.emValue;
}


//-------------------------------------------------------------------


extern "C"
paramWrapper* createIntParam(std::string*tag, int value){
	return new paramWrapper(std::unique_ptr<std::string>(tag), value);
}

extern "C"
paramWrapper* createFloatParam(std::string *tag, float value){
	return new paramWrapper(std::unique_ptr<std::string>(tag), value);
}

extern "C"
paramWrapper* createStringParam(std::string *tag, std::string *value){
	return new paramWrapper(std::unique_ptr<std::string>(tag), value);
}

extern "C"
paramWrapper* createVec3Param(std::string *tag, Eigen::Vector3f*value){
	return new paramWrapper(std::unique_ptr<std::string>(tag), value);
}

extern "C"
paramWrapper* createQuatParam(std::string *tag, Eigen::Quaternionf*value){
	return new paramWrapper(std::unique_ptr<std::string>(tag), value);
}

extern "C"
paramWrapper* createModelParam(std::string *tag, vertexManager *value){
	return new paramWrapper(std::unique_ptr<std::string>(tag), value);
}

extern "C"
paramWrapper* createEmParam(std::string *tag, elementManager *value){
	return new paramWrapper(std::unique_ptr<std::string>(tag), value);
}


//-------------------------------------------------------------------


parameterPack::parameterPack(){
}

parameterPack::parameterPack(int count, va_list arguments){
	for(int i = 0; i < count; i++){
		paramList.push_back(std::shared_ptr<paramWrapper>(va_arg(arguments, paramWrapper*)));
	}
	va_end(arguments);
}

parameterPack::parameterPack(const parameterPack& rhs): paramList(rhs.paramList){
}

parameterPack::~parameterPack(){

}

void parameterPack::destroy(){
	delete this;
}

std::shared_ptr<paramWrapper> parameterPack::search(std::string input){
	for(auto elem: paramList){
		if(*(elem->tag) == input){
			return elem;
		}
	}

	throw parameterPackException(input);

}

void parameterPack::add(paramWrapper* input){
	paramList.push_back(std::shared_ptr<paramWrapper>(input));
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
	return new paramWrapper(std::make_unique<std::string>(tag), value);
}

paramWrapper* param(std::string tag, float value){
	return new paramWrapper(std::make_unique<std::string>(tag), value);
}

paramWrapper* param(std::string tag, std::string value){
	return new paramWrapper(std::make_unique<std::string>(tag), new std::string(value));
}

paramWrapper* param(std::string tag, Eigen::Vector3f* value){
	return new paramWrapper(std::make_unique<std::string>(tag), value);
}

paramWrapper* param(std::string tag, Eigen::Quaternionf* value){
	return new paramWrapper(std::make_unique<std::string>(tag), value);
}

paramWrapper* param(std::string tag, vertexManager* value){
	return new paramWrapper(std::make_unique<std::string>(tag), value);
}

paramWrapper* param(std::string tag, elementManager* value){
	return new paramWrapper(std::make_unique<std::string>(tag), value);
}

//-------------------------------------------------------------------

