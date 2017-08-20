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
#include <bullet/btBulletDynamicsCommon.h>

class elementManager_interface{
	public:
	virtual elementNode* generate(parameterPack* input) = 0;
	virtual void destroySelf() = 0;
	virtual void destroyElement(int id) = 0;
};

class elementManager : public elementManager_interface {

	std::vector<vertex> elementData;
	btRigidBody* (*bodyGenerator)(parameterPack*);

	GLuint indexBufferObject;
	GLuint instanceMatrixBuffer;

	std::vector<GLuint> indexBufferArray;
	std::vector<glm::mat4> instanceMatrixArray;
	std::vector<elementNode*> elements;



	public:

	static std::vector<elementManager*> elementManagerList;

	elementManager(std::vector<vertex> elementData, btRigidBody* (*bodyGenerator)(parameterPack*));

	virtual elementNode* generate(parameterPack* input);
	virtual void destroySelf();
	virtual void destroyElement(int id);
	virtual ~elementManager();
	void render();

};

extern "C" elementManager* createElementManager(vertexManager& vm, btRigidBody* (*bodyGenerator)(parameterPack*));

#endif
