CC=gcc
CFLAG=-I.

all : server client

server :
	$(CC) -lpthread server.c -L/projects/os_lab_2019/lab6/multmodulo -lmultmodulo -o ./server $(CFLAG)

client : 
	$(CC) -lpthread client.c -L/projects/os_lab_2019/lab6/multmodulo -lmultmodulo -o ./client $(CFLAG)

server.o :
	$(CC) -lpthread server.c -o server.o $(CFLAG)

client.o : 
	$(CC) -lpthread client.c -o client.o $(CFLAG)

clean :
	rm server client