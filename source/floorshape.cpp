#include "floorshape.hpp"

class floorshapeObject{
	int id;
	btRigidBody* body;

	public:
	floorshapeObject(int id, btRigidBody* body){
		this->id = id;
		this->body = body;
	}
};

namespace floorshape{

	GLuint indexBufferObject;
	GLuint instanceMatrixBuffer;
	std::vector<GLuint> indexBufferArray;

	int numOfObject = 0;;

	std::vector<vertex> objectData = {
		vertex(-1000.0f,  0.0f, -1000.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f),
		vertex(-1000.0f,  0.0f,  1000.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f),
		vertex( 1000.0f,  0.0f, -1000.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f),
		vertex( 1000.0f,  0.0f,  1000.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f)
	};

	std::vector<glm::mat4> instanceMatrixArray;

	std::vector<floorshapeObject*> objects;

	void init(){

		registervertex(&objectData, &indexBufferArray);

		glGenBuffers(1, &indexBufferObject);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferArray.size() * sizeof(GLuint), &indexBufferArray[0], GL_STATIC_DRAW);


		glGenBuffers(1, &instanceMatrixBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, instanceMatrixBuffer);
		glBufferData(GL_ARRAY_BUFFER, instanceMatrixArray.size() * sizeof(glm::mat4), &instanceMatrixArray[0], GL_DYNAMIC_DRAW);


	}

	floorshapeObject* create(glm::vec3 position, glm::vec3 face, glm::quat quat, btDiscreteDynamicsWorld *dynamicsWorld){


		instanceMatrixArray.push_back(
				glm::translate(glm::mat4(1.0f), position) 
				* glm::toMat4(quat)
			);

		glBindBuffer(GL_ARRAY_BUFFER, instanceMatrixBuffer);
		glBufferData(GL_ARRAY_BUFFER, instanceMatrixArray.size() * sizeof(glm::mat4), &instanceMatrixArray[0], GL_DYNAMIC_DRAW);

		btCollisionShape* shape = new btStaticPlaneShape(btVector3(face.x, face.y, face.z), 0);

		btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(quat.x, quat.y, quat.z, quat.w), btVector3(position.x, position.y, position.z)));
		btRigidBody::btRigidBodyConstructionInfo bodyCI(0, motionState, shape, btVector3(0, 0, 0));
		btRigidBody* body = new btRigidBody(bodyCI);

		btScalar friction = btScalar(0.7);
        body->setFriction(friction);

		dynamicsWorld->addRigidBody(body);



		objects.push_back(new floorshapeObject(numOfObject++, body));

		return objects.back();

	}

	void destroy(){
	}

	void render(){
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

		glDrawElementsInstanced(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (void*)0, numOfObject);
	}

}
