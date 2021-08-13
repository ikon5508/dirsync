all: server client

sandbox: sandbox.c libmemory.o logging.h logging.c
	cc -g -o sandbox sandbox.c libmemory.o logging.c

server: server.c server.h socket.o libmemory.o libmemory.h shared.c shared.h logging.h logging.c
	cc -g -o server server.c socket.o libmemory.o shared.c logging.c

client: client.c client.h libmemory.o libmemory.h socket.o socket.h shared.c shared.h logging.h logging.c
	cc -g -o  client client.c libmemory.o socket.o shared.c logging.c

libmemory.o: libmemory.c libmemory.h
	cc  -g -o libmemory.o -c libmemory.c

socket.o: socket.c
	cc -g -o socket.o -c socket.c

clean:
	rm -vf *.o
	rm -v sandbox

