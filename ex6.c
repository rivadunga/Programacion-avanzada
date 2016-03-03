#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>
#include <sys/types.h>

int t    = 3;
int numc = 0;
int numz = 0;
int child;


void handler_ctrlc(int signal)
{
    t    = t + 1;
    numc = numc + 1;
    printf("Pulso CTRL + C, te quedaban %d seg\n", alarm(t));
}

void handler_ctrlz(int signal)
{
    if (t > 1)
        t = t - 1;
    numz = numz + 1;
    printf("Pulso CTRL + Z, te quedaban %d seg\n", alarm(t));
}

void handler_alarm(int signal)
{
    printf("Aparezco cada %d segundos.\n", t);
}

void handler_sigusr(int signal)
{
    printf("Has intentado cortar %d veces.\nHas intentado detener %d veces\n", numc, numz);
    kill(child, SIGKILL);
    raise(SIGKILL);
}



int main(int argc, const char * argv[])
{
    struct sigaction gest;
    int              err, status;
    pid_t            pid;

    if (pid == 0)
    {
        printf("Hijo pid = %d; Padre ppid = %d\n", getpid(), getppid());
        child = getpid();
        sleep(10);
        kill(getppid(), SIGUSR1);
        exit(0);
    }
    else if (pid > 0)
    {
        gest.sa_handler = handler_ctrlc;
        gest.sa_flags   = SA_RESTART;
        err             = sigaction(SIGINT, &gest, 0);

        gest.sa_handler = handler_ctrlz;
        gest.sa_flags   = SA_RESTART;
        err             = sigaction(SIGTSTP, &gest, 0);

        gest.sa_handler = handler_alarm;
        gest.sa_flags   = SA_RESTART;
        err             = sigaction(SIGALRM, &gest, 0);

        alarm(t);
        gest.sa_handler = handler_sigusr;
        gest.sa_flags   = SA_ONESHOT;
        err             = sigaction(SIGUSR1, &gest, 0);

        if (waitpid(pid, &status, 0) != -1)
            wait(NULL);
    }

    return 0;
}
