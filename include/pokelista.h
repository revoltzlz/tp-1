#ifndef POKELISTA_H
#define POKELISTA_H

#include <stdio.h>

#include "pokemon.h"

// pokelista: lista encadeada de pokemon

typedef struct pokecelula {
   Pokemon pokemon;
   struct pokecelula *prox;
} Pokecelula;

typedef struct {
   Pokecelula *primeiro;
   Pokecelula *ultimo;
   int tamanho;
} Pokelista;

// Inicializa a pokelista e devolve 0 se o malloc falhar
int pokelistaInicializar(Pokelista *pl);

// insere uma copia do pokemon no fim e devolve 0 se o malloc falhar
int pokelistaInserir(Pokelista *pl, const Pokemon *p);

// remove o pokemon de identificacao id, copiando-o para *removido se este nao for null e devolve 1 se encontrou e removeu, 0 se nao existe
int pokelistaRemover(Pokelista *pl, int id, Pokemon *removido);

// remove o primeiro pokemon e o copia para *removido e devolve 0 se a lista estava vazia e o que faz a entrega sair na ordem de captura
int pokelistaRemoverPrimeiro(Pokelista *pl, Pokemon *removido);

// procura o pokemon de identificacao id, copiando-o para *encontrado se este nao for null e devolve 1 se encontrou e 0 se nao
int pokelistaBuscar(const Pokelista *pl, int id, Pokemon *encontrado);

// imprime todos os pokemon e se a lista estiver vazia e avisado
void pokelistaImprimir(const Pokelista *pl);

// escreve no arquivo ja aberto uma linha "<numpokedex> <nome>" por pokemon
void pokelistaEscreverRelatorio(const Pokelista *pl, FILE *saida);

// devolve 1 se a lista nao tem nenhum pokemon
int pokelistaVazia(const Pokelista *pl);

// devolve a quantidade de pokemon armazenados
int pokelistaGetTamanho(const Pokelista *pl);

// libera toda a pokelista
void pokelistaLiberar(Pokelista *pl);

#endif
