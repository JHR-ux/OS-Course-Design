CC=gcc
CFLAGS=-Wall -O2
TARGET=os_project
SRC=src/main.c src/scheduler.c src/memory.c src/sync.c src/filesystem.c

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) -lpthread

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)
