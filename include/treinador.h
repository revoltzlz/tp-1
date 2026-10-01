/* TAD Treinador: um dos dois treinadores do esquadrao de recuperacao. */

#ifndef TREINADOR_H
#define TREINADOR_H

#include "pokelista.h"

/* Tamanho do vetor do nome e a largura de leitura correspondente. */
#define TAM_NOME_COACH 30

/* Posicao inicial de todo treinador. */
#define TREINADOR_X_INICIAL 0
#define TREINADOR_Y_INICIAL 0

/* A lista guarda os Pokemon que ele capturou e ainda nao entregou. */
typedef struct {
    int identificador;
    char nome[TAM_NOME_COACH];
    Coordenada loccoach;
    Pokelista lista;
    int qntdpokebolas;
} Treinador;

/* Prepara o treinador em (0,0) e cria a PokeLista dele. Devolve 0 se a lista
   nao puder ser criada. */
int treinadorInicializar(Treinador *t, int identificador, const char *nome, int qntdpokebolas);

/* Atribui o identificador. */
void treinadorSetId(Treinador *t, int identificador);

/* Copia o nome, truncando em TAM_NOME_COACH - 1 caracteres. */
void treinadorSetNome(Treinador *t, const char *nome);

/* Atribui a localizacao no mapa. */
void treinadorSetLocalizacao(Treinador *t, int cordX, int cordY);

/* Atribui a quantidade de Pokebolas. */
void treinadorSetPokebolas(Treinador *t, int qntdpokebolas);

/* Devolve o identificador. */
int treinadorGetId(const Treinador *t);

/* Devolve o endereco do nome, somente para leitura. */
const char *treinadorGetNome(const Treinador *t);

/* Devolve uma copia da localizacao. */
Coordenada treinadorGetLocalizacao(const Treinador *t);

/* Devolve quantas Pokebolas ele ainda tem. */
int treinadorGetPokebolas(const Treinador *t);

/* Move o treinador para (cordX, cordY). */
void treinadorMovimentar(Treinador *t, int cordX, int cordY);

/* Gasta uma Pokebola e guarda uma copia do Pokemon na lista do treinador.
   Devolve 0 se ele nao tinha Pokebola ou se a insercao falhou. */
int treinadorCapturar(Treinador *t, const Pokemon *p);

/* Retira o primeiro Pokemon da lista dele e o copia para *retirado. Devolve 0
   se ele nao esta carregando nenhum. */
int treinadorRetirarPokemon(Treinador *t, Pokemon *retirado);

/* Imprime nome, posicao e Pokebolas. */
void treinadorImprimir(const Treinador *t);

/* Libera a PokeLista do treinador. */
void treinadorLiberar(Treinador *t);

#endif
