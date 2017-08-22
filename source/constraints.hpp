#ifndef CONSTRAINTS_HPP
#define CONSTRAINTS_HPP

#include <bullet/btBulletDynamicsCommon.h>

#include "elementNode.hpp"
#include "utilities/utilities.hpp"

extern btDiscreteDynamicsWorld *dynamicsWorld;

class hingeConstraint_interface{
	public:
	virtual void enableMotor(bool flag) = 0;
	virtual void setLimit(float lower, float upper) = 0;
	virtual void setMaxMotorImpulse(float power) = 0;
	virtual void setMotorTarget(float angle, float duration) = 0;
	virtual void destroy() = 0;
};

class hingeConstraint: public hingeConstraint_interface{
	btHingeConstraint* hinge;
	public:
	static int count;
	hingeConstraint() = delete;
	hingeConstraint(elementNode* elemA, elementNode* elemB, vec3 &positionA, vec3 &positionB, vec3 &axisA, vec3 &axisB);
	virtual void enableMotor(bool flag);
	virtual void setLimit(float lower, float upper);
	virtual void setMaxMotorImpulse(float power);
	virtual void setMotorTarget(float angle, float duration);
	virtual void destroy();
	virtual ~hingeConstraint();
};

class generic6DofConstraint_interface{
	public:
	virtual float getAngle(int index) = 0;
	virtual void setAngularLimit(vec3 &lower, vec3 &upper) = 0;
	virtual void setLinearLimit(vec3 &lower, vec3 &upper) = 0;
	virtual void setRotationalMotor(int index) = 0;
	virtual void setLinearMotor(int index) = 0;
	virtual void setMaxRotationalMotorForce(int index, float force) = 0;
	virtual void setMaxLinearMotorForce(vec3 &force) = 0;
	virtual void setRotationalTargetVelocity(vec3 &velocity) = 0;
	virtual void setLinearTargetVelocity(vec3 &velocity) = 0;
	virtual void destroy() = 0;
};

class generic6DofConstraint: public generic6DofConstraint_interface{
	btGeneric6DofConstraint* gen6Dof;
	public:
	static int count;
	generic6DofConstraint() = delete;
	generic6DofConstraint(elementNode* elemA, elementNode* elemB, vec3 &positionA, vec3 &positionB, quat &rotation);
	virtual float getAngle(int index);
	virtual void setAngularLimit(vec3 &lower, vec3 &upper);
	virtual void setLinearLimit(vec3 &lower, vec3 &upper);
	virtual void setRotationalMotor(int index);
	virtual void setLinearMotor(int index);
	virtual void setMaxRotationalMotorForce(int index, float force);
	virtual void setMaxLinearMotorForce(vec3 &force);
	virtual void setRotationalTargetVelocity(vec3 &velocity);
	virtual void setLinearTargetVelocity(vec3 &velocity);
	virtual void destroy();
	virtual ~generic6DofConstraint();
};


extern "C" hingeConstraint* createHingeConstraint(elementNode* elemA, elementNode* elemB, vec3 &positionA, vec3 &positionB, vec3 &axisA, vec3 &axisB);
extern "C" generic6DofConstraint* createGeneric6DofConstraint_create(elementNode* elemA, elementNode* elemB, vec3 &positionA, vec3 &positionB, quat &rotation);



#endif
