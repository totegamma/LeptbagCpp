#ifndef VERTEXMANAGER_HPP
#define VERTEXMANAGER_HPP

#include <vector>
#include <algorithm>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <bullet/btBulletDynamicsCommon.h>




class vertex final{

	public:
	static int count;
	GLfloat positionX;
	GLfloat positionY;
	GLfloat positionZ;
	GLfloat normalX;
	GLfloat normalY;
	GLfloat normalZ;
	GLfloat colorR;
	GLfloat colorG;
	GLfloat colorB;

	vertex();
	vertex(	GLfloat positionX, GLfloat positionY, GLfloat positionZ,
			GLfloat normalX, GLfloat normalY, GLfloat normalZ,
			GLfloat colorR, GLfloat colorG, GLfloat colorB);
    vertex(const vertex &rhs);

	~vertex();

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
extern void registervertex(std::vector<vertex> input, std::vector<GLuint>* arrayaddr);




#endif
