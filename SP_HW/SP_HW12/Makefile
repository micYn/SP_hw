CC = gcc
TARGET = shm_server shm_client

all: $(TARGET)

shm_server: lookup5.c shm_server.c
	gcc -o shm_server lookup5.c shm_server.c

shm_client: lookup6.c main.c
	gcc -o shm_client lookup6.c main.c

clean:
	rm -f shm_server shm_client