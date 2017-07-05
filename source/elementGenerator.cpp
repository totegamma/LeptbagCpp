#include "elementGenerator.hpp"


elementGenerator::elementGeneratorList = std::vector<elementGenerator>;

elementGenerator* generateElementGenerator(){
	return new elementGenerator();
}

elementGenerator::elementGenerator(){
	elementGeneratorList.push_back(this);
}

elementGenerator::~elementGenerator(){
}


void elementGenerator::addVertex(vertex &newvertex){
	objectData.push_back(newvertex);
}

void elementGenerator::registerToSystem(){
		registervertex(&objectData, &indexBufferArray);

		glGenBuffers(1, &indexBufferObject);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferArray.size() * sizeof(GLuint), &indexBufferArray[0], GL_STATIC_DRAW);


		glGenBuffers(1, &instanceMatrixBuffer);
}

shapePointerObject* elementGenerator::generate(){
	return new shapePointerObject();
}

shapePointerObject* elementGenerator::generate(vec3 &position, vec3 &scale, quat &rotation){
	return new shapePointerObject(this, false, NULL, position, scale, rotation);
}

shapePointerObject* elementGenerator::generate(vec3 &position, vec3 &scale, quat &rotation, float mass){

		std::vector<btVector3> convexHullShapePoints;

		for(auto elem: objectData){
			btVector3 co = btVector3(elem.positionX, elem.positionY, elem.positionZ);
			auto itr = std::find(convexHullShapePoints.begin(), convexHullShapePoints.end(), co);
			if( itr == convexHullShapePoints.end() ){
				glm::vec4 target = glm::scale(glm::mat4(1.0f), scale.toGlm()) * glm::vec4(co.x(), co.y(), co.z(), 1);

				convexHullShapePoints.push_back(
						//NOTE: bulletは物体に0.04のマージンを加えるので、その分だけ小さいオブジェクトを作成する。
							btVector3(target.x - (0.04*(-1*((signbit(target.x)*2)-1))), target.y - (0.04*(-1*((signbit(target.y)*2)-1))), target.z - (0.04*(-1*((signbit(target.z)*2)-1))))
						);
			}
		}

		btCollisionShape* shape = new btConvexHullShape( &convexHullShapePoints[0][0], convexHullShapePoints.size(), sizeof(btVector3));

		btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(rotation.toBullet(), position.toBullet()));
		btVector3 inertia(0, 0, 0);
		shape->calculateLocalInertia(mass, inertia);
		btRigidBody::btRigidBodyConstructionInfo bodyCI(mass, motionState, shape, inertia);
		btRigidBody* body = new btRigidBody(bodyCI);
		dynamicsWorld->addRigidBody(body);

		objects.push_back(new shapePointerObject(this, true, body, position, scale, rotation));


		return objects.back();


}

void elementGenerator::destroy(int id){
}

void elementGenerator::render(){

	glm::mat4 instanceMatrixArray[objects.size()];

	for(int i = 0; i < objects.size(); i++){
		instanceMatrixArray[i] = objects[i]->loadMatrix();
	}

	glBindBuffer(GL_ARRAY_BUFFER, instanceMatrixBuffer);
	glBufferData(GL_ARRAY_BUFFER, objects.size() * sizeof(glm::mat4), instanceMatrixArray, GL_DYNAMIC_DRAW);

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

	glDrawElementsInstanced(GL_TRIANGLES, objectData.size(), GL_UNSIGNED_INT, (void*)0, objects.size());

}
