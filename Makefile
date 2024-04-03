CC = gcc
CFLAGS = -Wall -g
TARGET = cgames

SUBDIRS = src \
		  src/app \
		  src/app/controllers \
		  src/common \
		  src/model \
		  src/model/menu \
		  src/model/menu/views \
		  src/model/mine-sweeper \
		  src/model/mine-sweeper/views \
		  src/port/curses \
		  src/port/curses/views \
		  src/port/pthread \
		  src/utils

LIBS = -lncurses \
	   -lpthread

DEFS = -DNCURSES_OPAQUE=0

INCS = -Isrc/.

SRCS = $(wildcard *.c) \
       $(foreach dir, $(SUBDIRS), $(wildcard $(dir)/*.c))

all:
	$(CC) $(SRCS) $(INCS) $(DEFS) $(LIBS) -o $(TARGET)

clean:
	rm $(TARGET)