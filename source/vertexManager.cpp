#include "vertexManager.hpp"

vertex::vertex(){
}

vertex::vertex	(GLfloat positionX, GLfloat positionY, GLfloat positionZ,
				GLfloat normalX, GLfloat normalY, GLfloat normalZ,
				GLfloat colorR, GLfloat colorG, GLfloat colorB)
				:positionX(positionX), positionY(positionY), positionZ(positionZ),
				 normalX(normalX), normalY(normalY), normalZ(normalZ),
				 colorR(colorR), colorG(colorG), colorB(colorB){
}

vertex::vertex(const vertex& rhs)
				:positionX(rhs.positionX), positionY(rhs.positionY), positionZ(rhs.positionZ),
				 normalX(rhs.normalX), normalY(rhs.normalY), normalZ(rhs.normalZ),
				 colorR(rhs.colorR), colorG(rhs.colorG), colorB(rhs.colorB){
}

vertex::~vertex(){
}

GLuint vertexBufferObject;
std::vector <vertex> vertexBufferArray;

void initVBO(){
	glGenBuffers(1, &vertexBufferObject);
}

void registervertex(std::shared_ptr<std::vector<std::shared_ptr<vertex>>> input, std::vector<GLuint>* arrayaddr){

	for(auto elem: *input){
		auto itr = std::find(vertexBufferArray.begin(), vertexBufferArray.end(), *elem);
		if( itr != vertexBufferArray.end() ){
			arrayaddr->push_back(std::distance(vertexBufferArray.begin(), itr));
		}else{
			vertexBufferArray.push_back(*elem);
			arrayaddr->push_back(vertexBufferArray.size() -1);
		}
	}


	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, vertexBufferArray.size() * sizeof(vertex), &vertexBufferArray[0], GL_STATIC_DRAW);
}


void vertexManager::addVertex(GLfloat positionX, GLfloat positionY, GLfloat positionZ,
				GLfloat normalX, GLfloat normalY, GLfloat normalZ,
				GLfloat colorR, GLfloat colorG, GLfloat colorB){
	vertexList->push_back(std::make_shared<vertex>(positionX, positionY, positionZ, normalX, normalY, normalZ, colorR, colorG, colorB));
}

std::shared_ptr<std::vector<std::shared_ptr<vertex>>> vertexManager::getList(){
	return vertexList;
}

extern "C" vertexManager* createVertexManager(){
	return new vertexManager();
}


vertexManager::vertexManager(){
	vertexList = std::make_shared<std::vector<std::shared_ptr<vertex>>>();
}

void vertexManager::destroy(){
	delete this;
}

vertexManager::~vertexManager(){
}

