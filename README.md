# NetPBM Component Labeler

A tool for labeling and analyzing connected components in NetPBM images (PBM, PGM, PPM).

## Features

- Reads PBM (P1) binary images
- Labels all connected components in an image
- Detects and counts holes in components 
- Outputs a PGM image with different gray levels for each component and hole
- Provides statistics on number of objects, holes, and objects with holes
- Comprehensive support for NetPBM formats through specialized handlers:
  - PBM support for binary image input and processing
  - PGM support for grayscale output with labeled components
  - PPM support for color visualizations and component analysis

## Requirements

- C compiler (GCC recommended)
- Make

## Building the Project

Clone the repository and build the project:

```bash
git clone https://github.com/yourusername/netpbm-component-labeler.git
cd netpbm-component-labeler
make
```

## Usage

```bash
./bin/netpbm-component-labeler input_image.pbm
```

The program will generate a file named `saida.ppm` in the current directory, showing the labeled components.

## Example

```bash
./bin/netpbm-component-labeler examples/input/teste.pbm
```

## Project Structure

```
netpbm-component-labeler/
├── src/
│  ├── main.c                    # Main program code
│  ├── image_formats/
│  │  ├── pbm_handler.c          # PBM image handling functions
│  │  ├── pgm_handler.c          # PGM image handling functions
│  │  └── ppm_handler.c          # PPM image handling functions
│  ├── algorithms/
│  │  └── connected_components.c # Component labeling and analysis algorithms
│  └── data_structures/
│     └── data_structures.c      # Data structures for algorithm support
├── include/
│  ├── image_formats.h           # Header for image format functions
│  ├── algorithms.h              # Header for algorithms
│  └── data_structures.h         # Header for data structures
├── examples/                    # Example images
│  ├── input/
│  └── output/
├── Makefile                     # Build configuration
├── README.md                    # This file (English)
└── README.pt-BR.md              # Portuguese version of this file
```

## Image Format Handlers

### PBM Handler
The PBM (Portable Bitmap) handler processes binary images:
- Reads and parses P1 format binary images (ASCII format)
- Converts binary images to internal representation for processing
- Provides core functions for component labeling algorithm
- Implements morphological operations (dilation, erosion) for image preprocessing
- Supports recursive component labeling to identify connected regions

### PGM Handler
The PGM (Portable Graymap) handler provides grayscale image support:
- Creates output images with numeric labels for each component
- Maps component labels to distinct gray levels for visualization
- Processes input grayscale images with thresholding capabilities
- Implements equalization and histogram analysis functions
- Handles the generation of labeled component output

### PPM Handler
The PPM (Portable Pixmap) handler enables color image processing:
- Supports RGB color model for advanced visualizations
- Provides color-coding of components and holes for clearer distinction
- Implements channel-specific operations (RGB histogram, equalization)
- Handles brightness adjustments and threshold operations on color images
- Enables more detailed component analysis through color differentiation

## Using the Image Format Handlers

### PBM Handler Functions

```c
// Initialize a new PBM image structure
imagem* inicializaImagem(imagem *im, int altura, int largura);

// Load a PBM image from a file
imagem* carregaImagem(FILE* file, char* tipo, char* head);

// Create a negative of a PBM image
void negativo(imagem* im);

// Apply dilation morphological operation
imagem* dilata(imagem *im);

// Apply erosion morphological operation
imagem* erode(imagem *im);
```

### PGM Handler Functions

```c
// Apply threshold to a PGM image
void limiar(int limiar, imagem* im);

// Generate histogram for a PGM image
void histograma(imagem* im, unsigned int histo[]);

// Apply histogram equalization to a PGM image
void equalizar(imagem* im, unsigned int histo[]);
```

### PPM Handler Functions

```c
// Initialize a new PPM image structure
imagemPPM* inicializaImagemPPM(imagemPPM *im, int altura, int largura);

// Load a PPM image from a file
imagemPPM* getImage(FILE* file, char* tipo, char* head);

// Create a negative of a PPM image
void negativoPPM(imagemPPM* im);

// Apply threshold to each RGB channel
void limiarPPM(int limiarR, int limiarG, int limiarB, imagemPPM* im);

// Generate histogram for a PPM image (average of channels)
void histogramaPPM(imagemPPM* im, unsigned int histo[]);

// Generate separate histograms for each RGB channel
void histogramaRGB(imagemPPM* im, unsigned int histoR[], unsigned int histoG[], unsigned int histoB[]);

// Apply histogram equalization to a PPM image
void equalizarPPM(imagemPPM* im, unsigned int histo[]);

// Apply histogram equalization separately to each RGB channel
void equalizarRGB(imagemPPM* im, unsigned int histoR[], unsigned int histoG[], unsigned int histoB[]);

// Adjust brightness of a PPM image
void brilho(imagemPPM* im, int valor);
```

### Component Labeling Functions

```c
// Label all components in an image
int LabelImage(int width, int height, char* input, int* output);

// Label a single connected component
void LabelComponent(int width, int height, char* input, int* output, int labelNo, int x, int y);
```

## Example Code for Using the Handlers

Here's an example of how to use the PBM handler to load an image, apply morphological operations, and process it:

```c
#include <stdio.h>
#include <stdlib.h>
#include "image_formats.h"

int main() {
    FILE* file = fopen("input.pbm", "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return 1;
    }
    
    char tipo[3];
    char head[40];
    
    // Load a PBM image
    imagem* img = carregaImagem(file, tipo, head);
    fclose(file);
    
    if (img == NULL) {
        printf("Error loading image\n");
        return 1;
    }
    
    // Apply morphological operations
    img = dilata(img);  // Dilate the image
    img = erode(img);   // Erode the image
    
    // Process connected components
    int* output = calloc(img->altura * img->largura, sizeof(int));
    int labelCount = LabelImage(img->largura, img->altura, img->pixels, output);
    
    printf("Found %d components\n", labelCount);
    
    // Clean up
    free(output);
    free(img->pixels);
    free(img->tipo);
    free(img->head);
    free(img);
    
    return 0;
}
```

## Algorithm

The program uses a recursive flood-filling algorithm to identify connected components in binary images. It then uses a similar algorithm to identify holes within these components. The output is a PGM image where each component and hole is assigned a different gray level.

The workflow consists of:
1. Reading and parsing input binary (PBM) images
2. Applying connected component labeling to identify distinct objects
3. Processing holes detection within components
4. Generating grayscale (PGM) or color (PPM) output with labeled components
5. Computing statistics on components and holes

## License

This project is licensed under the MIT License - see the LICENSE file for details.