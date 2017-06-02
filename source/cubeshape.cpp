#include "cubeshape.hpp"


cubeshapeObject::cubeshapeObject(int id, btRigidBody* body, glm::vec3 size, btDiscreteDynamicsWorld *dynamicsWorld){
	this->id = id;
	this->body = body;
	this->size = size;
	this->dynamicsWorld = dynamicsWorld;
}


void cubeshapeObject::loadMotionState(){

	btTransform transform;
	body->getMotionState()->getWorldTransform(transform);

	btVector3 pos = transform.getOrigin();
	btQuaternion quaternion = transform.getRotation();

	cubeshape::instanceMatrixArray[id] = glm::translate(glm::mat4(1.0f), glm::vec3(pos.getX(), pos.getY(), pos.getZ())) 
		* glm::toMat4(glm::quat(quaternion.getW(), quaternion.getX(), quaternion.getY(), quaternion.getZ()))
		* glm::scale(glm::mat4(1.0f), size);

}

void cubeshapeObject::destroy(){
	cubeshape::destroy(id);

	dynamicsWorld->removeRigidBody(body);
	delete body->getMotionState();
	delete body;

}

float cubeshapeObject::getXpos(){
	btTransform transform;
	body->getMotionState()->getWorldTransform(transform);
	btVector3 pos = transform.getOrigin();
	return pos.getX();
}

float cubeshapeObject::getYpos(){
	btTransform transform;
	body->getMotionState()->getWorldTransform(transform);
	btVector3 pos = transform.getOrigin();
	return pos.getY();
}

float cubeshapeObject::getZpos(){
	btTransform transform;
	body->getMotionState()->getWorldTransform(transform);
	btVector3 pos = transform.getOrigin();
	return pos.getZ();
}


void cubeshapeObject::changeID(int newID){
	id = newID;
}


namespace cubeshape{

	GLuint indexBufferObject;
	GLuint instanceMatrixBuffer;
	std::vector<GLuint> indexBufferArray;

	int numOfObject = 0;

	std::vector<vertex> objectData = {

		vertex(-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f),
		vertex(-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f),
		vertex(-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f),

		vertex(-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f),
		vertex(-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f),
		vertex(-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f),


		vertex( 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  1.0f),
		vertex(-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f),
		vertex( 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f),

		vertex( 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  1.0f),
		vertex(-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f),
		vertex(-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f),


		vertex( 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,  0.0f),
		vertex(-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f),
		vertex(-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f),

		vertex( 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,  0.0f),
		vertex( 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,  0.0f),
		vertex(-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f),


		vertex( 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f),
		vertex( 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f),
		vertex( 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f),

		vertex( 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f),
		vertex( 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f),
		vertex( 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f),


		vertex( 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,  1.0f),
		vertex( 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,  0.0f),
		vertex(-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f),

		vertex( 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,  1.0f),
		vertex(-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f),
		vertex(-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f),


		vertex(-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f),
		vertex(-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f),
		vertex( 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  1.0f),

		vertex( 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,  1.0f),
		vertex(-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f),
		vertex( 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  1.0f)

	};

	std::vector<glm::mat4> instanceMatrixArray;
	std::vector<cubeshapeObject*> objects;


	void init(){

		registervertex(&objectData, &indexBufferArray);

		glGenBuffers(1, &indexBufferObject);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferArray.size() * sizeof(GLuint), &indexBufferArray[0], GL_STATIC_DRAW);


		glGenBuffers(1, &instanceMatrixBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, instanceMatrixBuffer);
		glBufferData(GL_ARRAY_BUFFER, instanceMatrixArray.size() * sizeof(glm::mat4), &instanceMatrixArray[0], GL_DYNAMIC_DRAW);
	}


	cubeshapeObject* create(glm::vec3 position, glm::vec3 size, glm::quat quat, btScalar mass, btDiscreteDynamicsWorld *dynamicsWorld){


		instanceMatrixArray.push_back(
				glm::translate(glm::mat4(1.0f), position) 
				* glm::toMat4(quat)
				* glm::scale(glm::mat4(1.0f), size)
				);


		btCollisionShape* shape = new btBoxShape(btVector3(size.x, size.y, size.z));

		btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(quat.x, quat.y, quat.z, quat.w), btVector3(position.x, position.y, position.z)));
		btVector3 inertia(0, 0, 0);
		shape->calculateLocalInertia(mass, inertia);
		btRigidBody::btRigidBodyConstructionInfo bodyCI(mass, motionState, shape, inertia);
		btRigidBody* body = new btRigidBody(bodyCI);
		dynamicsWorld->addRigidBody(body);

		objects.push_back(new cubeshapeObject(numOfObject, body, size, dynamicsWorld));


		numOfObject++;

		return objects.back();
	}

	void destroy(int id){
		objects[id] = objects.back();
		objects[id]->changeID(id);
		objects.pop_back();

		instanceMatrixArray[id] = instanceMatrixArray.back();
		instanceMatrixArray.pop_back();
		numOfObject--;
	}

	void render(){

		for (auto elem: objects){
			elem->loadMotionState();
		}


		glBindBuffer(GL_ARRAY_BUFFER, instanceMatrixBuffer);
		glBufferData(GL_ARRAY_BUFFER, instanceMatrixArray.size() * sizeof(glm::mat4), &instanceMatrixArray[0], GL_DYNAMIC_DRAW);

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

		glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0, numOfObject);
	}


}


cubeshapeObject* cubeshape_create(float x, float y, float z, float w, float h, float d, float qw, float qx, float qy, float qz, float g){
	return cubeshape::create(glm::vec3(x, y, z), glm::vec3(w, h, d), glm::quat(qw, qx, qy, qz), g, dynamicsWorld);
}
