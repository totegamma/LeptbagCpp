#include "elementNode.hpp"

elementNode::elementNode(){
}

elementNode::elementNode(int id, elementManager* parent, btRigidBody* body, vec3 position, vec3 scale, quat rotation){
	this->id = id;
	this->parent = parent;
	this->body = body;

	this->initialPosition = position;
	this->initialScale = scale;
	this->initialRotation = rotation;
}

elementNode::~elementNode(){
	dynamicsWorld->removeRigidBody(body);
	delete body->getMotionState();
	delete body;
}

float elementNode::getXpos(){
	btTransform transform;
	body->getMotionState()->getWorldTransform(transform);
	btVector3 pos = transform.getOrigin();
	return pos.getX();
}

float elementNode::getYpos(){
	btTransform transform;
	body->getMotionState()->getWorldTransform(transform);
	btVector3 pos = transform.getOrigin();
	return pos.getY();
}

float elementNode::getZpos(){
	btTransform transform;
	body->getMotionState()->getWorldTransform(transform);
	btVector3 pos = transform.getOrigin();
	return pos.getZ();
}

void elementNode::loadMatrix(std::vector<glm::mat4> *input){
	btTransform transform;
	body->getMotionState()->getWorldTransform(transform);

	btVector3 pos = transform.getOrigin();
	btQuaternion rotation = transform.getRotation();

	input->at(id) = glm::translate(glm::mat4(1.0f), glm::vec3(pos.getX(), pos.getY(), pos.getZ())) 
		* glm::toMat4(glm::quat(rotation.getW(), rotation.getX(), rotation.getY(), rotation.getZ()))
		* glm::scale(glm::mat4(1.0f), initialScale.toGlm());
}

void elementNode::destroy(){
	parent->destroy(id);
}


btRigidBody* elementNode::getBody(){
	return body;
}

int elementNode::getID(){
	return id;
}

void elementNode::changeID(int newID){
	id = newID;
}

