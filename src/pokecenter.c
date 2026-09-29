#include <stdio.h>
#include <stdlib.h>

#include "pokecenter.h"

int pokecenterInicializar(PokeCenter *cp)
{
    /* A especificacao fixa o Centro em (0,0). */
    cp->locPokeCenter.cordX = CENTRO_X;
    cp->locPokeCenter.cordY = CENTRO_Y;

    if (!pokelistaInicializar(&cp->fugitivos)) {
        return 0;
    }

    if (!pokelistaInicializar(&cp->recuperados)) {
        /* A primeira deu certo e a segunda nao: libera a que ja existe. */
        pokelistaLiberar(&cp->fugitivos);
        return 0;
    }

    return 1;
}

int pokecenterRegistrarFugitivo(PokeCenter *cp, const Pokemon *p)
{
    /* Insere no fim: a lista fica na ordem do arquivo de entrada. */
    return pokelistaInserir(&cp->fugitivos, p);
}

int pokecenterRemoverFugitivo(PokeCenter *cp, int id, Pokemon *removido)
{
    return pokelistaRemover(&cp->fugitivos, id, removido);
}

int pokecenterBuscarFugitivo(const PokeCenter *cp, int id, Pokemon *encontrado)
{
    return pokelistaBuscar(&cp->fugitivos, id, encontrado);
}

void pokecenterImprimirFugitivos(const PokeCenter *cp)
{
    pokelistaImprimir(&cp->fugitivos);
}

int pokecenterTemFugitivos(const PokeCenter *cp)
{
    return !pokelistaVazia(&cp->fugitivos);
}

int pokecenterGetQtdFugitivos(const PokeCenter *cp)
{
    return pokelistaGetTamanho(&cp->fugitivos);
}

int pokecenterGetQtdRecuperados(const PokeCenter *cp)
{
    return pokelistaGetTamanho(&cp->recuperados);
}

cord pokecenterGetLocalizacao(const PokeCenter *cp)
{
    return cp->locPokeCenter;
}

int pokecenterReceberPokemon(PokeCenter *cp, Treinador *t)
{
    Pokemon entregue;
    int recebidos = 0;

    /* O Pokemon viaja dentro da variavel local: sai copiado da celula do
       treinador, que e liberada, e entra copiado numa celula nova do Centro.
       Como o treinador entrega o primeiro e o Centro insere no fim, a ordem de
       captura e preservada. */
    while (treinadorRetirarPokemon(t, &entregue)) {
        if (!pokelistaInserir(&cp->recuperados, &entregue)) {
            /* O Pokemon ja saiu da lista do treinador: avisa em vez de
               perde-lo em silencio. */
            printf("Erro: memoria insuficiente ao receber %s no Centro de Pesquisa.\n",
                   pokemonGetNome(&entregue));
            return recebidos;
        }
        recebidos++;
    }

    return recebidos;
}

int pokecenterRecarregarPokebolas(PokeCenter *cp, Treinador *t)
{
    cord posicaoCentro;
    cord posicaoTreinador;
    int quantidade;

    /* As Pokebolas ficam no Centro: uma chamada fora de hora nao pode criar
       Pokebolas do nada. No fluxo normal esta checagem nunca dispara. */
    posicaoCentro = pokecenterGetLocalizacao(cp);
    posicaoTreinador = treinadorGetLocalizacao(t);
    if (posicaoTreinador.cordX != posicaoCentro.cordX ||
        posicaoTreinador.cordY != posicaoCentro.cordY) {
        return 0;
    }

    /* O resto da divisao por (MAX - MIN + 1) da de 0 a MAX - MIN, e somar MIN
       desloca para o intervalo fechado [MIN, MAX]. O srand fica no main. */
    quantidade = MIN_RECARGA + rand() % (MAX_RECARGA - MIN_RECARGA + 1);

    treinadorSetPokebolas(t, quantidade);

    return quantidade;
}

int pokecenterGerarRelatorio(const PokeCenter *cp, const char *nomeArquivo)
{
    FILE *saida;

    saida = fopen(nomeArquivo, "w");
    if (saida == NULL) {
        return 0;
    }

    fprintf(saida, "Pokemon recuperados:\n");

    /* Quem percorre a lista e a lista. */
    pokelistaEscreverRelatorio(&cp->recuperados, saida);

    fclose(saida);

    return 1;
}

void pokecenterLiberar(PokeCenter *cp)
{
    pokelistaLiberar(&cp->fugitivos);
    pokelistaLiberar(&cp->recuperados);
}
