#include "parameterPack.hpp"


parameterPackException::parameterPackException() {
}

parameterPackException::parameterPackException(std::string key) {
	std::cout << "Exception: key not found(" << key << ")" << std::endl;
}

param::param(std::string tag, int data) {
	this->type = INT;
	this->data.intValue = data;
	this->tag = tag;
}

param::param(std::string tag, float data) {
	this->type = FLOAT;
	this->data.floatValue = data;
	this->tag = tag;
}

param::param(std::string tag, std::string data) {
	this->type = STRING;
	this->data.stringValue = data;
	this->tag = tag;
}

param::param(std::string tag, Eigen::Vector3f data) {
	this->type = VEC3;
	this->data.vec3Value = data;
	this->tag = tag;
}

param::param(std::string tag, Eigen::Quaternionf data) {
	this->type = QUAT;
	this->data.quatValue = data;
	this->tag = tag;
}

param::param(std::string tag, std::vector<vertex> data) {
	this->type = MODEL;
	this->data.modelValue = data;
	this->tag = tag;
}

param::param(std::string tag, elementManager* data) {
	this->type = EM;
	this->data.emValue = data;
	this->tag = tag;
}


bool param::isTag(std::string target) {
	if (this->tag == target) return true;
	return false;
}


int param::getInt(){
	assert(type == INT);
	return data.intValue;
}

float param::getFloat(){
	assert(type == FLOAT);
	return data.floatValue;
}

std::string param::getString(){
	assert(type == STRING);
	return data.stringValue;
}

Eigen::Vector3f param::getVec3(){
	assert(type == VEC3);
	return data.vec3Value;
}

Eigen::Quaternionf param::getQuat(){
	assert(type == QUAT);
	return data.quatValue;
}

std::vector<vertex> param::getModel(){
	assert(type == MODEL);
	return data.modelValue;
}

elementManager* param::getEm(){
	assert(type == EM);
	return data.emValue;
}



parameterPack::parameterPack(int count, ...) {
	va_list arg;
	va_start(arg, count);
	for (int i = 0; i < count; i++) {
		this->container.push_back(va_arg(arg, param));
	}
	va_end(arg);
}

param parameterPack::search(std::string target) {
	for (auto elem : container) {
		if (elem.isTag(target)) {
			return elem;
		}
	}
	throw parameterPackException(target);
}

void parameterPack::add(param input) {
	this->container.push_back(input);
}

