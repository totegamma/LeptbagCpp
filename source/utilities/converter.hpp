#ifndef CONVERTER_HPP
#define CONVERTER_HPP

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <bullet/btBulletDynamicsCommon.h>

glm::vec3 toGlmVec3(Eigen::Vector3f in);
//glm::vec3 toGlmVec3(Eigen::Vector3f* in);
glm::quat toGlmQuat(Eigen::Quaternionf in);
//glm::quat toGlmQuat(Eigen::Quaternionf* in);
btVector3 toBtVec3(Eigen::Vector3f in);
//btVector3 toBtVec3(Eigen::Vector3f* in);
btQuaternion toBtQuat(Eigen::Quaternionf in);
//btQuaternion toBtQuat(Eigen::Quaternionf* in);

#endif
