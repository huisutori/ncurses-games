CC = gcc
CFLAGS = -Wall -g
TARGET = cgames

SUBDIRS = common \
		  model \
		  model/mine-sweeper \
		  port/curses \
		  port/pthread \
		  utils

LIBS = -lncurses \
	   -lpthread

INCS = -I./.

SRCS = $(wildcard *.c) \
       $(foreach dir, $(SUBDIRS), $(wildcard $(dir)/*.c))

all:
	$(CC) $(SRCS) $(INCS) $(LIBS) -o $(TARGET)

clean:
	rm $(TARGET)