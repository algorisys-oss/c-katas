CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -g

all: http_parser http_server

http_parser: http_parser.c
	$(CC) $(CFLAGS) -o http_parser http_parser.c

http_server: http_server.c
	$(CC) $(CFLAGS) -o http_server http_server.c

test_parser: http_parser
	./http_parser

test_server: http_server
	./http_server --test

test: test_parser test_server

clean:
	rm -f http_parser http_server

.PHONY: all clean test test_parser test_server
