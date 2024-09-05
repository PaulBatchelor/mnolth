OBJ=\
base.o \
ftbl.o \
paulstretch.o \
stretcher.o \
kiss_fft.o \
kiss_fftr.o

default: stretcher

LIBS+=-lsndfile
CFLAGS+=-Wall -O3 #-DNO_LIBSNDFILE

LIBS+=-L../../ -lmnolth

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

stretcher: $(OBJ)
	$(CC) $(OBJ) -o $@ $(LIBS)

clean:
	$(RM) stretcher
	$(RM) $(OBJ)
