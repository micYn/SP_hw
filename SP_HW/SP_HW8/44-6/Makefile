CC = gcc
TARGET = server client
COMMON_SRC = error_functions.c get_num.c

all: $(TARGET)

server: fifo_seqnum_server.c $(COMMON_SRC)
	gcc -o server fifo_seqnum_server.c $(COMMON_SRC)

client: fifo_seqnum_client.c $(COMMON_SRC)
	gcc -o client fifo_seqnum_client.c $(COMMON_SRC)

clean:
	rm -f $(TARGET)