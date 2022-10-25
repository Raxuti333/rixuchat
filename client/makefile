VERSION=1.0
EXE=rhat
CC=gcc
CSRC=src/*
CFLAGS=-O2 -march=native -I. -DTARGET=\"${EXE}\" -DVERSION=\"${VERSION}\"

build:
	${CC} ${CFLAGS} -o ${EXE} ${CSRC}

install:
	cp ${EXE} /usr/bin/