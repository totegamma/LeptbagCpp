#include "constraints.hpp"
#include <iostream>

hingeConstraint::hingeConstraint(){
}

hingeConstraint::hingeConstraint(elementNode* elemA, elementNode* elemB, vec3 &positionA, vec3 &positionB, vec3 &axis){

	hinge = new btHingeConstraint(*(elemA->getBody()), *(elemB->getBody()), positionA.toBullet(), positionB.toBullet(), axis.toBullet(), axis.toBullet());
	dynamicsWorld->addConstraint(hinge, true);

}

void hingeConstraint::enableMotor(bool flag){
	hinge->enableMotor(flag);
}

void hingeConstraint::setLimit(float lower, float upper){
	hinge->setLimit(lower, upper);
}

void hingeConstraint::setMaxMotorImpulse(float power){
	hinge->setMaxMotorImpulse(power);
}

void hingeConstraint::setMotorTarget(float angle, float duration){
	hinge->setMotorTarget(angle, duration);
}

void hingeConstraint::destroy(){
	dynamicsWorld->removeConstraint(hinge);
	delete this;
}

hingeConstraint* hingeConstraint_create(elementNode* elemA, elementNode* elemB, vec3 &positionA, vec3 &positionB, vec3 &axis){
	return new hingeConstraint(elemA, elemB, positionA, positionB, axis);//XXX 未確認
}

generic6DofConstraint::generic6DofConstraint(){
}

generic6DofConstraint::generic6DofConstraint(elementNode* elemA, elementNode* elemB, vec3 &positionA, vec3 &positionB){

	// それぞれの物体の重心を原点としてローカル座標をとる。
	btTransform frameInA, frameInB;
	frameInA = elemA->getBody()->getCenterOfMassTransform();
	frameInB = elemB->getBody()->getCenterOfMassTransform();
	// デフォルトの関節の接点をローカル座標で指定する
	frameInA.setOrigin(positionA.toBullet());
	frameInB.setOrigin(positionB.toBullet());

	// 6Dofを生成
	gen6Dof = new btGeneric6DofConstraint(*(elemA->getBody()), *(elemB->getBody()), frameInA, frameInB, false);
	dynamicsWorld->addConstraint(gen6Dof);

}

void generic6DofConstraint::setAngularLowerLimit(vec3 &angularLower){
	gen6Dof->setAngularLowerLimit(angularLower.toBullet());
}

void generic6DofConstraint::setAngularUpperLimit(vec3 &angularUpper){
	gen6Dof->setAngularUpperLimit(angularUpper.toBullet());
}

void generic6DofConstraint::setLinearLowerLimit(vec3 &linearLower){
	gen6Dof->setLinearLowerLimit(linearLower.toBullet());
}

void generic6DofConstraint::setLinearUpperLimit(vec3 &linearUpper){
	gen6Dof->setLinearUpperLimit(linearUpper.toBullet());
}

void generic6DofConstraint::getRotationalLimitMotor(int index){
	// indexがよくわからん。誰か教えて欲しい。
	gen6Dof->getRotationalLimitMotor(index)->m_enableMotor = true;
}

void generic6DofConstraint::destroy(){
	dynamicsWorld->removeConstraint(gen6Dof);
	delete this;
}

generic6DofConstraint* generic6DofConstraint_create(elementNode* elemA, elementNode* elemB, vec3 &positionA, vec3 &positionB){
	return new generic6DofConstraint(elemA, elemB, positionA, positionB);//XXX 未確認
}
