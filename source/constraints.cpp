#include "constraints.hpp"

//hingeConstraint------------------------------------------

hingeConstraint::hingeConstraint(){
}

hingeConstraint::hingeConstraint(elementNode* elemA, elementNode* elemB, vec3 &positionA, vec3 &positionB, vec3 &axisA, vec3 &axisB){

	hinge = new btHingeConstraint(*(elemA->getBody()), *(elemB->getBody()), positionA.toBullet(), positionB.toBullet(), axisA.toBullet(), axisB.toBullet());
	dynamicsWorld->addConstraint(hinge, true);
}

void hingeConstraint::enableMotor(bool flag){
	hinge->enableMotor(flag);
}

void hingeConstraint::setLimit(float lower, float upper){
	hinge->setLimit(lower, upper);
}

void hingeConstraint::setMaxMotorImpulse(float power){
	hinge->setMaxMotorImpulse(power);
}

void hingeConstraint::setMotorTarget(float angle, float duration){
	hinge->setMotorTarget(angle, duration);
}

void hingeConstraint::destroy(){
	dynamicsWorld->removeConstraint(hinge);
}

hingeConstraint* hingeConstraint_create(elementNode* elemA, elementNode* elemB, vec3 &positionA, vec3 &positionB, vec3 &axisA, vec3 &axisB){
	return new hingeConstraint(elemA, elemB, positionA, positionB, axisA, axisB);
}

//----------------------------------------------------------



//generic6dofConstraint-------------------------------------

generic6DofConstraint::generic6DofConstraint(){
}

generic6DofConstraint::generic6DofConstraint(elementNode* elemA, elementNode* elemB, vec3 &positionA, vec3 &positionB, quat &rotation){



	// それぞれの物体の重心を原点としてローカル座標をとる。
	btTransform frameInA, frameInB;
	frameInA = elemA->getBody()->getCenterOfMassTransform();
	frameInB = elemB->getBody()->getCenterOfMassTransform();

	btQuaternion quatToBullet = btQuaternion(sin(-M_PI/4),0,0,cos(-M_PI/4));
	frameInA.setRotation(frameInA.getRotation().inverse() * rotation.toBullet() * quatToBullet);
	frameInB.setRotation(frameInB.getRotation().inverse() * rotation.toBullet() * quatToBullet);

	// デフォルトの関節の接点をローカル座標で指定する
	frameInA.setOrigin(positionA.toBullet());
	frameInB.setOrigin(positionB.toBullet());

	// 6Dofを生成
	//最後のboolはangularLimit, linearLimtitをAのローカル座標内で規定するならtrue．B内ならfalse．(たぶん．以下参照)
	//https://gamedev.stackexchange.com/questions/54349/what-are-frame-a-and-frame-b-in-btgeneric6dofconstraints-constructor-for
	gen6Dof = new btGeneric6DofConstraint(*(elemA->getBody()), *(elemB->getBody()), frameInA, frameInB, false);
	/*
	gen6Dof->setAngularLowerLimit(btVector3(0,0,0));
	gen6Dof->setAngularUpperLimit(btVector3(0,0,0));
	gen6Dof->setLinearLowerLimit(btVector3(0,0,0));
	gen6Dof->setLinearUpperLimit(btVector3(0,0,0));
	*/

	//elemAとelemBの衝突判定を無効にする
	//new btGeneric6dofConstraintの後に実行しないと働かない
	//void btRigidBody*->setIgnoreCollisionCheck( btRigidBody*, bool );
	elemA->getBody()->setIgnoreCollisionCheck( elemB->getBody(), true );

	dynamicsWorld->addConstraint(gen6Dof);

}

void generic6DofConstraint::setAngularLimit(vec3 &lower, vec3 &upper){
	//lower == upperでロック
	//lower > upperで制限なし．Linearも同様．
	gen6Dof->setAngularLowerLimit(lower.toBullet());
	gen6Dof->setAngularUpperLimit(upper.toBullet());
}

void generic6DofConstraint::setLinearLimit(vec3 &lower, vec3 &upper){
	gen6Dof->setLinearLowerLimit(lower.toBullet());
	gen6Dof->setLinearUpperLimit(upper.toBullet());
}

void generic6DofConstraint::setRotationalMotor(int index){
	// 0,1,2 -> x,y,z (?)
	gen6Dof->getRotationalLimitMotor(index)->m_enableMotor = true;
}

void generic6DofConstraint::setLinearMotor(int index){
	gen6Dof->getTranslationalLimitMotor()->m_enableMotor[index] = true;
}

void generic6DofConstraint::setMaxRotationalMotorForce(int index, float force){
	gen6Dof->getRotationalLimitMotor(index)->m_maxMotorForce = force;
}

void generic6DofConstraint::setMaxLinearMotorForce(vec3 &force){
	gen6Dof->getTranslationalLimitMotor()->m_maxMotorForce = force.toBullet();
}

void generic6DofConstraint::setRotationalTargetVelocity(vec3 &velocity){
	gen6Dof->getRotationalLimitMotor(0)->m_targetVelocity = velocity.getx();
	gen6Dof->getRotationalLimitMotor(1)->m_targetVelocity = velocity.gety();
	gen6Dof->getRotationalLimitMotor(2)->m_targetVelocity = velocity.getz();
}

void generic6DofConstraint::setLinearTargetVelocity(vec3 &velocity){
	gen6Dof->getTranslationalLimitMotor()->m_targetVelocity = velocity.toBullet();
}

void generic6DofConstraint::destroy(){
	dynamicsWorld->removeConstraint(gen6Dof);
}

generic6DofConstraint* generic6DofConstraint_create(elementNode* elemA, elementNode* elemB, vec3 &positionA, vec3 &positionB, quat &rotation){
	return new generic6DofConstraint(elemA, elemB, positionA, positionB, rotation);
}

//----------------------------------------------------------
