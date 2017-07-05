#ifndef ELEMENTNODE_HPP
#define ELEMENTNODE_HPP

class elementNode{
	bool isPhysicalBody;
	elementCreator* parent;
	btRigidBody* body;
	vec3 initialPosition;
	vec3 initialSize;
	quat initialQuat;

	public:

	shapePointerObject();
	shapePointerObject(commonshapeObject* parent, bool isPhysical, btRigidBody* body, vec3 posi, vec3 size, quat );
	glm::mat4 loadMatrix();
};



#endif
