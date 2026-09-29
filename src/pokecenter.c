#include <stdio.h>
#include <stdlib.h>

#include "pokecenter.h"

int pokecenterInicializar(PokeCenter *cp)
{
    /* A especificacao fixa o Centro de Pesquisa nas coordenadas (0,0). */
    cp->locPokeCenter.cordX = CENTRO_X;
    cp->locPokeCenter.cordY = CENTRO_Y;

    if (!pokelistaInicializar(&cp->fugitivos)) {
        return 0;
    }

    if (!pokelistaInicializar(&cp->recuperados)) {
        /* A primeira lista deu certo e a segunda nao. Libera a que ja existe
           para nao deixar a celula cabeca dela perdida na memoria. */
        pokelistaLiberar(&cp->fugitivos);
        return 0;
    }

    return 1;
}

int pokecenterRegistrarFugitivo(PokeCenter *cp, const Pokemon *p)
{
    /* Insere no fim, entao a lista de fugitivos guarda os Pokemon na mesma
       ordem em que apareceram no arquivo de entrada. */
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

    /* Esvazia a PokeLista do treinador um Pokemon por vez. Como o treinador
       entrega sempre o primeiro da lista dele e o Centro insere no fim da lista
       de recuperados, a ordem de captura e preservada.

       O Pokemon viaja dentro da variavel local entregue: sai copiado da celula
       do treinador, que e liberada, e entra copiado na celula nova do Centro.
       Nenhuma memoria e compartilhada entre as duas listas. */
    while (treinadorRetirarPokemon(t, &entregue)) {
        if (!pokelistaInserir(&cp->recuperados, &entregue)) {
            /* O Pokemon ja saiu da lista do treinador e nao ha memoria para a
               celula nova. Avisa em vez de perde-lo em silencio, e para a
               entrega: sem memoria, insistir nao ajudaria. */
            fprintf(stderr,
                    "Erro: memoria insuficiente ao receber %s no Centro de Pesquisa.\n",
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

    /* As Pokebolas ficam no Centro, entao so quem esta no Centro pode ser
       recarregado. No fluxo normal da missao o treinador sempre se movimenta
       para ca antes de pedir a recarga; esta checagem garante que uma chamada
       fora de hora nao crie Pokebolas do nada. */
    posicaoCentro = pokecenterGetLocalizacao(cp);
    posicaoTreinador = treinadorGetLocalizacao(t);
    if (posicaoTreinador.cordX != posicaoCentro.cordX ||
        posicaoTreinador.cordY != posicaoCentro.cordY) {
        return 0;
    }

    /* Sorteia um valor no intervalo fechado [MIN_RECARGA, MAX_RECARGA]. O resto
       da divisao por (MAX - MIN + 1) da um numero de 0 ate MAX - MIN, e somar
       MIN desloca o intervalo para o lugar certo.

       O srand e chamado uma unica vez, no programa principal. Se ele estivesse
       aqui, duas recargas no mesmo segundo receberiam a mesma semente e
       sorteariam o mesmo numero. */
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

    /* Quem percorre a lista e a propria lista: o Centro so abre o arquivo,
       escreve o cabecalho e fecha. */
    pokelistaEscreverRelatorio(&cp->recuperados, saida);

    fclose(saida);

    return 1;
}

void pokecenterLiberar(PokeCenter *cp)
{
    pokelistaLiberar(&cp->fugitivos);
    pokelistaLiberar(&cp->recuperados);
}
