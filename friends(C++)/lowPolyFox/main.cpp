#include <iostream>
#include <chrono>
#include <json11.hpp>
#include "../../source/japarilib.hpp"


std::string foxJsonString =
#include "lowPolyFox.fpm"
;

template <typename... ARGS>
parameterPack* paramPack(ARGS... args){
	return new parameterPack(sizeof... (ARGS), args...);
}


elementManager* foxElementManager;

Eigen::Vector3f foxPosition;
Eigen::Vector3f foxScale;
Eigen::Quaternionf foxRotation;
float foxMass;

std::vector<vertex> foxVertices;


class fox {

	elementNode* entity;

	public:

	fox (float x, float y, float z) {
		spawn(Eigen::Vector3f(x, y, z), Eigen::Quaternionf(1, 0, 0, 0));
	}

	fox (Eigen::Vector3f position, Eigen::Quaternionf rotation) {
		spawn(position, rotation);
	}

	void spawn(Eigen::Vector3f position, Eigen::Quaternionf rotation) {

		entity = foxElementManager->generate(paramPack(
							param("position", foxPosition + position),
							param("scale",    foxScale),
							param("rotation", foxRotation * rotation),
							param("model",    foxVertices),
							param("mass",     foxMass)));
	}

	void destroy() {
		entity->destroy();
	}
};

extern "C"
void init() {

	std::string err;
	auto json = json11::Json::parse(foxJsonString, err);

	for (auto elem : json.array_items()) {
		if (elem["objectType"].string_value() == "MESH") {
			if (elem["name"].string_value() == "Fox") {
				foxPosition	= Eigen::Vector3f(elem["xpos"].number_value(), elem["ypos"].number_value(), elem["zpos"].number_value());
				foxScale	= Eigen::Vector3f(elem["xscl"].number_value(), elem["yscl"].number_value(), elem["zscl"].number_value());
				foxRotation	= Eigen::Quaternionf(elem["wqat"].number_value(), elem["xqat"].number_value(), elem["yqat"].number_value(), elem["zqat"].number_value());
				foxMass		= elem["mass"].number_value();

				for (auto objvertex : elem["vertex"].array_items()) {
					foxVertices.push_back(vertex(
								objvertex.array_items()[0].number_value(), objvertex.array_items()[1].number_value(), objvertex.array_items()[2].number_value(),
								objvertex.array_items()[3].number_value(), objvertex.array_items()[4].number_value(), objvertex.array_items()[5].number_value(),
								objvertex.array_items()[6].number_value(), objvertex.array_items()[7].number_value(), objvertex.array_items()[8].number_value()
							));
				}
			}
		}
	}

	foxElementManager = new elementManager(foxVertices, createConvexHullShapeBody);


}

int timeDivisor = 0;

extern "C"
void tick() {
	if (timeDivisor++ == 300) {
		timeDivisor = 0;
		new fox(0, 10, 0);
	}
}

