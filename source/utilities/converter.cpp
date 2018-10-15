#include "converter.hpp"

glm::vec3 toGlmVec3(Eigen::Vector3f in) {
	return glm::vec3(in.x(), in.y(), in.z());
}

glm::vec3 toGlmVec3(Eigen::Vector3f* in) {
	return glm::vec3(in->x(), in->y(), in->z());
}

glm::quat toGlmQuat(Eigen::Quaternionf in) {
	return glm::quat(in.w(), in.x(), in.y(), in.z());
}

glm::quat toGlmQuat(Eigen::Quaternionf* in) {
	return glm::quat(in->w(), in->x(), in->y(), in->z());
}

btVector3 toBtVec3(Eigen::Vector3f in) {
	return btVector3(in.x(), in.y(), in.z());
}

btVector3 toBtVec3(Eigen::Vector3f* in) {
	return btVector3(in->x(), in->y(), in->z());
}

btQuaternion toBtQuat(Eigen::Quaternionf in) {
	return btQuaternion(in.x(), in.y(), in.z(), in.w());
}

btQuaternion toBtQuat(Eigen::Quaternionf* in) {
	return btQuaternion(in->x(), in->y(), in->z(), in->w());
}
