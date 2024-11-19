CC = gcc
SRC = 12-1.c 12-2.c 12-3.c
TARGET = 12-1 12-2 12-3

all: $(TARGET)

12-1: 12-1.c
	$(CC) 12-1.c -o 12-1

12-2: 12-2.c
	$(CC) 12-2.c -o 12-2

12-3: 12-3.c
	$(CC) 12-3.c -o 12-3
	
clean: 
	rm -f $(TARGET)
	
.PHONY: all clean

