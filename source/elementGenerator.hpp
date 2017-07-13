#ifndef ELEMENTGENERATOR_HPP
#define ELEMENTGENERATOR_HPP

class elementGenerator;

#include "universalVector.hpp"
#include "vertexmanage.hpp"
#include "elementNode.hpp"

class elementGenerator{
	GLuint indexBufferObject;
	GLuint instanceMatrixBuffer;

	std::vector<GLuint> indexBufferArray;
	std::vector<vertex> elementData;
	std::vector<elementNode*> elements;

	static std::vector<elementGenerator*> elementGeneratorList;

	public:

	elementGenerator();
	~elementGenerator();

	virtual void addVertex(vertex &newvertex);
	virtual void registerToSystem();
	virtual elementNode* generate();
	virtual elementNode* generate(vec3 &position, vec3 &scale, quat &rotation);
	virtual elementNode* generate(vec3 &position, vec3 &scale, quat &rotation, float mass);
	void destroy(int id);
	void render();
};

#endif
