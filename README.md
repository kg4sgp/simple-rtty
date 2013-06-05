simple-rtty
===========

A simple RTTY modulator using port audio.

To compile these files:

gcc mod-rtty.c -lm -lportaudio -o mod-rtty

gcc rtty.c -o rtty

To use these files:

./rtty text-input-file binary-outpu-file

./mod-rtty binary-output-file audio-output-file
