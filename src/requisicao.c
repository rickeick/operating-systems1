#ifndef _REQUISICAO_C_
#define _REQUISICAO_C_

#include <mpfr.h>
#include <stdlib.h>
#include <string.h>
#include "requisicao.h"

typedef struct _requisicao_ {
    int i;
    int id;
    char *pi;
    int tempo_espera;
} Requisicao;

Requisicao *proximaRequisicao(FILE *arquivo) {
    char linha[10];
    int static id = 1;
    Requisicao *requisicao;
    fgets(linha, sizeof(linha), arquivo);
    requisicao = (Requisicao *)malloc(sizeof(Requisicao));
    if (strcmp(linha, "fim") != 0 && requisicao != NULL) {
        char delimitador = ';';
        int i = atoi(strtok(linha, &delimitador));
        int tempo_espera = atoi(strtok(NULL, &delimitador));
        requisicao->tempo_espera = tempo_espera;
        requisicao->id = id; id++;
        requisicao->i = i;
        return requisicao;
    }
    return NULL;
}

void processarRequisicao(Requisicao *requisicao) {
    mpfr_t pi;
    int i = requisicao->i;
    mpfr_init2(pi, i + 10);
    mpfr_const_pi(pi, MPFR_RNDN);
    mpfr_asprintf(&requisicao->pi, "%.*Rf", i, pi);
    mpfr_clear(pi);
}

int getI(Requisicao *requisicao) {
    return requisicao->i;
}

int getId(Requisicao *requisicao) {
    return requisicao->id;
}

char *getPi(Requisicao *requisicao) {
    return requisicao->pi;
}

int getTempoEspera(Requisicao *requisicao) {
    return requisicao->tempo_espera;
}

#endif
