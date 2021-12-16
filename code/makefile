TARGET = project
INCLUDE = -I. -Irendering -Itools -Imain -Iresources -Isupport -Iinclude -Iinclude/freetype
CC = gcc
CFLAGS = -std=c99 -Wall -lGL -lGLU -lGLEW -lglut -lm -g -lfreetype -lSOIL
dirs = tools/* main/* rendering/* support/* resources/* *

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(foreach dir, $(dirs),$(patsubst $(dir)%.c,$(dir)%.o, $(wildcard $(dir)*.c)))
HEADERS = $(foreach dir, $(dirs),$(wildcard $(dir)*.h))

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(INCLUDE) $(OBJECTS) $(CFLAGS) $(LIBS) -o $@

clean:
	-rm -f -R *.o
	-rm -f -R *.h.gch
	-rm -f $(TARGET)
