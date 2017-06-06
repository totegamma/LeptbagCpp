import core.runtime;
import std.stdio;
import std.math;
import std.json;
import std.random;

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

extern (C++){
	interface vertex{
	}
}

extern (C++){
	interface shapePointerObject{
	}
}

extern (C++){
	interface commonshapeObject{
		void addVertex(vertex newvertex);
		void registerToSystem();
		shapePointerObject create();
		shapePointerObject create(vec3 position, vec3 size, quat rotation);
		shapePointerObject create(vec3 position, vec3 size, quat rotation, float mass);
	}
}

extern (C) {
	vertex createVertex(float coordinate_x, float coordinate_y, float coordinate_z, float normal_x, float normal_y, float normal_z, float color_r, float color_g, float color_b);
	commonshapeObject createCommonShapeObject();
}

//------------------------------------------

Random rnd;

commonshapeObject leaf;
commonshapeObject trunk;

float leafXpos;
float leafYpos;
float leafZpos;
float leafWquat;
float leafXquat;
float leafYquat;
float leafZquat;
float leafXscl;
float leafYscl;
float leafZscl;

float trunkXpos;
float trunkYpos;
float trunkZpos;
float trunkWquat;
float trunkXquat;
float trunkYquat;
float trunkZquat;
float trunkXscl;
float trunkYscl;
float trunkZscl;

class tree{

	this(float x, float y, float z) {
		spawn(x, y, z);
	}



	void spawn(float x, float y, float z){
		leaf.create(leafXpos + x, leafYpos + y, leafZpos + z, leafXscl, leafYscl, leafZscl, leafWquat, leafXquat, leafYquat, leafZquat, 0);
		trunk.create(trunkXpos + x, trunkYpos + y, trunkZpos + z, trunkXscl, trunkYscl, trunkZscl, trunkWquat, trunkXquat, trunkYquat, trunkZquat, 0);
	}
}


//ApplicationInterface----------------------

extern (C) void init(){
	try{
	rt_init();
	Random(unpredictableSeed);
	writeln("lowPolyTree.d loaded");

	leaf = createCommonShapeObject();
	trunk = createCommonShapeObject();

	//HACK コンパイル時にjsonStringにlowPolyTree.fpmの内容が代入される(要-Jオプション)
	auto jsonString = import("lowPolyTree.fpm");

	JSONValue model = parseJSON(jsonString);

	foreach(elem; model.array){
		if(elem["objectType"].str == "MESH"){
			if(elem["name"].str == "leaf"){


				leafXpos  = elem["xpos"].floating;
				leafYpos  = elem["ypos"].floating;
				leafZpos  = elem["zpos"].floating;
				leafWquat = elem["wqat"].floating;
				leafXquat = elem["xqat"].floating;
				leafYquat = elem["yqat"].floating;
				leafZquat = elem["zqat"].floating;
				leafXscl  = elem["xscl"].floating;
				leafYscl  = elem["yscl"].floating;
				leafZscl  = elem["zscl"].floating;

				foreach(objvertex; elem["vertex"].array){
					leaf.addVertex(createVertex(objvertex.array[0].floating, objvertex.array[1].floating, objvertex.array[2].floating,
												objvertex.array[3].floating, objvertex.array[4].floating, objvertex.array[5].floating,
												objvertex.array[6].floating, objvertex.array[7].floating, objvertex.array[8].floating));
				}
				leaf.registerToSystem();
			}else if(elem["name"].str == "trunk"){

				trunkXpos  = elem["xpos"].floating;
				trunkYpos  = elem["ypos"].floating;
				trunkZpos  = elem["zpos"].floating;
				trunkWquat = elem["wqat"].floating;
				trunkXquat = elem["xqat"].floating;
				trunkYquat = elem["yqat"].floating;
				trunkZquat = elem["zqat"].floating;
				trunkXscl  = elem["xscl"].floating;
				trunkYscl  = elem["yscl"].floating;
				trunkZscl  = elem["zscl"].floating;

				foreach(objvertex; elem["vertex"].array){
					trunk.addVertex(createVertex(objvertex.array[0].floating, objvertex.array[1].floating, objvertex.array[2].floating,
													objvertex.array[3].floating, objvertex.array[4].floating, objvertex.array[5].floating,
													objvertex.array[6].floating, objvertex.array[7].floating, objvertex.array[8].floating));
				}
				trunk.registerToSystem();
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
