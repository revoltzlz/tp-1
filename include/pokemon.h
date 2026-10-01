#ifndef POKEMON_H
#define POKEMON_H

#include "coordenadas.h"

// pokemon: os dados de um pokemon

// tamanho dos vetores
#define TAM_NOME 30
#define TAM_TIPO 20

typedef struct {
    int identificacao;
    int numPokedex;
    char nome[TAM_NOME];
    char tipo[TAM_TIPO];
    Coordenada localizacao;
} Pokemon;

// preenche os cinco campos, chamando os proprios set
void pokemonInicializar(Pokemon *p, int identificacao, int numPokedex, const char *nome, const char *tipo, int cordX, int cordY);

// atribui a identificacao
void pokemonSetId(Pokemon *p, int identificacao);

// atribui o numero na pokedex
void pokemonSetNumPokedex(Pokemon *p, int numPokedex);

// atribui o nome do pokemon
void pokemonSetNome(Pokemon *p, const char *nome);

// atribui o tipo do pokemon
void pokemonSetTipo(Pokemon *p, const char *tipo);

// atribui a localizacao no mapa
void pokemonSetLocalizacao(Pokemon *p, int cordX, int cordY);

// devolve a identificacao unica
int pokemonGetId(const Pokemon *p);

// devolve o numero na pokedex
int pokemonGetNumPokedex(const Pokemon *p);

// devolve o endereco do nome
const char *pokemonGetNome(const Pokemon *p);

// devolve o endereco do tipo
const char *pokemonGetTipo(const Pokemon *p);

// devolve uma copia da localizacao
Coordenada pokemonGetLocalizacao(const Pokemon *p);

// imprime os cinco campos
void pokemonImprimir(const Pokemon *p);

#endif
