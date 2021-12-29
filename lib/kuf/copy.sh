if [ $# -lt 1 ]
then
    echo "Usage: $0 KUF_PATH"
    exit
fi

KUF_PATH=$1
CURDIR=$(pwd)
cd $KUF_PATH
make kuf.c
rsync -rvt kuf.c kuf.h $CURDIR
cd $CURDIR
