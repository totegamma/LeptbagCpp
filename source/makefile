
PROG := japari_park
SRCS := main.cpp elementNode.cpp elementManager.cpp bodyGenerator.cpp vertexManager.cpp constraints.cpp shader.cpp primitiveShape.cpp utilities/universalVector.cpp utilities/universalString.cpp utilities/parameterPack.cpp
OBJS := $(SRCS:%.cpp=%.o)

DEPS := $(SRCS:%.cpp=%.depend)

UNAME := $(shell uname)

# Linuxの場合
ifeq ($(UNAME),Linux)
CC := g++
CCFLAGS := -std=c++17 -rdynamic -Wall -g -O0
INCLUDEPATH :=
LIBPATH :=
LIBS := -lGL -lGLU -lglfw3 -lXrandr -lXinerama -lXcursor -lXi -lXxf86vm -lX11 -lrt -lm -lpthread -lGLEW -ldl -lBulletDynamics -lBulletCollision -lLinearMath
endif

# Macの場合
ifeq ($(UNAME),Darwin)
CC := g++-7
CCFLAGS := -std=c++17 -rdynamic -Wall -g -O0
INCLUDEPATH := 
LIBPATH := -L/usr/local/lib
LIBS := -ldl -lBulletDynamics -lBulletCollision -lLinearMath -lglfw -lglew -framework OpenGL
endif


all: $(DEPENDS) $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CCFLAGS) -o $@ $^ $(LIBPATH) $(LIBS)

.cpp.o:
	$(CC) $(CCFLAGS) $(INCLUDEPATH) -MMD -MP -MF $(<:%.cpp=%.depend) -c $< -o $(<:%.cpp=%.o)



.PHONY: clean
clean:
	$(RM) $(PROG) $(OBJS) $(DEPS)

-include $(DEPS)

