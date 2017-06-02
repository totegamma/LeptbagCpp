#ifndef COMMONSHAPE_HPP
#define COMMONSHAPE_HPP

#include <vector>
#include <stdio.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <btBulletDynamicsCommon.h>

#include "vertexmanage.hpp"

extern btDiscreteDynamicsWorld *dynamicsWorld;

//TODO 別ファイルにする
class shapePointerObject;

class commonshapeObject{
	GLuint indexBufferObject;
	GLuint instanceMatrixBuffer;

	std::vector<GLuint> indexBufferArray;

	std::vector<vertex> objectData;

	std::vector<shapePointerObject*> objects;

	public:

	commonshapeObject();
	~commonshapeObject();

	virtual void addVertex(vertex &newvertex);
	virtual void registerToSystem();
	virtual shapePointerObject* create();
	virtual shapePointerObject* create(float posx, float posy, float posz, float sizx, float sizy, float sizz, float quatw, float quatx, float quaty, float quatz);
	virtual shapePointerObject* create(float posx, float posy, float posz, float sizx, float sizy, float sizz, float quatw, float quatx, float quaty, float quatz, float mass);
	void destroy(int id);
	void render();

};

extern "C" commonshapeObject* createCommonShapeObject();

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

