#pragma once

typedef struct Paciente
{
    char*  nombre;
    char*   apellido;
    int   edad;
    char*   telefono;
	Cama* cama;

} Paciente;

Paciente *Paciente_new();
void Paciente_construc(Paciente *, char*nombre, char* apellido, int edad, char* telefono);
void Paciente_destroy(Paciente *);


/** Implementation */

Paciente *Paciente_new()
{
    Paciente *paciente = (Paciente *)malloc(sizeof(Paciente));
    return paciente;
}


void Paciente_construc(Paciente * paciente, char*nombre, char* apellido, int edad, char* telefono)
{
    paciente->nombre = nombre;
    paciente->apellido = apellido;
    paciente->edad = edad;
    paciente->telefono = telefono;
	paciente->cama     = (Cama *)malloc(sizeof(Cama));
}

void Paciente_destroy(Paciente * paciente){

}
