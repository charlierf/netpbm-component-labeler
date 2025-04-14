# NetPBM Component Labeler

A tool for labeling and analyzing connected components in NetPBM images (PBM, PGM, PPM).

## Features

- Reads PBM (P1) binary images
- Labels all connected components in an image
- Detects and counts holes in components 
- Outputs a PGM image with different gray levels for each component and hole
- Provides statistics on number of objects, holes, and objects with holes

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
./bin/netpbm-component-labeler examples/input/test.pbm
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
│  └── algorithms/
│     └── connected_components.c # Component labeling and analysis algorithms
├── include/
│  ├── image_formats.h           # Header for image format functions
│  └── algorithms.h              # Header for algorithms
├── examples/                    # Example images
│  ├── input/
│  └── output/
├── Makefile                     # Build configuration
├── README.md                    # This file (English)
└── README.pt-BR.md              # Portuguese version of this file
```

## Algorithm

The program uses a recursive flood-filling algorithm to identify connected components in binary images. It then uses a similar algorithm to identify holes within these components. The output is a PGM image where each component and hole is assigned a different gray level.

## License

This project is licensed under the MIT License - see the LICENSE file for details.