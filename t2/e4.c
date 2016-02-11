#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int N, P;

void crearNodo(int level, int pos)
{
    pid_t pid;

    pid = fork();

    if (pos < P)
    {
        if (pid == -1)
        {
            fprintf(stderr, "can't fork, error %d\n", errno);
            exit(EXIT_FAILURE);
        }

        if (pid == 0)
        {
            fprintf(stderr, "Level is %d, i am %ld , my parent is %ld\n", level, (long)getpid(), (long)getppid());
            exit(0);
        }
        else
        {
            crearNodo(level, (pos + 1));
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

    int j = 0;

    for (; j < N; j++)
    {
        crearNodo(j + 1, 0);
    }
    return 0;
}
