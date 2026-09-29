#ifndef CONTA_MEMORIA_H
#define CONTA_MEMORIA_H

/* Verificacao de memoria sem valgrind.
 *
 * Este arquivo NAO faz parte do programa entregue: ele nao e incluido por
 * nenhum .c nem .h do projeto. Ele e injetado de fora, so no build de
 * verificacao, pela opcao -include do gcc:
 *
 *     gcc -Wall -Wextra -std=c99 -Iinclude -c src/pokelista.c \
 *         -include testes/conta_memoria.h -o pokelista.o
 *
 * Toda a memoria dinamica do projeto e alocada e liberada em src/pokelista.c,
 * entao interceptar esse unico arquivo cobre o programa inteiro.
 *
 * As duas macros do fim do arquivo trocam malloc e free por funcoes que
 * contam as chamadas antes de repassar para as originais. Elas sao definidas
 * DEPOIS das funcoes, de proposito: se viessem antes, o malloc de dentro de
 * cm_malloc tambem seria substituido e a funcao chamaria a si mesma para
 * sempre.
 *
 * Ao terminar o programa, a funcao registrada com atexit imprime os dois
 * contadores em stderr. Se a diferenca for zero, cada bloco alocado foi
 * liberado uma vez.
 */

#include <stdio.h>
#include <stdlib.h>

static long cmAlocacoes = 0;
static long cmLiberacoes = 0;
static int cmRegistrado = 0;

static void cmRelatorio(void)
{
    fprintf(stderr, "[conta_memoria] malloc bem-sucedidos:      %ld\n", cmAlocacoes);
    fprintf(stderr, "[conta_memoria] free de ponteiro nao nulo: %ld\n", cmLiberacoes);
    fprintf(stderr, "[conta_memoria] diferenca (deve ser 0):    %ld\n",
            cmAlocacoes - cmLiberacoes);
}

static void *cmMalloc(size_t bytes)
{
    void *bloco;

    if (!cmRegistrado) {
        cmRegistrado = 1;
        atexit(cmRelatorio);
    }

    bloco = malloc(bytes);
    if (bloco != NULL) {
        cmAlocacoes++;
    }

    return bloco;
}

static void cmFree(void *bloco)
{
    if (bloco != NULL) {
        cmLiberacoes++;
    }

    free(bloco);
}

#define malloc(bytes) cmMalloc(bytes)
#define free(bloco) cmFree(bloco)

#endif
