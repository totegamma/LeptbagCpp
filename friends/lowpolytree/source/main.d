import core.runtime;
import std.stdio;
import std.math;
import std.json;
import std.random;

import japarilib;
import dlib.math.vector;
import dlib.math.quaternion;

Random rnd;

vertexManager leafVertices;
vertexManager trunkVertices;

elementManager leaf;
elementManager trunk;

Vector3f leafPosition;
Vector3f leafScale;
Quaternionf leafRotation;

Vector3f trunkPosition;
Vector3f trunkScale;
Quaternionf trunkRotation;

class tree{

	this(float x, float y, float z) {
		spawn(Vector3f(x, y, z));
	}

	void spawn(Vector3f position){

		leaf.generate(paramWrap(
							param("position", leafPosition + position),
							param("scale",    leafScale),
							param("face",     leafRotation),
							param("rotation", Quaternionf(0, 0, 0, 1)),
							param("model",    leafVertices),
							param("mass",     0.0f)));

		trunk.generate(paramWrap(
							param("position", trunkPosition + position),
							param("scale",    trunkScale),
							param("face",     trunkRotation),
							param("rotation", Quaternionf(0, 0, 0, 1)),
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

				leafPosition = Vector3f(elem["xpos"].floating, elem["ypos"].floating, elem["zpos"].floating);
				leafScale    = Vector3f(elem["xscl"].floating, elem["yscl"].floating, elem["zscl"].floating);
				leafRotation = Quaternionf(elem["wqat"].floating, elem["xqat"].floating, elem["yqat"].floating, elem["zqat"].floating);

				foreach(objvertex; elem["vertex"].array){
					leafVertices.addVertex(createVertex(objvertex.array[0].floating, objvertex.array[1].floating, objvertex.array[2].floating,
												objvertex.array[3].floating, objvertex.array[4].floating, objvertex.array[5].floating,
												objvertex.array[6].floating, objvertex.array[7].floating, objvertex.array[8].floating));
				}

				leaf = createElementManager(leafVertices, &createConvexHullShapeBody);

			}else if(elem["name"].str == "trunk"){

				trunkPosition = Vector3f(elem["xpos"].floating, elem["ypos"].floating, elem["zpos"].floating);
				trunkScale    = Vector3f(elem["xscl"].floating, elem["yscl"].floating, elem["zscl"].floating);
				trunkRotation = Quaternionf(elem["wqat"].floating, elem["xqat"].floating, elem["yqat"].floating, elem["zqat"].floating);

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
