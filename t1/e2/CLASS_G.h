#pragma once

typedef struct CLASS
{
    T1   N1;
    T2   N2;
    T3   N3;
    T4   N4;

    int NUM_ELE;
    int numEle;
    TA1* A1;

} CLASS;

CLASS *CLASS_new();
void CLASS_construc(CLASS *, T1 N1, T2 N2, T3 N3, T4 N4);
void CLASS_destroy(CLASS *);


/** Implementation */

CLASS *CLASS_new()
{
    CLASS *OBJ = (CLASS *)malloc(sizeof(CLASS));
    return OBJ;
}


void CLASS_construc(CLASS * OBJ, T1 N1, T2 N2, T3 N3, T4 N4)
{
    OBJ->N1 = N1;
    OBJ->N2 = N2;
    OBJ->N3 = N3;
    OBJ->N4 = N4;

    OBJ->NUM_ELE = ?;
    OBJ->numEle = 0;
    OBJ->A1     = (TA1 *)malloc(OBJ->NUM_ELE * sizeof(TA1));
}

void CLASS_destroy(CLASS * OBJ){

}
