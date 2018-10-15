#include <iostream>
#include <vector>
#include "../../source/japarilib.hpp"

template <typename... ARGS>
parameterPack* paramPack(ARGS... args){
	return new parameterPack(sizeof... (ARGS), args...);
}



const int numofdog = 5;
const int dnacol = 20;
const int dnarow = 4;

class dog {

	int tag = 0;

	float dna [20][4] = {
		{ -0.74438, -0.0280514,  -0.237172,  0.612237},
		{ 0.512584,   -0.32883,  -0.845608, -0.138221},
		{ 0.418906,   -1.33678,    1.37841,   1.42834},
		{-0.832418, -0.0840262,   0.833291, -0.134319},
		{ -1.26931,   0.726787, -0.0124514, -0.314659},
		{-0.861292,   0.348597,   0.405946,  -1.06412},
		{ 0.311971,  0.0144064,   0.329286, -0.333543},
		{ 0.936644,  -0.988285,  -0.450094, -0.718639},
		{ -1.47339,  -0.423863,  0.0677283,  0.440922},
		{ -1.06763,   -1.39734,   -0.70326,   -0.5131},
		{ 0.444998,   0.245339,    1.23144,  -1.28619},
		{ 0.867622,   0.464909,   -1.08054,  -0.46792},
		{  0.73467,   0.637727,   -1.10756,  0.303961},
		{-0.112369,  -0.839047,   0.822885,  -1.19256},
		{-0.995347,  -0.271012,  -0.809764,  0.915184},
		{ 0.333942,   -1.25962, -0.0206624,  0.866605},
		{ -1.43722,   0.904334,   0.617428,   1.17754},
		{ -1.47288,   0.316685,  -0.962764, -0.394483},
		{-0.374311,   0.313911,   -1.27085, -0.203249},
		{-0.166439,   -1.56434,   0.858862,  -1.05044}
	};

	elementNode* chest;
	elementNode* head;
	elementNode* muzzle;
	elementNode* earLeft;
	elementNode* earRight;
	elementNode* legFrontLeft;
	elementNode* legFrontRight;
	elementNode* legBackLeft;
	elementNode* legBackRight;
	elementNode* tail;

	hingeConstraint* hinge_body_head;
	hingeConstraint* hinge_head_muzzle;
	hingeConstraint* hinge_earLeft_head;
	hingeConstraint* hinge_earRight_head;
	hingeConstraint* hinge_body_legFrontLeft;
	hingeConstraint* hinge_body_legFrontRight;
	hingeConstraint* hinge_body_legBackLeft;
	hingeConstraint* hinge_body_legBackRight;
	hingeConstraint* hinge_body_tail;

	public:

	dog (float x, float y, float z) {
		spawn(x, y, z);
	}



	void spawn(float x, float y, float z){
		//犬の体の構造を定義している
		//キューブで肉体を作る cubeshape::create(位置, 大きさ, 傾き, 重さ, 追加先物理世界);

		chest = getCubeshape()->generate(paramPack(
					param("position", Eigen::Vector3f(    x,     y,     z)),
					param("scale",    Eigen::Vector3f(  1, 0.5, 0.5)),
					param("rotation", Eigen::Quaternionf(1, 0, 0, 0)),
					param("mass", 2.0f)));

		head = getCubeshape()->generate(paramPack(
					param("position", Eigen::Vector3f(x+1.4,     y,     z)),
					param("scale", Eigen::Vector3f(0.4, 0.4, 0.4)),
					param("rotation", Eigen::Quaternionf(1, 0, 0, 0)),
					param("mass", 0.5f)));

		muzzle = getCubeshape()->generate(paramPack(
					param("position", Eigen::Vector3f(x+2.1, y-0.2,     z)),
					param("scale", Eigen::Vector3f(0.3, 0.2, 0.2)),
					param("rotation", Eigen::Quaternionf(1, 0, 0, 0)),
					param("mass", 0.1f)));

		earLeft = getCubeshape()->generate(paramPack(
					param("position", Eigen::Vector3f(x+1.4, y+0.5, z-0.2)),
					param("scale", Eigen::Vector3f(0.1, 0.1, 0.1)),
					param("rotation", Eigen::Quaternionf(1, 0, 0, 0)),
					param("mass", 0.05f)));

		earRight = getCubeshape()->generate(paramPack(
					param("position", Eigen::Vector3f(x+1.4, y+0.5, z+0.2)),
					param("scale", Eigen::Vector3f(0.1, 0.1, 0.1)),
					param("rotation", Eigen::Quaternionf(1, 0, 0, 0)),
					param("mass", 0.05f)));

		legFrontLeft = getCubeshape()->generate(paramPack(
					param("position", Eigen::Vector3f(x+0.5,   y-1, z-0.4)),
					param("scale", Eigen::Vector3f(0.1, 0.5, 0.1)),
					param("rotation", Eigen::Quaternionf(1, 0, 0, 0)),
					param("mass", 0.3f)));

		legFrontRight = getCubeshape()->generate(paramPack(
					param("position", Eigen::Vector3f(x+0.5,   y-1, z+0.4)),
					param("scale", Eigen::Vector3f(0.1, 0.5, 0.1)),
					param("rotation", Eigen::Quaternionf(1, 0, 0, 0)),
					param("mass", 0.3f)));

		legBackLeft = getCubeshape()->generate(paramPack(
					param("position", Eigen::Vector3f(x-0.5,   y-1, z-0.4)),
					param("scale", Eigen::Vector3f(0.1, 0.5, 0.1)),
					param("rotation", Eigen::Quaternionf(1, 0, 0, 0)),
					param("mass", 0.3f)));

		legBackRight = getCubeshape()->generate(paramPack(
					param("position", Eigen::Vector3f(x-0.5,   y-1, z+0.4)),
					param("scale", Eigen::Vector3f(0.1, 0.5, 0.1)),
					param("rotation", Eigen::Quaternionf(1, 0, 0, 0)),
					param("mass", 0.3f)));

		tail = getCubeshape()->generate(paramPack(
					param("position", Eigen::Vector3f(x-1.5, y+0.4,     z)),
					param("scale", Eigen::Vector3f(0.5, 0.1, 0.1)),
					param("rotation", Eigen::Quaternionf(1, 0, 0, 0)),
					param("mass", 0.2f)));


		hinge_body_head			= new hingeConstraint(chest   , head         , Eigen::Vector3f(   1,    0,    0), Eigen::Vector3f(-0.4,   0,    0), Eigen::Vector3f(0, 0, 1), Eigen::Vector3f(0, 0, 1));
		hinge_head_muzzle		= new hingeConstraint(head    , muzzle       , Eigen::Vector3f( 0.4, -0.2,    0), Eigen::Vector3f(-0.3,   0,    0), Eigen::Vector3f(0, 0, 1), Eigen::Vector3f(0, 0, 1));
		hinge_earLeft_head		= new hingeConstraint(earLeft , head         , Eigen::Vector3f(   0, -0.1,    0), Eigen::Vector3f(   0, 0.4, -0.2), Eigen::Vector3f(0, 0, 1), Eigen::Vector3f(0, 0, 1));
		hinge_earRight_head		= new hingeConstraint(earRight, head         , Eigen::Vector3f(   0, -0.1,    0), Eigen::Vector3f(   0, 0.4,  0.2), Eigen::Vector3f(0, 0, 1), Eigen::Vector3f(0, 0, 1));
		hinge_body_legFrontLeft = new hingeConstraint(chest   , legFrontLeft , Eigen::Vector3f( 0.5, -0.5, -0.4), Eigen::Vector3f(   0, 0.5,  0.0), Eigen::Vector3f(0, 0, 1), Eigen::Vector3f(0, 0, 1));
		hinge_body_legFrontRight= new hingeConstraint(chest   , legFrontRight, Eigen::Vector3f( 0.5, -0.5,  0.4), Eigen::Vector3f(   0, 0.5,  0.0), Eigen::Vector3f(0, 0, 1), Eigen::Vector3f(0, 0, 1));
		hinge_body_legBackLeft	= new hingeConstraint(chest   , legBackLeft  , Eigen::Vector3f(-0.5, -0.5, -0.4), Eigen::Vector3f(   0, 0.5,  0.0), Eigen::Vector3f(0, 0, 1), Eigen::Vector3f(0, 0, 1));
		hinge_body_legBackRight	= new hingeConstraint(chest   , legBackRight , Eigen::Vector3f(-0.5, -0.5,  0.4), Eigen::Vector3f(   0, 0.5,  0.0), Eigen::Vector3f(0, 0, 1), Eigen::Vector3f(0, 0, 1));
		hinge_body_tail			= new hingeConstraint(chest   , tail         , Eigen::Vector3f(  -1,  0.4,    0), Eigen::Vector3f( 0.5,   0,  0.0), Eigen::Vector3f(0, 0, 1), Eigen::Vector3f(0, 0, 1));

		hinge_body_head->setLimit(-M_PI/6, M_PI/6);
		hinge_head_muzzle->setLimit(0, 0);
		hinge_earLeft_head->setLimit(0, 0);
		hinge_earRight_head->setLimit(0, 0);
		hinge_body_legFrontLeft->setLimit(-M_PI/2, M_PI/2);
		hinge_body_legFrontRight->setLimit(-M_PI/2, M_PI/2);
		hinge_body_legBackLeft->setLimit(-M_PI/2, M_PI/2);
		hinge_body_legBackRight->setLimit(-M_PI/2, M_PI/2);
		hinge_body_tail->setLimit(-M_PI/3, M_PI/3);

		hinge_body_legFrontLeft->enableMotor(true);
		hinge_body_legFrontLeft->setMaxMotorImpulse(2);
		hinge_body_legFrontRight->enableMotor(true);
		hinge_body_legFrontRight->setMaxMotorImpulse(2);
		hinge_body_legBackLeft->enableMotor(true);
		hinge_body_legBackLeft->setMaxMotorImpulse(2);
		hinge_body_legBackRight->enableMotor(true);
		hinge_body_legBackRight->setMaxMotorImpulse(2);
	}

	void move(int sequence){
		hinge_body_legFrontLeft->setMotorTarget(dna[sequence][0], 0.3);
		hinge_body_legFrontRight->setMotorTarget(dna[sequence][1], 0.3);
		hinge_body_legBackLeft->setMotorTarget(dna[sequence][2], 0.3);
		hinge_body_legBackRight->setMotorTarget(dna[sequence][3], 0.3);
	}


	void despawn(){
		chest->destroy();
		head->destroy();
		muzzle->destroy();
		earLeft->destroy();
		earRight->destroy();
		legFrontLeft->destroy();
		legFrontRight->destroy();
		legBackLeft->destroy();
		legBackRight->destroy();
		tail->destroy();
		hinge_body_head->destroy();
		hinge_head_muzzle->destroy();
		hinge_earLeft_head->destroy();
		hinge_earRight_head->destroy();
		hinge_body_legFrontLeft->destroy();
		hinge_body_legFrontRight->destroy();
		hinge_body_legBackLeft->destroy();
		hinge_body_legBackRight->destroy();
		hinge_body_tail->destroy();

	}
};


std::vector<dog*> doglist;

extern "C"
void init() {

	for (int i = 0; i < numofdog; i++) {
		doglist.push_back(new dog(0, 1.5, -5*i));
	}

}

int timerDivisor = 0;
int sequence = 0;

extern "C"
void tick() {
	if(timerDivisor++ == 6){
		sequence = (sequence+1)%20;
		timerDivisor = 0;

		for (auto elem : doglist){
			elem->move(sequence);
		}
	}
}

