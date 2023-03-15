CC=gcc
CFLAGS=-I.

client: client.c requests.c helpers.c buffer.c parson.c response.c
	$(CC) -o client client.c requests.c parson.c helpers.c buffer.c response.c -Wall

run: client
	./client

clean:
	rm -f *.o client
