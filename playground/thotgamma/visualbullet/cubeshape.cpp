#include "cubeshape.hpp"

class cubeshapeObject{
	int id;
	btRigidBody* body;
	glm::vec3 size;

	public:
	cubeshapeObject(int id, btRigidBody* body, glm::vec3 size){
		this->id = id;
		this->body = body;
		this->size = size;
	}

	void loadMotionState(){

		btTransform transform;
		body->getMotionState()->getWorldTransform(transform);

		btVector3 pos = transform.getOrigin();
		btQuaternion quaternion = transform.getRotation();

		cubeshape::instanceMatrixArray[id] = glm::translate(glm::mat4(1.0f), glm::vec3(pos.getX(), pos.getY(), pos.getZ())) 
			* glm::toMat4(glm::quat(quaternion.getW(), quaternion.getX(), quaternion.getY(), quaternion.getZ()))
			* glm::scale(glm::mat4(1.0f), size);

	}

};


namespace cubeshape{

	GLuint indexBufferObject;
	GLuint instanceMatrixBuffer;
	GLuint indexBufferArray[14];

	int numOfObject = 0;;

	vertex objectData[14] = {
		vertex(-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f),
		vertex( 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f),
		vertex(-0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f),
		vertex( 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f),
		vertex( 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f),
		vertex( 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f),
		vertex( 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f),
		vertex(-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f),
		vertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f),
		vertex(-0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f),
		vertex(-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f),
		vertex( 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f),
		vertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f),
		vertex( 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f)
	};

	std::vector<glm::mat4> instanceMatrixArray;

	std::vector<cubeshapeObject*> objects;


	void init(){

		registervertex(objectData, indexBufferArray, 14);

		glGenBuffers(1, &indexBufferObject);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexBufferArray), indexBufferArray, GL_STATIC_DRAW);


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


		btCollisionShape* shape = new btBoxShape(btVector3(size.x/2, size.y/2, size.z/2));

		btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(quat.x, quat.y, quat.z, quat.w), btVector3(position.x, position.y, position.z)));
		btVector3 inertia(0, 0, 0);
		shape->calculateLocalInertia(mass, inertia);
		btRigidBody::btRigidBodyConstructionInfo bodyCI(mass, motionState, shape, inertia);
		btRigidBody* body = new btRigidBody(bodyCI);
		dynamicsWorld->addRigidBody(body);

		objects.push_back(new cubeshapeObject(numOfObject++, body, size));

		return objects.back();
	}

	void destroy(int id){
	}

	void render(){

		for (auto elem: objects){
			elem->loadMotionState();
		}


		glBindBuffer(GL_ARRAY_BUFFER, instanceMatrixBuffer);
		glBufferData(GL_ARRAY_BUFFER, instanceMatrixArray.size() * sizeof(glm::mat4), &instanceMatrixArray[0], GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, instanceMatrixBuffer);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4)*0));
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4)*1));
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4)*2));
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4)*3));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glVertexAttribDivisor(2, 1);
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);

		glDrawElementsInstanced(GL_TRIANGLE_STRIP, 14, GL_UNSIGNED_INT, (void*)0, numOfObject);
	}


}
