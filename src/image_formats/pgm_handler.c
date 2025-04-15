#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image_formats.h"

void limiar(int limiar, imagem* im)
{
    int l = im->altura;
    int c = im->largura;
    
    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < c; j++)
        {
            if (im->pixels[(i * c) + j] >= limiar)
            {
                im->pixels[(i * c) + j] = 255;  // Assuming max value is 255
            }
            else
            {
                im->pixels[(i * c) + j] = 0;
            }
        }
    }
}

void histograma(imagem* im, unsigned int histo[])
{
    int l = im->altura;
    int c = im->largura;
    
    // Initialize histogram array
    for (int i = 0; i < 256; i++)
    {
        histo[i] = 0;
    }
    
    // Count pixel values
    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < c; j++)
        {
            int pixelValue = im->pixels[(i * c) + j];
            histo[pixelValue]++;
        }
    }
}

void equalizar(imagem* im, unsigned int histo[])
{
    int l = im->altura;
    int c = im->largura;
    int pixelCount = l * c;
    float cdf[256] = {0};
    int newValues[256] = {0};
    
    // Calculate cumulative distribution function (CDF)
    float sum = 0.0f;
    for (int i = 0; i < 256; i++)
    {
        sum += (float)histo[i] / pixelCount;
        cdf[i] = sum;
        newValues[i] = (int)(cdf[i] * 255.0f + 0.5f);
    }
    
    // Apply equalization
    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < c; j++)
        {
            int oldValue = im->pixels[(i * c) + j];
            im->pixels[(i * c) + j] = newValues[oldValue];
        }
    }
}
