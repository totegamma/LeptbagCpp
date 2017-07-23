#ifndef UNIVSTR_HPP
#define UNIVSTR_HPP

#include <string.h>

class univStr{
	char* str;
	int length;

	public:
	univStr() = default;
	univStr(char* str, int length){
		this->str = str;
		this->length = length;
	}

	std::string getString(){
		return std::string(str, length);
	}

	bool operator==(univStr& rv) {
		return (this->length == rv.length) && (strncmp(this->str, rv.str, length) == 0);
	}


};


univStr makeUnivStr(std::string in){
	char *cstr = new char[in.length()];
	strcpy(cstr, in.c_str());
	return univStr(cstr, in.length());
}

#endif
