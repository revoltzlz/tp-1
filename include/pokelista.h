/* TAD PokeLista: lista encadeada de Pokemon, com celula cabeca (Ziviani 2.1.2). */

#ifndef POKELISTA_H
#define POKELISTA_H

#include <stdio.h>

#include "pokemon.h"

/* O Pokemon e guardado por valor, e nao por ponteiro: assim cada lista tem a
   sua propria copia e nenhuma memoria e compartilhada entre duas listas.

   O nome struct conec e obrigatorio: o campo prox aponta para o proprio tipo,
   e dentro do typedef o nome conec ainda nao existe. */
typedef struct pokecelula {
   Pokemon pokemon;
   struct pokecelula *prox;
} Pokecelula;

/* primeiro aponta para a celula cabeca, que nao guarda Pokemon: o primeiro
   Pokemon de verdade fica em primeiro->prox, e a lista esta vazia quando
   primeiro == ultimo. O apontador ultimo faz a insercao no fim custar O(1). */
typedef struct {
   Pokecelula *primeiro;
   Pokecelula *ultimo;
   int tamanho;
} Pokelista;

/* Cria a celula cabeca e deixa a lista vazia. Devolve 0 se o malloc falhar. */
int pokelistaInicializar(Pokelista *pl);

/* Insere uma copia do Pokemon no fim, em O(1). Devolve 0 se o malloc falhar. */
int pokelistaInserir(Pokelista *pl, const Pokemon *p);

/* Remove o Pokemon de identificacao id, copiando-o para *removido se este nao
   for NULL. Devolve 1 se encontrou e removeu, 0 se nao existe. Custa O(n). */
int pokelistaRemover(Pokelista *pl, int id, Pokemon *removido);

/* Remove o primeiro Pokemon e o copia para *removido. Devolve 0 se a lista
   estava vazia. E o que faz a entrega sair na ordem de captura. */
int pokelistaRemoverPrimeiro(Pokelista *pl, Pokemon *removido);

/* Procura o Pokemon de identificacao id, copiando-o para *encontrado se este
   nao for NULL. Devolve 1 se encontrou, 0 se nao. Custa O(n). */
int pokelistaBuscar(const Pokelista *pl, int id, Pokemon *encontrado);

/* Imprime todos os Pokemon, um por linha. Se a lista estiver vazia, avisa. */
void pokelistaImprimir(const Pokelista *pl);

/* Escreve no arquivo ja aberto uma linha "<numPokedex> <nome>" por Pokemon. */
void pokelistaEscreverRelatorio(const Pokelista *pl, FILE *saida);

/* Devolve 1 se a lista nao tem nenhum Pokemon. */
int pokelistaVazia(const Pokelista *pl);

/* Devolve a quantidade de Pokemon armazenados. */
int pokelistaGetTamanho(const Pokelista *pl);

/* Libera todas as celulas, inclusive a cabeca, e anula os apontadores. Depois
   disto a lista precisa ser inicializada de novo. */
void pokelistaLiberar(Pokelista *pl);

#endif
