CC = cc
CC_FLAGS = -W -Wall -Wextra -O2 
SRC = forth.c
TARGET = bin/forth
TFORTH = prg.tf
all: 
	mkdir -p bin
	$(CC) $(CC_FLAGS) $(SRC) -o $(TARGET)

run: 
	./$(TARGET) $(TFORTH)
clean:
	rm -rf bin/*

del: 
	rm -rf bin
