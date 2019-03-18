PROG := lowPolyFox
SRCS := main.cpp
INSTALL_DEST := ../../source/plugins/
CC := g++-8
LIBS := -ljson11
CCFLAGS := -std=c++17 -Wall -DGLM_ENABLE_EXPERIMENTAL -shared -fPIC -undefined dynamic_lookup
INCLUDEPATH := -I/usr/local/include/freetype2 -I/usr/local/include -I/usr/local/include/bullet -I/usr/local/include/eigen3
LIBPATH :=

make:
	$(CC) $(INCLUDEPATH) $(CCFLAGS) $(LIBPATH) $(LIBS) -o $(PROG).so $(SRCS)


install:
	cp $(PROG).so $(INSTALL_DEST)
