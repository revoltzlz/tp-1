#include <stdio.h>
#include <stdlib.h>

#include "pokelista.h"

int pokelistaInicializar(Pokelista *pl)
{
    // a celula cabeca nao guarda pokemon: ela existe para que o primeiro elemento tambem tenha um antecessor, e a remocao nao precise de um caso especial
    pl->primeiro = (Pokecelula *) malloc(sizeof(Pokecelula));
    
    if (pl->primeiro == NULL) {
        pl->ultimo = NULL;
        pl->tamanho = 0;
        return 0;
    }

    pl->primeiro->prox = NULL;

    // lista vazia: primeiro e ultimo apontam para a mesma celula, a cabeca
    pl->ultimo = pl->primeiro;
    pl->tamanho = 0;

    return 1;
}

int pokelistaInserir(Pokelista *pl, const Pokemon *p)
{
    Pokecelula *nova;

    nova = (Pokecelula *) malloc(sizeof(Pokecelula));
    if (nova == NULL) {
        return 0;
    }

    // copia a struct inteira: a lista fica dona da sua propria copia
    nova->pokemon = *p;
    nova->prox = NULL;

    pl->ultimo->prox = nova;
    pl->ultimo = nova;
    pl->tamanho++;

    return 1;
}

int pokelistaRemover(Pokelista *pl, int id, Pokemon *removido)
{
    Pokecelula *anterior;
    Pokecelula *alvo;

    // anda com um apontador para a celula anterior ao alvo: para desligar uma celula e preciso ter quem aponta para ela tendo comecado na cabeca
    anterior = pl->primeiro;
    while (anterior->prox != NULL && pokemonGetId(&anterior->prox->pokemon) != id) {
        anterior = anterior->prox;
    }

    // chegou ao fim sem achar
    if (anterior->prox == NULL) {
        return 0;
    }

    alvo = anterior->prox;

    if (removido != NULL) {
        *removido = alvo->pokemon;
    }

    // desliga o alvo da corrente
    anterior->prox = alvo->prox;

    // sem esta correcao, ultimo ficaria apontando para memoria liberada e a proxima insercao escreveria nela
    if (alvo == pl->ultimo) {
        pl->ultimo = anterior;
    }

    free(alvo);
    pl->tamanho--;

    return 1;
}

int pokelistaRemoverPrimeiro(Pokelista *pl, Pokemon *removido)
{
    if (pokelistaVazia(pl)) {
        return 0;
    }

    // reaproveita a remocao por id, para haver um unico algoritmo de remocao. o primeiro pokemon de verdade fica na celula seguinte a cabeca
    return pokelistaRemover(pl, pokemonGetId(&pl->primeiro->prox->pokemon), removido);
}

int pokelistaBuscar(const Pokelista *pl, int id, Pokemon *encontrado)
{
    Pokecelula *atual;

    atual = pl->primeiro->prox;
    while (atual != NULL) {
        if (pokemonGetId(&atual->pokemon) == id) {
            if (encontrado != NULL) {
                *encontrado = atual->pokemon;
            }
            return 1;
        }
        atual = atual->prox;
    }

    return 0;
}

void pokelistaImprimir(const Pokelista *pl)
{
    Pokecelula *atual;

    if (pokelistaVazia(pl)) {
        printf("(nenhum Pokemon na lista)\n");
        return;
    }

    atual = pl->primeiro->prox;
    while (atual != NULL) {
        pokemonImprimir(&atual->pokemon);
        atual = atual->prox;
    }
}

void pokelistaEscreverRelatorio(const Pokelista *pl, FILE *saida)
{
    Pokecelula *atual;

    atual = pl->primeiro->prox;
    while (atual != NULL) {
        fprintf(saida, "%03d %s\n", pokemonGetNumPokedex(&atual->pokemon),
                pokemonGetNome(&atual->pokemon));
        atual = atual->prox;
    }
}

int pokelistaVazia(const Pokelista *pl)
{
    // vazia quando a unica celula e a cabeca
    return pl->primeiro == pl->ultimo;
}

int pokelistaGetTamanho(const Pokelista *pl)
{
    return pl->tamanho;
}

void pokelistaLiberar(Pokelista *pl)
{
    Pokecelula *atual;
    Pokecelula *seguinte;

    // comeca na cabeca, que tambem foi alocada, o prox e guardado antes do free, senao seria lido de memoria ja devolvida
    atual = pl->primeiro;
    while (atual != NULL) {
        seguinte = atual->prox;
        free(atual);
        atual = seguinte;
    }

    // anula os apontadores: um uso acidental depois disto falha de imediato
    pl->primeiro = NULL;
    pl->ultimo = NULL;
    pl->tamanho = 0;
}
