import core.runtime;
import std.stdio;
import std.math;
import std.json;
import std.random;
import std.conv;

import japariSDK.japarilib;

Random rnd;

int dogNum = 30;

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
vec3[string] hingeAxis;
bool[string] useLimit;
float[string] limitLower;
float[string] limitUpper;
vec3[string] hingeObject1Position;
vec3[string] hingeObject2Position;




class chorodog{

	elementNode[string] parts;
	hingeConstraint[string] hinges;


	this(float x, float y, float z){
		spawn(createVec3(x, y, z));
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
							partsMass[s])));

		}

		foreach(s; hingeName){




			hinges[s] = hingeConstraint_create(parts[hingeObject1Name[s]], parts[hingeObject2Name[s]],
					hingeObject1Position[s], hingeObject2Position[s],
					hingeAxis[s]);
					//createVec3(0.0f, -1.0f, 0.0f));
			//hinges[s].setLimit(limitLower[s], limitUpper[s]);
			//hinges[s].enableMotor(true);
			//hinges[s].setMaxMotorImpulse(5);
		}

	}
}


//ApplicationInterface----------------------

extern (C) void init(){
	try{
		rt_init();
		Random(unpredictableSeed);
		writeln("chorodog.d loaded");



		//HACK コンパイル時にjsonStringにlowPolyTree.fpmの内容が代入される(要-Jオプション)
		auto jsonString = import("chorodog.fpm");

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
				hingePosition[name] = createVec3(elem["xpos"].floating, elem["ypos"].floating, elem["zpos"].floating);

				hingeAxis[name] = createVec3(elem["xaxs"].floating , elem["yaxs"].floating, elem["zaxs"].floating);

				if(elem["useLimit"].str == "True") useLimit[name] = true; else useLimit[name] = false;
				limitLower[name] = elem["limitLower"].floating;
				limitUpper[name] = elem["limitUpper"].floating;
				hingeObject1Position[name] = createVec3(elem["object1xpos"].floating, elem["object1ypos"].floating, elem["object1zpos"].floating);
				hingeObject2Position[name] = createVec3(elem["object2xpos"].floating, elem["object2ypos"].floating, elem["object2zpos"].floating);
				hingeObject1Name[name] = elem["object1"].str;
				hingeObject2Name[name] = elem["object2"].str;

			}
		}

		for (int i = 0; i < dogNum; i++){
			new chorodog(to!float(i)*5.0f, 0.0f, 0.0f);
		}


	}
	catch (Exception ex){
		writeln(ex.toString);
	}



}


extern (C) void tick(){
}



//------------------------------------------
