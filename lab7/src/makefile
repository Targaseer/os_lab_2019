CC=gcc
CFLAG=-I.

all: tcpclient tcpserver udpclient udpserver

tcpclient: tcpclient.c
	$(CC) -o tcpclient tcpclient.c $(CFLAG)

tcpserver: tcpserver.c
	$(CC) -o tcpserver tcpserver.c $(CFLAG)
	
udpclient: udpclient.c
	$(CC) -o udpclient udpclient.c $(CFLAG)

udpserver: udpserver.c
	$(CC) -o udpserver udpserver.c $(CFLAG)

clean:
	rm tcpclient tcpserver udpclient udpserver