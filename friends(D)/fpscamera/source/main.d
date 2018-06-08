import core.stdc.stdio;
import std.stdio;
import std.math;
import core.runtime;
import std.json;


import leptbagSDK.leptbaglib;
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

float horizontalDiff = 0.0f;
float horizontalAngle = 0.0f;
float verticalAngle = 0.0f;

float FoV = 45.0f;


bool holdingForward     = false;
bool holdingBackward    = false;
bool holdingLeftStrafe  = false;
bool holdingRightStrafe = false;

bool holdingSneek = false;
bool holdingSpace = false;


extern (C) void handleMouseButton(int button, int action, int mods) {
	if (action == GLFW_PRESS) {
		switch(button) {
			case GLFW_MOUSE_BUTTON_1:
				new bullet(posx, posy, posz);
				break;
			default:
				break;
		}
	}
}



extern (C) void handleMouseMove(double xpos, double ypos) {

	float midWindowX = getWindowWidth()/2;
	float midWindowY = getWindowHeight()/2;

	horizontalDiff = mouseSpeed * float(midWindowX - xpos);

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


vertexManager bulletVertices;

elementManager bulletElementManager;


Vector3f bulletPosition;
Vector3f bulletScale;
Quaternionf bulletRotation;
float bulletMass;


class bullet{

	this(float x, float y, float z) {
		spawn(Vector3f(x, y, z));
	}

	void spawn(Vector3f position){

		bulletElementManager.generate(parameterPack(
							param("position", bulletPosition + position),
							param("scale",    bulletScale),
							param("rotation", bulletRotation),
							param("model",    bulletVertices),
							param("mass",     bulletMass)));

	}
}

elementNode cube;

//ApplicationInterface----------------------

extern (C) void _updateCamera() {
	updateCamera(posx, posy, posz, horizontalAngle, verticalAngle, FoV);
}


extern (C) void init(){
	rt_init();

	JSONValue model;



	// ## Bullet(弾丸の意味で)のロード ##
	bulletVertices = new vertexManager();

	//HACK コンパイル時にjsonStringにlowPolyTree.fpmの内容が代入される(要-Jオプション)
	auto bulletJsonString = import("bullet.fpm");

	model = parseJSON(bulletJsonString);

	foreach(elem; model.array){
		if(elem["objectType"].str == "MESH"){
			if(elem["name"].str == "bullet"){
				bulletPosition = Vector3f(elem["xpos"].floating, elem["ypos"].floating, elem["zpos"].floating);
				bulletScale    = Vector3f(elem["xscl"].floating, elem["yscl"].floating, elem["zscl"].floating);
				bulletRotation = Quaternionf(elem["xqat"].floating, elem["yqat"].floating, elem["zqat"].floating, elem["wqat"].floating);
				bulletMass     = elem["mass"].floating;

				foreach(objvertex; elem["vertex"].array){
					bulletVertices.addVertex(objvertex.array[0].floating, objvertex.array[1].floating, objvertex.array[2].floating,
											objvertex.array[3].floating, objvertex.array[4].floating, objvertex.array[5].floating,
											objvertex.array[6].floating, objvertex.array[7].floating, objvertex.array[8].floating);
				}
				bulletElementManager = new elementManager(bulletVertices, &createConvexHullShapeBody);
			}
		}
	}

	cube = getCubeShape().generate(parameterPack(
				param("position", Vector3f(0, 0.5, 0)),
				param("scale", Vector3f(0.5, 0.5, 0.5)),
				param("rotation", Quaternionf(0, 0, 0, 1)),
				param("mass", 10.0f)));

	requestCameraAccess(&_updateCamera);
	registerMouseButtonCallback(&handleMouseButton);
	registerMouseMoveCallback(&handleMouseMove);
	registerKeyCallback(&handleKeypress);
}



extern (C) void tick() {

	cube.entity.activate();

	Vector3f pos = cube.getPos();

	Quaternionf origQuat = cube.getRot();
	Quaternionf quat_xy = Quaternionf(origQuat.y, origQuat.x, origQuat.z, origQuat.w);
	Vector3f eul = quat_xy.toEulerAngles();
	horizontalAngle = eul.x;

	posx = pos.x;
	posy = pos.y;
	posz = pos.z;

	bool isGrounded = false;

	if (closestRayTest(pos.x, pos.y, pos.z, pos.x, pos.y - 100, pos.z) < 0.01) {
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
		cube.setLinearVelocity(Vector3f(velx, vely, velz));
	}

	cube.setAngularVelocity(Vector3f(0, horizontalDiff*30, 0));
	horizontalDiff = 0;


	_updateCamera();

}

//------------------------------------------
