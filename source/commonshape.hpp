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




#endif

