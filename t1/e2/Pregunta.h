#pragma once

typedef struct Pregunta
{
    char *texto;
    int  NUM_ELE;
    char **resTxt;
} Pregunta;

Pregunta *Pregunta_new();
void Pregunta_construc(Pregunta *, char *texto);
void Pregunta_destroy(Pregunta *);


/** Implementation */

Pregunta *Pregunta_new()
{
    Pregunta *pregunta = (Pregunta *)malloc(sizeof(Pregunta));
    return pregunta;
}


void Pregunta_construc(Pregunta *pregunta, char *texto)
{
    pregunta->texto   = texto;
    pregunta->NUM_ELE = 6;
    pregunta->resTxt  = (char *)malloc(pregunta->NUM_ELE * sizeof(char));
}


void Pregunta_setRespuestaTxt(Pregunta *pregunta, int pos, char *txt)
{
    *(pregunta->(resTxt+pos)) = txt;
}


void Pregunta_destroy(Pregunta *pregunta)
{
    free(pregunta);
}
