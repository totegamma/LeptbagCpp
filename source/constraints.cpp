#include "constraints.hpp"
#include <iostream>

hingeConstraint::hingeConstraint(){
}

hingeConstraint::hingeConstraint(cubeshapeObject* cubeA, cubeshapeObject* cubeB, float ax, float ay, float az, float bx, float by, float bz, float vx, float vy, float vz){

	hinge = new btHingeConstraint(*(cubeA->body), *(cubeB->body), btVector3(ax, ay, az), btVector3(bx, by, bz), btVector3(vx, vy, vz), btVector3(vx, vy, vz));
	dynamicsWorld->addConstraint(hinge, true);

}

void hingeConstraint::sayHello(){
	std::cout << "Hello!" << std::endl;
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

void hingeConstraint::destroy(){
	dynamicsWorld->removeConstraint(hinge);
}

hingeConstraint* hingeConstraint_create(cubeshapeObject* cubeA, cubeshapeObject* cubeB, float ax, float ay, float az, float bx, float by, float bz, float vx, float vy, float vz){
	return new hingeConstraint(cubeA, cubeB, ax, ay, az, bx, by, bz, vx, vy, vz);
}
