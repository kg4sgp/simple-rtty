simple-rtty
===========

A simple RTTY modulator using port audio. This project consists of two programs: one that creates a RTTY bit stream (rtty) and one that uses port audio to send Frequency Shift Keyed tones to the speakers (mod-rtty). I wanted to create a simple implementation of RTTY that could actually be somewhat useful.

To compile these files:

gcc mod-rtty.c -lm -lportaudio -o mod-rtty

gcc rtty.c -o rtty

To use these files:

./rtty text-input-file binary-output-file

./mod-rtty binary-output-file audio-output-file
