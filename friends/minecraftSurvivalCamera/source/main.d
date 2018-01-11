import core.stdc.stdio;
import std.stdio;
import core.runtime;
import std.random;
import std.math;
import std.algorithm;


import japariSDK.japarilib;
import dlib.math.vector;
import dlib.math.quaternion;


float posx = 0.0f;
float posy = 0.0f;
float posz = 0.0f;


float velx = 0.0f;
float vely = 0.0f;
float velz = 0.0f;

float speed = 1f;
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


extern (C) void handleMouseMove(double xpos, double ypos) {

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

extern (C) void handleKeypress(int key, int scancode, int action, int mods) {
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

elementNode kaban;


//ApplicationInterface----------------------

extern (C) void _updateCamera() {
	updateCamera(posx, posy, posz, horizontalAngle, verticalAngle, FoV);
}


extern (C) void init(){
	rt_init();

	kaban = getCubeShape().generate(parameterPack(
			param("position", Vector3f(0, 1, 0)),
			param("scale", Vector3f(0.5, 0.5, 0.5)),
			param("rotation", Quaternionf(0, 0, 0, 1)),
			param("mass", 1.0f)));

	requestCameraAccess(&_updateCamera);
	registerMouseMoveCallback(&handleMouseMove);
	registerKeyCallback(&handleKeypress);

}



extern (C) void tick() {

	kaban.activate();

	Vector3f pos = kaban.getPos();

	posx = pos.x;
	posy = pos.y;
	posz = pos.z;

	


	bool isGrounded = false;

	if (closestRayTest(posx, posy, posz, posx, posy - 100, posz) < 0.01) {
		isGrounded = true;
	}

	float velx = 0;
	float vely = 0;
	float velz = 0;

	if (holdingForward == true) {
		velx += sin(horizontalAngle)* speed;
		velz += cos(horizontalAngle)* speed;
	}

	if (holdingBackward == true) {
		velx += sin(horizontalAngle+3.14)* speed;
		velz += cos(horizontalAngle+3.14)* speed;
	}
	if (holdingRightStrafe == true) {
		velx += sin(horizontalAngle-(3.14/2))* speed;
		velz += cos(horizontalAngle-(3.14/2))* speed;
	}

	if (holdingLeftStrafe == true) {
		velx += sin(horizontalAngle+(3.14/2)) * speed;
		velz += cos(horizontalAngle+(3.14/2)) * speed;
	}

	if (holdingSneek == true) {
		velx /= 2;
		velz /= 2;
	}


	if (holdingSpace == true) {
		vely += 10;
	}

	if (isGrounded == true) {
		kaban.setLinearVelocity(Vector3f(velx, vely, velz));
	}


	_updateCamera();

}

//------------------------------------------
