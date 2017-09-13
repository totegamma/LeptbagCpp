#include "universalVector.hpp"
#include <iostream>

int vec3::count = 0;

vec3::vec3(){
	x = y = z = 0;
	count++;
}

vec3::vec3(float x, float y, float z): x(x), y(y), z(z){
	count++;
}

vec3::vec3(const vec3& rhs): x(rhs.x), y(rhs.y), z(rhs.z){
	count++;
}

vec3::~vec3(){
	count--;
}

float vec3::getx(){return x;};
float vec3::gety(){return y;};
float vec3::getz(){return z;};

void vec3::destroy(){
	delete this;
}

btVector3 vec3::toBullet(){
	return btVector3(x, y, z);
}

glm::vec3 vec3::toGlm(){
	return glm::vec3(x, y, z);
}

vec3* createVec3(float x, float y, float z){
	return new vec3(x, y, z);//XXX 未確認
}

int quat::count = 0;

quat::quat(){
	w = 1;
	x = y = z = 0;

	count++;
}

quat::quat(float w, float x, float y, float z){
	this->w = w;
	this->x = x;
	this->y = y;
	this->z = z;
	
	count++;
}

quat::quat(const quat& rhs): w(rhs.w), x(rhs.x), y(rhs.y), z(rhs.z){
	count++;
}

quat::~quat(){
	count--;
}

float quat::getw(){return w;};
float quat::getx(){return x;};
float quat::gety(){return y;};
float quat::getz(){return z;};

void quat::destroy(){
	delete this;
}

btQuaternion quat::toBullet(){
	return btQuaternion(x, y, z, w);
}

glm::quat quat::toGlm(){
	return glm::quat(w, x, y, z);
}

quat* createQuat(float w, float x, float y, float z){
	return new quat(w, x, y, z);//XXX 未確認
}
