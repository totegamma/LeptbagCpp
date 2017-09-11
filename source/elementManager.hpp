#ifndef ELEMENTMANAGER_HPP
#define ELEMENTMANAGER_HPP

class elementManager;

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <bullet/btBulletDynamicsCommon.h>

#include "utilities/utilities.hpp"
#include "elementNode.hpp"
#include "bodyGenerator.hpp"
#include "vertexManager.hpp"

class elementManager_interface{
	public:
	virtual elementNode* generate(parameterPack* raw_input) = 0;
	virtual void destroySelf() = 0;
	virtual void destroyElement(int id) = 0;
};

class elementManager : public elementManager_interface {

	GLuint indexBufferObject;
	GLuint instanceMatrixBuffer;
	std::vector<GLuint> indexBufferArray;
	std::vector<glm::mat4> instanceMatrixArray;
	std::vector<elementNode*> elements;

	public:
	static int count;
	static std::vector<elementManager*> elementManagerList;
	std::shared_ptr<std::vector<vertex>> elementData;
	btRigidBody* (*bodyGenerator)(std::unique_ptr<parameterPack>);

	elementManager(std::shared_ptr<std::vector<vertex>> elementData, btRigidBody* (*bodyGenerator)(std::unique_ptr<parameterPack>));

	virtual elementNode* generate(parameterPack* input);
	virtual void destroySelf();
	virtual void destroyElement(int id);
	virtual ~elementManager();
	void render();
	std::shared_ptr<std::vector<vertex>> getElementDataPtr();


};

extern "C" elementManager* createElementManager(vertexManager* vm, btRigidBody* (*bodyGenerator)(std::unique_ptr<parameterPack>));

#endif
