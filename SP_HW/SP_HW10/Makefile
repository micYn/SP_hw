CC = gcc
TARGET = producer consumer

all: $(TARGET)

producer: producer.c
	$(CC) -o producer producer.c
consumer: consumer.c
	$(CC) -o consumer consumer.c

clean:
	rm -f $(TARGET)