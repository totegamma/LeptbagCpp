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
#include "driveNetwork.cpp"


class dog{

	public:

	float dna[20][8] = {};

	//蔵本モデルのパラメータ
	double omega[8];
	double omegaLimit;
	double thetaPre[8], thetaPost[8];
	double kUpDown, kUpUp, kDownUp, kDownDown, kSameLegUp, kSameLegDown;
	double thetaUpDown;
	double Aup, Adown;
	double phiLeanUp, phiLeanDown;
	bool phiOnLimit[8];
	double thetaIsAboveLineX[8];
	double deltaT = 0.1;

	driveNetwork cpg;

	btDiscreteDynamicsWorld* dynamicsWorld;


	cubeshapeObject* bodyFront;
	cubeshapeObject* bodyBack;
	cubeshapeObject* head;
	cubeshapeObject* muzzle;
	cubeshapeObject* earLeft;
	cubeshapeObject* earRight;
	cubeshapeObject* legUpperFrontLeft;
	cubeshapeObject* legUpperFrontRight;
	cubeshapeObject* legUpperBackLeft;
	cubeshapeObject* legUpperBackRight;
	cubeshapeObject* legLowerFrontLeft;
	cubeshapeObject* legLowerFrontRight;
	cubeshapeObject* legLowerBackLeft;
	cubeshapeObject* legLowerBackRight;
	cubeshapeObject* tail;


	btHingeConstraint* hinge_bodyFront_bodyBack;
	btHingeConstraint* hinge_bodyFront_head;
	btHingeConstraint* hinge_head_muzzle;
	btHingeConstraint* hinge_earLeft_head;
	btHingeConstraint* hinge_earRight_head;
	btHingeConstraint* hinge_bodyFront_legUpperFrontLeft;
	btHingeConstraint* hinge_bodyFront_legUpperFrontRight;
	btHingeConstraint* hinge_bodyBack_legUpperBackLeft;
	btHingeConstraint* hinge_bodyBack_legUpperBackRight;
	btHingeConstraint* hinge_legUpperFrontLeft_legLowerFrontLeft;
	btHingeConstraint* hinge_legUpperFrontRight_legLowerFrontRight;
	btHingeConstraint* hinge_legUpperBackLeft_legLowerBackLeft;
	btHingeConstraint* hinge_legUpperBackRight_legLowerBackRight;
	btHingeConstraint* hinge_bodyBack_tail;

	dog(btDiscreteDynamicsWorld* dynamicsWorld, float x, float y, float z, bool initialDNA){

		this->dynamicsWorld = dynamicsWorld;


		//DNAをランダムで初期化する
		if(initialDNA == true){
			std::random_device rd;
			std::mt19937 mt(rd());
			std::uniform_real_distribution<double> score(-1.57,1.57);
			std::uniform_real_distribution<double> scoreLower(0.0, 1.50);

			for(auto elem: dna){
				elem[0] = score(mt);
				elem[1] = score(mt);
				elem[2] = score(mt);
				elem[3] = score(mt);
				elem[4] = scoreLower(mt);
				elem[5] = scoreLower(mt);
				elem[6] = scoreLower(mt);
				elem[7] = scoreLower(mt);
			}
			/*
			   for(int i=0;i<20;i++){
			   dna[0][i] = sin( 3.1415 * (double)i / 20.0 - 1.57);
			   dna[1][i] = sin( 1.50 * (double)i / 20.0);
			   }
			   */


		}

		spawn(x, y, z);

	}

	//driveNetwork用
	dog(btDiscreteDynamicsWorld* dynamicsWorld, float x, float y, float z){

		this->dynamicsWorld = dynamicsWorld;
		spawn(x, y, z);

	}

	//蔵本モデル用
	dog(btDiscreteDynamicsWorld* dynamicsWorld, float x, float y, float z, bool initialDNA, bool letKuramoto){

		this->dynamicsWorld = dynamicsWorld;


		//DNAをランダムで初期化する
		if(initialDNA == true){
			std::random_device rd;
			std::mt19937 mt(rd());
			std::uniform_real_distribution<double> score(0, 3.141592);

			omegaLimit = score(mt); //最大の自然振動数
			std::uniform_real_distribution<double> omegaLim(0.0, omegaLimit);
			for(int i=0; i<8; i++){
				omega[i] = omegaLim(mt);
				thetaPre[i] = 3.141592 / 2.0;
				thetaPost[i] = 3.141592 / 2.0;
				phiOnLimit[i] = false;
				thetaIsAboveLineX[i] = 1.0;
			}
			std::uniform_real_distribution<double> kLim(0.0, 10.0);
			kUpDown = kLim(mt);
			kUpUp = kLim(mt);
			kDownUp = kLim(mt);
			kDownDown = kLim(mt);
			kSameLegUp = kLim(mt);
			kSameLegDown = kLim(mt);
			thetaUpDown = score(mt);
			Aup = 1.0 * fabs( sin( score(mt) ) );
			Adown = 1.0 * fabs( sin( score(mt) ) );
			phiLeanUp = score(mt) - 1.57079;
			phiLeanDown = -1.0 * score(mt) / 2.0;



		}

		spawn(x, y, z);

	}


	

	void spawn(float x, float y, float z){
		//犬の体の構造を定義している
		//キューブで肉体を作る cubeshape::create(位置, 大きさ, 傾き, 重さ, 追加先物理世界);


		bodyFront		= cubeshape::create(glm::vec3(x+0.5,     y,     z),		glm::vec3(1, 1, 1),			glm::quat(1, 0, 0, 0), 1,		dynamicsWorld);
		bodyBack		= cubeshape::create(glm::vec3(x-0.5,     y,     z),		glm::vec3(1, 1, 1),			glm::quat(1, 0, 0, 0), 1,		dynamicsWorld);
		head			= cubeshape::create(glm::vec3(x+1.4, y,     z),		glm::vec3(0.8, 0.8, 0.8),	glm::quat(1, 0, 0, 0), 0.2,		dynamicsWorld);
		muzzle			= cubeshape::create(glm::vec3(x+2.1, y-0.2, z),		glm::vec3(0.6, 0.4, 0.4),	glm::quat(1, 0, 0, 0), 0.03,		dynamicsWorld);
		earLeft			= cubeshape::create(glm::vec3(x+1.4, y+0.5, z-0.2),	glm::vec3(0.2, 0.2, 0.2),	glm::quat(1, 0, 0, 0), 0.01,	dynamicsWorld);
		earRight		= cubeshape::create(glm::vec3(x+1.4, y+0.5, z+0.2),	glm::vec3(0.2, 0.2, 0.2),	glm::quat(1, 0, 0, 0), 0.01,	dynamicsWorld);

		//脚
		legUpperFrontLeft	= cubeshape::create(glm::vec3(x+0.5, y-0.75,   z-0.4),	glm::vec3(0.2, 0.5, 0.2),		glm::quat(1, 0, 0, 0), 0.3,		dynamicsWorld);
		legUpperFrontRight	= cubeshape::create(glm::vec3(x+0.5, y-0.75,   z+0.4),	glm::vec3(0.2, 0.5, 0.2),		glm::quat(1, 0, 0, 0), 0.3,		dynamicsWorld);
		legUpperBackLeft		= cubeshape::create(glm::vec3(x-0.5, y-0.75,   z-0.4),	glm::vec3(0.2, 0.5, 0.2),		glm::quat(1, 0, 0, 0), 0.3,		dynamicsWorld);
		legUpperBackRight	= cubeshape::create(glm::vec3(x-0.5, y-0.75,   z+0.4),	glm::vec3(0.2, 0.5, 0.2),		glm::quat(1, 0, 0, 0), 0.3,		dynamicsWorld);

		legLowerFrontLeft	= cubeshape::create(glm::vec3(x+0.5, y-1.25, z-0.4),	glm::vec3(0.2, 0.5, 0.2),	glm::quat(1, 0, 0, 0), 0.25,	dynamicsWorld);
		legLowerFrontRight	= cubeshape::create(glm::vec3(x+0.5, y-1.25, z+0.4),	glm::vec3(0.2, 0.5, 0.2),	glm::quat(1, 0, 0, 0), 0.25,	dynamicsWorld);
		legLowerBackLeft	= cubeshape::create(glm::vec3(x-0.5, y-1.25, z-0.4),	glm::vec3(0.2, 0.5, 0.2),	glm::quat(1, 0, 0, 0), 0.25,	dynamicsWorld);
		legLowerBackRight	= cubeshape::create(glm::vec3(x-0.5, y-1.25, z+0.4),	glm::vec3(0.2, 0.5, 0.2),	glm::quat(1, 0, 0, 0), 0.25,	dynamicsWorld);

		tail			= cubeshape::create(glm::vec3(x-1.5, y+0.4, z),		glm::vec3(1, 0.2, 0.2),		glm::quat(1, 0, 0, 0), 0.2,		dynamicsWorld);

		//肉体同士を関節で接続する	btHingeConstraint(物体A, 物体B, 物体A上の位置, 物体B上の位置, ヒンジの軸の方向);

		hinge_bodyFront_bodyBack = new btHingeConstraint(*(bodyFront->body), *(bodyBack->body), 
				btVector3(-0.5, 0, 0), btVector3(0.5, 0, 0), btVector3(0, 1, 0), btVector3(0, 1, 0));
		hinge_bodyFront_bodyBack->setLimit(-3.14/24.0, 3.14/24.0);
		dynamicsWorld->addConstraint(hinge_bodyFront_bodyBack, true);

		hinge_bodyFront_head = new btHingeConstraint(*(bodyFront->body), *(head->body),
				btVector3(0.5, 0, 0), btVector3(-0.4, 0, 0), btVector3(0, 0, 1), btVector3(0, 0, 1));
		hinge_bodyFront_head->setLimit(-3.14/6, 3.14/6);
		dynamicsWorld->addConstraint(hinge_bodyFront_head, true);

		hinge_head_muzzle = new btHingeConstraint(*(head->body), *(muzzle->body), btVector3(0.4, -0.2, 0), btVector3(-0.3, 0, 0), btVector3(1, 0, 0), btVector3(1, 0, 0));
		hinge_head_muzzle->setLimit(0, 0);
		dynamicsWorld->addConstraint(hinge_head_muzzle, true);

		hinge_earLeft_head = new btHingeConstraint(*(earLeft->body), *(head->body), btVector3(0, -0.1, 0), btVector3(0, 0.4, -0.2), btVector3(1, 0, 0), btVector3(1, 0, 0));
		hinge_earLeft_head->setLimit(0, 0);
		dynamicsWorld->addConstraint(hinge_earLeft_head, true);

		hinge_earRight_head = new btHingeConstraint(*(earRight->body), *(head->body), btVector3(0, -0.1, 0), btVector3(0, 0.4, 0.2), btVector3(1, 0, 0), btVector3(1, 0, 0));
		hinge_earRight_head->setLimit(0, 0);
		dynamicsWorld->addConstraint(hinge_earRight_head, true);


		//脚関節
		hinge_bodyFront_legUpperFrontLeft = new btHingeConstraint(*(bodyFront->body), *(legUpperFrontLeft->body),
				btVector3(0.5, -0.5, -0.49), btVector3(0, 0.25, 0.0), btVector3(0, 0, 1), btVector3(0, 0, 1));
		hinge_bodyFront_legUpperFrontLeft->setLimit(-3.14/2.0, 3.14/2.0);
		dynamicsWorld->addConstraint(hinge_bodyFront_legUpperFrontLeft, true);

		hinge_bodyFront_legUpperFrontRight = new btHingeConstraint(*(bodyFront->body), *(legUpperFrontRight->body),
				btVector3(0.5, -0.5, 0.49), btVector3(0, 0.25, 0.0), btVector3(0, 0, 1), btVector3(0, 0, 1));
		hinge_bodyFront_legUpperFrontRight->setLimit(-3.14/2.0, 3.14/2.0);
		dynamicsWorld->addConstraint(hinge_bodyFront_legUpperFrontRight, true);

		hinge_bodyBack_legUpperBackLeft = new btHingeConstraint(*(bodyBack->body), *(legUpperBackLeft->body),
				btVector3(-0.5, -0.5, -0.49), btVector3(0, 0.25, 0.0), btVector3(0, 0, 1), btVector3(0, 0, 1));
		hinge_bodyBack_legUpperBackLeft->setLimit(-3.14/2.0, 3.14/2.0);
		dynamicsWorld->addConstraint(hinge_bodyBack_legUpperBackLeft, true);

		hinge_bodyBack_legUpperBackRight = new btHingeConstraint(*(bodyBack->body), *(legUpperBackRight->body),
				btVector3(-0.5, -0.5, 0.49), btVector3(0, 0.25, 0.0), btVector3(0, 0, 1), btVector3(0, 0, 1));
		hinge_bodyBack_legUpperBackRight->setLimit(-3.14/2.0, 3.14/2.0);
		dynamicsWorld->addConstraint(hinge_bodyBack_legUpperBackRight, true);

		hinge_bodyBack_tail = new btHingeConstraint(*(bodyBack->body), *(tail->body),
				btVector3(-0.5, 0.4, 0), btVector3(0.5, 0, 0.0), btVector3(0, 0, 1), btVector3(0, 0, 1));
		hinge_bodyBack_tail->setLimit(-3.14/6.0, 3.14/6.0);
		dynamicsWorld->addConstraint(hinge_bodyBack_tail, true);


		hinge_legUpperFrontLeft_legLowerFrontLeft = new btHingeConstraint(*(legUpperFrontLeft->body), *(legLowerFrontLeft->body),
				btVector3(0, -0.25, 0), btVector3(0, 0.25, 0), btVector3(0, 0, 1), btVector3(0, 0, 1));
		hinge_legUpperFrontLeft_legLowerFrontLeft->setLimit(0.0, 1.50);
		dynamicsWorld->addConstraint(hinge_legUpperFrontLeft_legLowerFrontLeft, true);

		hinge_legUpperFrontRight_legLowerFrontRight = new btHingeConstraint(*(legUpperFrontRight->body), *(legLowerFrontRight->body),
				btVector3(0, -0.25, 0), btVector3(0, 0.25, 0), btVector3(0, 0, 1), btVector3(0, 0, 1));
		hinge_legUpperFrontRight_legLowerFrontRight->setLimit(0.0, 1.50);
		dynamicsWorld->addConstraint(hinge_legUpperFrontRight_legLowerFrontRight, true);

		hinge_legUpperBackLeft_legLowerBackLeft = new btHingeConstraint(*(legUpperBackLeft->body), *(legLowerBackLeft->body),
				btVector3(0, -0.25, 0), btVector3(0, 0.25, 0), btVector3(0, 0, 1), btVector3(0, 0, 1));
		hinge_legUpperBackLeft_legLowerBackLeft->setLimit(0.0, 1.50);
		dynamicsWorld->addConstraint(hinge_legUpperBackLeft_legLowerBackLeft, true);

		hinge_legUpperBackRight_legLowerBackRight = new btHingeConstraint(*(legUpperBackRight->body), *(legLowerBackRight->body),
				btVector3(0, -0.25, 0), btVector3(0, 0.25, 0), btVector3(0, 0, 1), btVector3(0, 0, 1));
		hinge_legUpperBackRight_legLowerBackRight->setLimit(0.0, 1.50);
		dynamicsWorld->addConstraint(hinge_legUpperBackRight_legLowerBackRight, true);
		



		//足の関節にモーターをつけている
		hinge_bodyFront_legUpperFrontLeft->enableMotor(true);
		hinge_bodyFront_legUpperFrontLeft->setMaxMotorImpulse(7);
		hinge_bodyFront_legUpperFrontRight->enableMotor(true);
		hinge_bodyFront_legUpperFrontRight->setMaxMotorImpulse(7);
		hinge_bodyBack_legUpperBackLeft->enableMotor(true);
		hinge_bodyBack_legUpperBackLeft->setMaxMotorImpulse(7);
		hinge_bodyBack_legUpperBackRight->enableMotor(true);
		hinge_bodyBack_legUpperBackRight->setMaxMotorImpulse(7);

		hinge_legUpperFrontLeft_legLowerFrontLeft->enableMotor(true);
		hinge_legUpperFrontLeft_legLowerFrontLeft->setMaxMotorImpulse(7);
		hinge_legUpperFrontRight_legLowerFrontRight->enableMotor(true);
		hinge_legUpperFrontRight_legLowerFrontRight->setMaxMotorImpulse(7);
		hinge_legUpperBackLeft_legLowerBackLeft->enableMotor(true);
		hinge_legUpperBackLeft_legLowerBackLeft->setMaxMotorImpulse(7);
		hinge_legUpperBackRight_legLowerBackRight->enableMotor(true);
		hinge_legUpperBackRight_legLowerBackRight->setMaxMotorImpulse(7);


	}


	//シーケンス番号に対応するDNAに記録されている角度まで足を動かす
	void move(int sequence){

		hinge_bodyFront_legUpperFrontLeft->setMotorTarget(dna[sequence][0], 0.3);
		hinge_bodyFront_legUpperFrontRight->setMotorTarget(dna[sequence][1], 0.3);
		hinge_bodyBack_legUpperBackLeft->setMotorTarget(dna[sequence][2], 0.3);
		hinge_bodyBack_legUpperBackRight->setMotorTarget(dna[sequence][3], 0.3);

		hinge_legUpperFrontLeft_legLowerFrontLeft->setMotorTarget(dna[sequence][4], 0.3);
		hinge_legUpperFrontRight_legLowerFrontRight->setMotorTarget(dna[sequence][5], 0.3);
		hinge_legUpperBackLeft_legLowerBackLeft->setMotorTarget(dna[sequence][6], 0.3);
		hinge_legUpperBackRight_legLowerBackRight->setMotorTarget(dna[sequence][7], 0.3);

	}

	//driveNetwork
	void cpgMove(){

		double phi[8];

		phi[0] = hinge_bodyFront_legUpperFrontRight->getHingeAngle();
		phi[1] = hinge_bodyBack_legUpperBackLeft->getHingeAngle();
		phi[2] = hinge_bodyFront_legUpperFrontLeft->getHingeAngle();
		phi[3] = hinge_bodyBack_legUpperBackRight->getHingeAngle();

		phi[4] = hinge_legUpperFrontRight_legLowerFrontRight->getHingeAngle();
		phi[5] = hinge_legUpperBackLeft_legLowerBackLeft->getHingeAngle();
		phi[6] = hinge_legUpperFrontLeft_legLowerFrontLeft->getHingeAngle();
		phi[7] = hinge_legUpperBackRight_legLowerBackRight->getHingeAngle();

		Eigen::MatrixXf ped = Eigen::MatrixXf::Zero(1, 8);
		for(int i=0; i<8; i++) ped(0, i) = phi[i];
		ped = this->cpg.forward(ped);
		for(int i=0; i<8; i++) phi[i] = ped(0, i);

		hinge_bodyFront_legUpperFrontRight->setMotorTarget( phi[0], 0.008);
		hinge_bodyBack_legUpperBackLeft->setMotorTarget( phi[1], 0.008);
		hinge_bodyFront_legUpperFrontLeft->setMotorTarget( phi[2], 0.008);
		hinge_bodyBack_legUpperBackRight->setMotorTarget( phi[3], 0.008);

		hinge_legUpperFrontRight_legLowerFrontRight->setMotorTarget( phi[4], 0.008);
		hinge_legUpperBackLeft_legLowerBackLeft->setMotorTarget( phi[5], 0.008);
		hinge_legUpperFrontLeft_legLowerFrontLeft->setMotorTarget( phi[6], 0.008);
		hinge_legUpperBackRight_legLowerBackRight->setMotorTarget( phi[7], 0.008);

	}


	//蔵本モデルに基づいたmove()
	void kuramotoMove(){

		double phi[8];

		thetaIsAboveLineX[0] = hinge_bodyFront_legUpperFrontRight->getHingeAngle() - phi[0];
		thetaIsAboveLineX[1] = hinge_bodyBack_legUpperBackLeft->getHingeAngle() - phi[1];
		thetaIsAboveLineX[2] = hinge_bodyFront_legUpperFrontLeft->getHingeAngle() - phi[2];
		thetaIsAboveLineX[3] = hinge_bodyBack_legUpperBackRight->getHingeAngle() - phi[3];

		thetaIsAboveLineX[4] = hinge_legUpperFrontRight_legLowerFrontRight->getHingeAngle() - phi[4];
		thetaIsAboveLineX[5] = hinge_legUpperBackLeft_legLowerBackLeft->getHingeAngle() - phi[5];
		thetaIsAboveLineX[6] = hinge_legUpperFrontLeft_legLowerFrontLeft->getHingeAngle() - phi[6];
		thetaIsAboveLineX[7] = hinge_legUpperBackRight_legLowerBackRight->getHingeAngle() - phi[7];

		phi[0] = hinge_bodyFront_legUpperFrontRight->getHingeAngle();
		phi[1] = hinge_bodyBack_legUpperBackLeft->getHingeAngle();
		phi[2] = hinge_bodyFront_legUpperFrontLeft->getHingeAngle();
		phi[3] = hinge_bodyBack_legUpperBackRight->getHingeAngle();

		phi[4] = hinge_legUpperFrontRight_legLowerFrontRight->getHingeAngle();
		phi[5] = hinge_legUpperBackLeft_legLowerBackLeft->getHingeAngle();
		phi[6] = hinge_legUpperFrontLeft_legLowerFrontLeft->getHingeAngle();
		phi[7] = hinge_legUpperBackRight_legLowerBackRight->getHingeAngle();

		//std::cout<<Aup*cos(thetaPre[0])<<" : "<<0.5*sin(phi[0])<<std::endl;

		//振動中心が傾いているのでそれを一旦鉛直にする．
		for(int i=0; i<4; i++) if(!phiOnLimit[i]) phi[i] -= phiLeanUp;
		for(int i=4; i<8; i++) if(!phiOnLimit[i]) phi[i] -= phiLeanDown;

		//現在のモデルの位相thetaPreを計算
		for(int i=0; i<4; i++){
			//phiOnLimitがtrueのときは限界角度にphiがある．
			if(!phiOnLimit[i]){
				thetaPre[i] = acos( 0.5 * sin(phi[i]) / Aup );
				//acosではモデルの位相thetaがx軸より上にあるかどうかわからないのでここで修正
				if( thetaIsAboveLineX[i]>0 ) thetaPre[i] = -1.0 * thetaPre[i];
			}else{
				//限界角度にphiがあるときはphiはモデルから出さない．
				thetaPre[i] = thetaPost[i];
			}
		}

		//ひざ下のときのthetaPre
		for(int i=4; i<8; i++){
			if(!phiOnLimit[i]){
				thetaPre[i] = acos( 0.5 * tan(phi[i]) / Aup );
				if( thetaIsAboveLineX[i]>0 ) thetaPre[i] = -1.0 * thetaPre[i];
			}else{
				thetaPre[i] = thetaPost[i];
			}
		}

		//thetaをどれくらい動かすか考えるので計算を実行する前に現在のthetaを保存しておく．
		for(int j=0; j<8; j++) thetaPost[j] = thetaPre[j];

		//thetaを計算する
		for(int i=0; i<4; i++){
			//relateToUpが膝上の関節の影響を反映した項
			double relateToUp = 0.0;
			double relateToDown = 0.0;
			//thetaはpi/2ずつ位相差を持っているのでそれを一旦同位相にする
			for(int j=0; j<4; j++){ thetaPre[j] -= 3.141592*j/2.0; thetaPre[j+4] -= 3.141592*j/2.0; }

			for(int j=0; j<4; j++) relateToUp += kUpUp * sin( thetaPre[j] - thetaPre[i] );
			for(int j=4; j<8; j++){
				if( i!= j-4 ) relateToDown = kUpDown * sin( thetaPre[j] + thetaUpDown - thetaPre[i] );
				else relateToDown += kSameLegUp * sin( thetaPre[j] + thetaUpDown - thetaPre[i] );
			}
			relateToUp = relateToUp / 4.0;
			relateToDown = relateToDown / 4.0;

			for(int j=0; j<4; j++){ thetaPre[j] += 3.141592*j/2.0; thetaPre[j+4] += 3.141592*j/2.0; }

			thetaPost[i] += ( omega[i] + relateToUp + relateToDown ) * deltaT;
		}


		for(int i=4; i<8; i++){
			double relateToUp = 0.0;
			double relateToDown = 0.0;
			for(int j=0; j<4; j++){ thetaPre[j] -= 3.141592*j/2.0; thetaPre[j+4] -= 3.141592*j/2.0; }

			for(int j=0; j<4; j++){
				if( i-4 != j ) relateToUp = kDownUp * sin( thetaPre[j] - thetaUpDown - thetaPre[i] );
				else relateToUp += kSameLegDown * sin( thetaPre[j] - thetaUpDown - thetaPre[i] );
			}
			for(int j=4; j<8; j++) relateToDown += kDownDown * sin( thetaPre[j] - thetaPre[i] );
			relateToUp = relateToUp / 4.0;
			relateToDown = relateToDown / 4.0;

			for(int j=0; j<4; j++){ thetaPre[j] += 3.141592*j/2.0; thetaPre[j+4] += 3.141592*j/2.0; }

			thetaPost[i] += ( omega[i] + relateToUp + relateToDown ) * deltaT;
		}

		//thetaが大きくなりすぎないようにする
		for(int i=0; i<8; i++) if(thetaPost[i]>=3.141592) thetaPost[i] -= 3.141592;

		//thetaからphiを計算
		for(int i=0; i<4; i++) phi[i] = asin( Aup * cos(thetaPost[i]) / 0.5);
		for(int i=4; i<8; i++) phi[i] = asin( Adown * cos(thetaPost[i]) / 0.5);

		//振動中心に向けて傾ける
		for(int i=0; i<4; i++) if(!phiOnLimit[i]) phi[i] += phiLeanUp;
		for(int i=4; i<8; i++) if(!phiOnLimit[i]) phi[i] += phiLeanDown;

		//限界角度を超えていたらphiOnLimitをtrueにし，phiを限界角度に戻す
		for(int i=0; i<4; i++){
			if(phi[i]>1.50){ phi[i] = 1.50; phiOnLimit[i] = true; }
			else if(phi[i]<-1.50){ phi[i] = -1.50; phiOnLimit[i] = true; }
			else phiOnLimit[i] = false;
		}

		for(int i=4; i<8; i++){
			if(phi[i] > 0.1){ phi[i] = 0.0; phiOnLimit[i] = true; }
			else if(phi[i]<-1.50){ phi[i] = -1.50; phiOnLimit[i] = true; }
			else phiOnLimit[i] = false;
		}



		hinge_bodyFront_legUpperFrontRight->setMotorTarget( phi[0], 0.3);
		hinge_bodyBack_legUpperBackLeft->setMotorTarget( phi[1], 0.3);
		hinge_bodyFront_legUpperFrontLeft->setMotorTarget( phi[2], 0.3);
		hinge_bodyBack_legUpperBackRight->setMotorTarget( phi[3], 0.3);

		hinge_legUpperFrontRight_legLowerFrontRight->setMotorTarget( phi[4], 0.3);
		hinge_legUpperBackLeft_legLowerBackLeft->setMotorTarget( phi[5], 0.3);
		hinge_legUpperFrontLeft_legLowerFrontLeft->setMotorTarget( phi[6], 0.3);
		hinge_legUpperBackRight_legLowerBackRight->setMotorTarget( phi[7], 0.3);

	}

	void destroy(){

		dynamicsWorld->removeConstraint(hinge_bodyFront_bodyBack);
		dynamicsWorld->removeConstraint(hinge_bodyFront_head);
		dynamicsWorld->removeConstraint(hinge_head_muzzle);
		dynamicsWorld->removeConstraint(hinge_earLeft_head);
		dynamicsWorld->removeConstraint(hinge_earRight_head);
		dynamicsWorld->removeConstraint(hinge_bodyFront_legUpperFrontLeft);
		dynamicsWorld->removeConstraint(hinge_bodyFront_legUpperFrontRight);
		dynamicsWorld->removeConstraint(hinge_bodyBack_legUpperBackLeft);
		dynamicsWorld->removeConstraint(hinge_bodyBack_legUpperBackRight);
		dynamicsWorld->removeConstraint(hinge_legUpperFrontLeft_legLowerFrontLeft);
		dynamicsWorld->removeConstraint(hinge_legUpperFrontRight_legLowerFrontRight);
		dynamicsWorld->removeConstraint(hinge_legUpperBackLeft_legLowerBackLeft);
		dynamicsWorld->removeConstraint(hinge_legUpperBackRight_legLowerBackRight);
		dynamicsWorld->removeConstraint(hinge_bodyBack_tail);


		bodyFront->destroy();
		bodyBack->destroy();
		head->destroy();
		muzzle->destroy();
		earLeft->destroy();
		earRight->destroy();
		legUpperFrontLeft->destroy();
		legUpperFrontRight->destroy();
		legUpperBackLeft->destroy();
		legUpperBackRight->destroy();
		legLowerFrontLeft->destroy();
		legLowerFrontRight->destroy();
		legLowerBackLeft->destroy();
		legLowerBackRight->destroy();
		tail->destroy();
	}


};



