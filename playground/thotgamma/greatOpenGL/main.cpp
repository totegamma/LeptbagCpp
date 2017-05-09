#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "shader.hpp"

GLFWwindow* window;


GLint windowWidth  = 1000;                    // Width of our window
GLint windowHeight = 800;                    // Heightof our window


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




GLuint cube_complexBuffer;
GLuint elementBuffer;
GLuint cube_instanceMatrixBuffer;


GLfloat cube_vertexPositionArray[8*3] = { 
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f,  0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f
};

GLfloat cube_vertexColorArray[8*3] = {
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f
};

GLfloat cube_complexArray[8*3*2];

GLuint cube_indexBufferObject[14] = {
	//5, 1, 7, 3, 2, 1, 0, 5, 4, 7, 6, 2, 4, 0
	1, 5, 3, 7, 6, 5, 4, 1, 0, 3, 2, 6, 0, 4
};


glm::mat4 cube_instanceMatrixArray[8];


void init_cube_shape(){


	for(int i = 0; i < 8; i ++){
		cube_complexArray[(i*6)+0] = cube_vertexPositionArray[(i*3)+0];
		cube_complexArray[(i*6)+1] = cube_vertexPositionArray[(i*3)+1];
		cube_complexArray[(i*6)+2] = cube_vertexPositionArray[(i*3)+2];
		cube_complexArray[(i*6)+3] = cube_vertexColorArray[(i*3)+0];
		cube_complexArray[(i*6)+4] = cube_vertexColorArray[(i*3)+1];
		cube_complexArray[(i*6)+5] = cube_vertexColorArray[(i*3)+2];
	}

	cube_instanceMatrixArray[0] = glm::rotate(glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 0.0f, 0.0f)) , glm::vec3(1.0f, 1.0f ,1.0f)), 3.0f, glm::vec3(1.0f, 1.0f ,1.0f));
	cube_instanceMatrixArray[1] = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 5.0f)) 
									* glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 3.0f ,2.0f));
	cube_instanceMatrixArray[2] = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 5.0f, 0.0f)) 
									* glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f ,1.0f));
	cube_instanceMatrixArray[3] = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 5.0f, 5.0f)) 
									* glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f ,2.0f));
	cube_instanceMatrixArray[4] = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 0.0f, 0.0f)) 
									* glm::scale(glm::mat4(1.0f), glm::vec3(3.0f, 1.0f ,1.0f));
	cube_instanceMatrixArray[5] = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 0.0f, 5.0f)) 
									* glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 1.0f ,2.0f));
	cube_instanceMatrixArray[6] = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 5.0f, 0.0f)) 
									* glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 3.0f ,1.0f));
	cube_instanceMatrixArray[7] = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 5.0f, 5.0f)) 
									* glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f ,5.0f));


	glGenBuffers(1, &cube_complexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cube_complexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_complexArray), cube_complexArray, GL_STATIC_DRAW);

	glGenBuffers(1, &elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indexBufferObject), cube_indexBufferObject, GL_STATIC_DRAW);

	glGenBuffers(1, &cube_instanceMatrixBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cube_instanceMatrixBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_instanceMatrixArray), cube_instanceMatrixArray, GL_STATIC_DRAW);
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






	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);

	while (glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == GL_FALSE){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		computeMatricesFromInputs();

		// Use our shader
		glUseProgram(programID);

		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		glBindBuffer(GL_ARRAY_BUFFER, cube_complexBuffer);
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

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);


		glDrawElementsInstanced(GL_TRIANGLE_STRIP, 14, GL_UNSIGNED_INT, (void*)0, 8);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
	glDisableVertexAttribArray(5);

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
