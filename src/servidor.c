#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "requisicao.h"
#include "dlq.h"

#define NUM_THREADS 4
#define TEMPO_REQ 500

DLQueue *fila;
int requisicoes = 0;
pthread_mutex_t mutex;
pthread_cond_t vazio;

void dormir(int ms) {
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000;
    nanosleep(&ts, NULL);
}

void gerarEntrada(int n) {
    FILE *arquivo;
    arquivo = fopen("files/requisicoes.txt", "w");
    if (arquivo == NULL) {
        printf("Erro!: Falha ao criar/abrir o arquivo de entrada.\n");
        exit(1);
    }
    srand(time(NULL));
    for (int k = 0; k<n; k++) {
        int i = rand()%91 + 10;
        int tempo_espera = rand()%1001 + 500;
        fprintf(arquivo, "%d;%d\n", i, tempo_espera);
    }
    fprintf(arquivo, "fim");
    fclose(arquivo);
}

void escreverSaida(FILE *arquivo, Requisicao *requisicao) {
    int i = getI(requisicao);
    int id = getId(requisicao);
    char *pi = getPi(requisicao);
    int tempo_espera = getTempoEspera(requisicao);
    fprintf(arquivo, "%d;%d;%d;%s\n", id, i, tempo_espera, pi);
    printf("Requisição %d Processada!\n", id);
}

void *threadTrabalhadora(void *arg) {
    char nome[30];
    FILE *arquivo;
    Requisicao *requisicao;
    snprintf(nome, sizeof(nome), "files/trabalhadora%d.txt", *(int *)arg);
    arquivo = fopen(nome, "w");
    if (arquivo == NULL) {
        printf("Erro!: Falha ao criar/abrir arquivo de saída.\n");
        exit(1);
    }
    printf("Thread Trabalhadora %d Criada!\n", *(int *)arg);
    while (1) {
        pthread_mutex_lock(&mutex);
        if (requisicoes == -1) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        while (requisicoes == 0) {
            pthread_cond_wait(&vazio, &mutex);
        }
        requisicao = (Requisicao *)dlqDequeue(fila);
        requisicoes--;
        if (requisicao == NULL) {
            requisicoes = -1;
            pthread_mutex_unlock(&mutex);
            break;
        }
        pthread_mutex_unlock(&mutex);
        processarRequisicao(requisicao);
        dormir(getTempoEspera(requisicao));
        escreverSaida(arquivo, requisicao);
    }
    fclose(arquivo);
    pthread_exit(NULL);
}

void *threadDespachadora(void *arg) {
    Requisicao *requisicao;
    FILE *arquivo = (FILE *)arg;
    printf("Thread Despachadora Criada!\n");
    while (1) {
        requisicao = proximaRequisicao(arquivo);
        pthread_mutex_lock(&mutex); requisicoes++;
        dlqEnqueue(fila, (void *)requisicao);
        pthread_cond_broadcast(&vazio);
        pthread_mutex_unlock(&mutex);
        if (requisicao == NULL) {
            break;
        }
        dormir(TEMPO_REQ);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    struct stat buffer;
    if (stat("files", &buffer) != 0) {
        if (mkdir("files", 0777) != 0) {
            printf("Erro!: Não foi possível criar a pasta 'files'.\n");
            return -1;
        }
    }

    if (argc < 2) {
        printf("Erro!: Passe a quantidade de requisições.\n");
        return 1;
    } else {
        gerarEntrada(atoi(argv[1]));
    }

    int res;
    int IDs[NUM_THREADS];
    pthread_t despachadora;
    pthread_t trabalhadoras[NUM_THREADS];
    FILE *arquivo = fopen("files/requisicoes.txt", "r");

    fila = dlqCreate();
    if (fila == NULL) {
        printf("Erro!: Não foi possível criar a fila.\n");
        return -1;
    }
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&vazio, NULL);

    res = pthread_create(&despachadora, NULL, threadDespachadora, (void *)arquivo);
    if (res != 0) {
        printf("Erro!: Não foi possível criar uma das threads.\n");
        return -1;
    }
    for (int i = 0; i<NUM_THREADS; i++) {
        IDs[i] = i;
        res = pthread_create(&trabalhadoras[i], NULL, threadTrabalhadora, (void *)&IDs[i]);
        if (res != 0) {
            printf("Erro!: Não foi possível criar uma das threads.\n");
            return -1;
        }
    }

    pthread_join(despachadora, NULL);
    for (int i = 0; i<NUM_THREADS; i++) {
        pthread_join(trabalhadoras[i], NULL);
    }

    printf("Processamento Concluído com Sucesso!\n");
    
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&vazio);
    dlqDestroy(fila);
    fclose(arquivo);

    pthread_exit(NULL);
    return 0;
}
