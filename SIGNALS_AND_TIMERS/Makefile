CC = gcc
CFLAGS = -Wall -Wextra -O2 -g
TARGET = signal_timer

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	rm -f $(TARGET)