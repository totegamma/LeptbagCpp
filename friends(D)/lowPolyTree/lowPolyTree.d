import core.runtime;
import std.stdio;
import std.math;
import std.json;
import std.random;

import japariSDK.japarilib;

Random rnd;

vertexManager leafVertices;
vertexManager trunkVertices;

elementManager leaf;
elementManager trunk;

vec3 leafPosition;
vec3 leafScale;
quat leafRotation;

vec3 trunkPosition;
vec3 trunkScale;
quat trunkRotation;

class tree{

	this(float x, float y, float z) {
		spawn(createVec3(x, y, z));
	}

	void spawn(vec3 position){

		leaf.generate(paramWrap(
							param("position", addVec(leafPosition, position)),
							param("scale",    leafScale),
							param("face",     leafRotation),
							param("rotation", createQuat(1, 0, 0, 0)),
							param("model",    leafVertices),
							param("mass",     0.0f)));

		trunk.generate(paramWrap(
							param("position", addVec(trunkPosition, position)),
							param("scale",    trunkScale),
							param("face",     trunkRotation),
							param("rotation", createQuat(1, 0, 0, 0)),
							param("model",    trunkVertices),
							param("mass",     0.0f)));

	}
}


//ApplicationInterface----------------------

extern (C) void init(){
	try{
	rt_init();
	Random(unpredictableSeed);
	writeln("lowPolyTree.d loaded");


	leafVertices = createVertexManager();
	trunkVertices = createVertexManager();

	//HACK コンパイル時にjsonStringにlowPolyTree.fpmの内容が代入される(要-Jオプション)
	auto jsonString = import("lowPolyTree.fpm");

	JSONValue model = parseJSON(jsonString);

	foreach(elem; model.array){
		if(elem["objectType"].str == "MESH"){
			if(elem["name"].str == "leaf"){

				leafPosition = createVec3(elem["xpos"].floating, elem["ypos"].floating, elem["zpos"].floating);
				leafScale    = createVec3(elem["xscl"].floating, elem["yscl"].floating, elem["zscl"].floating);
				leafRotation = createQuat(elem["wqat"].floating, elem["xqat"].floating, elem["yqat"].floating, elem["zqat"].floating);

				foreach(objvertex; elem["vertex"].array){
					leafVertices.addVertex(createVertex(objvertex.array[0].floating, objvertex.array[1].floating, objvertex.array[2].floating,
												objvertex.array[3].floating, objvertex.array[4].floating, objvertex.array[5].floating,
												objvertex.array[6].floating, objvertex.array[7].floating, objvertex.array[8].floating));
				}

				leaf = createElementManager(leafVertices, &createConvexHullShapeBody);

			}else if(elem["name"].str == "trunk"){

				trunkPosition = createVec3(elem["xpos"].floating, elem["ypos"].floating, elem["zpos"].floating);
				trunkScale    = createVec3(elem["xscl"].floating, elem["yscl"].floating, elem["zscl"].floating);
				trunkRotation = createQuat(elem["wqat"].floating, elem["xqat"].floating, elem["yqat"].floating, elem["zqat"].floating);

				foreach(objvertex; elem["vertex"].array){
					trunkVertices.addVertex(createVertex(objvertex.array[0].floating, objvertex.array[1].floating, objvertex.array[2].floating,
													objvertex.array[3].floating, objvertex.array[4].floating, objvertex.array[5].floating,
													objvertex.array[6].floating, objvertex.array[7].floating, objvertex.array[8].floating));
				}
				
				trunk = createElementManager(trunkVertices, &createConvexHullShapeBody);

			}
		}
	}


	for (int i = 0; i < 100; i++){
		new tree(uniform(-30, 30, rnd), 0, uniform(-30, 30, rnd));
	}

	
	}
	catch (Exception ex){
		writeln(ex.toString);
	}



}


extern (C) void tick(){
}



//------------------------------------------
