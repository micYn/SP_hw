CC = gcc
TARGET = server client
COMMON_SRC = error_functions.c get_num.c

all: $(TARGET)

server: server.c $(COMMON_SRC)
	gcc -o server server.c $(COMMON_SRC)

client: client.c $(COMMON_SRC)
	gcc -o client client.c $(COMMON_SRC)

clean:
	rm -f $(TARGET)
