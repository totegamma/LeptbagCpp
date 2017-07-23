TARGET := chorodog

$(TARGET): $(TARGET).d
	dmd -c $(TARGET).d -fPIC
	dmd -of$(TARGET).friends $(TARGET).o diffEvo.o -shared -defaultlib=libphobos2.so -L-undefined -Ldynamic_lookup

diffEvo.o: diffEvo.d
	dmd -c diffEvo.d -fPIC
