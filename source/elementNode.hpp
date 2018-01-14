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
	virtual float getXrot() const = 0;
	virtual float getYrot() const = 0;
	virtual float getZrot() const = 0;
	virtual float getWrot() const = 0;
	virtual float getBasis(int row, int column) const = 0;
	virtual float getFriction() const = 0;
	virtual void setFriction(float coef) = 0;
	virtual void setLinearVelocity(float x, float y, float z) = 0;
	virtual void setAngularVelocity(float x, float y, float z) = 0;
	virtual void activate() = 0;
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
	virtual float getXrot() const;
	virtual float getYrot() const;
	virtual float getZrot() const;
	virtual float getWrot() const;
	virtual float getBasis(int row, int column) const;
	virtual float getFriction() const;
	virtual void setFriction(float coef);
	virtual void setLinearVelocity(float x, float y, float z);
	virtual void setAngularVelocity(float x, float y, float z);
	virtual void activate();
	virtual void destroy();

	virtual~elementNode();

	void loadMatrix(std::vector<glm::mat4> *input);
	btRigidBody* getBody() const;
	int getID() const;
	void changeID(int newID);
};



#endif
