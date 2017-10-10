#include "universalString.hpp"


univStr::univStr(std::string input){
	char *cstr = new char[input.length()];
	strcpy(cstr, input.c_str());
	this->str = cstr;
	this->length = input.length();

}

univStr::univStr(char* str, int length){
	this->str = new char[length];
	strncpy(this->str, str, length);
	this->length = length;
	
}

univStr::~univStr(){
}

void univStr::destroy(){
	delete this;
}

std::string univStr::getString(){
	return std::string(str, length);
}


extern "C"
univStr* createUnivStr(char *str, int length){
	return new univStr(str, length);
}


