# Rotulador de Componentes NetPBM

Uma ferramenta para rotular e analisar componentes conexos em imagens NetPBM (PBM, PGM, PPM).

## Funcionalidades

- Leitura de imagens binárias PBM (P1)
- Rotulação de todos os componentes conexos em uma imagem
- Detecção e contagem de furos nos componentes
- Geração de uma imagem PGM com diferentes níveis de cinza para cada componente e furo
- Fornecimento de estatísticas sobre o número de objetos, furos e objetos com furos

## Requisitos

- Compilador C (recomendado GCC)
- Make

## Compilando o Projeto

Clone o repositório e compile o projeto:

```bash
git clone https://github.com/seunome/netpbm-component-labeler.git
cd netpbm-component-labeler
make
```

## Uso

```bash
./bin/netpbm-component-labeler imagem_entrada.pbm
```

O programa irá gerar um arquivo chamado `saida.ppm` no diretório atual, mostrando os componentes rotulados.

## Exemplo

```bash
./bin/netpbm-component-labeler examples/input/teste.pbm
```

## Estrutura do Projeto

```
netpbm-component-labeler/
├── src/
│  ├── main.c                    # Código principal do programa
│  ├── image_formats/
│  │  ├── pbm_handler.c          # Funções para manipulação de imagens PBM
│  │  ├── pgm_handler.c          # Funções para manipulação de imagens PGM
│  │  └── ppm_handler.c          # Funções para manipulação de imagens PPM
│  └── algorithms/
│     └── connected_components.c # Algoritmos de rotulação e análise de componentes
├── include/
│  ├── image_formats.h           # Cabeçalho para funções de formatos de imagem
│  └── algorithms.h              # Cabeçalho para algoritmos
├── examples/                    # Imagens de exemplo
│  ├── input/
│  └── output/
├── Makefile                     # Configuração de compilação
├── README.md                    # Versão em inglês deste arquivo
└── README.pt-BR.md              # Este arquivo (português)
```

## Algoritmo

O programa usa um algoritmo recursivo de preenchimento por inundação (flood fill) para identificar componentes conexos em imagens binárias. Em seguida, utiliza um algoritmo semelhante para identificar furos dentro desses componentes. A saída é uma imagem PGM onde cada componente e furo recebe um nível de cinza diferente.

## Licença

Este projeto está licenciado sob a Licença MIT - veja o arquivo LICENSE para detalhes.