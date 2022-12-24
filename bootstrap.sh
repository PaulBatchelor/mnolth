MNOLTH_TOP=$(pwd)

echo "building lua"
cd lib/lua && ./build.sh $MNOLTH_TOP/lua && cd $MNOLTH_TOP
echo "building samurai"
cd util/samurai && ./build.sh $MNOLTH_TOP/samu
