#ifndef ELEMENTCREATOR_HPP
#define ELEMENTCREATOR_HPP

class elementCreator{
	GLuint indexBufferObject;
	GLuint instanceMatrixBuffer;

	std::vector<GLuint> indexBufferArray;
	std::vector<vertex> elementData;
	std::vector<elementNode*> elements;

	public:

	elementCreator();
	~elementCreator();

	virtual void addVertex(vertex &newvertex);
	virtual void registerToSystem();
	virtual elementNode* create();
	virtual elementNode* create(vec3 &position, vec3 &size, quat &rotate);
	virtual elementNode* create(vec3 &position, vec3 &size, quat &rotate, float mass);
	void destroy(int id);
	void render();
};

#endif
