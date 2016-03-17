#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define N    10

sem_t semaforo_cuchillos;
sem_t semaforo_tenedores;
sem_t semaforo_sillas;


void* comer(void*);

int main()
{
    sem_init(&semaforo_tenedores, 0, 3);
    sem_init(&semaforo_cuchillos, 0, 3);
    sem_init(&semaforo_sillas, 0, 4);

    srand((int)time(NULL));
    pthread_t* threads = (pthread_t*)malloc(N * sizeof(pthread_t));
    pthread_t* aux;

    int      id = 1;
    for (aux = threads; aux < (threads + N); ++aux)
    {
        pthread_create(aux, NULL, comer, (void*)id);
        id++;
    }

    for (aux = threads; aux < (threads + N); ++aux)
        pthread_join(*aux, NULL);


    sem_destroy(&semaforo_cuchillos);
    sem_destroy(&semaforo_sillas);
    sem_destroy(&semaforo_tenedores);

    free(threads);

    return 0;
}

void* comer(void* p)
{
    int id = (int)p;

    sem_wait(&semaforo_sillas);
    printf("Sentar %d\n", id);

    sem_wait(&semaforo_cuchillos);
    printf("Cuchillo %d\n", id);

    sem_wait(&semaforo_tenedores);
    printf("Tenedor %d\n", id);

    printf("Comer %d\n", id);
    sleep((rand() % 16) + 5);
    printf("Terminar %d\n", id);

    sem_post(&semaforo_cuchillos);
    sem_post(&semaforo_tenedores);
    sem_post(&semaforo_sillas);

    pthread_exit(NULL);
}
