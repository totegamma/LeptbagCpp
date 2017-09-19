#include "vertexManager.hpp"
#include <iostream>

int vertex::count = 0;

vertex::vertex(){
	count++;
}

vertex::vertex	(GLfloat positionX, GLfloat positionY, GLfloat positionZ,
				GLfloat normalX, GLfloat normalY, GLfloat normalZ,
				GLfloat colorR, GLfloat colorG, GLfloat colorB)
				:positionX(positionX), positionY(positionY), positionZ(positionZ),
				 normalX(normalX), normalY(normalY), normalZ(normalZ),
				 colorR(colorR), colorG(colorG), colorB(colorB){
	
	count++;
	//std::cout << "vertex constructed(" << count << ")" << std::endl;
}

vertex::vertex(const vertex& rhs)
				:positionX(rhs.positionX), positionY(rhs.positionY), positionZ(rhs.positionZ),
				 normalX(rhs.normalX), normalY(rhs.normalY), normalZ(rhs.normalZ),
				 colorR(rhs.colorR), colorG(rhs.colorG), colorB(rhs.colorB){
	
	count++;
	//std::cout << "vertex copied     (" << count << ")" << std::endl;
}

vertex::~vertex(){
	count--;
	//std::cout << "vertex destructed (" << count << ")" << std::endl;
}

GLuint vertexBufferObject;
std::vector <vertex> vertexBufferArray;

void initVBO(){
	glGenBuffers(1, &vertexBufferObject);
}

void registervertex(std::shared_ptr<std::vector<vertex>> input, std::vector<GLuint>* arrayaddr){

	for(auto elem: *input){
		auto itr = std::find(vertexBufferArray.begin(), vertexBufferArray.end(), elem);
		if( itr != vertexBufferArray.end() ){
			arrayaddr->push_back(std::distance(vertexBufferArray.begin(), itr));
		}else{
			vertexBufferArray.push_back(elem);
			arrayaddr->push_back(vertexBufferArray.size() -1);
		}
	}


	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, vertexBufferArray.size() * sizeof(vertex), &vertexBufferArray[0], GL_STATIC_DRAW);
}

/*
vertex* createVertex(float coordinate_x, float coordinate_y, float coordinate_z, float normal_x, float normal_y, float normal_z, float color_r, float color_g, float color_b){
	return new vertex(coordinate_x, coordinate_y, coordinate_z, normal_x, normal_y, normal_z, color_r, color_g, color_b);//XXX 未確認
}
*/


int vertexManager::count = 0;


void vertexManager::addVertex(GLfloat positionX, GLfloat positionY, GLfloat positionZ,
				GLfloat normalX, GLfloat normalY, GLfloat normalZ,
				GLfloat colorR, GLfloat colorG, GLfloat colorB){
	vertexList->push_back(vertex(positionX, positionY, positionZ, normalX, normalY, normalZ, colorR, colorG, colorB));
}

std::shared_ptr<std::vector<vertex>> vertexManager::getList(){
	return vertexList;
}

extern "C" vertexManager* createVertexManager(){
	std::cout << "createVertexManager called" << std::endl;
	return new vertexManager();//XXX 未確認
}


vertexManager::vertexManager(){
	vertexList = std::make_shared<std::vector<vertex>>();
	count++;
}

void vertexManager::destroy(){
	delete this;
}

vertexManager::~vertexManager(){
	count--;
}

