#include <stdio.h>
#include <string.h>

#include "pokemon.h"

void pokemonInicializar(Pokemon *p, int identificacao, int numPokedex,
                        const char *nome, const char *tipo, int cordX, int cordY)
{
    pokemonSetId(p, identificacao);
    pokemonSetNumPokedex(p, numPokedex);
    pokemonSetNome(p, nome);
    pokemonSetTipo(p, tipo);
    pokemonSetLocalizacao(p, cordX, cordY);
}

void pokemonSetId(Pokemon *p, int identificacao)
{
    p->identificacao = identificacao;
}

void pokemonSetNumPokedex(Pokemon *p, int numPokedex)
{
    p->numPokedex = numPokedex;
}

void pokemonSetNome(Pokemon *p, const char *nome)
{
    /* O '\0' explicito e necessario: o strncpy nao termina a string quando o
       texto de origem enche o limite. */
    strncpy(p->nome, nome, TAM_NOME - 1);
    p->nome[TAM_NOME - 1] = '\0';
}

void pokemonSetTipo(Pokemon *p, const char *tipo)
{
    strncpy(p->tipo, tipo, TAM_TIPO - 1);
    p->tipo[TAM_TIPO - 1] = '\0';
}

void pokemonSetLocalizacao(Pokemon *p, int cordX, int cordY)
{
    p->localizacao.cordX = cordX;
    p->localizacao.cordY = cordY;
}

int pokemonGetId(const Pokemon *p)
{
    return p->identificacao;
}

int pokemonGetNumPokedex(const Pokemon *p)
{
    return p->numPokedex;
}

const char *pokemonGetNome(const Pokemon *p)
{
    return p->nome;
}

const char *pokemonGetTipo(const Pokemon *p)
{
    return p->tipo;
}

cord pokemonGetLocalizacao(const Pokemon *p)
{
    return p->localizacao;
}

void pokemonImprimir(const Pokemon *p)
{
    cord posicao = pokemonGetLocalizacao(p);

    /* O %03d mantem o zero a esquerda, como em 025. */
    printf("Id %d | Pokedex %03d | %s | Tipo: %s | Localizacao: (%d,%d)\n",
           pokemonGetId(p), pokemonGetNumPokedex(p), pokemonGetNome(p),
           pokemonGetTipo(p), posicao.cordX, posicao.cordY);
}
