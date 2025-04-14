#ifndef IMAGE_FORMATS_H
#define IMAGE_FORMATS_H

#include <stdio.h>

// Define uma estrutura imagem para o formato PBM (P1)
typedef struct {
    int altura;
    int largura;
    char *tipo;
    char *head;
    char *pixels;
} imagem;

// Define uma estrutura para imagens PGM (P2)
typedef struct {
    int altura;
    int largura;
    int gamma;
    char *tipo;
    char *head;
    int *pixels;
} imagemPGM;

// Define uma estrutura para pixels RGB
typedef struct {
    unsigned int red;
    unsigned int green;
    unsigned int blue;
} pixel;

// Define uma estrutura para imagens PPM (P3)
typedef struct {
    int altura;
    int largura;
    int gamma;
    pixel *pixels;
} imagemPPM;

// Funções para imagens PBM (P1)
imagem* inicializaImagem(imagem *im, int altura, int largura);
imagem* carregaImagem(FILE* file, char* tipo, char* head);
void negativo(imagem* im);
imagem* dilata(imagem *im);
imagem* erode(imagem *im);

// Funções para imagens PGM (P2)
imagemPGM* inicializaImagemPGM(imagemPGM *im, int altura, int largura);
imagemPGM* getImagePGM(FILE* file, char* tipo, char* head);
void negativoPGM(imagemPGM* im);
void limiar(int limiar, imagemPGM* im);
void histograma(imagemPGM* im, unsigned int histo[]);
void equalizar(imagemPGM* im, unsigned int histo[]);

// Funções para imagens PPM (P3)
imagemPPM* inicializaImagemPPM(imagemPPM *im, int altura, int largura);
imagemPPM* getImagePPM(FILE* file, char* tipo, char* head);
void negativoPPM(imagemPPM* im);
void limiarPPM(int limiarR, int limiarG, int limiarB, imagemPPM* im);
void histogramaPPM(imagemPPM* im, unsigned int histo[]);
void histogramaRGB(imagemPPM* im, unsigned int histoR[], unsigned int histoG[], unsigned int histoB[]);
void equalizarPPM(imagemPPM* im, unsigned int histo[]);
void equalizarRGB(imagemPPM* im, unsigned int histoR[], unsigned int histoG[], unsigned int histoB[]);
void brilho(imagemPPM* im, int valor);

#endif /* IMAGE_FORMATS_H */