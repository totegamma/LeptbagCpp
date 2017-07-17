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

	btRigidBody (*bodyGenerator)();

	static std::vector<elementManager*> elementManagerList;

	public:

	elementManager();
	elementManager(std::vector<vertex> elementData, btRigidBody (*bodyGenerator)());
	~elementManager();

	//virtual void addVertex(vertex &newvertex);
	//virtual void registerToSystem();
	virtual elementNode* generate();
	template<typename... Args>
	elementNode* generate(vec3 position, vec3 scale, quat rotation, Args... args);
	void destroy(int id);
	void render();
};

#endif
