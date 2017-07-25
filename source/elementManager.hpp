#ifndef ELEMENTMANAGER_HPP
#define ELEMENTMANAGER_HPP

class elementManager;

#include "vertexmanager.hpp"
#include "elementNode.hpp"
#include "utilities/utilities.hpp"
#include "bodyGenerator.hpp"

class elementManager{
	GLuint indexBufferObject;
	GLuint instanceMatrixBuffer;

	std::vector<GLuint> indexBufferArray;
	std::vector<vertex> elementData;
	std::vector<elementNode*> elements;

	btRigidBody* (*bodyGenerator)(parameterPack*);


	public:

	static std::vector<elementManager*> elementManagerList;

	elementManager();
	elementManager(std::vector<vertex> elementData, btRigidBody* (*bodyGenerator)(parameterPack*));
	~elementManager();

	virtual elementNode* generate(parameterPack* input);
	void destroy(int id);
	void render();
};

#endif
