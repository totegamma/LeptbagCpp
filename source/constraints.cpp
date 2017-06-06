#include "constraints.hpp"
#include <iostream>

hingeConstraint::hingeConstraint(){
}

hingeConstraint::hingeConstraint(cubeshapeObject* cubeA, cubeshapeObject* cubeB, vec3 positionA, vec3 positionB, vec3 axis){

	hinge = new btHingeConstraint(*(cubeA->body), *(cubeB->body), positionA.toBullet(), positionB.toBullet(), axis.toBullet(), axis.toBullet());
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
}

hingeConstraint* hingeConstraint_create(cubeshapeObject* cubeA, cubeshapeObject* cubeB, vec3 &positionA, vec3 &positionB, vec3 &axis){
	return new hingeConstraint(cubeA, cubeB, positionA, positionB, axis);
}
