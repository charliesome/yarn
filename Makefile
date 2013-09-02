DEBUG ?= -g

CFLAGS = -Wall -Wextra -pedantic -Werror -iquote inc -std=c99 $(DEBUG)

RAGEL ?= ragel

OBJS = src/connection.o src/irc_parser.o src/main.o src/string.o src/vector.o

yarn: $(OBJS) Makefile
	$(CC) -o $@ $(OBJS) $(CFLAGS) $(LDFLAGS)

%.c: %.rl Makefile
	$(RAGEL) -o $@ -C -G2 $<

%.o: %.c Makefile inc/*.h
	$(CC) -o $@ -c $< $(CFLAGS)
