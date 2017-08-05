import core.runtime;
import std.stdio;
import std.math;
import std.json;
import std.random;
import std.conv;
import std.algorithm;

import japariSDK.japarilib;
import libGA;

Random rnd;

//strategy 1:DE, 2:simple GA
int strategy = 2;

string measuredPart = "head";
int dogNum = 80;
int attackProbability = 30;
float bodyMass = 42.592;

chorodog[] chorodogs;

vertexManager[string] partsVertices;
elementManager[string] partsGenerator;
vec3[string] partsPosition;
vec3[string] partsScale;
quat[string] partsRotation;
float[string] partsMass;


string[] hingeName;
string[string] hingeObject1Name;
string[string] hingeObject2Name;
vec3[string] hingePosition;
vec3[string] hingeAxis1;
vec3[string] hingeAxis2;
bool[string] hingeEnabled;
bool[string] useLimit;
float[string] limitLower;
float[string] limitUpper;
vec3[string] hingeObject1Position;
vec3[string] hingeObject2Position;




class chorodog{

	elementNode[string] parts;
	hingeConstraint[string] hinges;

	float[string][20] dna;

	this(float x, float y, float z, bool initialDNA){

		spawn(createVec3(x, y, z));

		if(initialDNA == true){
			foreach(string s, hinge; hinges){
				if(hingeEnabled[s]){
					for(int row = 0; row < 20; row++){
						dna[row][s] = uniform(-PI/2, PI/2, rnd);
					}
				}
			}
		}

	}

	void spawn(vec3 position){


		foreach(string s, elementManager partsGen; partsGenerator){

			parts[s] = partsGen.generate(paramWrap(
						param("position", addVec(partsPosition[s], position)),
						param("scale",    partsScale[s]),
						param("rotation", partsRotation[s]),
						param("model",    partsVertices[s]),
						param("mass", 
							//0.0f)));
							partsMass[s] * bodyMass)));

		}

		foreach(s; hingeName){
			hinges[s] = hingeConstraint_create(parts[hingeObject1Name[s]], parts[hingeObject2Name[s]],
					hingeObject1Position[s], hingeObject2Position[s],
					hingeAxis1[s], hingeAxis2[s]);
			hinges[s].setLimit(limitLower[s], limitUpper[s]);
			if(hingeEnabled[s]){
				hinges[s].enableMotor(true);
				hinges[s].setMaxMotorImpulse(5);
			}
		}

	}

	void move(int sequence){
		foreach(string s, hinge; hinges){
			if(hingeEnabled[s]){
				float target = abs(limitLower[s]-limitUpper[s]) * dna[sequence][s] * 2.0/PI;
				hinge.setMotorTarget(target, 0.5);
			}

		}
	}

	void despawn(){
		foreach(part; parts) part.destroy();
		foreach(hinge; hinges) hinge.destroy();
	}


}


//ApplicationInterface----------------------

extern (C) void init(){
	try{
		rt_init();
		Random(unpredictableSeed);
		writeln("chorodog.d loaded");



		//HACK コンパイル時にjsonStringにlowPolyTree.fpmの内容が代入される(要-Jオプション)
		auto jsonString = import("models/chorodog.fpm");
		//auto jsonString = import("models/chorodog_simplified.fpm");
		//auto jsonString = import("hingeTest.fpm");

		JSONValue model = parseJSON(jsonString);

		foreach(elem; model.array){
			if(elem["objectType"].str == "MESH"){
				string name = elem["name"].str;

				partsPosition[name] = createVec3(elem["xpos"].floating, elem["ypos"].floating, elem["zpos"].floating);
				partsScale[name]	= createVec3(elem["xscl"].floating, elem["yscl"].floating, elem["zscl"].floating);
				partsRotation[name] = createQuat(elem["wqat"].floating, elem["xqat"].floating, elem["yqat"].floating, elem["zqat"].floating);
				partsMass[name] = elem["mass"].floating;

				partsVertices[name] = createVertexManager();

				foreach(objvertex; elem["vertex"].array){
					partsVertices[name].addVertex(createVertex(objvertex.array[0].floating, objvertex.array[1].floating, objvertex.array[2].floating,
								objvertex.array[3].floating, objvertex.array[4].floating, objvertex.array[5].floating,
								objvertex.array[6].floating, objvertex.array[7].floating, objvertex.array[8].floating));
				}

				partsGenerator[name] = createElementManager(partsVertices[name], &createConvexHullShapeBody);


			}
		}

		foreach(elem; model.array){
			if(elem["objectType"].str == "constraint"){
				string name = elem["name"].str;

				hingeName ~= name;

				hingeAxis1[name] = createVec3(elem["xaxs1"].floating , elem["yaxs1"].floating, elem["zaxs1"].floating);
				hingeAxis2[name] = createVec3(elem["xaxs2"].floating , elem["yaxs2"].floating, elem["zaxs2"].floating);


				if(elem["enabled"].str == "True") hingeEnabled[name] = true; else hingeEnabled[name] = false;
				if(elem["useLimit"].str == "True") useLimit[name] = true; else useLimit[name] = false;
				limitLower[name] = elem["limitLower"].floating;
				limitUpper[name] = elem["limitUpper"].floating;
				hingeObject1Position[name] = createVec3(elem["object1xpos"].floating, elem["object1ypos"].floating, elem["object1zpos"].floating);
				hingeObject2Position[name] = createVec3(elem["object2xpos"].floating, elem["object2ypos"].floating, elem["object2zpos"].floating);
				hingeObject1Name[name] = elem["object1"].str;
				hingeObject2Name[name] = elem["object2"].str;

			}
		}

		chorodogs.length = dogNum;

		foreach(int i, ref elem; chorodogs) elem = new chorodog(to!float(i)*5.0f, 0.0f, -1.0f, true);
		


	}
	catch (Exception ex){
		writeln(ex.toString);
	}



}

bool evaluation = false;
float topRecord = 128.0;
int timerDivisor = 0;
int time = 0;
int generation = 0;
int sequence = 0;
float[] preRecords;
float[string][20][] preDNAs;
chorodog[] evaluateds;

extern (C) void tick(){

	if(timerDivisor++ == 6){
		sequence = (sequence+1)%20;
		timerDivisor = 0;

		if(!evaluation) foreach(elem; chorodogs) elem.move(sequence);
		else foreach(elem; evaluateds) elem.move(sequence);

		time++;
	}

	//世代終わり
	if(time == 30 + generation*2){


		float proRecordTmp = topRecord;

		if(!evaluation){
			generation++;
			writeln("end generation: " ~ to!string(generation));
		}else{
			writeln("end evaluating to generation: " ~ to!string(generation));
		}

		time = 0;

		switch(strategy){

			//DE
			case 1:

				if(!evaluation){

					preRecords.length = chorodogs.length;
					evaluateds.length = chorodogs.length;
					preDNAs.length = chorodogs.length;

					foreach(int i, dog; chorodogs){
						preRecords[i] = dog.parts[measuredPart].getZpos();
						preDNAs[i] = dog.dna;
					}

					//犬のリセット
					foreach(int i, ref elem; chorodogs){
						if(proRecordTmp>elem.parts[measuredPart].getZpos()) proRecordTmp = elem.parts[measuredPart].getZpos();
						elem.despawn();
					}
					float ditherF = uniform(0.5f, 1.0f, rnd);
					evaluateds = diffEvo( chorodogs, 0.9f, ditherF);
					evaluation = true;

				}else{
					//犬のリセット
					foreach(int i, ref elem; chorodogs){
						if(proRecordTmp>evaluateds[i].parts[measuredPart].getZpos()) proRecordTmp = evaluateds[i].parts[measuredPart].getZpos();
						elem = new chorodog(to!float(i)*5.0f, 0.0f, 0.0f, true);
						if(evaluateds[i].parts[measuredPart].getZpos() <= preRecords[i]) elem.dna = evaluateds[i].dna;
						else elem.dna = preDNAs[i];
					}
					foreach(int i, ref elem; evaluateds) elem.despawn();
					evaluation = false;
				}

				if(proRecordTmp<topRecord){
					topRecord = proRecordTmp;
					writeln("new record! : ", -1.0*topRecord);
				}


				break;


				//simple GA
			case 2:

				//前回の優秀個体を残す
				float[string][20] pre1FirstDNA = chorodogs[0].dna;
				float[string][20] pre1SecondDNA = chorodogs[5].dna;
				float[string][20] pre1ThirdDNA = chorodogs[10].dna;


				//成績順にソート
				sort!((a,b)=>a.parts[measuredPart].getZpos() > b.parts[measuredPart].getZpos())(chorodogs);
				//優秀なDNAをコピー
				float[string][20] firstDNA = chorodogs[$-1].dna;
				float[string][20] secondDNA = chorodogs[$-2].dna;
				float[string][20] thirdDNA = chorodogs[$-3].dna;

				//新記録を更新したDNAを表示
				if(topRecord > chorodogs[$-1].parts[measuredPart].getZpos()){
					topRecord = -1.0 * chorodogs[$-1].parts[measuredPart].getZpos();
					writeln("New Proceeding Record!: " ~ to!string(topRecord));
				}


				//犬のリセット
				foreach(elem; chorodogs) elem.despawn();
				foreach(int i, ref elem; chorodogs){
					elem = new chorodog(to!float(i)*5.0f, 0.0f, -1.0f, true);
				}

				foreach(int j, dog; chorodogs){

					//最初の2個体はさっきの優秀個体をそのまま動かす
					if( (j >= 0)&&(j < 5)) dog.dna = firstDNA;
					else if( (j >= 5)&&(j < 10) ) dog.dna = secondDNA;
					else if( (j >= 10)&&(j < 15)) dog.dna = thirdDNA;
					else if( (j >= 15)&&(j < 20) ) dog.dna = pre1FirstDNA;
					else if( (j >= 20)&&(j < 25) ) dog.dna = pre1SecondDNA;
					else if( (j >= 25)&&(j < 30) ) dog.dna = pre1ThirdDNA;

					else if(j >= 30){
						//交配
						foreach(int i, dnas; dog.dna){
							foreach(string s, eachdna; dnas){
								if(uniform(0, 2, rnd) == 0) eachdna = firstDNA[i][s];
								else eachdna = secondDNA[i][s];

								//突然変異
								int proOfAttack = uniform(0, 101, rnd);

								if(proOfAttack > attackProbability) eachdna = uniform(-PI/2, PI/2, rnd);
							}
						}
					}

				}

				break;

			default: assert(0);

		}



	}




}



//------------------------------------------
