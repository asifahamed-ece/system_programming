CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
TARGET = msgQ_sender msgQ_receiver

all: $(TARGET)

msgQ_sender: msgQ_sender.c
	$(CC) $(CFLAGS) -o $@ $^

msgQ_receiver: msgQ_receiver.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET) 
	ipcrm -Q 0x1234 2>/dev/null || true

# Run both Programs in separate terminals
run: all
	@echo "Compiled successfully. Launching terminals..."
	@konsole --noclose -e ./msgQ_sender & sleep 0.5 && konsole --new-tab --noclose -e ./msgQ_receiver &
	@echo "Terminals opened. Check the tabs!"

# Clean and run (Fresh start)
fresh: clean all run