weight_tal = [[
%NUM { #24 DEO }
%DEN { #25 DEO }
%NEXT { #26 DEO }
%NOTE { #33 ADD NEXT }
%BHVR { #27 DEO }
%SKDR { #23 DEO }
%SKTY { #22 DEO }
%MASS { #20 DEO }
%INRT { #21 DEO }
%MASS-POS { #7f ADD #20 DEO }
%MASS-NEG { #7f SWP SUB #20 DEO }

|0100

@init
#0b NOTE
#02 NUM #01 DEN
#02 BHVR
#08 INRT
BRK

@mel
#0c NOTE #20 MASS-POS #02 BHVR BRK
#08 NOTE #10 MASS-POS BRK
#0b NOTE #00 MASS-POS BRK
#09 NOTE #40 MASS-NEG BRK
#08 NOTE #50 MASS-NEG BRK

#05 NOTE #40 MASS-NEG BRK
#04 NOTE #20 MASS-POS BRK
#02 NOTE #10 MASS-POS BRK
#04 NOTE #30 MASS-NEG BRK
#08 NOTE #40 MASS-NEG BRK
#04 NOTE #50 MASS-NEG BRK
#0b NOTE #60 MASS-NEG #03 BHVR BRK
;mel JMP2
]]

lil([[
gmemnew mem
gestvmnew gvm
]])
gestvm_compile("mem", weight_tal)
lil("print [gmemsym [grab mem] mel]")

lil("gmemcpy [grab mem] [grab gvm]")
lil([[
hold [prev]
regset zz 1

regget 1
add zz 2
phasor zz 0

hold zz
regset zz 0

gestvmnode [grab gvm] [gmemsym [grab mem] mel] [regget 0]
gestvmweight [grab gvm] 1.2 2
cabclr [regget 1]
mix zz [regget 1] 1.0
gestvmeval [grab gvm] [gmemsym [grab mem] init]

sine 6.5 0.4
gestvmweight [grab gvm] 1 1
smoother zz 0.01
biscale zz 1 0.1
mul zz zz
add zz zz

mtof zz
blsaw zz
mul zz 0.5

gestvmweight [grab gvm] 1 1
smoother zz 0.1
biscale zz 2000 500
butlp zz zz
butlp zz 4000

dup
dup
verbity zz zz 0.1 0.1 0.1
drop
mul zz [dblin -15]
dcblocker zz
add zz zz

unhold [regget 0]
unhold [regget 1]

wavout zz memops_test.wav

computes 10
]])
