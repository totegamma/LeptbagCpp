#ifndef CONSTRAINTS_HPP
#define CONSTRAINTS_HPP

#include <btBulletDynamicsCommon.h>

#include "elementNode.hpp"
#include "utilities/utilities.hpp"

extern btDiscreteDynamicsWorld *dynamicsWorld;

class hingeConstraint{
	btHingeConstraint* hinge;
	public:
	hingeConstraint();
	hingeConstraint(elementNode* elemA, elementNode* elemB, vec3 positionA, vec3 positionB, vec3 axis);
	virtual void enableMotor(bool flag);
	virtual void setLimit(float lower, float upper);
	virtual void setMaxMotorImpulse(float power);
	virtual void setMotorTarget(float angle, float duration);
	virtual void destroy();
};

extern "C" hingeConstraint* hingeConstraint_create(elementNode* elemA, elementNode* elemB, vec3 &positionA, vec3 &positionB, vec3 &axis);



#endif
