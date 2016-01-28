#pragma once

typedef struct Persona
{
    char *nombre;
    int  edad;
    int  NUM_ELE;
    int  *res;
} Persona;

Persona *Persona_new();
void Persona_construc(Persona *, char *nombre, int edad);
void Persona_setRespuesta(Persona *, int, int);
void Persona_destroy(Persona *);


/** Implementation */

Persona *Persona_new()
{
    Persona *persona = (Persona *)malloc(sizeof(Persona));
    return persona;
}


void Persona_construc(Persona *persona, char *nombre, int edad)
{
    persona->nombre  = nombre;
    persona->edad    = edad;
    persona->NUM_ELE = 6;
    persona->res     = (int *)malloc(persona->NUM_ELE * sizeof(int));
}


void Persona_setRespuesta(Persona *persona, int preg, int res)
{
    *(persona->res + preg) = res;
}


void Persona_destroy(Persona *persona)
{
    free(persona);
}
