#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FLOAT_TO_INT(x) ((x)>=0?(int)((x)+0.5):(int)((x)-0.5))

typedef struct
{
    unsigned int red;
    unsigned int green;
    unsigned int blue;
} pixel;

typedef struct
{
    int altura;
    int largura;
    int gamma;
    pixel * pixels;
} imagem;

imagem* inicializaImagem (imagem *im, int altura, int largura)
{
    im = malloc(sizeof(imagem));
    im->altura = altura;
    im->largura = largura;
    im->gamma = 0;
    im->pixels = malloc(altura*largura*sizeof(pixel));

    return im;
}

imagem* getImage(FILE* file, char* tipo, char* head)
{
    char t = '\0';
    char taux[10];
    int n = 0;
    int c = 0;
    int l = 0;
    imagem * p;
    pixel pix;

    for (int i = 0; i < 10; i++)
    {
        taux[i] = '\0';
    }

    if (file == NULL)
    {
        printf("ERRO! O arquivo nao foi aberto! Verifique o caminho e tente novamente.\n");
    }
    else
    {
        printf("O arquivo foi aberto com sucesso!\n");

        while (t != '\n')
        {
            t = fgetc(file);
            tipo[n] = t;
            n++;
        }
        t = '\0';
        n = 0;

        if (tipo != NULL)
        {
            //Verificar se o formato é correto
            if (strcmp(tipo, "P3\n") == 0)
            {

                t = getc(file);
                if (t == '#')
                {
                    head[n] = t;
                    n++;
                    //Pegar cabeçalho até posição 39 e pular o resto
                    while (t != '\n')
                    {
                        if (n < 40)
                        {
                            t = fgetc(file);
                            head[n] = t;
                            n++;
                        } else
                        {
                            t = fgetc(file);
                            n++;
                        }
                    }
                    n = 0;
                    t = '\0';

                    //Pegar colunas
                    while (t != ' ')
                    {
                        t = fgetc(file);
                        taux[n] = t;
                        n++;
                    }
                    n = 0;
                    c = atoi(taux);
                    t = '\0';

                } else
                {
                    taux[n] = t;
                    n++;

                    //Pegar colunas
                    while (t != ' ')
                    {
                        t = fgetc(file);
                        taux[n] = t;
                        n++;
                    }
                    n = 0;
                    c = atoi(taux);
                    t = '\0';

                }

                //Pegar linhas
                while (t != '\n')
                {
                    t = fgetc(file);
                    taux[n] = t;
                    n++;
                }
                n = 0;
                l = atoi(taux);
                t = '\0';
                printf("A imagem possui %d x %d pixels", c, l);

                //Criar imagem struct
                p = inicializaImagem(p, l, c);

                //Pegar tamanho do pixel
                while (t != '\n')
                {
                    t = fgetc(file);
                    taux[n] = t;
                    n++;
                }
                p->gamma = atoi(taux);
                n = 0;
                t = '\0';
                printf(" e %d de valor max do pixel.\nCarregando...\n[", p->gamma);

                //Criando matriz da imagem
                for(int i = 0; i < l; i++)
                {
                    for(int j = 0; j < c; j++)
                    {
                        //RED
                        n = 0;
                        while (t != '\n' && t != ' ' && t != EOF)
                        {
                            t = fgetc(file);
                            taux[n] = t;
                            if(t != '\n' || t != ' ' || t != EOF){ n++;}
                        }
                        pix.red = atoi(taux);
                        t = '\0';

                        //GREEN
                        n = 0;
                        while (t != '\n' && t != ' ' && t != EOF)
                        {
                            t = fgetc(file);
                            taux[n] = t;
                            if(t != '\n' || t != ' ' || t != EOF){ n++;}
                        }
                        pix.green = atoi(taux);
                        t = '\0';

                        //BLUE
                        n = 0;
                        while (t != '\n' && t != ' ' && t != EOF)
                        {
                            t = fgetc(file);
                            taux[n] = t;
                            if(t != '\n' || t != ' ' || t != EOF){ n++;}
                        }
                        pix.blue = atoi(taux);
                        t = '\0';
                        p->pixels[(i*c)+j] = pix;

                    }
                    if(i%100 == 0 ){printf(".");}
                }
                printf("] Imagem carregada.\n\n");
            } else
            {
                printf("Tipo invalido %sCarregue um arquivo P3\n", tipo);
            }
        } else
        {
            printf("Arquivo invalido\n");
        }
    }
                return p;
}

void negativo(imagem* im)
{
    int l = im->altura;
    int c = im->largura;
    printf("Gerando negativo...\n[");
    for(int i = 0; i < l; i++)
    {
        for(int j = 0; j < c; j++)
        {
            im->pixels[(i*c)+j].red = im->gamma - im->pixels[(i*c)+j].red;
            im->pixels[(i*c)+j].green = im->gamma - im->pixels[(i*c)+j].green;
            im->pixels[(i*c)+j].blue = im->gamma - im->pixels[(i*c)+j].blue;
        }
        if(i%100 == 0 ){printf(".");}
    }
    printf("] 100%%\n\n");
}


void limiar(int limiarR, int limiarG, int limiarB, imagem* im)
{
    int l = im->altura;
    int c = im->largura;
    printf("Aplicando limiar R:%d, G:%d, B:%d ...\n[", limiarR, limiarG, limiarB);
    for(int i = 0; i < l; i++)
    {
        for(int j = 0; j < c; j++)
        {
            //RED
            if (im->pixels[(i*c)+j].red >= limiarR)
            {
                im->pixels[(i*c)+j].red = im->gamma;
            } else {
            im->pixels[(i*c)+j].red = 0;
            }

            //GREEN
            if (im->pixels[(i*c)+j].green >= limiarG)
            {
                im->pixels[(i*c)+j].green = im->gamma;
            } else {
            im->pixels[(i*c)+j].green = 0;
            }

            //BLUE
            if (im->pixels[(i*c)+j].blue >= limiarB)
            {
                im->pixels[(i*c)+j].blue = im->gamma;
            } else {
            im->pixels[(i*c)+j].blue = 0;
            }
        }
        if(i%100 == 0 ){printf(".");}
    }
    printf("] 100%%\n\n");
}

void histograma (imagem* im, unsigned int histo[im->gamma + 1])
{
    int l = im->altura;
    int c = im->largura;
    int Lmax = im->gamma + 1;
    printf("Calculando histograma simples...\n[");
    for (int i = 0; i < Lmax; i++)
    {
        histo[i] = 0;
    }

    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < c; j++)
        {
            int aux = im->pixels[(i*c)+j].red;
            histo[aux] = ++histo[aux];
            aux = im->pixels[(i*c)+j].green;
            histo[aux] = ++histo[aux];
            aux = im->pixels[(i*c)+j].blue;
            histo[aux] = ++histo[aux];
        }
        if(i%100 == 0 ){printf(".");}
    }
    printf("] 100%%\n\n");
    /*
    for (int i = 0; i < Lmax; i++)
    {
        printf("h(%d): %d\n", i, histo[i]);
    }
    */
}

void histogramaRGB (imagem* im, unsigned int histoR[im->gamma + 1], unsigned int histoG[im->gamma + 1], unsigned int histoB[im->gamma + 1])
{
    int l = im->altura;
    int c = im->largura;
    int Lmax = im->gamma + 1;
    printf("Calculando histograma RGB...\n[");
    for (int i = 0; i < Lmax; i++)
    {
        histoR[i] = 0;
        histoG[i] = 0;
        histoB[i] = 0;
    }

    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < c; j++)
        {
            int aux = im->pixels[(i*c)+j].red;
            histoR[aux] = ++histoR[aux];

            aux = im->pixels[(i*c)+j].green;
            histoG[aux] = ++histoG[aux];

            aux = im->pixels[(i*c)+j].blue;
            histoB[aux] = ++histoB[aux];
        }
        if(i%100 == 0 ){printf(".");}
    }
    printf("] 100%%\n\n");
    /*
    for (int i = 0; i < Lmax; i++)
    {
        printf("h(%d): %d\n", i, histo[i]);
    }
    */
}

void equalizar (imagem* im, unsigned int histo[im->gamma + 1])
{
    int l = im->altura;
    int c = im->largura;
    int Lmax = im->gamma + 1;
    float probabilidade[Lmax], pA[Lmax];
    int g[Lmax];
    printf("Equalizando imagem...\n[");


    for (int i = 0; i < Lmax; i++)
    {
        probabilidade[i] = (float)histo[i]/(float)(3*l*c);
        //printf("P(%d) = %d/%d: %f ", i, histo[i], c*l, probabilidade[i]);
        if(i%100 == 0 ){printf(".");}
    }

    for (int i = 0; i < Lmax; i++)
    {
        if (i == 0) {pA[i] = probabilidade[i];}
        else {pA[i] = probabilidade[i] + pA[i-1];}
        //printf("pA(%d): %f ", i, pA[i]);
        if(i%100 == 0 ){printf(".");}
    }

    for (int i = 0; i < Lmax; i++)
    {
        g[i] = FLOAT_TO_INT(pA[i] * (float)(Lmax-1));
        //printf("g(%d): %d ", i, g[i]);
        if(i%100 == 0 ){printf(".");}
    }

    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < c; j++)
        {
            int aux = im->pixels[(i*c)+j].red;
            im->pixels[(i*c)+j].red = g[aux];

            aux = im->pixels[(i*c)+j].green;
            im->pixels[(i*c)+j].green = g[aux];

            aux = im->pixels[(i*c)+j].blue;
            im->pixels[(i*c)+j].blue = g[aux];
        }
        if(i%100 == 0 ){printf(".");}
    }
    printf("] 100%%\n\n");

}

void equalizarRGB (imagem* im, unsigned int histoR[im->gamma + 1], unsigned int histoG[im->gamma + 1], unsigned int histoB[im->gamma + 1])
{
    int l = im->altura;
    int c = im->largura;
    int Lmax = im->gamma + 1;
    float prR[Lmax], prG[Lmax], prB[Lmax], pAR[Lmax], pAG[Lmax], pAB[Lmax], aux;
    int gR[Lmax], gG[Lmax], gB[Lmax];
    aux = 0;
    printf("Equalizando canais da imagem...\n[");

    for (int i = 0; i < Lmax; i++)
    {
        prR[i] = (float)histoR[i]/(float)(l*c);
        prG[i] = (float)histoG[i]/(float)(l*c);
        prB[i] = (float)histoB[i]/(float)(l*c);
        //printf("P(%d) = %d/%d: %f ", i, histo[i], c*l, probabilidade[i]);
        if(i%100 == 0 ){printf(".");}
    }

    for (int i = 0; i < Lmax; i++)
    {
        pAR[i] = prR[i] + aux;
        aux = pAR[i];

        pAG[i] = prG[i] + aux;
        aux = pAG[i];

        pAB[i] = prB[i] + aux;
        aux = pAB[i];
        //printf("pA(%d): %f ", i, pA[i]);
        if(i%100 == 0 ){printf(".");}
    }

    for (int i = 0; i < Lmax; i++)
    {
        gR[i] = FLOAT_TO_INT(pAR[i] * (float)(Lmax-1));
        gG[i] = FLOAT_TO_INT(pAG[i] * (float)(Lmax-1));
        gB[i] = FLOAT_TO_INT(pAB[i] * (float)(Lmax-1));
        //printf("g(%d): %d ", i, g[i]);
        if(i%100 == 0 ){printf(".");}
    }

    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < c; j++)
        {
            int aux = im->pixels[(i*c)+j].red;
            im->pixels[(i*c)+j].red = gR[aux];

            aux = im->pixels[(i*c)+j].green;
            im->pixels[(i*c)+j].green = gG[aux];

            aux = im->pixels[(i*c)+j].blue;
            im->pixels[(i*c)+j].blue = gB[aux];
        }
        if(i%100 == 0 ){printf(".");}
    }
    printf("] 100%%\n\n");

}

void brilho (imagem* im, int valor)
{
    int l = im->altura;
    int c = im->largura;
    printf("Aumentando o brilho em $d ... [", valor);
    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < c; j++)
        {
            if ((im->pixels[(i*c)+j].red + valor) > im->gamma)
            {
                im->pixels[(i*c)+j].red = 255;
            } else { im->pixels[(i*c)+j].red = im->pixels[(i*c)+j].red + valor;}

            if ((im->pixels[(i*c)+j].green + valor) > im->gamma)
            {
                im->pixels[(i*c)+j].green = 255;
            } else { im->pixels[(i*c)+j].green = im->pixels[(i*c)+j].green + valor;}

            if ((im->pixels[(i*c)+j].blue + valor) > im->gamma)
            {
                im->pixels[(i*c)+j].blue = 255;
            } else { im->pixels[(i*c)+j].blue = im->pixels[(i*c)+j].blue + valor;}
        }
        if(i%100 == 0 ){printf(".");}
    }
    printf("] 100%%\n\n");
}

int main ()
{
    FILE *entrada, *saida;
    char *tipo;
    char *head;
    imagem *p;

    char t = '\0';
    char taux[10];
    pixel pix;
    int c, l;
    int n = 0;

    tipo = malloc(3*sizeof(char));
    head = malloc(40*sizeof(char));
    pix.red = 0; pix.green = 0; pix.blue = 0;

    entrada = fopen("Figuras/Lana Conquista.ppm", "r");

    p = getImage(entrada, tipo, head);

    c = p->largura;
    l = p->altura;

    //negativo(p);
    //limiar(127, 127, 127, p);
    //brilho(p, 100);

    unsigned int hi[p->gamma+1], hiR[p->gamma+1], hiG[p->gamma+1], hiB[p->gamma+1];

    histograma(p, hi);
    equalizar(p, hi);

    //histogramaRGB(p, hiR, hiG, hiB);
    //equalizarRGB(p, hiR, hiG, hiB);

    /*
    for (int i = 0; i < gamma+1; i++)
    {
        printf("h(%d): %d\n", i, histo[i]);
    }
    */

    //Imprimindo matriz da imagem
    saida = fopen("Figuras/teste.ppm", "w");
    fclose(saida);
    saida = fopen("Figuras/teste.ppm", "a");
    printf("Salvando imagem Tipo:%c%c com %dx%d pixels e %d tons...\n[", tipo[0], tipo[1], p->largura, p->altura, p->gamma);
    fprintf(saida, "%s#Criado por CharlieRF\n%d %d\n%d\n", tipo, p->largura, p->altura, p->gamma);

    for(int a = 0; a < l; a++)
    {
        for(int b = 0; b < c; b++)
        {
            fprintf(saida, "%d\n", p->pixels[(a*c)+b].red);
            fprintf(saida, "%d\n", p->pixels[(a*c)+b].green);
            fprintf(saida, "%d\n", p->pixels[(a*c)+b].blue);
        }
        if(a%100 == 0 ){printf(".");}
    }
    printf("] 100%%\n");

    if (saida != NULL)
    {
        printf("Arquivo 'teste.ppm' criado com sucesso.\n");
    }
    else{
        printf("Falha na operacao.");
    }


    fclose(entrada);
    fclose(saida);
    free(tipo);
    free(head);
    free(p);

    system("pause");

    return 0;

}
