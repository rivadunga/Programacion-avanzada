/**
 *  NOTACIÓN MAPA:
 *      * = recorrido
 *      0 = libre
 *      X = obstaculo
 */

#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>
#include <omp.h>

#define N        12
#define P        9
#define QS       4
#define TRUE     1
#define FALSE    0

typedef struct Data
{
    int id;
    int iniX;
    int iniY;
    int solved;
    int isDiagonal;
    int solution[QS][QS];
} Data;


int  map[N][N];
int  numQ;
Data * quadrantData;
int  flagSignal = FALSE;

void * quadrantCtrl(void *);
void signalCtrl(int);

void search(int, int, int, int[QS][QS]);

int main()
{
    pthread_t * threads = (pthread_t*)malloc(sizeof(pthread_t) * P);

    quadrantData = (Data*)malloc(sizeof(Data) * P);
    srand((int)time(NULL));

    //Signal
    signal(SIGUSR1, signalCtrl);

    //Map
    #pragma omp parallel for
    for (int j = 0; j < N; j++)
        for (int i = 0; i < N; i++)
            map[i][j] =
                rand() % 4 == 1 &&
                (i % QS != 0 || j % QS != 0) &&
                (i % QS != (QS -1) || j % QS != QS - 1) ? 1 : 0;


    //Print map
    printf("\n");
    printf("MAP\n");
    for (int j = 0; j < N; j++)
    {
        for (int i = 0; i < N; i++)
        {
            printf("%i ", map[i][j]);
        }
        printf("\n");
    }
    printf("\n");


    //Threads
    int ind = 0;
    for (int j = 0; j < N; j += QS)
        for (int i = 0; i < N; i += QS)
        {
            quadrantData[ind].id         = ind;
            quadrantData[ind].iniX       = i;
            quadrantData[ind].iniY       = j;
            quadrantData[ind].solved     = FALSE;
            quadrantData[ind].isDiagonal = i == j ? TRUE : FALSE;
            ind++;
        }

    for (int i = 0; i < P; ++i)
        pthread_create((threads + i), NULL, quadrantCtrl, (void*)(quadrantData + i));

    for (int i = 0; i < P; ++i)
        pthread_join(*(threads + i), NULL);

    //Print results
    printf("\n==============================\n");
    printf("Solution \n");
    printf("Order ►  0, 1, 2  ▼ \n");
    for (int k = 0; k < P; ++k)
    {
        printf("\n");
        printf("Quadrant :: %i \n", k);
        for (int j = 0; j < QS; j++)
        {
            for (int i = 0; i < QS; i++)
            {
                if (quadrantData[k].solution[i][j] == 0)
                    printf("0 ");
                else if (quadrantData[k].solution[i][j] == 1)
                    printf("X ");
                else if (quadrantData[k].solution[i][j] == 2)
                    printf("* ");
            }
            printf("\n");
        }
        printf("\n");
    }

    return 0;
}


void signalCtrl(int n)
{
    flagSignal = TRUE;
}

void * quadrantCtrl(void * arg)
{
    Data * data = (Data*)arg;
    int  id     = data->id;
    int  iniX   = data->iniX;
    int  iniY   = data->iniY;

    int  subMap[QS][QS];

    for (int j = iniY; j < iniY + QS; j++)
        for (int i = iniX; i < iniX + QS; i++)
            subMap[i - iniX][j - iniY] = map[i][j];

    if (data->isDiagonal)
        search(0, 0, id, subMap);

    //Save solution
    for (int j = 0; j < QS; j++)
        for (int i = 0; i < QS; i++)
            data->solution[i][j] = subMap[i][j];
}

void search(int x, int y, int id, int subMap[QS][QS])
{
    if (subMap[x][y] == 0 &&
        x < QS && x >= 0 &&
        y < QS && y >= 0 &&
        flagSignal == FALSE && //Check the signal
        quadrantData[id].solved == FALSE)
    {
        subMap[x][y] = 2;

        if (x == QS - 1 && y == QS - 1)
            quadrantData[id].solved = TRUE;

        //Print each step
        printf("\n");
        printf("Q(id) :: %i \n", id);
        for (int j = 0; j < QS; j++)
        {
            for (int i = 0; i < QS; i++)
            {
                if (subMap[i][j] == 0)
                    printf("0 ");
                else if (subMap[i][j] == 1)
                    printf("X ");
                else if (subMap[i][j] == 2)
                    printf("* ");
            }
            printf("\n");
        }
        printf("\n");

        //Recursive search
        search(x + 1, (y + 1), id, subMap);
        search((x + 1), y, id, subMap);
        search(x, (y + 1), id, subMap);
    }
}
