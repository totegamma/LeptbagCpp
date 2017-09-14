#ifndef PARAMETERPACK_HPP
#define PARAMETERPACK_HPP

class paramWrapper;
class parameterPack;

#include <iostream>
#include <assert.h>
#include <memory>

#include "universalString.hpp"
#include "universalVector.hpp"
#include "../vertexManager.hpp"
#include "../elementManager.hpp"

class paramWrapper{

	union data_t{
		int intValue;
		float floatValue;
		univStr* stringValue;
		vec3* vec3Value;
		quat* quatValue;
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
	static int count;
	std::unique_ptr<univStr> tag;

	paramWrapper() = delete;
	paramWrapper(std::unique_ptr<univStr>, int intValue);
	paramWrapper(std::unique_ptr<univStr>, float floatValue);
	paramWrapper(std::unique_ptr<univStr>, univStr* stringValue);
	paramWrapper(std::unique_ptr<univStr>, vec3* vec3Value);
	paramWrapper(std::unique_ptr<univStr>, quat* quatValue);
	paramWrapper(std::unique_ptr<univStr>, vertexManager* modelValue);
	paramWrapper(std::unique_ptr<univStr>, elementManager* emValue);

	virtual void destroy();
	virtual ~paramWrapper();

	int getInt();
	float getFloat();
	univStr* getString();
	vec3* getVec3();
	quat* getQuat();
	vertexManager* getModel();
	elementManager* getEm();
};

extern "C" paramWrapper* createIntParam   (univStr *tag, int value);
extern "C" paramWrapper* createFloatParam (univStr *tag, float value);
extern "C" paramWrapper* createStringParam(univStr *tag, univStr *value);
extern "C" paramWrapper* createVec3Param  (univStr *tag, vec3 *value);
extern "C" paramWrapper* createQuatParam  (univStr *tag, quat *value);
extern "C" paramWrapper* createModelParam (univStr *tag, vertexManager *value);
extern "C" paramWrapper* createEmParam    (univStr *tag, elementManager *value);

class parameterPack_interface{
	virtual void destroy() = 0;
};


class parameterPack final: public parameterPack_interface{

	std::vector<std::shared_ptr<paramWrapper>> paramList;

	public:
	static int count;

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
paramWrapper* param(std::string tag, vec3* value);
paramWrapper* param(std::string tag, quat* value);
paramWrapper* param(std::string tag, vertexManager* value);
paramWrapper* param(std::string tag, elementManager* value);


template <typename... ARGS>
parameterPack* paramWrap(ARGS... args){
	return createParameterPack(sizeof... (ARGS), args...);
}


#endif
