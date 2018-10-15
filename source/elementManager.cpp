#include "elementManager.hpp"


std::vector<elementManager*> elementManager::elementManagerList;


elementManager::elementManager(std::vector<vertex> elementData, btRigidBody* (*bodyGenerator)(std::unique_ptr<parameterPack>))
	: elementData(elementData), bodyGenerator(bodyGenerator) {


	registervertex(elementData, &indexBufferArray);

	glGenBuffers(1, &indexBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferArray.size() * sizeof(GLuint), &indexBufferArray[0], GL_STATIC_DRAW);


	glGenBuffers(1, &instanceMatrixBuffer);


	elementManagerList.push_back(this);

}

elementManager::~elementManager(){
	while(elements.empty() == false){
		delete elements.back();
		elements.pop_back();
	}

}

void elementManager::destroySelf(){
	delete this;
}



elementNode* elementManager::generate(parameterPack* raw_input){

	auto input = std::unique_ptr<parameterPack>(raw_input);

	Eigen::Vector3f position = input->search("position").getVec3();
	Eigen::Vector3f scale    = input->search("scale").getVec3();
	Eigen::Quaternionf rotation = input->search("rotation").getQuat();

	instanceMatrixArray.push_back(
					glm::translate(glm::mat4(1.0f), toGlmVec3(position))
					* glm::toMat4(toGlmQuat(rotation))
					* glm::scale(glm::mat4(1.0f), toGlmVec3(scale))
					);

	input->add(param("caller", this));

	btRigidBody *newbody = bodyGenerator(std::move(input));
	elementNode *newNode = new elementNode(elements.size(), this, newbody, position, scale, rotation);
	elements.push_back(newNode);

	return newNode;
}


void elementManager::destroyElement(int id){
	if (id+1 == elements.size()) {
		delete elements.at(id);
		elements.pop_back();
		instanceMatrixArray.pop_back();
	} else {
		delete elements.at(id);
		elements[id] = elements.back();
		elements[id]->changeID(id);
		elements.pop_back();

		instanceMatrixArray[id] = instanceMatrixArray.back();
		instanceMatrixArray.pop_back();
	}
}


void elementManager::render(){

	for(auto elem: elements){
		elem->loadMatrix(&instanceMatrixArray);
	}

	glBindBuffer(GL_ARRAY_BUFFER, instanceMatrixBuffer);
	glBufferData(GL_ARRAY_BUFFER, elements.size() * sizeof(glm::mat4), &instanceMatrixArray[0], GL_DYNAMIC_DRAW);

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

std::vector<vertex>* elementManager::getElementDataPtr(){
	return &elementData;

}
