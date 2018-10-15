#include "primitiveShape.hpp"

elementManager* cubeshape;
elementManager* planeshape;

void initPrimitives(){
	cubeshape = new elementManager(std::vector<vertex>(std::vector<vertex>{
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
				}), createBoxBody);


	planeshape = new elementManager(std::vector<vertex>(std::vector<vertex>{
					vertex(-100.0f,  0.0f, -100.0f, 0.0f, 1.0f, 0.0f, 0.3f, 0.35f, 0.3f),
					vertex(-100.0f,  0.0f,  100.0f, 0.0f, 1.0f, 0.0f, 0.3f, 0.35f, 0.3f),
					vertex( 100.0f,  0.0f, -100.0f, 0.0f, 1.0f, 0.0f, 0.3f, 0.35f, 0.3f),
					vertex( 100.0f,  0.0f,  100.0f, 0.0f, 1.0f, 0.0f, 0.3f, 0.35f, 0.3f),
					vertex( 100.0f,  0.0f, -100.0f, 0.0f, 1.0f, 0.0f, 0.3f, 0.35f, 0.3f),
					vertex(-100.0f,  0.0f,  100.0f, 0.0f, 1.0f, 0.0f, 0.3f, 0.35f, 0.3f)
				}), createPlaneBody);
}

elementManager* getCubeshape(){
	return cubeshape;
}

elementManager* getPlaneshape(){
	return planeshape;
}
