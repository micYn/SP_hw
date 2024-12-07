CC = gcc
TARGET = isock_server isock_client
all: $(TARGET)
isock_server: lookup5.c isock_server.c
	$(CC) lookup5.c isock_server.c -o isock_server
isock_client: lookup8.c main.c
	$(CC) lookup8.c main.c -o isock_client
clean:
	rm -f isock_server isock_client
