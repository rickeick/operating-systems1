#ifndef _REQUISICAO_H_
#define _REQUISICAO_H_

#include <stdio.h>

typedef struct _requisicao_ Requisicao;

Requisicao *proximaRequisicao(FILE *arquivo);

void processarRequisicao(Requisicao *requisicao);

int getI(Requisicao *requisicao);

int getId(Requisicao *requisicao);

char *getPi(Requisicao *requisicao);

int getTempoEspera(Requisicao *requisicao);

#endif
