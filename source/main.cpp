#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <dlfcn.h>
#include <dirent.h>

#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <sstream>
#include <cstdlib>

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
#include "font.hpp"
#include "misc.hpp"

#define ENABLE_SHADOW

constexpr int shadowMapBufferSize = 1024;

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
GLuint uniform_depthBiasVP0;
GLuint uniform_depthBiasVP1;
GLuint uniform_depthBiasVP2;
GLuint uniform_depthBiasVP3;
GLuint uniform_shadowmap0;
GLuint uniform_shadowmap1;
GLuint uniform_shadowmap2;
GLuint uniform_shadowmap3;


btDiscreteDynamicsWorld* dynamicsWorld;


std::vector<void (*)()> pluginInitVector;
std::vector<void (*)()> pluginTickVector;

std::vector<void (*)(int key, int scancode, int action, int mods)> pluginKeyCallbackVector;
std::vector<void (*)(double xpos, double ypos)> pluginMouseMoveCallbackVector;
std::vector<void (*)(int button, int action, int mods)> pluginMouseButtonCallbackVector;

std::vector<void (*)(int width, int height)> pluginWindowResizeCallbackVector;

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

extern "C" void registerWindowResizeCallback(void (*func)(int width, int height)) {
	pluginWindowResizeCallbackVector.push_back(func);
}

extern "C" int getWindowWidth() {
	return ::windowWidth;
}

extern "C" int getWindowHeight() {
	return ::windowHeight;
}

extern "C" void setCursorPos(float x, float y) {
	glfwSetCursorPos(::window, x, y);
}


glm::vec3 lightColor = glm::vec3(1, 1, 1);
float lightPower = 1.0f;
glm::vec3 lightDirection = glm::vec3(-1, 1, 0);

float camx = 0;
float camy = 0;
float camz = 0;


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

	camx = posx;
	camy = posy;
	camz = posz;

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
	ProjectionMatrix = glm::perspective(FoV, (float)::windowWidth/(float)::windowHeight, 0.1f, 300.0f);

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
				glfwSetCursorPosCallback(::window, NULL);
				glfwSetKeyCallback(::window, NULL);
				glfwSetCursorPos(::window, ::windowWidth/2, ::windowHeight/2);
				glfwSetInputMode(::window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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
				glfwSetInputMode(::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				glfwSetCursorPos(::window, ::windowWidth/2, ::windowHeight/2);
				glfwSetCursorPosCallback(::window, handleMouseMove);
				glfwSetKeyCallback(::window, handleKeypress);
				break;
		}
	}
}

void handleWindowResize(GLFWwindow* window, int width, int height) {
	::windowWidth  = width;
	::windowHeight = height;

	for(auto elem : pluginWindowResizeCallbackVector) {
		(elem)(width, height);
	}

	font::updateWindowSize(width, height);
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

	setlocale(LC_ALL, "ja_JP.UTF-8");

	if (!glfwInit()) {
		std::cout << "glfw init failed...." << std::endl;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	::window = glfwCreateWindow(::windowWidth, ::windowHeight, "LeptBag", NULL, NULL);
	if (!::window) {
		std::cout << "cannot open OpenGL window" << std::endl;
	}
	glfwMakeContextCurrent(::window);

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
	// Get a handle for our "VP" uniform
	uniform_viewMatrix = glGetUniformLocation(programID, "V");
	uniform_projectionMatrix = glGetUniformLocation(programID, "P");
	uniform_LightColor = glGetUniformLocation(programID, "LightColor");
	uniform_LightPower = glGetUniformLocation(programID, "LightPower");
	uniform_LightDirection = glGetUniformLocation(programID, "LightDirection");
	uniform_depthBiasVP0 = glGetUniformLocation(programID, "DepthBiasVP0");
	uniform_depthBiasVP1 = glGetUniformLocation(programID, "DepthBiasVP1");
	uniform_depthBiasVP2 = glGetUniformLocation(programID, "DepthBiasVP2");
	uniform_depthBiasVP3 = glGetUniformLocation(programID, "DepthBiasVP3");
	uniform_shadowmap0 = glGetUniformLocation(programID, "shadowMap0");
	uniform_shadowmap1 = glGetUniformLocation(programID, "shadowMap1");
	uniform_shadowmap2 = glGetUniformLocation(programID, "shadowMap2");
	uniform_shadowmap3 = glGetUniformLocation(programID, "shadowMap3");


	//----- 影関連 -----//

#ifdef ENABLE_SHADOW

	// Create and compile our GLSL program from the shaders
	GLuint depthProgramID = LoadShaders( "depthBuffer.vert", "depthBuffer.frag");
	// Get a handle for our "VP" uniform
	GLuint depthMatrixID = glGetUniformLocation(depthProgramID, "depthMV");

	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	GLuint shadowMap0FrameBuffer;
	glGenFramebuffers(1, &shadowMap0FrameBuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowMap0FrameBuffer);

	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	GLuint depthTexture0;
	glGenTextures(1, &depthTexture0);
	glBindTexture(GL_TEXTURE_2D, depthTexture0);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT32, shadowMapBufferSize, shadowMapBufferSize, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture0, 0);

	// No color output in the bound framebuffer, only depth.
	glDrawBuffer(GL_NONE);

	// Always check that our framebuffer is ok
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		return false;
	}

	GLuint shadowMap1FrameBuffer;
	glGenFramebuffers(1, &shadowMap1FrameBuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowMap1FrameBuffer);

	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	GLuint depthTexture1;
	glGenTextures(1, &depthTexture1);
	glBindTexture(GL_TEXTURE_2D, depthTexture1);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT32, shadowMapBufferSize, shadowMapBufferSize, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture1, 0);

	// No color output in the bound framebuffer, only depth.
	glDrawBuffer(GL_NONE);

	// Always check that our framebuffer is ok
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		return false;
	}

	GLuint shadowMap2FrameBuffer;
	glGenFramebuffers(1, &shadowMap2FrameBuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowMap2FrameBuffer);

	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	GLuint depthTexture2;
	glGenTextures(1, &depthTexture2);
	glBindTexture(GL_TEXTURE_2D, depthTexture2);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT32, shadowMapBufferSize, shadowMapBufferSize, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture2, 0);

	// No color output in the bound framebuffer, only depth.
	glDrawBuffer(GL_NONE);

	// Always check that our framebuffer is ok
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		return false;
	}

	GLuint shadowMap3FrameBuffer;
	glGenFramebuffers(1, &shadowMap3FrameBuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowMap3FrameBuffer);

	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	GLuint depthTexture3;
	glGenTextures(1, &depthTexture3);
	glBindTexture(GL_TEXTURE_2D, depthTexture3);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT32, shadowMapBufferSize, shadowMapBufferSize, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture3, 0);

	// No color output in the bound framebuffer, only depth.
	glDrawBuffer(GL_NONE);

	// Always check that our framebuffer is ok
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		return false;
	}

#endif

	//----- 影関連 -----//

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	//入力のコールバック・カーソルタイプの設定
	glfwSetInputMode(::window, GLFW_STICKY_KEYS, 1);
	glfwSetKeyCallback(::window, handleKeypress);
	glfwSetMouseButtonCallback(::window, handleMouseButton);
	glfwSetCursorPosCallback(::window, handleMouseMove);
	glfwSetInputMode(::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetWindowSizeCallback(::window, handleWindowResize);


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


	//フォント描画モジュールの初期化
	font::setup(::windowWidth, ::windowHeight);

	//ロードされるダイナミックライブラリのリスト
	std::vector<void*> dllList;

	const char* path = "./plugins/";
	DIR *dp;       // ディレクトリへのポインタ
	dirent* entry; // readdir() で返されるエントリーポイント

	dp = opendir(path);
	if (dp==NULL) exit(1);
	entry = readdir(dp);
	while (entry != NULL) {
		std::string filename(entry->d_name);
		if (split(filename,'.').size() >= 2 && split(filename, '.')[1] == "so") {

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

	//---------------------------------------------------------------------


	float vertAng = 0;
	float horizAng = 0;

	glm::vec3 position = glm::vec3(0, 0, 0);

	//カメラの向きを計算する
	glm::vec3 direction(
			cos(vertAng) * sin(horizAng), 
			sin(vertAng),
			cos(vertAng) * cos(horizAng)
			);


	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(45.0f, (float)::windowWidth/(float)::windowHeight, 0.1f, 300.0f);

	// Camera matrix
	ViewMatrix = glm::lookAt(
			position,           // Camera is here
			position+direction, // and looks here : at the same position, plus "direction"
			glm::vec3(0,1,0)    // Head is up (set to 0,-1,0 to look upside-down)
			);

	//--------------------------------------------------------------------------


	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glEnableVertexAttribArray(6);


	initPrimitives();

	//毎フレーム描画
	while (glfwWindowShouldClose(::window) == GL_FALSE) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		for (auto elem: pluginTickVector) {
			(elem)();
		}

		//物理演算1ステップ進める
		dynamicsWorld->stepSimulation(1 / 60.f, 10);


		// :: OpenGL描画 ::

#ifdef ENABLE_SHADOW

		// まずはデプスバッファを作る

		// Render to our framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, shadowMap0FrameBuffer);
		glViewport(0,0,shadowMapBufferSize,shadowMapBufferSize);

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(depthProgramID);

		glm::vec3 lightPosition = glm::vec3(camx, 0, camz);

		// Compute the VP matrix from the light's point of view
		glm::mat4 depthProjectionMatrix0 = glm::ortho<float>(-4,4,-4,4,-30,30);
		glm::mat4 depthViewMatrix0 = glm::lookAt(lightPosition, lightPosition-lightDirection, glm::vec3(0,1,0));

		glm::mat4 depthVP0 = depthProjectionMatrix0 * depthViewMatrix0;

		// Send our transformation to the currently bound shader,
		// in the "VP" uniform
		glUniformMatrix4fv(depthMatrixID, 1, GL_FALSE, &depthVP0[0][0]);


		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(sizeof(GLfloat)*3));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(sizeof(GLfloat)*6));


		for(auto elem: elementManager::elementManagerList){ //TODO 最適化できるはず(下にも同じコード)
			elem->render();
		}

		// まずはデプスバッファを作る

		// Render to our framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, shadowMap1FrameBuffer);
		glViewport(0,0,shadowMapBufferSize,shadowMapBufferSize); // Render on the whole framebuffer, complete from the lower left corner to the upper right

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(depthProgramID);


		// Compute the VP matrix from the light's point of view
		glm::mat4 depthProjectionMatrix1 = glm::ortho<float>(-8,8,-8,8,-30,30);
		glm::mat4 depthViewMatrix1 = glm::lookAt(lightPosition, lightPosition-lightDirection, glm::vec3(0,1,0));

		glm::mat4 depthVP1 = depthProjectionMatrix1 * depthViewMatrix1;

		// Send our transformation to the currently bound shader,
		// in the "VP" uniform
		glUniformMatrix4fv(depthMatrixID, 1, GL_FALSE, &depthVP1[0][0]);


		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(sizeof(GLfloat)*3));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(sizeof(GLfloat)*6));


		for(auto elem: elementManager::elementManagerList){ //TODO 最適化できるはず(下にも同じコード)
			elem->render();
		}

		// まずはデプスバッファを作る

		// Render to our framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, shadowMap2FrameBuffer);
		glViewport(0,0,shadowMapBufferSize,shadowMapBufferSize); // Render on the whole framebuffer, complete from the lower left corner to the upper right

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(depthProgramID);


		// Compute the VP matrix from the light's point of view
		glm::mat4 depthProjectionMatrix2 = glm::ortho<float>(-16,16,-16,16,-30,30);
		glm::mat4 depthViewMatrix2 = glm::lookAt(lightPosition, lightPosition-lightDirection, glm::vec3(0,1,0));

		glm::mat4 depthVP2 = depthProjectionMatrix2 * depthViewMatrix2;

		// Send our transformation to the currently bound shader,
		// in the "VP" uniform
		glUniformMatrix4fv(depthMatrixID, 1, GL_FALSE, &depthVP2[0][0]);


		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(sizeof(GLfloat)*3));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(sizeof(GLfloat)*6));


		for(auto elem: elementManager::elementManagerList){ //TODO 最適化できるはず(下にも同じコード)
			elem->render();
		}

		// まずはデプスバッファを作る

		// Render to our framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, shadowMap3FrameBuffer);
		glViewport(0,0,shadowMapBufferSize,shadowMapBufferSize); // Render on the whole framebuffer, complete from the lower left corner to the upper right

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(depthProgramID);


		// Compute the VP matrix from the light's point of view
		glm::mat4 depthProjectionMatrix3 = glm::ortho<float>(-32,32,-32,32,-30,30);
		glm::mat4 depthViewMatrix3 = glm::lookAt(lightPosition, lightPosition-lightDirection, glm::vec3(0,1,0));

		glm::mat4 depthVP3 = depthProjectionMatrix3 * depthViewMatrix3;

		// Send our transformation to the currently bound shader,
		// in the "VP" uniform
		glUniformMatrix4fv(depthMatrixID, 1, GL_FALSE, &depthVP3[0][0]);


		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(sizeof(GLfloat)*3));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(sizeof(GLfloat)*6));


		for(auto elem: elementManager::elementManagerList){ //TODO 最適化できるはず(下にも同じコード)
			elem->render();
		}

#endif

		// 通常描画
		// Render to the screen
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, ::windowWidth*2, ::windowHeight*2); // Render on the whole framebuffer, complete from the lower left corner to the upper right

		//glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(programID);

		glm::mat4 biasMatrix(
			0.5, 0.0, 0.0, 0.0,
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
			0.5, 0.5, 0.5, 1.0
		);

#ifdef ENABLE_SHADOW
		glm::mat4 depthBiasVP0 = biasMatrix*depthVP0;
		glm::mat4 depthBiasVP1 = biasMatrix*depthVP1;
		glm::mat4 depthBiasVP2 = biasMatrix*depthVP2;
		glm::mat4 depthBiasVP3 = biasMatrix*depthVP3;
#endif

		glUniformMatrix4fv(uniform_viewMatrix,       1, GL_FALSE, &ViewMatrix[0][0]);
		glUniformMatrix4fv(uniform_projectionMatrix, 1, GL_FALSE, &ProjectionMatrix[0][0]);
#ifdef ENABLE_SHADOW
		glUniformMatrix4fv(uniform_depthBiasVP0,      1, GL_FALSE, &depthBiasVP0[0][0]);
		glUniformMatrix4fv(uniform_depthBiasVP1,      1, GL_FALSE, &depthBiasVP1[0][0]);
		glUniformMatrix4fv(uniform_depthBiasVP2,      1, GL_FALSE, &depthBiasVP2[0][0]);
		glUniformMatrix4fv(uniform_depthBiasVP3,      1, GL_FALSE, &depthBiasVP3[0][0]);
#endif
		glUniform3fv      (uniform_LightColor,       1, &lightColor[0]);
		glUniform1fv      (uniform_LightPower,       1, &lightPower);
		glUniform3fv      (uniform_LightDirection,   1, &lightDirection[0]);

#ifdef ENABLE_SHADOW
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthTexture0);
		glUniform1i(uniform_shadowmap0, 1);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, depthTexture1);
		glUniform1i(uniform_shadowmap1, 2);

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, depthTexture2);
		glUniform1i(uniform_shadowmap2, 3);

		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, depthTexture3);
		glUniform1i(uniform_shadowmap3, 4);
#endif

		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(sizeof(GLfloat)*3));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(sizeof(GLfloat)*6));


		for (auto elem: elementManager::elementManagerList) {
			elem->render();
		}

		//フォントの描画
		font::draw();

		glfwSwapBuffers(::window);
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
