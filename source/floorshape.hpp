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

class floorshapeObject;

namespace floorshape{
	extern GLuint floor_indexBufferObject;
	extern GLuint floor_instanceMatrixBuffer;
	extern GLuint floor_indexBufferArray[4];

	/*
	GLuint floor_indexBufferArray[4] = {
		8, 9, 10, 11
	};
	*/

	//TODO 長さ
	extern std::vector<glm::mat4> floor_instanceMatrixArray;



	extern void init();
	extern floorshapeObject* create(vec3 position, vec3 face, quat rotate, btDiscreteDynamicsWorld *dynamicsWorld);
	extern void destroy(int id);
	extern void render();


}


#endif
