CC = gcc
CFLAGS = -pthread -lrt
TARGETS = producer consumer

all: $(TARGETS)

producer: producer.c
	$(CC) producer.c -o producer $(CFLAGS)

consumer: consumer.c
	$(CC) consumer.c -o consumer $(CFLAGS)

clean:
	rm -f $(TARGETS)
	rm -f /dev/shm/empty /dev/shm/full /dev/shm/table_shm

