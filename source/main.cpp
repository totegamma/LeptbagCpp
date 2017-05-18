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


class dog{

	public:

	float dna[20][4] = {};
	btDiscreteDynamicsWorld* dynamicsWorld;


	cubeshapeObject* body;
	cubeshapeObject* head;
	cubeshapeObject* muzzle;
	cubeshapeObject* earLeft;
	cubeshapeObject* earRight;
	cubeshapeObject* legFrontLeft;
	cubeshapeObject* legFrontRight;
	cubeshapeObject* legBackLeft;
	cubeshapeObject* legBackRight;
	cubeshapeObject* tail;

	btHingeConstraint* hinge_body_head;
	btHingeConstraint* hinge_head_muzzle;
	btHingeConstraint* hinge_earLeft_head;
	btHingeConstraint* hinge_earRight_head;
	btHingeConstraint* hinge_body_legFrontLeft;
	btHingeConstraint* hinge_body_legFrontRight;
	btHingeConstraint* hinge_body_legBackLeft;
	btHingeConstraint* hinge_body_legBackRight;
	btHingeConstraint* hinge_body_tail;


	dog(btDiscreteDynamicsWorld* dynamicsWorld, float x, float y, float z, bool initialDNA){

		this->dynamicsWorld = dynamicsWorld;


		//DNAをランダムで初期化する
		if(initialDNA == true){
			std::random_device rd;
			std::mt19937 mt(rd());
			std::uniform_real_distribution<double> score(-1.57,1.57);

			for(auto elem: dna){
				elem[0] = score(mt);
				elem[1] = score(mt);
				elem[2] = score(mt);
				elem[3] = score(mt);
			}
		}

		spawn(x, y, z);

	}

	void spawn(float x, float y, float z){
		//犬の体の構造を定義している
		//キューブで肉体を作る cubeshape::create(位置, 大きさ, 傾き, 重さ, 追加先物理世界);
		body			= cubeshape::create(glm::vec3(x,     y,     z),		glm::vec3(2, 1, 1),			glm::quat(1, 0, 0, 0), 2,		dynamicsWorld);
		head			= cubeshape::create(glm::vec3(x+1.4, y,     z),		glm::vec3(0.8, 0.8, 0.8),	glm::quat(1, 0, 0, 0), 0.5,		dynamicsWorld);
		muzzle			= cubeshape::create(glm::vec3(x+2.1, y-0.2, z),		glm::vec3(0.6, 0.4, 0.4),	glm::quat(1, 0, 0, 0), 0.1,		dynamicsWorld);
		earLeft			= cubeshape::create(glm::vec3(x+1.4, y+0.5, z-0.2),	glm::vec3(0.2, 0.2, 0.2),	glm::quat(1, 0, 0, 0), 0.05,	dynamicsWorld);
		earRight		= cubeshape::create(glm::vec3(x+1.4, y+0.5, z+0.2),	glm::vec3(0.2, 0.2, 0.2),	glm::quat(1, 0, 0, 0), 0.05,	dynamicsWorld);
		legFrontLeft	= cubeshape::create(glm::vec3(x+0.5, y-1,   z-0.4),	glm::vec3(0.2, 1, 0.2),		glm::quat(1, 0, 0, 0), 0.3,		dynamicsWorld);
		legFrontRight	= cubeshape::create(glm::vec3(x+0.5, y-1,   z+0.4),	glm::vec3(0.2, 1, 0.2),		glm::quat(1, 0, 0, 0), 0.3,		dynamicsWorld);
		legBackLeft		= cubeshape::create(glm::vec3(x-0.5, y-1,   z-0.4),	glm::vec3(0.2, 1, 0.2),		glm::quat(1, 0, 0, 0), 0.3,		dynamicsWorld);
		legBackRight	= cubeshape::create(glm::vec3(x-0.5, y-1,   z+0.4),	glm::vec3(0.2, 1, 0.2),		glm::quat(1, 0, 0, 0), 0.3,		dynamicsWorld);
		tail			= cubeshape::create(glm::vec3(x-1.5, y+0.4, z),		glm::vec3(1, 0.2, 0.2),		glm::quat(1, 0, 0, 0), 0.2,		dynamicsWorld);

		//肉体同士を関節で接続する	btHingeConstraint(物体A, 物体B, 物体A上の位置, 物体B上の位置, ヒンジの軸の方向);
		hinge_body_head = new btHingeConstraint(*(body->body), *(head->body), btVector3(1, 0, 0), btVector3(-0.4, 0, 0), btVector3(0, 0, 1), btVector3(0, 0, 1));
		hinge_body_head->setLimit(-3.14/6, 3.14/6);
		dynamicsWorld->addConstraint(hinge_body_head, true);

		hinge_head_muzzle = new btHingeConstraint(*(head->body), *(muzzle->body), btVector3(0.4, -0.2, 0), btVector3(-0.3, 0, 0), btVector3(1, 0, 0), btVector3(1, 0, 0));
		hinge_head_muzzle->setLimit(0, 0);
		dynamicsWorld->addConstraint(hinge_head_muzzle, true);

		hinge_earLeft_head = new btHingeConstraint(*(earLeft->body), *(head->body), btVector3(0, -0.1, 0), btVector3(0, 0.4, -0.2), btVector3(1, 0, 0), btVector3(1, 0, 0));
		hinge_earLeft_head->setLimit(0, 0);
		dynamicsWorld->addConstraint(hinge_earLeft_head, true);

		hinge_earRight_head = new btHingeConstraint(*(earRight->body), *(head->body), btVector3(0, -0.1, 0), btVector3(0, 0.4, 0.2), btVector3(1, 0, 0), btVector3(1, 0, 0));
		hinge_earRight_head->setLimit(0, 0);
		dynamicsWorld->addConstraint(hinge_earRight_head, true);


		hinge_body_legFrontLeft = new btHingeConstraint(*(body->body), *(legFrontLeft->body), btVector3(0.5, -0.5, -0.4), btVector3(0, 0.5, 0.0), btVector3(0, 0, 1), btVector3(0, 0, 1));
		hinge_body_legFrontLeft->setLimit(-3.14/2, 3.14/2);
		dynamicsWorld->addConstraint(hinge_body_legFrontLeft, true);

		hinge_body_legFrontRight = new btHingeConstraint(*(body->body), *(legFrontRight->body), btVector3(0.5, -0.5, 0.4), btVector3(0, 0.5, 0.0), btVector3(0, 0, 1), btVector3(0, 0, 1));
		hinge_body_legFrontRight->setLimit(-3.14/2, 3.14/2);
		dynamicsWorld->addConstraint(hinge_body_legFrontRight, true);

		hinge_body_legBackLeft = new btHingeConstraint(*(body->body), *(legBackLeft->body), btVector3(-0.5, -0.5, -0.4), btVector3(0, 0.5, 0.0), btVector3(0, 0, 1), btVector3(0, 0, 1));
		hinge_body_legBackLeft->setLimit(-3.14/2, 3.14/2);
		dynamicsWorld->addConstraint(hinge_body_legBackLeft, true);

		hinge_body_legBackRight = new btHingeConstraint(*(body->body), *(legBackRight->body), btVector3(-0.5, -0.5, 0.4), btVector3(0, 0.5, 0.0), btVector3(0, 0, 1), btVector3(0, 0, 1));
		hinge_body_legBackRight->setLimit(-3.14/2, 3.14/2);
		dynamicsWorld->addConstraint(hinge_body_legBackRight, true);

		hinge_body_tail = new btHingeConstraint(*(body->body), *(tail->body), btVector3(-1, 0.4, 0), btVector3(0.5, 0, 0.0), btVector3(0, 0, 1), btVector3(0, 0, 1));
		hinge_body_tail->setLimit(-3.14/3, 3.14/3);
		dynamicsWorld->addConstraint(hinge_body_tail, true);

		//足の関節にモーターをつけている
		hinge_body_legFrontLeft->enableMotor(true);
		hinge_body_legFrontLeft->setMaxMotorImpulse(2);
		hinge_body_legFrontRight->enableMotor(true);
		hinge_body_legFrontRight->setMaxMotorImpulse(2);
		hinge_body_legBackLeft->enableMotor(true);
		hinge_body_legBackLeft->setMaxMotorImpulse(2);
		hinge_body_legBackRight->enableMotor(true);
		hinge_body_legBackRight->setMaxMotorImpulse(2);
	}


	//シーケンス番号に対応するDNAに記録されている角度まで足を動かす
	void move(int sequence){
		hinge_body_legFrontLeft->setMotorTarget(dna[sequence][0], 0.3);
		hinge_body_legFrontRight->setMotorTarget(dna[sequence][1], 0.3);
		hinge_body_legBackLeft->setMotorTarget(dna[sequence][2], 0.3);
		hinge_body_legBackRight->setMotorTarget(dna[sequence][3], 0.3);
	}

	void destroy(){
		dynamicsWorld->removeConstraint(hinge_body_head);
		dynamicsWorld->removeConstraint(hinge_body_head);
		dynamicsWorld->removeConstraint(hinge_head_muzzle);
		dynamicsWorld->removeConstraint(hinge_earLeft_head);
		dynamicsWorld->removeConstraint(hinge_earRight_head);
		dynamicsWorld->removeConstraint(hinge_body_legFrontLeft);
		dynamicsWorld->removeConstraint(hinge_body_legFrontRight);
		dynamicsWorld->removeConstraint(hinge_body_legBackLeft);
		dynamicsWorld->removeConstraint(hinge_body_legBackRight);
		dynamicsWorld->removeConstraint(hinge_body_tail);

		body->destroy();
		head->destroy();
		muzzle->destroy();
		earLeft->destroy();
		earRight->destroy();
		legFrontLeft->destroy();
		legFrontRight->destroy();
		legBackLeft->destroy();
		legBackRight->destroy();
		tail->destroy();
	}


};




int main(){


	void *lh = dlopen("plugins/testdll.so", RTLD_LAZY);
	if (!lh) {
		fprintf(stderr, "dlopen error: %s\n", dlerror());
		exit(1);
	}
	printf("libdll.so is loaded\n");

	int (*fn)() = (int (*)())dlsym(lh, "dll");
	char *error = dlerror();
	if (error) {
		fprintf(stderr, "dlsym error: %s\n", error);
		exit(1);
	}
	printf("dll() function is found\n");

	(*fn)();

	printf("unloading libdll.so\n");
	dlclose(lh);




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
	btDiscreteDynamicsWorld* dynamicsWorld;
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



	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);



	float topRecord = 0;

	int timerDivisor = 0;
	int time = 0; //0.1秒ごとに増えるんじゃない？知らんけど。
	int generation = 0;
	int sequence = 0;

	std::vector<dog*> doglist;

	//0世代目の犬。全部ランダム。
	for(int i = 0; i < 100; i++){
		doglist.push_back(new dog(dynamicsWorld, 0, 1.5, -5*i, true));
	}


	//犬の交配計算に使う乱数生成器
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> coin(0,1);
	std::uniform_int_distribution<int> RNDnumOfAttack(0,10);
	std::uniform_int_distribution<int> RNDnumOfRow(0,3);
	std::uniform_int_distribution<int> RNDnumOfColumn(0,19);
	std::uniform_real_distribution<double> score(-1.57,1.57);



	//毎フレームごとにこの中が実装される。
	while (glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == GL_FALSE){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//カメラ位置等を計算する
		computeMatricesFromInputs();

		//物理演算1ステップ進める
		dynamicsWorld->stepSimulation(1 / 60.f, 10);

		//時間
		if(timerDivisor++ == 6){
			sequence = (sequence+1)%20; //犬のポーズを変更する
			timerDivisor = 0;

			//犬を動かす
			for(auto elem: doglist){
				elem->move(sequence);
			}

			time ++;
		}

		//世代終わり
		if(time == 30 + generation*2){

			//まずは優良個体を調べる
			float firstdna[20][4];
			float seconddna[20][4];

			float current1stMax = -128; //まぁ-128も後退することはないだろうなという気持ち(これが最低値だろう)
			float current2ndMax = -128; //まぁ-128も後退することはないだろうなという気持ち(これが最低値だろう)

			for(auto elem: doglist){
				btTransform transform;
				elem->muzzle->body->getMotionState()->getWorldTransform(transform);
				btVector3 pos = transform.getOrigin();

				//std::cout << pos.getX() << std::endl;

				//1番と2番を求める
				if(pos.getX() > current1stMax){
					current2ndMax = current1stMax;
					current1stMax = pos.getX();
					memcpy(seconddna, firstdna, sizeof(float)*20*4);
					memcpy(firstdna, elem->dna, sizeof(float)*20*4);
				}else if(pos.getX() > current2ndMax){
					current2ndMax = pos.getX();
					memcpy(seconddna, elem->dna, sizeof(float)*20*4);
				}
			}

			if(current1stMax > topRecord){
				topRecord = current1stMax;
				std::cout << "New Record! " << topRecord << "m" << std::endl;
				std::cout << std::endl;
				std::cout << "---DNA DATA--------------------" << std::endl;
				for(int a = 0; a < 20; a++){
					for(int b = 0; b < 4; b++){
						std::cout << firstdna[a][b] << ", ";
					}
					std::cout << std::endl;
				}
				std::cout << "-------------------------------" << std::endl;
				std::cout << std::endl;
			}


			//今の犬を削除
			while(doglist.size() > 0){
				doglist.back()->destroy();
				doglist.pop_back();
			}


			//新しく犬をつくる

			dog* newdog;

			//1番の犬
			newdog = new dog(dynamicsWorld, 0, 1.5, -5*0, false);
			memcpy(newdog->dna, firstdna, sizeof(float)*20*4);

			doglist.push_back(newdog);

			//2番の犬
			newdog = new dog(dynamicsWorld, 0, 1.5, -5*1, false);
			memcpy(newdog->dna, seconddna, sizeof(float)*20*4);
			doglist.push_back(newdog);

			//残りの犬
			for(int i = 2; i < 100; i++){
				newdog = new dog(dynamicsWorld, 0, 1.5, -5*i, false);

				//交叉
				for(int dnaIndex = 0; dnaIndex < 20; dnaIndex++){
					if(coin(mt) == 0){
						memcpy(newdog->dna[dnaIndex], firstdna[dnaIndex], sizeof(float)*4);
					}else{
						memcpy(newdog->dna[dnaIndex], seconddna[dnaIndex], sizeof(float)*4);
					}
				}

				//突然変異の回数
				int numOfAttack = RNDnumOfAttack(mt);
				
				for(int j = 0; j < numOfAttack; j++){
					newdog->dna[RNDnumOfColumn(mt)][RNDnumOfRow(mt)] = score(mt);
				}

				doglist.push_back(newdog);
			}

			time = 0;
			generation++;
			std::cout << "generation" << generation << " begin" << std::endl;
		}




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


	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();


	return 0;
}
