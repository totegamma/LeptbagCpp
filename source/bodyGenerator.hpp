#ifndef BODYGENERATOR_HPP
#define BODYGENERATOR_HPP

#include "vertexManager.hpp"
#include "utilities/utilities.hpp"
#include <algorithm>
#include <memory>
#include <bullet/btBulletDynamicsCommon.h>

extern btDiscreteDynamicsWorld* dynamicsWorld;

extern "C" btRigidBody* createBoxBody(std::unique_ptr<parameterPack> input);
extern "C" btRigidBody* createPlaneBody(std::unique_ptr<parameterPack> input);
extern "C" btRigidBody* createConvexHullShapeBody(std::unique_ptr<parameterPack> input);

#endif
