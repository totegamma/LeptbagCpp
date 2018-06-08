#ifndef PARAMETERPACK_HPP
#define PARAMETERPACK_HPP

class paramWrapper;
class parameterPack;

#include <iostream>
#include <assert.h>
#include <memory>
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

class paramWrapper{

	union data_t{
		int intValue;
		float floatValue;
		std::string* stringValue;
		Eigen::Vector3f* vec3Value;
		Eigen::Quaternionf* quatValue;
		vertexManager* modelValue;
		elementManager* emValue;
	} data;

	enum type{
		INT,
		FLOAT,
		STRING,
		VEC3,
		QUAT,
		MODEL,
		EM,
	};

	type contain;

	public:
	std::unique_ptr<std::string> tag;

	paramWrapper() = delete;
	paramWrapper(std::unique_ptr<std::string>, int intValue);
	paramWrapper(std::unique_ptr<std::string>, float floatValue);
	paramWrapper(std::unique_ptr<std::string>, std::string* stringValue);
	paramWrapper(std::unique_ptr<std::string>, Eigen::Vector3f* vec3Value);
	paramWrapper(std::unique_ptr<std::string>, Eigen::Quaternionf* quatValue);
	paramWrapper(std::unique_ptr<std::string>, vertexManager* modelValue);
	paramWrapper(std::unique_ptr<std::string>, elementManager* emValue);

	virtual void destroy();
	virtual ~paramWrapper();

	int getInt();
	float getFloat();
	std::string* getString();
	Eigen::Vector3f* getVec3();
	Eigen::Quaternionf* getQuat();
	vertexManager* getModel();
	elementManager* getEm();
};


class parameterPack_interface{
	virtual void destroy() = 0;
};


class parameterPack final: public parameterPack_interface{

	std::vector<std::shared_ptr<paramWrapper>> paramList;

	public:

	parameterPack();
	parameterPack(int count, va_list arguments);
	parameterPack(const parameterPack& rhs);
	~parameterPack();

	virtual void destroy();

	std::shared_ptr<paramWrapper> search(std::string input);
	void add(paramWrapper* input);

	parameterPack& operator=(const parameterPack& rhs){
		return *this;
	}


};


extern "C" parameterPack* createParameterPack(int count, ...);


paramWrapper* param(std::string tag, int value);
paramWrapper* param(std::string tag, float value);
paramWrapper* param(std::string tag, std::string value);
paramWrapper* param(std::string tag, Eigen::Vector3f* value);
paramWrapper* param(std::string tag, Eigen::Quaternionf* value);
paramWrapper* param(std::string tag, vertexManager* value);
paramWrapper* param(std::string tag, elementManager* value);


template <typename... ARGS>
parameterPack* paramWrap(ARGS... args){
	return createParameterPack(sizeof... (ARGS), args...);
}


#endif
