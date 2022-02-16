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

mkdir -p $INSTALL_PATH/include/mnolth/sndkit
mkdir -p $INSTALL_PATH/include/mnolth/sndkit/nodes
mkdir -p $INSTALL_PATH/include/mnolth/sndkit/lil
mkdir -p $INSTALL_PATH/include/mnolth/sndkit/dsp
mkdir -p $INSTALL_PATH/include/mnolth/sndkit/graforge
cp $LIBPATH/sndkit/core.h $INSTALL_PATH/include/mnolth/sndkit
cp $LIBPATH/sndkit/nodes/sknodes.h $INSTALL_PATH/include/mnolth/sndkit/nodes
cp $LIBPATH/sndkit/nodes/sklil.h $INSTALL_PATH/include/mnolth/sndkit/nodes
cp $LIBPATH/sndkit/dsp/*.h $INSTALL_PATH/include/mnolth/sndkit/dsp
cp $LIBPATH/sndkit/lil/lil.h $INSTALL_PATH/include/mnolth/sndkit/lil
cp $LIBPATH/sndkit/graforge/graforge.h $INSTALL_PATH/include/mnolth/sndkit/graforge

mkdir -p $INSTALL_PATH/share/mnolth
cp $MNOLTH_PATH/scm/*.scm $INSTALL_PATH/share/mnolth
