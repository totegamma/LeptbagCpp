#ifndef VERTEXMANAGER_HPP
#define VERTEXMANAGER_HPP

#include <vector>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <btBulletDynamicsCommon.h>


class vertex{

	public:

	GLfloat positionX;
	GLfloat positionY;
	GLfloat positionZ;
	GLfloat normalX;
	GLfloat normalY;
	GLfloat normalZ;
	GLfloat colorR;
	GLfloat colorG;
	GLfloat colorB;

	vertex(	GLfloat positionX, GLfloat positionY, GLfloat positionZ,
			GLfloat normalX, GLfloat normalY, GLfloat normalZ,
			GLfloat colorR, GLfloat colorG, GLfloat colorB){
		this->positionX = positionX;
		this->positionY = positionY;
		this->positionZ = positionZ;
		this->normalX = normalX;
		this->normalY = normalY;
		this->normalZ = normalZ;
		this->colorR = colorR;
		this->colorG = colorG;
		this->colorB = colorB;
	}

	bool operator==(const vertex& v) {
		return (	this->positionX == v.positionX
				&&	this->positionY == v.positionY
				&&	this->positionZ == v.positionZ
				&&	this->normalX == v.normalX
				&&	this->normalY == v.normalY
				&&	this->normalZ == v.normalZ
				&&	this->colorR == v.colorR
				&&	this->colorG == v.colorG
				&&	this->colorB == v.colorB);
	}

};


extern GLuint vertexBufferObject;

extern std::vector <vertex> vertexBufferArray;

extern void initVBO();

extern void registervertex(std::vector<vertex>* input, std::vector<GLuint>* arrayaddr);

extern "C" vertex* createVertex(float coordinate_x, float coordinate_y, float coordinate_z, float normal_x, float normal_y, float normal_z, float color_r, float color_g, float color_b);


class vertexManager{
	std::vector<vertex> vertexList;
	public:
	virtual void addVertex(vertex& input);
	std::vector<vertex> getList();
};

extern "C" vertexManager* createVertexManager();


#endif
