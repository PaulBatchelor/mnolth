OBJ +=  lib/gestvm/gestvm.o \
	lib/gestvm/l_gestvm.o \
	lib/gestvm/uxn/uxn.o \
	lib/gestvm/uxn/uxnasm.o

# Extra objects (not part of standard gestvm)

OBJ += lib/gestvm/memops.o
OBJ += lib/gestvm/gestlive.o

CFLAGS += -DNO_UXNASM_MAIN
