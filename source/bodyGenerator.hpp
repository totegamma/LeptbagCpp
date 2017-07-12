#ifndef BODYGENERATOR_HPP
#define BODYGENERATOR_HPP

#include "universalVector.hpp"

rigidBody* createBoxBody(vec3 position, vec3 scale, quat rotation, btScalar mass);
rigidBody* createPlaneBody(vec3 position, vec3 scale, quat rotation, btScalar mass);
rigidBody* createConvexHullShapeBody(vec3 position, vec3 scale, quat rotation, btScalar mass);

#endif
