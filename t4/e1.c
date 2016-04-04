#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#define NUM_ENANOS    7

int             sillas    = 4;
int             pendiente = 0;

pthread_mutex_t enanoMtx   = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t blancaMtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  enanoCond    = PTHREAD_COND_INITIALIZER;
pthread_cond_t  blancaCond  = PTHREAD_COND_INITIALIZER;

void * blancaCtrl(void*);
void * enanoCtrl(void*);


int main(void)
{
    int i = 0;

    srand(time(NULL));

    pthread_t tBlanca  = pthread_create(&tBlanca, NULL, blancaCtrl, NULL);
    pthread_t * tEnano = (pthread_t*)malloc(sizeof(pthread_t) * NUM_ENANOS);

    for (i = 0; i < NUM_ENANOS; i++)
        pthread_create(tEnano + i, NULL, enanoCtrl, i);

    for (i = 0; i < NUM_ENANOS; i++)
        pthread_join(*(tEnano + i), NULL);
    pthread_join(tBlanca, NULL);

    free(tEnano);

    return 0;
}

void * blancaCtrl(void * arg)
{
    while (1)
    {
        pthread_mutex_lock(&blancaMtx);
        if (pendiente == 0)
        {
            printf("B :: Vuelta\n");
            pthread_cond_wait(&blancaCond, &blancaMtx);
        }

        printf("B :: Pedir\n");
        pthread_mutex_unlock(&blancaMtx);
            printf("B :: Cocinando\n");
            sleep(rand() % 5);
            pthread_mutex_lock(&blancaMtx);
                pendiente--;
                pthread_cond_signal(&enanoCond);
        pthread_mutex_unlock(&blancaMtx);
        printf("B :: Liberando\n");
    }

    pthread_exit(NULL);
}



void * enanoCtrl(void * arg)
{
    while (1)
    {
        printf("E :: trabajando\n", (int)arg);
        sleep(rand() % 17);

        pthread_mutex_lock(&enanoMtx);

        if (sillas > 0)
        {
            sillas--;
            printf(" E(%d) :: Sentado en %d silla\n", (int)arg, sillas);
            pthread_mutex_unlock(&enanoMtx);

            pthread_mutex_lock(&blancaMtx);
                pendiente++;
            pthread_mutex_unlock(&blancaMtx);
            pthread_cond_signal(&blancaCond);
            printf(" E(%d) :: Pidiendo\n", (int)arg);

            pthread_mutex_lock(&blancaMtx);
                pthread_cond_wait(&enanoCond, &blancaMtx);
            pthread_mutex_unlock(&blancaMtx);
            printf(" E(%d) :: Comiendo\n", (int)arg);
            sleep(rand() % 2); //tiempo q tarda en enanoCondr

            pthread_mutex_lock(&enanoMtx);
                sillas++;
                printf(" E(%d) :: Dejando la %d silla\n", (int)arg, sillas);
            pthread_mutex_unlock(&enanoMtx);
        }
        else
        {
            printf(" E(%d) ::  No disponible\n", (int)arg);
            pthread_mutex_unlock(&enanoMtx);
        }
    }
    pthread_exit(NULL);
}
