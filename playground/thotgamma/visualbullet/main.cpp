#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <btBulletDynamicsCommon.h>

#include "shader.hpp"

GLFWwindow* window;

const GLint num_of_cube = 1;


GLint windowWidth  = 1000;                    // Width of our window
GLint windowHeight = 800;                    // Heightof our window


GLint midWindowX = windowWidth  / 2;         // Middle of the window horizontally
GLint midWindowY = windowHeight / 2;         // Middle of the window vertically

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

// Initial position : on +Z
glm::vec3 position = glm::vec3( 2, 30, 2 ); 
// Initial horizontal angle : toward -Z
double horizontalAngle = 2.5f;
// Initial vertical angle : none
double verticalAngle = 1.0f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 0.1f; // 3 units / second
float mouseSpeed = 0.001f;


// Hoding any keys down?
bool holdingForward     = false;
bool holdingBackward    = false;
bool holdingLeftStrafe  = false;
bool holdingRightStrafe = false;


GLuint MatrixID;

void computeMatricesFromInputs(){


	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
			cos(verticalAngle) * sin(horizontalAngle), 
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
			);

	// Right vector
	glm::vec3 right = glm::vec3(
			sin(horizontalAngle - 3.14f/2.0f), 
			0,
			cos(horizontalAngle - 3.14f/2.0f)
			);

	glm::vec3 up = glm::cross( right, direction );

	// Move forward
	if (holdingForward == true){
		position += direction * speed;
	}
	// Move backward
	if (holdingBackward == true){
		position -= direction * speed;
	}
	// Strafe right
	if (holdingRightStrafe == true){
		position += right * speed;
	}
	// Strafe left
	if (holdingLeftStrafe == true){
		position -= right * speed;
	}

	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix       = glm::lookAt(
			position,           // Camera is here
			position+direction, // and looks here : at the same position, plus "direction"
			up                  // Head is up (set to 0,-1,0 to look upside-down)
			);

}

void handleMouseMove(GLFWwindow* window, double xpos, double ypos){
	horizontalAngle += mouseSpeed * float(midWindowX - xpos );
	verticalAngle   += mouseSpeed * float(midWindowY - ypos );
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

			default:
				break;
		}

	}
}




GLuint cube_sequencialbuffer;
GLuint elementbuffer;
GLuint positionbuffer;


GLfloat cube_vertex_buffer_data[8*3] = { 
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f,  0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f
};

GLfloat cube_color_buffer_data[8*3];

GLfloat cube_sequencial_array[8*3*2];

GLuint cube_index_buffer_object[14] = {
	//5, 1, 7, 3, 2, 1, 0, 5, 4, 7, 6, 2, 4, 0
	1, 5, 3, 7, 6, 5, 4, 1, 0, 3, 2, 6, 0, 4
};

GLfloat g_position_instanced_array[num_of_cube*3];


void init_cube_shape(){

	for(int i = 0; i < 8*3; i++){
		cube_color_buffer_data[i] = (cube_vertex_buffer_data[i] == 0.5f) ? 0.0f : 1.0f;
	}

	for(int i = 0; i < 8; i ++){
		cube_sequencial_array[(i*6)+0] = cube_vertex_buffer_data[(i*3)+0];
		cube_sequencial_array[(i*6)+1] = cube_vertex_buffer_data[(i*3)+1];
		cube_sequencial_array[(i*6)+2] = cube_vertex_buffer_data[(i*3)+2];
		cube_sequencial_array[(i*6)+3] = cube_color_buffer_data[(i*3)+0];
		cube_sequencial_array[(i*6)+4] = cube_color_buffer_data[(i*3)+1];
		cube_sequencial_array[(i*6)+5] = cube_color_buffer_data[(i*3)+2];
	}




	glGenBuffers(1, &cube_sequencialbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cube_sequencialbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_sequencial_array), cube_sequencial_array, GL_STATIC_DRAW);

	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_index_buffer_object), cube_index_buffer_object, GL_STATIC_DRAW);

	glGenBuffers(1, &positionbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, positionbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_position_instanced_array), g_position_instanced_array, GL_DYNAMIC_DRAW);
}

void setcubeposition(GLint id, GLfloat X, GLfloat Y, GLfloat Z){
	g_position_instanced_array[(id*3)+0] = X;
	g_position_instanced_array[(id*3)+1] = Y;
	g_position_instanced_array[(id*3)+2] = Z;

	glBindBuffer(GL_ARRAY_BUFFER, positionbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_position_instanced_array), g_position_instanced_array, GL_DYNAMIC_DRAW);
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



	init_cube_shape();



















	btBroadphaseInterface* broadphase = new btDbvtBroadphase();

	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, -10, 0));


	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);

	btCollisionShape* fallShape = new btBoxShape(btVector3(2, 2, 2));


	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	btRigidBody::btRigidBodyConstructionInfo
		groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
	dynamicsWorld->addRigidBody(groundRigidBody);


	btDefaultMotionState* fallMotionState =
		new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));
	btScalar mass = 1;
	btVector3 fallInertia(0, 0, 0);
	fallShape->calculateLocalInertia(mass, fallInertia);
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
	btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI);
	dynamicsWorld->addRigidBody(fallRigidBody);





















	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	GLfloat cubex, cubey, cubez = 0;

	while (glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == GL_FALSE){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		computeMatricesFromInputs();

		// Use our shader
		glUseProgram(programID);


		glBindBuffer(GL_ARRAY_BUFFER, cube_sequencialbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*3*2, (void*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*3*2, (void*)(sizeof(GLfloat)*3));

		glBindBuffer(GL_ARRAY_BUFFER, positionbuffer);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glVertexAttribDivisor(2, 1);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);


		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);


		glDrawElementsInstanced(GL_TRIANGLE_STRIP, 14, GL_UNSIGNED_INT, (void*)0, num_of_cube);





		dynamicsWorld->stepSimulation(1 / 60.f, 10);

		btTransform trans;
		fallRigidBody->getMotionState()->getWorldTransform(trans);


		setcubeposition(0, trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());





		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);













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
