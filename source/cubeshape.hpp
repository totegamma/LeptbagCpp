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

#include "vertexmanager.hpp"
#include "universalVector.hpp"

extern btDiscreteDynamicsWorld *dynamicsWorld;

class cubeshapeObject{
	public:

	int id;
	btDiscreteDynamicsWorld *dynamicsWorld;
	btRigidBody* body;
	vec3 size;

	//cubeshapeObject(int id, btRigidBody* body, glm::vec3 size, btDiscreteDynamicsWorld *dynamicsWorld);
	cubeshapeObject(int id, btRigidBody* body, vec3 size, btDiscreteDynamicsWorld *dynamicsWorld);
	virtual void destroy();
	virtual float getXpos();
	virtual float getYpos();
	virtual float getZpos();

	void changeID(int newID);
	void loadMotionState();



};

namespace cubeshape{

	extern GLuint indexBufferObject;
	extern GLuint instanceMatrixBuffer;

	//TODO 長さ36を初期値に設定したい
	extern std::vector<GLuint> indexBufferArray;

	extern std::vector<glm::mat4> instanceMatrixArray;


	extern void init();
	//extern cubeshapeObject* create(glm::vec3 position, glm::vec3 size, glm::quat quat, btScalar mass, btDiscreteDynamicsWorld *dynamicsWorld);
	extern cubeshapeObject* create(vec3 position, vec3 size, quat quat, btScalar mass, btDiscreteDynamicsWorld *dynamicsWorld);
	extern void destroy(int id);
	extern void render();
}

extern "C" cubeshapeObject* cubeshape_create(vec3 &position, vec3 &size, quat &rotation, float weight);

#endif
