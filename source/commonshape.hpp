#ifndef COMMONSHAPE_HPP
#define COMMONSHAPE_HPP

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <btBulletDynamicsCommon.h>

#include "vertexmanage.hpp"

extern btDiscreteDynamicsWorld *dynamicsWorld;

class commonshapeObject{

	private:

	static GLuint indexBufferObject;
	static GLuint instanceMatrixBuffer;
	static std::vector<GLuint> indexBufferArray;

	static std::vector<glm::mat4> instanceMatrixArray;
	static std::vector<commonshapeObject*> objects;

	static std::vector<vertex> objectData;


	public:

	static int numOfObject;

	int id;
	btDiscreteDynamicsWorld *dynamicsWorld;
	btRigidBody* body;
	glm::vec3 size;

	commonshapeObject(int id, btRigidBody* body, glm::vec3 size, btDiscreteDynamicsWorld *dynamicsWorld);
	virtual void  destroy();
	virtual float getXpos();
	virtual float getYpos();
	virtual float getZpos();

	void changeID(int newID);
	void loadMotionState();

	static void init();
	static commonshapeObject* create(glm::vec3 position, glm::vec3 size, glm::quat quat, btScalar mass, btDiscreteDynamicsWorld *dynamicsWorld);
	static void destroy(int id);
	static void render();


};

GLuint commonshapeObject::indexBufferObject;
GLuint commonshapeObject::instanceMatrixBuffer;
std::vector<GLuint> commonshapeObject::indexBufferArray;
std::vector<glm::mat4> commonshapeObject::instanceMatrixArray;
std::vector<commonshapeObject*> commonshapeObject::objects;
std::vector<vertex> commonshapeObject::objectData;
int commonshapeObject::numOfObject;


extern commonshapeObject* commonshape_create(float x, float y, float z, float w, float h, float d, float qw, float qx, float qy, float qz, float g);

#endif

