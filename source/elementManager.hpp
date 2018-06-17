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


class elementManager {

	GLuint indexBufferObject;
	GLuint instanceMatrixBuffer;
	std::vector<GLuint> indexBufferArray;
	std::vector<glm::mat4> instanceMatrixArray;
	std::vector<elementNode*> elements;

	public:
	static std::vector<elementManager*> elementManagerList;
	std::vector<vertex> elementData;
	btRigidBody* (*bodyGenerator)(std::unique_ptr<parameterPack>);

	elementManager(std::vector<vertex> elementData, btRigidBody* (*bodyGenerator)(std::unique_ptr<parameterPack>));

	elementNode* generate(parameterPack* input);
	void destroySelf();
	void destroyElement(int id);
	~elementManager();
	void render();
	std::vector<vertex>* getElementDataPtr(); // TODO WEAKにする？


};


#endif
