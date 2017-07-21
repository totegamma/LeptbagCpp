
dmd -c plugin.d -fPIC
dmd -ofplugin.so plugin.o -shared -defaultlib=libphobos2.so -L-undefined -Ldynamic_lookup
