#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <btBulletDynamicsCommon.h>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "shader.hpp"

GLFWwindow* window;


GLint windowWidth  = 1000;                    // Width of our window
GLint windowHeight = 800;                    // Heightof our window

const int numOfCube = 1;


GLint midWindowX = windowWidth  / 2;         // Middle of the window horizontally
GLint midWindowY = windowHeight / 2;         // Middle of the window vertically

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;


glm::vec3 position = glm::vec3( 0, 0, 0 ); 
double horizontalAngle = 3.14f;
double verticalAngle = 0.0f;


float initialFoV = 45.0f;

float speed = 0.1f; // 3 units / second
float mouseSpeed = 0.001f;


// Hoding any keys down?
bool holdingForward     = false;
bool holdingBackward    = false;
bool holdingLeftStrafe  = false;
bool holdingRightStrafe = false;

bool holdingSneek = false;
bool holdingSpace = false;

bool DOstepsim = false;


GLuint MatrixID;

void computeMatricesFromInputs(){


	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
			cos(verticalAngle) * sin(horizontalAngle), 
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
			);


	// Move forward
	if (holdingForward == true){
		position[0] += sin(horizontalAngle)* speed;
		position[2] += cos(horizontalAngle)* speed;
	}
	// Move backward
	if (holdingBackward == true){
		position[0] += sin(horizontalAngle+3.14)* speed;
		position[2] += cos(horizontalAngle+3.14)* speed;
	}

	if (holdingRightStrafe == true){
		position[0] += sin(horizontalAngle-(3.14/2))* speed;
		position[2] += cos(horizontalAngle-(3.14/2))* speed;
	}
	// Strafe left
	if (holdingLeftStrafe == true){
		position[0] += sin(horizontalAngle+(3.14/2)) * speed;
		position[2] += cos(horizontalAngle+(3.14/2)) * speed;
	}

	if (holdingSpace == true){
		position[1] += speed;
	}

	if (holdingSneek == true){
		position[1] -= speed;
	}


	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix       = glm::lookAt(
			position,           // Camera is here
			position+direction, // and looks here : at the same position, plus "direction"
			glm::vec3(0,1,0)                  // Head is up (set to 0,-1,0 to look upside-down)
			);

}

void handleMouseMove(GLFWwindow* window, double xpos, double ypos){

	if(horizontalAngle + mouseSpeed * float(midWindowX - xpos) > 3.14){
		horizontalAngle = (horizontalAngle + mouseSpeed * float(midWindowX - xpos)) - (3.14*2);
	}else if(horizontalAngle + mouseSpeed * float(midWindowX - xpos) < -3.14){
		horizontalAngle = (horizontalAngle + mouseSpeed * float(midWindowX - xpos)) + (3.14*2);
	}else{
		horizontalAngle += mouseSpeed * float(midWindowX - xpos );
	}

	if(verticalAngle + mouseSpeed * float(midWindowY - ypos ) > 3.14/2){
		verticalAngle = 3.14/2;
	}else if(verticalAngle + mouseSpeed * float(midWindowY - ypos ) < -3.14/2){
		verticalAngle = -3.14/2;
	}else{
		verticalAngle   += mouseSpeed * float(midWindowY - ypos );
	}

	glfwSetCursorPos(window, midWindowX, midWindowY);
}

void handleKeypress(GLFWwindow* window, int key, int scancode, int action, int mods){
	// If a key is pressed, toggle the relevant key-press flag
	if (action == GLFW_PRESS){
		switch(key) {
			case 'W':
				holdingForward = true;
				break;

			case 'S':
				holdingBackward = true;
				break;

			case 'A':
				holdingLeftStrafe = true;
				break;

			case 'D':
				holdingRightStrafe = true;
				break;

			case 'P':
				DOstepsim = true;
				break;

			case GLFW_KEY_LEFT_SHIFT:
				holdingSneek = true;
				break;

			case GLFW_KEY_SPACE:
				holdingSpace = true;
				break;

			default:
				break;
		}
	}else if(action == GLFW_RELEASE){ // If a key is released, toggle the relevant key-release flag
		switch(key) {
			case 'W':
				holdingForward = false;
				break;

			case 'S':
				holdingBackward = false;
				break;

			case 'A':
				holdingLeftStrafe = false;
				break;

			case 'D':
				holdingRightStrafe = false;
				break;

			case 'P':
				DOstepsim = false;
				break;

			case GLFW_KEY_LEFT_SHIFT :
				holdingSneek = false;
				break;

			case GLFW_KEY_SPACE:
				holdingSpace = false;
				break;

			default:
				break;
		}

	}
}




GLuint vertexBufferObject;

GLuint cube_indexBufferObject;
GLuint cube_instanceMatrixBuffer;

GLuint floor_indexBufferObject;
GLuint floor_instanceMatrixBuffer;

struct vertex{
	GLfloat positionX;
	GLfloat positionY;
	GLfloat positionZ;
	GLfloat colorR;
	GLfloat colorG;
	GLfloat colorB;

	vertex(GLfloat positionX, GLfloat positionY, GLfloat positionZ, GLfloat colorR, GLfloat colorG, GLfloat colorB){
		this->positionX = positionX;
		this->positionY = positionY;
		this->positionZ = positionZ;
		this->colorR = colorR;
		this->colorG = colorG;
		this->colorB = colorB;
	}
};

vertex vertexBufferArray[] = {
	vertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f),
	vertex(-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f),
	vertex(-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f),
	vertex(-0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f),
	vertex( 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f),
	vertex( 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f),
	vertex( 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f),
	vertex( 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f),

	vertex(-100.0f, 0.0f, -100.0f, 0.0f, 0.0f, 1.0f),
	vertex(-100.0f, 0.0f,  100.0f, 0.0f, 1.0f, 0.0f),
	vertex( 100.0f, 0.0f, -100.0f, 1.0f, 0.0f, 0.0f),
	vertex( 100.0f, 0.0f,  100.0f, 1.0f, 1.0f, 1.0f)
};



GLuint cube_indexBufferArray[14] = {
	//5, 1, 7, 3, 2, 1, 0, 5, 4, 7, 6, 2, 4, 0
	1, 5, 3, 7, 6, 5, 4, 1, 0, 3, 2, 6, 0, 4
};

GLuint floor_indexBufferArray[4] = {
	8, 9, 10, 11
};


glm::mat4 cube_instanceMatrixArray[numOfCube];
glm::mat4 floor_instanceMatrixArray[1];


void init_cube_shape(){


	float angle = 20.0f;
	floor_instanceMatrixArray[0] = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)) 
									* glm::toMat4(glm::quat(cos((3.14f/angle) / 2), (1) * sin((3.14f/angle) / 2), (0) * sin((3.14f/angle) / 2), (0) * sin((3.14f/angle) / 2)))
									* glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f ,1.0f));



	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferArray), vertexBufferArray, GL_STATIC_DRAW);

	glGenBuffers(1, &cube_indexBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_indexBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indexBufferArray), cube_indexBufferArray, GL_STATIC_DRAW);

	glGenBuffers(1, &cube_instanceMatrixBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cube_instanceMatrixBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_instanceMatrixArray), cube_instanceMatrixArray, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &floor_indexBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floor_indexBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floor_indexBufferArray), floor_indexBufferArray, GL_STATIC_DRAW);

	glGenBuffers(1, &floor_instanceMatrixBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, floor_instanceMatrixBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floor_instanceMatrixArray), floor_instanceMatrixArray, GL_STATIC_DRAW);




}




void setCubeState(float posx, float posy, float posz, float rotw, float rotx, float roty, float rotz){
	cube_instanceMatrixArray[0] = glm::translate(glm::mat4(1.0f), glm::vec3(posx, posy, posz)) 
									* glm::toMat4(glm::quat(rotw, rotx, roty, rotz))
									* glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f ,1.0f));

	glBindBuffer(GL_ARRAY_BUFFER, cube_instanceMatrixBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_instanceMatrixArray), cube_instanceMatrixArray, GL_DYNAMIC_DRAW);

}



btQuaternion createq(double RotationAngle, double RotationAxisX, double RotationAxisY, double RotationAxisZ){
	double x = RotationAxisX * sin(RotationAngle / 2);
	double y = RotationAxisY * sin(RotationAngle / 2);
	double z = RotationAxisZ * sin(RotationAngle / 2);
	double w = cos(RotationAngle / 2);
	return btQuaternion(x, y, z, w);
}




int main(){
	if (!glfwInit()){
		// Initialization failed
	}


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(windowWidth, windowHeight, "My Title", NULL, NULL);
	if (!window){
		// Window or OpenGL context creation failed
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if(glewInit () != GLEW_OK){
		//失敗😩
	}

	glClearColor(0.5f, 0.5f, 1.0f, 0.0f);


	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	//glEnable( GL_CULL_FACE );


	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);


	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader" );

	// Get a handle for our "MVP" uniform
	MatrixID = glGetUniformLocation(programID, "MVP");

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);



	glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
	glfwSetKeyCallback(window, handleKeypress);
	glfwSetCursorPosCallback(window, handleMouseMove);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);




	init_cube_shape();




	btBroadphaseInterface* broadphase = new btDbvtBroadphase();

	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, -10, 0));


	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);

	btCollisionShape* fallShape = new btBoxShape(btVector3(0.5, 0.5, 0.5));


//	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(createq(3.14f/20.0f, 1, 0, 0), btVector3(0, 0, 0)));
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
	dynamicsWorld->addRigidBody(groundRigidBody);


	btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));
	btScalar mass = 1;
	btVector3 fallInertia(0, 0, 0);
	fallShape->calculateLocalInertia(mass, fallInertia);
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
	btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI);
	dynamicsWorld->addRigidBody(fallRigidBody);










	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);

	while (glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == GL_FALSE){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		computeMatricesFromInputs();



		if(DOstepsim == true)
		dynamicsWorld->stepSimulation(1 / 60.f, 10);

		btTransform transform;
		fallRigidBody->getMotionState()->getWorldTransform(transform);

		btVector3 pos = transform.getOrigin();
		btQuaternion quaternion = transform.getRotation();

		setCubeState(	pos.getX(),
						pos.getY(),
						pos.getZ(),
						quaternion.getW(),
						quaternion.getX(),
						quaternion.getY(),
						quaternion.getZ()
						);



		// Use our shader
		glUseProgram(programID);

		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*3*2, (void*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*3*2, (void*)(sizeof(GLfloat)*3));

		glBindBuffer(GL_ARRAY_BUFFER, cube_instanceMatrixBuffer);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4)*0));
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4)*1));
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4)*2));
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4)*3));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glVertexAttribDivisor(2, 1);
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_indexBufferObject);


		glDrawElementsInstanced(GL_TRIANGLE_STRIP, 14, GL_UNSIGNED_INT, (void*)0, numOfCube);


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


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
	glDisableVertexAttribArray(5);






	dynamicsWorld->removeRigidBody(fallRigidBody);
	delete fallRigidBody->getMotionState();
	delete fallRigidBody;

	dynamicsWorld->removeRigidBody(groundRigidBody);
	delete groundRigidBody->getMotionState();
	delete groundRigidBody;


	delete fallShape;

	delete groundShape;


	delete dynamicsWorld;
	delete solver;
	delete collisionConfiguration;
	delete dispatcher;
	delete broadphase;












	// Cleanup VBO
	/*
	   glDeleteBuffers(1, &cube_vertexbuffer);
	   glDeleteBuffers(1, &cube_colorbuffer);
	   */
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();


	return 0;
}
