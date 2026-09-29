#ifndef POKEMON_H
#define POKEMON_H

#include "coordenadas.h"

/* Tamanho dos vetores de caracteres, contando o '\0' final. */
#define TAM_NOME 30
#define TAM_TIPO 20

/* Larguras maximas de leitura para o scanf/fscanf, sempre TAM_* - 1, para que
   o '\0' caiba. Ficam aqui, ao lado dos tamanhos, para que os dois nunca
   saiam de sincronia. */
#define FMT_NOME "%29s"
#define FMT_TIPO "%19s"

/* Um Pokemon fugitivo. O id e a identificacao unica atribuida pelo programa na
   ordem de leitura; o numPokedex e o numero da especie, que pode repetir (o
   arquivo de teste oficial tem quatro Pikachus com numPokedex 25). */
typedef struct {
    int identificacao;
    int numPokedex;
    char nome[TAM_NOME];
    char tipo[TAM_TIPO];
    cord localizacao;
} Pokemon;

/* Inicializa o Pokemon com todos os seus atributos. Chama os proprios set,
   para que exista um unico lugar que copia cada campo. */
void pokemonInicializar(Pokemon *p, int identificacao, int numPokedex,
                        const char *nome, const char *tipo, int cordX, int cordY);

/* Atribui a identificacao unica do Pokemon. */
void pokemonSetId(Pokemon *p, int identificacao);

/* Atribui o numero da especie na Pokedex. */
void pokemonSetNumPokedex(Pokemon *p, int numPokedex);

/* Copia o nome para dentro do Pokemon, truncando em TAM_NOME - 1 caracteres e
   garantindo o '\0' final. */
void pokemonSetNome(Pokemon *p, const char *nome);

/* Copia o tipo para dentro do Pokemon, truncando em TAM_TIPO - 1 caracteres e
   garantindo o '\0' final. */
void pokemonSetTipo(Pokemon *p, const char *tipo);

/* Atribui a localizacao do Pokemon no mapa. */
void pokemonSetLocalizacao(Pokemon *p, int cordX, int cordY);

/* Devolve a identificacao unica do Pokemon. */
int pokemonGetId(const Pokemon *p);

/* Devolve o numero da especie na Pokedex. */
int pokemonGetNumPokedex(const Pokemon *p);

/* Devolve o endereco do nome guardado no Pokemon. O const avisa que quem
   recebe so pode ler: para alterar o nome existe o pokemonSetNome. */
const char *pokemonGetNome(const Pokemon *p);

/* Devolve o endereco do tipo guardado no Pokemon, somente para leitura. */
const char *pokemonGetTipo(const Pokemon *p);

/* Devolve uma copia da localizacao do Pokemon. */
cord pokemonGetLocalizacao(const Pokemon *p);

/* Imprime no terminal todos os atributos do Pokemon, em uma linha. */
void pokemonImprimir(const Pokemon *p);

#endif
