
#include "misc.hpp"

extern btDiscreteDynamicsWorld* dynamicsWorld;

float closestRayTest(float fromx, float fromy, float fromz, float tox, float toy, float toz) {
	btVector3 btFrom(fromx, fromy, fromz);
	btVector3 btTo(tox, toy, toz);
	btCollisionWorld::ClosestRayResultCallback res(btFrom, btTo);

	dynamicsWorld->rayTest(btFrom, btTo, res);

	if(!res.hasHit()) return -1;
	return res.m_closestHitFraction;
}
