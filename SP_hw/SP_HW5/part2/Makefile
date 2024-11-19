CC = gcc
SRC = builtin.c is_background.c parse.c pipe_command.c pipe_present.c redirect_in.c redirect_out.c run_command.c shell.c
OBJ = builtin.o is_background.o parse.o pipe_command.o pipe_present.o redirect_in.o redirect_out.o run_command.o shell.o
TARGET = shell

all: $(TARGET)

$(TARGET): builtin.o is_background.o parse.o pipe_command.o pipe_present.o redirect_in.o redirect_out.o run_command.o shell.o
	$(CC) -o $(TARGET) builtin.o is_background.o parse.o pipe_command.o pipe_present.o redirect_in.o redirect_out.o run_command.o shell.o

builtin.o: builtin.c
	$(CC) -c builtin.c
	
is_background.o: is_background.c
	$(CC) -c is_background.c
	
parse.o: parse.c
	$(CC) -c parse.c
	
pipe_command.o: pipe_command.c
	$(CC) -c pipe_command.c
	
pipe_present.o: pipe_present.c
	$(CC) -c pipe_present.c
	
redirect_in.o: redirect_in.c
	$(CC) -c redirect_in.c
	
redirect_out.o: redirect_out.c
	$(CC) -c redirect_out.c
run_command.o: run_command.c
	$(CC) -c run_command.c
shell.o: shell.c
	$(CC) -c shell.c
	
clean: 
	rm -f $(TARGET) $(OBJ)
	
.PHONY: all clean
