#ifndef ELEMENTMANAGER_HPP
#define ELEMENTMANAGER_HPP

class elementManager;

#include "universalVector.hpp"
#include "vertexmanager.hpp"
#include "elementNode.hpp"
#include "utilities/utilities.hpp"
#include <tuple>

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

	virtual elementNode* generate();
	template<typename... Args>
	elementNode* generate(Args... args);
	void destroy(int id);
	void render();
};

#endif
