# TP1 - Algoritmos e Estruturas de Dados I (CCF211) - UFV Campus Florestal
# Resgate de Pokemon com listas encadeadas
#
# COMO COMPILAR
#
#   Linux ou Git Bash:    make
#   Windows (MinGW):      mingw32-make
#
# A compilacao tem duas etapas. Primeiro cada arquivo .c vira um arquivo-objeto
# .o separado, compilado com -Iinclude para o gcc achar os cabecalhos dentro da
# pasta include/, com os avisos -Wall -Wextra ligados e o padrao da linguagem
# fixado em C99. Depois os seis .o sao ligados em um unico executavel. A
# biblioteca matematica (-lm) entra depois dos objetos, porque o ligador
# resolve os simbolos da esquerda para a direita e e o nosso codigo que chama
# sqrt.
#
# COMO EXECUTAR
#
#   Linux ou Git Bash:    ./tp1
#   Windows (PowerShell): .\tp1.exe
#
# O programa abre um menu: a opcao 1 le os dados de um arquivo, a opcao 2 pede
# os dados pelo teclado e a opcao 0 encerra. Ha arquivos de entrada de exemplo
# na pasta testes/.
#
# COMO LIMPAR
#
#   make clean            (ou mingw32-make clean no Windows)

CC     = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude
LDLIBS = -lm

ALVO = tp1
OBJS = main.o pokemon.o pokelista.o treinador.o pokecenter.o missao.o

# all e clean sao nomes de tarefa, e nao de arquivo.
.PHONY: all clean

all: $(ALVO)

$(ALVO): $(OBJS)
	$(CC) $(CFLAGS) -o $(ALVO) $(OBJS) $(LDLIBS)

# Cada regra lista os arquivos de que o .o depende: o proprio .c e os
# cabecalhos que ele inclui, direta ou indiretamente. Assim, mudar um .h
# recompila so o que precisa.
main.o: main.c include/missao.h include/pokecenter.h include/treinador.h \
        include/pokelista.h include/conexao.h include/pokemon.h include/coordenadas.h
	$(CC) $(CFLAGS) -c main.c -o main.o

pokemon.o: src/pokemon.c include/pokemon.h include/coordenadas.h
	$(CC) $(CFLAGS) -c src/pokemon.c -o pokemon.o

pokelista.o: src/pokelista.c include/pokelista.h include/conexao.h \
             include/pokemon.h include/coordenadas.h
	$(CC) $(CFLAGS) -c src/pokelista.c -o pokelista.o

treinador.o: src/treinador.c include/treinador.h include/pokelista.h \
             include/conexao.h include/pokemon.h include/coordenadas.h
	$(CC) $(CFLAGS) -c src/treinador.c -o treinador.o

pokecenter.o: src/pokecenter.c include/pokecenter.h include/treinador.h \
              include/pokelista.h include/conexao.h include/pokemon.h \
              include/coordenadas.h
	$(CC) $(CFLAGS) -c src/pokecenter.c -o pokecenter.o

missao.o: src/missao.c include/missao.h include/pokecenter.h include/treinador.h \
          include/pokelista.h include/conexao.h include/pokemon.h include/coordenadas.h
	$(CC) $(CFLAGS) -c src/missao.c -o missao.o

# O comando de apagar depende do shell: no Linux e no Git Bash existe "rm", no
# cmd e no PowerShell existe "del". Os dois sao tentados, e o "-" na frente
# manda o make seguir em frente com o que nao existir neste sistema.
clean:
	-rm -f $(ALVO) $(ALVO).exe *.o
	-del /Q /F $(ALVO).exe *.o
