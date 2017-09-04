#include "constraints.hpp"

//hingeConstraint------------------------------------------

int hingeConstraint::count = 0;

hingeConstraint::hingeConstraint(elementNode* elemA_rawp, elementNode* elemB_rawp, vec3* positionA_rawp, vec3* positionB_rawp, vec3* axisA_rawp, vec3* axisB_rawp){

	auto positionA = std::unique_ptr<vec3>(positionA_rawp);
	auto positionB = std::unique_ptr<vec3>(positionB_rawp);
	auto axisA = std::unique_ptr<vec3>(axisA_rawp);
	auto axisB = std::unique_ptr<vec3>(axisB_rawp);

	hinge = new btHingeConstraint(	*(elemA_rawp->getBody()),
									*(elemB_rawp->getBody()),
									positionA->toBullet(),
									positionB->toBullet(),
									axisA->toBullet(),
									axisB->toBullet());
	dynamicsWorld->addConstraint(hinge, true);

	count++;
}

hingeConstraint::~hingeConstraint(){
	count--;
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
	delete this;
}

hingeConstraint* createHingeConstraint(elementNode* elemA_rawp, elementNode* elemB_rawp, vec3* positionA_rawp, vec3* positionB_rawp, vec3* axisA_rawp, vec3* axisB_rawp){
	return new hingeConstraint(elemA_rawp, elemB_rawp, positionA_rawp, positionB_rawp, axisA_rawp, axisB_rawp);//XXX 未確認
}

//----------------------------------------------------------



//generic6dofConstraint-------------------------------------

int generic6DofConstraint::count = 0;

generic6DofConstraint::generic6DofConstraint(elementNode* elemA_rawp, elementNode* elemB_rawp, vec3* positionA_rawp, vec3* positionB_rawp, quat* rotation_rawp){

	auto positionA = std::unique_ptr<vec3>(positionA_rawp);
	auto positionB = std::unique_ptr<vec3>(positionB_rawp);
	auto rotation = std::unique_ptr<quat>(rotation_rawp);



	// それぞれの物体の重心を原点としてローカル座標をとる。
	btTransform frameInA, frameInB;
	frameInA = elemA_rawp->getBody()->getCenterOfMassTransform();
	frameInB = elemB_rawp->getBody()->getCenterOfMassTransform();

	btQuaternion quatToBullet = btQuaternion(sin(-M_PI/4),0,0,cos(-M_PI/4));
	frameInA.setRotation(frameInA.getRotation().inverse() * rotation->toBullet() * quatToBullet);
	frameInB.setRotation(frameInB.getRotation().inverse() * rotation->toBullet() * quatToBullet);

	// デフォルトの関節の接点をローカル座標で指定する
	frameInA.setOrigin(positionA->toBullet());
	frameInB.setOrigin(positionB->toBullet());

	// 6Dofを生成
	//最後のboolはangularLimit, linearLimtitをAのローカル座標内で規定するならtrue．B内ならfalse．(たぶん．以下参照)
	//https://gamedev.stackexchange.com/questions/54349/what-are-frame-a-and-frame-b-in-btgeneric6dofconstraints-constructor-for
	gen6Dof = new btGeneric6DofConstraint(*(elemA_rawp->getBody()), *(elemB_rawp->getBody()), frameInA, frameInB, false);

	//elemAとelemBの衝突判定を無効にする
	//new btGeneric6dofConstraintの後に実行しないと働かない
	//void btRigidBody*->setIgnoreCollisionCheck( btRigidBody*, bool );
	elemA_rawp->getBody()->setIgnoreCollisionCheck(elemB_rawp->getBody(), true );

	dynamicsWorld->addConstraint(gen6Dof);

	count++;

}

generic6DofConstraint::~generic6DofConstraint(){
	count--;
}


float generic6DofConstraint::getAngle(int index){
	gen6Dof->calculateTransforms();
	return gen6Dof->getAngle(index);
}


void generic6DofConstraint::setAngularLimit(vec3* lower_rawp, vec3* upper_rawp){
	//lower == upperでロック
	//lower > upperで制限なし．Linearも同様．

	auto lower = std::unique_ptr<vec3>(lower_rawp);
	auto upper= std::unique_ptr<vec3>(upper_rawp);

	gen6Dof->setAngularLowerLimit(lower->toBullet());
	gen6Dof->setAngularUpperLimit(upper->toBullet());
}

void generic6DofConstraint::setLinearLimit(vec3* lower_rawp, vec3* upper_rawp){

	auto lower = std::unique_ptr<vec3>(lower_rawp);
	auto upper= std::unique_ptr<vec3>(upper_rawp);

	gen6Dof->setLinearLowerLimit(lower->toBullet());
	gen6Dof->setLinearUpperLimit(upper->toBullet());
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

void generic6DofConstraint::setMaxLinearMotorForce(vec3* force_rawp){

	auto force = std::unique_ptr<vec3>(force_rawp);

	gen6Dof->getTranslationalLimitMotor()->m_maxMotorForce = force->toBullet();
}

void generic6DofConstraint::setRotationalTargetVelocity(vec3* velocity_rawp){

	auto velocity = std::unique_ptr<vec3>(velocity_rawp);

	gen6Dof->getRotationalLimitMotor(0)->m_targetVelocity = velocity->getx();
	gen6Dof->getRotationalLimitMotor(1)->m_targetVelocity = velocity->gety();
	gen6Dof->getRotationalLimitMotor(2)->m_targetVelocity = velocity->getz();
}

void generic6DofConstraint::setLinearTargetVelocity(vec3* velocity_rawp){

	auto velocity = std::unique_ptr<vec3>(velocity_rawp);

	gen6Dof->getTranslationalLimitMotor()->m_targetVelocity = velocity->toBullet();
}





void generic6DofConstraint::destroy(){
	dynamicsWorld->removeConstraint(gen6Dof);
	delete this;
}


generic6DofConstraint* createGeneric6DofConstraint(elementNode* elemA_rawp, elementNode* elemB_rawp, vec3* positionA_rawp, vec3* positionB_rawp, quat* rotation_rawp){
	return new generic6DofConstraint(elemA_rawp, elemB_rawp, positionA_rawp, positionB_rawp, rotation_rawp);//XXX 未確認
}

//----------------------------------------------------------
