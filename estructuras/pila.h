#pragma once
#include "nodo.h"

typedef struct {
    Nodo *tope;
} Pila;

void pila_init(Pila *p);
int  pila_vacia(Pila *p);
int  pila_push(Pila *p, void *valor);
void* pila_pop(Pila *p);
void pila_free(Pila *p);