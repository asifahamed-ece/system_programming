CC = gcc
CFLAGS = -Wall -g
TARGET = zombie_orphan

all: $(TARGET)

$(TARGET): zombie_orphan.c
	$(CC) $(CFLAGS) zombie_orphan.c -o $(TARGET)

clean:
	rm -f $(TARGET)
	