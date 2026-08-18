#compiler
CC = gcc

#compiler warning flags
CFLAGS = -Wall -Wextra

#target
TARGET = list_program.exe

#dependancies
SOURCES = main.c linked_list.c

#build rule
$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

#clean rule
clean:
	del	/Q $(TARGET)