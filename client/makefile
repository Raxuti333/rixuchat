CC=gcc
CSRC=src/*
CFLAGS=-O2 -march=native -I.
EXE=rhat

build:
	${CC} ${CFLAGS} -o ${EXE} ${CSRC}

install:
	cp ${EXE} /usr/bin/