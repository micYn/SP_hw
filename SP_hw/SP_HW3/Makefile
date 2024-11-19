CC = gcc
# CFLAGS = -Wall
TARGET = shell

all: $(TARGET)

$(TARGET): shell.o parse.o run_command.o builtin.o is_background.o
	$(CC) $(CFLAGS) -o $(TARGET) shell.o parse.o run_command.o builtin.o is_background.o

shell.o: shell.c
	$(CC) $(CFLAGS) -c shell.c

parse.o: parse.c
	$(CC) $(CFLAGS) -c parse.c

run_command.o: run_command.c
	$(CC) $(CFLAGS) -c run_command.c

builtin.o: builtin.c
	$(CC) $(CFLAGS) -c builtin.c

is_background.o: is_background.c
	$(CC) $(CFLAGS) -c is_background.c

clean:
	rm -f $(TARGET) *.o
