#include <iostream>
#include <chrono>
#include "../../source/japarilib.hpp"

const int buffer_size = 16;

textbox* fpsLabel;
wchar_t textBuff[buffer_size];
std::chrono::system_clock::time_point previousFrame;

extern "C"
void init() {
	fpsLabel = new textbox(u"fps: 0", 3, 10, 30, 0, 0, 0);
}

extern "C"
void tick() {
	auto now = std::chrono::system_clock::now();
	double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - previousFrame).count();
	double fps = 1000.0/elapsed;

	swprintf(textBuff, buffer_size, L"FPS: %lf", fps);
	fpsLabel->updateText((char16_t*)textBuff, wcslen(textBuff));

	previousFrame = std::chrono::system_clock::now();
}

