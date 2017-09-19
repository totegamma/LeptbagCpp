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

extern void registervertex(std::shared_ptr<std::vector<std::shared_ptr<vertex>>> input, std::vector<GLuint>* arrayaddr);

//extern "C" vertex* createVertex(float coordinate_x, float coordinate_y, float coordinate_z, float normal_x, float normal_y, float normal_z, float color_r, float color_g, float color_b);

class vertexManager_interface{
	virtual void addVertex(vertex& input) = 0;
	virtual void destroy() = 0;
};


class vertexManager{
	std::shared_ptr<std::vector<std::shared_ptr<vertex>>> vertexList;
	public:
	static int count;
	virtual void addVertex(float coordinate_x, float coordinate_y, float coordinate_z, float normal_x, float normal_y, float normal_z, float color_r, float color_g, float color_b);
	virtual void destroy();
	virtual ~vertexManager();
	std::shared_ptr<std::vector<std::shared_ptr<vertex>>> getList();
	vertexManager();
};

extern "C" vertexManager* createVertexManager();


#endif
