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

commonshapeObject plank;




//ApplicationInterface----------------------

extern (C) void init(){
	try{
	rt_init();

	float r = 0.5;
	float g = 0.3;
	float b = 0.2;

	plank = createCommonShapeObject();
	plank.addVertex(createVertex(-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  r,  g,  b));
	plank.addVertex(createVertex(-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  r,  g,  b));
	plank.addVertex(createVertex(-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  r,  g,  b));
	plank.addVertex(createVertex(-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  r,  g,  b));
	plank.addVertex(createVertex(-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  r,  g,  b));
	plank.addVertex(createVertex(-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  r,  g,  b));
	plank.addVertex(createVertex( 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  r,  g,  b));
	plank.addVertex(createVertex(-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  r,  g,  b));
	plank.addVertex(createVertex( 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  r,  g,  b));
	plank.addVertex(createVertex( 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  r,  g,  b));
	plank.addVertex(createVertex(-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  r,  g,  b));
	plank.addVertex(createVertex(-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  r,  g,  b));
	plank.addVertex(createVertex( 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  r,  g,  b));
	plank.addVertex(createVertex(-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  r,  g,  b));
	plank.addVertex(createVertex(-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  r,  g,  b));
	plank.addVertex(createVertex( 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  r,  g,  b));
	plank.addVertex(createVertex( 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  r,  g,  b));
	plank.addVertex(createVertex(-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  r,  g,  b));
	plank.addVertex(createVertex( 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  r,  g,  b));
	plank.addVertex(createVertex( 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  r,  g,  b));
	plank.addVertex(createVertex( 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  r,  g,  b));
	plank.addVertex(createVertex( 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  r,  g,  b));
	plank.addVertex(createVertex( 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  r,  g,  b));
	plank.addVertex(createVertex( 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  r,  g,  b));
	plank.addVertex(createVertex( 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  r,  g,  b));
	plank.addVertex(createVertex( 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  r,  g,  b));
	plank.addVertex(createVertex(-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  r,  g,  b));
	plank.addVertex(createVertex( 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  r,  g,  b));
	plank.addVertex(createVertex(-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  r,  g,  b));
	plank.addVertex(createVertex(-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  r,  g,  b));
	plank.addVertex(createVertex(-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  r,  g,  b));
	plank.addVertex(createVertex(-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  r,  g,  b));
	plank.addVertex(createVertex( 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  r,  g,  b));
	plank.addVertex(createVertex( 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  r,  g,  b));
	plank.addVertex(createVertex(-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  r,  g,  b));
	plank.addVertex(createVertex( 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  r,  g,  b));

	plank.registerToSystem();

	plank.create(createVec3(0, 10, 0), createVec3(1, 1, 1), createQuat(1, 0, 0, 0), 1);

	}
	catch (Exception ex){
		writeln(ex.toString);
	}
}


extern (C) void tick(){
}



//------------------------------------------
