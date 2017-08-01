#ifndef PARAMETERPACK_HPP
#define PARAMETERPACK_HPP

#include <assert.h>

#include "../vertexManager.hpp"
#include "universalString.hpp"
#include "universalVector.hpp"

class paramWrapper{

	int intValue;
	float floatValue;
	univStr stringValue;
	vec3 vec3Value;
	quat quatValue;
	vertexManager modelValue;

	enum type{
		INT,
		FLOAT,
		STRING,
		VEC3,
		QUAT,
		MODEL
	};

	type contain;

	public:
	univStr tag;

	paramWrapper() = default;
	paramWrapper(univStr tag, int intValue);
	paramWrapper(univStr tag, float floatValue);
	paramWrapper(univStr tag, univStr stringValue);
	paramWrapper(univStr tag, vec3 vec3Value);
	paramWrapper(univStr tag, quat quatValue);
	paramWrapper(univStr tag, vertexManager modelValue);

	int getInt();
	float getFloat();
	univStr getString();
	vec3 getVec3();
	quat getQuat();
	vertexManager getModel();
};

extern "C" paramWrapper* createIntParam(univStr *tag, int value);
extern "C" paramWrapper* createFloatParam(univStr *tag, float value);
extern "C" paramWrapper* createStringParam(univStr *tag, univStr *value);
extern "C" paramWrapper* createVec3Param(univStr *tag, vec3 *value);
extern "C" paramWrapper* createQuatParam(univStr *tag, quat *value);
extern "C" paramWrapper* createModelParam(univStr *tag, vertexManager *value);


class parameterPack{
	paramWrapper **paramList;
	int length;

	public:
	parameterPack() = default;
	parameterPack(int count, va_list arguments);
	~parameterPack();
	paramWrapper* search(std::string input);


};

extern "C" parameterPack* createParameterPack(int count, ...);

paramWrapper* param(std::string tag, int value);
paramWrapper* param(std::string tag, float value);
paramWrapper* param(std::string tag, std::string value);
paramWrapper* param(std::string tag, vec3 value);
paramWrapper* param(std::string tag, quat value);
paramWrapper* param(std::string tag, vertexManager value);


template <typename... ARGS>
parameterPack* paramWrap(ARGS... args){
	return createParameterPack(sizeof... (ARGS), args...);
}


#endif
