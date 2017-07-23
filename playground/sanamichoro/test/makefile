TARGET := connectingTest

$(TARGET): $(TARGET).o libnyao.so
	dmd $(TARGET).o -L-lnyao -L-L./

$(TARGET).o: $(TARGET).d
	dmd -c $(TARGET).d

libnyao.so: nyao.cpp
	g++ -shared nyao.cpp -I /usr/local/include/ -o libnyao.so
