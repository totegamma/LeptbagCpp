#include "floorshape.hpp"

void floorshape::init(){
	glGenBuffers(1, &floor_indexBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floor_indexBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floor_indexBufferArray), floor_indexBufferArray, GL_STATIC_DRAW);

	glGenBuffers(1, &floor_instanceMatrixBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, floor_instanceMatrixBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floor_instanceMatrixArray), floor_instanceMatrixArray, GL_STATIC_DRAW);


}

void cubeshape::create(){
	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);


	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
	dynamicsWorld->addRigidBody(groundRigidBody);

}

void floorshape::destroy(){
}

void floorshape::render(){
	glBindBuffer(GL_ARRAY_BUFFER, floor_instanceMatrixBuffer);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4)*0));
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4)*1));
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4)*2));
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4)*3));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);



	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floor_indexBufferObject);

	glDrawElementsInstanced(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (void*)0, 1);
}

/*
   float angle = 20.0f;
   floor_instanceMatrixArray[0] = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)) 
 * glm::toMat4(glm::quat(cos((3.14f/angle) / 2), (1) * sin((3.14f/angle) / 2), (0) * sin((3.14f/angle) / 2), (0) * sin((3.14f/angle) / 2)))
 * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f ,1.0f));
 */
