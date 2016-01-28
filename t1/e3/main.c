#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Cama.h"
#include "Paciente.h"

int numPacientes;
int numCamas;
Cama* camas;
Paciente* pacientes;

void agregarPaciente(char* nombre);
void quitarPaciente(char* nombre);
void agregarCamas();
void mostrarStatus();

int main()
{
    numPacientes = 0;
    numCamas = 0;
    camas = (Cama*)malloc(sizeof(Cama));
    agregarPaciente("Yolo");

    return 0;
}

void agregarPaciente(char* nombre)
{
    Cama* cama;
    int i = 0;
    int pos = -1;

    for (i = 0; i < numCamas; i++)
    {
        if ((cama+i)->status)
            pos = i;
    }

    if (pos == -1){
        agregarCama();
        pos = numCamas+1;
    }

    numPacientes++;
    pacientes = (Paciente*)realloc(pacientes, numPacientes*sizeof(Paciente));

}

void quitarPaciente(char* nombre){

}


void agregarCama()
{
    numCamas+=5;
    camas = (Cama*)realloc(camas, numCamas*sizeof(Cama));
    Cama_construc(camas+numCamas-4,numCamas-4);
    Cama_construc(camas+numCamas-3,numCamas-3);
    Cama_construc(camas+numCamas-2,numCamas-2);
    Cama_construc(camas+numCamas-1,numCamas-1);
    Cama_construc(camas+numCamas,numCamas);
}

void mostrarStatus(){
    int i = 0;
    for (i = 0; i < numCamas; i++)
        printf("cama %i, status %s\n", i, (camas+i)->status == 1 ? "Disponible" : "No disponible");

}
