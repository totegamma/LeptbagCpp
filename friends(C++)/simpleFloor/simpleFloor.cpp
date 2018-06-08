#include <iostream>
#include "../../source/japarilib.hpp"

extern "C"
void init() {
	std::cout << "plugin loaded!!!" << std::endl;
	getPlaneshape()->generate(paramWrap(
								param("position", new Eigen::Vector3f(0, -3, 0)),
								param("scale",    new Eigen::Vector3f(1, 1, 1)),
								param("face",     new Eigen::Vector3f(0, 1, 0)),
								param("rotation", new Eigen::Quaternionf(1, 0, 0, 0)),
								param("mass",     1.0f)));
}

extern "C"
void tick() {
}

