#ifndef PARAMETERPACK_HPP
#define PARAMETERPACK_HPP

#include "univStr.hpp"

class paramWrapper{

	public:
	univStr tag;
	union value_t{
		int intValue;
		float floatValue;
		univStr stringValue;
	} value;


	paramWrapper() = default;
	paramWrapper(univStr tag, int intValue){
		this->tag = tag;
		this->value.intValue = intValue;
	}

	paramWrapper(univStr tag, float floatValue){
		this->tag = tag;
		this->value.floatValue = floatValue;
	}

	paramWrapper(univStr tag, univStr stringValue){
		this->tag = tag;
		this->value.stringValue = stringValue;
	}

	int getInt(){
		return value.intValue;
	}
	float getFloat(){
		return value.floatValue;
	}
	univStr getString(){
		return value.stringValue;
	}

};

extern "C"
paramWrapper* createIntParam(univStr *tag, int value){
	return new paramWrapper(*tag, value);
}

extern "C"
paramWrapper* createFloatParam(univStr *tag, float value){
	return new paramWrapper(*tag, value);
}

extern "C"
paramWrapper* createStringParam(univStr *tag, univStr *value){
	return new paramWrapper(*tag, *value);
}


class parameterPack{
	paramWrapper **paramList;
	int length;
	public:
	parameterPack() = default;
	parameterPack(int count, va_list arguments){


		paramList = new paramWrapper*[count];
		length = count;

		for(int i = 0; i < count; i++){
			paramList[i] = va_arg(arguments, paramWrapper*);
		}
		va_end(arguments);
	}
	~parameterPack(){
		for(int i = 0; i < length; i++){
			delete paramList[i];
		}
		delete[] paramList;
	}

	paramWrapper* search(univStr in){
		for(int i = 0; i < length; i++){
			if(paramList[i]->tag == in){
				return paramList[i];
			}
		}
		return nullptr;
	}


};

extern "C"
parameterPack* createParameterPack(int count, ...){
	va_list arguments;
	va_start(arguments, count);
	return new parameterPack(count, arguments);
}

paramWrapper* param(std::string tag, int value){
	return new paramWrapper(makeUnivStr(tag), value);
}

paramWrapper* param(std::string tag, float value){
	return new paramWrapper(makeUnivStr(tag), value);
}

paramWrapper* param(std::string tag, std::string value){
	return new paramWrapper(makeUnivStr(tag), makeUnivStr(value));
}

template <typename... ARGS>
parameterPack* paramWrap(ARGS... args){
	return createParameterPack(sizeof... (ARGS), args...);
}

#endif
