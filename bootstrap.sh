MNOLTH_TOP=$(pwd)

mkdir -p tools
echo "building lua"
cd lib/lua && ./build.sh $MNOLTH_TOP/tools/lua && cd $MNOLTH_TOP
echo "building samurai"
cd util/samurai && ./build.sh $MNOLTH_TOP/tools/samu
