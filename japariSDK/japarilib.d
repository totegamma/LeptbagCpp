module japariSDK.japarilib;
import std.stdio;

import dlib.math.vector;
import dlib.math.quaternion;

const int GLFW_RELEASE = 0;
const int GLFW_PRESS   = 1;
const int GLFW_REPEAT  = 2;

const int GLFW_MOUSE_BUTTON_1 = 0;
const int GLFW_MOUSE_BUTTON_2 = 1;
const int GLFW_MOUSE_BUTTON_3 = 2;
const int GLFW_MOUSE_BUTTON_4 = 3;
const int GLFW_MOUSE_BUTTON_5 = 4;
const int GLFW_MOUSE_BUTTON_6 = 5;
const int GLFW_MOUSE_BUTTON_7 = 6;
const int GLFW_MOUSE_BUTTON_8 = 7;

const int GLFW_KEY_UNKNOWN = -1;
const int GLFW_KEY_SPACE = 32;
const int GLFW_KEY_APOSTROPHE = 39; /* ' */
const int GLFW_KEY_COMMA = 44; /* , */
const int GLFW_KEY_MINUS = 45; /* - */
const int GLFW_KEY_PERIOD = 46; /* . */
const int GLFW_KEY_SLASH = 47; /* / */
const int GLFW_KEY_0 = 48;
const int GLFW_KEY_1 = 49;
const int GLFW_KEY_2 = 50;
const int GLFW_KEY_3 = 51;
const int GLFW_KEY_4 = 52;
const int GLFW_KEY_5 = 53;
const int GLFW_KEY_6 = 54;
const int GLFW_KEY_7 = 55;
const int GLFW_KEY_8 = 56;
const int GLFW_KEY_9 = 57;
const int GLFW_KEY_SEMICOLON = 59; /* ; */
const int GLFW_KEY_EQUAL = 61; /* = */
const int GLFW_KEY_A = 65;
const int GLFW_KEY_B = 66;
const int GLFW_KEY_C = 67;
const int GLFW_KEY_D = 68;
const int GLFW_KEY_E = 69;
const int GLFW_KEY_F = 70;
const int GLFW_KEY_G = 71;
const int GLFW_KEY_H = 72;
const int GLFW_KEY_I = 73;
const int GLFW_KEY_J = 74;
const int GLFW_KEY_K = 75;
const int GLFW_KEY_L = 76;
const int GLFW_KEY_M = 77;
const int GLFW_KEY_N = 78;
const int GLFW_KEY_O = 79;
const int GLFW_KEY_P = 80;
const int GLFW_KEY_Q = 81;
const int GLFW_KEY_R = 82;
const int GLFW_KEY_S = 83;
const int GLFW_KEY_T = 84;
const int GLFW_KEY_U = 85;
const int GLFW_KEY_V = 86;
const int GLFW_KEY_W = 87;
const int GLFW_KEY_X = 88;
const int GLFW_KEY_Y = 89;
const int GLFW_KEY_Z = 90;
const int GLFW_KEY_LEFT_BRACKET = 91; /* [ */
const int GLFW_KEY_BACKSLASH = 92; /* \ */
const int GLFW_KEY_RIGHT_BRACKET = 93; /* ] */
const int GLFW_KEY_GRAVE_ACCENT = 96; /* ` */
const int GLFW_KEY_WORLD_1 = 161; /* non-US #1 */
const int GLFW_KEY_WORLD_2 = 162; /* non-US #2 */
const int GLFW_KEY_ESCAPE = 256;
const int GLFW_KEY_ENTER = 257;
const int GLFW_KEY_TAB = 258;
const int GLFW_KEY_BACKSPACE = 259;
const int GLFW_KEY_INSERT = 260;
const int GLFW_KEY_DELETE = 261;
const int GLFW_KEY_RIGHT = 262;
const int GLFW_KEY_LEFT = 263;
const int GLFW_KEY_DOWN = 264;
const int GLFW_KEY_UP = 265;
const int GLFW_KEY_PAGE_UP = 266;
const int GLFW_KEY_PAGE_DOWN = 267;
const int GLFW_KEY_HOME = 268;
const int GLFW_KEY_END = 269;
const int GLFW_KEY_CAPS_LOCK = 280;
const int GLFW_KEY_SCROLL_LOCK = 281;
const int GLFW_KEY_NUM_LOCK = 282;
const int GLFW_KEY_PRINT_SCREEN = 283;
const int GLFW_KEY_PAUSE = 284;
const int GLFW_KEY_F1 = 290;
const int GLFW_KEY_F2 = 291;
const int GLFW_KEY_F3 = 292;
const int GLFW_KEY_F4 = 293;
const int GLFW_KEY_F5 = 294;
const int GLFW_KEY_F6 = 295;
const int GLFW_KEY_F7 = 296;
const int GLFW_KEY_F8 = 297;
const int GLFW_KEY_F9 = 298;
const int GLFW_KEY_F10 = 299;
const int GLFW_KEY_F11 = 300;
const int GLFW_KEY_F12 = 301;
const int GLFW_KEY_F13 = 302;
const int GLFW_KEY_F14 = 303;
const int GLFW_KEY_F15 = 304;
const int GLFW_KEY_F16 = 305;
const int GLFW_KEY_F17 = 306;
const int GLFW_KEY_F18 = 307;
const int GLFW_KEY_F19 = 308;
const int GLFW_KEY_F20 = 309;
const int GLFW_KEY_F21 = 310;
const int GLFW_KEY_F22 = 311;
const int GLFW_KEY_F23 = 312;
const int GLFW_KEY_F24 = 313;
const int GLFW_KEY_F25 = 314;
const int GLFW_KEY_KP_0 = 320;
const int GLFW_KEY_KP_1 = 321;
const int GLFW_KEY_KP_2 = 322;
const int GLFW_KEY_KP_3 = 323;
const int GLFW_KEY_KP_4 = 324;
const int GLFW_KEY_KP_5 = 325;
const int GLFW_KEY_KP_6 = 326;
const int GLFW_KEY_KP_7 = 327;
const int GLFW_KEY_KP_8 = 328;
const int GLFW_KEY_KP_9 = 329;
const int GLFW_KEY_KP_DECIMAL = 330;
const int GLFW_KEY_KP_DIVIDE = 331;
const int GLFW_KEY_KP_MULTIPLY = 332;
const int GLFW_KEY_KP_SUBTRACT = 333;
const int GLFW_KEY_KP_ADD = 334;
const int GLFW_KEY_KP_ENTER = 335;
const int GLFW_KEY_KP_EQUAL = 336;
const int GLFW_KEY_LEFT_SHIFT = 340;
const int GLFW_KEY_LEFT_CONTROL = 341;
const int GLFW_KEY_LEFT_ALT = 342;
const int GLFW_KEY_LEFT_SUPER = 343;
const int GLFW_KEY_RIGHT_SHIFT = 344;
const int GLFW_KEY_RIGHT_CONTROL = 345;
const int GLFW_KEY_RIGHT_ALT = 346;
const int GLFW_KEY_RIGHT_SUPER = 347;
const int GLFW_KEY_MENU = 348;
const int GLFW_KEY_LAST = GLFW_KEY_MENU;
 


elementManager getCubeShape() {
	static elementManager instance;
	if(!instance) instance = new elementManager(getCubeshape());
	return instance;
}

elementManager getPlaneShape() {
	static elementManager instance;
	if(!instance) instance = new elementManager(getPlaneshape());
	return instance;
}

extern (C) {
	int getWindowWidth();
	int getWindowHeight();
	void setCursorPos(float x, float y);
	void registerKeyCallback(void function(int, int, int, int));
	void registerMouseMoveCallback(void function(double, double));
	void registerMouseButtonCallback(void function(int, int, int));

	int  requestCameraAccess(void function());
	void updateCamera(float posx, float posy, float posz, float horizAng, float vertAng, float FoV);
	float closestRayTest(float fromx, float fromy, float fromz, float tox, float toy, float toz);
}


// -------- 目次 --------
//vec3
//quat
//vertex
//vertexManager
//univStr
//paramWrapper
//parameterPack
//elementManager
//elementNode
//hingeConstraint
//generic6DofConstraint
// ----------------------


//========[vec3]========
extern (C++) {
	interface vec3_interface {
		float getx();
		float gety();
		float getz();
		void destroy();
	}
}

extern (C) vec3_interface createVec3(float x, float y, float z);

struct vec3 {
	vec3_interface entity;
	bool exported;

	this(float x, float y, float z) {
		entity = createVec3(x, y, z);
		exported = false;
	}

	float getx() {
		return entity.getx();
	}

	float gety() {
		return entity.gety();
	}

	float getz() {
		return entity.getz();
	}

	~this() {
		if(exported == false) {
			entity.destroy();
		}
	}
}

vec3_interface toVec3(Vector3f input) {
	return createVec3(input.x, input.y, input.z);
}


//========[quat]========
extern (C++) {
	interface quat_interface{
		float getw();
		float getx();
		float gety();
		float getz();
		void destroy();
	}
}

extern (C) quat_interface createQuat(float w, float x, float y, float z);

struct quat{
	quat_interface entity;
	bool exported;

	this(float w, float x, float y, float z) {
		entity = createQuat(w, x, y, z);
		exported = false;
	}

	float getw() {
		return entity.getw();
	}

	float getx() {
		return entity.getx();
	}

	float gety() {
		return entity.gety();
	}

	float getz() {
		return entity.getz();
	}

	~this() {
		if(exported == false) {
			entity.destroy();
		}
	}
}

quat_interface toQuat(Quaternionf input) {
	return createQuat(input.w, input.x, input.y, input.z);
}


//========[vertex]========
/*
extern (C++) {
	interface vertex_interface{
		void destroy();
	}
}
extern (C) vertex_interface createVertex(float coordinate_x, float coordinate_y, float coordinate_z, float normal_x, float normal_y, float normal_z, float color_r, float color_g, float color_b);


class vertex {
	vertex_interface entity;
	bool exported;

	this(float coordinate_x, float coordinate_y, float coordinate_z, float normal_x, float normal_y, float normal_z, float color_r, float color_g, float color_b) {
		entity = createVertex(coordinate_x, coordinate_y, coordinate_z, normal_x, normal_y, normal_z, color_r, color_g, color_b);
		exported = false;
	}


	~this() {
		if(exported == false) {
			entity.destroy();
		}
	}

}
*/


//========[vertexManager]========
extern (C++) {
	interface vertexManager_interface {
		void addVertex(float coordinate_x, float coordinate_y, float coordinate_z, float normal_x, float normal_y, float normal_z, float color_r, float color_g, float color_b);
		void destroy();
	}
}

extern (C) vertexManager_interface createVertexManager();

class vertexManager {
	vertexManager_interface entity;
	bool exported;

	this() {
		entity = createVertexManager();
		exported = false;
	}

	void addVertex(float coordinate_x, float coordinate_y, float coordinate_z, float normal_x, float normal_y, float normal_z, float color_r, float color_g, float color_b) {
		entity.addVertex(coordinate_x, coordinate_y, coordinate_z, normal_x, normal_y, normal_z, color_r, color_g, color_b);
	}
}


//========[univStr]========
extern (C++) {
	interface univStr_interface {
		void destroy();
	}
}
extern (C) univStr_interface createUnivStr(char* str, int length);

struct univStr{
	univStr_interface entity;
	bool exported;

	this(char* str, int length) {
		entity = createUnivStr(str, length);
		exported = false;
	}

	this(string input) {
		char* cstr = &input.dup[0];
		entity = createUnivStr(cstr, cast(int)input.length);
		exported = false;
	}

	~this() {
		if(exported == false) {
			entity.destroy();
		}
	}
}

univStr_interface str2unis(string input) {
	char* cstr = &input.dup[0];
	return createUnivStr(cstr, cast(int)input.length);
}



//========[paramWrapper]========
extern (C++) {
	interface paramWrapper{
	}
}

extern (C) {
	paramWrapper createIntParam(univStr_interface tag, int value);
	paramWrapper createFloatParam(univStr_interface tag, float value);
	paramWrapper createStringParam(univStr_interface tag, univStr_interface value);
	paramWrapper createVec3Param(univStr_interface tag, vec3_interface value);
	paramWrapper createQuatParam(univStr_interface tag, quat_interface value);
	paramWrapper createModelParam(univStr_interface tag, vertexManager_interface value);
}



paramWrapper param(string tag, int value) {
	return createIntParam(str2unis(tag), value);
}

paramWrapper param(string tag, float value) {
	return createFloatParam(str2unis(tag), value);
}

paramWrapper param(string tag, string value) {
	return createStringParam(str2unis(tag), str2unis(value));
}

paramWrapper param(string tag, Vector3f value) {
	return createVec3Param(str2unis(tag), toVec3(value));
}

paramWrapper param(string tag, Quaternionf value) {
	return createQuatParam(str2unis(tag), toQuat(value));
}

paramWrapper param(string tag, vertexManager value) {
	return createModelParam(str2unis(tag), value.entity);
}



//========[parameterPack]========
extern (C++) {
	interface parameterPack_interface {
		void destroy();
	}
}


extern (C) parameterPack_interface createParameterPack(int count, ...);

struct parameterPack{
	parameterPack_interface entity;
	bool exported;

	this(T...)(T args) {
		entity = createParameterPack(args.length, args);
		exported = false;
	}

	~this() {
		if(exported == false) {
			entity.destroy();
		}
	}
}


//========[elementManager]========
extern (C++) {
	interface elementManager_interface{
		elementNode_interface generate(parameterPack_interface input);
		void destroy();
	}
}
extern (C) elementManager_interface createElementManager(vertexManager_interface vm, btRigidBody function(parameterPack));

class elementManager {

	elementManager_interface entity;

	this(elementManager_interface input) {
		entity = input;
	}

	extern(C)
	this(vertexManager vm, btRigidBody function(parameterPack) func) {
		entity = createElementManager(vm.entity, func);
	}

	elementNode generate(parameterPack input) {
		input.exported = true;
		return new elementNode(entity.generate(input.entity));
	}
}


//========[elementNode]========
extern (C++) {
	interface elementNode_interface {
		float getXpos();
		float getYpos();
		float getZpos();
		float getXrot();
		float getYrot();
		float getZrot();
		float getWrot();
		float getBasis(int row, int column);
		float getFriction();
		void setFriction(float coef);
		void setLinearVelocity(float x, float y, float z);
		void setAngularVelocity(float x, float y, float z);
		void activate();
		bool isCollision();
		void destroy();
	}
}

class elementNode {

	elementNode_interface entity;
	bool exported;

	this(elementNode_interface input) {
		entity = input;
		exported = false;
	}

	Vector3f getPos() {
		return Vector3f(entity.getXpos(), entity.getYpos(), entity.getZpos());
	}

	Quaternionf getRot() {
		return Quaternionf(entity.getXrot(), entity.getYrot(), entity.getZrot(), entity.getWrot());
	}

	float getBasis(int row, int column) {
		return entity.getBasis(row, column);
	}

	float getFriction() {
		return entity.getFriction();
	}

	void setFriction(float coef) {
		entity.setFriction(coef);
	}

	void setLinearVelocity(Vector3f linearVelocity) {
		entity.setLinearVelocity(linearVelocity.x, linearVelocity.y, linearVelocity.z);
	}

	void setAngularVelocity(Vector3f angularVelocity) {
		entity.setAngularVelocity(angularVelocity.x, angularVelocity.y, angularVelocity.z);
	}

	void activate() {
		entity.activate();
	}

	bool isCollision() {
		return entity.isCollision();
	}

	void destroy() {
		entity.destroy();
		exported = true;
		entity = null;
	}

	~this() {
	}

}

//========[hingeConstraint]========

extern(C++) {
	interface hingeConstraint_interface {
		void enableMotor(bool flag);
		void setLimit(float lower, float upper);
		void setMaxMotorImpulse(float power);
		void setMotorTarget(float angle, float duration);
		void destroy();
	}
}

extern (C) hingeConstraint_interface createHingeConstraint(elementNode_interface cubeA, elementNode_interface cubeB, vec3_interface positionA, vec3_interface positionB, vec3_interface axisA, vec3_interface axisB);

class hingeConstraint {

	hingeConstraint_interface entity;
	bool exported;

	this(elementNode cubeA, elementNode cubeB, Vector3f positionA, Vector3f positionB, Vector3f axis) {
		entity = createHingeConstraint(cubeA.entity, cubeB.entity, toVec3(positionA), toVec3(positionB), toVec3(axis), toVec3(axis));
		exported = false;
	}

	this(elementNode cubeA, elementNode cubeB, Vector3f positionA, Vector3f positionB, Vector3f axisA, Vector3f axisB) {
		entity = createHingeConstraint(cubeA.entity, cubeB.entity, toVec3(positionA), toVec3(positionB), toVec3(axisA), toVec3(axisB));
		exported = false;
	}
	

	void enableMotor(bool flag) {
		entity.enableMotor(flag);
	}
	void setLimit(float lower, float upper) {
		entity.setLimit(lower, upper);
	}
	void setMaxMotorImpulse(float power) {
		entity.setMaxMotorImpulse(power);
	}
	void setMotorTarget(float angle, float duration) {
		entity.setMotorTarget(angle, duration);
	}
	void destroy() {
		entity.destroy();
		exported = true;
		entity = null;
	}

	~this() {
		if(exported == false) {
			entity.destroy();
		}
	}
}


//========[generic6DofConstraint]========

extern (C++) {
	interface generic6DofConstraint_interface {
		float getAngle(int index);
		void setAngularLimit(vec3 lower, vec3 upper);
		void setLinearLimit(vec3 lower, vec3 upper);
		void setRotationalMotor(int index);
		void setLinearMotor(int index);
		void setMaxRotationalMotorForce(int index, float force);
		void setMaxLinearMotorForce(vec3 force);
		void setRotationalTargetVelocity(vec3 velocity);
		void setLinearTargetVelocity(vec3 velocity);
		void destroy();
	}
}

extern (C) generic6DofConstraint_interface createGeneric6DofConstraint(elementNode_interface elemA, elementNode_interface elemB, vec3 positionA, vec3 positionB, quat rotation);


//-------------------------------------------------------------------------------------------------------

extern (C++) {
	interface btRigidBody {
	}
	elementManager_interface getCubeshape();
	elementManager_interface getPlaneshape();
}

extern (C) {
	btRigidBody createBoxBody(parameterPack input);
	btRigidBody createPlaneBody(parameterPack input);
	btRigidBody createConvexHullShapeBody(parameterPack input);
}

