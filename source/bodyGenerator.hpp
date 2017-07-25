#ifndef BODYGENERATOR_HPP
#define BODYGENERATOR_HPP

#include "vertexmanager.hpp"
#include "utilities/utilities.hpp"
#include <algorithm>
#include <btBulletDynamicsCommon.h>

extern btDiscreteDynamicsWorld* dynamicsWorld;

extern "C" btRigidBody* createBoxBody(parameterPack* input);
extern "C" btRigidBody* createPlaneBody(parameterPack* input);
extern "C" btRigidBody* createConvexHullShapeBody(parameterPack* input);

#endif
