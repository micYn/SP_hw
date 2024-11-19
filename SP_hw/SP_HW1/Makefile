CC = gcc                  
CFLAGS = -Wall
EX1_TARGET = tee           
EX1_SRC = tee.c            
EX2_TARGET = cp
EX2_SRC = cp.c

all: $(EX1_TARGET) $(EX2_TARGET)

$(EX1_TARGET): $(EX1_SRC)
	$(CC) $(CFLAGS) -o $(EX1_TARGET) $(EX1_SRC)

$(EX2_TARGET): $(EX2_SRC)
	$(CC) $(CFLAGS) -o $(EX2_TARGET) $(EX2_SRC)

clean:
	rm -f $(EX1_TARGET) $(EX2_TARGET)

.PHONY: all clean
