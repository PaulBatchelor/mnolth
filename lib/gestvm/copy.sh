if [ $# -lt 1 ]
then
    echo "Usage: $0 GESTVM_PATH"
    exit
fi

GESTVM_PATH=$1
CURDIR=$(pwd)
cd $GESTVM_PATH
make gestvm.c
rsync -rvt gestvm.c gestvm.h l_gestvm.c $CURDIR
cd $CURDIR
