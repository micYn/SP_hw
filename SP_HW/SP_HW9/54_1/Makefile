CC = gcc
TARGET = writer reader

all : $(TARGET)

writer : shm_xfr_writer.c
	$(CC) -o writer shm_xfr_writer.c
reader : shm_xfr_reader.c
	$(CC) -o reader shm_xfr_reader.c

clean :
	rm -f $(TARGET)