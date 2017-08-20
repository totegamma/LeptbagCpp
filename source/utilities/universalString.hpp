#ifndef UNIVERSALSTRING_HPP
#define UNIVERSALSTRING_HPP

#include <string>
#include <string.h>

class univStr{
	char* str;
	int length;

	public:
	univStr() = delete;
	univStr(std::string input);

	univStr(char* str, int length);
	std::string getString();

	bool operator==(univStr& rv) {
		return (this->length == rv.length) && (strncmp(this->str, rv.str, length) == 0);
	}


};


extern "C" univStr* createUnivStr(char*str, int length);

#endif
