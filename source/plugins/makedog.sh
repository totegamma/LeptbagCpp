dmd -c dog.d -fPIC
dmd -oflibdog.so dog.o -shared -defaultlib=libphobos2.so -L-undefined -Ldynamic_lookup
