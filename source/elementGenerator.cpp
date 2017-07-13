#include "elementGenerator.hpp"


std::vector<elementGenerator*> elementGenerator::elementGeneratorList;

elementGenerator* generateElementGenerator(){
	return new elementGenerator();
}

elementGenerator::elementGenerator(){
	elementGeneratorList.push_back(this);
}

elementGenerator::~elementGenerator(){
}


void elementGenerator::addVertex(vertex &newvertex){
	elementData.push_back(newvertex);
}

void elementGenerator::registerToSystem(){
		registervertex(&elementData, &indexBufferArray);

		glGenBuffers(1, &indexBufferObject);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferArray.size() * sizeof(GLuint), &indexBufferArray[0], GL_STATIC_DRAW);


		glGenBuffers(1, &instanceMatrixBuffer);
}

elementNode* elementGenerator::generate(){
	return new elementNode();
}

elementNode* elementGenerator::generate(vec3 &position, vec3 &scale, quat &rotation){
	//return new elementNode(this, false, NULL, position, scale, rotation);
	return nullptr;
}

elementNode* elementGenerator::generate(vec3 &position, vec3 &scale, quat &rotation, float mass){
	return nullptr;

}

void elementGenerator::destroy(int id){
}

void elementGenerator::render(){

	glm::mat4 instanceMatrixArray[elements.size()];

	for(int i = 0; i < elements.size(); i++){
		instanceMatrixArray[i] = elements[i]->loadMatrix();
	}

	glBindBuffer(GL_ARRAY_BUFFER, instanceMatrixBuffer);
	glBufferData(GL_ARRAY_BUFFER, elements.size() * sizeof(glm::mat4), instanceMatrixArray, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, instanceMatrixBuffer);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4)*0));
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4)*1));
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4)*2));
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4)*3));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);

	glDrawElementsInstanced(GL_TRIANGLES, elementData.size(), GL_UNSIGNED_INT, (void*)0, elements.size());

}
