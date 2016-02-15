#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void leer(int * fd);
void escribir(int * fd, char c);


int main(int argc, char * argv[])
{
    int c;
    int n;

    opterr = 0;

    while ((c = getopt(argc, argv, "n:")) != -1)
    {
        switch (c)
        {
        case 'n':
            n = atoi(optarg);
            break;
        }
    }


    int   tuberia[2];
    int   status;
    int   estado;
    pid_t pid;
    pipe(tuberia);

    int i;
    for (i = 0; i < n; ++i)
    {
        pid = fork();

        if (pid == -1)
        {
            printf("Error");
        }
        else if (pid == 0)
        {
            char c;
            int  bits;
            while (1)
            {
                close(tuberia[1]);
                read(tuberia[0], &c, sizeof(char));

                bits = read(tuberia[0], &c, sizeof(char));
                if (bits == sizeof(char))
                {
                    printf("Soy el proceso con PID <%d>  y recibí el testigo <%c>, el cual tendré por 5 segundos\n", getpid(), c);
                    sleep(1);
                    printf("Soy el proceso con PID <%d>  y acabo de enviar el testigo <%c>\n", getpid(), c);
                    close(tuberia[0]);
                    write(tuberia[1], &c, sizeof(char));
                    break;
                }
            }
            exit(0);
        }
        else
        {
            char c = 'A';
            close(tuberia[0]);
            write(tuberia[1], &c, sizeof(char));
        }
    }
    //Esperaa que acaben
    while ((pid = wait(&status)) > 0)
    {
    }


    return 0;
}
