#ifndef CONSTRAINTS_HPP
#define CONSTRAINTS_HPP

#include <btBulletDynamicsCommon.h>

#include "cubeshape.hpp"

extern btDiscreteDynamicsWorld *dynamicsWorld;

//TODO 名前これでええんか？？？？
class hingeConstraint{
	btHingeConstraint* hinge;
	public:
	hingeConstraint();
	hingeConstraint(cubeshapeObject* cubeA, cubeshapeObject* cubeB, float ax, float ay, float az, float bx, float by, float bz, float vx, float vy, float vz);
	void enableMotor(bool flag);
	void setLimit(float lower, float upper);
	void setMaxMotorImpulse(float power);
	void destroy();
};

extern "C" hingeConstraint* hingeConstraint_create(cubeshapeObject* cubeA, cubeshapeObject* cubeB, float ax, float ay, float az, float bx, float by, float bz, float vx, float vy, float vz);



#endif
