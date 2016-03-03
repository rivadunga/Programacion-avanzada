

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

void control_sigusr(int signal)
{
    //loop = 0;
    printf("Has intentado cortar %d veces.\nHas intentado detener %d veces\n", numc, numz);
    kill(child, SIGKILL);
    printf("The son has been killed\n");
    raise(SIGKILL);
}

void control_alarm(int signal)
{
    printf("Aparezco cada %d segundos.\n", t);
    int res = alarm(t);
}

void control_ctrlc(int signal)
{
    t++;
    int res = alarm(t);
    printf("Pulso CTRL + C, te quedaban %d seg\n", res);
    ++numc;
}

void control_ctrlz(int signal)
{
    if (t > 1)
        t--;
    int res = alarm(t);
    printf("Pulso CTRL + Z, te quedaban %d seg\n", res);
    ++numz;
}

int main(int argc, const char * argv[])
{
    struct sigaction gest;
    int              err, status;
    pid_t            pid;

    if (pid == 0)
    {
        /* sleep 10 seconds */
        printf("I'm the child with pid %d and my father pid is %d\n", getpid(), getppid());
        child = getpid();
        sleep(10);
        /* send the signal */
        kill(getppid(), SIGUSR1);
        exit(0);
    }
    else if (pid > 0)
    {
        /* catching CTRL+C */
        gest.sa_handler = control_ctrlc;
        gest.sa_flags   = SA_RESTART;

        err = sigaction(SIGINT, &gest, 0);

        /* catching CTRL+Z */
        gest.sa_handler = control_ctrlz;
        gest.sa_flags   = SA_RESTART;

        err = sigaction(SIGTSTP, &gest, 0);

        /* catching alarm */
        gest.sa_handler = control_alarm;
        gest.sa_flags   = SA_RESTART;

        err = sigaction(SIGALRM, &gest, 0);

        /* start alarm*/
        alarm(t);

        /* catching sigusr*/
        gest.sa_handler = control_sigusr;
        gest.sa_flags   = SA_ONESHOT;

        err = sigaction(SIGUSR1, &gest, 0);
        if (waitpid(pid, &status, 0) != -1)
            wait(NULL);
    }

    return 0;
}
