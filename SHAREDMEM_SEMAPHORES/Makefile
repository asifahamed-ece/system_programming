CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
TARGET = shm_reader shm_writer

all: $(TARGET)

shm_reader: shm_reader.c
	$(CC) $(CFLAGS) shm_reader.c -o shm_reader
shm_writer: shm_writer.c
	$(CC) $(CFLAGS) shm_writer.c -o shm_writer

clean:
	rm -f $(TARGET)

# Run both Programs in separate terminals
run: all
	@echo "Compiled successfully. Launching terminals..."
	@konsole --noclose -e ./shm_writer & sleep 0.5 && konsole --new-tab --noclose -e ./shm_reader &
	@echo "Terminals opened. Check the tabs!"

# Clean and run (Fresh start)
fresh: clean all run