#include "universalString.hpp"


univStr::univStr(char* str, int length){
	this->str = str;
	this->length = length;
}

std::string univStr::getString(){
	return std::string(str, length);
}

univStr makeUnivStr(std::string in){
	char *cstr = new char[in.length()];
	strcpy(cstr, in.c_str());
	return univStr(cstr, in.length());
}


