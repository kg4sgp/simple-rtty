simple-rtty
===========

A simple RTTY modulator using port audio. This project consists of two programs: one that creates a RTTY bit stream (rtty) and one that uses port audio to send Frequency Shift Keyed tones to the speakers (mod-rtty). I wanted to create a simple implementation of RTTY that could actually be somewhat useful.

The RTTY produced as-is is 45baud, 170Hz shift, 1 start bit, 2 stop bits, 5 character bits (baudot code). As such it will decode using FL-Digi on the RTTY-45 setting

To compile these files, just run `make` and hope for the best.

To use these files:

`./rtty text-input-file binary-output-file`

then

`./mod-rtty binary-output-file audio-output-file`
