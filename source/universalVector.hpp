#ifndef UNIVERSAL_VECTOR_HPP 
#define UNIVERSAL_VECTOR_HPP

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <btBulletDynamicsCommon.h>


class vec3{

	public:

	float x;
	float y;
	float z;

	vec3();
	vec3(float x, float y, float z);
	btVector3 toBullet();
	glm::vec3 toGlm();
};

extern "C" createVec3(float x, float y, float z);

class quat{

	public:

	float w;
	float x;
	float y;
	float z;

	quat(){
		w = x = y = z = 0;
	}

	quat(float w, float x, float y, float z){
		this->w = w;
		this->x = x;
		this->y = y;
		this->z = z;
	}


	btQuaternion toBullet(){
		return btQuaternion(w, x, y, z);
	}

	glm::quat toGlm(){
		return glm::quat(x, y, z, w);
	}
}


extern "C" createQuat(float w, float x, float y, float z);


#endif
