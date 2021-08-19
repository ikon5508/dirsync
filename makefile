dirsync: server.c server.h socket.o socket.h libmemory.o libmemory.h shared.c shared.h logging.h logging.c client.c client.h main.c main.h
	cc -g -o dirsync server.c socket.o libmemory.o shared.c logging.c client.c main.c

libmemory.o: libmemory.c libmemory.h
	cc  -g -o libmemory.o -c libmemory.c

socket.o: socket.c
	cc -g -o socket.o -c socket.c

clean:
	rm -vf *.o

