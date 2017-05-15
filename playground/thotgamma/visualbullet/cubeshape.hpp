#ifndef CUBESHAPE_HPP
#define CUBESHAPE_HPP

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <btBulletDynamicsCommon.h>

#include "vertexmanage.hpp"

class cubeshapeObject{
	public:
	int id;
	btRigidBody* body;
	glm::vec3 size;
	cubeshapeObject(int id, btRigidBody* body, glm::vec3 size);
	void loadMotionState();
};

namespace cubeshape{

	extern GLuint indexBufferObject;
	extern GLuint instanceMatrixBuffer;

	extern GLuint indexBufferArray[14];

	extern std::vector<glm::mat4> instanceMatrixArray;


	extern void init();
	extern cubeshapeObject* create(glm::vec3 position, glm::vec3 size, glm::quat quat, btScalar mass, btDiscreteDynamicsWorld *dynamicsWorld);
	extern void destroy(int id);
	extern void render();
}

#endif
