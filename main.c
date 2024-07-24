#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define CANT_FILOSOFOS 5

pthread_t filosofos[CANT_FILOSOFOS];
sem_t tenedores[CANT_FILOSOFOS];
int inicio = 1;

void *filosofo(void *arg){
    int id = *((int *)arg);
    int izquierda = id;
    int derecha = (id + 1) % CANT_FILOSOFOS;

    while (inicio){//Bucle infinito
        printf("Filosofo %d pensando. \n", id);
        usleep(rand() %1000000); //Pensamiento

        printf("Filosofo %d con hambre. \n", id);

        sem_wait(&tenedores[izquierda]); //Esperar por el tenedor izquierdo
        sem_wait(&tenedores[derecha]); //Esperar por el tenedor derecho

        printf("Filosofo %d usa los tenedores (%d, %d). \n", id, izquierda, derecha);

        printf("Filosofo %d comiendo. \n", id);
        usleep(rand() %1000000); //Comiendo

        sem_post(&tenedores[izquierda]);// Termina de usar el tenedor derecho
        sem_post(&tenedores[derecha]);// Termina de usar el tenedor izquierdo
        printf("Filosofo %d regreso los tenedores (%d, %d). \n", id, izquierda, derecha);

    }
    pthread_exit(NULL);
}

int main() {
    srand(time(NULL));
    int i;
    int id[CANT_FILOSOFOS];

    for ( i = 0; i < CANT_FILOSOFOS; i++){
        sem_init(&tenedores[i],0,1);
        id[i] = i;
        pthread_create(&filosofos[i], NULL, filosofo, &id[i]);
    }//Creacion

    sleep(8);

    inicio = 0;

    for ( i = 0; i < CANT_FILOSOFOS; i++){
        pthread_join(filosofos[i], NULL);
        sem_destroy(&tenedores[i]);
    }

    return 0;
        
}