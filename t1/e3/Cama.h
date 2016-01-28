#pragma once

typedef struct Cama
{
    int   numero;
	int status;
} Cama;

Cama *Cama_new();
void Cama_construc(Cama *, int numero);
void Cama_destroy(Cama *);


/** Implementation */

Cama *Cama_new()
{
    Cama *cama = (Cama *)malloc(sizeof(Cama));
    return cama;
}

void Cama_setStatus(Cama * cama, int status){
	cama->status = status;
}


void Cama_construc(Cama * cama, int numero)
{
    cama->numero = numero;
}

void Cama_destroy(Cama * cama){

}
