#include "universalVector.hpp"

vec3::vec3(){
	x = y = z = 0;
}

vec3::vec3(float x, float y, float z){
	this->x = x;
	this->y = y;
	this->z = z;
}

float vec3::getx(){return x;};
float vec3::gety(){return y;};
float vec3::getz(){return z;};

btVector3 vec3::toBullet(){
	return btVector3(x, y, z);
}

glm::vec3 vec3::toGlm(){
	return glm::vec3(x, y, z);
}

vec3* createVec3(float x, float y, float z){
	return new vec3(x, y, z);//XXX 未確認
}

quat::quat(){
	w = 1;
	x = y = z = 0;
}

quat::quat(float w, float x, float y, float z){
	this->w = w;
	this->x = x;
	this->y = y;
	this->z = z;
}

float quat::getw(){return w;};
float quat::getx(){return x;};
float quat::gety(){return y;};
float quat::getz(){return z;};


btQuaternion quat::toBullet(){
	return btQuaternion(x, y, z, w);
}

glm::quat quat::toGlm(){
	return glm::quat(w, x, y, z);
}

quat* createQuat(float w, float x, float y, float z){
	return new quat(w, x, y, z);//XXX 未確認
}
