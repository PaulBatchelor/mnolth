CFLAGS += -g -I. -D_FILE_OFFSET_BITS=64 -Wall -Werror
CFLAGS += -DSQLAR_ZLIB
ZLIB = -lz
SQLITELIB= -lsqlite3
FUSELIB = -lfuse -lpthread -ldl

all: sqlar sqlarfs

sqlar:	sqlar.c main.c
	$(CC) $(CFLAGS) -o sqlar $(OPT) sqlar.c main.c $(ZLIB) $(SQLITELIB)

sqlarfs:	sqlarfs.c
	$(CC) -D_FILE_OFFSET_BITS=64 -o sqlarfs $(OPT) sqlarfs.c $(ZLIB) $(FUSELIB) $(SQLITELIB)

clean:
	$(RM) -f sqlar sqlarfs sqlite3.o

install: sqlar sqlarfs
	cp sqlar sqlarfs /usr/local/bin
