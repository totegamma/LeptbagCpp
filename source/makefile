visualBullet: main.o cubeshape.o floorshape.o vertexmanage.o shader.o
	g++ -rdynamic --std=c++11 -I /usr/local/include/ -I /usr/local/include/bullet/ -I /usr/local/Cellar/glm/0.9.8.4/include -L /usr/local/lib/ -ldl -l BulletDynamics -l BulletCollision -l LinearMath -l glfw -lglew -framework OpenGL -o visualBullet main.o cubeshape.o floorshape.o vertexmanage.o shader.o

main.o: main.cpp
	g++ --std=c++11 -I /usr/local/include/ -I /usr/local/include/bullet/ -I /usr/local/Cellar/glm/0.9.8.4/include -c main.cpp

cubeshape.o: cubeshape.cpp
	g++ --std=c++11 -I /usr/local/include/ -I /usr/local/include/bullet/ -I /usr/local/Cellar/glm/0.9.8.4/include -c cubeshape.cpp

floorshape.o: floorshape.cpp
	g++ --std=c++11 -I /usr/local/include/ -I /usr/local/include/bullet/ -I /usr/local/Cellar/glm/0.9.8.4/include -c floorshape.cpp

vertexmanage.o: vertexmanage.cpp
	g++ --std=c++11 -I /usr/local/include/ -I /usr/local/include/bullet/ -I /usr/local/Cellar/glm/0.9.8.4/include -c vertexmanage.cpp

shader.o: shader.cpp
	g++ --std=c++11 -I /usr/local/include/ -I /usr/local/include/bullet/ -I /usr/local/Cellar/glm/0.9.8.4/include -c shader.cpp

clean:
	rm -f main.o cubeshape.o floorshape.o vertexmanage.o shader.o
