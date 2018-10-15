#include <iostream>
#include "../../source/japarilib.hpp"

template <typename... ARGS>
parameterPack* paramPack(ARGS... args){
	return new parameterPack(sizeof... (ARGS), args...);
}


extern "C"
void init() {
	float r = 0.5;
	float g = 0.3;
	float b = 0.2;

	elementManager* plank = new elementManager(std::vector<vertex>(std::vector<vertex>{
									vertex(-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  r,  g,  b),
									vertex(-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  r,  g,  b),
									vertex(-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  r,  g,  b),
									vertex(-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  r,  g,  b),
									vertex(-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  r,  g,  b),
									vertex(-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  r,  g,  b),
									vertex( 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  r,  g,  b),
									vertex(-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  r,  g,  b),
									vertex( 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  r,  g,  b),
									vertex( 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  r,  g,  b),
									vertex(-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  r,  g,  b),
									vertex(-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  r,  g,  b),
									vertex( 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  r,  g,  b),
									vertex(-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  r,  g,  b),
									vertex(-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  r,  g,  b),
									vertex( 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  r,  g,  b),
									vertex( 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  r,  g,  b),
									vertex(-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  r,  g,  b),
									vertex( 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  r,  g,  b),
									vertex( 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  r,  g,  b),
									vertex( 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  r,  g,  b),
									vertex( 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  r,  g,  b),
									vertex( 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  r,  g,  b),
									vertex( 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  r,  g,  b),
									vertex( 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  r,  g,  b),
									vertex( 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  r,  g,  b),
									vertex(-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  r,  g,  b),
									vertex( 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  r,  g,  b),
									vertex(-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  r,  g,  b),
									vertex(-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  r,  g,  b),
									vertex(-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  r,  g,  b),
									vertex(-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  r,  g,  b),
									vertex( 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  r,  g,  b),
									vertex( 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  r,  g,  b),
									vertex(-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  r,  g,  b),
									vertex( 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  r,  g,  b)
							}), createBoxBody);

	for(int x = 0; x < 4; x++){
		for(int z = 0; z < 4; z++){
			for(int y = 0; y < 20; y++){

				plank->generate(paramPack(
							param("position", Eigen::Vector3f(0.1 + 2*x, 0.2 + 0.8*y, 1 + 2*z)),
							param("scale",    Eigen::Vector3f(0.1, 0.2, 1)),
							param("rotation", Eigen::Quaternionf(1, 0, 0, 0)),
							param("mass", 0.5f)));

				plank->generate(paramPack(
							param("position", Eigen::Vector3f(1.9 + 2*x, 0.2 + 0.8*y, 1 + 2*z)),
							param("scale",    Eigen::Vector3f(0.1, 0.2, 1)),
							param("rotation", Eigen::Quaternionf(1, 0, 0, 0)),
							param("mass", 0.5f)));

				plank->generate(paramPack(
							param("position", Eigen::Vector3f(1 + 2*x, 0.6 + 0.8*y, 0.1 + 2*z)),
							param("scale",    Eigen::Vector3f(1, 0.2, 0.1)),
							param("rotation", Eigen::Quaternionf(1, 0, 0, 0)),
							param("mass", 0.5f)));

				plank->generate(paramPack(
							param("position", Eigen::Vector3f(1 + 2*x, 0.6 + 0.8*y, 1.9 + 2*z)),
							param("scale",    Eigen::Vector3f(1, 0.2, 0.1)),
							param("rotation", Eigen::Quaternionf(1, 0, 0, 0)),
							param("mass", 0.5f)));

			}
		}
	}
}

extern "C"
void tick() {
}

