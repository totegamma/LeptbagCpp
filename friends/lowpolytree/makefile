
TARGET := lowPolyTree

$(TARGET): $(TARGET).d
	dmd -c $(TARGET).d -fPIC -J.
	dmd -c japariSDK/japarilib.d -fPIC
	dmd -of$(TARGET).friends $(TARGET).o japarilib.o -shared -defaultlib=libphobos2.so -L-undefined -Ldynamic_lookup
