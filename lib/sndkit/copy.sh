if [ "$#" -lt 1 ]
then
printf "usage: $0 SNDKIT_PATH\n"
exit
fi

CURDIR=$(pwd)
SNDKIT_PATH=$1

cd $SNDKIT_PATH
make copy
cd out
rsync -Rrvt * $CURDIR
cd $CURDIR
