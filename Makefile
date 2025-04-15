# Compilador
CC = C:\MinGW\bin\gcc.exe

# Opções de compilação
CFLAGS = -Wall -Wextra -g

# Diretórios
SRC_DIR = src
INCLUDE_DIR = include
BIN_DIR = bin

# Nome do executável
TARGET = $(BIN_DIR)\netpbm-component-labeler.exe

# Lista de todos os arquivos de origem
SRC_FILES = $(SRC_DIR)\main.c \
			$(SRC_DIR)\data_structures\data_structures.c \
            $(SRC_DIR)\image_formats\pbm_handler.c \
            $(SRC_DIR)\image_formats\pgm_handler.c \
            $(SRC_DIR)\image_formats\ppm_handler.c \
            $(SRC_DIR)\algorithms\connected_components.c

# Regra principal
all: create_dirs build

# Criar diretórios necessários
create_dirs:
	if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	if not exist $(SRC_DIR) mkdir $(SRC_DIR)
	if not exist $(SRC_DIR)\image_formats mkdir $(SRC_DIR)\image_formats
	if not exist $(SRC_DIR)\algorithms mkdir $(SRC_DIR)\algorithms
	if not exist $(SRC_DIR)\data_structures mkdir $(SRC_DIR)\data_structures
	if not exist $(INCLUDE_DIR) mkdir $(INCLUDE_DIR)

# Compilar e linkar diretamente
build:
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -I$(SRC_DIR) $(SRC_FILES) -o $(TARGET)

# Limpeza
clean:
	if exist $(BIN_DIR) rmdir /s /q $(BIN_DIR)

# Regra para rodar o programa
run: all
	$(TARGET)

# Garantir que esses alvos sejam executados mesmo se existirem arquivos com o mesmo nome
.PHONY: all clean run create_dirs build