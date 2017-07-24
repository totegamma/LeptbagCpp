#ifndef ELEMENTNODE_HPP
#define ELEMENTNODE_HPP

class elementNode;

#include "elementManager.hpp"
#include "utilities/utilities.hpp"

class elementNode{
	elementManager* parent;
	btRigidBody* body;
	vec3 initialPosition;
	vec3 initialScale;
	quat initialRotation;

	public:
	elementNode();
	elementNode(elementManager* parent, btRigidBody* body, vec3 position, vec3 scale, quat rotation);
	~elementNode();

	glm::mat4 loadMatrix();
	btRigidBody* getBody();
};



#endif
