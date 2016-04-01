//Nota: si se compila con gcc no olvidar -pthread y -lm

#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>


#define C        5  //Num cines
#define T        3  //Num taquillas por cine
#define S        5  //Num salas por cine
#define U        50 //Num usuarios
#define CA       5  //Capacidad de salas
#define FALSE    0
#define TRUE     1


typedef struct Sala
{
    int id;
    int cineId;
    int status;
    int ocupados;
} Sala;

typedef struct Taquilla
{
    int id;
    int cineId;
} Taquilla;

typedef struct Usuario
{
    int id;
} Usuario;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int             atendidos = 0;
int             iCola     = 0;
Usuario         * cola;
Sala            * salas;
Taquilla        * taquillas;
Usuario         * usuarios;


void * salaCtrl(void *);
void * taquillaCtrl(void *);
void * usuarioCtrl(void *);


int main(int argc, const char * argv[])
{
    pthread_t * tSala     = (pthread_t*)malloc(sizeof(pthread_t) * C * S);
    pthread_t * tTaquilla = (pthread_t*)malloc(sizeof(pthread_t) * C * T);
    pthread_t * tUsuario  = (pthread_t*)malloc(sizeof(pthread_t) * U);

    cola      = (Usuario*)malloc(sizeof(Usuario) * U);
    salas     = (Sala*)malloc(sizeof(Sala) * C * S);
    taquillas = (Taquilla*)malloc(sizeof(Taquilla) * C * T);
    usuarios  = (Usuario*)malloc(sizeof(Usuario) * U);


    srand((int)time(NULL));

    for (int i = 0; i < C * S; ++i)
    {
        salas[i].id       = (int)floor(i / S) + 1;
        salas[i].cineId   = (i % C) + 1;
        salas[i].status   = FALSE;
        salas[i].ocupados = 0;
        pthread_create((tSala + i), NULL, salaCtrl, (void*)(salas + i));
    }

    for (int i = 0; i < C * T; ++i)
    {
        taquillas[i].id     = (int)floor(i / T) + 1;
        taquillas[i].cineId = (i % C) + 1;
        pthread_create((tTaquilla + i), NULL, taquillaCtrl, (void*)(taquillas + i));
    }

    for (int i = 0; i < U; ++i)
    {
        usuarios[i].id = i + 1;
        pthread_create((tUsuario + i), NULL, usuarioCtrl, (void*)(usuarios + i));
    }

    for (int i = 0; i < C * S; ++i)
        pthread_join(*(tSala + i), NULL);
    for (int i = 0; i < C * T; ++i)
        pthread_join(*(tTaquilla + i), NULL);
    for (int i = 0; i < U; ++i)
        pthread_join(*(tUsuario + i), NULL);

    free(cola);
    free(salas);
    free(taquillas);
    free(usuarios);

    return 0;
}

void * salaCtrl(void * arg)
{
    Sala* data = (struct Sala*)arg;
    int id     = data->id;
    int cineId = data->cineId;

    while (atendidos < U || data->status == TRUE)
    {
        if (data->status == TRUE)
            sleep(rand() % 10);  //Reproduce pelicula

        pthread_mutex_lock(&mutex);

        if (data->ocupados > 0 && data->status == FALSE)
        //Si hay personas y no se está reproduciendo
        {
            printf(" * Inicia pelicula en sala %d :: cine %d :: (%d/%d) \n",
                   id, cineId, data->ocupados, CA);
            data->status = TRUE;
        }
        else
        {
            if (data->status == TRUE)
            {
                printf(" / Termina pelicula en sala %d :: cine %d :: (%d/%d) \n",
                       id, cineId, data->ocupados, CA);
                data->status   = FALSE;
                data->ocupados = 0;
            }
        }
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

void * taquillaCtrl(void * arg)
{
    Taquilla* data = (struct Taquilla*)arg;
    int     id     = data->id;
    int     cineId = data->cineId;

    sleep(rand() % 4);

    while (atendidos < U)
    {
        int posSala = -1;

        pthread_mutex_lock(&mutex);

        for (int i = 0; i < C * S; i++)
        {
            if (salas[i].cineId == cineId    //Sala perteneciente a mismo cine
                && salas[i].ocupados < CA    //Con suficientes lugares
                && salas[i].status == FALSE) //Que no este iniciada
                posSala = i;
        }

        if (iCola > 0 && posSala != -1)
        {
            Usuario usuario = cola[iCola];
            iCola--;
            atendidos++;
            salas[posSala].ocupados++;
            printf(" - Atendiendo al usuario %d :: taquilla %d :: cine %d :: sala %d \n",
                   usuario.id, id, cineId, salas[posSala].id);
        }
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

void * usuarioCtrl(void * arg)
{
    Usuario* data = (struct Usuario*)arg;
    int    id     = data->id;

    usleep(rand() % (id + 1));
    pthread_mutex_lock(&mutex);
    printf(" + Llegó el usuario %d \n", id);
    iCola++;
    cola[iCola] = *data;
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}
