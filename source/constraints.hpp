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
	hingeConstraint() = delete;
	hingeConstraint(elementNode* elemA_rawp, elementNode* elemB_rawp, vec3* positionA_rawp, vec3* positionB_rawp, vec3* axisA_rawp, vec3* axisB_rapw);
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
	virtual void setAngularLimit(vec3* lower_rawp, vec3* upper_rawp) = 0;
	virtual void setLinearLimit(vec3* lower_rawp, vec3* upper_rawp) = 0;
	virtual void setRotationalMotor(int index) = 0;
	virtual void setLinearMotor(int index) = 0;
	virtual void setMaxRotationalMotorForce(int index, float force) = 0;
	virtual void setMaxLinearMotorForce(vec3* force_rawp) = 0;
	virtual void setRotationalTargetVelocity(vec3* velocity_rawp) = 0;
	virtual void setLinearTargetVelocity(vec3* velocity_rawp) = 0;
	virtual void destroy() = 0;
};

class generic6DofConstraint: public generic6DofConstraint_interface{
	btGeneric6DofConstraint* gen6Dof;
	public:
	generic6DofConstraint() = delete;
	generic6DofConstraint(elementNode* elemA_rawp, elementNode* elemB_rawp, vec3* positionA_rawp, vec3* positionB_rawp, quat* rotation_rawp);
	virtual float getAngle(int index);
	virtual void setAngularLimit(vec3* lower_rawp, vec3* upper_rawp);
	virtual void setLinearLimit(vec3* lower_rawp, vec3* upper_rawp);
	virtual void setRotationalMotor(int index);
	virtual void setLinearMotor(int index);
	virtual void setMaxRotationalMotorForce(int index, float force);
	virtual void setMaxLinearMotorForce(vec3* force_rawp);
	virtual void setRotationalTargetVelocity(vec3* velocity_rawp);
	virtual void setLinearTargetVelocity(vec3* velocity_rawp);
	virtual void destroy();
	virtual ~generic6DofConstraint();
};


extern "C" hingeConstraint* createHingeConstraint(elementNode* elemA_rawp, elementNode* elemB_rawp, vec3* positionA_rawp, vec3* positionB_rawp, vec3* axisA_rawp, vec3* axisB_rawp);
extern "C" generic6DofConstraint* createGeneric6DofConstraint_create(elementNode* elemA_rawp, elementNode* elemB_rawp, vec3* positionA_rawp, vec3* positionB_rawp, quat* rotation_rawp);



#endif
