CC =	gcc
CFLAGS =	-Wall -g
TARGET =	exec_wait

all: $(TARGET)

$(TARGET): exec_wait.c
	$(CC) $(CFLAGS) exec_wait.c -o $(TARGET)
clean:
	rm -f $(TARGET)