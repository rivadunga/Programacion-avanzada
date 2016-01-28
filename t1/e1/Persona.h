#pragma once

typedef struct Persona
{
    char *nombre;
    char *apellidos;
    char *edad;
    char *rol;
} Persona;
Persona *Persona_new();
void Persona_constr(Persona *, char *, char *, char *, char *);
void Persona_destroy(Persona *);


/** Implementation */

Persona *Persona_new()
{
    Persona *persona = (Persona *)malloc(sizeof(Persona));
    return persona;
}


void Persona_constr(Persona *persona, char *nombre, char *apellidos, char *edad, char *rol)
{
    persona->nombre    = nombre;
    persona->apellidos = apellidos;
    persona->edad      = edad;
    persona->rol       = rol;
}


void Persona_destroy(Persona *persona)
{
    free(persona);
}
