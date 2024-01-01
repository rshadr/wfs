 CC     = gcc -Wall -pedantic -std=c2x
# (broken)
# CC     = g++ -Wall -pedantic
CFLAGS = -O2 -march=native -ftree-vectorize -ggdb3

AR     = ar
RANLIB = ranlib

LIBS   = -lgrapheme
