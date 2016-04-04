#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t lock  = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond  = PTHREAD_COND_INITIALIZER;

void* formarCtrl(void *);
void* pasarCtrl(void *);

int sig   = -1;
int dir   = -1;
int numM  = 0;
int numH  = 0;
int flagS = 0;
int flagO = 0;


int main(void)
{
    pthread_t * threads = (pthread_t*)malloc(sizeof(pthread_t) * 3);

    pthread_create(threads, NULL, &formarCtrl, 2);
    pthread_create(threads + 1, NULL, &pasarCtrl, 0);
    pthread_create(threads + 2, NULL, &pasarCtrl, 1);

    pthread_join(*(threads), NULL);
    pthread_join(*(threads + 1), NULL);
    pthread_join(*(threads + 2), NULL);

    free(threads);
    return 0;
}


void* formarCtrl(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&lock2);
        pthread_mutex_lock(&lock);

        int temp = rand() % 2;

        if (temp % 2 == 0)
        {
            numM++;
            printf(" Llega mujer :: %d esperando\n", numM);

            if (flagS == 0)
            {
                dir   = 0;
                flagS = 1;
            }
        }

        if (temp % 3 == 0)
        {
            numH++;
            printf("Llega hombre :: %d esperando\n", numH);

            if (flagS == 0)
            {
                dir   = 1;
                flagS = 1;
            }
        }

        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&lock);
        sig = 1;
    }
}


void* pasarCtrl(void * arg)
{
    int args = (int)arg;

    while (1)
    {
        int sum = 0;
        pthread_mutex_lock(&lock);

        while (sig == -1)
            pthread_cond_wait(&cond, &lock);

        if (flagO == 0)
            printf(" Baño vacío\n");
        if (flagO == 1)
            printf(" Entra mujer\n");
        if (flagO == 2)
            printf(" Entra hombre\n");

        if (args == 0 && dir == 0 && numM > 0)
        {
            flagO = 1;
            numM--;
            printf(" Sale mujer :: %d esperando\n", numM);
        }

        if (args == 0 && dir == 1 && numH > 0)
        {
            flagO = 2;
            numH--;
            printf(" Sale mujer :: %d esperando\n", numH);
        }

        if (numH == 0 || numM == 0)
        {
            flagO = 0;
            if (numM == 0)
            {
                flagO = 2;
                dir   = 1;
            }
            if (numH == 0)
            {
                flagO = 1;
                dir   = 0;
            }
        }

        pthread_mutex_unlock(&lock);
        pthread_mutex_unlock(&lock2);
        sleep(1);
    }
}
