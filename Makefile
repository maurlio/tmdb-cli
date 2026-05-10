# Compilador e flags
CC = gcc
CFLAGS = -std=c17 -Wall -Wextra -pedantic -Werror -I./include
LDFLAGS = -lcurl -lcjson

# Diretórios
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

# Arquivos
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))
TARGET = tmdb-cli

# Target padrão
.PHONY: all
all: release

# Build de produção (Otimizado)
.PHONY: release
release: CFLAGS += -O3
release: $(TARGET)

# Build de desenvolvimento (Debug)
.PHONY: debug
debug: CFLAGS += -g -O0
debug: $(TARGET)

# Build com Address Sanitizer para detectar vazamentos de memória
.PHONY: sanitizer
sanitizer: CFLAGS += -g -O1 -fsanitize=address,undefined
sanitizer: LDFLAGS += -fsanitize=address,undefined
sanitizer: clean $(TARGET)

# Linkagem final (Gera o executável na raiz)
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Compilação dos objetos (Gera os .o na pasta build)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Formatação de código
.PHONY: format
format:
	clang-format -i $(SRC_DIR)/*.c $(INC_DIR)/*.h

# Limpeza dos artefatos
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Executar a aplicação
.PHONY: run
run: release
	./$(TARGET)
