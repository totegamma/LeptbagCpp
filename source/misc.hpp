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



#endif
