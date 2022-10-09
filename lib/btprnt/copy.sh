if [ $# -lt 1 ]
then
    echo "Usage: $0 BTPRNT_PATH"
    exit
fi

BTPRNT_PATH=$1
CURDIR=$(pwd)
cd $BTPRNT_PATH
make btprnt.h
rsync -rvt btprnt.h $CURDIR
cd $CURDIR
