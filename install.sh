INSTALL_PATH=/usr/local
MNOLTH_PATH=.
LIBPATH=lib

# typical binary/library install

cp $MNOLTH_PATH/libmnolth.a $INSTALL_PATH/lib

# use install instead of cp because to avoid crashes on OSX
install $MNOLTH_PATH/mnolth $INSTALL_PATH/bin

# mathc

mkdir -p $INSTALL_PATH/include/mnolth/mathc
cp $LIBPATH/mathc/mathc.h $INSTALL_PATH/include/mnolth/mathc

# btprnt

mkdir -p $INSTALL_PATH/include/mnolth/btprnt
cp $LIBPATH/btprnt/btprnt.h $INSTALL_PATH/include/mnolth/btprnt

# mnolth core headers

mkdir -p $INSTALL_PATH/include/mnolth/gfxbuf
cp $MNOLTH_PATH/core/gfxbuf.h $INSTALL_PATH/include/mnolth/gfxbuf
cp $MNOLTH_PATH/core/draw.h $INSTALL_PATH/include/mnolth/gfxbuf
cp $MNOLTH_PATH/core/hotswap.h $INSTALL_PATH/include/mnolth/
cp $MNOLTH_PATH/core/uf2.h $INSTALL_PATH/include/mnolth/

# tinyscheme

mkdir -p $INSTALL_PATH/include/mnolth/scheme
cp $MNOLTH_PATH/core/scheme.h $INSTALL_PATH/include/mnolth/scheme

# sndkit

mkdir -p $INSTALL_PATH/include/mnolth/sndkit
mkdir -p $INSTALL_PATH/include/mnolth/sndkit/nodes
mkdir -p $INSTALL_PATH/include/mnolth/sndkit/lil
mkdir -p $INSTALL_PATH/include/mnolth/sndkit/dsp
mkdir -p $INSTALL_PATH/include/mnolth/sndkit/graforge
mkdir -p $INSTALL_PATH/include/mnolth/sndkit/kissfft
cp $LIBPATH/sndkit/core.h $INSTALL_PATH/include/mnolth/sndkit
cp $LIBPATH/sndkit/nodes/sknodes.h $INSTALL_PATH/include/mnolth/sndkit/nodes
cp $LIBPATH/sndkit/nodes/sklil.h $INSTALL_PATH/include/mnolth/sndkit/nodes
cp $LIBPATH/sndkit/dsp/*.h $INSTALL_PATH/include/mnolth/sndkit/dsp
cp $LIBPATH/sndkit/lil/lil.h $INSTALL_PATH/include/mnolth/sndkit/lil
cp $LIBPATH/sndkit/graforge/graforge.h $INSTALL_PATH/include/mnolth/sndkit/graforge
cp $LIBPATH/sndkit/extra/kissfft/kiss_fft.h $INSTALL_PATH/include/mnolth/sndkit/kissfft
cp $LIBPATH/sndkit/extra/kissfft/kiss_fftr.h $INSTALL_PATH/include/mnolth/sndkit/kissfft

# lua
mkdir -p $INSTALL_PATH/include/mnolth/lua
cp $LIBPATH/lua/lua.h $INSTALL_PATH/include/mnolth/lua
cp $LIBPATH/lua/lualib.h $INSTALL_PATH/include/mnolth/lua
cp $LIBPATH/lua/lauxlib.h $INSTALL_PATH/include/mnolth/lua
cp $LIBPATH/lua/luaconf.h $INSTALL_PATH/include/mnolth/lua

# sdf2d

mkdir -p $INSTALL_PATH/include/mnolth/sdf2d
cp $LIBPATH/sdf2d/sdf.h $INSTALL_PATH/include/mnolth/sdf2d
cp $LIBPATH/sdf2d/sdfvm.h $INSTALL_PATH/include/mnolth/sdf2d

# copy SCM files

mkdir -p $INSTALL_PATH/share/mnolth
cp $MNOLTH_PATH/scm/*.scm $INSTALL_PATH/share/mnolth

# cmp

mkdir -p $INSTALL_PATH/include/mnolth/cmp
cp $LIBPATH/cmp/cmp.h $INSTALL_PATH/include/mnolth/cmp

# miniz

mkdir -p $INSTALL_PATH/include/mnolth/miniz
cp $LIBPATH/miniz/miniz.h $INSTALL_PATH/include/mnolth/miniz

# lodepng

mkdir -p $INSTALL_PATH/include/mnolth/lodepng
cp $LIBPATH/lodepng/lodepng.h $INSTALL_PATH/include/mnolth/lodepng
