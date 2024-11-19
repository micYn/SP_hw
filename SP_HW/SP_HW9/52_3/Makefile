CC = gcc
TARGET = server client

all: $(TARGET)

server: posixmq_file_server.c
	$(CC) -o server posixmq_file_server.c
client: posixmq_file_client.c
	$(CC) -o client posixmq_file_client.c

clean:
	rm -f $(TARGET)