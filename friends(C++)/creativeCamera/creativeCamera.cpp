#include <iostream>
#include "../../source/japarilib.hpp"


float posx = 0.0f;
float posy = 0.0f;
float posz = 0.0f;

float speed = 0.1f;
float mouseSpeed = 0.001f;

float horizontalAngle = 3.14f;
float verticalAngle = 0.0f;

float FoV = 45.0f;


bool holdingForward     = false;
bool holdingBackward    = false;
bool holdingLeftStrafe  = false;
bool holdingRightStrafe = false;

bool holdingSneek = false;
bool holdingSpace = false;


extern "C"
void handleMouseMove(double xpos, double ypos) {

	//カメラが一回転したら強制的に2PI回すことで無限に回れるようにする
	float midWindowX = getWindowWidth()/2;
	float midWindowY = getWindowHeight()/2;

	if (horizontalAngle + mouseSpeed * float(midWindowX - xpos) > 3.14) {
		horizontalAngle = (horizontalAngle + mouseSpeed * float(midWindowX - xpos)) - (3.14*2);
	} else if(horizontalAngle + mouseSpeed * float(midWindowX - xpos) < -3.14) {
		horizontalAngle = (horizontalAngle + mouseSpeed * float(midWindowX - xpos)) + (3.14*2);
	} else {
		horizontalAngle += mouseSpeed * float(midWindowX - xpos );
	}

	//カメラは真下から真上までの範囲しか動かない。頭は縦に一回転しない。
	if (verticalAngle + mouseSpeed * float(midWindowY - ypos) > 3.14/2) {
		verticalAngle = 3.14/2;
	} else if(verticalAngle + mouseSpeed * float(midWindowY - ypos) < -3.14/2) {
		verticalAngle = -3.14/2;
	} else {
		verticalAngle += mouseSpeed * float(midWindowY - ypos);
	}

	//マウスを強制的に真ん中に戻す
	setCursorPos(midWindowX, midWindowY);
}

extern "C"
void handleKeypress(int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS){
		switch(key) {
			case 'W':
				holdingForward = true;
				break;

			case 'S':
				holdingBackward = true;
				break;

			case 'A':
				holdingLeftStrafe = true;
				break;

			case 'D':
				holdingRightStrafe = true;
				break;

			case GLFW_KEY_LEFT_SHIFT:
				holdingSneek = true;
				break;

			case GLFW_KEY_SPACE:
				holdingSpace = true;
				break;

			default:
				break;
		}
	}else if(action == GLFW_RELEASE){
		switch(key) {
			case 'W':
				holdingForward = false;
				break;

			case 'S':
				holdingBackward = false;
				break;

			case 'A':
				holdingLeftStrafe = false;
				break;

			case 'D':
				holdingRightStrafe = false;
				break;

			case GLFW_KEY_LEFT_SHIFT :
				holdingSneek = false;
				break;

			case GLFW_KEY_SPACE:
				holdingSpace = false;
				break;

			default:
				break;
		}

	}
}

extern "C"
void _updateCamera() {
	updateCamera(posx, posy, posz, horizontalAngle, verticalAngle, FoV);
}

extern "C"
void init() {
	requestCameraAccess(&_updateCamera);
	registerMouseMoveCallback(&handleMouseMove);
	registerKeyCallback(&handleKeypress);
}

extern "C"
void tick() {
	if (holdingForward == true) {
		posx += sin(horizontalAngle)* speed;
		posz += cos(horizontalAngle)* speed;
	}

	if (holdingBackward == true) {
		posx += sin(horizontalAngle+3.14)* speed;
		posz += cos(horizontalAngle+3.14)* speed;
	}

	if (holdingRightStrafe == true) {
		posx += sin(horizontalAngle-(3.14/2))* speed;
		posz += cos(horizontalAngle-(3.14/2))* speed;
	}

	if (holdingLeftStrafe == true) {
		posx += sin(horizontalAngle+(3.14/2)) * speed;
		posz += cos(horizontalAngle+(3.14/2)) * speed;
	}

	if (holdingSpace == true) {
		posy += speed;
	}

	if (holdingSneek == true) {
		posy -= speed;
	}

	_updateCamera();
}

