#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#define NUM_FUMADORES       3
#define NUM_INGREDIENTES    3
#define FALSE               0
#define TRUE                1


typedef struct Fumador
{
    int id;
    int tieneTabaco;
    int tienePapel;
    int tieneFosforo;
    int estaFumando;     //Para facilitar condicionales
    int estaProduciendo; //Para facilitar condicionales
    int idProduccion;    //Para indicar que ingrediente produce;
} Fumador;

typedef struct Mesa
{
    int hayTabaco;
    int hayPapel;
    int hayFosforo;
} Mesa;

pthread_mutex_t mutex         = PTHREAD_MUTEX_INITIALIZER;

Fumador         * fumadores;
Mesa            mesa;

void * fumadorCtrl(void *);
void * productorCtrl(void *);
void printMesa();
void printFumador(int);

int main(int argc, const char * argv[])
{
    pthread_t * tFumador   = (pthread_t*)malloc(sizeof(pthread_t) * NUM_FUMADORES);
    pthread_t * tProductor = (pthread_t*)malloc(sizeof(pthread_t) * NUM_FUMADORES);

    fumadores = (Fumador*)malloc(sizeof(Fumador) * NUM_FUMADORES);

    srand((int)time(NULL));

    //Iniciar fumadores
    for (int i = 0; i < NUM_FUMADORES; ++i)
    {
        fumadores[i].id              = i;
        fumadores[i].tieneTabaco     = FALSE;
        fumadores[i].tienePapel      = FALSE;
        fumadores[i].tieneFosforo    = FALSE;
        fumadores[i].estaFumando     = FALSE;
        fumadores[i].estaProduciendo = FALSE;
        fumadores[i].idProduccion    = i % 3;

        pthread_create((tFumador + i), NULL, fumadorCtrl, (void*)(fumadores + i));
        pthread_create((tProductor + i), NULL, productorCtrl, (void*)(fumadores + i));
    }


    mesa.hayTabaco  = FALSE;
    mesa.hayPapel   = FALSE;
    mesa.hayFosforo = FALSE;

    for (int i = 0; i < NUM_FUMADORES; ++i)
    {
        pthread_join(*(tFumador + i), NULL);
        pthread_join(*(tProductor + i), NULL);
    }

    free(fumadores);
    return 0;
}

void * fumadorCtrl(void * arg)
{
    Fumador* data = (struct Fumador*)arg;

    while (1 == 1)
    {
        int aFumar = FALSE;
        pthread_mutex_lock(&mutex);
        if (data->estaFumando == FALSE && data->estaProduciendo == FALSE &&
            data->tienePapel == TRUE && data->tieneTabaco == TRUE && data->tieneFosforo == TRUE)
        {
            aFumar             = TRUE;
            data->estaFumando  = TRUE;
            data->tienePapel   = FALSE;
            data->tieneTabaco  = FALSE;
            data->tieneFosforo = FALSE;
        }
        pthread_mutex_unlock(&mutex);

        if (aFumar == TRUE)
        {
            printf("Fumador (%d) :: esta fumando\n", data->id);
            sleep(2);
            printf("Fumador (%d) :: esperando\n", data->id);
            sleep(1);
        }

        pthread_mutex_lock(&mutex);

        data->estaFumando = FALSE;

        if (mesa.hayPapel == TRUE && data->tienePapel == FALSE)
        {
            printf("Fumador (%d) :: obtiene papel\n", data->id);
            data->tienePapel = TRUE;
            mesa.hayPapel    = FALSE;
            printFumador(data->id);
            printMesa();
        }

        if (mesa.hayTabaco == TRUE && data->tieneTabaco == FALSE)
        {
            printf("Fumador (%d) :: obtiene tabaco\n", data->id);
            data->tieneTabaco = TRUE;
            mesa.hayTabaco    = FALSE;
            printFumador(data->id);
            printMesa();
        }


        if (mesa.hayFosforo == TRUE && data->tieneFosforo == FALSE)
        {
            printf("Fumador (%d) :: obtiene fosforo\n", data->id);
            data->tieneFosforo = TRUE;
            mesa.hayFosforo    = FALSE;
            printFumador(data->id);
            printMesa();
        }

        pthread_mutex_unlock(&mutex);
    }
}

void * productorCtrl(void * arg)
{
    Fumador* data = (struct Fumador*)arg;

    while (1 == 1)
    {
        int aProducir;
        pthread_mutex_lock(&mutex);
        if (data->estaProduciendo == FALSE && data->estaFumando == FALSE)
        {
            aProducir             = TRUE;
            data->estaProduciendo = TRUE;
        }
        pthread_mutex_unlock(&mutex);

        if (aProducir == TRUE)
            sleep(1);

        pthread_mutex_lock(&mutex);

        int idProduccion = data->idProduccion;

        if (idProduccion % 3 == 0 && mesa.hayPapel == FALSE)
        {
            mesa.hayPapel = TRUE;
            printf("Produciendo papel\n");
            printMesa();
        }

        if (idProduccion % 3 == 1 && mesa.hayTabaco == FALSE)
        {
            mesa.hayTabaco = TRUE;
            printf("Produciendo tabaco\n");
            printMesa();
        }

        if (idProduccion % 3 == 2 && mesa.hayFosforo == FALSE)
        {
            mesa.hayFosforo = TRUE;
            printf("Produciendo fosforo\n");
            printMesa();
        }

        data->estaProduciendo = FALSE;

        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

void printMesa()
{
    printf("     Mesa -> ");
    printf(mesa.hayPapel ? "P" : "-");
    printf(mesa.hayFosforo ? "F" : "-");
    printf(mesa.hayTabaco ? "T" : "-");
    printf("\n");
}


void printFumador(int id)
{
    printf("     Fumador -> ");
    printf(fumadores[id].tienePapel ? "P" : "-");
    printf(fumadores[id].tieneFosforo ? "F" : "-");
    printf(fumadores[id].tieneTabaco ? "T" : "-");
    printf("\n");
}
