#compiler
CC = gcc

#compiler flags
CFLAGS = -Wall -Wextra

#target
TARGET = vector_pgm.exe

#sourecs
SOURCES = main.c vector.c

#build rule
$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

#clean rule
clean:
	del /Q $(TARGET)	