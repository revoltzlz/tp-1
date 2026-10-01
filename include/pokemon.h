/* TAD Pokemon: os dados de um Pokemon fugitivo e as operacoes sobre ele. */

#ifndef POKEMON_H
#define POKEMON_H

#include "coordenadas.h"

/* Tamanho dos vetores, contando o '\0'. */
#define TAM_NOME 30
#define TAM_TIPO 20

/* A identificacao e unica; o numPokedex e o numero da especie e pode repetir. */
typedef struct {
    int identificacao;
    int numPokedex;
    char nome[TAM_NOME];
    char tipo[TAM_TIPO];
    Coordenada localizacao;
} Pokemon;

/* Preenche os cinco atributos, chamando os proprios set. */
void pokemonInicializar(Pokemon *p, int identificacao, int numPokedex, const char *nome, const char *tipo, int cordX, int cordY);

/* Atribui a identificacao unica. */
void pokemonSetId(Pokemon *p, int identificacao);

/* Atribui o numero da especie na Pokedex. */
void pokemonSetNumPokedex(Pokemon *p, int numPokedex);

/* Copia o nome, truncando em TAM_NOME - 1 caracteres. */
void pokemonSetNome(Pokemon *p, const char *nome);

/* Copia o tipo, truncando em TAM_TIPO - 1 caracteres. */
void pokemonSetTipo(Pokemon *p, const char *tipo);

/* Atribui a localizacao no mapa. */
void pokemonSetLocalizacao(Pokemon *p, int cordX, int cordY);

/* Devolve a identificacao unica. */
int pokemonGetId(const Pokemon *p);

/* Devolve o numero da especie na Pokedex. */
int pokemonGetNumPokedex(const Pokemon *p);

/* Devolve o endereco do nome, somente para leitura. */
const char *pokemonGetNome(const Pokemon *p);

/* Devolve o endereco do tipo, somente para leitura. */
const char *pokemonGetTipo(const Pokemon *p);

/* Devolve uma copia da localizacao. */
Coordenada pokemonGetLocalizacao(const Pokemon *p);

/* Imprime os cinco atributos em uma linha. */
void pokemonImprimir(const Pokemon *p);

#endif
