INSTALL_PATH=/usr/local
MNOLTH_PATH=.
LIBPATH=lib

cp $MNOLTH_PATH/libmnolth.a $INSTALL_PATH/lib
cp $MNOLTH_PATH/mnoscm $INSTALL_PATH/bin
cp $MNOLTH_PATH/mnolth $INSTALL_PATH/bin
cp $MNOLTH_PATH/mnotil $INSTALL_PATH/bin

mkdir -p $INSTALL_PATH/include/mnolth/mathc
cp $LIBPATH/mathc/mathc.h $INSTALL_PATH/include/mnolth/mathc

mkdir -p $INSTALL_PATH/include/mnolth/btprnt
cp $LIBPATH/btprnt/btprnt.h $INSTALL_PATH/include/mnolth/btprnt

mkdir -p $INSTALL_PATH/include/mnolth/gfxbuf
cp $MNOLTH_PATH/core/gfxbuf.h $INSTALL_PATH/include/mnolth/gfxbuf
cp $MNOLTH_PATH/core/draw.h $INSTALL_PATH/include/mnolth/gfxbuf

mkdir -p $INSTALL_PATH/include/mnolth/scheme
cp $MNOLTH_PATH/core/scheme.h $INSTALL_PATH/include/mnolth/scheme

mkdir -p $INSTALL_PATH/include/mnolth/scheme
cp $MNOLTH_PATH/core/hotswap.h $INSTALL_PATH/include/mnolth/

mkdir -p $INSTALL_PATH/share/mnolth
cp $MNOLTH_PATH/scm/*.scm $INSTALL_PATH/share/mnolth
