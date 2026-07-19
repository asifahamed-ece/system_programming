CC = gcc
CFLAGS = -Wall -g

all: fifo_writer fifo_reader

fifo_writer: fifo_writer.c
	$(CC) $(CFLAGS) fifo_writer.c -o fifo_writer

fifo_reader: fifo_reader.c
	$(CC) $(CFLAGS) fifo_reader.c -o fifo_reader

clean:
	rm -f fifo_writer fifo_reader /tmp/sensor_fifo
