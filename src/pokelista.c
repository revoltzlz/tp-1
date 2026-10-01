#include <stdio.h>
#include <stdlib.h>

#include "pokelista.h"

int pokelistaInicializar(Pokelista *pl)
{
    /* A celula cabeca nao guarda Pokemon: ela existe para que o primeiro
       elemento tambem tenha um antecessor, e a remocao nao precise de um caso
       especial. */
    pl->primeiro = (conec *) malloc(sizeof(conec));
    if (pl->primeiro == NULL) {
        pl->ultimo = NULL;
        pl->tamanho = 0;
        return 0;
    }

    pl->primeiro->prox = NULL;

    /* Lista vazia: primeiro e ultimo apontam para a mesma celula, a cabeca. */
    pl->ultimo = pl->primeiro;
    pl->tamanho = 0;

    return 1;
}

int pokelistaInserir(Pokelista *pl, const Pokemon *p)
{
    conec *nova;

    nova = (conec *) malloc(sizeof(conec));
    if (nova == NULL) {
        return 0;
    }

    /* Copia a struct inteira: a lista fica dona da sua propria copia. */
    nova->pokemon = *p;
    nova->prox = NULL;

    /* O apontador ultimo faz isto custar O(1), sem percorrer a lista. */
    pl->ultimo->prox = nova;
    pl->ultimo = nova;
    pl->tamanho++;

    return 1;
}

int pokelistaRemover(Pokelista *pl, int id, Pokemon *removido)
{
    conec *anterior;
    conec *alvo;

    /* Anda com um apontador para a celula ANTERIOR ao alvo: para desligar uma
       celula e preciso ter quem aponta para ela. Comeca na cabeca. */
    anterior = pl->primeiro;
    while (anterior->prox != NULL && pokemonGetId(&anterior->prox->pokemon) != id) {
        anterior = anterior->prox;
    }

    /* Chegou ao fim sem achar. */
    if (anterior->prox == NULL) {
        return 0;
    }

    alvo = anterior->prox;

    if (removido != NULL) {
        *removido = alvo->pokemon;
    }

    /* Desliga o alvo da corrente. */
    anterior->prox = alvo->prox;

    /* Sem esta correcao, ultimo ficaria apontando para memoria liberada e a
       proxima insercao escreveria nela. */
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

    /* Reaproveita a remocao por id, para haver um unico algoritmo de remocao.
       O primeiro Pokemon de verdade fica na celula seguinte a cabeca. */
    return pokelistaRemover(pl, pokemonGetId(&pl->primeiro->prox->pokemon), removido);
}

int pokelistaBuscar(const Pokelista *pl, int id, Pokemon *encontrado)
{
    conec *atual;

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
    conec *atual;

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
    conec *atual;

    atual = pl->primeiro->prox;
    while (atual != NULL) {
        fprintf(saida, "%03d %s\n", pokemonGetNumPokedex(&atual->pokemon),
                pokemonGetNome(&atual->pokemon));
        atual = atual->prox;
    }
}

int pokelistaVazia(const Pokelista *pl)
{
    /* Vazia quando a unica celula e a cabeca. */
    return pl->primeiro == pl->ultimo;
}

int pokelistaGetTamanho(const Pokelista *pl)
{
    return pl->tamanho;
}

void pokelistaLiberar(Pokelista *pl)
{
    conec *atual;
    conec *seguinte;

    /* Comeca na cabeca, que tambem foi alocada. O prox e guardado ANTES do
       free, senao seria lido de memoria ja devolvida. */
    atual = pl->primeiro;
    while (atual != NULL) {
        seguinte = atual->prox;
        free(atual);
        atual = seguinte;
    }

    /* Anula os apontadores: um uso acidental depois disto falha de imediato. */
    pl->primeiro = NULL;
    pl->ultimo = NULL;
    pl->tamanho = 0;
}
