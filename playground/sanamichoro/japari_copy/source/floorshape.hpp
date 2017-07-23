#ifndef FLOORSHAPE_HPP
#define FLOORSHAPE_HPP

#include <vector>

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

class floorshapeObject{
	public:
	int id;
	btDiscreteDynamicsWorld *dynamicsWorld;
	btRigidBody* body;

	floorshapeObject(int id, btRigidBody* body, btDiscreteDynamicsWorld *dynamicsWorld);

	void changeID(int newID);

};

namespace floorshape{
	extern GLuint floor_indexBufferObject;
	extern GLuint floor_instanceMatrixBuffer;
	extern GLuint floor_indexBufferArray[4];

	//TODO 長さ
	extern std::vector<glm::mat4> floor_instanceMatrixArray;



	extern void init();
	extern floorshapeObject* create(vec3 position, vec3 face, quat rotate, btDiscreteDynamicsWorld *dynamicsWorld);
	extern void destroy(int id);
	extern void render();
}

extern "C" floorshapeObject* floorshape_create(vec3 &position, vec3 &face, quat &rotate);


#endif
