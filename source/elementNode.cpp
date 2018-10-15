#include "elementNode.hpp"

elementNode::elementNode(int id, elementManager* parent, btRigidBody* body, Eigen::Vector3f position, Eigen::Vector3f scale, Eigen::Quaternionf rotation)
	: id(id), parent(parent), body(body), initialPosition(position), initialScale(scale), initialRotation(rotation) {
}

elementNode::~elementNode(){
	dynamicsWorld->removeRigidBody(body);
	delete body->getMotionState();
	delete body;
}

float elementNode::getXpos() const {
	btTransform transform;
	body->getMotionState()->getWorldTransform(transform);
	btVector3 pos = transform.getOrigin();
	return pos.getX();
}

float elementNode::getYpos() const {
	btTransform transform;
	body->getMotionState()->getWorldTransform(transform);
	btVector3 pos = transform.getOrigin();
	return pos.getY();
}

float elementNode::getZpos() const {
	btTransform transform;
	body->getMotionState()->getWorldTransform(transform);
	btVector3 pos = transform.getOrigin();
	return pos.getZ();
}

float elementNode::getXrot() const {
	btTransform transform;
	body->getMotionState()->getWorldTransform(transform);
	btQuaternion rot = transform.getRotation();
	return rot.getX();
}

float elementNode::getYrot() const {
	btTransform transform;
	body->getMotionState()->getWorldTransform(transform);
	btQuaternion rot = transform.getRotation();
	return rot.getY();
}

float elementNode::getZrot() const {
	btTransform transform;
	body->getMotionState()->getWorldTransform(transform);
	btQuaternion rot = transform.getRotation();
	return rot.getZ();
}

float elementNode::getWrot() const {
	btTransform transform;
	body->getMotionState()->getWorldTransform(transform);
	btQuaternion rot = transform.getRotation();
	return rot.getW();
}


float elementNode::getBasis(int row, int column) const {
	btTransform transform;
	//transform = body->getCenterOfMassTransform();
	body->getMotionState()->getWorldTransform(transform);
	btVector3 x = transform.getBasis().getColumn(column);
	if(row == 0) return x.getX();
	if(row == 1) return x.getY();
	else         return x.getZ(); //TODO
}

float elementNode::getFriction() const{
	return body->getFriction();
}

void elementNode::setFriction(float coef){
	body->setFriction(coef);
}

void elementNode::setLinearVelocity(float x, float y, float z) {
	body->setLinearVelocity(btVector3(x, y, z));
}
void elementNode::setAngularVelocity(float x, float y, float z) {
	body->setAngularVelocity(btVector3(x, y, z));
}


void elementNode::loadMatrix(std::vector<glm::mat4> *input) {
	btTransform transform;
	body->getMotionState()->getWorldTransform(transform);

	btVector3 pos = transform.getOrigin();
	btQuaternion rotation = transform.getRotation();

	input->at(id) = glm::translate(glm::mat4(1.0f), glm::vec3(pos.getX(), pos.getY(), pos.getZ()))
		* glm::toMat4(glm::quat(rotation.getW(), rotation.getX(), rotation.getY(), rotation.getZ()))
		* glm::scale(glm::mat4(1.0f), toGlmVec3(initialScale));
}

void elementNode::activate() {
	body->activate();
}


void elementNode::destroy() {
	parent->destroyElement(id);
}


btRigidBody* elementNode::getBody() const {
	return body;
}

int elementNode::getID() const {
	return id;
}

void elementNode::changeID(int newID) {
	id = newID;
}
