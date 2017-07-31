#ifndef ELEMENTNODE_HPP
#define ELEMENTNODE_HPP

class elementNode;

#include "elementManager.hpp"
#include "utilities/utilities.hpp"
#include <iostream>

class elementNode{
	elementManager* parent;
	btRigidBody* body;
	vec3 initialPosition;
	vec3 initialScale;
	quat initialRotation;

	int id;

	public:
	elementNode();
	elementNode(int id, elementManager* parent, btRigidBody* body, vec3 position, vec3 scale, quat rotation);
	virtual float getXpos();
	virtual float getYpos();
	virtual float getZpos();
	virtual float getBasis(int row, int column);
	virtual void destroy();

	virtual~elementNode();

	void loadMatrix(std::vector<glm::mat4> *input);
	btRigidBody* getBody();
	int getID();
	void changeID(int newID);
};



#endif
