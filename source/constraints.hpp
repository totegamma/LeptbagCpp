#ifndef CONSTRAINTS_HPP
#define CONSTRAINTS_HPP

#include <bullet/btBulletDynamicsCommon.h>
#include <Eigen/Core>
#include <Eigen/Geometry>

#include "elementNode.hpp"
#include "utilities/utilities.hpp"

extern btDiscreteDynamicsWorld *dynamicsWorld;


class hingeConstraint {
	btHingeConstraint* hinge;
	public:
	hingeConstraint() = delete;
	hingeConstraint(elementNode* elemA_rawp, elementNode* elemB_rawp, Eigen::Vector3f* positionA_rawp, Eigen::Vector3f* positionB_rawp, Eigen::Vector3f* axisA_rawp, Eigen::Vector3f* axisB_rapw);
	void enableMotor(bool flag);
	void setLimit(float lower, float upper);
	void setMaxMotorImpulse(float power);
	void setMotorTarget(float angle, float duration);
	void destroy();
	~hingeConstraint();
};


class generic6DofConstraint {
	btGeneric6DofConstraint* gen6Dof;
	public:
	generic6DofConstraint() = delete;
	generic6DofConstraint(elementNode* elemA_rawp, elementNode* elemB_rawp, Eigen::Vector3f* positionA_rawp, Eigen::Vector3f* positionB_rawp, Eigen::Quaternionf* rotation_rawp);
	float getAngle(int index);
	void setAngularLimit(Eigen::Vector3f* lower_rawp, Eigen::Vector3f* upper_rawp);
	void setLinearLimit(Eigen::Vector3f* lower_rawp, Eigen::Vector3f* upper_rawp);
	void setRotationalMotor(int index);
	void setLinearMotor(int index);
	void setMaxRotationalMotorForce(int index, float force);
	void setMaxLinearMotorForce(Eigen::Vector3f* force_rawp);
	void setRotationalTargetVelocity(Eigen::Vector3f* velocity_rawp);
	void setLinearTargetVelocity(Eigen::Vector3f* velocity_rawp);
	void destroy();
	~generic6DofConstraint();
};


extern "C" hingeConstraint* createHingeConstraint(elementNode* elemA_rawp, elementNode* elemB_rawp, Eigen::Vector3f* positionA_rawp, Eigen::Vector3f* positionB_rawp, Eigen::Vector3f* axisA_rawp, Eigen::Vector3f* axisB_rawp);
extern "C" generic6DofConstraint* createGeneric6DofConstraint_create(elementNode* elemA_rawp, elementNode* elemB_rawp, Eigen::Vector3f* positionA_rawp, Eigen::Vector3f* positionB_rawp, Eigen::Quaternionf* rotation_rawp);



#endif
