#ifndef ELEMENTNODE_HPP
#define ELEMENTNODE_HPP

class elementNode;

#include "elementManager.hpp"
#include "utilities/utilities.hpp"
#include <iostream>

#include <Eigen/Core>
#include <Eigen/Geometry>


class elementNode {

	int id;
	elementManager* parent;
	btRigidBody* body;
	Eigen::Vector3f initialPosition;
	Eigen::Vector3f initialScale;
	Eigen::Quaternionf initialRotation;


	public:
	elementNode() = delete;
	elementNode(int id, elementManager* parent, btRigidBody* body, Eigen::Vector3f position, Eigen::Vector3f scale, Eigen::Quaternionf rotation);
	float getXpos() const;
	float getYpos() const;
	float getZpos() const;
	float getXrot() const;
	float getYrot() const;
	float getZrot() const;
	float getWrot() const;
	float getBasis(int row, int column) const;
	float getFriction() const;
	void setFriction(float coef);
	void setLinearVelocity(float x, float y, float z);
	void setAngularVelocity(float x, float y, float z);
	void activate();
	void destroy();

	~elementNode();

	void loadMatrix(std::vector<glm::mat4> *input);
	btRigidBody* getBody() const;
	int getID() const;
	void changeID(int newID);
};



#endif
