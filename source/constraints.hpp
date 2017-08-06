#ifndef CONSTRAINTS_HPP
#define CONSTRAINTS_HPP

#include <bullet/btBulletDynamicsCommon.h>

#include "elementNode.hpp"
#include "utilities/utilities.hpp"

extern btDiscreteDynamicsWorld *dynamicsWorld;

class hingeConstraint{
	btHingeConstraint* hinge;
	public:
	hingeConstraint();
	hingeConstraint(elementNode* elemA, elementNode* elemB, vec3 &positionA, vec3 &positionB, vec3 &axisA, vec3 &axisB);
	virtual void enableMotor(bool flag);
	virtual void setLimit(float lower, float upper);
	virtual void setMaxMotorImpulse(float power);
	virtual void setMotorTarget(float angle, float duration);
	virtual void destroy();
};

class generic6DofConstraint{
	btGeneric6DofConstraint* gen6Dof;
	public:
	generic6DofConstraint();
	generic6DofConstraint(elementNode* elemA, elementNode* elemB, vec3 &positionA, vec3 &positionB, quat &rotation);
	virtual void setAngularLimit(vec3 &lower, vec3 &upper);
	virtual void setLinearLimit(vec3 &lower, vec3 &upper);

	virtual void setRotationalMotor(int index);
	virtual void setLinearMotor(int index);
	virtual void setMaxRotationalMotorForce(int index, float force);
	virtual void setMaxLinearMotorForce(vec3 &force);
	virtual void setRotationalTargetVelocity(vec3 &velocity);
	virtual void setLinearTargetVelocity(vec3 &velocity);

	virtual void destroy();
};

extern "C" hingeConstraint* hingeConstraint_create(elementNode* elemA, elementNode* elemB, vec3 &positionA, vec3 &positionB, vec3 &axisA, vec3 &axisB);
extern "C" generic6DofConstraint* generic6DofConstraint_create(elementNode* elemA, elementNode* elemB, vec3 &positionA, vec3 &positionB, quat &rotation);



#endif
