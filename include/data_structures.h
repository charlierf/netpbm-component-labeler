#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

// Estrutura para uma célula da fila ou pilha
typedef struct celula {
    int valor;
    struct celula *prox;
} celula;

// Estrutura para uma fila
typedef struct fila {
    celula *inicio;
    celula *fim;
    int cont;
} fila;

// Estrutura para uma pilha
typedef struct pilha {
    celula *topo;
} pilha;

// Funções para manipulação de filas
fila* criaFila();
void enfileira(fila *f, int valor);
int desenfileira(fila *f);

// Funções para manipulação de pilhas
pilha* criaPilha();
void push(pilha *p, int valor);
int pop(pilha *p);

#endif /* DATA_STRUCTURES_H */