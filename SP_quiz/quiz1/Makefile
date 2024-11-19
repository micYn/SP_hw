CC = gcc
SRC = myrpc.c detector.c
TARGET = myrpc detector

all: $(TARGET)

myrpc: myrpc.c
	$(CC) -o myrpc myrpc.c

detector: detector.c
	$(CC) -o detector detector.c

clean: $(TARGET)
	rm -f $(TARGET)