#include <stdio.h>
#include <stdlib.h>
#include "Puerto.h"
#include "Barco.h"
#include "Persona.h"

typedef void (*opcion_t)();

Puerto *puerto;
void incorporarTripulantes(char*, char*);
void incorporarJefe(char*, char*);

int main()
{
    puerto = Puerto_new();
    Puerto_constr(puerto);

	//Agregar barcos
    Barco* barcos = (Barco *)malloc(4*sizeof(Barco));
    Barco_constr(barcos, "Barco1", "B2", "B3");
    Barco_constr(barcos+1, "Barco2", "B2", "B3");
    Barco_constr(barcos+2, "Barco3", "B2", "B3");
    Barco_constr(barcos+3, "Barco4", "B2", "B3");

	Puerto_addBarco(puerto, barcos);
    Puerto_addBarco(puerto, barcos+1);
    Puerto_addBarco(puerto, barcos+2);
    Puerto_addBarco(puerto, barcos+3);

    incorporarJefe("Barco1","Erick");
    incorporarJefe("Barco2","John");
    incorporarJefe("Barco3","Juan");
    incorporarJefe("Barco4","Javier");

    incorporarTripulantes("Barco1","Maria");
    incorporarTripulantes("Barco1","Jimmu");
    incorporarTripulantes("Barco2","Jiji");
    incorporarTripulantes("Barco3","Jdsa");

    verBarco("Barco1");
    verBarco("Barco2");
    verBarco("Barco3");
    verBarco("Barco4");

    verTodo();

	free(barcos);
}


void incorporarTripulantes(char* nombreBarco, char* nombrePersona)
{
    int  i;
    Barco *barcos = Puerto_getBarcos(puerto);
    for (i = 0; i < puerto->numBarcos; i++)
    {
        if (strcmp(nombreBarco, (barcos + i)->nombre) == 0) {
            Persona *persona = Persona_new();
            Persona_constr(persona, nombrePersona, "B2", "B3", "B4");
            Barco_addTripulante((barcos + i), persona);
        }
    }
}

void incorporarJefe(char* nombreBarco, char* nombreJefe)
{
    int  i;
    Barco *barcos = Puerto_getBarcos(puerto);
    for (i = 0; i < puerto->numBarcos; i++)
    {
        if (strcmp(nombreBarco, (barcos + i)->nombre) == 0) {
            Persona *persona = Persona_new();
            Persona_constr(persona, nombreJefe, "B2", "B3", "B4");
            Barco_addJefe((barcos + i), persona);
        }
    }
}


void verBarco(char* nombreBarco)
{
    int  i, j;

    Barco *barcos = Puerto_getBarcos(puerto);
    for (i = 0; i < puerto->numBarcos; i++)
    {
        if (strcmp(nombreBarco, (barcos + i)->nombre) == 0) {
            printf("* %s\n", (barcos + i)->nombre);
            printf("  Jefe: %s\n", (barcos + i)->jefe->nombre);
            Persona *personas = (barcos + i)->tripulantes;
            for (j = 0; j < (barcos + i)->numTripulacion; j++) {
                printf("   %s (%s)\n", (personas + j)->nombre, (personas + j)->rol);
            }
            printf("\n");
        }
    }
}


void verTodo()
{
    int   i, j;
    Barco *barcos = Puerto_getBarcos(puerto);
    printf("\n\n\n---- PUERTO ----\n\n");
    for (i = 0; i < puerto->numBarcos; i++)
    {
        printf("Barco %s \n", (barcos + i)->nombre);
        Persona *personas = (barcos + i)->tripulantes;
        printf("  %s (%s)\n", (barcos+i)->jefe->nombre, (barcos+i)->jefe->rol);
        for (j = 0; j < (barcos + i)->numTripulacion; j++) {
            printf("  %s (%s)\n", (personas + j)->nombre, (personas + j)->rol);
        }
        printf("  Plazas disponibles %i\n\n ", (barcos+i)->MAX_TRIPULACION - (barcos+i)->numTripulacion);
    }
}
