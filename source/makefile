
PROG := japari_park
SRCS := main.cpp elementNode.cpp elementManager.cpp bodyGenerator.cpp vertexManager.cpp constraints.cpp shader.cpp primitiveShape.cpp utilities/universalVector.cpp utilities/universalString.cpp utilities/parameterPack.cpp
OBJS := $(SRCS:%.cpp=%.o)

DEPS := $(SRCS:%.cpp=%.depend)

CC := g++-7
CCFLAGS := -std=c++17 -rdynamic -Wall -g -O0
INCLUDEPATH := 
LIBPATH := -L/usr/local/lib
LIBS := -ldl -l BulletDynamics -l BulletCollision -l LinearMath -l glfw -lglew -framework OpenGL


all: $(DEPENDS) $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CCFLAGS) $(LIBPATH) $(LIBS) -o $@ $^

.cpp.o:
	$(CC) $(CCFLAGS) $(INCLUDEPATH) -MMD -MP -MF $(<:%.cpp=%.depend) -c $< -o $(<:%.cpp=%.o)



.PHONY: clean
clean:
	$(RM) $(PROG) $(OBJS) $(DEPS)

-include $(DEPS)

