#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <wait.h>
#include <unistd.h>

const int t = 3;
int       i = 0;

void handler_alarm(int signal)
{
    printf("Dese prisa. Dato X(%d): \n", i);
    alarm(t);
}

int main(int argc, char *argv[])
{
    struct sigaction gest;
    int              err;

    int              n     = atoi(argv[1]);
    int              *nums = (int*)malloc(sizeof(int) * n);

    gest.sa_handler = handler_alarm;
    gest.sa_flags   = SA_RESTART;
    err             = sigaction(SIGALRM, &gest, 0);

    signal(SIGINT, SIG_IGN);
    alarm(t);

    for (i = 0; i < n; ++i)
    {
        printf("Dato_X(%d)\n", i);
        scanf("%d", (nums + i));
    }

    for (i = 0; i < n; i++)
        printf("Numero %d: %d\n", i, *(nums + i));

    free(nums);
    return 0;
}
