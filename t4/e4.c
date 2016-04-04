#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#define UAD    5

pthread_t       tCentral;
pthread_t       * tUads;
pthread_mutex_t mutexes[UAD * 2] = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m_archivo        = PTHREAD_MUTEX_INITIALIZER;

void * centralCtrl(void *);
void * threadCtrl(void *);
void cambio(int);
void cambioCritico(int);

FILE * datos;
int  medidas[UAD];
int  alarmas[UAD];


int main(void)
{
    srand(time(NULL));

    tUads = (pthread_t*)malloc(sizeof(pthread_t) * UAD);

    for (int i = 0; i < UAD; i++)
        pthread_create(tUads + i, NULL, threadCtrl, i);
    pthread_create(&tCentral, NULL, centralCtrl, NULL);

    for (int i = 0; i < UAD; i++)
        pthread_join(*(tUads + i), NULL);
    pthread_join(tCentral, NULL);

    free(tUads);
    return 0;
}


void * threadCtrl(void * arg)
{
    int temp;
    while (1)
    {
        sleep(rand() % 3 + 1);
        pthread_mutex_lock(&(*(mutexes + (int)arg)));

        medidas[(int)arg] = rand() % 10;
        pthread_mutex_unlock(&(*(mutexes + (int)arg)));

        if (!(temp = rand() % 10))
        {
            printf("Uad (%d) :: Valor critico, alarma enviada\n", (int)arg);
            pthread_mutex_lock(mutexes + (int)arg + UAD);
            alarmas[(int)arg]++;
            pthread_kill(tCentral, SIGUSR1);
            pthread_mutex_unlock(mutexes + (int)arg + UAD);
        }
    }
    pthread_exit(NULL);
}

void * centralCtrl(void * arg)
{
    signal(SIGALRM, cambio);
    signal(SIGUSR1, cambioCritico);
    alarm(rand() % 5);
    while (1)
        ;
    pthread_exit(NULL);
}

void cambio(int ids)
{
    printf("Tomando registros\n");

    for (int i = 0; i < UAD; i++)
    {
        pthread_mutex_lock(mutexes + i);
        pthread_mutex_lock(&m_archivo);

        datos = fopen("datos", "a+");
        fprintf(datos, "Uad (%d) :: medida %d\n", i, medidas[i]);
        fclose(datos);
        pthread_mutex_unlock(&m_archivo);
        pthread_mutex_unlock(mutexes + i);
        alarm(rand() % 5);
    }
    printf("Cerrando archivo\n");
}

void cambioCritico(int ids)
{
    for (int i = 0; i < UAD; i++)
    {
        pthread_mutex_lock(mutexes + UAD + i);

        if (alarmas[i])
        {
            pthread_mutex_lock(&m_archivo);
            datos = fopen("datos", "a+");
            fprintf(datos, "Uad (%d) :: valor critico\n", i);
            fclose(datos);
            pthread_mutex_unlock(&m_archivo);
            alarmas[i]--;
        }
        pthread_mutex_unlock(mutexes + UAD + i);
    }
}
