#ifndef POKECENTER_H
#define POKECENTER_H

#include "treinador.h"

/* Posicao do Centro de Pesquisa, fixada pela especificacao. */
#define CENTRO_X 0
#define CENTRO_Y 0

/* Intervalo fechado da recarga de Pokebolas exigido pela especificacao. */
#define MIN_RECARGA 1
#define MAX_RECARGA 20

/* O Centro de Pesquisa Pokemon. Mantem duas PokeLista: fugitivos, com os
   Pokemon cuja fuga foi registrada e que ainda nao voltaram, e recuperados,
   com os que os treinadores ja entregaram. */
typedef struct {
    Pokelista fugitivos;
    Pokelista recuperados;
    cord locPokeCenter;
} PokeCenter;

/* Inicializa o Centro na posicao (0,0) e cria as duas PokeLista vazias.
   Devolve 1 em caso de sucesso e 0 se alguma das listas nao puder ser criada. */
int pokecenterInicializar(PokeCenter *cp);

/* Registra a fuga de um Pokemon, inserindo uma copia dele no fim da lista de
   fugitivos. Devolve 1 em caso de sucesso e 0 se a insercao falhar. */
int pokecenterRegistrarFugitivo(PokeCenter *cp, const Pokemon *p);

/* Remove da lista de fugitivos o Pokemon de identificacao id, chamado quando
   um treinador informa uma captura. Se removido nao for NULL, copia para lá o
   Pokemon retirado. Devolve 1 se encontrou e removeu, 0 caso contrario. */
int pokecenterRemoverFugitivo(PokeCenter *cp, int id, Pokemon *removido);

/* Procura na lista de fugitivos o Pokemon de identificacao id e, se achar,
   copia-o para *encontrado. Devolve 1 se encontrou, 0 se nao. */
int pokecenterBuscarFugitivo(const PokeCenter *cp, int id, Pokemon *encontrado);

/* Imprime no terminal os Pokemon que ainda nao foram recuperados. */
void pokecenterImprimirFugitivos(const PokeCenter *cp);

/* Devolve 1 se ainda ha Pokemon fugitivos e 0 se todos foram recuperados. */
int pokecenterTemFugitivos(const PokeCenter *cp);

/* Devolve quantos Pokemon ainda estao fugidos. */
int pokecenterGetQtdFugitivos(const PokeCenter *cp);

/* Devolve quantos Pokemon ja foram recuperados. */
int pokecenterGetQtdRecuperados(const PokeCenter *cp);

/* Devolve uma copia da localizacao do Centro de Pesquisa. */
cord pokecenterGetLocalizacao(const PokeCenter *cp);

/* Recebe todos os Pokemon que o treinador esta carregando: retira um por um da
   PokeLista dele, na ordem de captura, e insere no fim da lista de
   recuperados. Devolve quantos Pokemon foram recebidos. */
int pokecenterReceberPokemon(PokeCenter *cp, Treinador *t);

/* Entrega ao treinador uma quantidade aleatoria de Pokebolas no intervalo
   [MIN_RECARGA, MAX_RECARGA] e devolve essa quantidade. Depende de srand ter
   sido chamado uma vez pelo programa principal. */
int pokecenterRecarregarPokebolas(PokeCenter *cp, Treinador *t);

/* Emite o relatorio final em arquivo .txt com os Pokemon recuperados. Devolve
   1 em caso de sucesso e 0 se o arquivo nao puder ser aberto para escrita. */
int pokecenterGerarRelatorio(const PokeCenter *cp, const char *nomeArquivo);

/* Libera as duas PokeLista do Centro de Pesquisa. */
void pokecenterLiberar(PokeCenter *cp);

#endif
