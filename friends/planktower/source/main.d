import core.runtime;
import std.stdio;
import std.math;
import std.json;
import std.random;


import japarilib;
import dlib.math.vector;
import dlib.math.quaternion;


elementManager plank;
vertexManager plankVertex;


//ApplicationInterface----------------------

extern (C) void init(){
	try{
	rt_init();

	float r = 0.5;
	float g = 0.3;
	float b = 0.2;

	plankVertex = createVertexManager();
	plankVertex.addVertex(createVertex(-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  r,  g,  b));
	plankVertex.addVertex(createVertex(-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  r,  g,  b));
	plankVertex.addVertex(createVertex(-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  r,  g,  b));
	plankVertex.addVertex(createVertex(-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  r,  g,  b));
	plankVertex.addVertex(createVertex(-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  r,  g,  b));
	plankVertex.addVertex(createVertex(-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  r,  g,  b));
	plankVertex.addVertex(createVertex( 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  r,  g,  b));
	plankVertex.addVertex(createVertex(-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  r,  g,  b));
	plankVertex.addVertex(createVertex( 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  r,  g,  b));
	plankVertex.addVertex(createVertex( 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  r,  g,  b));
	plankVertex.addVertex(createVertex(-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  r,  g,  b));
	plankVertex.addVertex(createVertex(-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  r,  g,  b));
	plankVertex.addVertex(createVertex( 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  r,  g,  b));
	plankVertex.addVertex(createVertex(-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  r,  g,  b));
	plankVertex.addVertex(createVertex(-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  r,  g,  b));
	plankVertex.addVertex(createVertex( 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  r,  g,  b));
	plankVertex.addVertex(createVertex( 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  r,  g,  b));
	plankVertex.addVertex(createVertex(-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  r,  g,  b));
	plankVertex.addVertex(createVertex( 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  r,  g,  b));
	plankVertex.addVertex(createVertex( 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  r,  g,  b));
	plankVertex.addVertex(createVertex( 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  r,  g,  b));
	plankVertex.addVertex(createVertex( 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  r,  g,  b));
	plankVertex.addVertex(createVertex( 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  r,  g,  b));
	plankVertex.addVertex(createVertex( 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  r,  g,  b));
	plankVertex.addVertex(createVertex( 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  r,  g,  b));
	plankVertex.addVertex(createVertex( 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  r,  g,  b));
	plankVertex.addVertex(createVertex(-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  r,  g,  b));
	plankVertex.addVertex(createVertex( 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  r,  g,  b));
	plankVertex.addVertex(createVertex(-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  r,  g,  b));
	plankVertex.addVertex(createVertex(-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  r,  g,  b));
	plankVertex.addVertex(createVertex(-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  r,  g,  b));
	plankVertex.addVertex(createVertex(-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  r,  g,  b));
	plankVertex.addVertex(createVertex( 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  r,  g,  b));
	plankVertex.addVertex(createVertex( 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  r,  g,  b));
	plankVertex.addVertex(createVertex(-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  r,  g,  b));
	plankVertex.addVertex(createVertex( 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  r,  g,  b));

	plank = createElementManager(plankVertex, &createBoxBody);



	for(int x = 0; x < 4; x++){
		for(int z = 0; z < 4; z++){
			for(int y = 0; y < 20; y++){

				plank.generate(paramWrap(
							param("position", Vector3f(0.1 + 2*x, 0.2 + 0.8*y, 1 + 2*z)),
							param("scale", Vector3f(0.1, 0.2, 1)),
							param("rotation", Quaternionf(0, 0, 0, 1)),
							param("mass", 0.5f)));

				plank.generate(paramWrap(
							param("position", Vector3f(1.9 + 2*x, 0.2 + 0.8*y, 1 + 2*z)),
							param("scale", Vector3f(0.1, 0.2, 1)),
							param("rotation", Quaternionf(0, 0, 0, 1)),
							param("mass", 0.5f)));

				plank.generate(paramWrap(
							param("position", Vector3f(1 + 2*x, 0.6 + 0.8*y, 0.1 + 2*z)),
							param("scale", Vector3f(1, 0.2, 0.1)),
							param("rotation", Quaternionf(0, 0, 0, 1)),
							param("mass", 0.5f)));

				plank.generate(paramWrap(
							param("position", Vector3f(1 + 2*x, 0.6 + 0.8*y, 1.9 + 2*z)),
							param("scale", Vector3f(1, 0.2, 0.1)),
							param("rotation", Quaternionf(0, 0, 0, 1)),
							param("mass", 0.5f)));

			}
		}
	}


	}
	catch (Exception ex){
		writeln(ex.toString);
	}
}


extern (C) void tick(){
}



//------------------------------------------
