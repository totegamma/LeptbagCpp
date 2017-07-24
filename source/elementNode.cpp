#include "elementNode.hpp"

elementNode::elementNode(){
}

elementNode::elementNode(elementManager* parent, btRigidBody* body, vec3 position, vec3 scale, quat rotation){
	this->parent = parent;
	this->body = body;

	this->initialPosition = position;
	this->initialScale = scale;
	this->initialRotation = rotation;
}

glm::mat4 elementNode::loadMatrix(){
	btTransform transform;
	body->getMotionState()->getWorldTransform(transform);

	btVector3 pos = transform.getOrigin();
	btQuaternion rotation = transform.getRotation();

	return glm::translate(glm::mat4(1.0f), glm::vec3(pos.getX(), pos.getY(), pos.getZ())) 
		* glm::toMat4(glm::quat(rotation.getW(), rotation.getX(), rotation.getY(), rotation.getZ()))
		* glm::scale(glm::mat4(1.0f), initialScale.toGlm());
}


btRigidBody* elementNode::getBody(){
	return body;
}

