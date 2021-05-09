TARGET = PokeCenter
LIBS = -lm
CC = gcc
CFLAGS = -g -Wall

.PHONY: default all run clean

default: $(TARGET)
all: default

OBJECTS = main.o
HEADERS = 

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)