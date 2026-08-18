# compiler used
CC = gcc

# compiler warning flags
CFLAGS = -Wall -Wextra

#target to be executed
TARGET = stack_program.exe

# sources/dependancies
SOURCES = main.c stack.c

#build rule
$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

#clean rule
#clean:
#rm -f #$(TARGET)   

# or use the command -> 
clean:
	del /Q $(TARGET)	
