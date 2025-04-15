#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "image_formats.h"

#define FLOAT_TO_INT(x) ((x)>=0?(int)((x)+0.5):(int)((x)-0.5))

// Forward declaration of the LabelComponent function
void LabelComponent(int width, int height, char* input, int* output, int labelNo, int x, int y);

imagem* inicializaImagem(imagem *im, int altura, int largura)
{
    im = malloc(sizeof(imagem));
    if (im == NULL) {
        printf("Erro: Falha na alocação de memória para imagem\n");
        return NULL;
    }
    
    im->altura = altura;
    im->largura = largura;
    im->gamma = 255; // Definindo um valor padrão para gamma
    im->tipo = malloc(3 * sizeof(char));
    im->head = malloc(40 * sizeof(char));
    im->pixels = malloc(altura * largura * sizeof(char));
    
    if (im->tipo == NULL || im->head == NULL || im->pixels == NULL) {
        printf("Erro: Falha na alocação de memória para componentes da imagem\n");
        free(im->tipo);
        free(im->head);
        free(im->pixels);
        free(im);
        return NULL;
    }
    
    return im;
}

imagem* carregaImagem(FILE* file, char* tipo, char* head)
{
    char t = '\0';
    int largura = 0;
    int altura = 0;
    imagem *p = NULL;

    if (file == NULL)
    {
        printf("ERRO! O arquivo nao foi aberto! Verifique o caminho e tente novamente.\n");
        return NULL;
    }

    printf("O arquivo foi aberto com sucesso!\n");

    // Ler o tipo de arquivo
    int n = 0;
    while ((t = fgetc(file)) != '\n' && n < 3)
    {
        tipo[n++] = t;
    }
    tipo[n] = '\0';

    // Verificar se é um arquivo P1
    if (strcmp(tipo, "P1") == 0)
    {
        // Pular comentários
        t = fgetc(file);
        if (t == '#')
        {
            ungetc(t, file);
            char line[100];
            fgets(line, sizeof(line), file);
        }
        else
        {
            ungetc(t, file);
        }

        // Ler dimensões
        fscanf(file, "%d %d", &largura, &altura);
        printf("A imagem possui %d x %d pixels\n", largura, altura);

        // Inicializar imagem
        p = inicializaImagem(p, altura, largura);
        if (p == NULL) {
            return NULL;
        }
        
        // Copiar tipo e cabeçalho
        strcpy(p->tipo, tipo);
        strcpy(p->head, head);

        // Ler pixels
        for (int i = 0; i < altura; i++)
        {
            for (int j = 0; j < largura; j++)
            {
                do
                {
                    t = fgetc(file);
                } while (t == '\n' || t == ' ' || t == '\t');
                
                p->pixels[(i * largura) + j] = t;
            }
        }
    }
    else
    {
        printf("Tipo invalido. Carregue um arquivo P1\n");
        return NULL;
    }

    return p;
}

void negativo(imagem* im)
{
    int l = im->altura;
    int c = im->largura;
    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < c; j++)
        {
            im->pixels[(i * c) + j] = (im->pixels[(i * c) + j] == '0') ? '1' : '0';
        }
    }
}

imagem* dilata(imagem *im)
{
    // Criar e inicializar a máscara
    imagem *mask = inicializaImagem(NULL, 3, 3);
    if (mask == NULL) {
        return im; // Retorna a imagem original em caso de erro
    }
    
    strcpy(mask->tipo, "P1");
    mask->pixels[0] = '0'; mask->pixels[1] = '1'; mask->pixels[2] = '0';
    mask->pixels[3] = '1'; mask->pixels[4] = '1'; mask->pixels[5] = '1';
    mask->pixels[6] = '0'; mask->pixels[7] = '1'; mask->pixels[8] = '0';

    // Alocar espaço para a imagem temporária
    char **novaMatriz = malloc((im->altura + 2) * sizeof(char*));
    char **dilatada = malloc((im->altura + 2) * sizeof(char*));
    
    if (novaMatriz == NULL || dilatada == NULL) {
        free(mask->pixels);
        free(mask->tipo);
        free(mask->head);
        free(mask);
        free(novaMatriz);
        free(dilatada);
        return im;
    }
    
    for (int i = 0; i < im->altura + 2; i++) {
        novaMatriz[i] = malloc((im->largura + 2) * sizeof(char));
        dilatada[i] = malloc((im->largura + 2) * sizeof(char));
        
        if (novaMatriz[i] == NULL || dilatada[i] == NULL) {
            // Liberar memória alocada em caso de erro
            for (int j = 0; j < i; j++) {
                free(novaMatriz[j]);
                free(dilatada[j]);
            }
            free(novaMatriz);
            free(dilatada);
            free(mask->pixels);
            free(mask->tipo);
            free(mask->head);
            free(mask);
            return im;
        }
    }

    // Inicializar matrizes
    for (int i = 0; i < im->altura + 2; i++)
    {
        for (int j = 0; j < im->largura + 2; j++)
        {
            dilatada[i][j] = '0';

            if (i == 0 || j == 0 || i == im->altura + 1 || j == im->largura + 1)
            {
                novaMatriz[i][j] = '0';
            }
            else
            {
                novaMatriz[i][j] = im->pixels[((i - 1) * im->largura) + (j - 1)];
            }
        }
    }

    // Aplicar dilatação
    for (int i = 1; i < im->altura + 1; i++)
    {
        for (int j = 1; j < im->largura + 1; j++)
        {
            // Verificar se qualquer pixel da vizinhança corresponde à máscara
            if ((mask->pixels[0] == '1' && novaMatriz[i-1][j-1] == '1') ||
                (mask->pixels[1] == '1' && novaMatriz[i-1][j] == '1') ||
                (mask->pixels[2] == '1' && novaMatriz[i-1][j+1] == '1') ||
                (mask->pixels[3] == '1' && novaMatriz[i][j-1] == '1') ||
                (mask->pixels[4] == '1' && novaMatriz[i][j] == '1') ||
                (mask->pixels[5] == '1' && novaMatriz[i][j+1] == '1') ||
                (mask->pixels[6] == '1' && novaMatriz[i+1][j-1] == '1') ||
                (mask->pixels[7] == '1' && novaMatriz[i+1][j] == '1') ||
                (mask->pixels[8] == '1' && novaMatriz[i+1][j+1] == '1'))
            {
                dilatada[i][j] = '1';
            }
        }
    }

    // Copiar resultado para a imagem original
    for (int i = 0; i < im->altura; i++)
    {
        for (int j = 0; j < im->largura; j++)
        {
            im->pixels[(i * im->largura) + j] = dilatada[i + 1][j + 1];
        }
    }

    // Liberar memória
    for (int i = 0; i < im->altura + 2; i++) {
        free(novaMatriz[i]);
        free(dilatada[i]);
    }
    free(novaMatriz);
    free(dilatada);
    free(mask->pixels);
    free(mask->tipo);
    free(mask->head);
    free(mask);

    return im;
}

imagem* erode(imagem *im)
{
    // Criar e inicializar a máscara
    imagem *mask = inicializaImagem(NULL, 3, 3);
    if (mask == NULL) {
        return im; // Retorna a imagem original em caso de erro
    }
    
    strcpy(mask->tipo, "P1");
    mask->pixels[0] = '0'; mask->pixels[1] = '1'; mask->pixels[2] = '0';
    mask->pixels[3] = '1'; mask->pixels[4] = '1'; mask->pixels[5] = '1';
    mask->pixels[6] = '0'; mask->pixels[7] = '1'; mask->pixels[8] = '0';

    // Alocar espaço para a imagem temporária
    char **novaMatriz = malloc((im->altura + 2) * sizeof(char*));
    char **erodida = malloc((im->altura + 2) * sizeof(char*));
    
    if (novaMatriz == NULL || erodida == NULL) {
        free(mask->pixels);
        free(mask->tipo);
        free(mask->head);
        free(mask);
        free(novaMatriz);
        free(erodida);
        return im;
    }
    
    for (int i = 0; i < im->altura + 2; i++) {
        novaMatriz[i] = malloc((im->largura + 2) * sizeof(char));
        erodida[i] = malloc((im->largura + 2) * sizeof(char));
        
        if (novaMatriz[i] == NULL || erodida[i] == NULL) {
            // Liberar memória alocada em caso de erro
            for (int j = 0; j < i; j++) {
                free(novaMatriz[j]);
                free(erodida[j]);
            }
            free(novaMatriz);
            free(erodida);
            free(mask->pixels);
            free(mask->tipo);
            free(mask->head);
            free(mask);
            return im;
        }
    }

    // Inicializar matrizes
    for (int i = 0; i < im->altura + 2; i++)
    {
        for (int j = 0; j < im->largura + 2; j++)
        {
            erodida[i][j] = '0';

            if (i == 0 || j == 0 || i == im->altura + 1 || j == im->largura + 1)
            {
                novaMatriz[i][j] = '0';
            }
            else
            {
                novaMatriz[i][j] = im->pixels[((i - 1) * im->largura) + (j - 1)];
            }
        }
    }

    // Aplicar erosão
    for (int i = 1; i < im->altura + 1; i++)
    {
        for (int j = 1; j < im->largura + 1; j++)
        {
            if ((mask->pixels[0] == '0' || (mask->pixels[0] == '1' && novaMatriz[i-1][j-1] == '1')) &&
                (mask->pixels[1] == '0' || (mask->pixels[1] == '1' && novaMatriz[i-1][j] == '1')) &&
                (mask->pixels[2] == '0' || (mask->pixels[2] == '1' && novaMatriz[i-1][j+1] == '1')) &&
                (mask->pixels[3] == '0' || (mask->pixels[3] == '1' && novaMatriz[i][j-1] == '1')) &&
                (mask->pixels[4] == '0' || (mask->pixels[4] == '1' && novaMatriz[i][j] == '1')) &&
                (mask->pixels[5] == '0' || (mask->pixels[5] == '1' && novaMatriz[i][j+1] == '1')) &&
                (mask->pixels[6] == '0' || (mask->pixels[6] == '1' && novaMatriz[i+1][j-1] == '1')) &&
                (mask->pixels[7] == '0' || (mask->pixels[7] == '1' && novaMatriz[i+1][j] == '1')) &&
                (mask->pixels[8] == '0' || (mask->pixels[8] == '1' && novaMatriz[i+1][j+1] == '1')))
            {
                erodida[i][j] = '1';
            }
        }
    }

    // Copiar resultado para a imagem original
    for (int i = 0; i < im->altura; i++)
    {
        for (int j = 0; j < im->largura; j++)
        {
            im->pixels[(i * im->largura) + j] = erodida[i + 1][j + 1];
        }
    }

    // Liberar memória
    for (int i = 0; i < im->altura + 2; i++) {
        free(novaMatriz[i]);
        free(erodida[i]);
    }
    free(novaMatriz);
    free(erodida);
    free(mask->pixels);
    free(mask->tipo);
    free(mask->head);
    free(mask);

    return im;
}

int LabelImage(int width, int height, char* input, int* output)
{
    int labelNo = 0;
    int index = -1;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            index++;
            if (input[index] == '0' || output[index] != 0)
                continue;

            labelNo++;
            LabelComponent(width, height, input, output, labelNo, x, y);
        }
    }
    return labelNo;
}

void LabelComponent(int width, int height, char* input, int* output, int labelNo, int x, int y)
{
    int index = x + width * y;
    if (input[index] == '0' || output[index] != 0)
        return;

    output[index] = labelNo;

    if (x > 0)
        LabelComponent(width, height, input, output, labelNo, x - 1, y);
    if (x > 0 && y > 0)
        LabelComponent(width, height, input, output, labelNo, x - 1, y - 1);
    if (y > 0)
        LabelComponent(width, height, input, output, labelNo, x, y - 1);
    if (x < width - 1 && y > 0)
        LabelComponent(width, height, input, output, labelNo, x + 1, y - 1);
    if (x < width - 1)
        LabelComponent(width, height, input, output, labelNo, x + 1, y);
    if (x < width - 1 && y < height - 1)
        LabelComponent(width, height, input, output, labelNo, x + 1, y + 1);
    if (y < height - 1)
        LabelComponent(width, height, input, output, labelNo, x, y + 1);
    if (x > 0 && y < height - 1)
        LabelComponent(width, height, input, output, labelNo, x - 1, y + 1);
}
