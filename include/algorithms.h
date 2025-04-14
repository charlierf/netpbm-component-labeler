#ifndef ALGORITHMS_H
#define ALGORITHMS_H

// Funções de rotulação de componentes conexos
int RotularImagem(int largura, int altura, char* entrada, int* saida);
void RotularComponente(int largura, int altura, char* entrada, int* saida, int numeroRotulo, int x, int y);

// Funções para detecção de furos
int PreencheFuros(int largura, int altura, int* saida, int rotulo, int x, int y);
int PreencheFundo(int largura, int altura, int* saida, int x, int y);
int ContaFuros(int largura, int altura, int* rotulos, int* furos);

#endif /* ALGORITHMS_H */