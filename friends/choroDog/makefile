
TARGET := chorodog



$(TARGET): $(TARGET).d
	dmd -c libevo.d -fPIC
	dmd -c $(TARGET).d -fPIC -J.
	dmd -c japariSDK/japarilib.d -fPIC
	dmd -of$(TARGET).friends $(TARGET).o japarilib.o libevo.o -shared -defaultlib=libphobos2.so -L-undefined -Ldynamic_lookup
