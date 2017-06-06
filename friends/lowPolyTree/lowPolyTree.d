import core.runtime;
import std.stdio;
import std.math;
import std.json;
import std.random;

//Library-----------------------------------

extern (C++) {
	interface vec3{
		float getx();
		float gety();
		float getz();
	}
	interface quat{
		float getw();
		float getx();
		float gety();
		float getz();
	}
}

vec3 addVec(vec3 A, vec3 B){
	return createVec3(A.getx() + B.getx(), A.gety() + B.gety(), A.getz() + B.getz());
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
		leaf.create(addVec(leafPosition, position), leafScale, leafRotation, 0);
		trunk.create(addVec(trunkPosition, position), trunkScale, trunkRotation, 0);
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


				leafPosition = createVec3(elem["xpos"].floating, elem["ypos"].floating, elem["zpos"].floating);
				leafScale    = createVec3(elem["xscl"].floating, elem["yscl"].floating, elem["zscl"].floating);
				leafRotation = createQuat(elem["wqat"].floating, elem["xqat"].floating, elem["yqat"].floating, elem["zqat"].floating);

				foreach(objvertex; elem["vertex"].array){
					leaf.addVertex(createVertex(objvertex.array[0].floating, objvertex.array[1].floating, objvertex.array[2].floating,
												objvertex.array[3].floating, objvertex.array[4].floating, objvertex.array[5].floating,
												objvertex.array[6].floating, objvertex.array[7].floating, objvertex.array[8].floating));
				}
				leaf.registerToSystem();
			}else if(elem["name"].str == "trunk"){

				trunkPosition = createVec3(elem["xpos"].floating, elem["ypos"].floating, elem["zpos"].floating);
				trunkScale    = createVec3(elem["xscl"].floating, elem["yscl"].floating, elem["zscl"].floating);
				trunkRotation = createQuat(elem["wqat"].floating, elem["xqat"].floating, elem["yqat"].floating, elem["zqat"].floating);

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
