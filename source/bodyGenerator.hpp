#ifndef BODYGENERATOR_HPP
#define BODYGENERATOR_HPP

#include "vertexmanager.hpp"
#include "utilities/utilities.hpp"
#include <algorithm>
#include <btBulletDynamicsCommon.h>

extern btDiscreteDynamicsWorld* dynamicsWorld;

btRigidBody* createBoxBody(parameterPack* input);
btRigidBody* createPlaneBody(parameterPack* input);
//btRigidBody* createConvexHullShapeBody(parameterPack* input);

#endif
