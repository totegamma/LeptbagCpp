#include "primitiveShape.hpp"

elementManager* cubeshape;
elementManager* planeshape;

void initPrimitives(){
	cubeshape = new elementManager(std::make_shared<std::vector<std::shared_ptr<vertex>>>(std::vector<std::shared_ptr<vertex>>{
					std::make_shared<vertex>(-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f),
					std::make_shared<vertex>(-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f),
					std::make_shared<vertex>(-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f),
					std::make_shared<vertex>(-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f),
					std::make_shared<vertex>(-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f),
					std::make_shared<vertex>(-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f),
					std::make_shared<vertex>( 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  1.0f),
					std::make_shared<vertex>(-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f),
					std::make_shared<vertex>( 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f),
					std::make_shared<vertex>( 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  1.0f),
					std::make_shared<vertex>(-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f),
					std::make_shared<vertex>(-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f),
					std::make_shared<vertex>( 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,  0.0f),
					std::make_shared<vertex>(-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f),
					std::make_shared<vertex>(-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f),
					std::make_shared<vertex>( 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,  0.0f),
					std::make_shared<vertex>( 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,  0.0f),
					std::make_shared<vertex>(-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f),
					std::make_shared<vertex>( 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f),
					std::make_shared<vertex>( 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f),
					std::make_shared<vertex>( 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f),
					std::make_shared<vertex>( 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f),
					std::make_shared<vertex>( 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f),
					std::make_shared<vertex>( 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f),
					std::make_shared<vertex>( 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,  1.0f),
					std::make_shared<vertex>( 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,  0.0f),
					std::make_shared<vertex>(-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f),
					std::make_shared<vertex>( 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,  1.0f),
					std::make_shared<vertex>(-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f),
					std::make_shared<vertex>(-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f),
					std::make_shared<vertex>(-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f),
					std::make_shared<vertex>(-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f),
					std::make_shared<vertex>( 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  1.0f),
					std::make_shared<vertex>( 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,  1.0f),
					std::make_shared<vertex>(-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f),
					std::make_shared<vertex>( 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  1.0f)
				}), createBoxBody);


	planeshape = new elementManager(std::make_shared<std::vector<std::shared_ptr<vertex>>>(std::vector<std::shared_ptr<vertex>>{
					std::make_shared<vertex>(-100.0f,  0.0f, -100.0f, 0.0f, 1.0f, 0.0f, 0.3f, 0.35f, 0.3f),
					std::make_shared<vertex>(-100.0f,  0.0f,  100.0f, 0.0f, 1.0f, 0.0f, 0.3f, 0.35f, 0.3f),
					std::make_shared<vertex>( 100.0f,  0.0f, -100.0f, 0.0f, 1.0f, 0.0f, 0.3f, 0.35f, 0.3f),
					std::make_shared<vertex>( 100.0f,  0.0f,  100.0f, 0.0f, 1.0f, 0.0f, 0.3f, 0.35f, 0.3f),
					std::make_shared<vertex>( 100.0f,  0.0f, -100.0f, 0.0f, 1.0f, 0.0f, 0.3f, 0.35f, 0.3f),
					std::make_shared<vertex>(-100.0f,  0.0f,  100.0f, 0.0f, 1.0f, 0.0f, 0.3f, 0.35f, 0.3f)
				}), createPlaneBody);
}

elementManager* getCubeshape(){
	return cubeshape;
}

elementManager* getPlaneshape(){
	return planeshape;
}
