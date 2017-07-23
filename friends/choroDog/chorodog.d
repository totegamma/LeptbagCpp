import core.stdc.stdio;
import std.stdio;
import core.runtime;
import std.random;
import std.math;
import std.algorithm;

//Library-----------------------------------

extern (C++) {
	interface vec3{
	}
	interface quat{
	}
}

extern (C) {
	vec3 createVec3(float x, float y, float z);
	quat createQuat(float w, float x, float y, float z);
}

extern (C++) {
	interface cubeshapeObject{
		void destroy();
		float getXpos();
		float getYpos();
		float getZpos();
	}


}

extern (C) {
	cubeshapeObject cubeshape_create(vec3 position, vec3 size, quat rotation, float weight);
}

extern (C++) {
	interface hingeConstraint{
		void enableMotor(bool flag);
		void setLimit(float lower, float upper);
		void setMaxMotorImpulse(float power);
		void setMotorTarget(float angle, float duration);
		void destroy();	
	}
}

extern (C) {
	hingeConstraint hingeConstraint_create(cubeshapeObject cubeA, cubeshapeObject cubeB, vec3 positionA, vec3 positionB, vec3 axis);
}

//------------------------------------------

dog mydog;
dog[] doglist;
Random rnd;

const int numofdog = 2;
const int dnacol = 20;
const int dnarow = 4;

class dog{

	int tag = 0;

	float[4][20] dna;

	cubeshapeObject chestF;
	cubeshapeObject chestB;
	cubeshapeObject head;
	cubeshapeObject muzzle;
	cubeshapeObject earL;
	cubeshapeObject earR;
	cubeshapeObject legFLT;
	cubeshapeObject legFRT;
	cubeshapeObject legBLT;
	cubeshapeObject legBRT;
	cubeshapeObject tail;

	hingeConstraint hinge_chestF_head;
	hingeConstraint hinge_chestF_chestB;
	hingeConstraint hinge_head_muzzle;
	hingeConstraint hinge_earL_head;
	hingeConstraint hinge_earR_head;
	hingeConstraint hinge_chestF_legFLT;
	hingeConstraint hinge_chestF_legFRT;
	hingeConstraint hinge_chestB_legBLT;
	hingeConstraint hinge_chestB_legBRT;
	hingeConstraint hinge_chestB_tail;


	this(float x, float y, float z, bool initialDNA) {

		if(initialDNA == true){
			for(int col = 0; col < dnacol; col++){
				for(int row = 0; row < dnarow; row++){
					dna[col][row] = uniform(-PI/2, PI/2, rnd);
				}
			}
		}

		spawn(x, y, z);
	}



	void spawn(float x, float y, float z){
		//犬の体の構造を定義している
		//キューブで肉体を作る cubeshape::create(位置, 大きさ, 傾き, 重さ, 追加先物理世界);
		chestF		= cubeshape_create(createVec3(x+0.5,     y,     z),
				createVec3(0.5, 0.5, 0.5),			createQuat(1, 0, 0, 0), 1);
		chestB		= cubeshape_create(createVec3(x-0.5,     y,     z),
				createVec3(0.5, 0.5, 0.5),			createQuat(0.5, 0, 0, 0), 0.5);
		head			= cubeshape_create(createVec3(x+1.4,     y,     z),
				createVec3(0.35, 0.35, 0.35),	createQuat(1, 0, 0, 0),	 0.5);
		muzzle			= cubeshape_create(createVec3(x+2.1, y-0.2,     z),
				createVec3(0.18, 0.1, 0.1),	createQuat(1, 0, 0, 0),	 0.1);
		earL			= cubeshape_create(createVec3(x+1.4, y+0.5, z-0.2),
				createVec3(0.1, 0.1, 0.1),	createQuat(1, 0, 0, 0),	0.05);
		earR		= cubeshape_create(createVec3(x+1.4, y+0.5, z+0.2),
				createVec3(0.1, 0.1, 0.1),	createQuat(1, 0, 0, 0),	0.05);
		legFLT	= cubeshape_create(createVec3(x+0.5,   y-1, z-0.4),
				createVec3(0.1, 0.5, 0.1),	createQuat(1, 0, 0, 0),	 0.3);
		legFRT	= cubeshape_create(createVec3(x+0.5,   y-1, z+0.4),
				createVec3(0.1, 0.5, 0.1),	createQuat(1, 0, 0, 0),	 0.3);
		legBLT		= cubeshape_create(createVec3(x-0.5,   y-1, z-0.4),
				createVec3(0.1, 0.5, 0.1),	createQuat(1, 0, 0, 0),	 0.3);
		legBRT	= cubeshape_create(createVec3(x-0.5,   y-1, z+0.4),
				createVec3(0.1, 0.5, 0.1),	createQuat(1, 0, 0, 0),	 0.3);
		tail			= cubeshape_create(createVec3(x-1.5, y+0.4,     z),
				createVec3(0.5, 0.1, 0.1),	createQuat(1, 0, 0, 0),	 0.2);


		hinge_chestF_head			= hingeConstraint_create(chestF   , head         ,
				createVec3(   0.5,    0,    0), createVec3(-0.4,   0,    0), createVec3(0, 0, 1));
		hinge_chestF_chestB = hingeConstraint_create( chestF, chestB, 
				createVec3(-0.5, 0, 0), createVec3(0.5, 0, 0), createVec3(0, 1, 0));
		hinge_head_muzzle		= hingeConstraint_create(head    , muzzle       ,
				createVec3( 0.4, -0.2,    0), createVec3(-0.3,   0,    0), createVec3(0, 0, 1));
		hinge_earL_head		= hingeConstraint_create(earL , head         ,
				createVec3(   0, -0.1,    0), createVec3(   0, 0.4, -0.2), createVec3(0, 0, 1));
		hinge_earR_head		= hingeConstraint_create(earR, head         ,
				createVec3(   0, -0.1,    0), createVec3(   0, 0.4,  0.2), createVec3(0, 0, 1));

		hinge_chestF_legFLT = hingeConstraint_create(chestF   , legFLT ,
				createVec3( 0.5, -0.5, -0.49), createVec3(   0, 0.25,  0.0), createVec3(0, 0, 1));
		hinge_chestF_legFRT= hingeConstraint_create(chestF   , legFRT,
				createVec3( 0.5, -0.5,  0.49), createVec3(   0, 0.25,  0.0), createVec3(0, 0, 1));
		hinge_chestB_legBLT	= hingeConstraint_create(chestB   , legBLT  ,
				createVec3(-0.5, -0.5, -0.49), createVec3(   0, 0.25,  0.0), createVec3(0, 0, 1));
		hinge_chestB_legBRT	= hingeConstraint_create(chestB   , legBRT ,
				createVec3(-0.5, -0.5,  0.49), createVec3(   0, 0.25,  0.0), createVec3(0, 0, 1));

		hinge_chestB_tail			= hingeConstraint_create(chestB   , tail         ,
				createVec3(  -0.5,  0.4,    0), createVec3( 0.5,   0,  0.0), createVec3(0, 0, 1));


		hinge_chestF_head.setLimit(-PI/6, PI/6);
		hinge_chestF_chestB.setLimit(-PI/24.0, PI/24.0);
		hinge_head_muzzle.setLimit(0, 0);
		hinge_earL_head.setLimit(0, 0);
		hinge_earR_head.setLimit(0, 0);
		hinge_chestF_legFLT.setLimit(-PI/2, PI/2);
		hinge_chestF_legFRT.setLimit(-PI/2, PI/2);
		hinge_chestB_legBLT.setLimit(-PI/2, PI/2);
		hinge_chestB_legBRT.setLimit(-PI/2, PI/2);
		hinge_chestB_tail.setLimit(-PI/3, PI/3);

		hinge_chestF_chestB.enableMotor(true);
		hinge_chestF_chestB.setMaxMotorImpulse(2);

		hinge_chestF_legFLT.enableMotor(true);
		hinge_chestF_legFLT.setMaxMotorImpulse(2);
		hinge_chestF_legFRT.enableMotor(true);
		hinge_chestF_legFRT.setMaxMotorImpulse(2);
		hinge_chestB_legBLT.enableMotor(true);
		hinge_chestB_legBLT.setMaxMotorImpulse(2);
		hinge_chestB_legBRT.enableMotor(true);
		hinge_chestB_legBRT.setMaxMotorImpulse(2);
	}

	void move(int sequence){
		hinge_chestF_legFLT.setMotorTarget(dna[sequence][0], 0.3);
		hinge_chestF_legFRT.setMotorTarget(dna[sequence][1], 0.3);
		hinge_chestB_legBLT.setMotorTarget(dna[sequence][2], 0.3);
		hinge_chestB_legBRT.setMotorTarget(dna[sequence][3], 0.3);
	}


	void despawn(){
		chestF.destroy();
		chestB.destroy();
		head.destroy();
		muzzle.destroy();
		earL.destroy();
		earR.destroy();
		legFLT.destroy();
		legFRT.destroy();
		legBLT.destroy();
		legBRT.destroy();
		tail.destroy();
		hinge_chestF_head.destroy();
		hinge_chestF_chestB.destroy();
		hinge_head_muzzle.destroy();
		hinge_earL_head.destroy();
		hinge_earR_head.destroy();
		hinge_chestF_legFLT.destroy();
		hinge_chestF_legFRT.destroy();
		hinge_chestB_legBLT.destroy();
		hinge_chestB_legBRT.destroy();
		hinge_chestB_tail.destroy();

	}
}


//ApplicationInterface----------------------

extern (C) void init(){
try{
	rt_init();
	Random(unpredictableSeed);

	doglist.length = 100;

	foreach(int i, ref elem; doglist){
		elem = new dog(0, 1.5, -5*i, true);
	}
}
catch (Exception ex){
	writeln(ex.toString);
}
}


float topRecord = 0;
int timerDivisor = 0;
int time = 0;
int generation = 0;
int sequence = 0;

extern (C) void tick(){
	if(timerDivisor++ == 6){
		sequence = (sequence+1)%20;
		timerDivisor = 0;

		foreach(elem; doglist){
			elem.move(sequence);
		}

		time++;

	}

	//世代終わり
	if(time == 30 + generation*2){

		//成績順にソート
		sort!((a,b)=>a.muzzle.getXpos() < b.muzzle.getXpos())(doglist);

		//優秀なDNAをコピー
		float[4][20] firstDNA = doglist[$-1].dna;
		float[4][20] secondDNA = doglist[$-2].dna;

		//犬のリセット
		foreach(int i, ref elem; doglist){
			elem.despawn();
			elem = new dog(0, 1.5, -5*i, false);
		}

		//最初の2個体はさっきの優秀個体をそのまま動かす
		doglist[0].dna = firstDNA;
		doglist[1].dna = secondDNA;

		//残りの個体
		foreach(i; 2..100){
			//交配
			foreach(uint j, ref elem; doglist[i].dna[]){
				if(uniform(0, 2, rnd) == 0){
					elem = firstDNA[j];
				}else{
					elem = secondDNA[j];
				}
			}

			//突然変異
			int numOfAttack = uniform(0, 10, rnd);
				
			for(int j = 0; j < numOfAttack; j++){
				doglist[i].dna[uniform(0, dnacol, rnd)][uniform(0, dnarow, rnd)] = uniform(-PI/2, PI/2, rnd);
			}

		}

		time = 0;
	}





}



//------------------------------------------
