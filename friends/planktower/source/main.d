import core.runtime;
import std.stdio;
import std.math;
import std.json;
import std.random;


import leptbagSDK.leptbaglib;
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

	plankVertex = new vertexManager();

	plankVertex.addVertex(-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  r,  g,  b);
	plankVertex.addVertex(-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  r,  g,  b);
	plankVertex.addVertex(-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  r,  g,  b);
	plankVertex.addVertex(-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  r,  g,  b);
	plankVertex.addVertex(-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  r,  g,  b);
	plankVertex.addVertex(-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  r,  g,  b);
	plankVertex.addVertex( 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  r,  g,  b);
	plankVertex.addVertex(-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  r,  g,  b);
	plankVertex.addVertex( 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  r,  g,  b);
	plankVertex.addVertex( 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  r,  g,  b);
	plankVertex.addVertex(-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  r,  g,  b);
	plankVertex.addVertex(-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  r,  g,  b);
	plankVertex.addVertex( 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  r,  g,  b);
	plankVertex.addVertex(-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  r,  g,  b);
	plankVertex.addVertex(-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  r,  g,  b);
	plankVertex.addVertex( 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  r,  g,  b);
	plankVertex.addVertex( 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  r,  g,  b);
	plankVertex.addVertex(-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  r,  g,  b);
	plankVertex.addVertex( 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  r,  g,  b);
	plankVertex.addVertex( 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  r,  g,  b);
	plankVertex.addVertex( 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  r,  g,  b);
	plankVertex.addVertex( 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  r,  g,  b);
	plankVertex.addVertex( 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  r,  g,  b);
	plankVertex.addVertex( 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  r,  g,  b);
	plankVertex.addVertex( 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  r,  g,  b);
	plankVertex.addVertex( 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  r,  g,  b);
	plankVertex.addVertex(-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  r,  g,  b);
	plankVertex.addVertex( 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  r,  g,  b);
	plankVertex.addVertex(-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  r,  g,  b);
	plankVertex.addVertex(-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  r,  g,  b);
	plankVertex.addVertex(-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  r,  g,  b);
	plankVertex.addVertex(-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  r,  g,  b);
	plankVertex.addVertex( 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  r,  g,  b);
	plankVertex.addVertex( 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  r,  g,  b);
	plankVertex.addVertex(-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  r,  g,  b);
	plankVertex.addVertex( 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  r,  g,  b);

	plank = new elementManager(plankVertex, &createBoxBody);

	float mass = 0.3f;
	float offsetx = 2;
	float offsetz = 3;
	float xcount = 4;
	float ycount = 13;
	float zcount = 4;
	float plankLength = 1;
	float plankWidth = 0.15;

	for (int y = 0; y < ycount*2; y+=2) {
		for (int x = 0; x < xcount; x++) {
			//始点
			plank.generate(parameterPack(
						param("position", Vector3f(offsetx + x*plankLength*2, 0.2 + 0.4*y, offsetz - 1 + plankWidth)),
						param("scale", Vector3f(1, 0.2, plankWidth)),
						param("rotation", Quaternionf(0, 0, 0, 1)),
						param("mass", mass)));

			for (int i = 1; i < zcount; i++) {
				plank.generate(parameterPack(
							param("position", Vector3f(offsetx + x*plankLength*2, 0.2 + 0.4*y, offsetz + (i * plankLength * 2) - 1)),
							param("scale", Vector3f(1, 0.2, plankWidth)),
							param("rotation", Quaternionf(0, 0, 0, 1)),
							param("mass", mass)));
			}

			//終点
			plank.generate(parameterPack(
						param("position", Vector3f(offsetx + x*plankLength*2, 0.2 + 0.4*y, offsetz + zcount * plankLength * 2 - 1 - plankWidth)),
						param("scale", Vector3f(1, 0.2, plankWidth)),
						param("rotation", Quaternionf(0, 0, 0, 1)),
						param("mass", mass)));

		}


		for (int z = 0; z < zcount; z++) {
			//始点
			plank.generate(parameterPack(
						param("position", Vector3f(offsetx - 1 + plankWidth, 0.2 + 0.4*(y+1), offsetz + z*plankLength*2)),
						param("scale", Vector3f(plankWidth, 0.2, 1)),
						param("rotation", Quaternionf(0, 0, 0, 1)),
						param("mass", mass)));

			for (int i = 1; i < xcount; i++) {
				plank.generate(parameterPack(
							param("position", Vector3f(offsetx + (i * plankLength * 2)- 1, 0.2 + 0.4*(y+1), offsetz + z*plankLength*2)),
							param("scale", Vector3f(plankWidth, 0.2, 1)),
							param("rotation", Quaternionf(0, 0, 0, 1)),
							param("mass", mass)));
			}

			//終点
			plank.generate(parameterPack(
						param("position", Vector3f(offsetx + xcount * plankLength * 2 - 1 - plankWidth, 0.2 + 0.4*(y+1), offsetz + z*plankLength*2)),
						param("scale", Vector3f(plankWidth, 0.2, 1)),
						param("rotation", Quaternionf(0, 0, 0, 1)),
						param("mass", mass)));

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
