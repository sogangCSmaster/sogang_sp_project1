CC = gcc
CFLAGS = -Wall	-g
OBJECTS = 20131575.o
TARGET = 20131575.out

all : $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CC) -o $@ ${CFLAGS} ${OBJECTS}

20131575.o : 20131575.c

clean: 
	rm -f $(OBJECTS) $(TARGET)
