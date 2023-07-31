#!/bin/bash
if [ ! -d "./bin" ]; then
    mkdir ./bin
fi

gcc -c ./src/dlq.c -o ./bin/dlq.o
gcc -c ./src/servidor.c -o ./bin/servidor.o
gcc -c ./src/requisicao.c -o ./bin/requisicao.o

gcc ./bin/servidor.o ./bin/dlq.o ./bin/requisicao.o -o ./servidor -l mpfr
