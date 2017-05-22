import core.stdc.stdio;
import std.stdio;
import core.runtime;


//Library-----------------------------------

extern (C++) {
	interface cubeshapeObject{
		void destroy();
		void loadMotionState();
	}

	cubeshapeObject cubeshape_create(float x, float y, float z, float w, float h, float d, float qw, float qx, float qy, float qz, float g);

}

extern (C++) {
	interface hingeConstraint{
		void sayHello();
		void enableMotor(bool flag);
		void setLimit(float lower, float upper);
		void setMaxMotorImpulse(float power);
		void destroy();	
	}
}

extern (C) {
	hingeConstraint hingeConstraint_create(cubeshapeObject cubeA, cubeshapeObject cubeB, float ax, float ay, float az, float bx, float by, float bz, float vx, float vy, float vz);
}

//------------------------------------------

//dog[100] doglist;

dog mydog;

class dog{

cubeshapeObject chest;
cubeshapeObject head;
cubeshapeObject muzzle;
cubeshapeObject earLeft;
cubeshapeObject earRight;
cubeshapeObject legFrontLeft;
cubeshapeObject legFrontRight;
cubeshapeObject legBackLeft;
cubeshapeObject legBackRight;
cubeshapeObject tail;

hingeConstraint hinge_body_head;
hingeConstraint hinge_head_muzzle;
hingeConstraint hinge_earLeft_head;
hingeConstraint hinge_earRight_head;
hingeConstraint hinge_body_legFrontLeft;
hingeConstraint hinge_body_legFrontRight;
hingeConstraint hinge_body_legBackLeft;
hingeConstraint hinge_body_legBackRight;
hingeConstraint hinge_body_tail;


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


		hinge_body_head			= hingeConstraint_create(chest   , head         ,    1,    0,    0, -0.4,   0,    0, 0, 0, 1);
		hinge_head_muzzle		= hingeConstraint_create(head    , muzzle       ,  0.4, -0.2,    0, -0.3,   0,    0, 0, 0, 1);
		hinge_earLeft_head		= hingeConstraint_create(earLeft , head         ,    0, -0.1,    0,    0, 0.4, -0.2, 0, 0, 1);
		hinge_earRight_head		= hingeConstraint_create(earRight, head         ,    0, -0.1,    0,    0, 0.4,  0.2, 0, 0, 1);
		hinge_body_legFrontLeft = hingeConstraint_create(chest   , legFrontLeft ,  0.5, -0.5, -0.4,    0, 0.5,  0.0, 0, 0, 1);
		hinge_body_legFrontRight= hingeConstraint_create(chest   , legFrontRight,  0.5, -0.5,  0.4,    0, 0.5,  0.0, 0, 0, 1);
		hinge_body_legBackLeft	= hingeConstraint_create(chest   , legBackLeft  , -0.5, -0.5, -0.4,    0, 0.5,  0.0, 0, 0, 1);
		hinge_body_legBackRight	= hingeConstraint_create(chest   , legBackRight , -0.5, -0.5,  0.4,    0, 0.5,  0.0, 0, 0, 1);
		hinge_body_tail			= hingeConstraint_create(chest   , tail         ,   -1,  0.4,    0,  0.5,   0,  0.0, 0, 0, 1);

		hinge_body_head.setLimit(-3.14/6, 3.14/6);
		hinge_head_muzzle.setLimit(0, 0);
		hinge_earLeft_head.setLimit(0, 0);
		hinge_earRight_head.setLimit(0, 0);
		hinge_body_legFrontLeft.setLimit(-3.14/2, 3.14/2);
		hinge_body_legFrontRight.setLimit(-3.14/2, 3.14/2);
		hinge_body_legBackLeft.setLimit(-3.14/2, 3.14/2);
		hinge_body_legBackRight.setLimit(-3.14/2, 3.14/2);
		hinge_body_tail.setLimit(-3.14/3, 3.14/3);

		hinge_body_legFrontLeft.enableMotor(true);
		hinge_body_legFrontLeft.setMaxMotorImpulse(2);
		hinge_body_legFrontRight.enableMotor(true);
		hinge_body_legFrontRight.setMaxMotorImpulse(2);
		hinge_body_legBackLeft.enableMotor(true);
		hinge_body_legBackLeft.setMaxMotorImpulse(2);
		hinge_body_legBackRight.enableMotor(true);
		hinge_body_legBackRight.setMaxMotorImpulse(2);



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
	mydog = new dog(0, 0, 0);
}



extern (C) void refreash(){
}



//------------------------------------------

/*

class dog{

	public:

	float dna[20][4] = {};


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
