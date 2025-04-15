# Rotulador de Componentes NetPBM

Uma ferramenta para rotular e analisar componentes conexos em imagens NetPBM (PBM, PGM, PPM).

## Funcionalidades

- Lê imagens binárias PBM (P1)
- Rotula todos os componentes conexos em uma imagem
- Detecta e conta os furos nos componentes
- Gera uma imagem PGM com diferentes níveis de cinza para cada componente e furo
- Fornece estatísticas sobre o número de objetos, furos e objetos com furos
- Suporte abrangente para formatos NetPBM através de manipuladores especializados:
  - Suporte a PBM para entrada e processamento de imagens binárias
  - Suporte a PGM para saída em escala de cinza com componentes rotulados
  - Suporte a PPM para visualizações coloridas e análise de componentes

## Requisitos

- Compilador C (GCC recomendado)
- Make

## Compilando o Projeto

Clone o repositório e compile o projeto:

```bash
git clone https://github.com/seu-usuario/netpbm-component-labeler.git
cd netpbm-component-labeler
make
```

## Uso

```bash
./bin/netpbm-component-labeler imagem_entrada.pbm
```

O programa irá gerar um arquivo chamado `saida.ppm` no diretório atual, mostrando os componentes rotulados.

## Exemplo

```bash
./bin/netpbm-component-labeler examples/input/teste.pbm
```

## Estrutura do Projeto

```
netpbm-component-labeler/
├── src/
│  ├── main.c                    # Código do programa principal
│  ├── image_formats/
│  │  ├── pbm_handler.c          # Funções para manipulação de imagens PBM
│  │  ├── pgm_handler.c          # Funções para manipulação de imagens PGM
│  │  └── ppm_handler.c          # Funções para manipulação de imagens PPM
│  ├── algorithms/
│  │  └── connected_components.c # Algoritmos de rotulação e análise de componentes
│  └── data_structures/
│     └── data_structures.c      # Estruturas de dados para suporte aos algoritmos
├── include/
│  ├── image_formats.h           # Cabeçalho para funções de formatos de imagem
│  ├── algorithms.h              # Cabeçalho para algoritmos
│  └── data_structures.h         # Cabeçalho para estruturas de dados
├── examples/                    # Imagens de exemplo
│  ├── input/
│  └── output/
├── Makefile                     # Configuração de compilação
├── README.md                    # Descrição do projeto em inglês
└── README.pt-BR.md              # Este arquivo (português)
```

## Manipuladores de Formatos de Imagem

### Manipulador PBM
O manipulador PBM (Portable Bitmap) processa imagens binárias:
- Lê e analisa imagens binárias no formato P1 (formato ASCII)
- Converte imagens binárias para representação interna para processamento
- Fornece funções principais para o algoritmo de rotulação de componentes
- Implementa operações morfológicas (dilatação, erosão) para pré-processamento de imagens
- Suporta rotulação recursiva de componentes para identificar regiões conexas

### Manipulador PGM
O manipulador PGM (Portable Graymap) fornece suporte a imagens em escala de cinza:
- Cria imagens de saída com rótulos numéricos para cada componente
- Mapeia rótulos de componentes para níveis de cinza distintos para visualização
- Processa imagens de entrada em escala de cinza com capacidades de limiarização
- Implementa funções de equalização e análise de histograma
- Gerencia a geração de saída com componentes rotulados

### Manipulador PPM
O manipulador PPM (Portable Pixmap) permite o processamento de imagens coloridas:
- Suporta o modelo de cores RGB para visualizações avançadas
- Fornece codificação de cores para componentes e furos para distinção mais clara
- Implementa operações específicas por canal (histograma RGB, equalização)
- Gerencia ajustes de brilho e operações de limiar em imagens coloridas
- Permite análise mais detalhada de componentes através da diferenciação de cores

## Usando os Manipuladores de Formatos de Imagem

### Funções do Manipulador PBM

```c
// Inicializa uma nova estrutura de imagem PBM
imagem* inicializaImagem(imagem *im, int altura, int largura);

// Carrega uma imagem PBM de um arquivo
imagem* carregaImagem(FILE* file, char* tipo, char* head);

// Cria um negativo de uma imagem PBM
void negativo(imagem* im);

// Aplica operação morfológica de dilatação
imagem* dilata(imagem *im);

// Aplica operação morfológica de erosão
imagem* erode(imagem *im);
```

### Funções do Manipulador PGM

```c
// Aplica limiar a uma imagem PGM
void limiar(int limiar, imagem* im);

// Gera histograma para uma imagem PGM
void histograma(imagem* im, unsigned int histo[]);

// Aplica equalização de histograma a uma imagem PGM
void equalizar(imagem* im, unsigned int histo[]);
```

### Funções do Manipulador PPM

```c
// Inicializa uma nova estrutura de imagem PPM
imagemPPM* inicializaImagemPPM(imagemPPM *im, int altura, int largura);

// Carrega uma imagem PPM de um arquivo
imagemPPM* getImage(FILE* file, char* tipo, char* head);

// Cria um negativo de uma imagem PPM
void negativoPPM(imagemPPM* im);

// Aplica limiar a cada canal RGB
void limiarPPM(int limiarR, int limiarG, int limiarB, imagemPPM* im);

// Gera histograma para uma imagem PPM (média dos canais)
void histogramaPPM(imagemPPM* im, unsigned int histo[]);

// Gera histogramas separados para cada canal RGB
void histogramaRGB(imagemPPM* im, unsigned int histoR[], unsigned int histoG[], unsigned int histoB[]);

// Aplica equalização de histograma a uma imagem PPM
void equalizarPPM(imagemPPM* im, unsigned int histo[]);

// Aplica equalização de histograma separadamente a cada canal RGB
void equalizarRGB(imagemPPM* im, unsigned int histoR[], unsigned int histoG[], unsigned int histoB[]);

// Ajusta o brilho de uma imagem PPM
void brilho(imagemPPM* im, int valor);
```

### Funções de Rotulação de Componentes

```c
// Rotula todos os componentes em uma imagem
int LabelImage(int width, int height, char* input, int* output);

// Rotula um único componente conexo
void LabelComponent(int width, int height, char* input, int* output, int labelNo, int x, int y);
```

## Exemplo de Código para Usar os Manipuladores

Aqui está um exemplo de como usar o manipulador PBM para carregar uma imagem, aplicar operações morfológicas e processá-la:

```c
#include <stdio.h>
#include <stdlib.h>
#include "image_formats.h"

int main() {
    FILE* file = fopen("input.pbm", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo\n");
        return 1;
    }
    
    char tipo[3];
    char head[40];
    
    // Carrega uma imagem PBM
    imagem* img = carregaImagem(file, tipo, head);
    fclose(file);
    
    if (img == NULL) {
        printf("Erro ao carregar a imagem\n");
        return 1;
    }
    
    // Aplica operações morfológicas
    img = dilata(img);  // Dilata a imagem
    img = erode(img);   // Erode a imagem
    
    // Processa componentes conexos
    int* output = calloc(img->altura * img->largura, sizeof(int));
    int labelCount = LabelImage(img->largura, img->altura, img->pixels, output);
    
    printf("Encontrados %d componentes\n", labelCount);
    
    // Libera memória
    free(output);
    free(img->pixels);
    free(img->tipo);
    free(img->head);
    free(img);
    
    return 0;
}
```

## Algoritmo

O programa utiliza um algoritmo de preenchimento por inundação recursivo para identificar componentes conexos em imagens binárias. Em seguida, utiliza um algoritmo similar para identificar furos dentro desses componentes. A saída é uma imagem PGM onde cada componente e furo recebe um nível de cinza diferente.

O fluxo de trabalho consiste em:
1. Leitura e análise de imagens binárias (PBM) de entrada
2. Aplicação de rotulação de componentes conexos para identificar objetos distintos
3. Processamento para detecção de furos nos componentes
4. Geração de saída em escala de cinza (PGM) ou colorida (PPM) com componentes rotulados
5. Cálculo de estatísticas sobre componentes e furos

## Licença

Este projeto está licenciado sob a Licença MIT - veja o arquivo LICENSE para detalhes.