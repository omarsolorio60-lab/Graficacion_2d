#include <stdlib.h>
#include "pila.h"

void pila_init(Pila *p) {
    p->tope = NULL;
}

int pila_vacia(Pila *p) {
    return p->tope == NULL;
}

int pila_push(Pila *p, void *valor) {
    Nodo *n = (Nodo*)malloc(sizeof(Nodo));
    if (!n) return 0;
    n->dato = valor;
    n->sig  = p->tope;
    p->tope = n;
    return 1;
}

void* pila_pop(Pila *p) {
    if (pila_vacia(p)) return NULL;
    Nodo *n = p->tope;
    void *dato = n->dato;
    p->tope = n->sig;
    free(n);
    return dato;
}

void pila_free(Pila *p) {
    while (!pila_vacia(p)) {
        Nodo *n = p->tope;
        p->tope = n->sig;
        free(n);
    }
}
