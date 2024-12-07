CC = gcc
TARGET = usock_server usock_client
all: $(TARGET)
usock_server: lookup5.c usock_server.c
	$(CC) lookup5.c usock_server.c -o usock_server
usock_client: lookup7.c main.c
	$(CC) lookup7.c main.c -o usock_client
clean:
	rm -f usock_server usock_client
