# tp1 - aeds i (ccf211) - resgate de pokemon
#
# para compilar:  make            (no windows: mingw32-make)
# para executar:  make run        (ou ./tp1)
# para limpar:    make clean

# compilador e flags
CC     = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -Iinclude
LDLIBS = -lm

# diretorios
SRC_DIR = src
BIN     = tp1

# arquivos fonte, objetos e cabecalhos
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

# regra padrao
all: $(BIN)

# linkagem final
$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJS) $(LDLIBS)

# compilacao dos .c
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS): $(HDRS)

# executar
run: $(BIN)
	./$(BIN)

# limpar arquivos compilados (rm no linux/git bash, del no windows)
clean:
	-rm -f $(BIN) $(BIN).exe $(OBJS)
	-del /Q /F $(BIN).exe $(OBJS)
