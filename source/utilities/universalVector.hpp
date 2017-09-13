#ifndef UNIVERSAL_VECTOR_HPP 
#define UNIVERSAL_VECTOR_HPP

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <bullet/btBulletDynamicsCommon.h>

class vec3_interface{
	public:
	virtual float getx() = 0;
	virtual float gety() = 0;
	virtual float getz() = 0;
	virtual void destroy() = 0;
};


class vec3 final: public vec3_interface{

	float x;
	float y;
	float z;

	public:
	static int count;

	vec3();
	vec3(float x, float y, float z);
	vec3(const vec3& rhs);

	virtual float getx();
	virtual float gety();
	virtual float getz();
	virtual void destroy();
	virtual ~vec3();

	btVector3 toBullet();
	glm::vec3 toGlm();

	vec3& operator= (const vec3& rhs){
		this->x = rhs.x;
		this->y = rhs.y;
		this->z = rhs.z;
		return *this;
	}
};

extern "C" vec3* createVec3(float x, float y, float z);

class quat_interface{
	public:
	virtual float getw() = 0;
	virtual float getx() = 0;
	virtual float gety() = 0;
	virtual float getz() = 0;
	virtual void destroy() = 0;
};

class quat final: quat_interface{

	float w;
	float x;
	float y;
	float z;

	public:
	static int count;

	quat();
	quat(float w, float x, float y, float z);
	quat(const quat& rhs);

	virtual float getw();
	virtual float getx();
	virtual float gety();
	virtual float getz();
	virtual void destroy();
	virtual ~quat();

	btQuaternion toBullet();
	glm::quat toGlm();

	quat& operator= (const quat& rhs){
		this->w = rhs.w;
		this->x = rhs.x;
		this->y = rhs.y;
		this->z = rhs.z;
		return *this;
	}
};


extern "C" quat* createQuat(float w, float x, float y, float z);


#endif
