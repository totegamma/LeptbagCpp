#ifndef UNIVERSALSTRING_HPP
#define UNIVERSALSTRING_HPP

#include <string>
#include <string.h>

class univStr{
	char* str;
	int length;

	public:
	univStr() = default;
	univStr(char* str, int length);
	std::string getString();

	bool operator==(univStr& rv) {
		return (this->length == rv.length) && (strncmp(this->str, rv.str, length) == 0);
	}


};


univStr makeUnivStr(std::string in);

#endif
