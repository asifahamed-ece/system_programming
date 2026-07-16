CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = file_copier

all: $(TARGET)

$(TARGET): $(TARGET).c 
	$(CC) $(CFLAGS) $(TARGET).c -o $(TARGET)

clean:
	rm -f $(TARGET)