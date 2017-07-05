#ifndef ELEMENTNODE_HPP
#define ELEMENTNODE_HPP

#include "elementCreator.hpp"
#include "universalVector.hpp"

class elementNode{
	elementCreator* parent;
	btRigidBody* body;
	vec3 initialPosition;
	vec3 initialScale;
	quat initialRotation;

	public:
	elementNode();
	~elementNode();

	shapePointerObject();
	shapePointerObject(commonshapeObject* parent, btRigidBody* body, vec3 position, vec3 scale, quat rotation);
	glm::mat4 loadMatrix();
};



#endif
