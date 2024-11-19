CC = gcc
SRC = hostinfo.c mycat.c mydate.c pipe_ls.c printdir.c
TARGET = hostinfo mycat mydate pipe_ls printdir

all: $(TARGET)

hostinfo: hostinfo.c
	$(CC) hostinfo.c -o hostinfo

mycat: mycat.c
	$(CC) mycat.c -o mycat

mydate: mydate.c
	$(CC) mydate.c -o mydate

pipe_ls: pipe_ls.c
	$(CC) pipe_ls.c -o pipe_ls

printdir: printdir.c
	$(CC) printdir.c -o printdir
	
clean: 
	rm -f $(TARGET)
	
.PHONY: all clean
