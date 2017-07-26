japari_park: main.o elementNode.o elementManager.o bodyGenerator.o  constraints.o vertexmanager.o shader.o universalVector.o parameterPack.o universalString.o primitiveShape.o
	g++ -rdynamic --std=c++1z -L /usr/local/lib/ -ldl -l BulletDynamics -l BulletCollision -l LinearMath -l glfw -lglew -framework OpenGL -o japari_park main.o elementNode.o elementManager.o bodyGenerator.o constraints.o vertexmanager.o shader.o universalVector.o parameterPack.o universalString.o primitiveShape.o

main.o: main.cpp 
	g++ --std=c++1z -I /usr/local/include/ -I /usr/local/include/bullet/ -I /usr/local/Cellar/glm/0.9.8.4/include -c main.cpp

elementNode.o: elementNode.cpp elementNode.hpp
	g++ --std=c++1z -I /usr/local/include/ -I /usr/local/include/bullet/ -I /usr/local/Cellar/glm/0.9.8.4/include -c elementNode.cpp

elementManager.o: elementManager.cpp elementManager.hpp elementNode.hpp
	g++ --std=c++1z -I /usr/local/include/ -I /usr/local/include/bullet/ -I /usr/local/Cellar/glm/0.9.8.4/include -c elementManager.cpp

bodyGenerator.o: bodyGenerator.cpp bodyGenerator.hpp 
	g++ --std=c++1z -I /usr/local/include/ -I /usr/local/include/bullet/ -I /usr/local/Cellar/glm/0.9.8.4/include -c bodyGenerator.cpp

constraints.o: constraints.cpp constraints.hpp
	g++ --std=c++1z -I /usr/local/include/ -I /usr/local/include/bullet/ -I /usr/local/Cellar/glm/0.9.8.4/include -c constraints.cpp

vertexmanager.o: vertexmanager.cpp vertexmanager.hpp
	g++ --std=c++1z -I /usr/local/include/ -I /usr/local/include/bullet/ -I /usr/local/Cellar/glm/0.9.8.4/include -c vertexmanager.cpp

shader.o: shader.cpp shader.hpp
	g++ --std=c++1z -I /usr/local/include/ -I /usr/local/include/bullet/ -I /usr/local/Cellar/glm/0.9.8.4/include -c shader.cpp

universalVector.o: utilities/universalVector.cpp
	g++ --std=c++1z -I /usr/local/include/ -I /usr/local/include/bullet/ -I /usr/local/Cellar/glm/0.9.8.4/include -c utilities/universalVector.cpp

parameterPack.o: utilities/parameterPack.cpp
	g++ --std=c++1z -I /usr/local/include/ -I /usr/local/include/bullet/ -I /usr/local/Cellar/glm/0.9.8.4/include -c utilities/parameterPack.cpp

universalString.o: utilities/universalString.cpp
	g++ --std=c++1z -I /usr/local/include/ -I /usr/local/include/bullet/ -I /usr/local/Cellar/glm/0.9.8.4/include -c utilities/universalString.cpp

primitiveShape.o: primitiveShape.cpp primitiveShape.hpp
	g++ -g -O0 --std=c++1z -I /usr/local/include/ -I /usr/local/include/bullet/ -I /usr/local/Cellar/glm/0.9.8.4/include -c primitiveShape.cpp

clean:
	rm -f *.o
