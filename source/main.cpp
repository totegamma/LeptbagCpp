#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <sstream>
#include <cstdlib>

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <dirent.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <bullet/btBulletDynamicsCommon.h>


#include "vertexManager.hpp"
#include "shader.hpp"
#include "constraints.hpp"
#include "utilities/utilities.hpp"
#include "elementNode.hpp"
#include "elementManager.hpp"
#include "bodyGenerator.hpp"
#include "primitiveShape.hpp"
#include "misc.hpp"


GLFWwindow* window;

//ウィンドウの大きさ
GLint windowWidth  = 1000;
GLint windowHeight = 800;

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

GLuint uniform_viewMatrix;
GLuint uniform_projectionMatrix;
GLuint uniform_LightColor;
GLuint uniform_LightPower;
GLuint uniform_LightDirection;


btDiscreteDynamicsWorld* dynamicsWorld;


std::vector<void (*)()> pluginInitVector;
std::vector<void (*)()> pluginTickVector;

std::vector<void (*)(int key, int scancode, int action, int mods)> pluginKeyCallbackVector;
std::vector<void (*)(double xpos, double ypos)> pluginMouseMoveCallbackVector;
std::vector<void (*)(int button, int action, int mods)> pluginMouseButtonCallbackVector;

// TODO:　unregister関数を作成すること!!!!!
extern "C" void registerKeyCallback(void (*func)(int key, int scancode, int action, int mods)) {
	pluginKeyCallbackVector.push_back(func);
}

extern "C" void registerMouseMoveCallback(void (*func)(double xpos, double ypos)) {
	pluginMouseMoveCallbackVector.push_back(func);
}

extern "C" void registerMouseButtonCallback(void (*func)(int button, int action, int mods)) {
	pluginMouseButtonCallbackVector.push_back(func);
}

extern "C" int getWindowWidth() {
	return windowWidth;
}

extern "C" int getWindowHeight() {
	return windowHeight;
}

extern "C" void setCursorPos(float x, float y) {
	glfwSetCursorPos(window, x, y);
}


glm::vec3 lightColor = glm::vec3(1, 1, 1);
float lightPower = 1.0f;
glm::vec3 lightDirection = glm::vec3(-1, 1, 0);


std::vector<std::string> split(const std::string &str, char sep) {
	std::vector<std::string> v;
	std::stringstream ss(str);
	std::string buffer;
	while (std::getline(ss, buffer, sep)) {
		v.push_back(buffer);
	}
	return v;
}

void (*cameraAccessAllowedFuncAddr)(void) = nullptr;

extern "C" int requestCameraAccess(void (*func)(void)) {
	if (cameraAccessAllowedFuncAddr != nullptr) {
		return -1;
	}

	cameraAccessAllowedFuncAddr = func;
	return 1;
}


extern "C" void updateCamera(float posx, float posy, float posz, float horizAng, float vertAng, float FoV) {

	Dl_info info;
	dladdr(__builtin_return_address(0), &info);

	if (info.dli_saddr != cameraAccessAllowedFuncAddr) {
		return;
	}

	glm::vec3 position = glm::vec3(posx, posy, posz);

	//カメラの向きを計算する
	glm::vec3 direction(
			cos(vertAng) * sin(horizAng), 
			sin(vertAng),
			cos(vertAng) * cos(horizAng)
			);


	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(FoV, (float)windowWidth/(float)windowHeight, 0.1f, 300.0f);

	// Camera matrix
	ViewMatrix = glm::lookAt(
			position,           // Camera is here
			position+direction, // and looks here : at the same position, plus "direction"
			glm::vec3(0,1,0)    // Head is up (set to 0,-1,0 to look upside-down)
			);

}



void handleMouseMove(GLFWwindow* window, double xpos, double ypos) {

	for (auto elem: pluginMouseMoveCallbackVector) {
		(elem)(xpos, ypos);
	}

}


void handleKeypress(GLFWwindow* window, int key, int scancode, int action, int mods) {

	for(auto elem: pluginKeyCallbackVector) {
		(elem)(key, scancode, action, mods);
	}

	// [esc]でマウスポインタを開放する
	if (action == GLFW_PRESS) {
		switch(key) {
			case GLFW_KEY_ESCAPE:
				glfwSetCursorPosCallback(window, NULL);
				glfwSetKeyCallback(window, NULL);
				glfwSetCursorPos(window, windowWidth/2, windowHeight/2);
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				break;

			default:
				break;
		}
	}
}

void handleMouseButton(GLFWwindow* window, int button, int action, int mods) {

	for (auto elem: pluginMouseButtonCallbackVector) {
		(elem)(button, action, mods);
	}

	// 非フォーカスからの復帰
	if (action == GLFW_PRESS) {
		switch(button){
			case GLFW_MOUSE_BUTTON_1:
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				glfwSetCursorPos(window, windowWidth/2, windowHeight/2);
				glfwSetCursorPosCallback(window, handleMouseMove);
				glfwSetKeyCallback(window, handleKeypress);
				break;
		}
	}
}

void handleWindowResize(GLFWwindow* window, int width, int height) {
	windowWidth  = width;
	windowHeight = height;
}


//オイラー角から４次元数を計算する。opengl-math用とbullet用で2つある。
glm::quat createq(double RotationAngle, double RotationAxisX, double RotationAxisY, double RotationAxisZ) {
	double x = RotationAxisX * sin(RotationAngle / 2);
	double y = RotationAxisY * sin(RotationAngle / 2);
	double z = RotationAxisZ * sin(RotationAngle / 2);
	double w = cos(RotationAngle / 2);
	return glm::quat(w, x, y, z);
}

btQuaternion btcreateq(double RotationAngle, double RotationAxisX, double RotationAxisY, double RotationAxisZ) {
	double x = RotationAxisX * sin(RotationAngle / 2);
	double y = RotationAxisY * sin(RotationAngle / 2);
	double z = RotationAxisZ * sin(RotationAngle / 2);
	double w = cos(RotationAngle / 2);
	return btQuaternion(x, y, z, w);
}



int main() {

	if (!glfwInit()) {
		std::cout << "glfw init failed...." << std::endl;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(windowWidth, windowHeight, "Japari", NULL, NULL);
	if (!window) {
		std::cout << "cannot open OpenGL window" << std::endl;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit () != GLEW_OK) {
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
	GLuint programID = LoadShaders( "main.vert", "main.frag" );
	// Get a handle for our "MVP" uniform
	uniform_viewMatrix = glGetUniformLocation(programID, "V");
	uniform_projectionMatrix = glGetUniformLocation(programID, "P");
	uniform_LightColor = glGetUniformLocation(programID, "LightColor");
	uniform_LightPower = glGetUniformLocation(programID, "LightPower");
	uniform_LightDirection = glGetUniformLocation(programID, "LightDirection");


	//入力のコールバック・カーソルタイプの設定
	glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
	glfwSetKeyCallback(window, handleKeypress);
	glfwSetMouseButtonCallback(window, handleMouseButton);
	glfwSetCursorPosCallback(window, handleMouseMove);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetWindowSizeCallback(window, handleWindowResize);


	//物理ワールドの生成
	btBroadphaseInterface* broadphase = new btDbvtBroadphase();
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -10, 0));


	//頂点バッファオブジェクトを作る
	initVBO();


	initPrimitives();



	std::vector<void*> dllList;

	const char* path = "./friends/";
	DIR *dp;       // ディレクトリへのポインタ
	dirent* entry; // readdir() で返されるエントリーポイント

	dp = opendir(path);
	if (dp==NULL) exit(1);
	entry = readdir(dp);
	while (entry != NULL) {
		std::string filename(entry->d_name);
		if (split(filename,'.').size() >= 2 && split(filename, '.')[1] == "friends") {

			void* lh = dlopen((path + filename).c_str(), RTLD_LAZY);
			if (!lh) {
				fprintf(stderr, "dlopen error: %s\n", dlerror());
				exit(1);
			}

			void (*pluginInit)() = (void (*)())dlsym(lh, "init");
			char *error = dlerror();
			if (error) {
				fprintf(stderr, "dlsym error: %s\n", error);
				exit(1);
			}
			pluginInitVector.push_back(*pluginInit);

			void (*pluginTick)() = (void (*)())dlsym(lh, "tick");
			error = dlerror();
			if (error) {
				fprintf(stderr, "dlsym error: %s\n", error);
				exit(1);
			}
			pluginTickVector.push_back(*pluginTick);

			dllList.push_back(lh);


		}

		entry = readdir(dp);
	}


	for (auto elem: pluginInitVector) {
		(elem)();
	}



	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glEnableVertexAttribArray(6);



	//毎フレームごとにこの中が実装される。
	while (glfwWindowShouldClose(window) == GL_FALSE) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		for (auto elem: pluginTickVector) {
			(elem)();
		}

		//物理演算1ステップ進める
		dynamicsWorld->stepSimulation(1 / 60.f, 10);


		//OpenGL描画
		glUseProgram(programID);

		glUniformMatrix4fv(uniform_viewMatrix,       1, GL_FALSE, &ViewMatrix[0][0]);
		glUniformMatrix4fv(uniform_projectionMatrix, 1, GL_FALSE, &ProjectionMatrix[0][0]);
		glUniform3fv(uniform_LightColor, 1, &lightColor[0]);
		glUniform1fv(uniform_LightPower, 1, &lightPower);
		glUniform3fv(uniform_LightDirection, 1, &lightDirection[0]);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(sizeof(GLfloat)*3));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(sizeof(GLfloat)*6));


		for (auto elem: elementManager::elementManagerList) {
			elem->render();
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	std::cout << "stopping program..." << std::endl;

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
	glDisableVertexAttribArray(5);
	glDisableVertexAttribArray(6);

	printf("unloading libdll.so\n");

	while (dllList.empty() == false) {
		dlclose(dllList.back());
		dllList.pop_back();
	}


	while (elementManager::elementManagerList.empty() == false) {
		delete elementManager::elementManagerList.back();
		elementManager::elementManagerList.pop_back();
	}

	delete dynamicsWorld;
	delete solver;
	delete dispatcher;
	delete collisionConfiguration;
	delete broadphase;

	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();



	return 0;
}
