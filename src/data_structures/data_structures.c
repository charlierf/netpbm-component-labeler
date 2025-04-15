#include <stdio.h>
#include <stdlib.h>
#include "data_structures.h"

// Fixed warnings and ensured proper return types

fila* criaFila()
{
    fila *novaFila = malloc(sizeof(fila));
    if (novaFila == NULL)
    {
        printf("Erro ao alocar memória para a fila\n");
        exit(1);
    }
    novaFila->inicio = NULL;
    novaFila->fim = NULL;
    novaFila->cont = 0;
    return novaFila;
}

void enfileira(fila *f, int valor)
{
    celula *novaCelula = malloc(sizeof(celula));
    if (novaCelula == NULL)
    {
        printf("Erro ao alocar memória para a célula\n");
        exit(1);
    }
    novaCelula->valor = valor;
    novaCelula->prox = NULL;

    if (f->fim == NULL)
    {
        f->inicio = novaCelula;
    }
    else
    {
        f->fim->prox = novaCelula;
    }
    f->fim = novaCelula;
    f->cont++;
}

int desenfileira(fila *f)
{
    if (f->inicio == NULL)
    {
        printf("Fila vazia\n");
        return -1;
    }

    celula *aux = f->inicio;
    int valor = aux->valor;
    f->inicio = aux->prox;

    if (f->inicio == NULL)
    {
        f->fim = NULL;
    }

    free(aux);
    f->cont--;
    return valor;
}

int pop(pilha *p)
{
    if (p->topo == NULL)
    {
        printf("Pilha vazia\n");
        return -1;
    }

    celula *aux = p->topo;
    int valor = aux->valor;
    p->topo = aux->prox;
    free(aux);
    return valor;
}
