#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>


#define NUM_CAJEROS     8
#define N               150
#define MAX_FILA        150
#define NUM_CLIENTES    150
typedef struct Data
{
    int id;
    int tipo;   // 1 general; 2 empresarial
} Data;

Data            fila[MAX_FILA];
int             in        = 0;
int             out       = 0;
int             formados  = 0;
int             atendidos = 0;

pthread_mutex_t mutex     = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cajero_t  = PTHREAD_COND_INITIALIZER;
pthread_cond_t  cliente_t = PTHREAD_COND_INITIALIZER;

void * cajero(void *);
void * cliente(void *);



int main(int argc, const char * argv[])
{
    int       numThreads      = NUM_CAJEROS + NUM_CLIENTES;
    pthread_t * threads       = (pthread_t*)malloc(sizeof(pthread_t) * numThreads);
    Data      * datosCajeros  = (Data*)malloc(sizeof(Data) * (NUM_CAJEROS + 1));
    Data      * datosClientes = (Data*)malloc(sizeof(Data) * (NUM_CLIENTES + 1));


    pthread_t * aux;
    int       idCajero = 1;

    srand((int)time(NULL));

    for (aux = threads; aux < (threads + NUM_CAJEROS); aux++)
    {
        (datosCajeros + idCajero)->id = idCajero;
        if (idCajero <= 5)
            (datosCajeros + idCajero)->tipo = 1;
        else
            (datosCajeros + idCajero)->tipo = 2;
        pthread_create(aux, NULL, cajero, (void*)(datosCajeros + idCajero));
        idCajero++;
    }

    int idCliente = 1;
    for (; aux < (threads + numThreads); ++aux)
    {
        (datosClientes + idCliente)->id = idCliente;
        if (idCliente <= 100)
            (datosClientes + idCliente)->tipo = 1;
        else
            (datosClientes + idCliente)->tipo = 2;
        pthread_create(aux, NULL, cliente, (void*)(datosClientes + idCliente));
        idCliente++;
    }


    for (aux = threads; aux < (threads + numThreads); ++aux)
        pthread_join(*aux, NULL);

    free(datosClientes);
    free(datosCajeros);
    free(threads);

    return 0;
}

void * cajero(void * arg)
{
    Data* data = (struct Data*)arg;
    int id     = data->id;
    int tipo   = data->tipo;

    while (atendidos < N)
    {
        usleep(3 + rand() % 5);

        pthread_mutex_lock(&mutex);

        int hayCEmpresarial = 0;

        //Checa si hay clientes empresariales formados
        for (int i = 0; i < formados; i++)
        {
            if (fila[out].tipo == 2)
                hayCEmpresarial = 1;
        }

        if (formados > 0 &&
            //Si es tipo 1 pasan todos, si es 2 solo si no hay empresariales o
            //si pasa un empresarial
            (tipo == 1
             || (tipo == 2 && hayCEmpresarial == 0)
             || (tipo == 2 && fila[out].tipo == 2)))
        {
            printf(" +++ Atendiendo a %d por caja %d de tipo %s\n",
                   fila[out].id, id, fila[out].tipo == 1 ? "general" : "empresarial");
            usleep(3);

            atendidos++;
            out++;
            out %= MAX_FILA;
            formados--;

            if (formados == MAX_FILA - 1)
                pthread_cond_broadcast(&cliente_t);
        }
        else
        {
            pthread_cond_wait(&cajero_t, &mutex);
        }

        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

void * cliente(void * arg)
{
    Data* data = (struct Data*)arg;
    int id     = data->id;
    int tipo   = data->tipo;
    int status = 1;

    while (atendidos < N)
    {
        if (tipo == 1)
            usleep(10 + rand() % 5);
        else
            usleep(20 + rand() % 5);

        pthread_mutex_lock(&mutex);

        if (formados < MAX_FILA && status == 1)
        {
            fila[in] = *data;
            printf("** Llego el cliente %d %s\n", id, tipo == 1 ? "general" : "empresarial");

            in++;
            formados++;
            in    %= MAX_FILA;
            status = 0;

            if (formados == 1)
                pthread_cond_broadcast(&cajero_t);
        }
        else
        {
            pthread_cond_wait(&cliente_t, &mutex);
        }
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}
