#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image_formats.h"
#include "algorithms.h"

int main(int argc, char **argv)
{
    FILE *entrada, *saida;
    char *tipo;
    char *head;
    imagem *p;
    int *matrizSaida;
    int *furos;
    int totalComponentes;
    int totalFuros;
    int totalCores;

    // Inicializar ponteiros
    tipo = malloc(3*sizeof(char));
    head = malloc(40*sizeof(char));

    // Verificar se o nome da imagem foi passado como argumento
    if (argc != 2) {
        printf("Uso: %s nome_da_imagem\n", argv[0]);
        exit(1);
    }

    entrada = fopen(argv[1], "r"); // Abrir a imagem com o nome passado como argumento

    // Criar o arquivo de saída com o nome "saida.ppm"
    saida = fopen("./examples/output/saida.ppm", "w");

    // Carregar imagem de entrada no ponteiro para struct imagem, p
    p = carregaImagem(entrada, tipo, head);
    int altura = p->altura;
    int largura = p->largura;

    // Inicializar a matriz da imagem de saída com 0 em todas as posições
    matrizSaida = malloc(altura*largura*(sizeof(int)));
    for (int i = 0; i < altura; i++)
    {
        for (int j = 0; j < largura; j++)
        {
            matrizSaida[(i*largura)+j] = 0;
        }
    }

    // Iniciar a função que conta os componentes da imagem e guardar o resultado em totalComponentes
    totalComponentes = RotularImagem(p->largura, p->altura, p->pixels, matrizSaida);

    // Inicializar o vetor que marca se há furos na posição do componente com zeros
    furos = malloc(totalComponentes*sizeof(int));
    for (int i = 0; i < totalComponentes; i++)
    {
        furos[i] = 0;
    }

    // Passa o vetor como parâmetro para a função ContaFuros
    totalFuros = ContaFuros(p->largura, p->altura, matrizSaida, furos);
    totalCores = totalComponentes + totalFuros;

    // Percorre o vetor e conta quantos componentes têm furos
    int objetosComFuros = 0;
    for (int i = 0; i < totalComponentes; i++)
    {
        if (furos[i] == 1)
        {
            objetosComFuros++;
        }
    }

    // Imprimindo matriz da imagem
    printf("Total de Objetos: %d\nTotal de Furos: %d\nObjetos com Furos: %d\n", totalComponentes, totalFuros, objetosComFuros);
    printf("Salvando arquivo 'saida.ppm' destacando objetos e furos em %d tons de cinza.\n", totalCores+1);
    fprintf(saida, "%s#Criado por Charlie Fonseca e Elana Sande\n%d %d\n%d\n", "P2\n", p->largura, p->altura, totalCores);

    // Imprime a matrizSaida no arquivo de saída como tons de cinza, diferenciando furos, objetos e background com contraste
    for(int a = 0; a < altura; a++)
    {
        for(int b = 0; b < largura; b++)
        {
            // Os valores positivos referem-se aos objetos e são impressos como valores do início dos tons de cinza
            if(matrizSaida[(a*largura)+b] > 0) fprintf(saida, "%d\n", matrizSaida[(a*largura)+b]-1);
            // Os valores negativos e zero referem-se ao background e furos e são impressos como valores positivos no final dos tons de cinza
            if(matrizSaida[(a*largura)+b] <= 0) fprintf(saida, "%d\n", (totalCores+1)+(matrizSaida[(a*largura)+b]));
        }
    }
    if (saida != NULL)
    {
        printf("O arquivo 'saida.ppm' foi criado com sucesso em './examples/output' e vai ser aberto em seguida...\n");
    }
    else
    {
        printf("Falha na operacao.");
    }

    // Fechar arquivos abertos
    fclose(entrada);
    fclose(saida);

    // Abrir a imagem com o programa padrão do sistema operacional
    system("saida.ppm");

    // Liberar a memória
    free(tipo);
    free(head);
    free(p);
    free(matrizSaida);
    free(furos);

    system("pause");

    return 0;
}
