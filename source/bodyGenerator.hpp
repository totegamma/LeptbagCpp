#ifndef BODYGENERATOR_HPP
#define BODYGENERATOR_HPP

#include "vertexManager.hpp"
#include "utilities/utilities.hpp"
#include <algorithm>
#include <memory>
#include <bullet/btBulletDynamicsCommon.h>

extern btDiscreteDynamicsWorld* dynamicsWorld;

btRigidBody* createBoxBody(std::unique_ptr<parameterPack> input);
btRigidBody* createPlaneBody(std::unique_ptr<parameterPack> input);
btRigidBody* createConvexHullShapeBody(std::unique_ptr<parameterPack> input);

#endif
