#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/wait.h>
#include <time.h>

#define N    1000000

int main(int argc, const char *argv[])
{
    pid_t pid;
    int   estado;
    int   i;
    int   n;

    scanf("%i", &n);

    for (i = 0; i < n; i++)
    {
        pid = fork();

        if (pid == -1)
        {
            printf("Error: Solo se pudieron crear %i procesos", i);
            return 0;
        }
        else if (pid == 0)
        {
           int promedio = (int)(getpid()+getppid())/2;
           exit(promedio);
        }
        else
        {
            if (waitpid(pid, &estado, 0) != -1)
            {
                if (WIFEXITED(estado))
                {
                   printf("Promedio %i\n", WEXITSTATUS(estado));
                }
            }
        }
    }
}
