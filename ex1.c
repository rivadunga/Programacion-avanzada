#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char * argv[])
{
    char * cadena;
    int* freq;
    int longitud;

    printf("L: ");
    scanf("%d", &longitud);

    cadena = (char *) malloc(sizeof(char) * longitud);
    freq = (int *) malloc(sizeof(int)*30);
    int w = 0;
    for (w = 0; w < 28; w++)
        *(freq+w) = 0;



    printf("Cadena: ");
    scanf("%s", cadena);

    char * p = cadena;
    while (*p++ != '\0')
    {
        char val = *(p-1);
        if (val >= 'a' && val <= 'z')
            (*(freq + val - 'a'))++;

        if (val >= 'A' && val <= 'Z')
            (*(freq + val - 'A'))++;
    };

    int i;
    for (i = 0; i < 28; i++)
    {
        int max = 0;
        int pos = 0;
        int j;

        for (j = 0; j < 28; j++)
        {
            if (*(freq+j) > max){
                pos = j;
                max = *(freq+j);
            }
        }


        if (max > 0)
        {
            printf("%c ", (pos+'a'));
            printf("%i ", max);
            int z;
            for (z = 0; z < max; z++)
                printf("%c", '*');
            printf("\n");
            *(freq+pos) = 0;
        }


    }

    free(cadena);
    free(freq);

    return 0;
}
