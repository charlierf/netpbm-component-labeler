#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image_formats.h"
#define FLOAT_TO_INT(x) ((x)>=0?(int)((x)+0.5):(int)((x)-0.5))

// PPM-specific implementation of initialization function
imagemPPM* inicializaImagemPPM(imagemPPM *im, int altura, int largura)
{
    im = malloc(sizeof(imagemPPM));
    if (im == NULL) {
        printf("Erro: Falha na alocação de memória para imagem PPM\n");
        return NULL;
    }
    
    im->altura = altura;
    im->largura = largura;
    im->gamma = 255; // Valor padrão
    im->tipo = malloc(3 * sizeof(char));
    im->head = malloc(40 * sizeof(char));
    im->pixels = malloc(altura * largura * sizeof(pixel));
    
    if (im->tipo == NULL || im->head == NULL || im->pixels == NULL) {
        printf("Erro: Falha na alocação de memória para componentes da imagem PPM\n");
        free(im->tipo);
        free(im->head);
        free(im->pixels);
        free(im);
        return NULL;
    }
    
    return im;
}

imagemPPM* getImage(FILE* file, char* tipo, char* head)
{
    char t = '\0';
    char taux[10] = {0};
    int n = 0;
    int c = 0;
    int l = 0;
    imagemPPM *p = NULL;
    pixel pix;

    if (file == NULL)
    {
        printf("ERRO! O arquivo nao foi aberto! Verifique o caminho e tente novamente.\n");
        return NULL;
    }

    printf("O arquivo foi aberto com sucesso!\n");

    // Ler o tipo de arquivo
    n = 0;
    while ((t = fgetc(file)) != '\n' && n < 3)
    {
        tipo[n++] = t;
    }
    tipo[n] = '\0';

    // Verificar se é um arquivo P3
    if (strcmp(tipo, "P3") == 0)
    {
        // Pular comentários
        t = fgetc(file);
        if (t == '#')
        {
            n = 0;
            head[n++] = t;
            while ((t = fgetc(file)) != '\n' && n < 40)
            {
                head[n++] = t;
            }
            head[n] = '\0';
        }
        else
        {
            ungetc(t, file);
        }

        // Ler dimensões
        fscanf(file, "%d %d", &c, &l);
        printf("A imagem possui %d x %d pixels", c, l);

        // Inicializar imagem
        p = inicializaImagemPPM(NULL, l, c);
        if (p == NULL) {
            return NULL;
        }
        
        // Ler o valor máximo (gamma)
        fscanf(file, "%d", &p->gamma);
        printf(" e %d de valor max do pixel.\nCarregando...\n[", p->gamma);

        // Ler pixels
        for (int i = 0; i < l; i++)
        {
            for (int j = 0; j < c; j++)
            {
                fscanf(file, "%u", &pix.red);
                fscanf(file, "%u", &pix.green);
                fscanf(file, "%u", &pix.blue);
                p->pixels[(i * c) + j] = pix;
            }
            if (i % 100 == 0)
            {
                printf(".");
            }
        }
        printf("] Imagem carregada.\n\n");
    }
    else
    {
        printf("Tipo invalido. Carregue um arquivo P3 (PPM).\n");
        return NULL;
    }

    return p;
}

// PPM-specific version of negativo
void negativoPPM(imagemPPM *im)
{
    int l = im->altura;
    int c = im->largura;
    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < c; j++)
        {
            im->pixels[(i * c) + j].red = im->gamma - im->pixels[(i * c) + j].red;
            im->pixels[(i * c) + j].green = im->gamma - im->pixels[(i * c) + j].green;
            im->pixels[(i * c) + j].blue = im->gamma - im->pixels[(i * c) + j].blue;
        }
    }
}

void limiarPPM(int limiarR, int limiarG, int limiarB, imagemPPM* im)
{
    int l = im->altura;
    int c = im->largura;
    
    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < c; j++)
        {
            // Apply thresholds - convert to unsigned for comparison
            im->pixels[(i * c) + j].red = (im->pixels[(i * c) + j].red >= (unsigned int)limiarR) ? 
                                           im->gamma : 0;
            im->pixels[(i * c) + j].green = (im->pixels[(i * c) + j].green >= (unsigned int)limiarG) ? 
                                              im->gamma : 0;
            im->pixels[(i * c) + j].blue = (im->pixels[(i * c) + j].blue >= (unsigned int)limiarB) ? 
                                             im->gamma : 0;
        }
    }
}

void histogramaPPM(imagemPPM* im, unsigned int histo[])
{
    int l = im->altura;
    int c = im->largura;
    
    // Initialize histogram
    for (int i = 0; i <= im->gamma; i++)
    {
        histo[i] = 0;
    }
    
    // Count average pixel values
    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < c; j++)
        {
            unsigned int avg = (im->pixels[(i * c) + j].red + 
                               im->pixels[(i * c) + j].green + 
                               im->pixels[(i * c) + j].blue) / 3;
            histo[avg]++;
        }
    }
}

void histogramaRGB(imagemPPM* im, unsigned int histoR[], unsigned int histoG[], unsigned int histoB[])
{
    int l = im->altura;
    int c = im->largura;
    
    // Initialize histograms
    for (int i = 0; i <= im->gamma; i++)
    {
        histoR[i] = 0;
        histoG[i] = 0;
        histoB[i] = 0;
    }
    
    // Count pixel values
    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < c; j++)
        {
            histoR[im->pixels[(i * c) + j].red]++;
            histoG[im->pixels[(i * c) + j].green]++;
            histoB[im->pixels[(i * c) + j].blue]++;
        }
    }
}

void equalizarPPM(imagemPPM* im, unsigned int histo[])
{
    int l = im->altura;
    int c = im->largura;
    int pixelCount = l * c;
    float cdf[256] = {0};
    int newValues[256] = {0};
    
    // Calculate CDF
    float sum = 0.0f;
    for (int i = 0; i <= im->gamma; i++)
    {
        sum += (float)histo[i] / pixelCount;
        cdf[i] = sum;
        newValues[i] = (int)(cdf[i] * im->gamma + 0.5f);
    }
    
    // Apply equalization
    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < c; j++)
        {
            unsigned int avg = (im->pixels[(i * c) + j].red + 
                               im->pixels[(i * c) + j].green + 
                               im->pixels[(i * c) + j].blue) / 3;
            float factor = avg > 0 ? (float)newValues[avg] / avg : 1.0f;
            
            unsigned int newRed = (unsigned int)(im->pixels[(i * c) + j].red * factor);
            unsigned int newGreen = (unsigned int)(im->pixels[(i * c) + j].green * factor);
            unsigned int newBlue = (unsigned int)(im->pixels[(i * c) + j].blue * factor);
            
            im->pixels[(i * c) + j].red = newRed <= im->gamma ? newRed : im->gamma;
            im->pixels[(i * c) + j].green = newGreen <= im->gamma ? newGreen : im->gamma;
            im->pixels[(i * c) + j].blue = newBlue <= im->gamma ? newBlue : im->gamma;
        }
    }
}

void equalizarRGB(imagemPPM* im, unsigned int histoR[], unsigned int histoG[], unsigned int histoB[])
{
    int l = im->altura;
    int c = im->largura;
    int pixelCount = l * c;
    float cdfR[256] = {0}, cdfG[256] = {0}, cdfB[256] = {0};
    int newR[256] = {0}, newG[256] = {0}, newB[256] = {0};
    
    // Calculate CDFs
    float sumR = 0.0f, sumG = 0.0f, sumB = 0.0f;
    for (int i = 0; i <= im->gamma; i++)
    {
        sumR += (float)histoR[i] / pixelCount;
        sumG += (float)histoG[i] / pixelCount;
        sumB += (float)histoB[i] / pixelCount;
        
        cdfR[i] = sumR;
        cdfG[i] = sumG;
        cdfB[i] = sumB;
        
        newR[i] = (int)(cdfR[i] * im->gamma + 0.5f);
        newG[i] = (int)(cdfG[i] * im->gamma + 0.5f);
        newB[i] = (int)(cdfB[i] * im->gamma + 0.5f);
    }
    
    // Apply equalization
    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < c; j++)
        {
            im->pixels[(i * c) + j].red = newR[im->pixels[(i * c) + j].red];
            im->pixels[(i * c) + j].green = newG[im->pixels[(i * c) + j].green];
            im->pixels[(i * c) + j].blue = newB[im->pixels[(i * c) + j].blue];
        }
    }
}

void brilho(imagemPPM* im, int valor)
{
    int l = im->altura;
    int c = im->largura;
    
    printf("Ajustando brilho em %d...\n", valor);
    
    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < c; j++)
        {
            // Apply brightness with bounds checking using proper type comparison
            int newRed = (int)im->pixels[(i * c) + j].red + valor;
            int newGreen = (int)im->pixels[(i * c) + j].green + valor;
            int newBlue = (int)im->pixels[(i * c) + j].blue + valor;
            
            // Clamp values
            im->pixels[(i * c) + j].red = (newRed < 0) ? 0 : 
                                          (newRed > (int)im->gamma) ? im->gamma : (unsigned int)newRed;
            
            im->pixels[(i * c) + j].green = (newGreen < 0) ? 0 : 
                                            (newGreen > (int)im->gamma) ? im->gamma : (unsigned int)newGreen;
            
            im->pixels[(i * c) + j].blue = (newBlue < 0) ? 0 : 
                                          (newBlue > (int)im->gamma) ? im->gamma : (unsigned int)newBlue;
        }
    }
    
    printf("Brilho ajustado com sucesso.\n");
}
