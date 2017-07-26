#ifndef ELEMENTMANAGER_HPP
#define ELEMENTMANAGER_HPP

class elementManager;

#include "vertexmanager.hpp"
#include "elementNode.hpp"
#include "utilities/utilities.hpp"
#include "bodyGenerator.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <btBulletDynamicsCommon.h>

class elementManager{
	GLuint indexBufferObject;
	GLuint instanceMatrixBuffer;

	std::vector<GLuint> indexBufferArray;
	std::vector<glm::mat4> instanceMatrixArray;
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

extern "C" elementManager* createElementManager(vertexManager& vm, btRigidBody* (*bodyGenerator)(parameterPack*));

#endif
