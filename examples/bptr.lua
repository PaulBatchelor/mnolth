lil([[

gfxnew gfx 256 256
bpnew bp 256 256

bpset [grab bp] 0 0 0 256 256
bpcirc [bpget [grab bp] 0] 128 128 64 1

bppbm [grab bp] test.pbm

grab gfx
gfxclrset 0 1.0 1.0 1.0
gfxclrset 1 0.0 0.0 0.0
gfxfill 0
drop

grab gfx
bptr [grab bp] 0 0 256 256 0 0 1

grab gfx
gfxppm test.ppm
drop

]])
