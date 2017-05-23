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
	virtual void enableMotor(bool flag);
	virtual void setLimit(float lower, float upper);
	virtual void setMaxMotorImpulse(float power);
	virtual void setMotorTarget(float angle, float duration);
	virtual void destroy();
};

extern "C" hingeConstraint* hingeConstraint_create(cubeshapeObject* cubeA, cubeshapeObject* cubeB, float ax, float ay, float az, float bx, float by, float bz, float vx, float vy, float vz);



#endif
