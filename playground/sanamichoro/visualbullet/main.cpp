#include <iostream>
#include <vector>
#include <random>
#include <math.h>

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

#include </usr/local/include/eigen3/Eigen/Core>
#include "dog.cpp"
//#include "driveNetwork.cpp"

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


int main(){

	Eigen::MatrixXf neko = Eigen::MatrixXf::Random(3, 3);



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
		doglist.push_back(new dog(dynamicsWorld, 0, 1.5, -5*i));
	}


	//犬の交配計算に使う乱数生成器
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> coin(0,1);
	std::uniform_int_distribution<int> RNDnumOfAttack(0,10);
	std::uniform_int_distribution<int> RNDnumOfRow(0,7);
	std::uniform_int_distribution<int> RNDnumOfColumn(0,19);
	std::uniform_real_distribution<double> score(-1.57, 1.57);
	std::uniform_real_distribution<double> scoreLower(0.0, 1.50);


	//毎フレームごとにこの中が実行される。
	while (glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == GL_FALSE){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//カメラ位置等を計算する
		computeMatricesFromInputs();

		//物理演算1ステップ進める
		dynamicsWorld->stepSimulation(1 / 180.f, 10);

		//時間
		//if(timerDivisor++ == 6){
			sequence = (sequence+1)%20; //犬のポーズを変更する
			timerDivisor = 0;

			//犬を動かす
			if(sequence%10==0){
			for(auto elem: doglist){
				elem->cpgMove();
			}
			}
			/*
			std::cout<<doglist[0]->Aup*cos(doglist[0]->thetaPre[0])<<" : "<<
							0.5*sin(doglist[0]->hinge_bodyFront_legUpperFrontRight->getHingeAngle())<<
								std::endl;
								*/


			time ++;
		//}
		/*
		double neko = fabs( doglist[0]->hinge_bodyFront_legUpperFrontLeft->getHingeAngle() - doglist[0]->omega[0]);
		if( neko >= 0.05) std::cout<<neko<<" : "<<doglist[0]->hinge_bodyFront_legUpperFrontLeft->getHingeAngle()<<" : "<<doglist[0]->omega[0]<<std::endl;
		doglist[0]->omega[0] = doglist[0]->hinge_bodyFront_legUpperFrontLeft->getHingeAngle();
		*/





		//世代終わり
		if(false){//time == 50 + generation*3){

			//まずは優良個体を調べる
			float firstdna[20][8];
			float seconddna[20][8];

			float current1stMax = -128; //まぁ-128も後退することはないだろうなという気持ち(これが最低値だろう)
			float current2ndMax = -128; //まぁ-128も後退することはないだろうなという気持ち(これが最低値だろう)


			driveNetwork first, second;
			for(auto elem: doglist){
				btTransform transform;
				elem->muzzle->body->getMotionState()->getWorldTransform(transform);
				btVector3 pos = transform.getOrigin();

				//std::cout << pos.getX() << std::endl;

				//1番と2番を求める
				if(pos.getX() > current1stMax){
					current2ndMax = current1stMax;
					current1stMax = pos.getX();
					first = elem->cpg;
					//memcpy(seconddna, firstdna, sizeof(float)*20*8);
					//memcpy(firstdna, elem->dna, sizeof(float)*20*8);
				}else if(pos.getX() > current2ndMax){
					current2ndMax = pos.getX();
					second = elem->cpg;
					//memcpy(seconddna, elem->dna, sizeof(float)*20*8);
				}
			}

			if(current1stMax > topRecord){
				topRecord = current1stMax;
				std::cout << "New Record! " << topRecord << "m" << std::endl;
				std::cout << std::endl;
				/*
				std::cout << "---DNA DATA--------------------" << std::endl;
				for(int a = 0; a < 20; a++){
					for(int b = 0; b < 8; b++){
						std::cout << firstdna[a][b] << ", ";
					}
					std::cout << std::endl;
				}
				std::cout << "-------------------------------" << std::endl;
				std::cout << std::endl;
				*/
			}


			//今の犬を削除
			while(doglist.size() > 0){
				doglist.back()->destroy();
				doglist.pop_back();
			}


			//新しく犬をつくる

			dog* newdog;

			//1番の犬
			newdog = new dog(dynamicsWorld, 0, 1.5, -5*0);
			//memcpy(newdog->dna, firstdna, sizeof(float)*20*8);
			newdog->cpg = first;
			doglist.push_back(newdog);

			//2番の犬
			newdog = new dog(dynamicsWorld, 0, 1.5, -5*1);
			//memcpy(newdog->dna, seconddna, sizeof(float)*20*8);
			newdog->cpg = second;
			doglist.push_back(newdog);

			//残りの犬
			for(int i = 2; i < 100; i++){
				newdog = new dog(dynamicsWorld, 0, 1.5, -5*i);

				/*
				//交叉
				for(int dnaIndex = 0; dnaIndex < 20; dnaIndex++){
					if(coin(mt) == 0){
						memcpy(newdog->dna[dnaIndex], firstdna[dnaIndex], sizeof(float)*8);
					}else{
						memcpy(newdog->dna[dnaIndex], seconddna[dnaIndex], sizeof(float)*8);
					}
				}
				*/

				//newdog->cpg.cross(first, second);

				//突然変異の回数
				int numOfAttack = RNDnumOfAttack(mt);
				
				for(int j = 0; j < numOfAttack; j++){
					//newdog->dna[RNDnumOfColumn(mt)][RNDnumOfRow(mt)] = score(mt);
					//newdog->cpg.mutate(RNDnumOfAttack);
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
