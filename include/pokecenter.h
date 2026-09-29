/* TAD Centro de Pesquisa: guarda os fugitivos, recebe os recuperados e
   recarrega as Pokebolas dos treinadores. */

#ifndef POKECENTER_H
#define POKECENTER_H

#include "treinador.h"

/* Posicao do Centro, fixada pela especificacao. */
#define CENTRO_X 0
#define CENTRO_Y 0

/* Intervalo fechado da recarga, exigido pela especificacao. */
#define MIN_RECARGA 1
#define MAX_RECARGA 20

/* As duas PokeLista: quem ainda esta fugido e quem ja foi entregue. */
typedef struct {
    Pokelista fugitivos;
    Pokelista recuperados;
    cord locPokeCenter;
} PokeCenter;

/* Poe o Centro em (0,0) e cria as duas listas vazias. Devolve 0 se alguma
   delas nao puder ser criada. */
int pokecenterInicializar(PokeCenter *cp);

/* Insere uma copia do Pokemon no fim da lista de fugitivos. Devolve 0 se a
   insercao falhar. */
int pokecenterRegistrarFugitivo(PokeCenter *cp, const Pokemon *p);

/* Remove o Pokemon de identificacao id da lista de fugitivos, copiando-o para
   *removido se este nao for NULL. Devolve 1 se encontrou e removeu. */
int pokecenterRemoverFugitivo(PokeCenter *cp, int id, Pokemon *removido);

/* Procura o Pokemon de identificacao id entre os fugitivos. Devolve 1 se
   encontrou, 0 se nao. */
int pokecenterBuscarFugitivo(const PokeCenter *cp, int id, Pokemon *encontrado);

/* Imprime os Pokemon que ainda nao foram recuperados. */
void pokecenterImprimirFugitivos(const PokeCenter *cp);

/* Devolve 1 se ainda ha Pokemon fugitivos. */
int pokecenterTemFugitivos(const PokeCenter *cp);

/* Devolve quantos Pokemon ainda estao fugidos. */
int pokecenterGetQtdFugitivos(const PokeCenter *cp);

/* Devolve quantos Pokemon ja foram recuperados. */
int pokecenterGetQtdRecuperados(const PokeCenter *cp);

/* Devolve uma copia da localizacao do Centro. */
cord pokecenterGetLocalizacao(const PokeCenter *cp);

/* Retira um por um os Pokemon do treinador e insere no fim dos recuperados,
   preservando a ordem de captura. Devolve quantos foram recebidos. */
int pokecenterReceberPokemon(PokeCenter *cp, Treinador *t);

/* Sorteia uma quantidade de Pokebolas em [MIN_RECARGA, MAX_RECARGA], entrega
   ao treinador e devolve essa quantidade. Devolve 0, sem recarregar, se o
   treinador nao estiver no Centro. Depende de srand, chamado no main. */
int pokecenterRecarregarPokebolas(PokeCenter *cp, Treinador *t);

/* Grava o relatorio final dos recuperados. Devolve 0 se nao conseguir abrir o
   arquivo para escrita. */
int pokecenterGerarRelatorio(const PokeCenter *cp, const char *nomeArquivo);

/* Libera as duas PokeLista do Centro. */
void pokecenterLiberar(PokeCenter *cp);

#endif
