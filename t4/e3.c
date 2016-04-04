#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NUM_PISOS     5
#define NUM_ROBOTS    5

pthread_t       *tRobots;
pthread_mutex_t *nivelMtxs;
pthread_cond_t  *nivelConds;

void * robotCtrl(void*);

int pesoRobot[NUM_ROBOTS];
int pesoPisos[NUM_PISOS];
int pesoTotal[NUM_PISOS];


int main(void)
{
    tRobots    = (pthread_t*)malloc(NUM_ROBOTS * 2 * sizeof(pthread_t));
    nivelMtxs  = (pthread_mutex_t*)malloc(NUM_PISOS * sizeof(pthread_mutex_t));
    nivelConds = (pthread_cond_t*)malloc(NUM_PISOS * sizeof(pthread_cond_t));

    srand(time(NULL));

    for (int i = 0; i < NUM_PISOS; ++i)
    {
        pesoPisos[i] = rand() % 10 + 5;
        pesoTotal[i] = 0;
        printf("Sección(%d) ::  peso %d \n", i, pesoPisos[i]);
    }

    for (int i = 0; i < NUM_PISOS; ++i)
    {
        pthread_mutex_init(nivelMtxs + i, NULL);
        pthread_cond_init(nivelConds + i, NULL);
    }

    for (int i = 0; i < NUM_ROBOTS; i++)
    {
        pesoRobot[i] = rand() % 4 + 1;
        printf("Robot(%d) :: peso %d \n", i, pesoRobot[i]);
        pthread_create(tRobots + i, NULL, robotCtrl, (i));
    }

    for (int i = 0; i < NUM_ROBOTS; ++i)
        pthread_join(*(tRobots), NULL);

    free(nivelMtxs);
    free(nivelConds);
    free(tRobots);

    return 0;
}

void *robotCtrl(void *arg)
{
    printf("Comprar robot(%d)\n", (int)arg);

    int cont = 0;
    while (cont < NUM_PISOS)
    {
        pthread_mutex_lock(nivelMtxs + cont);
        if (pesoRobot[(int)arg] + pesoTotal[cont] > pesoPisos[cont])
        {
            printf("Robot(%d) :: espera a que baje el peso\n", (int)arg);
            pthread_cond_wait(nivelConds + cont, nivelMtxs + cont);
            pthread_mutex_unlock(nivelMtxs + cont);
        }
        else
        {
            pesoTotal[cont] += pesoRobot[(int)arg];
            pthread_mutex_unlock(nivelMtxs + cont);
            printf("Robot(%d) :: compra en sección %d peso %d  de  %d\n", (int)arg, cont, pesoTotal[cont], pesoPisos[cont]);
            int compra = rand() % 3;
            sleep(compra);
            printf("Robot(%d) :: terminó %d\n", (int)arg, cont);

            pthread_mutex_lock(nivelMtxs + cont);
            pesoTotal[cont] -= pesoRobot[(int)arg];
            pthread_cond_broadcast(nivelConds + cont);
            pthread_mutex_unlock(nivelMtxs + cont);
            cont++;
        }
    }

    pthread_exit(NULL);
}
