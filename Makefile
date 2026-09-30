# TP1 - Algoritmos e Estruturas de Dados I (CCF211) - UFV Campus Florestal
# Resgate de Pokemon com listas encadeadas
#
# Escrito na forma do 2o modelo do documento "Makefile" da disciplina: as
# variaveis CC, CFLAGS, SRC_DIR, BIN, SRCS e OBJS, os alvos all, run e clean, e
# uma regra de padrao com $< e $@ no lugar de uma regra por arquivo.
#
# COMO COMPILAR
#
#   Linux ou Git Bash:    make
#   Windows (MinGW):      mingw32-make
#
# COMO EXECUTAR
#
#   make run              (ou apenas ./tp1, ou .\tp1.exe no PowerShell)
#
# O programa abre um menu: a opcao 1 le os dados de um arquivo, a opcao 2 pede
# os dados pelo teclado e a opcao 0 encerra. Ha arquivos de entrada de exemplo
# na pasta testes/, e os dois oficiais em testes/oficiais/.
#
# COMO LIMPAR
#
#   make clean

# Compilador e flags
#
# O modelo da disciplina usa "-Wall -Iinclude". Os tres acrescimos:
#   -Wextra   liga avisos que o -Wall nao cobre, como parametro nao usado
#   -std=c99  fixa a versao da linguagem, para o codigo compilar igual em
#             qualquer maquina em vez de depender do padrao do compilador
#   -g        guarda as informacoes de depuracao que o gdb e o valgrind usam,
#             como o proprio documento da disciplina recomenda
CC     = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -Iinclude

# A biblioteca matematica entra depois dos objetos, porque o ligador resolve os
# simbolos da esquerda para a direita e e o nosso codigo que chama sqrt.
LDLIBS = -lm

# Diretorios e nome do executavel
SRC_DIR = src
BIN     = tp1

# Arquivos fonte, objetos e cabecalhos
#
# O main.c fica na raiz, e nao em src/, porque a especificacao pede o programa
# principal separado dos TADs.
SRCS = main.c \
       $(SRC_DIR)/pokemon.c \
       $(SRC_DIR)/pokelista.c \
       $(SRC_DIR)/treinador.c \
       $(SRC_DIR)/pokecenter.c \
       $(SRC_DIR)/missao.c

OBJS = main.o pokemon.o pokelista.o treinador.o pokecenter.o missao.o

HDRS = include/coordenadas.h include/pokemon.h include/conexao.h \
       include/pokelista.h include/treinador.h include/pokecenter.h \
       include/missao.h

# all, run e clean sao nomes de tarefa, e nao de arquivo.
.PHONY: all run clean

# Regra padrao
all: $(BIN)

# Linkagem final
$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJS) $(LDLIBS)

# Compilacao dos .c. Sao duas regras de padrao porque o main.c esta na raiz e
# os outros estao em src/. O $< e o arquivo de entrada e o $@ e o de saida.
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Qualquer cabecalho que mude recompila tudo. Sao seis arquivos: seguir a
# dependencia exata de cada um economizaria segundos e custaria seis linhas.
$(OBJS): $(HDRS)

# Executar. O programa nao recebe o arquivo pela linha de comando: ele abre um
# menu e pergunta o caminho.
run: $(BIN)
	./$(BIN)

# O comando de apagar depende do shell: no Linux e no Git Bash existe "rm", no
# cmd e no PowerShell existe "del". Os dois sao tentados, e o "-" na frente
# manda o make seguir em frente com o que nao existir neste sistema.
clean:
	-rm -f $(BIN) $(BIN).exe $(OBJS)
	-del /Q /F $(BIN).exe $(OBJS)
