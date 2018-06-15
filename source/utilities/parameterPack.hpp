#ifndef PARAMETERPACK_H
#define PARAMETERPACK_H

class param;
class parameterPack;

#include <iostream>
#include <vector>
#include <exception>
#include <Eigen/Core>
#include <Eigen/Geometry>

#include "../vertexManager.hpp"
#include "../elementManager.hpp"

class parameterPackException : public std::exception {
	public:
	parameterPackException();
	parameterPackException(std::string key);
};


class param {

	enum type{
		INT,
		FLOAT,
		STRING,
		VEC3,
		QUAT,
		MODEL,
		EM,
	} type;

	struct data_t{
		int intValue;
		float floatValue;
		std::string stringValue;
		Eigen::Vector3f vec3Value;
		Eigen::Quaternionf quatValue;
		std::vector<vertex> modelValue;
		elementManager* emValue;
	} data;

	std::string tag;

	public:
	param() = delete;
	param(std::string tag, int data);
	param(std::string tag, float data);
	param(std::string tag, std::string data);
	param(std::string tag, Eigen::Vector3f data);
	param(std::string tag, Eigen::Quaternionf data);
	param(std::string tag, std::vector<vertex> data);
	param(std::string tag, elementManager* data);

	bool isTag(std::string target);

	int getInt();
	float getFloat();
	std::string getString();
	Eigen::Vector3f getVec3();
	Eigen::Quaternionf getQuat();
	std::vector<vertex> getModel();
	elementManager* getEm();
};

class parameterPack {

	std::vector<param> container;

	public:
	parameterPack();
	parameterPack(int i, ...);

	param search(std::string target);
	void add(param input);

};

#endif
