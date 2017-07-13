#ifndef BODYGENERATOR_HPP
#define BODYGENERATOR_HPP

#include "universalVector.hpp"
#include "namedArg.hpp"
#include "vertexmanage.hpp"

template<typename... Args>
btRigidBody* createBoxBody(Args... args);

template<typename... Args>
btRigidBody* createPlaneBody(Args... args);

template<typename... Args>
btRigidBody* createConvexHullShapeBody(Args... args);

#endif
