#pragma once
#include "Persona.h"

typedef struct Barco
{
    int     MAX_TRIPULACION;
    int     numTripulacion;
    char    *nombre;
    char    *eslora;
    char    *manga;
    int     maxT;
    Persona *jefe;
    Persona *tripulantes;
} Barco;
Barco *Barco_new();
void Barco_constr(Barco *, char *, char *, char *);
void Barco_addTripulante(Barco *, Persona *);
void Barco_addJefe(Barco *, Persona *);
Persona *Barco_getTripulantes(Barco *);
Persona *Barco_getJefe(Barco *);
void Barco_destroy(Barco *);


/** Implementation */

Barco *Barco_new()
{
    Barco *barco = (Barco *)malloc(sizeof(Barco));
    return barco;
}


void Barco_constr(Barco *barco, char *nombre, char *eslora, char *manga)
{
    barco->MAX_TRIPULACION = 10;
    barco->numTripulacion  = 0;
    barco->nombre          = nombre;
    barco->eslora          = eslora;
    barco->manga           = manga;
    barco->jefe            = (Persona *)malloc(sizeof(Persona));
    barco->tripulantes     = (Persona *)malloc(barco->MAX_TRIPULACION * sizeof(Persona));
}


void Barco_addTripulante(Barco *barco, Persona *persona)
{
    Persona *personas = barco->tripulantes;
    *(personas + barco->numTripulacion) = *persona;
    barco->numTripulacion++;
    if (barco->numTripulacion > barco->MAX_TRIPULACION){
        barco->MAX_TRIPULACION++;
        barco->tripulantes = (Barco*)realloc(barco->tripulantes, barco->MAX_TRIPULACION * sizeof(Persona));
    }
}


void Barco_addJefe(Barco *barco, Persona *jefe)
{
    barco->jefe = jefe;
}


Persona *Barco_getTripulantes(Barco *barco)
{
    return barco->tripulantes;
}


Persona *Barco_getJefe(Barco *barco)
{
    return barco->jefe;
}


void Barco_destroy(Barco *barco)
{
}
