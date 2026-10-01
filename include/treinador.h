// tad treinador: um dos dois treinadores do esquadrao de recuperacao.

#ifndef TREINADOR_H
#define TREINADOR_H

#include "pokelista.h"

// tamanho do vetor do nome, contando o '\0'.
#define TAM_NOME_COACH 30

// posicao inicial de todo treinador.
#define TREINADOR_X_INICIAL 0
#define TREINADOR_Y_INICIAL 0

// a lista guarda os pokemon que ele capturou e ainda nao entregou.
typedef struct {
    int identificador;
    char nome[TAM_NOME_COACH];
    Coordenada loccoach;
    Pokelista lista;
    int qntdpokebolas;
} Treinador;

// prepara o treinador em (0,0) e cria a pokelista dele. devolve 0 se a lista
// nao puder ser criada.
int treinadorInicializar(Treinador *t, int identificador, const char *nome, int qntdpokebolas);

// atribui o identificador.
void treinadorSetId(Treinador *t, int identificador);

// copia o nome, truncando em tam_nome_coach - 1 caracteres.
void treinadorSetNome(Treinador *t, const char *nome);

// atribui a localizacao no mapa.
void treinadorSetLocalizacao(Treinador *t, int cordX, int cordY);

// atribui a quantidade de pokebolas.
void treinadorSetPokebolas(Treinador *t, int qntdpokebolas);

// devolve o identificador.
int treinadorGetId(const Treinador *t);

// devolve o endereco do nome, somente para leitura.
const char *treinadorGetNome(const Treinador *t);

// devolve uma copia da localizacao.
Coordenada treinadorGetLocalizacao(const Treinador *t);

// devolve quantas pokebolas ele ainda tem.
int treinadorGetPokebolas(const Treinador *t);

// move o treinador para (cordx, cordy).
void treinadorMovimentar(Treinador *t, int cordX, int cordY);

// gasta uma pokebola e guarda uma copia do pokemon na lista do treinador.
// devolve 0 se ele nao tinha pokebola ou se a insercao falhou.
int treinadorCapturar(Treinador *t, const Pokemon *p);

// retira o primeiro pokemon da lista dele e o copia para *retirado. devolve 0
// se ele nao esta carregando nenhum.
int treinadorRetirarPokemon(Treinador *t, Pokemon *retirado);

// imprime nome, posicao e pokebolas.
void treinadorImprimir(const Treinador *t);

// libera a pokelista do treinador.
void treinadorLiberar(Treinador *t);

#endif
