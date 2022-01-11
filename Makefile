CFLAGS += -DSTANDALONE=0
CFLAGS += -O3 -Wall -fPIC

CFLAGS+= -Ilib/sndkit/graforge
CFLAGS+= -Ilib
CFLAGS+= -Icore
CFLAGS+= -g

OBJ += core/loader.o
OBJ += core/gfxbuf.o core/l_gfxbuf.o
OBJ += core/var.o core/l_var.o
OBJ += core/mains.o
OBJ += core/scheme.o
OBJ += core/l_sqlar.o
OBJ += core/l_tract.o
OBJ += core/draw.o core/l_draw.o
OBJ += core/hotswap.o core/l_hotswap.o
OBJ += util/txt2cdb.o
OBJ += core/misc.o
OBJ += util/sqlar/sqlar.o

TANGLED += lib/btprnt/regpool.c
TANGLED += core/hotswap.c
TANGLED += core/hotswap.h
TANGLED += core/gfxbuf.c
TANGLED += core/gfxbuf.h
TANGLED += core/draw.c
TANGLED += core/draw.h
TANGLED += core/var.c
TANGLED += core/var.h
TANGLED += scm/ugens.scm
TANGLED += core/misc.c

include lib/sndkit/config.mk
include lib/mathc/config.mk
include lib/minimp4/config.mk
include lib/gest/config.mk
include lib/btprnt/config.mk
include lib/lodepng/config.mk
include lib/kuf/config.mk
include lib/cdb/config.mk
include lib/sqlite3/config.mk
include lib/miniz/config.mk

WORGLE=$(abspath util/worgle/worglite)
WORGLE_FLAGS=-Werror -g

LIBS= -lm -lx264 -lpthread

ALL = mnoscm mnolth libmnolth.a mnotil

default: $(TANGLED) $(ALL)

tools: util/worgle/worglite

C89?=$(CC) -std=c89
C99?=$(CC) -std=c99

.SUFFIXES: .c .o
.c.o:
	@echo "$(C89) $<"
	@$(C89) -pedantic -c $(CFLAGS) $< -o $@

.SUFFIXES: .c .c99
.c.c99:
	@echo "$(C99) $<"
	$(C99) -c $(CFLAGS) $< -o $@

.SUFFIXES: .org .c
.org.c:
	@echo "WORGLE $<"
	@cd $(dir $<); $(WORGLE) $(WORGLE_FLAGS) $(notdir $<)

.SUFFIXES: .org .scm
.org.scm:
	@echo "WORGLE $<"
	@cd $(dir $<); $(WORGLE) -Werror $(notdir $<)

util/worgle/worglite: util/worgle/worgle.c util/worgle/parg.c
	$(CC) -std=c89 -Wall -pedantic -O3 -DWORGLITE $^ -o $@

mnoscm: $(OBJ) core/scm_main.o
	@echo "building mnoscm"
	@$(CC) $^ -o $@ $(LIBS)

mnolth: $(OBJ) core/lil_main.o
	@echo "building mnolth"
	@$(CC) $^ -o $@ $(LIBS)

mnotil: $(OBJ) util/mnotil.o
	@echo "building mnotil"
	@$(CC) $^ -o $@ $(LIBS)

libmnolth.a: $(OBJ)
	@echo "Building libmnolth"
	@$(AR) rcs $@ $^

install: $(ALL)
	sh install.sh

tangle: $(TANGLED)

clean:
	@echo "clean"
	@$(RM) $(OBJ)
	@$(RM) mnoscm
	@$(RM) mnolth
	@$(RM) libmnolth.a
	@$(RM) core/scm_main.o
	@$(RM) core/lil_main.o
	@$(RM) $(TANGLED)
