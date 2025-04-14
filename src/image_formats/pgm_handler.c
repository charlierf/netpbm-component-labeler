#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FLOAT_TO_INT(x) ((x)>=0?(int)((x)+0.5):(int)((x)-0.5))

typedef struct
{
    int altura;
    int largura;
    int gamma;
    char * tipo;
    char * head;
    int * pixels;
} imagem;

imagem* inicializaImagem (imagem *im, int altura, int largura)
{
    im = malloc(sizeof(imagem));
    im->altura = altura;
    im->largura = largura;
    im->gamma = 0;
    im->tipo = malloc(3*sizeof(char));
    im->head = malloc(40*sizeof(char));
    im->pixels = malloc(altura*largura*sizeof(int));

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
            if (strcmp(tipo, "P2\n") == 0)
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
                printf(" e %d de valor max do pixel.\n", p->gamma);

                p->tipo = tipo;
                p->head = head;

                //Criando matriz da imagem
                for(int i = 0; i < l; i++)
                {
                    for(int j = 0; j < c; j++)
                    {
                        n = 0;
                        while (t != '\n' && t != ' ' && t != EOF)
                        {
                            t = fgetc(file);
                            taux[n] = t;
                            if(t != '\n' || t != ' ' || t != EOF){ n++;}
                        }
                        p->pixels[(i*c)+j] = atoi(taux);
                        t = '\0';
                    }
                }
            } else
            {
                printf("Tipo invalido %sCarregue um arquivo P2\n", tipo);
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
    for(int i = 0; i < l; i++)
    {
        for(int j = 0; j < c; j++)
        {
            im->pixels[(i*c)+j] = im->gamma - im->pixels[(i*c)+j];
        }
    }
}

void limiar(int limiar, imagem* im)
{
    int l = im->altura;
    int c = im->largura;
    for(int i = 0; i < l; i++)
    {
        for(int j = 0; j < c; j++)
        {
            if (im->pixels[(i*c)+j] >= limiar)
            {
                im->pixels[(i*c)+j] = im->gamma;
            } else {
            im->pixels[(i*c)+j] = 0;
            }
        }
    }
}

void histograma (imagem* im, unsigned int histo[im->gamma + 1])
{
    int l = im->altura;
    int c = im->largura;
    int Lmax = im->gamma + 1;
    for (int i = 0; i < Lmax; i++)
    {
        histo[i] = 0;
    }

    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < c; j++)
        {
            int aux = im->pixels[(i*c)+j];
            histo[aux] = ++histo[aux];
        }
    }
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
    float aux = 0;


    for (int i = 0; i < Lmax; i++)
    {
        probabilidade[i] = (float)histo[i]/(float)(l*c);
        //printf("P(%d) = %d/%d: %f ", i, histo[i], c*l, probabilidade[i]);
    }

    for (int i = 0; i < Lmax; i++)
    {
        pA[i] = probabilidade[i] + aux;
        aux = pA[i];
        //printf("pA(%d): %f ", i, pA[i]);
    }

    for (int i = 0; i < Lmax; i++)
    {
        g[i] = FLOAT_TO_INT(pA[i] * (float)(Lmax-1));
        //printf("g(%d): %d ", i, g[i]);
    }

    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < c; j++)
        {
            int aux = im->pixels[(i*c)+j];
            im->pixels[(i*c)+j] = g[aux];
        }
    }

}


int main ()
{
    FILE *entrada, *saida;
    char *tipo;
    char *head;
    imagem *p;

    char t = '\0';
    char taux[10];
    int c, l;
    int n = 0;

    tipo = malloc(3*sizeof(char));
    head = malloc(40*sizeof(char));

    entrada = fopen("Figuras/elanaescura.pgm", "r");
    saida = fopen("Figuras/teste.ppm", "w");

    p = getImage(entrada, tipo, head);
    l = p->altura;
    c = p->largura;

    //negativo(gamma, l, c, p);
    //limiar(11, gamma, l, c, p);

    unsigned int histo[p->gamma+1];
    histograma(p, histo);
    equalizar(p, histo);

    /*
    for (int i = 0; i < gamma+1; i++)
    {
        printf("h(%d): %d\n", i, histo[i]);
    }
    */

    //Imprimindo matriz da imagem
    fclose(saida);
    saida = fopen("Figuras/teste.ppm", "a");
    printf("%s%d %d\n%d\n", p->tipo, p->largura, p->altura, p->gamma);
    fprintf(saida, "%s#Criado por CharlieRF\n%d %d\n%d\n", p->tipo, p->largura, p->altura, p->gamma);

    for(int a = 0; a < l; a++)
    {
        for(int b = 0; b < c; b++)
        {
            fprintf(saida, "%d\n", p->pixels[(a*c)+b]);
        }
        //printf("\n");
    }

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
