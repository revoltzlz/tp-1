#ifndef POKECENTER_H
#define POKECENTER_H

// centro de pesquisa: guarda os fugitivos, recebe os recuperados e recarrega as pokebolas dos treinadores

#include "treinador.h"

// posicao do centro
#define CENTRO_X 0
#define CENTRO_Y 0

// intervalo da recarga de pokebolas
#define MIN_RECARGA 1
#define MAX_RECARGA 20

typedef struct {
   Pokelista fugitivos;
   Pokelista recuperados;
   Coordenada locPokeCenter;
} PokeCenter;

// poe o centro em (0,0) e cria as duas listas vazias e devolve 0 se alguma delas nao puder ser criada
int pokecenterInicializar(PokeCenter *cp);

// insere um pokemon no fim da lista de fugitivos e devolve 0 se a insercao falhar
int pokecenterRegistrarFugitivo(PokeCenter *cp, const Pokemon *p);

// remove o pokemon de identificacao id da lista de fugitivos, copiando-o para *removido se este nao for null. devolve 1 se encontrou e removeu
int pokecenterRemoverFugitivo(PokeCenter *cp, int id, Pokemon *removido);

// procura o pokemon de identificacao id entre os fugitivos e devolve 1 se encontrou e 0 se nao
int pokecenterBuscarFugitivo(const PokeCenter *cp, int id, Pokemon *encontrado);

// imprime os pokemon que ainda nao foram recuperados
void pokecenterImprimirFugitivos(const PokeCenter *cp);

// devolve 1 se ainda ha pokemon fugitivos
int pokecenterTemFugitivos(const PokeCenter *cp);

// devolve quantos pokemon ainda estao fugidos
int pokecenterGetQtdFugitivos(const PokeCenter *cp);

// devolve uma copia da localizacao do centro
Coordenada pokecenterGetLocalizacao(const PokeCenter *cp);

// retira um por um os pokemon do treinador e insere no fim dos recuperados, preservando a ordem de captura. devolve quantos foram recebidos
int pokecenterReceberPokemon(PokeCenter *cp, Treinador *t);

// sorteia uma quantidade de pokebolas em [min_recarga, max_recarga], entrega ao treinador e devolve essa quantidade. devolve 0, sem recarregar, se o treinador nao estiver no centro. depende de srand, chamado no main
int pokecenterRecarregarPokebolas(PokeCenter *cp, Treinador *t);

// grava o relatorio final dos recuperados. devolve 0 se nao conseguir abrir o arquivo para escrita
int pokecenterGerarRelatorio(const PokeCenter *cp, const char *nomeArquivo);

// libera as duas pokelista do centro
void pokecenterLiberar(PokeCenter *cp);

#endif
