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

//TODO 別ファイルにする
class shapePointerObject;

class commonshapeObject{
	GLuint indexBufferObject;
	GLuint instanceMatrixBuffer;

	std::vector<GLuint> indexBufferArray;
	//std::vector<glm::mat4> instanceMatrixArray;

	std::vector<vertex> objectData;

	std::vector<shapePointerObject*> objects;

	public:

	commonshapeObject();
	~commonshapeObject();

	void addVertex(vertex newvertex);
	void registerToSystem();
	shapePointerObject* create();
	shapePointerObject* create(glm::vec3 position, glm::vec3 size, glm::quat quat);
	shapePointerObject* create(glm::vec3 position, glm::vec3 size, glm::quat quat, btScalar mass, btDiscreteDynamicsWorld *dynamicsWorld);
	void destroy(int id);
	void render();

};

extern std::vector<commonshapeObject*> commonshapeList;


class shapePointerObject{
	bool isPhysicalBody;
	commonshapeObject* parent;
	btRigidBody* body;
	glm::vec3 initialPosition;
	glm::vec3 initialSize;
	glm::quat initialQuat;

	public:

	shapePointerObject();
	shapePointerObject(commonshapeObject* parent, bool isPhysical, btRigidBody* body, glm::vec3 posi, glm::vec3 size, glm::quat quat);
	glm::mat4 loadMatrix();
};

#endif

