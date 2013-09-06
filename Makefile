DEBUG ?= -g

CFLAGS = -Wall -Wextra -pedantic -Werror -iquote inc -iquote vendor/klib -std=c99 $(DEBUG)

LDFLAGS = -lncurses

RAGEL ?= ragel

OBJS = src/connection.o src/irc_parser.o src/main.o src/string.o src/ui.o

LIBS = vendor/vitapi/src/libvitapi.a

yarn: $(OBJS) $(LIBS) Makefile
	$(CC) -o $@ $(OBJS) $(LIBS) $(CFLAGS) $(LDFLAGS)

%.c: %.rl Makefile
	$(RAGEL) -o $@ -C -G2 $<

%.o: %.c Makefile inc/*.h
	$(CC) -o $@ -c $< $(CFLAGS)

vendor/vitapi/src/libvitapi.a: vendor/vitapi/Makefile
	cd vendor/vitapi; make

vendor/vitapi/Makefile: vendor/vitapi/CMakeLists.txt
	cd vendor/vitapi; cmake .

.PHONY: clean

clean:
	rm -f $(OBJS) yarn
	git submodule foreach git clean -xdf
