PROG := sayHello
SRCS := main.cpp
INSTALL_DEST := ../../source/plugins/
CC := g++-8
LIBS :=
CCFLAGS := -std=c++17 -DGLM_ENABLE_EXPERIMENTAL -shared -fPIC -undefined dynamic_lookup
INCLUDEPATH := -I/usr/local/include/freetype2
LIBPATH :=

make:
	$(CC) $(INCLUDEPATH) $(CCFLAGS) $(LIBPATH) $(LIBS) -o $(PROG).so $(SRCS)


install:
	cp $(PROG).so $(INSTALL_DEST)
