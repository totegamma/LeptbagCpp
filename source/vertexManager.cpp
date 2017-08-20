#include "vertexManager.hpp"
#include <iostream>


GLuint vertexBufferObject;


std::vector <vertex> vertexBufferArray;

void initVBO(){
	glGenBuffers(1, &vertexBufferObject);
}

void registervertex(std::vector<vertex>* input, std::vector<GLuint>* arrayaddr){

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

vertex* createVertex(float coordinate_x, float coordinate_y, float coordinate_z, float normal_x, float normal_y, float normal_z, float color_r, float color_g, float color_b){
	return new vertex(coordinate_x, coordinate_y, coordinate_z, normal_x, normal_y, normal_z, color_r, color_g, color_b);//XXX 未確認
}


void vertexManager::addVertex(vertex& input){
	vertexList.push_back(input);
}

std::vector<vertex> vertexManager::getList(){
	return vertexList;
}

extern "C" vertexManager* createVertexManager(){
	std::cout << "createVertexManager called" << std::endl;
	return new vertexManager();//XXX 未確認
}


vertexManager::vertexManager(){
	std::cout << "vertexManager constructed" << std::endl;
}

vertexManager::~vertexManager(){
	std::cout << "vertexManager destructed" << std::endl;
}
