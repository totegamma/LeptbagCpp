japari_park: main.o universalVector.o elementNode.o elementManager.o bodyGenerator.o  constraints.o vertexmanage.o shader.o
	g++ -rdynamic --std=c++14 -L /usr/local/lib/ -ldl -l BulletDynamics -l BulletCollision -l LinearMath -l glfw -lglew -framework OpenGL -o japari_park main.o universalVector.o elementNode.o elementManager.o bodyGenerator.o constraints.o vertexmanage.o shader.o

main.o: main.cpp namedArg.hpp
	g++ --std=c++14 -I /usr/local/include/ -I /usr/local/include/bullet/ -I /usr/local/Cellar/glm/0.9.8.4/include -c main.cpp

universalVector.o: universalVector.cpp universalVector.hpp
	g++ --std=c++14 -I /usr/local/include/ -I /usr/local/include/bullet/ -I /usr/local/Cellar/glm/0.9.8.4/include -c universalVector.cpp

elementNode.o: elementNode.cpp elementNode.hpp
	g++ --std=c++14 -I /usr/local/include/ -I /usr/local/include/bullet/ -I /usr/local/Cellar/glm/0.9.8.4/include -c elementNode.cpp

elementManager.o: elementManager.cpp elementManager.hpp elementNode.hpp
	g++ --std=c++14 -I /usr/local/include/ -I /usr/local/include/bullet/ -I /usr/local/Cellar/glm/0.9.8.4/include -c elementManager.cpp

bodyGenerator.o: bodyGenerator.cpp bodyGenerator.hpp namedArg.hpp
	g++ --std=c++14 -I /usr/local/include/ -I /usr/local/include/bullet/ -I /usr/local/Cellar/glm/0.9.8.4/include -c bodyGenerator.cpp

constraints.o: constraints.cpp constraints.hpp
	g++ --std=c++14 -I /usr/local/include/ -I /usr/local/include/bullet/ -I /usr/local/Cellar/glm/0.9.8.4/include -c constraints.cpp

vertexmanage.o: vertexmanage.cpp vertexmanage.hpp
	g++ --std=c++14 -I /usr/local/include/ -I /usr/local/include/bullet/ -I /usr/local/Cellar/glm/0.9.8.4/include -c vertexmanage.cpp

shader.o: shader.cpp shader.hpp
	g++ --std=c++14 -I /usr/local/include/ -I /usr/local/include/bullet/ -I /usr/local/Cellar/glm/0.9.8.4/include -c shader.cpp

clean:
	rm -f main.o universalVector.o commonshape.o cubeshape.o floorshape.o constraints.o vertexmanage.o shader.o
