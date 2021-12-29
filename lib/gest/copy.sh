if [ $# -lt 1 ]
then
    echo "Usage: $0 GEST_PATH"
    exit
fi

GEST_PATH=$1
CURDIR=$(pwd)
cd $GEST_PATH
make gest.c
rsync -rvt \
      gest.c \
      gest.h \
      l_gest.c \
      skgest.c \
      skgest.c \
      gestlang.c \
      gestlang.h \
      l_gestlang.c \
      $CURDIR
cd $CURDIR
