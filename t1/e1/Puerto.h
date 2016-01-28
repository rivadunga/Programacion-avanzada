#pragma once
#include "Barco.h"

typedef struct Puerto
{
    int   MAX_BARCOS;
    int   numBarcos;
    Barco *barcos;
} Puerto;

Puerto *Puerto_new();
void Puerto_constr(Puerto *);
void Puerto_addBarco(Puerto *, Barco *);
int Puerto_getNumBarcos(Puerto *);
Barco *Puerto_getBarcos(Puerto *);
void Puerto_destroy(Puerto *);


/** Implementation */

Puerto *Puerto_new()
{
    Puerto *puerto = (Puerto *)malloc(sizeof(Puerto));
    return puerto;
}


void Puerto_constr(Puerto *puerto)
{
    puerto->MAX_BARCOS = 10;
    puerto->numBarcos  = 0;
    puerto->barcos     = (Barco *)malloc(puerto->MAX_BARCOS * sizeof(Barco));
}


void Puerto_addBarco(Puerto *puerto, Barco *barco)
{
    Barco *barcos = puerto->barcos;
    *(barcos + puerto->numBarcos) = *barco;
    puerto->numBarcos++;
    if (puerto->numBarcos > puerto->MAX_BARCOS){
        puerto->MAX_BARCOS++;
        puerto->barcos = (Barco*)realloc(puerto->barcos, puerto->MAX_BARCOS * sizeof(Barco));
    }

}


Barco *Puerto_getBarcos(Puerto *puerto)
{
    return puerto->barcos;
}


void Puerto_destroy(Puerto *puerto)
{
    free(puerto);
}
