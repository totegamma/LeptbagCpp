import core.stdc.stdio;
import std.stdio;
import core.runtime;

extern (C) int dll() {
    printf("dll()\n");
    return 0;
}

//Library-----------------------------------

extern (C++) {
	interface cubeshapeObject{
		void destroy();
		void loadMotionState();
	}

	cubeshapeObject* cubeshape_create(float x, float y, float z, float w, float h, float d, float qw, float qx, float qy, float qz, float g);
}

//------------------------------------------

//dog[100] doglist;

dog mydog;

class dog{

cubeshapeObject* chest;
cubeshapeObject* head;
cubeshapeObject* muzzle;
cubeshapeObject* earLeft;
cubeshapeObject* earRight;
cubeshapeObject* legFrontLeft;
cubeshapeObject* legFrontRight;
cubeshapeObject* legBackLeft;
cubeshapeObject* legBackRight;
cubeshapeObject* tail;


	this(float x, float y, float z) {
		spawn(x, y, z);
	}
	void spawn(float x, float y, float z){
		//犬の体の構造を定義している
		//キューブで肉体を作る cubeshape::create(位置, 大きさ, 傾き, 重さ, 追加先物理世界);
		chest			= cubeshape_create(     x,     y,     z,		  2,   1,   1,		1, 0, 0, 0,		   2);
		head			= cubeshape_create( x+1.4,     y,     z,		0.8, 0.8, 0.8,		1, 0, 0, 0,		 0.5);
		muzzle			= cubeshape_create( x+2.1, y-0.2,     z,		0.6, 0.4, 0.4,		1, 0, 0, 0,		 0.1);
		earLeft			= cubeshape_create( x+1.4, y+0.5, z-0.2,		0.2, 0.2, 0.2,		1, 0, 0, 0,		0.05);
		earRight		= cubeshape_create( x+1.4, y+0.5, z+0.2,		0.2, 0.2, 0.2,		1, 0, 0, 0,		0.05);
		legFrontLeft	= cubeshape_create( x+0.5,   y-1, z-0.4,		0.2,   1, 0.2,		1, 0, 0, 0,		 0.3);
		legFrontRight	= cubeshape_create( x+0.5,   y-1, z+0.4,		0.2,   1, 0.2,		1, 0, 0, 0,		 0.3);
		legBackLeft		= cubeshape_create( x-0.5,   y-1, z-0.4,		0.2,   1, 0.2,		1, 0, 0, 0,		 0.3);
		legBackRight	= cubeshape_create( x-0.5,   y-1, z+0.4,		0.2,   1, 0.2,		1, 0, 0, 0,		 0.3);
		tail			= cubeshape_create( x-1.5, y+0.4,     z,		  1, 0.2, 0.2,		1, 0, 0, 0,		 0.2);
	}
	void despawn(){
		chest.destroy();
		head.destroy();
		muzzle.destroy();
		earLeft.destroy();
		earRight.destroy();
		legFrontLeft.destroy();
		legFrontRight.destroy();
		legBackLeft.destroy();
		legBackRight.destroy();
		tail.destroy();
	}
}


//ApplicationInterface----------------------

extern (C) void init(){
	rt_init();
	printf("I will call trueinit!\n");
	mydog = new dog(0, 0, 0);
}



extern (C) void refreash(){
}



//------------------------------------------

/*

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
*/


/*
	std::vector<dog*> doglist;

	//0世代目の犬。全部ランダム。
	for(int i = 0; i < 100; i++){
		doglist.push_back(new dog(dynamicsWorld, 0, 1.5, -5*i, true));
	}





   */
