#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int N, P;

void crearNodos(int level)
{
    if (level < N)
    {
        pid_t pid;
        pid_t pid2;
        int   i, j, status;
        pid = fork();

        if (pid == -1)
        {
            printf("error\n");
        }
        else if (pid == 0)
        {
            printf("%i Nodo: [%i] Padre[%i]\n", (level + 1), getpid(), getppid());
            for (i = 0; i < P; i++)
            {
                crearNodos(level + 1);
            }
            exit(0);
        }
        else
        {
            while ((pid2 = wait(&status)) > 0)
            {
            }
        }
    }
}

int main(int argc, char * argv[])
{
    int c;

    while ((c = getopt(argc, argv, "n:p:")) != -1)
        switch (c)
        {
        case 'n':
            N = atoi(optarg);
            break;

        case 'p':
            P = atoi(optarg);
            break;

        default:
            abort();
        }
    crearNodos(0);
    return 0;
}

