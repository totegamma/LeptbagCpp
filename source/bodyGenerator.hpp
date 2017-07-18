#ifndef BODYGENERATOR_HPP
#define BODYGENERATOR_HPP

#include "universalVector.hpp"
#include "vertexmanage.hpp"
#include "utilities/utilities.hpp"
#include <algorithm>

template<typename... Args>
btRigidBody* createBoxBody(Args... args);

template<typename... Args>
btRigidBody* createPlaneBody(Args... args);

template<typename... Args>
btRigidBody* createConvexHullShapeBody(Args... args);

#endif
