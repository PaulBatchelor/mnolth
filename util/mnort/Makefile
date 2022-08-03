CFLAGS = -I/usr/local/include/mnolth
CFLAGS += -O3
CFLAGS += -Wall -pedantic -std=c89
LIBS += -lmnolth -ljack -lx264 -lm -lpthread
CFLAGS+=-DLUA_USE_C89 -DLUA_COMPAT_5_2

OBJ = rt.o server.o client.o

default: mnort libmnort.a

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

mnort: mnort.o $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

libmnort.a: $(OBJ)
	@echo "Building libmnort"
	@$(AR) rcs $@ $^

clean:
	$(RM) mnort libmnort.a
	$(RM) $(OBJ) mnort.o

install: mnort libmnort.a
	cp mnort /usr/local/bin
	cp libmnort.a /usr/local/lib
