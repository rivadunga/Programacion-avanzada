#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char * argv[])
{
    int  c;
    int  n;
    char *p = NULL;

    opterr = 0;

    while ((c = getopt(argc, argv, "n:p:")) != -1)
        switch (c)
        {
        case 'n':
            n = atoi(optarg);
            break;

        case 'p':
            p = optarg;
            break;

        default:
            abort();
        }

    int   i, j;

    pid_t pid;
    for (i = 0; i < n; ++i)
    {
        system(p);
    }
    wait(NULL);

    return 0;
}
