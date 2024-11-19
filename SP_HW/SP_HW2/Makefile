CC = gcc                  
CFLAGS = -Wall
EX1_TARGET = tail           
EX1_SRC = tail.c

all: $(EX1_TARGET)

$(EX1_TARGET): $(EX1_SRC)
	$(CC) $(CFLAGS) -o $(EX1_TARGET) $(EX1_SRC)

clean:
	rm -f $(EX1_TARGET)

.PHONY: all clean
