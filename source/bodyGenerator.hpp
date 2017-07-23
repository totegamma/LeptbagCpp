#ifndef BODYGENERATOR_HPP
#define BODYGENERATOR_HPP

#include "vertexmanager.hpp"
#include "utilities/utilities.hpp"
#include <algorithm>

btRigidBody* createBoxBody(parameterPack* input);
btRigidBody* createPlaneBody(parameterPack* input);
//btRigidBody* createConvexHullShapeBody(parameterPack* input);

#endif
