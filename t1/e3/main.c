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
    pacientes = (Paciente*)malloc(sizeof(Paciente));

    agregarPaciente("Yolo1");
    agregarPaciente("Yolo2");
    agregarPaciente("Yolo3");
    agregarPaciente("Yolo4");
    agregarPaciente("Yolo5");
    agregarPaciente("Yolo6");
    quitarPaciente("Yolo3");
    mostrarStatus();
    free(pacientes);
    free(camas);
    return 0;
}

void agregarPaciente(char* nombre)
{
    int i = 0;
    int pos = -1;

    for (i = 0; i < numCamas; i++)
    {
        if ((camas+i)->status == 1){
            pos = i;
            break;
        }
    }

    if (pos == -1){
        pos = numCamas;
        agregarCama();
    }

    pacientes = (Paciente*)realloc(pacientes, (numPacientes+1)*sizeof(Paciente));
    Paciente_construc(pacientes+numPacientes,nombre,"apellido",0,"telefono");
    (pacientes+numPacientes)->cama = (camas+pos);
    (camas+pos)->status = 0;
    numPacientes++;
}

void quitarPaciente(char* nombre){
    int i = 0;
    int pos = -1;
    for (i = 0; i < numPacientes; i++)
    {
        if (strcmp((pacientes+i)->nombre,nombre) == 0){
            pos = i;
            break;
        }
    }
    if (pos != -1)
        (pacientes+pos)->cama->status = 1;
}


void agregarCama()
{
    camas = (Cama*)realloc(camas, (numCamas+5)*sizeof(Cama));
        Cama_construc(camas+numCamas,numCamas);
    Cama_construc(camas+numCamas+1,numCamas+1);
    Cama_construc(camas+numCamas+2,numCamas+2);
    Cama_construc(camas+numCamas+3,numCamas+3);
    Cama_construc(camas+numCamas+4,numCamas+4);
    numCamas+=5;
}

void mostrarStatus(){
    int i = 0;
    int disponibles = 0;
    for (i = 0; i < numCamas; i++){
        printf("cama %i, status %s\n", i, (camas+i)->status == 1 ? "Disponible" : "No disponible");
        if ((camas+i)->status == 1) disponibles++;
    }

    printf("Disponibilidad %i de %i", disponibles,numCamas);
}
