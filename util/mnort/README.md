# mnort

mnort is a realtime live-coding server and client for mnolth.

mnort spins up an instance of mnolth inside of a JACK audio
render thread, and creates a UDP listener that allows it
to be remotely controlled.

The scripting languages used are a combination of scheme
(tinyscheme) and LIL (a TCL-like). LIL is evaluated as inline
code via the `lil` command in scheme.

## Install

Compile and install mnolth: [sourcehut](https://git.sr.ht/~pbatch/mnolth),
[github](https://github.com/paulbatchelor/mnolth).

mnort can compiled using:

    $ make

Followed by:

    $ sudo make install

## Usage

At the moment, live coding with mnort has only been
designed to work using Vim.

It is useful to to install and run the mnort.vim script,
either at runtime or manually using `:so mnort.vim`. This
provides some handy mappings.

Start up the JACK server.

Start the mnort server by running:

    $ mnort server

Open up `test.scm` inside of Vim, and follow the
instructions. This provides some boilerplate scheme
code to get live coding working.

The vim keybindings provide use the leader key.

leader + s: evaluates a code block. Anything that's inside
of a tag like `<aaa></aaa>` will get evaluated.

leader + p: toggles play/pause. It's nice to shut things up.
