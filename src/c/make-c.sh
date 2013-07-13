FLAGS="-g -std=c99 -Wall"
HEADERS="-I headers/ -I public-headers/"
LIBS="-lportaudio -lasound -ljack -lm"

gcc $FLAGS $HEADERS *.c $LIBS -o Whisper

