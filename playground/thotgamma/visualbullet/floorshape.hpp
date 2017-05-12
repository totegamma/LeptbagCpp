#ifndef FLOORSHAPE_HPP
#define FLOORSHAPE_HPP

namespace floorshape{
	GLuint floor_indexBufferObject;
	GLuint floor_instanceMatrixBuffer;

	GLuint floor_indexBufferArray[4] = {
		8, 9, 10, 11
	};

	glm::mat4 floor_instanceMatrixArray[1];



	void init();
	void create(int id);
	void destroy(int id);
	void render(int id);


}


#endif
