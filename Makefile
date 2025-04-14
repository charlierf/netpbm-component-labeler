# Compilador
CC = gcc

# Opções de compilação
CFLAGS = -Wall -Wextra -g

# Diretórios
SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

# Criação de diretórios se não existirem
$(shell mkdir -p $(OBJ_DIR))
$(shell mkdir -p $(BIN_DIR))

# Arquivos de origem
SRCS = $(SRC_DIR)/main.c \
       $(SRC_DIR)/image_formats/pbm_handler.c \
       $(SRC_DIR)/image_formats/pgm_handler.c \
       $(SRC_DIR)/image_formats/ppm_handler.c \
       $(SRC_DIR)/algorithms/connected_components.c

# Objetos
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Nome do executável
TARGET = $(BIN_DIR)/netpbm-component-labeler

# Regra principal
all: $(TARGET)

# Criação do executável
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Regra para compilar arquivos .c em objetos .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Limpeza
clean:
	rm -rf $(OBJ_DIR)/* $(BIN_DIR)/*

# Regra para rodar o programa
run: $(TARGET)
	$(TARGET)

# Garantir que esses alvos sejam executados mesmo se existirem arquivos com o mesmo nome
.PHONY: all clean run