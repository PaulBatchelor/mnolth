OBJ +=  lib/gestvm/gestvm.o \
	lib/gestvm/l_gestvm.o \
	lib/gestvm/uxn/uxn.o \
	lib/gestvm/uxn/uxnasm.o

CFLAGS += -DNO_UXNASM_MAIN
