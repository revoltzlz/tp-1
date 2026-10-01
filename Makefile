# TP1 - AEDS I (CCF211) - Resgate de Pokemon
#
# Para compilar:  make            (no Windows: mingw32-make)
# Para executar:  make run        (ou ./tp1)
# Para limpar:    make clean

# Compilador e flags
CC     = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -Iinclude
LDLIBS = -lm

# Diretorios
SRC_DIR = src
BIN     = tp1

# Arquivos fonte, objetos e cabecalhos
SRCS = main.c \
       $(SRC_DIR)/pokemon.c \
       $(SRC_DIR)/pokelista.c \
       $(SRC_DIR)/treinador.c \
       $(SRC_DIR)/pokecenter.c \
       $(SRC_DIR)/missao.c

OBJS = main.o pokemon.o pokelista.o treinador.o pokecenter.o missao.o

HDRS = include/coordenadas.h include/pokemon.h \
       include/pokelista.h include/treinador.h include/pokecenter.h \
       include/missao.h

.PHONY: all run clean

# Regra padrao
all: $(BIN)

# Linkagem final
$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJS) $(LDLIBS)

# Compilacao dos .c
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS): $(HDRS)

# Executar
run: $(BIN)
	./$(BIN)

# Limpar arquivos compilados (rm no Linux/Git Bash, del no Windows)
clean:
	-rm -f $(BIN) $(BIN).exe $(OBJS)
	-del /Q /F $(BIN).exe $(OBJS)
