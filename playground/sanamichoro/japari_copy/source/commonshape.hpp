#ifndef COMMONSHAPE_HPP
#define COMMONSHAPE_HPP

#include <vector>
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <btBulletDynamicsCommon.h>

#include "vertexmanage.hpp"
#include "universalVector.hpp"


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
	virtual shapePointerObject* create(vec3 &position, vec3 &size, quat &rotate);
	virtual shapePointerObject* create(vec3 &position, vec3 &size, quat &rotate, float mass);
	void destroy(int id);
	void render();

};

extern "C" commonshapeObject* createCommonShapeObject();

extern std::vector<commonshapeObject*> commonshapeList;


class shapePointerObject{
	bool isPhysicalBody;
	commonshapeObject* parent;
	btRigidBody* body;
	vec3 initialPosition;
	vec3 initialSize;
	quat initialQuat;

	public:

	shapePointerObject();
	shapePointerObject(commonshapeObject* parent, bool isPhysical, btRigidBody* body, vec3 posi, vec3 size, quat );
	glm::mat4 loadMatrix();
};

#endif

