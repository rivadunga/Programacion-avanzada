#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Persona.h"
#include "Pregunta.h"


Persona* personas;
Pregunta* preguntas;
void histogramaFrecuencia();
void histogramaEdad();

int main()
{
    int NUM_PERS = 10;
    int NUM_PREG = 10;
    int NUM_RESP = 6;

    int i,j,k;

    personas = (Persona*)malloc(NUM_PERS*sizeof(Persona));
    Persona_construc(personas,"Erick",60);
    Persona_construc(personas+1,"Ale",20);
    Persona_construc(personas+2,"Juan",18);
    Persona_construc(personas+3,"Andres",38);
    Persona_construc(personas+4,"Vero",110);
    Persona_construc(personas+5,"Jaime",42);
    Persona_construc(personas+6,"Ruben",21);
    Persona_construc(personas+7,"Carolina",76);
    Persona_construc(personas+8,"Mike",35);
    Persona_construc(personas+9,"John",50);

    preguntas = (Pregunta*)malloc(NUM_PREG*sizeof(Pregunta));

    //Inicializar preguntas
    for (j = 0; j < NUM_PREG; j++){
        char* nombre = (char*)malloc(20*sizeof(char));
        sprintf(nombre,"Pregunta %i", (j+1));
        Pregunta_construc(preguntas+j,nombre);
        for(i = 0; i < NUM_RESP; i++){
            char nombre2[20];
            sprintf(nombre2,"Respuesta %i de %i", &i,&j);
            Pregunta_setRespuestaTxt(preguntas+j,i,nombre2);
        }
    }

    //Respuestas random
    for (j = 0; j < NUM_PERS; j++){
        for (i = 0; i < NUM_PREG; i++){
            srand (time(NULL)+i+j);
            Persona_setRespuesta(personas+j,i,rand()%5);
        }
    }

    //Imprimir histograma
    int temp;
    for (k = 0; k < NUM_PREG; k++){
        printf("Pregunta %i \n", (k+1));
        for (j = 0; j < NUM_RESP; j++){
            printf("  %i  ", (j+1));
            temp = 0;
            for (i = 0; i < NUM_PERS; i++){
                int* res = (personas+i)->res;
                if (*(res+k) == j){
                    temp++;
                }
            }
            printf("(%i) ", temp);

            for (i = 0; i < temp; i++)
                printf("*");
            printf("\n");
        }
    }


     //Imprimir histograma edades
    int r1,r2,r3,r4,r5;
    for (k = 0; k < NUM_PREG; k++){
        printf("Pregunta %i \n", (k+1));
        for (j = 0; j < NUM_RESP; j++){
            printf("  %i  ", (j+1));

            r1 = r2 = r3 = r4 = r5 = 0;
            temp = 0;
            for (i = 0; i < NUM_PERS; i++){
                int* res = (personas+i)->res;
                if (*(res+k) == j){
                    temp++;
                    int edad = (personas+i)->edad;
                    if (edad >= 18 && edad <= 25) r1++;
                    if (edad >= 26 && edad <= 35) r2++;
                    if (edad >= 36 && edad <= 45) r3++;
                    if (edad >= 46 && edad <= 65) r4++;
                    if (edad >= 65 && edad <= 120) r5++;

                }
            }
            printf("(%i)\n",temp);
            printf("    18 - 25 ");
            for (i = 0; i < r1; i++) printf("*");
            printf("\n");
            printf("    26 - 35 ");
            for (i = 0; i < r2; i++) printf("*");
            printf("\n");
            printf("    36 - 45 ");
            for (i = 0; i < r3; i++) printf("*");
            printf("\n");
            printf("    46 - 65 ");
            for (i = 0; i < r4; i++) printf("*");
            printf("\n");
            printf("    65 - 120 ");
            for (i = 0; i < r5; i++) printf("*");
            printf("\n");
        }
    }


    return 0;
}
