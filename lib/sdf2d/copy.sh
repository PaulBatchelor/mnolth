if [ $# -lt 1 ]
then
    echo "Usage: $0 SDF2D_PATH"
    exit
fi

SDF2D_PATH=$1
CURDIR=$(pwd)
cd $SDF2D_PATH
rsync -rvt sdf.c sdf.h $CURDIR
cd $CURDIR
