#include <iostream>
#include <vector>
#include <random>

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

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
#include "constraints.hpp"

GLFWwindow* window;

//ウィンドウの大きさ
GLint windowWidth  = 1000;
GLint windowHeight = 800;

//半分の大きさを定義しておく。ポインタを固定する位置に使う。
GLint midWindowX = windowWidth  / 2;
GLint midWindowY = windowHeight / 2;


glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

GLuint MatrixID;


btDiscreteDynamicsWorld* dynamicsWorld;



//カメラの位置など
glm::vec3 position = glm::vec3( 0, 0, 0 ); 
double horizontalAngle = 3.14f;
double verticalAngle = 0.0f;

float initialFoV = 45.0f;

float speed = 0.1f;
float mouseSpeed = 0.001f;



// Hoding any keys down?
bool holdingForward     = false;
bool holdingBackward    = false;
bool holdingLeftStrafe  = false;
bool holdingRightStrafe = false;

bool holdingSneek = false;
bool holdingSpace = false;


void computeMatricesFromInputs(){


	//カメラの向きを計算する
	glm::vec3 direction(
			cos(verticalAngle) * sin(horizontalAngle), 
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
			);


	//カメラ移動
	if (holdingForward == true){
		position[0] += sin(horizontalAngle)* speed;
		position[2] += cos(horizontalAngle)* speed;
	}

	if (holdingBackward == true){
		position[0] += sin(horizontalAngle+3.14)* speed;
		position[2] += cos(horizontalAngle+3.14)* speed;
	}

	if (holdingRightStrafe == true){
		position[0] += sin(horizontalAngle-(3.14/2))* speed;
		position[2] += cos(horizontalAngle-(3.14/2))* speed;
	}

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


	float FoV = initialFoV;

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 300.0f);

	// Camera matrix
	ViewMatrix = glm::lookAt(
			position,           // Camera is here
			position+direction, // and looks here : at the same position, plus "direction"
			glm::vec3(0,1,0)    // Head is up (set to 0,-1,0 to look upside-down)
			);

}

void handleMouseMove(GLFWwindow* window, double xpos, double ypos){

	//カメラが一回転したら強制的に2PI回すことで無限に回れるようにする
	if(horizontalAngle + mouseSpeed * float(midWindowX - xpos) > 3.14){
		horizontalAngle = (horizontalAngle + mouseSpeed * float(midWindowX - xpos)) - (3.14*2);
	}else if(horizontalAngle + mouseSpeed * float(midWindowX - xpos) < -3.14){
		horizontalAngle = (horizontalAngle + mouseSpeed * float(midWindowX - xpos)) + (3.14*2);
	}else{
		horizontalAngle += mouseSpeed * float(midWindowX - xpos );
	}

	//カメラは真下から真上までの範囲しか動かない。頭は縦に一回転しない。
	if(verticalAngle + mouseSpeed * float(midWindowY - ypos ) > 3.14/2){
		verticalAngle = 3.14/2;
	}else if(verticalAngle + mouseSpeed * float(midWindowY - ypos ) < -3.14/2){
		verticalAngle = -3.14/2;
	}else{
		verticalAngle   += mouseSpeed * float(midWindowY - ypos );
	}

	//マウスを強制的に真ん中に戻す
	glfwSetCursorPos(window, midWindowX, midWindowY);
}

void handleKeypress(GLFWwindow* window, int key, int scancode, int action, int mods){

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

			case GLFW_KEY_LEFT_SHIFT:
				holdingSneek = true;
				break;

			case GLFW_KEY_SPACE:
				holdingSpace = true;
				break;

			default:
				break;
		}
	}else if(action == GLFW_RELEASE){
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


//オイラー角から４次元数を計算する。opengl-math用とbullet用で2つある。
glm::quat createq(double RotationAngle, double RotationAxisX, double RotationAxisY, double RotationAxisZ){
	double x = RotationAxisX * sin(RotationAngle / 2);
	double y = RotationAxisY * sin(RotationAngle / 2);
	double z = RotationAxisZ * sin(RotationAngle / 2);
	double w = cos(RotationAngle / 2);
	return glm::quat(w, x, y, z);
}

btQuaternion btcreateq(double RotationAngle, double RotationAxisX, double RotationAxisY, double RotationAxisZ){
	double x = RotationAxisX * sin(RotationAngle / 2);
	double y = RotationAxisY * sin(RotationAngle / 2);
	double z = RotationAxisZ * sin(RotationAngle / 2);
	double w = cos(RotationAngle / 2);
	return btQuaternion(x, y, z, w);
}


int main(){

	if (!glfwInit()){
		std::cout << "glfw init failed...." << std::endl;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(windowWidth, windowHeight, "My Title", NULL, NULL);
	if (!window){
		std::cout << "cannot open OpenGL window" << std::endl;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if(glewInit () != GLEW_OK){
		std::cout << "glew init failed...." << std::endl;
	}

	glClearColor(0.5f, 0.5f, 1.0f, 0.0f);


	glEnable(GL_DEPTH_TEST); //隠面消去
	glDepthFunc(GL_LESS);    //近いものを表示
	glEnable(GL_CULL_FACE);  //ウラは表示しない


	//VAOを作る
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);


	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader" );
	// Get a handle for our "MVP" uniform
	MatrixID = glGetUniformLocation(programID, "MVP");
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);


	//入力のコールバック・カーソルタイプの設定
	glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
	glfwSetKeyCallback(window, handleKeypress);
	glfwSetCursorPosCallback(window, handleMouseMove);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	//物理ワールドの生成
	btBroadphaseInterface* broadphase = new btDbvtBroadphase();
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -10, 0));


	//頂点バッファオブジェクトを作る
	initVBO();

	//使う図形についてinit
	cubeshape::init();
	floorshape::init();

	//床を作る
	floorshape::create(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), glm::quat(1, 0, 0, 0), dynamicsWorld);

	void *lh = dlopen("plugins/libdog.so", RTLD_LAZY);
	if (!lh) {
		fprintf(stderr, "dlopen error: %s\n", dlerror());
		exit(1);
	}
	printf("libdog.so is loaded\n");

	void (*pluginInit)() = (void (*)())dlsym(lh, "init");

	char *error = dlerror();
	if (error) {
		fprintf(stderr, "dlsym error: %s\n", error);
		exit(1);
	}
	printf("init() function is found\n");

	(*pluginInit)();

	void (*pluginTick)() = (void (*)())dlsym(lh, "tick");
	error = dlerror();
	if (error) {
		fprintf(stderr, "dlsym error: %s\n", error);
		exit(1);
	}
	printf("tick() function is found\n");




	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);



	//毎フレームごとにこの中が実装される。
	while (glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == GL_FALSE){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//カメラ位置等を計算する
		computeMatricesFromInputs();

		(*pluginTick)();


		//物理演算1ステップ進める
		dynamicsWorld->stepSimulation(1 / 60.f, 10);


		//OpenGL描画
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

	printf("unloading libdll.so\n");
	dlclose(lh);


	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();


	return 0;
}
