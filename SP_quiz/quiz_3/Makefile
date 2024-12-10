CC = gcc
SRC = router.c node.c
TARGET = router node

all: $(TARGET)

myrpc: router.c
	$(CC) -o router router.c

detector: node.c
	$(CC) -o node node.c

clean: $(TARGET)
	rm -f $(TARGET)