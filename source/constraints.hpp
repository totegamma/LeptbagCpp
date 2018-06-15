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
	hingeConstraint(elementNode* elemA, elementNode* elemB, Eigen::Vector3f positionA, Eigen::Vector3f positionB, Eigen::Vector3f axisA, Eigen::Vector3f axisB);
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
	generic6DofConstraint(elementNode* elemA, elementNode* elemB, Eigen::Vector3f positionA, Eigen::Vector3f positionB, Eigen::Quaternionf rotation);
	float getAngle(int index);
	void setAngularLimit(Eigen::Vector3f lower, Eigen::Vector3f upper);
	void setLinearLimit(Eigen::Vector3f lower, Eigen::Vector3f upper);
	void setRotationalMotor(int index);
	void setLinearMotor(int index);
	void setMaxRotationalMotorForce(int index, float force);
	void setMaxLinearMotorForce(Eigen::Vector3f force);
	void setRotationalTargetVelocity(Eigen::Vector3f velocity);
	void setLinearTargetVelocity(Eigen::Vector3f velocity);
	void destroy();
	~generic6DofConstraint();
};



#endif
