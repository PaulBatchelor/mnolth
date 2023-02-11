if [ $# -lt 1 ]
then
    echo "Usage: $0 BITLANG_PATH"
    exit
fi

BITLANG_PATH=$1
CURDIR=$(pwd)
WORGLE=$CURDIR/../../util/worgle/worglite
cd $BITLANG_PATH
$WORGLE bitlang.org
rsync -rvt bitlang.c bitlang.h $CURDIR
cd $CURDIR
