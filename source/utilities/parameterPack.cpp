#include "parameterPack.hpp"
#include <iostream>
#include <stdarg.h>

int paramWrapper::count = 0;

paramWrapper::paramWrapper(std::unique_ptr<univStr> tag, int intValue){
	this->tag = std::move(tag);
	this->data.intValue = intValue;

	contain = INT;
	count ++;
}

paramWrapper::paramWrapper(std::unique_ptr<univStr> tag, float floatValue){
	this->tag = std::move(tag);
	this->data.floatValue = floatValue;

	contain = FLOAT;
	count ++;
}

paramWrapper::paramWrapper(std::unique_ptr<univStr> tag, univStr* stringValue){
	this->tag = std::move(tag);
	this->data.stringValue = stringValue;

	contain = STRING;
	count ++;
}

paramWrapper::paramWrapper(std::unique_ptr<univStr> tag, vec3* vec3Value){
	this->tag = std::move(tag);
	this->data.vec3Value = vec3Value;

	contain = VEC3;
	count ++;
}

paramWrapper::paramWrapper(std::unique_ptr<univStr> tag, quat* quatValue){
	this->tag = std::move(tag);
	this->data.quatValue = quatValue;

	contain = QUAT;
	count ++;
}

paramWrapper::paramWrapper(std::unique_ptr<univStr> tag, vertexManager* modelValue){
	this->tag = std::move(tag);
	this->data.modelValue = modelValue;

	contain = MODEL;
	count ++;
}

paramWrapper::paramWrapper(std::unique_ptr<univStr> tag, elementManager* emValue){
	this->tag = std::move(tag);
	this->data.emValue = emValue;

	contain = EM;
	count ++;
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

	count --;
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

elementManager* paramWrapper::getEm(){
	assert(contain == EM);
	return data.emValue;
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

extern "C"
paramWrapper* createEmParam(univStr *tag, elementManager *value){
	return new paramWrapper(std::unique_ptr<univStr>(tag), value);//XXX 未確認
}


//-------------------------------------------------------------------

int parameterPack::count = 0;

parameterPack::parameterPack(){
	count++;
}

parameterPack::parameterPack(int count, va_list arguments){
	for(int i = 0; i < count; i++){
		paramList.push_back(std::shared_ptr<paramWrapper>(va_arg(arguments, paramWrapper*)));
	}
	va_end(arguments);
	this->count++;
}

parameterPack::parameterPack(const parameterPack& rhs): paramList(rhs.paramList){
	count++;
}

parameterPack::~parameterPack(){

	count--;
}

std::shared_ptr<paramWrapper> parameterPack::search(std::string input){
	for(auto elem: paramList){
		if(elem->tag->getString() == input){
			return elem;
		}
	}
	std::cout << "404: " << input << std::endl; //TODO ここは例外処理にしたい
	return nullptr;
}

void parameterPack::add(paramWrapper* input){
	paramList.push_back(std::shared_ptr<paramWrapper>(input));
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

paramWrapper* param(std::string tag, elementManager* value){
	return new paramWrapper(std::make_unique<univStr>(tag), value);//XXX 未確認
}

//-------------------------------------------------------------------

