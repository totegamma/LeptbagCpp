
TARGET := simpleFloor

$(TARGET): $(TARGET).d
	dmd -c $(TARGET).d -fPIC
	dmd -of$(TARGET).friends $(TARGET).o -shared -defaultlib=libphobos2.so -L-undefined -Ldynamic_lookup
