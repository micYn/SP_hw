CC = gcc
CFLAGS = -Wall
TARGET = suspend waitinfo
SRC = sig_speed_sigsuspend.c sig_speed_sigwaitinfo.c error_functions.c get_num.c

all: $(TARGET)

suspend: sig_speed_sigsuspend.c error_functions.c get_num.c
	$(CC) $(CFLAGS) -o suspend sig_speed_sigsuspend.c error_functions.c get_num.c

waitinfo: sig_speed_sigwaitinfo.c error_functions.c get_num.c
	$(CC) $(CFLAGS) -o waitinfo sig_speed_sigwaitinfo.c error_functions.c get_num.c

clean:
	rm -f $(TARGET)
