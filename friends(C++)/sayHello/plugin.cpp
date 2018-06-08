#include <iostream>

extern "C"
void init() {
	std::cout << "plugin loaded!!!" << std::endl;
}

extern "C"
void tick() {
}

