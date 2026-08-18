
# Compiler to use
# CC is a vairable, $(CC) gives gcc
CC = gcc

#  Warning flags passed to the compiler
CFLAGS = -Wall -Wextra

# Name of the executable
TARGET = hash_program.exe

# dependancies
# All C source files required for the project
SOURCES = main.c hash_table.c

# Rule to build the executable
# If either of these files changes, rebuild hash_program
$(TARGET): $(SOURCES)
    # Compile the program
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)
# here, even if one file is changed, it'll compile all other files!	

# Delete generated files
# -f is for force remove
# a command that can be invoked
clean:
	rm -f $(TARGET) 