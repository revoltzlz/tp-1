// tad pokemon: os dados de um pokemon fugitivo e as operacoes sobre ele.

#ifndef POKEMON_H
#define POKEMON_H

#include "coordenadas.h"

// tamanho dos vetores, contando o '\0'.
#define TAM_NOME 30
#define TAM_TIPO 20

// a identificacao e unica; o numpokedex e o numero da especie e pode repetir.
typedef struct {
    int identificacao;
    int numPokedex;
    char nome[TAM_NOME];
    char tipo[TAM_TIPO];
    Coordenada localizacao;
} Pokemon;

// preenche os cinco atributos, chamando os proprios set.
void pokemonInicializar(Pokemon *p, int identificacao, int numPokedex, const char *nome, const char *tipo, int cordX, int cordY);

// atribui a identificacao unica.
void pokemonSetId(Pokemon *p, int identificacao);

// atribui o numero da especie na pokedex.
void pokemonSetNumPokedex(Pokemon *p, int numPokedex);

// copia o nome, truncando em tam_nome - 1 caracteres.
void pokemonSetNome(Pokemon *p, const char *nome);

// copia o tipo, truncando em tam_tipo - 1 caracteres.
void pokemonSetTipo(Pokemon *p, const char *tipo);

// atribui a localizacao no mapa.
void pokemonSetLocalizacao(Pokemon *p, int cordX, int cordY);

// devolve a identificacao unica.
int pokemonGetId(const Pokemon *p);

// devolve o numero da especie na pokedex.
int pokemonGetNumPokedex(const Pokemon *p);

// devolve o endereco do nome, somente para leitura.
const char *pokemonGetNome(const Pokemon *p);

// devolve o endereco do tipo, somente para leitura.
const char *pokemonGetTipo(const Pokemon *p);

// devolve uma copia da localizacao.
Coordenada pokemonGetLocalizacao(const Pokemon *p);

// imprime os cinco atributos em uma linha.
void pokemonImprimir(const Pokemon *p);

#endif
