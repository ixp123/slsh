CC = cc

CFLAGS = -g

RM = rm -f

VERSION = 0.1b

TARGET = slsh

TARGET_FILE=src/main.c

BIN_DIR=/usr/bin/slsh

MV_TO_CMD = cp slsh $(BIN_DIR)

EXEC_NAME = slsh

all: main

main: src/main.c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET_FILE)

install:
	$(MV_TO_CMD)

clean veryclean:
	$(RM) $(EXEC_NAME)
