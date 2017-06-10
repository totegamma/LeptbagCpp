#ifndef VERTEXMANAGE_HPP
#define VERTEXMANAGE_HPP

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <btBulletDynamicsCommon.h>


struct vertex{
	GLfloat positionX;
	GLfloat positionY;
	GLfloat positionZ;
	GLfloat colorR;
	GLfloat colorG;
	GLfloat colorB;

	vertex(GLfloat positionX, GLfloat positionY, GLfloat positionZ, GLfloat colorR, GLfloat colorG, GLfloat colorB){
		this->positionX = positionX;
		this->positionY = positionY;
		this->positionZ = positionZ;
		this->colorR = colorR;
		this->colorG = colorG;
		this->colorB = colorB;
	}

	bool operator==(const vertex& v) {
		return (	this->positionX == v.positionX
				&&	this->positionY == v.positionY
				&&	this->positionZ == v.positionZ
				&&	this->colorR == v.colorR
				&&	this->colorG == v.colorG
				&&	this->colorB == v.colorB);
	}

};


extern GLuint vertexBufferObject;

extern std::vector <vertex> vertexBufferArray;

extern void initVBO();

extern void registervertex(vertex input[], GLuint* arrayaddr, int length);

/*
std::vector <vertex> vertexBufferArray {
	vertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f),
	vertex(-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f),
	vertex(-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f),
	vertex(-0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f),
	vertex( 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f),
	vertex( 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f),
	vertex( 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f),
	vertex( 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f),

	vertex(-100.0f, 0.0f, -100.0f, 0.0f, 0.0f, 1.0f),
	vertex(-100.0f, 0.0f,  100.0f, 0.0f, 1.0f, 0.0f),
	vertex( 100.0f, 0.0f, -100.0f, 1.0f, 0.0f, 0.0f),
	vertex( 100.0f, 0.0f,  100.0f, 1.0f, 1.0f, 1.0f)
};
*/

#endif
