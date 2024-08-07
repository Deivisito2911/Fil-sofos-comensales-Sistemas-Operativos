#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h> // Libreria para los semaforos
#include <unistd.h>
// Sistemas Operativos 2024
// Profesor : Lic. Andres Luna
// Integrantes :
// Br Deivith Zanella
// Br Thamar Rodriguez
// Br Carlos Jimenez
#define CANT_FILOSOFOS 5 

pthread_t filosofos[CANT_FILOSOFOS];//Hilos de los filosofos
sem_t tenedores[CANT_FILOSOFOS];//Los semaforos de los tenedores
int inicio = 1; //Controlar la ejecucion de los filosofos

void *filosofo(void *arg){//Bucle, cada filoso lo reflejamos como una funcion que se ejecuta
                          //en un hilo independiente
    int id = *((int *)arg);//Numero de filosofo
    int izquierda = id;//tenedores a la izquierda
    int derecha = (id + 1) % CANT_FILOSOFOS;//tenedores a la derecha

    while (inicio){//Bucle infinito que sera el ciclo de pensar y comer
        printf("Filosofo %d pensando. \n", (id+1));
        usleep(rand() %1000000); //Pensamiento

        printf("Filosofo %d con hambre. \n", (id+1));//Mensaje para saber que tiene el filoso

        sem_wait(&tenedores[izquierda]); //Esperar por el tenedor izquierdo
        sem_wait(&tenedores[derecha]); //Esperar por el tenedor derecho

        printf("Filosofo %d usa los tenedores (%d, %d). \n", (id+1), (izquierda+1), (derecha+1));

        printf("Filosofo %d comiendo. \n", (id+1));
        usleep(rand() %1000000); //Comiendo

        sem_post(&tenedores[izquierda]);// Termina de usar el tenedor derecho
        sem_post(&tenedores[derecha]);// Termina de usar el tenedor izquierdo
        printf("Filosofo %d regreso los tenedores (%d, %d). \n", (id+1), (izquierda+1), (derecha+1));

    }
    pthread_exit(NULL);
}

int main() {//Funcion principal
    srand(time(NULL));//Para generar numeros aleatorios
    int i;
    int id[CANT_FILOSOFOS];

    for ( i = 0; i < CANT_FILOSOFOS; i++){
        sem_init(&tenedores[i],0,1);//Creacion de semaforos con los tenedores disponibles
        id[i] = i;//pasamos el id de los filosofos como argumento para crear hilos
        pthread_create(&filosofos[i], NULL, filosofo, &id[i]);
    }

    sleep(8);//Esperamos durante 8 segundos la ejecucion del problema

    inicio = 0;//Se para la ejecucion poniendo el inicio a 0

    for ( i = 0; i < CANT_FILOSOFOS; i++){
        pthread_join(filosofos[i], NULL);
        sem_destroy(&tenedores[i]);//se destruyen los semaforos
    }//Se liberan los recursos antes de la finalizacion

    return 0;
        
}