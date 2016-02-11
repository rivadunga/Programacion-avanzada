#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int main(int argc, char *argv[])
{
    int   num;
    pid_t pid;
    pid_t pid2;
    int   status;

    scanf("%d", &num);

    int i;
    for (i = 0; i < num; ++i)
    {
        int status;
        pid2 = fork();

        if (pid2 == -1)
        {
            printf("Error");
        }
        else if (pid2 == 0)
        {
            srand((int)time(NULL) + getpid());
            int tiempo = rand() % 20;
            sleep(tiempo);
            exit(0);
        }
    }

    while ((pid = wait(&status)) > 0)
    {
        printf("pid: %d\n", pid);
    }

    return 0;
}
