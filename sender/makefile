CC = g++
CFLAGS = -lssl -lcrypto -lgmp

rec:main.o receiver.o rsa.o
	$(CC) -o send main.o sender.o rsa.o $(CFLAGS)
main.o:main.cpp
	$(CC) -c main.cpp $(CFLAGS)
receiver.o:sender.cpp
	$(CC) -c sender.cpp $(CFLAGS)
rsa.o:rsa.cpp
	$(CC) -c rsa.cpp $(CFLAGS)
clean:
	rm rec main.o sender.o rsa.o
