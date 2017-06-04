#include "universalVector.hpp"

vec3::vec3(){
	x = y = z = 0;
}

vec3::vec3(float x, float y, float z){
	this->x = x;
	this->y = y;
	this->z = z;
}

btVector3 vec3::toBullet(){
	return btVector3(x, y, z);
}

glm::vec3 vec3::toGlm(){
	return glm::vec3(x, y, z);
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


btQuaternion quat::toBullet(){
	return btQuaternion(x, y, z, w);
}

glm::quat quat::toGlm(){
	return glm::quat(w, x, y, z);
}
