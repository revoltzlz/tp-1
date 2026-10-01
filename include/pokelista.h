// tad pokelista: lista encadeada de pokemon, com celula cabeca (ziviani 2.1.2)

#ifndef POKELISTA_H
#define POKELISTA_H

#include <stdio.h>

#include "pokemon.h"

// o pokemon e guardado por valor, e nao por ponteiro: assim cada lista tem a sua propria copia e nenhuma memoria e compartilhada entre duas listas. o nome struct pokecelula e obrigatorio: o campo prox aponta para o proprio tipo, e dentro do typedef o nome pokecelula ainda nao existe
typedef struct pokecelula {
   Pokemon pokemon;
   struct pokecelula *prox;
} Pokecelula;

// primeiro aponta para a celula cabeca, que nao guarda pokemon: o primeiro pokemon de verdade fica em primeiro->prox, e a lista esta vazia quando primeiro == ultimo. o apontador ultimo faz a insercao no fim custar o(1)
typedef struct {
   Pokecelula *primeiro;
   Pokecelula *ultimo;
   int tamanho;
} Pokelista;

// cria a celula cabeca e deixa a lista vazia. devolve 0 se o malloc falhar
int pokelistaInicializar(Pokelista *pl);

// insere uma copia do pokemon no fim, em o(1). devolve 0 se o malloc falhar
int pokelistaInserir(Pokelista *pl, const Pokemon *p);

// remove o pokemon de identificacao id, copiando-o para *removido se este nao for null. devolve 1 se encontrou e removeu, 0 se nao existe. custa o(n)
int pokelistaRemover(Pokelista *pl, int id, Pokemon *removido);

// remove o primeiro pokemon e o copia para *removido. devolve 0 se a lista estava vazia. e o que faz a entrega sair na ordem de captura
int pokelistaRemoverPrimeiro(Pokelista *pl, Pokemon *removido);

// procura o pokemon de identificacao id, copiando-o para *encontrado se este nao for null. devolve 1 se encontrou, 0 se nao. custa o(n)
int pokelistaBuscar(const Pokelista *pl, int id, Pokemon *encontrado);

// imprime todos os pokemon, um por linha. se a lista estiver vazia, avisa
void pokelistaImprimir(const Pokelista *pl);

// escreve no arquivo ja aberto uma linha "<numpokedex> <nome>" por pokemon
void pokelistaEscreverRelatorio(const Pokelista *pl, FILE *saida);

// devolve 1 se a lista nao tem nenhum pokemon
int pokelistaVazia(const Pokelista *pl);

// devolve a quantidade de pokemon armazenados
int pokelistaGetTamanho(const Pokelista *pl);

// libera todas as celulas, inclusive a cabeca, e anula os apontadores. depois disto a lista precisa ser inicializada de novo
void pokelistaLiberar(Pokelista *pl);

#endif
