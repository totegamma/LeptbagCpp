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
	generic6DofConstraint(elementNode* elemA, elementNode* elemB, vec3 &positionA, vec3 &positionB);
	virtual void setAngularLowerLimit(vec3 &angularLower);
	virtual void setAngularUpperLimit(vec3 &angularUpper);
	virtual void setLinearLowerLimit(vec3 &linearLower);
	virtual void setLinearUpperLimit(vec3 &linearUpper);
	virtual void getRotationalLimitMotor(int index);
	virtual void destroy();
};

extern "C" hingeConstraint* hingeConstraint_create(elementNode* elemA, elementNode* elemB, vec3 &positionA, vec3 &positionB, vec3 &axisA, vec3 &axisB);
extern "C" generic6DofConstraint* generic6DofConstraint_create(elementNode* elemA, elementNode* elemB, vec3 &positionA, vec3 &positionB);



#endif
