#ifndef UNIVERSALSTRING_HPP
#define UNIVERSALSTRING_HPP

#include <string>
#include <string.h>

class univStr_interface{
	virtual void destroy() = 0;
};

class univStr final: public univStr_interface{
	char* str;
	int length;

	public:
	univStr() = delete;
	univStr(std::string input);
	univStr(char* str, int length);
	~univStr();

	virtual void destroy();

	std::string getString();

	bool operator==(univStr& rv) {
		return (this->length == rv.length) && (strncmp(this->str, rv.str, length) == 0);
	}


};


extern "C" univStr* createUnivStr(char*str, int length);

#endif
