import core.stdc.stdio;
import std.stdio;
import core.runtime;
import std.random;
import std.math;
import std.algorithm;
import std.conv;

import japariSDK.japarilib;
import dlib.math.vector;
import dlib.math.quaternion;

dog mydog;
dog[] doglist;
Random rnd;

const int numofdog = 2;
const int dnacol = 20;
const int dnarow = 4;

class dog{

	int tag = 0;

	float[4][20] dna;

	elementNode chest;
	elementNode head;
	elementNode muzzle;
	elementNode earLeft;
	elementNode earRight;
	elementNode legFrontLeft;
	elementNode legFrontRight;
	elementNode legBackLeft;
	elementNode legBackRight;
	elementNode tail;

	hingeConstraint hinge_body_head;
	hingeConstraint hinge_head_muzzle;
	hingeConstraint hinge_earLeft_head;
	hingeConstraint hinge_earRight_head;
	hingeConstraint hinge_body_legFrontLeft;
	hingeConstraint hinge_body_legFrontRight;
	hingeConstraint hinge_body_legBackLeft;
	hingeConstraint hinge_body_legBackRight;
	hingeConstraint hinge_body_tail;


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

		chest = getCubeshape().generate(paramWrap(
					param("position", Vector3f(    x,     y,     z)),
					param("scale",    Vector3f(  1, 0.5, 0.5)),
					param("rotation", Quaternionf(0, 0, 0, 1)),
					param("mass", 2.0f)));

		head = getCubeshape().generate(paramWrap(
					param("position", Vector3f(x+1.4,     y,     z)),
					param("scale", Vector3f(0.4, 0.4, 0.4)),
					param("rotation", Quaternionf(0, 0, 0, 1)),
					param("mass", 0.5f)));

		muzzle = getCubeshape().generate(paramWrap(
					param("position", Vector3f(x+2.1, y-0.2,     z)),
					param("scale", Vector3f(0.3, 0.2, 0.2)),
					param("rotation", Quaternionf(0, 0, 0, 1)),
					param("mass", 0.1f)));

		earLeft = getCubeshape().generate(paramWrap(
					param("position", Vector3f(x+1.4, y+0.5, z-0.2)),
					param("scale", Vector3f(0.1, 0.1, 0.1)),
					param("rotation", Quaternionf(0, 0, 0, 1)),
					param("mass", 0.05f)));

		earRight = getCubeshape().generate(paramWrap(
					param("position", Vector3f(x+1.4, y+0.5, z+0.2)),
					param("scale", Vector3f(0.1, 0.1, 0.1)),
					param("rotation", Quaternionf(0, 0, 0, 1)),
					param("mass", 0.05f)));

		legFrontLeft = getCubeshape().generate(paramWrap(
					param("position", Vector3f(x+0.5,   y-1, z-0.4)),
					param("scale", Vector3f(0.1, 0.5, 0.1)),
					param("rotation", Quaternionf(0, 0, 0, 1)),
					param("mass", 0.3f)));

		legFrontRight = getCubeshape().generate(paramWrap(
					param("position", Vector3f(x+0.5,   y-1, z+0.4)),
					param("scale", Vector3f(0.1, 0.5, 0.1)),
					param("rotation", Quaternionf(0, 0, 0, 1)),
					param("mass", 0.3f)));

		legBackLeft = getCubeshape().generate(paramWrap(
					param("position", Vector3f(x-0.5,   y-1, z-0.4)),
					param("scale", Vector3f(0.1, 0.5, 0.1)),
					param("rotation", Quaternionf(0, 0, 0, 1)),
					param("mass", 0.3f)));

		legBackRight = getCubeshape().generate(paramWrap(
					param("position", Vector3f(x-0.5,   y-1, z+0.4)),
					param("scale", Vector3f(0.1, 0.5, 0.1)),
					param("rotation", Quaternionf(0, 0, 0, 1)),
					param("mass", 0.3f)));

		tail = getCubeshape().generate(paramWrap(
					param("position", Vector3f(x-1.5, y+0.4,     z)),
					param("scale", Vector3f(0.5, 0.1, 0.1)),
					param("rotation", Quaternionf(0, 0, 0, 1)),
					param("mass", 0.2f)));



		hinge_body_head			= new hingeConstraint(chest   , head         , Vector3f(   1,    0,    0), Vector3f(-0.4,   0,    0), Vector3f(0, 0, 1));
		hinge_head_muzzle		= new hingeConstraint(head    , muzzle       , Vector3f( 0.4, -0.2,    0), Vector3f(-0.3,   0,    0), Vector3f(0, 0, 1));
		hinge_earLeft_head		= new hingeConstraint(earLeft , head         , Vector3f(   0, -0.1,    0), Vector3f(   0, 0.4, -0.2), Vector3f(0, 0, 1));
		hinge_earRight_head		= new hingeConstraint(earRight, head         , Vector3f(   0, -0.1,    0), Vector3f(   0, 0.4,  0.2), Vector3f(0, 0, 1));
		hinge_body_legFrontLeft = new hingeConstraint(chest   , legFrontLeft , Vector3f( 0.5, -0.5, -0.4), Vector3f(   0, 0.5,  0.0), Vector3f(0, 0, 1));
		hinge_body_legFrontRight= new hingeConstraint(chest   , legFrontRight, Vector3f( 0.5, -0.5,  0.4), Vector3f(   0, 0.5,  0.0), Vector3f(0, 0, 1));
		hinge_body_legBackLeft	= new hingeConstraint(chest   , legBackLeft  , Vector3f(-0.5, -0.5, -0.4), Vector3f(   0, 0.5,  0.0), Vector3f(0, 0, 1));
		hinge_body_legBackRight	= new hingeConstraint(chest   , legBackRight , Vector3f(-0.5, -0.5,  0.4), Vector3f(   0, 0.5,  0.0), Vector3f(0, 0, 1));
		hinge_body_tail			= new hingeConstraint(chest   , tail         , Vector3f(  -1,  0.4,    0), Vector3f( 0.5,   0,  0.0), Vector3f(0, 0, 1));

		hinge_body_head.setLimit(-PI/6, PI/6);
		hinge_head_muzzle.setLimit(0, 0);
		hinge_earLeft_head.setLimit(0, 0);
		hinge_earRight_head.setLimit(0, 0);
		hinge_body_legFrontLeft.setLimit(-PI/2, PI/2);
		hinge_body_legFrontRight.setLimit(-PI/2, PI/2);
		hinge_body_legBackLeft.setLimit(-PI/2, PI/2);
		hinge_body_legBackRight.setLimit(-PI/2, PI/2);
		hinge_body_tail.setLimit(-PI/3, PI/3);

		hinge_body_legFrontLeft.enableMotor(true);
		hinge_body_legFrontLeft.setMaxMotorImpulse(2);
		hinge_body_legFrontRight.enableMotor(true);
		hinge_body_legFrontRight.setMaxMotorImpulse(2);
		hinge_body_legBackLeft.enableMotor(true);
		hinge_body_legBackLeft.setMaxMotorImpulse(2);
		hinge_body_legBackRight.enableMotor(true);
		hinge_body_legBackRight.setMaxMotorImpulse(2);
	}

	void move(int sequence){
		hinge_body_legFrontLeft.setMotorTarget(dna[sequence][0], 0.3);
		hinge_body_legFrontRight.setMotorTarget(dna[sequence][1], 0.3);
		hinge_body_legBackLeft.setMotorTarget(dna[sequence][2], 0.3);
		hinge_body_legBackRight.setMotorTarget(dna[sequence][3], 0.3);
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
		hinge_body_head.destroy();
		hinge_head_muzzle.destroy();
		hinge_earLeft_head.destroy();
		hinge_earRight_head.destroy();
		hinge_body_legFrontLeft.destroy();
		hinge_body_legFrontRight.destroy();
		hinge_body_legBackLeft.destroy();
		hinge_body_legBackRight.destroy();
		hinge_body_tail.destroy();

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

		//新記録を更新したDNAを表示
		if(topRecord < doglist[$-1].muzzle.getXpos()){
			topRecord = doglist[$-1].muzzle.getXpos();
			writeln("New Record!: " ~ to!string(topRecord));
			writeln("dna:");
			foreach(float[4] elem; doglist[$-1].dna){
				writeln(to!string(elem[0]) ~ ", " ~ to!string(elem[1]) ~ ", " ~ to!string(elem[2]) ~ ", " ~ to!string(elem[3]));
			}
		}

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

		generation++;
		time = 0;

		writeln("generation: " ~ to!string(generation));
	}





}



//------------------------------------------
