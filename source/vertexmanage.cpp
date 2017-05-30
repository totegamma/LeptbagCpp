#include "vertexmanage.hpp"


GLuint vertexBufferObject;


std::vector <vertex> vertexBufferArray;

void initVBO(){
	glGenBuffers(1, &vertexBufferObject);
}

void registervertex(vertex input[], GLuint* arrayaddr, int length){

	for(int i = 0; i < length; i++){
		auto itr = std::find(vertexBufferArray.begin(), vertexBufferArray.end(), input[i]);
		if( itr != vertexBufferArray.end() ) {
			arrayaddr[i] = std::distance(vertexBufferArray.begin(), itr);
		}else{
			vertexBufferArray.push_back(input[i]);
			arrayaddr[i] = vertexBufferArray.size() -1;
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, vertexBufferArray.size() * sizeof(vertex), &vertexBufferArray[0], GL_STATIC_DRAW);
}
