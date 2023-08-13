if [ $# -lt 1 ]
then
    echo "Usage: $0 XMT_PATH"
    exit
fi

XMT_PATH=$1
CURDIR=$(pwd)
cd $XMT_PATH
rsync -rvt parse.c obj.c obj.h moncmp.c moncmp.h $CURDIR
cd $CURDIR
