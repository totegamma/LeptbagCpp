TARGET := connectingTest

$(TARGET): $(TARGET).d
	g++ -shared nyao.cpp -I /usr/local/include/ -o libnyao.so
	dmd -c $(TARGET).d
	dmd $(TARGET).o -L-lnyao -L-L./
