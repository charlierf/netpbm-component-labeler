#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ListaFilaPilha.c"
#include <stdbool.h>


#define FLOAT_TO_INT(x) ((x)>=0?(int)((x)+0.5):(int)((x)-0.5))

typedef struct
{
    int altura;
    int largura;
    char * tipo;
    char * head;
    char * pixels;
} imagem;

imagem* inicializaImagem (imagem *im, int altura, int largura)
{
    im = malloc(sizeof(imagem));
    im->altura = altura;
    im->largura = largura;
    im->tipo = malloc(3*sizeof(char));
    im->head = malloc(40*sizeof(char));
    im->pixels = malloc(altura*largura*sizeof(char));
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
            if (strcmp(tipo, "P1\n") == 0)
            {

                t = fgetc(file);
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
                printf("A imagem possui %d x %d pixels\n", c, l);

                //Criar imagem struct
                p = inicializaImagem(p, l, c);
                p->tipo = tipo;
                p->head = head;

                //Criando matriz da imagem
                for(int i = 0; i < l; i++)
                {
                    for(int j = 0; j < c; j++)
                    {
                        t = fgetc(file);
                        while(t == '\n' || t == ' ' || t == EOF){
                            t = fgetc(file);
                        }
                        p->pixels[(i*c)+j] = t;
                        t = '\0';
                    }
                }
            } else
            {
                printf("Tipo invalido %sCarregue um arquivo P1\n", tipo);
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
    printf("Ihu");
    int l = im->altura;
    int c = im->largura;
    for(int i = 0; i < l; i++)
    {
        for(int j = 0; j < c; j++){
            switch (im->pixels[(i*c)+j]){
                case '0':
                im->pixels[(i*c)+j] = '1';
                break;

                case '1':
                im->pixels[(i*c)+j] = '0';
                break;

                default:
                im->pixels[(i*c)+j] = '\0';
            }
        }
    }
}

imagem* dilata(imagem *im){

    imagem *mask;
    mask = inicializaImagem(mask, 3, 3);
    mask->tipo = "P1\n";
    mask->pixels[0] = '0'; mask->pixels[1] = '1'; mask->pixels[2] = '0';
    mask->pixels[3] = '0'; mask->pixels[4] = '1'; mask->pixels[5] = '0';
    mask->pixels[6] = '0'; mask->pixels[7] = '1'; mask->pixels[8] = '0';

    char novaMatriz[im->altura + 2][im->largura + 2];
    char dilatada[im->altura + 2][im->largura + 2];

    for(int i = 0; i < im->altura + 2; i++)
    {
        for(int j = 0; j < im->largura + 2; j++)
        {
            dilatada[i][j] = '0';

            if(i == 0 || j == 0 || i == im->altura + 2 || j == im->largura + 2)
            {
                novaMatriz[i][j] = '0';
            } else {
                novaMatriz[i][j] = im->pixels[((i-1)*im->largura)+(j-1)];
            }

        }
    }

    for(int i = 0; i < im->altura + 2; i++)
    {
        for(int j = 0; j < im->largura + 2; j++)
        {
            if((mask->pixels[0] == '1' && novaMatriz[i][j] == '1') ||
            (mask->pixels[1] == '1' && novaMatriz[i][j+1] == '1') ||
            (mask->pixels[2] == '1' && novaMatriz[i][j+2] == '1') ||
            (mask->pixels[3] == '1' && novaMatriz[i+1][j] == '1') ||
            (mask->pixels[4] == '1' && novaMatriz[i+1][j+1] == '1') ||
            (mask->pixels[5] == '1' && novaMatriz[i+1][j+2] == '1') ||
            (mask->pixels[6] == '1' && novaMatriz[i+2][j] == '1') ||
            (mask->pixels[7] == '1' && novaMatriz[i+2][j+1] == '1') ||
            (mask->pixels[8] == '1' && novaMatriz[i+2][j+2] == '1'))
            {
                dilatada[i+1][j+1] = '1';
            }
        }
    }

        for(int i = 0; i < im->altura; i++)
        {
            for(int j = 0; j < im->largura; j++)
            {

                im->pixels[(i*im->largura)+j] = dilatada[i+1][j+1];

            }
        }

    return im;

}

imagem* erode(imagem *im){

    imagem *mask;
    mask = inicializaImagem(mask, 3, 3);
    mask->tipo = "P1\n";
    mask->pixels[0] = '0'; mask->pixels[1] = '1'; mask->pixels[2] = '0';
    mask->pixels[3] = '0'; mask->pixels[4] = '1'; mask->pixels[5] = '0';
    mask->pixels[6] = '0'; mask->pixels[7] = '1'; mask->pixels[8] = '0';

    char novaMatriz[im->altura + 2][im->largura + 2];
    char erodida[im->altura + 2][im->largura + 2];

    for(int i = 0; i < im->altura + 2; i++)
    {
        for(int j = 0; j < im->largura + 2; j++)
        {
            erodida[i][j] = '0';

            if(i == 0 || j == 0 || i == im->altura + 2 || j == im->largura + 2)
            {
                novaMatriz[i][j] = '0';
            } else {
                novaMatriz[i][j] = im->pixels[((i-1)*im->largura)+(j-1)];
            }

        }
    }

    for(int i = 0; i < im->altura + 2; i++)
    {
        for(int j = 0; j < im->largura + 2; j++)
        {
            if((mask->pixels[0] == '0' || (mask->pixels[0] == '1' && novaMatriz[i][j] == '1')) &&
                (mask->pixels[1] == '0' || (mask->pixels[1] == '1' && novaMatriz[i][j+1] == '1')) &&
                (mask->pixels[2] == '0' || (mask->pixels[2] == '1' && novaMatriz[i][j+2] == '1')) &&
                (mask->pixels[3] == '0' || (mask->pixels[3] == '1' && novaMatriz[i+1][j] == '1')) &&
                (mask->pixels[4] == '0' || (mask->pixels[4] == '1' && novaMatriz[i+1][j+1] == '1')) &&
                (mask->pixels[5] == '0' || (mask->pixels[5] == '1' && novaMatriz[i+1][j+2] == '1')) &&
                (mask->pixels[6] == '0' || (mask->pixels[6] == '1' && novaMatriz[i+2][j] == '1')) &&
                (mask->pixels[7] == '0' || (mask->pixels[7] == '1' && novaMatriz[i+2][j+1] == '1')) &&
                (mask->pixels[8] == '0' || (mask->pixels[8] == '1' && novaMatriz[i+2][j+2] == '1')))
            {

                erodida[i+1][j+1] = '1';
            }
        }
    }


        for(int i = 0; i < im->altura; i++)
        {
            for(int j = 0; j < im->largura; j++)
            {

                im->pixels[(i*im->largura)+j] = erodida[i+1][j+1];

            }
        }

    return im;

}

int LabelImage(int width, int height, char* input, int* output)
{
  printf("Ehnoismano");
  int labelNo = 0;
  int index   = -1;
  for (int y = 0; y < height; y++)
  {
    for (int x = 0; x < width; x++)
    {
      index++;
      if (input [index] == '0') continue;   /* This pixel is not part of a component */
      if (output[index] != 0) continue;   /* This pixel has already been labelled  */
      /* New component found */
      labelNo++;

      LabelComponent(width, height, input, output, labelNo, x, y);
    }
  }

  return labelNo;

}

void LabelComponent(int width, int height,
char* input, int* output, int labelNo, int x, int y)
{
  //printf("MuitoLoko");

  //printf("Componente: %d\n", labelNo);
  int index = x + width*y;
  if (input [index] == '0') return;   /* This pixel is not part of a component */
  if (output[index] != 0) return;   /* This pixel has already been labelled  */
  output[index] = labelNo;

  /* Now label the 8 neighbours: */
  if (x > 0)        LabelComponent(width, height, input, output, labelNo, x-1, y);   /* left  pixel */
  if (x > 0 && y > 0) LabelComponent(width, height, input, output, labelNo, x-1, y-1); /* upper left  pixel */
  if (y > 0)        LabelComponent(width, height, input, output, labelNo, x, y-1);   /* upper pixel */
  if (x < width-1 && y > 0)  LabelComponent(width, height, input, output, labelNo, x+1, y-1);   /* upper right pixel */
  if (x < width-1)  LabelComponent(width, height, input, output, labelNo, x+1, y);   /* right pixel */
  if (x < width-1 && y < height-1)  LabelComponent(width, height, input, output, labelNo, x+1, y+1);   /* lower right pixel */
  if (y < height-1) LabelComponent(width, height, input, output, labelNo, x, y+1);   /* lower pixel */
  if (x < 0 && y < height-1)  LabelComponent(width, height, input, output, labelNo, x-1, y+1);   /* lower left pixel */
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

    entrada = fopen("espc.pbm", "r");
    saida = fopen("teste.ppm", "w");

    p = getImage(entrada, tipo, head);
    l = p->altura;
    c = p->largura;

    //negativo(p);

    /*
    for (int i = 0; i < gamma+1; i++)
    {
        printf("h(%d): %d\n", i, histo[i]);
    }
    */
    int *total;
    total = malloc(l*c*(sizeof(int)));

    for (int i = 0; i < l; i++){
        for (int j = 0; j < c; j++){
            total[(i*c)+j] = 0;
        }
    }

    int totalB;
    //p = erode(p);
    //contaObjetos(p, total,totalBuracos);

    totalB = LabelImage(p->largura, p->altura, p->pixels, total);

    //Imprimindo matriz da imagem
    printf("%s%d %d\nTotal:%d\TotalBuracos:%d\n", p->tipo, p->largura, p->altura, totalB, totalB);
    fprintf(saida, "%s#Criado por CharlieRF\n%d %d\n%d\n", "P2\n", p->largura, p->altura, totalB);
    for(int a = 0; a < p->altura; a++)
    {
        for(int b = 0; b < p->largura; b++)
        {
            //fprintf(saida, "%c", p->pixels[(a*c)+b]);
            fprintf(saida, "%d\n", total[(a*c)+b]);
        }
    }
    if (saida != NULL)
    {
        printf("Arquivo 'teste.pbm' criado com sucesso.\n");
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
