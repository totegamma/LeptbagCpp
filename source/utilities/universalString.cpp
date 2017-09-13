#include "universalString.hpp"

int univStr::count = 0;

univStr::univStr(std::string input){
	char *cstr = new char[input.length()];//XXX 未確認
	strcpy(cstr, input.c_str());
	this->str = cstr;
	this->length = input.length();

	count++;
}

univStr::univStr(char* str, int length){
	this->str = str;
	this->length = length;
	
	count++;
}

univStr::~univStr(){
	count--;
}

void univStr::destroy(){
	delete this;
}

std::string univStr::getString(){
	return std::string(str, length);
}


extern "C"
univStr* createUnivStr(char *str, int length){
	return new univStr(str, length);//XXX 未確認
}


