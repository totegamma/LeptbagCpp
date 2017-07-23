#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string>
#include <iostream>
#include <string.h>




class stringHolder{
	char* str;
	int length;

	public:
	stringHolder() = default;
	stringHolder(char* str, int length){
		this->str = str;
		this->length = length;
	}

	std::string getString(){
		return std::string(str, length);
	}

	bool operator==(stringHolder& rv) {
		return (this->length == rv.length) && (strncmp(this->str, rv.str, length) == 0);
	}


};

stringHolder str2h(std::string in){
	char *cstr = new char[in.length()];
	strcpy(cstr, in.c_str());
	return stringHolder(cstr, in.length());
}

class argWrapper{

	public:
	stringHolder tag;
	union value_t{
		int intValue;
		float floatValue;
		stringHolder stringValue;
	} value;

	/*
	enum type_t{
		INT,
		FLOAT,
		STRING
	} type;
	*/


	argWrapper() = default;
	argWrapper(stringHolder tag, int intValue){
		this->tag = tag;
		this->value.intValue = intValue;
		//type = INT;
	}

	argWrapper(stringHolder tag, float floatValue){
		this->tag = tag;
		this->value.floatValue = floatValue;
		//type = FLOAT;
	}

	argWrapper(stringHolder tag, stringHolder stringValue){
		this->tag = tag;
		this->value.stringValue = stringValue;
		//type = STRING;
	}

	int getInt(){
		return value.intValue;
	}

	float getFloat(){
		return value.floatValue;
	}

	stringHolder getString(){
		return value.stringValue;
	}

};

extern "C"
argWrapper* createIntArg(stringHolder *tag, int value){
	return new argWrapper(*tag, value);
}

extern "C"
argWrapper* createFloatArg(stringHolder *tag, float value){
	return new argWrapper(*tag, value);
}

extern "C"
argWrapper* createStringArg(stringHolder *tag, stringHolder *value){
	return new argWrapper(*tag, *value);
}


class parameterPack{
	argWrapper **argList;
	int length;
	public:
	parameterPack() = default;
	parameterPack(int count, va_list arguments){


		argList = new argWrapper*[count];
		length = count;

		for(int i = 0; i < count; i++){
			argList[i] = va_arg(arguments, argWrapper*);
		}
		va_end (arguments);
	}
	~parameterPack(){
		for(int i = 0; i < length; i++){
			delete argList[i];
		}
		delete[] argList;
	}

	argWrapper* search(stringHolder in){
		for(int i = 0; i < length; i++){
			if(argList[i]->tag == in){
				return argList[i];
			}
		}
		return nullptr;
	}


};



extern "C"
stringHolder* makeStringHolder(char* str, int length){
	return new stringHolder(str, length);
}

extern "C"
parameterPack* createParameterPack(int count, ...){
	va_list arguments;
	va_start(arguments, count);
	return new parameterPack(count, arguments);
}

extern "C"
void hoge(parameterPack* input){

	std::cout << input->search(str2h("nekochan"))->getInt() << std::endl;
	std::cout << input->search(str2h("hogepiyo"))->getFloat() << std::endl;
	std::cout << input->search(str2h("nyannyan"))->getString().getString() << std::endl;

}

argWrapper* arg(std::string tag, int value){
	return new argWrapper(str2h(tag), value);
}

argWrapper* arg(std::string tag, float value){
	return new argWrapper(str2h(tag), value);
}

argWrapper* arg(std::string tag, std::string value){
	return new argWrapper(str2h(tag), str2h(value));
}

template <typename... ARGS>
parameterPack* argWrap(ARGS... args){
	return createParameterPack(sizeof... (ARGS), args...);
}


int main() {
	
	std::cout << "cpp内部:" << std::endl;

	hoge(argWrap(arg("nekochan", 101), arg("hogepiyo", 101.101f), arg("nyannyan", "nya~~n")));


	std::cout << "D言語より:" << std::endl;

	printf("+main()\n");

	void *lh = dlopen("plugin.so", RTLD_LAZY);
	if (!lh)
	{
		fprintf(stderr, "dlopen error: %s\n", dlerror());
		exit(1);
	}
	printf("libdll.so is loaded\n");

	int (*fn)() = (int (*)())dlsym(lh, "dll");
	char *error = dlerror();
	if (error)
	{
		fprintf(stderr, "dlsym error: %s\n", error);
		exit(1);
	}
	printf("dll() function is found\n");

	(*fn)();

	printf("unloading libdll.so\n");
	dlclose(lh);

	printf("-main()\n");

	return 0;
}
