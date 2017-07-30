import core.runtime;
import std.stdio;
import std.math;
import std.json;
import std.random;
import std.conv;

import japariSDK.japarilib;

Random rnd;

int dogNum = 1;

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

			float axisx = 0.0f;
			float axisy = 1.0f;
			float axisz = 0.0f;

			float thetax = hingeAxis[s].getx();
			float thetay = hingeAxis[s].gety();
			float thetaz = hingeAxis[s].getz();

			axisy = axisy * cos(thetax) - axisz * sin(thetax);
			axisz = axisy * sin(thetax) + axisz * cos(thetax);

			axisx = axisx * cos(thetaz) - axisy * sin(thetaz);
			axisy = axisx * sin(thetaz) + axisy * cos(thetaz);

			axisx = axisx * cos(thetay) - axisz * sin(thetay);
			axisz = axisx * sin(thetay) + axisz * cos(thetay);

			/*
			float ob1posx;
			float ob1posy;
			float ob1posz;

			ob1posy = hingeObject1Position[s].gety() * cos(thetax) - hingeObject1Position[s].getz() * sin(thetax);
			ob1posz = hingeObject1Position[s].gety() * sin(thetax) + hingeObject1Position[s].getz() * cos(thetax);

			ob1posx = hingeObject1Position[s].getx() * cos(thetaz) - hingeObject1Position[s].gety() * sin(thetaz);
			ob1posy = hingeObject1Position[s].getx() * sin(thetaz) + hingeObject1Position[s].gety() * cos(thetaz);

			ob1posx = hingeObject1Position[s].getx() * cos(thetay) - hingeObject1Position[s].getz() * sin(thetay);
			ob1posz = hingeObject1Position[s].getx() * sin(thetay) + hingeObject1Position[s].getz() * cos(thetay);


			float ob2posx;
			float ob2posy;
			float ob2posz;

			ob2posy = hingeObject2Position[s].gety() * cos(thetax) - hingeObject2Position[s].getz() * sin(thetax);
			ob2posz = hingeObject2Position[s].gety() * sin(thetax) + hingeObject2Position[s].getz() * cos(thetax);


			ob2posx = hingeObject2Position[s].getx() * cos(thetaz) - hingeObject2Position[s].gety() * sin(thetaz);
			ob2posy = hingeObject2Position[s].getx() * sin(thetaz) + hingeObject2Position[s].gety() * cos(thetaz);

			ob2posx = hingeObject2Position[s].getx() * cos(thetay) - hingeObject2Position[s].getz() * sin(thetay);
			ob2posz = hingeObject2Position[s].getx() * sin(thetay) + hingeObject2Position[s].getz() * cos(thetay);


			hingeObject1Position[s] = createVec3(ob1posx, ob1posy, ob1posz);
			hingeObject2Position[s] = createVec3(ob2posx, ob2posy, ob2posz);
			*/





			hinges[s] = hingeConstraint_create(parts[hingeObject1Name[s]], parts[hingeObject2Name[s]],
					hingeObject1Position[s], hingeObject2Position[s],
					//partsPosition[hingeObject1Name[s]], partsPosition[hingeObject2Name[s]],
					//hingeAxis[s]);
					//createVec3(axisx, axisy, axisz));
					createVec3(0.0f, 0.0f, 1.0f));
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
				hingeAxis[name] = createVec3(elem["xrot"].floating , elem["yrot"].floating, elem["zrot"].floating);
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
