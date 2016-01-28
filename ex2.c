#include <stdio.h>
#include <stdlib.h>


typedef struct  Libro {
    char * titulo;
    int paginas;
} Libro;

typedef void (*t_imprime)(void*);
typedef void  (*t_iterator)(void*, size_t,t_imprime);


void* begin(void* datos){
    return datos;
}

void* next(void* datos, size_t size){
    return (datos+size);
}

void* prev(void* datos, size_t size){
    return (datos-size);
}

void* end(void* datos,size_t size, int len){
    return (datos+(size*len));
}

void forwardIt(void* datos, size_t size, int len, t_imprime imprime)
{
    void* temp = begin(datos);
    while(temp != end(datos,size,len)){
        imprime(temp);
        temp = next(temp,size);
    }
}

void imprimeChar(char* dato)
{
    printf("%c",*dato);
}

int main(int argc, const char * argv[])
{
    char* datos = "Hola q ace";
    forwardIt(datos,sizeof(char),10,&imprimeChar);
}




/*
int main()
{

    // Ordenar números enteros

    int * vector = (int *) malloc(N * sizeof(int));

    int * aux = vector;
    int * last = vector + N;

    for (; aux < last; ++aux) {
        *aux = rand() % 100;
    }

    printf("\n\n--- Enteros ---\n\n");

    imprimeInt(vector, N);

    sort(&selectionSort, vector, N, sizeof(*vector), &ascInt, &swapInt);

    printf("\n\n--- Enteros Ordenados ---\n\n");

    imprimeInt(vector, N);

    free(vector);

    // Ordenar números flotantes

    float * vectorf = (float *) malloc(N * sizeof(float));


    float * auxf = vectorf;
    float * lastf = vectorf + N;

    for (; auxf < lastf; ++auxf) {
        *auxf = rand() % 100 * 1.5;
    }

    printf("\n\n--- Flotantes ---\n\n");

    imprimeFloat(vectorf, N);

    sort(&insertionSort, vectorf, N, sizeof(*vectorf), &descFloat, &swapFloat);

    printf("\n\n--- Flotantes Ordenados ---\n\n");

    imprimeFloat(vectorf, N);

    free(vectorf);

    return 0;
}

// Implementación de funciones genéricas

void sort(t_sort algorithm, void * vector, size_t count, size_t size, t_compare compare, t_swap swap )
{
    (*algorithm)(vector, count, size, compare, swap);
}

void insertionSort(void * vector, size_t count, size_t size, t_compare compare, t_swap swap )
{
    void * j;
    void * start = vector;
    void * aux = start;
    void * last = start + (count * size);

    for (; aux < last ; aux+=size)
    {
        j = aux - size;
        while (j >= start && (*compare)(j, j+size) )
        {
            (*swap)(j+size, j);
            j-=size;
        }
    }
}

void selectionSort(void * vector, size_t count, size_t size, t_compare compare, t_swap swap )
{
    void * minimo;

    void * j;
    void * start = vector;
    void * aux = start;
    void * last = start + (count * size);
    void * prelast = last - size;

    for(; aux < prelast; aux+=size)
    {
        minimo = aux;
        for(j = aux + size; j < last; j+=size)
        {
            if ( (*compare)(minimo, j) )
                minimo = j;
        }

        (*swap)(minimo, aux);
    }
}

// Implementación de funciones callback para enteros

int ascInt(void * pa, void * pb)
{
    int * a = (int *)pa;
    int * b = (int *)pb;
    return (*a > *b);
}

int descInt(void * pa, void * pb)
{
    int * a = (int *)pa;
    int * b = (int *)pb;
    return (*a < *b);
}

void swapInt(void * pa , void * pb )
{
    int temp;
    int * a = (int *)pa;
    int * b = (int *)pb;

    temp = *a;
    *a = *b;
    *b = temp;

}

// Implementación de funciones callback para flotantes

int ascFloat(void * pa, void * pb)
{
    float * a = (float *)pa;
    float * b = (float *)pb;
    return (*a > *b);
}

int descFloat(void * pa, void * pb)
{
    float * a = (float *)pa;
    float * b = (float *)pb;
    return (*a < *b);
}

void swapFloat(void * pa , void * pb )
{
    float temp;
    float * a = (float *)pa;
    float * b = (float *)pb;

    temp = *a;
    *a = *b;
    *b = temp;

}

// Implementación de funciones de impresión

void imprimeInt(int * vector, int count)
{
    int * aux = vector;
    int * last = vector + count;

    for (; aux < last; ++aux) {
        printf(" %4d ", *aux);
    }

    printf("\n");
}

void imprimeFloat(float * vector, int count)
{
    float * aux = vector;
    float * last = vector + count;

    for (; aux < last; ++aux) {
        printf(" %7.2f ", *aux);
    }

    printf("\n");
}
*/
