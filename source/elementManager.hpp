#ifndef ELEMENTMANAGER_HPP
#define ELEMENTMANAGER_HPP

class elementManager;

#include "universalVector.hpp"
#include "vertexmanage.hpp"
#include "elementNode.hpp"

class elementManager{
	GLuint indexBufferObject;
	GLuint instanceMatrixBuffer;

	std::vector<GLuint> indexBufferArray;
	std::vector<vertex> elementData;
	std::vector<elementNode*> elements;

	static std::vector<elementManager*> elementManagerList;

	public:

	elementManager();
	~elementManager();

	virtual void addVertex(vertex &newvertex);
	virtual void registerToSystem();
	virtual elementNode* generate();
	virtual elementNode* generate(vec3 &position, vec3 &scale, quat &rotation);
	virtual elementNode* generate(vec3 &position, vec3 &scale, quat &rotation, float mass);
	void destroy(int id);
	void render();
};

#endif
