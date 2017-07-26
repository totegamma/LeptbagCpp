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

	int id;

	public:
	elementNode();
	elementNode(int id, elementManager* parent, btRigidBody* body, vec3 position, vec3 scale, quat rotation);
	~elementNode();
	virtual float getXpos();
	virtual float getYpos();
	virtual float getZpos();
	virtual void destroy();

	void loadMatrix(std::vector<glm::mat4> *input);
	btRigidBody* getBody();
	int getID();
	void changeID(int newID);
};



#endif
