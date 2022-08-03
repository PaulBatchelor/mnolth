-- Hi! This is a test patch.
--
-- Before starting, be sure to load the mnort.vim script.
-- Also make sure that both the JACK server has started,
-- as well as the mnort server (via `mnort server`).


-- Below is a codeblock.


-- Codeblocks are chunks of scheme code that get sent to mnort to
-- be evaluated. The convention here is to use <@></@> to designate
-- a codeblock, but any valid tag should work.


-- To evaluate this code block, move the cursor so that it is
-- inside the block, then run <leader>+s.

-- <leader> + p will toggle play/pause.

-- <@>
lil([[
hsnew hs
rtnew [grab hs] rt

func out {} {
    hsout [grab hs]
    hsswp [grab hs]
}

func playtog {} {
    hstog [grab hs]
}
]])

lil([[
metro 9
trand zz 200 800
smoother zz 0.001
sine zz 0.3
out
]])
-- </@>

-- The "lil" command evaluates LIL code, a small scripting
-- language used to build up patches in sndkit, the underlying
-- synthesis system used in mnolth. The "out" command
-- will dynamically switch between patches.

-- Here's another patch to try:

-- <@>
lil([[
metro 1
tdiv zz 3 0
env zz 0.001 0.001 0.001
blsaw 800
butlp zz 1000
mul zz 0.9
mul zz zz
dup
vardelay zz 0 0.2 0.2
dup
bigverb zz zz 0.99 10000
drop
dcblocker zz
mul zz [dblin -20]
add zz zz
out
]])
-- </@>


-- To gracefully stop the server.

-- <@>
lil("stop")
-- </@>
