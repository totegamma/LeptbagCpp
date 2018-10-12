#include <iostream>
#include "../../source/japarilib.hpp"

template <typename... ARGS>
parameterPack* paramPack(ARGS... args){
	return new parameterPack(sizeof... (ARGS), args...);
}

extern "C"
void init() {
	std::cout << "plugin loaded!!!" << std::endl;

	getPlaneshape()->generate(paramPack(
								param("position", Eigen::Vector3f(0, 0, 0)),
								param("scale",    Eigen::Vector3f(1, 1, 1)),
								param("face",     Eigen::Vector3f(0, 1, 0)),
								param("rotation", Eigen::Quaternionf(1, 0, 0, 0)),
								param("mass",     0.0f)));

}

extern "C"
void tick() {
}

