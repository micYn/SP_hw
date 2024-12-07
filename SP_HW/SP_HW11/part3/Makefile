CC = gcc
TARGET = udp_server udp_client
all: $(TARGET)
udp_server: lookup5.c udp_server.c
	$(CC) lookup5.c udp_server.c -o udp_server
udp_client: lookup9.c main.c
	$(CC) lookup9.c main.c -o udp_client
clean:
	rm -f udp_server udp_client