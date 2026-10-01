#include <stdio.h>
#include <stdlib.h>

#include "pokecenter.h"

int pokecenterInicializar(PokeCenter *cp)
{
    cp->locPokeCenter.cordX = CENTRO_X;
    cp->locPokeCenter.cordY = CENTRO_Y;

    if (!pokelistaInicializar(&cp->fugitivos)) {
        return 0;
    }

    if (!pokelistaInicializar(&cp->recuperados)) {
        // libera a primeira, que ja tinha sido criada
        pokelistaLiberar(&cp->fugitivos);
        return 0;
    }

    return 1;
}

int pokecenterRegistrarFugitivo(PokeCenter *cp, const Pokemon *p)
{
    // insere no fim, para manter a ordem do arquivo
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

Coordenada pokecenterGetLocalizacao(const PokeCenter *cp)
{
    return cp->locPokeCenter;
}

int pokecenterReceberPokemon(PokeCenter *cp, Treinador *t)
{
    Pokemon entregue;
    int recebidos = 0;

    // o treinador entrega do primeiro ao ultimo e o centro insere no fim, entao a ordem de captura se mantem
    while (treinadorRetirarPokemon(t, &entregue)) {
        if (!pokelistaInserir(&cp->recuperados, &entregue)) {
            // o pokemon ja saiu da lista do treinador, entao avisa
            printf("Erro: memoria insuficiente ao receber %s no Centro de Pesquisa.\n", pokemonGetNome(&entregue));
            return recebidos;
        }
        recebidos++;
    }

    return recebidos;
}

int pokecenterRecarregarPokebolas(PokeCenter *cp, Treinador *t)
{
    Coordenada posicaoCentro;
    Coordenada posicaoTreinador;
    int quantidade;

    // so recarrega o treinador que esta no centro
    posicaoCentro = pokecenterGetLocalizacao(cp);
    posicaoTreinador = treinadorGetLocalizacao(t);
    
    if (posicaoTreinador.cordX != posicaoCentro.cordX ||
        posicaoTreinador.cordY != posicaoCentro.cordY) {
        return 0;
    }

    // o resto vai de 0 a max - min; somando min, fica entre min e max
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

    pokelistaEscreverRelatorio(&cp->recuperados, saida);

    fclose(saida);

    return 1;
}

void pokecenterLiberar(PokeCenter *cp)
{
    pokelistaLiberar(&cp->fugitivos);
    pokelistaLiberar(&cp->recuperados);
}
