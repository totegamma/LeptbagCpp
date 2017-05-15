#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <btBulletDynamicsCommon.h>


#include "vertexmanage.hpp"
#include "shader.hpp"
#include "cubeshape.hpp"
#include "floorshape.hpp"

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



glm::quat createq(double RotationAngle, double RotationAxisX, double RotationAxisY, double RotationAxisZ){
	double x = RotationAxisX * sin(RotationAngle / 2);
	double y = RotationAxisY * sin(RotationAngle / 2);
	double z = RotationAxisZ * sin(RotationAngle / 2);
	double w = cos(RotationAngle / 2);
	return glm::quat(w, x, y, z);
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
	glEnable( GL_CULL_FACE );


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


	btBroadphaseInterface* broadphase = new btDbvtBroadphase();

	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, -10, 0));

	initVBO();


	cubeshape::init();
	floorshape::init();

	/*
	int count = 10;
	for(int z = 0; z < count; z++){
		for(int y = 0; y < count*2; y++){
			for(int x = 0; x < count; x++){
				cubeshape::create(glm::vec3(1*x, 1*y+30, 1*z), glm::vec3(0.5, 0.5, 0.5), glm::quat(1, 0, 0, 0), 1, dynamicsWorld);
			}
		}
	}
	*/

	float dogHeight = 10;

	cubeshapeObject* body			= cubeshape::create(glm::vec3(0, dogHeight, 0),		glm::vec3(2, 1, 1),			glm::quat(1, 0, 0, 0), 2, dynamicsWorld);
	cubeshapeObject* head			= cubeshape::create(glm::vec3(1.4, dogHeight, 0),		glm::vec3(0.8, 0.8, 0.8),	glm::quat(1, 0, 0, 0), 0.6, dynamicsWorld);
	cubeshapeObject* muzzle			= cubeshape::create(glm::vec3(2.1, dogHeight -0.2, 0),		glm::vec3(0.6, 0.4, 0.4),	glm::quat(1, 0, 0, 0), 0.2, dynamicsWorld);
	cubeshapeObject* earLeft		= cubeshape::create(glm::vec3(1.4, dogHeight + 0.5, -0.2),	glm::vec3(0.2, 0.2, 0.2),	glm::quat(1, 0, 0, 0), 0.1, dynamicsWorld);
	cubeshapeObject* earRight		= cubeshape::create(glm::vec3(1.4, dogHeight + 0.5, 0.2),	glm::vec3(0.2, 0.2, 0.2),	glm::quat(1, 0, 0, 0), 0.1, dynamicsWorld);
	cubeshapeObject* legFrontLeft	= cubeshape::create(glm::vec3(0.5, dogHeight - 1, -0.4),	glm::vec3(0.2, 1, 0.2),		glm::quat(1, 0, 0, 0), 0.3, dynamicsWorld);
	cubeshapeObject* legFrontRight	= cubeshape::create(glm::vec3(0.5, dogHeight -1, 0.4),		glm::vec3(0.2, 1, 0.2),		glm::quat(1, 0, 0, 0), 0.3, dynamicsWorld);
	cubeshapeObject* legBackLeft	= cubeshape::create(glm::vec3(-0.5, dogHeight -1, -0.4),	glm::vec3(0.2, 1, 0.2),		glm::quat(1, 0, 0, 0), 0.3, dynamicsWorld);
	cubeshapeObject* legBackRight	= cubeshape::create(glm::vec3(-0.5, dogHeight -1, 0.4),	glm::vec3(0.2, 1, 0.2),		glm::quat(1, 0, 0, 0), 0.3, dynamicsWorld);
	cubeshapeObject* tail			= cubeshape::create(glm::vec3(-1.5, dogHeight + 0.4, 0),	glm::vec3(1, 0.2, 0.2),		glm::quat(1, 0, 0, 0), 0.2, dynamicsWorld);

	btHingeConstraint* hinge_body_head = new btHingeConstraint(*(body->body), *(head->body), btVector3(1, 0, 0), btVector3(-0.4, 0, 0), btVector3(0, 0, 1), btVector3(0, 0, 1));
	hinge_body_head->setLimit(-3.14/6, 3.14/6);
	dynamicsWorld->addConstraint(hinge_body_head, true);

	btHingeConstraint* hinge_head_muzzle = new btHingeConstraint(*(head->body), *(muzzle->body), btVector3(0.4, -0.2, 0), btVector3(-0.3, 0, 0), btVector3(1, 0, 0), btVector3(1, 0, 0));
	hinge_head_muzzle->setLimit(0, 0);
	dynamicsWorld->addConstraint(hinge_head_muzzle, true);

	btHingeConstraint* hinge_earLeft_head = new btHingeConstraint(*(earLeft->body), *(head->body), btVector3(0, -0.1, 0), btVector3(0, 0.4, -0.2), btVector3(1, 0, 0), btVector3(1, 0, 0));
	hinge_earLeft_head->setLimit(0, 0);
	dynamicsWorld->addConstraint(hinge_earLeft_head, true);

	btHingeConstraint* hinge_earRight_head = new btHingeConstraint(*(earRight->body), *(head->body), btVector3(0, -0.1, 0), btVector3(0, 0.4, 0.2), btVector3(1, 0, 0), btVector3(1, 0, 0));
	hinge_earRight_head->setLimit(0, 0);
	dynamicsWorld->addConstraint(hinge_earRight_head, true);


	btHingeConstraint* hinge_body_legFrontLeft = new btHingeConstraint(*(body->body), *(legFrontLeft->body), btVector3(0.5, -0.5, -0.4), btVector3(0, 0.5, 0.0), btVector3(0, 0, 1), btVector3(0, 0, 1));
	hinge_body_legFrontLeft->setLimit(-3.14/2, 3.14/2);
	dynamicsWorld->addConstraint(hinge_body_legFrontLeft, true);

	btHingeConstraint* hinge_body_legFrontRight = new btHingeConstraint(*(body->body), *(legFrontRight->body), btVector3(0.5, -0.5, 0.4), btVector3(0, 0.5, 0.0), btVector3(0, 0, 1), btVector3(0, 0, 1));
	hinge_body_legFrontRight->setLimit(-3.14/2, 3.14/2);
	dynamicsWorld->addConstraint(hinge_body_legFrontRight, true);

	btHingeConstraint* hinge_body_legBackLeft = new btHingeConstraint(*(body->body), *(legBackLeft->body), btVector3(-0.5, -0.5, -0.4), btVector3(0, 0.5, 0.0), btVector3(0, 0, 1), btVector3(0, 0, 1));
	hinge_body_legBackLeft->setLimit(-3.14/2, 3.14/2);
	dynamicsWorld->addConstraint(hinge_body_legBackLeft, true);

	btHingeConstraint* hinge_body_legBackRight = new btHingeConstraint(*(body->body), *(legBackRight->body), btVector3(-0.5, -0.5, 0.4), btVector3(0, 0.5, 0.0), btVector3(0, 0, 1), btVector3(0, 0, 1));
	hinge_body_legBackRight->setLimit(-3.14/2, 3.14/2);
	dynamicsWorld->addConstraint(hinge_body_legBackRight, true);

	btHingeConstraint* hinge_body_tail = new btHingeConstraint(*(body->body), *(tail->body), btVector3(-1, 0.4, 0), btVector3(0.5, 0, 0.0), btVector3(0, 0, 1), btVector3(0, 0, 1));
	hinge_body_tail->setLimit(-3.14/3, 3.14/3);
	dynamicsWorld->addConstraint(hinge_body_tail, true);


	/*
	cubeshapeObject* cubeA = cubeshape::create(glm::vec3(0, 15, 0), glm::vec3(1, 1, 1), glm::quat(1, 0, 0, 0), 1, dynamicsWorld);
	cubeshapeObject* cubeB = cubeshape::create(glm::vec3(1, 14, 1), glm::vec3(1, 1, 1), glm::quat(1, 0, 0, 0), 1, dynamicsWorld);
	cubeshapeObject* cubeC = cubeshape::create(glm::vec3(2, 13, 2), glm::vec3(1, 1, 1), glm::quat(1, 0, 0, 0), 1, dynamicsWorld);
	cubeshapeObject* cubeD = cubeshape::create(glm::vec3(3, 12, 3), glm::vec3(1, 1, 1), glm::quat(1, 0, 0, 0), 1, dynamicsWorld);
	cubeshapeObject* cubeE = cubeshape::create(glm::vec3(4, 11, 4), glm::vec3(1, 1, 1), glm::quat(1, 0, 0, 0), 1, dynamicsWorld);
	cubeshapeObject* cubeF = cubeshape::create(glm::vec3(5, 10, 5), glm::vec3(1, 1, 1), glm::quat(1, 0, 0, 0), 1, dynamicsWorld);



	btTypedConstraint *joint_A = new btPoint2PointConstraint(*(cubeA->body), btVector3(0.5, 0.5, 0.5));
	btTypedConstraint *jointAB = new btPoint2PointConstraint(*(cubeA->body), *(cubeB->body), btVector3(-0.5, -0.5, -0.5), btVector3(0.5, 0.5, 0.5));
	btTypedConstraint *jointBC = new btPoint2PointConstraint(*(cubeB->body), *(cubeC->body), btVector3(-0.5, -0.5, -0.5), btVector3(0.5, 0.5, 0.5));
	btTypedConstraint *jointCD = new btPoint2PointConstraint(*(cubeC->body), *(cubeD->body), btVector3(-0.5, -0.5, -0.5), btVector3(0.5, 0.5, 0.5));
	btTypedConstraint *jointDE = new btPoint2PointConstraint(*(cubeD->body), *(cubeE->body), btVector3(-0.5, -0.5, -0.5), btVector3(0.5, 0.5, 0.5));
	btTypedConstraint *jointEF = new btPoint2PointConstraint(*(cubeE->body), *(cubeF->body), btVector3(-0.5, -0.5, -0.5), btVector3(0.5, 0.5, 0.5));

	dynamicsWorld->addConstraint(joint_A);
	dynamicsWorld->addConstraint(jointAB);
	dynamicsWorld->addConstraint(jointBC);
	dynamicsWorld->addConstraint(jointCD);
	dynamicsWorld->addConstraint(jointDE);
	dynamicsWorld->addConstraint(jointEF);
	*/



	floorshape::create(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), glm::quat(1, 0, 0, 0), dynamicsWorld);

	/*
	floorshape::create(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), createq(3.14/4, 1, 0, 0), dynamicsWorld);
	floorshape::create(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), createq(-3.14/4, 1, 0, 0), dynamicsWorld);
	*/


	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);

	while (glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == GL_FALSE){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		computeMatricesFromInputs();



		if(DOstepsim == true) dynamicsWorld->stepSimulation(1 / 60.f, 10);



		// Use our shader
		glUseProgram(programID);

		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(sizeof(GLfloat)*3));



		cubeshape::render();
		floorshape::render();




		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
	glDisableVertexAttribArray(5);





	/*

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
	*/












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
