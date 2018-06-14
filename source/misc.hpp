#ifndef MISC_HPP
#define MISC_HPP


#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <bullet/btBulletDynamicsCommon.h>

#include "utilities/utilities.hpp"


extern "C" float closestRayTest(float fromx, float fromy, float fromz, float tox, float toy, float toz);

extern "C" void registerKeyCallback(void (*func)(int key, int scancode, int action, int mods));
extern "C" void registerMouseMoveCallback(void (*func)(double xpos, double ypos));
extern "C" void registerMouseButtonCallback(void (*func)(int button, int action, int mods));
extern "C" void registerWindowResizeCallback(void (*func)(int width, int height));
extern "C" int getWindowWidth();
extern "C" int getWindowHeight();
extern "C" void setCursorPos(float x, float y);
extern "C" int requestCameraAccess(void (*func)(void));
extern "C" void updateCamera(float posx, float posy, float posz, float horizAng, float vertAng, float FoV);


#endif
