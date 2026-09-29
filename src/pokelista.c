#include <stdio.h>
#include <stdlib.h>

#include "pokelista.h"

int pokelistaInicializar(Pokelista *pl)
{
    /* A celula cabeca e uma celula sem Pokemon, alocada uma unica vez. Ela
       existe para que o primeiro Pokemon da lista tambem tenha um antecessor,
       o que faz a remocao nao precisar de um caso especial para o primeiro
       elemento. */
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

    /* Copia do Pokemon para dentro da celula. A celula guarda o Pokemon por
       valor, e nao um apontador para ele, entao a lista fica dona da sua
       propria copia e nenhuma memoria e compartilhada com quem chamou. */
    nova->pokemon = *p;
    nova->prox = NULL;

    /* Insercao no fim em O(1): o apontador ultimo evita percorrer a lista. */
    pl->ultimo->prox = nova;
    pl->ultimo = nova;
    pl->tamanho++;

    return 1;
}

int pokelistaRemover(Pokelista *pl, int id, Pokemon *removido)
{
    conec *anterior;
    conec *alvo;

    /* Percorre com um apontador para a celula ANTERIOR ao alvo, porque em uma
       lista encadeada simples nao se volta: para desligar uma celula e preciso
       ter em maos quem aponta para ela. Comeca na celula cabeca, que e a
       anterior ao primeiro Pokemon de verdade. */
    anterior = pl->primeiro;
    while (anterior->prox != NULL && pokemonGetId(&anterior->prox->pokemon) != id) {
        anterior = anterior->prox;
    }

    /* Chegou ao fim sem achar: nao existe Pokemon com esse id na lista. */
    if (anterior->prox == NULL) {
        return 0;
    }

    alvo = anterior->prox;

    if (removido != NULL) {
        *removido = alvo->pokemon;
    }

    /* Desliga o alvo da corrente. */
    anterior->prox = alvo->prox;

    /* Se o alvo era a ultima celula, o apontador ultimo passa a ser o
       anterior. Sem esta linha, ultimo ficaria apontando para memoria
       liberada e a proxima insercao escreveria nela. */
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

    /* Reaproveita a remocao por id, para que exista um unico algoritmo de
       remocao no TAD: pega o id do primeiro Pokemon de verdade, que fica na
       celula seguinte a cabeca, e remove por ele. */
    return pokelistaRemover(pl, pokemonGetId(&pl->primeiro->prox->pokemon), removido);
}

int pokelistaBuscar(const Pokelista *pl, int id, Pokemon *encontrado)
{
    conec *atual;

    /* Comeca no primeiro Pokemon de verdade, que e o seguinte a celula cabeca.
       O custo e O(n) porque a unica forma de andar na lista encadeada e seguir
       os apontadores prox, um por um. */
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
    /* A lista esta vazia quando a unica celula e a cabeca, ou seja, quando
       primeiro e ultimo apontam para o mesmo lugar. */
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

    /* Comeca na celula cabeca, para que ela tambem seja liberada. Guarda o
       endereco da celula seguinte ANTES do free, senao o apontador prox seria
       lido de memoria que acabou de ser devolvida. */
    atual = pl->primeiro;
    while (atual != NULL) {
        seguinte = atual->prox;
        free(atual);
        atual = seguinte;
    }

    /* Anula os apontadores para que um uso acidental da lista depois desta
       chamada falhe de imediato, em vez de mexer em memoria liberada. */
    pl->primeiro = NULL;
    pl->ultimo = NULL;
    pl->tamanho = 0;
}
