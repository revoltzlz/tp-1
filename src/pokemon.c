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
    /* strncpy copia no maximo TAM_NOME - 1 caracteres, deixando a ultima
       posicao do vetor livre para o '\0' escrito na linha seguinte. Se o nome
       de origem for mais curto, o strncpy ja preenche o resto com '\0'. */
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
    /* Devolve uma copia da struct. Quem recebe pode mexer na copia sem
       alterar o Pokemon. */
    return p->localizacao;
}

void pokemonImprimir(const Pokemon *p)
{
    cord posicao = pokemonGetLocalizacao(p);

    /* A impressao le os atributos pelos proprios get, e nao direto da struct.
       Assim, se um dia a forma de guardar algum campo mudar, so o get precisa
       mudar de lugar.

       O %03d imprime o numero da Pokedex com pelo menos tres digitos, para que
       um numero como 025, que foi lido do arquivo como o inteiro 25, saia
       escrito do mesmo jeito que estava na entrada. */
    printf("Id %d | Pokédex %03d | %s | Tipo: %s | Localização: (%d,%d)\n",
           pokemonGetId(p), pokemonGetNumPokedex(p), pokemonGetNome(p),
           pokemonGetTipo(p), posicao.cordX, posicao.cordY);
}
