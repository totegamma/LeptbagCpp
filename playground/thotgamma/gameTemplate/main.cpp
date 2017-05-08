#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "shader.hpp"

GLFWwindow* window;


GLint windowWidth  = 800;                    // Width of our window
GLint windowHeight = 600;                    // Heightof our window

GLint midWindowX = windowWidth  / 2;         // Middle of the window horizontally
GLint midWindowY = windowHeight / 2;         // Middle of the window vertically

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

// Initial position : on +Z
glm::vec3 position = glm::vec3( 0, 0, 0 ); 
// Initial horizontal angle : toward -Z
double horizontalAngle = 3.14f;
// Initial vertical angle : none
double verticalAngle = 0.0f;
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



static GLfloat cube_color_buffer_data[36*3];

class cube_shape{


	double X;
	double Y;
	double Z;

	GLuint cube_vertexbuffer;
	GLuint cube_colorbuffer;




	public:

	static GLfloat cube_vertex_buffer_data[36*3];
	static GLfloat cube_color_buffer_data[36*3];


	cube_shape(double X, double Y, double Z){
		this->X = X;
		this->Y = Y;
		this->Z = Z;


		glGenBuffers(1, &cube_vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, cube_vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertex_buffer_data), cube_vertex_buffer_data, GL_STATIC_DRAW);

		glGenBuffers(1, &cube_colorbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, cube_colorbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube_color_buffer_data), cube_color_buffer_data, GL_STATIC_DRAW);



	}

	void render(){

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, cube_vertexbuffer);
		glVertexAttribPointer(
				0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
				);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, cube_colorbuffer);
		glVertexAttribPointer(
				1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
				3,                                // size
				GL_FLOAT,                         // type
				GL_FALSE,                         // normalized?
				0,                                // stride
				(void*)0                          // array buffer offset
				);

		//float angle = glfwGetTime() / 10.0 * 45;  // 45° per second
		//glm::vec3 axis_y(0.0, 1.0, 0.0);
		//glm::mat4 anim = glm::rotate(glm::mat4(1.0f), angle, axis_y);

		glm::vec3 position(X, Y, Z);
		glm::mat4 translateMatrix = glm::translate(glm::mat4(1.0f), position);

		// Compute the MVP matrix from keyboard and mouse input
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		//glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix * translateMatrix;
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix * translateMatrix;

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);


		glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles


		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}
};

GLfloat cube_shape::cube_vertex_buffer_data[36*3] = { 
	-0.5f,-0.5f,-0.5f,
	-0.5f,-0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,

	0.5f, 0.5f,-0.5f,
	-0.5f,-0.5f,-0.5f,
	-0.5f, 0.5f,-0.5f,

	0.5f,-0.5f, 0.5f,
	-0.5f,-0.5f,-0.5f,
	0.5f,-0.5f,-0.5f,

	0.5f, 0.5f,-0.5f,
	0.5f,-0.5f,-0.5f,
	-0.5f,-0.5f,-0.5f,

	-0.5f,-0.5f,-0.5f,
	-0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f,-0.5f,

	0.5f,-0.5f, 0.5f,
	-0.5f,-0.5f, 0.5f,
	-0.5f,-0.5f,-0.5f,

	-0.5f, 0.5f, 0.5f,
	-0.5f,-0.5f, 0.5f,
	0.5f,-0.5f, 0.5f,

	0.5f, 0.5f, 0.5f,
	0.5f,-0.5f,-0.5f,
	0.5f, 0.5f,-0.5f,

	0.5f,-0.5f,-0.5f,
	0.5f, 0.5f, 0.5f,
	0.5f,-0.5f, 0.5f,

	0.5f, 0.5f, 0.5f,
	0.5f, 0.5f,-0.5f,
	-0.5f, 0.5f,-0.5f,

	0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f,-0.5f,
	-0.5f, 0.5f, 0.5f,

	0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,
	0.5f,-0.5f, 0.5f
};
GLfloat cube_shape::cube_color_buffer_data[36*3];

std::vector<cube_shape> cube_shape_list;



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



	for(int i = 0; i < 36*3; i++){
		cube_shape::cube_color_buffer_data[i] = (cube_shape::cube_vertex_buffer_data[i] == 0.5f) ? 0.0f : 1.0f;
	}



	int size = 20;
	for(int z = 0; z < size; z++){
		for(int y = 0; y < size; y++){
			for(int x = 0; x < size; x++){
				cube_shape_list.push_back(cube_shape(2*x, 2*y, 2*z));
			}
		}
	}



	while (glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == GL_FALSE){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		computeMatricesFromInputs();

		// Use our shader
		glUseProgram(programID);


		for(auto elem: cube_shape_list){
			elem.render();
		}


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

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
