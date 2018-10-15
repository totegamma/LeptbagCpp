
PROG := LeptBag
SRCS := main.cpp elementNode.cpp elementManager.cpp bodyGenerator.cpp vertexManager.cpp constraints.cpp shader.cpp font.cpp primitiveShape.cpp misc.cpp utilities/parameterPack.cpp utilities/converter.cpp
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
CC := g++-8
CCFLAGS := -std=c++17 -rdynamic -Wall -g -O0 -DGLM_ENABLE_EXPERIMENTAL
INCLUDEPATH := -I/usr/local/include/freetype2
LIBPATH := -L/usr/local/lib
LIBS := -ldl -lfreetype -lBulletDynamics -lBulletCollision -lLinearMath -lglfw -lglew -framework OpenGL
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

