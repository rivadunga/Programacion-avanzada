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
#define PATH    "Data"

int flag = 0;


void createDir()
{
    int status;
    DIR *dir = opendir(PATH);

    if (dir)
    {
        printf("Borrando directorio\n");
        eraseDir();
        closedir(dir);
    }
    else if (ENOENT == errno)
    {
        printf("Creando directorio\n");
        mkdir(PATH, 0777);
    }
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
                strcpy(path, "Data/");
                strcat(path, d->d_name);
                stat(path, &s);
                printf("Data/%s Size: %lld Mbyte's\n", d->d_name, (long long)(((s.st_size) / 1024) / 1024));
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
            printf("Borrando %s\n", d->d_name);
            sprintf(buf, "%s/%s", PATH, d->d_name);
            unlink(buf);
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

    signal(SIGALRM, SIG_DFL);
    alarm(2);
    signal(SIGALRM, handler_alarm);
    createDir();

    for (i = 0; i < numDir; i++)
    {
        printf("Creando archivos\n");
        int signal;
        flag = 1;
        file = fopen(buffer, "w+");
        sprintf(buffer, "./Data/a%d", i);
        alarm(3);
        while (flag)
            fputc('x', file);
        fclose(file);
    }
    printf("INFO\n\n");
    printDir();
    printf("\n");
    system("sl -la -F");
    return 0;
}
