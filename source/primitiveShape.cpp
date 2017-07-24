#include "primitiveShape.hpp"

std::vector<vertex> cubeShapeVertices = {

	vertex(-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f),
	vertex(-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f),
	vertex(-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f),

	vertex(-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f),
	vertex(-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f),
	vertex(-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f),


	vertex( 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  1.0f),
	vertex(-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f),
	vertex( 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f),

	vertex( 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  1.0f),
	vertex(-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f),
	vertex(-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f),


	vertex( 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,  0.0f),
	vertex(-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f),
	vertex(-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f),

	vertex( 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,  0.0f),
	vertex( 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,  0.0f),
	vertex(-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f),


	vertex( 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f),
	vertex( 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f),
	vertex( 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f),

	vertex( 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f),
	vertex( 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f),
	vertex( 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f),


	vertex( 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,  1.0f),
	vertex( 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,  0.0f),
	vertex(-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f),

	vertex( 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,  1.0f),
	vertex(-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f),
	vertex(-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f),


	vertex(-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f),
	vertex(-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f),
	vertex( 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  1.0f),

	vertex( 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,  1.0f),
	vertex(-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f),
	vertex( 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  1.0f)

};

std::vector<vertex> planeShapeVertices = {
	vertex(-100.0f,  0.0f, -100.0f, 0.0f, 1.0f, 0.0f, 0.3f, 0.35f, 0.3f),
	vertex(-100.0f,  0.0f,  100.0f, 0.0f, 1.0f, 0.0f, 0.3f, 0.35f, 0.3f),
	vertex( 100.0f,  0.0f, -100.0f, 0.0f, 1.0f, 0.0f, 0.3f, 0.35f, 0.3f),
	vertex( 100.0f,  0.0f,  100.0f, 0.0f, 1.0f, 0.0f, 0.3f, 0.35f, 0.3f),
	vertex( 100.0f,  0.0f, -100.0f, 0.0f, 1.0f, 0.0f, 0.3f, 0.35f, 0.3f),
	vertex(-100.0f,  0.0f,  100.0f, 0.0f, 1.0f, 0.0f, 0.3f, 0.35f, 0.3f)
};

elementManager& getCubeshape(){
	static elementManager cubeshape =  elementManager(cubeShapeVertices, createBoxBody);
	return cubeshape;
}

elementManager& getPlaneshape(){
	static elementManager planeshape =  elementManager(planeShapeVertices, createPlaneBody);
	return planeshape;
}
