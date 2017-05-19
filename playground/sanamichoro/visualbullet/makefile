visualBullet: main.o cubeshape.o floorshape.o vertexmanage.o shader.o
		g++ --std=c++11 -I /usr/local/include/ -I /usr/local/include/bullet/ -I /usr/local/Cellar/glew/2.0.0/include/ -I /usr/local/Cellar/glm/0.9.8.4/include -I /usr/local/Cellar/glew/2.0.0/lib/ -L /usr/local/lib/ -L /usr/local/Cellar/glew/2.0.0/lib/ -l BulletDynamics -l BulletCollision -l LinearMath -l glfw -lglew -framework OpenGL -o visualBullet main.o shader.o cubeshape.o floorshape.o vertexmanage.o


main.o: main.cpp
	g++ --std=c++11 -I /usr/local/include/ -I /usr/local/include/bullet/ -I /usr/local/Cellar/glew/2.0.0/include/ -I /usr/local/Cellar/glm/0.9.8.4/include -c main.cpp

cubeshape.o: cubeshape.cpp
	g++ --std=c++11 -I /usr/local/include/ -I /usr/local/include/bullet/ -I /usr/local/Cellar/glew/2.0.0/include/ -I /usr/local/Cellar/glm/0.9.8.4/include -c cubeshape.cpp

floorshape.o: floorshape.cpp
	g++ --std=c++11 -I /usr/local/include/ -I /usr/local/include/bullet/  -I /usr/local/Cellar/glew/2.0.0/include/ -I /usr/local/Cellar/glm/0.9.8.4/include -c floorshape.cpp

vertexmanage.o: vertexmanage.cpp
	g++ --std=c++11 -I /usr/local/include/ -I /usr/local/include/bullet/ -I /usr/local/Cellar/glew/2.0.0/include/ -I /usr/local/Cellar/glm/0.9.8.4/include -c vertexmanage.cpp

shader.o: shader.cpp
	g++ --std=c++11 -I /usr/local/include/ -I /usr/local/include/bullet/ -I /usr/local/Cellar/glew/2.0.0/include/ -I /usr/local/Cellar/glm/0.9.8.4/include -c shader.cpp

clean:
	rm -f main.o cubeshape.o floorshape.o vertexmanage.o shader.o
