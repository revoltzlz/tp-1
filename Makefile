# ============================================================================
# TP1 - Algoritmos e Estruturas de Dados I (CCF211) - UFV Campus Florestal
# Resgate de Pokemon com listas encadeadas
# ============================================================================
#
# COMO COMPILAR
#
#   Linux / WSL / Git Bash:   make
#   Windows (MinGW):          mingw32-make
#
# O que a compilacao faz, em duas etapas:
#
#   1. Cada modulo .c vira um arquivo-objeto .o separado, compilado com
#      -Iinclude para que o gcc ache os cabecalhos dentro da pasta include/.
#      Os avisos -Wall -Wextra ficam ligados e o padrao da linguagem e fixado
#      em C99 (-std=c99). O projeto compila com zero avisos.
#   2. Os .o sao ligados em um unico executavel. A biblioteca matematica
#      (-lm) entra DEPOIS dos objetos, porque o ligador resolve os simbolos
#      da esquerda para a direita e e o nosso codigo que chama sqrt().
#
# COMO EXECUTAR
#
#   Linux / Git Bash:         ./tp1
#   Windows (PowerShell):     .\tp1.exe
#   Windows (cmd):            tp1.exe
#
#   O programa abre um menu: opcao 1 le os dados de um arquivo, opcao 2 pede
#   os dados pelo teclado e opcao 0 encerra. Exemplo de arquivo de entrada:
#   testes/oficiais/teste1.txt
#
# COMO LIMPAR
#
#   make clean      (ou mingw32-make clean no Windows)
#
#   Remove o executavel e todos os arquivos-objeto .o. Funciona nos dois
#   sistemas: a variavel OS so existe no Windows, e e ela que escolhe entre
#   o comando del e o comando rm.
#
# ============================================================================

# ---- Ferramentas e opcoes ---------------------------------------------------

CC      = gcc
CFLAGS  = -Wall -Wextra -std=c99 -Iinclude
LDLIBS  = -lm

ALVO    = tp1
OBJS    = main.o pokemon.o pokelista.o treinador.o pokecenter.o missao.o

# ---- Comando de limpeza, escolhido conforme o sistema -----------------------
# A variavel de ambiente OS vale Windows_NT apenas no Windows.

ifeq ($(OS),Windows_NT)
    LIMPAR = del /Q /F $(ALVO).exe *.o
else
    LIMPAR = rm -f $(ALVO) *.o
endif

# ---- Alvos ------------------------------------------------------------------
# .PHONY marca os alvos que sao nomes de tarefa, e nao de arquivo, para que o
# make os execute mesmo que apareca um arquivo com esse nome na pasta.

.PHONY: all clean

all: $(ALVO)

# Ligacao final: junta os objetos e a biblioteca matematica no executavel.
$(ALVO): $(OBJS)
	$(CC) $(CFLAGS) -o $(ALVO) $(OBJS) $(LDLIBS)

# Compilacao do programa principal, que fica na raiz do projeto.
main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

# Regra geral: qualquer alvo x.o e obtido compilando src/x.c.
# O $< e o primeiro pre-requisito (o .c) e o $@ e o alvo (o .o).
%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Dependencias de cabecalho: se um .h muda, os .o que o incluem sao
# recompilados. Cada TAD depende do seu proprio cabecalho e dos TADs abaixo
# dele na hierarquia pokemon -> pokelista -> treinador -> pokecenter.
main.o:      include/missao.h include/pokecenter.h
missao.o:    include/missao.h include/pokecenter.h include/treinador.h include/pokelista.h include/conexao.h include/pokemon.h include/coordenadas.h
pokecenter.o: include/pokecenter.h include/treinador.h include/pokelista.h include/conexao.h include/pokemon.h include/coordenadas.h
treinador.o: include/treinador.h include/pokelista.h include/conexao.h include/pokemon.h include/coordenadas.h
pokelista.o: include/pokelista.h include/conexao.h include/pokemon.h include/coordenadas.h
pokemon.o:   include/pokemon.h include/coordenadas.h

clean:
	-$(LIMPAR)
