CC = g++
CFLAGS = -lssl -lcrypto -lgmp

rec:main.o receiver.o rsa.o
	$(CC) -o rec main.o receiver.o rsa.o $(CFLAGS)
main.o:main.cpp
	$(CC) -c main.cpp $(CFLAGS)
receiver.o:receiver.cpp
	$(CC) -c receiver.cpp $(CFLAGS)
rsa.o:rsa.cpp
	$(CC) -c rsa.cpp $(CFLAGS)
clean:
	rm rec main.o receiver.o rsa.o
