#ifndef POKELISTA_H
#define POKELISTA_H

#include <stdio.h>

#include "conexao.h"

/* Lista encadeada de Pokemon com celula cabeca (Ziviani, secao 2.1.2).

   O campo primeiro aponta para uma celula que nao guarda Pokemon nenhum: o
   primeiro Pokemon de verdade fica em primeiro->prox. Isso elimina o caso
   especial "remover o primeiro elemento", porque todo elemento tem um
   antecessor. A lista esta vazia quando primeiro == ultimo.

   O campo ultimo aponta para a ultima celula e faz a insercao no fim custar
   O(1), sem percorrer a lista. */
typedef struct {
    conec *primeiro;
    conec *ultimo;
    int tamanho;
} Pokelista;

/* Cria a celula cabeca e deixa a lista vazia. Devolve 1 em caso de sucesso e
   0 se o malloc da celula cabeca falhar. */
int pokelistaInicializar(Pokelista *pl);

/* Insere uma copia do Pokemon no fim da lista, em O(1). Devolve 1 em caso de
   sucesso e 0 se o malloc da nova celula falhar. */
int pokelistaInserir(Pokelista *pl, const Pokemon *p);

/* Remove da lista o Pokemon de identificacao id. Se removido nao for NULL,
   copia para lá o Pokemon retirado antes de liberar a celula. Devolve 1 se
   encontrou e removeu, 0 se nao existe Pokemon com esse id. */
int pokelistaRemover(Pokelista *pl, int id, Pokemon *removido);

/* Remove o primeiro Pokemon da lista e o copia para *removido. Devolve 1 em
   caso de sucesso e 0 se a lista estava vazia. Usada na entrega ao Centro de
   Pesquisa, para que os Pokemon saiam na ordem em que foram capturados. */
int pokelistaRemoverPrimeiro(Pokelista *pl, Pokemon *removido);

/* Procura o Pokemon de identificacao id. Se encontrar e encontrado nao for
   NULL, copia o Pokemon para *encontrado. Devolve 1 se encontrou, 0 se nao.
   Custa O(n): a lista encadeada so permite chegar a um elemento andando pelos
   apontadores desde o comeco. */
int pokelistaBuscar(const Pokelista *pl, int id, Pokemon *encontrado);

/* Imprime no terminal todos os Pokemon armazenados, na ordem em que estao
   encadeados, um por linha. Se a lista estiver vazia, avisa. */
void pokelistaImprimir(const Pokelista *pl);

/* Escreve no arquivo ja aberto uma linha "<numPokedex> <nome>" para cada
   Pokemon da lista, na ordem em que estao encadeados. Quem sabe percorrer a
   lista e a propria lista; o Centro de Pesquisa so abre e fecha o arquivo. */
void pokelistaEscreverRelatorio(const Pokelista *pl, FILE *saida);

/* Devolve 1 se a lista nao tem nenhum Pokemon e 0 caso contrario. */
int pokelistaVazia(const Pokelista *pl);

/* Devolve a quantidade de Pokemon armazenados. */
int pokelistaGetTamanho(const Pokelista *pl);

/* Libera todas as celulas da lista, inclusive a celula cabeca, e anula os
   apontadores. Depois desta chamada a lista precisa ser inicializada de novo
   antes de qualquer outro uso. */
void pokelistaLiberar(Pokelista *pl);

#endif
