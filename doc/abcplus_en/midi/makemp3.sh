#!/bin/sh

# WARNING: for some reasons, some MP3 produced by this
# script are silent.

for MIDI in *mid; do
  timidity -Ow $MIDI
  FILE=$(basename $MIDI .mid)
  lame $FILE.wav $FILE.mp3
done

rm -f *wav
