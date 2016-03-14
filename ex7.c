#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#define PATH    "datos"

int flag = 0;

void createDir()
{
    int status;
    DIR *dir = opendir(PATH);

    if (dir)
    {
        eraseDir();
        closedir(dir);
    }
    else if (ENOENT == errno)
        mkdir(PATH, 0777);
}



void printDir()
{
    DIR           *dir = opendir(PATH);
    struct dirent *d;
    struct stat   s;
    char          *path;

    if (dir)
    {
        while (d = readdir(dir))
        {
            if (d->d_type == DT_REG)
            {
                path = (char*)malloc((strlen(path) + strlen(d->d_name) + 1) * sizeof(char));
                strcpy(path, "datos/");
                strcat(path, d->d_name);
                stat(path, &s);
                printf("Nombre\n", d->d_name);
            }
        }
        closedir(dir);
    }
}


void eraseDir()
{
    struct dirent *d;
    DIR           *dir;
    char          buf[256];

    dir = opendir(PATH);

    while (d = readdir(dir))
    {
        if (d->d_type == DT_REG)
        {
            unlink(buf);
            sprintf(buf, "%s/%s", PATH, d->d_name);
            remove(buf);
        }
    }
}


void handler_alarm(int signal)
{
    flag = 0;
}

int main(int argc, char *argv[])
{
    int  i      = 0;
    int  numDir = 5;
    FILE *file  = NULL;
    char *buffer[20];

    sigset_t bloqueados;

    sigfillset(&bloqueados);
    sigdelset(&bloqueados, SIGALRM);
    sigprocmask(SIG_SETMASK, &bloqueados, NULL);

    signal(SIGALRM, SIG_DFL);
    alarm(2);
    signal(SIGALRM, handler_alarm);
    createDir();

    sigset_t pendientes;
    for (i = 0; i < numDir; i++)
    {
        printf("Creando archivos\n");
        int signal;
        flag = 1;
        file = fopen(buffer, "w+");
        sprintf(buffer, "./datos/a%d", i);
        alarm(3);
        int numX = 0;
        while (flag && numX < 100){
            fputc('x', file);
            numX++;
        }
        fprintf(file, "Bloqueado: %s\n",sigpending(&pendientes));
        fclose(file);
    }
    printf("INFO\n\n");
    //printDir();
    printf("\n");
    return 0;
}
