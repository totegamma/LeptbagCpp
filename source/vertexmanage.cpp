#include "vertexmanage.hpp"


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
