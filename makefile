CC = gcc
CFLAGS = -Wall -g
TARGET = user_manager
SRC = usermanager.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET) *.o

zip: clean
	zip -r user_manager.zip $(SRC) account.txt Makefile
