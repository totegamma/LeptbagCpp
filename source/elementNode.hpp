#ifndef ELEMENTNODE_HPP
#define ELEMENTNODE_HPP

class elementNode;

#include "elementGenerator.hpp"
#include "universalVector.hpp"

class elementNode{
	elementGenerator* parent;
	btRigidBody* body;
	vec3 initialPosition;
	vec3 initialScale;
	quat initialRotation;

	public:
	elementNode();
	elementNode(elementGenerator* parent, btRigidBody* body, vec3 position, vec3 scale, quat rotation);
	~elementNode();

	glm::mat4 loadMatrix();
};



#endif
