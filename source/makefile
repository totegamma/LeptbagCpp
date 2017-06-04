japari_park: main.o universalVector.o commonshape.o cubeshape.o floorshape.o constraints.o vertexmanage.o shader.o
	g++ -rdynamic --std=c++11 -L /usr/local/lib/ -ldl -l BulletDynamics -l BulletCollision -l LinearMath -l glfw -lglew -framework OpenGL -o japari_park main.o commonshape.o cubeshape.o floorshape.o constraints.o vertexmanage.o shader.o

main.o: main.cpp
	g++ --std=c++11 -I /usr/local/include/ -I /usr/local/include/bullet/ -I /usr/local/Cellar/glm/0.9.8.4/include -c main.cpp

universalVector.o: universalVector.cpp universalVector.hpp
	g++ --std=c++11 -I /usr/local/include/ -I /usr/local/include/bullet/ -I /usr/local/Cellar/glm/0.9.8.4/include -c universalVector.cpp

commonshape.o: commonshape.cpp commonshape.hpp
	g++ --std=c++11 -I /usr/local/include/ -I /usr/local/include/bullet/ -I /usr/local/Cellar/glm/0.9.8.4/include -c commonshape.cpp

cubeshape.o: cubeshape.cpp cubeshape.hpp
	g++ --std=c++11 -I /usr/local/include/ -I /usr/local/include/bullet/ -I /usr/local/Cellar/glm/0.9.8.4/include -c cubeshape.cpp

floorshape.o: floorshape.cpp floorshape.hpp
	g++ --std=c++11 -I /usr/local/include/ -I /usr/local/include/bullet/ -I /usr/local/Cellar/glm/0.9.8.4/include -c floorshape.cpp

constraints.o: constraints.cpp constraints.hpp
	g++ --std=c++11 -I /usr/local/include/ -I /usr/local/include/bullet/ -I /usr/local/Cellar/glm/0.9.8.4/include -c constraints.cpp

vertexmanage.o: vertexmanage.cpp vertexmanage.hpp
	g++ --std=c++11 -I /usr/local/include/ -I /usr/local/include/bullet/ -I /usr/local/Cellar/glm/0.9.8.4/include -c vertexmanage.cpp

shader.o: shader.cpp shader.hpp
	g++ --std=c++11 -I /usr/local/include/ -I /usr/local/include/bullet/ -I /usr/local/Cellar/glm/0.9.8.4/include -c shader.cpp

clean:
	rm -f main.o cubeshape.o floorshape.o vertexmanage.o shader.o
