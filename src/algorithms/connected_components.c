#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/algorithms.h"
#include "../../include/image_formats.h"

// Estrutura da pilha para processamento não recursivo
typedef struct reg
{
    int x;
    int y;
    struct reg *prox;
} celula;

void push(int x, int y, celula *p)
{
    celula *nova;
    nova = malloc(sizeof(celula));
    if (nova == NULL)
    {
        printf("Erro de alocação de memória\n");
        exit(1);
    }
    nova->x = x;
    nova->y = y;
    nova->prox = p->prox;
    p->prox = nova;
}

celula* pop(celula *p)
{
    celula* pop = p->prox;
    p->prox = pop->prox;
    return pop;
}

celula* criaPilha()
{
    celula *pi = malloc(sizeof(celula));
    if (pi == NULL)
    {
        printf("Erro de alocação de memória\n");
        exit(1);
    }
    pi->x = -1;
    pi->y = -1;
    pi->prox = NULL;

    return pi;
}

// Função que retorna o total de componentes conexos (objetos) da imagem
int RotularImagem(int largura, int altura, char* entrada, int* saida)
{
    int numeroRotulos = 0;
    int index = -1;
    // Rotula os componentes
    printf("Contando os objetos...\n");
    for (int y = 0; y < altura; y++)
    {
        for (int x = 0; x < largura; x++)
        {
            index++;
            if (entrada[index] == '0') continue;   // Este pixel não faz parte de um componente
            if (saida[index] != 0) continue;       // Este pixel já foi rotulado
            // Novo componente encontrado
            numeroRotulos++;

            RotularComponente(largura, altura, entrada, saida, numeroRotulos, x, y);
        }
    }

    return numeroRotulos;
}

// Função recursiva que atribui um valor no vetor saida a todos os pixels de um componente conexo
void RotularComponente(int largura, int altura, char* entrada, int* saida, int numeroRotulo, int x, int y)
{
    int index = x + largura*y;
    if (entrada[index] == '0') return;   // pixel não faz parte do componente
    if (saida[index] != 0) return;       // pixel já foi rotulado
    saida[index] = numeroRotulo;

    // Agora preenche os 8 vizinhos:
    if (x > 0)                          RotularComponente(largura, altura, entrada, saida, numeroRotulo, x-1, y);     // pixel à esquerda
    if (x > 0 && y > 0)                 RotularComponente(largura, altura, entrada, saida, numeroRotulo, x-1, y-1);   // pixel superior esquerdo
    if (y > 0)                          RotularComponente(largura, altura, entrada, saida, numeroRotulo, x, y-1);     // pixel superior
    if (x < largura-1 && y > 0)         RotularComponente(largura, altura, entrada, saida, numeroRotulo, x+1, y-1);   // pixel superior direito
    if (x < largura-1)                  RotularComponente(largura, altura, entrada, saida, numeroRotulo, x+1, y);     // pixel à direita
    if (x < largura-1 && y < altura-1)  RotularComponente(largura, altura, entrada, saida, numeroRotulo, x+1, y+1);   // pixel inferior direito
    if (y < altura-1)                   RotularComponente(largura, altura, entrada, saida, numeroRotulo, x, y+1);     // pixel inferior
    if (x > 0 && y < altura-1)          RotularComponente(largura, altura, entrada, saida, numeroRotulo, x-1, y+1);   // pixel inferior esquerdo
}

// Função recursiva que atribui um valor negativo no vetor saida a todos os furos da imagem. Idêntica à RotularComponente, mas com vizinhança 4
int PreencheFuros(int largura, int altura, int* saida, int rotulo, int x, int y)
{
    int index = x + largura * y;
    if (saida[index] != 0) return 0; // Este pixel não faz parte de um furo
    saida[index] = rotulo;

    // Agora preenche os 4 vizinhos:
    if (x > 0)        PreencheFuros(largura, altura, saida, rotulo, x-1, y);   // pixel esquerdo
    if (y > 0)        PreencheFuros(largura, altura, saida, rotulo, x, y-1);   // pixel superior
    if (x < largura-1) PreencheFuros(largura, altura, saida, rotulo, x+1, y);   // pixel direito
    if (y < altura-1)  PreencheFuros(largura, altura, saida, rotulo, x, y+1);   // pixel inferior

    return 1;
}

// Função iterativa com pilha para preencher o fundo da imagem
// Não foi usada a abordagem recursiva pois em imagens grandes ocorria o estouro de pilha
int PreencheFundo(int largura, int altura, int* saida, int x, int y)
{
    int index = x + largura*y;
    if (saida[index] != 0) return 0; // Este pixel não faz parte do fundo
    celula *pilha = criaPilha(); // Cria uma pilha vazia
    push(x, y, pilha); // Insere as coordenadas iniciais na pilha

    while (pilha->prox != NULL)   // Enquanto a pilha não estiver vazia
    {
        celula *aux = pop(pilha); // Remove o elemento do topo da pilha
        x = aux->x; // Atualiza as coordenadas
        y = aux->y;
        free(aux);
        index = x + largura*y; // Atualiza o índice

        if (saida[index] == 0)   // Se o pixel for parte do fundo
        {
            saida[index] = -1; // Marca com um valor negativo
            // Agora verifica os 4 vizinhos e insere na pilha se forem válidos:
            if (x > 0 && saida[index-1] == 0) push(x-1, y, pilha); // pixel esquerdo
            if (y > 0 && saida[index-largura] == 0) push(x, y-1, pilha); // pixel superior
            if (x < largura-1 && saida[index+1] == 0) push(x+1, y, pilha); // pixel direito
            if (y < altura-1 && saida[index+largura] == 0) push(x, y+1, pilha); // pixel inferior
        }
    }

    free(pilha); // Libera a memória da pilha
    return 0;
}

// Função que retorna o total de furos da imagem
int ContaFuros(int largura, int altura, int* rotulos, int* furos)
{
    int contagemFuros = 0;
    int rotuloFuro = 0;

    printf("Contando os furos...\n");
    for (int y = 0; y < altura; y++)
    {
        for (int x = 0; x < largura; x++)
        {
            int index = x + largura*y;

            // Rotula o fundo com -1
            if (rotulos[index] == 0 && rotuloFuro == 0)
            {
                contagemFuros += PreencheFundo(largura, altura, rotulos, x, y);
                rotuloFuro--; // Usa um rótulo negativo para os furos
            }

            // Rotula os Furos
            if (rotulos[index] == 0 && rotuloFuro != 0) // Encontrou um furo
            {
                // Obtém o rótulo do objeto que contém o furo
                int rotuloObjeto = rotulos[index-1];
                // Marca o valor correspondente no vetor furos como um
                furos[rotuloObjeto-1] = 1;
                contagemFuros += PreencheFuros(largura, altura, rotulos, rotuloFuro, x, y);
                rotuloFuro--; // Usa um rótulo negativo para os furos
            }
        }
    }

    return contagemFuros;
}