#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define NUM_THREADS 20     // Número de carros
#define NUM_CANCELAS 7     // Número de cancelas
#define VAGAS_POR_CANCELA 10

sem_t cancelas[NUM_CANCELAS];  // Um semáforo por cancela

void *carro(void *arg) {
    int id = *((int *)arg);
    int cancela_escolhida = rand() % NUM_CANCELAS;

    printf("Carro %d tentando entrar na cancela %d\n", id, cancela_escolhida);

    // Tenta entrar na cancela (ocupar vaga)
    sem_wait(&cancelas[cancela_escolhida]);

    printf("Carro %d entrou na cancela %d\n", id, cancela_escolhida);
    sleep(1 + rand() % 3); // Simula tempo de passagem

    printf("Carro %d saindo da cancela %d\n", id, cancela_escolhida);
    sem_post(&cancelas[cancela_escolhida]); // Libera a vaga

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];

    srand(time(NULL));

    // Inicializa os semáforos (10 vagas por cancela)
    for (int i = 0; i < NUM_CANCELAS; i++) {
        sem_init(&cancelas[i], 0, VAGAS_POR_CANCELA);
    }

    // Cria as threads (carros)
    for (int i = 0; i < NUM_THREADS; i++) {
        ids[i] = i + 1;
        if (pthread_create(&threads[i], NULL, carro, &ids[i]) != 0) {
            perror("Erro ao criar thread");
            exit(1);
        }
        usleep(100000); // Simula chegada espaçada dos carros
    }

    // Espera todas as threads terminarem
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroi os semáforos
    for (int i = 0; i < NUM_CANCELAS; i++) {
        sem_destroy(&cancelas[i]);
    }

    printf("Todos os carros passaram pelas cancelas.\n");
    return 0;
}
