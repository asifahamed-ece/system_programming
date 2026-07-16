CC = gcc
CFLAGS = -Wall -g -pthread
TARGET = pthread_mutex

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) $(TARGET).c -o $(TARGET)
clean:
	rm -f $(TARGET)