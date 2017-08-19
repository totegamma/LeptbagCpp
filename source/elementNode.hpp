#ifndef ELEMENTNODE_HPP
#define ELEMENTNODE_HPP

class elementNode;

#include "elementManager.hpp"
#include "utilities/utilities.hpp"
#include <iostream>

class elementNode_interface{
	public:
	virtual float getXpos() const = 0;
	virtual float getYpos() const = 0;
	virtual float getZpos() const = 0;
	virtual float getBasis(int row, int column) const = 0;
	virtual float getFriction() const = 0;
	virtual void setFriction(float coef) = 0;
	virtual void destroy() = 0;
};

class elementNode : public elementNode_interface{

	int id;
	elementManager* parent;
	btRigidBody* body;
	vec3 initialPosition;
	vec3 initialScale;
	quat initialRotation;


	public:
	elementNode() = delete;
	elementNode(int id, elementManager* parent, btRigidBody* body, vec3 position, vec3 scale, quat rotation);
	virtual float getXpos() const;
	virtual float getYpos() const;
	virtual float getZpos() const;
	virtual float getBasis(int row, int column) const;
	virtual float getFriction() const;
	virtual void setFriction(float coef);
	virtual void destroy();

	virtual~elementNode();

	void loadMatrix(std::vector<glm::mat4> *input);
	btRigidBody* getBody() const;
	int getID() const;
	void changeID(int newID);
};



#endif
