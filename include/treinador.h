#ifndef TREINADOR_H
#define TREINADOR_H

#include "pokelista.h"

/* Tamanho do vetor do nome do treinador, contando o '\0', e a largura
   correspondente para o scanf/fscanf (sempre TAM_NOME_COACH - 1). */
#define TAM_NOME_COACH 30
#define FMT_NOME_COACH "%29s"

/* Posicao em que todo treinador comeca a missao, exigida pela especificacao.
   E a mesma posicao do Centro de Pesquisa. */
#define TREINADOR_X_INICIAL 0
#define TREINADOR_Y_INICIAL 0

/* Um treinador do esquadrao de recuperacao. A lista e a PokeLista propria
   dele, onde ficam os Pokemon que ele capturou e ainda nao entregou. */
typedef struct {
    int identificador;
    char nome[TAM_NOME_COACH];
    cord loccoach;
    Pokelista lista;
    int qntdpokebolas;
} Treinador;

/* Inicializa o treinador com identificador, nome e quantidade inicial de
   Pokebolas, coloca-o na posicao inicial (0,0) e inicializa a PokeLista dele.
   Nao recebe coordenadas porque a especificacao fixa a posicao inicial.
   Devolve 1 em caso de sucesso e 0 se a PokeLista nao puder ser criada. */
int treinadorInicializar(Treinador *t, int identificador, const char *nome,
                         int qntdpokebolas);

/* Atribui o identificador do treinador. */
void treinadorSetId(Treinador *t, int identificador);

/* Copia o nome para dentro do treinador, truncando em TAM_NOME_COACH - 1
   caracteres e garantindo o '\0' final. */
void treinadorSetNome(Treinador *t, const char *nome);

/* Atribui a localizacao do treinador no mapa. */
void treinadorSetLocalizacao(Treinador *t, int cordX, int cordY);

/* Atribui a quantidade de Pokebolas do treinador. */
void treinadorSetPokebolas(Treinador *t, int qntdpokebolas);

/* Devolve o identificador do treinador. */
int treinadorGetId(const Treinador *t);

/* Devolve o endereco do nome do treinador, somente para leitura. */
const char *treinadorGetNome(const Treinador *t);

/* Devolve uma copia da localizacao do treinador. */
cord treinadorGetLocalizacao(const Treinador *t);

/* Devolve quantas Pokebolas o treinador ainda tem. */
int treinadorGetPokebolas(const Treinador *t);

/* Devolve quantos Pokemon o treinador esta carregando. */
int treinadorGetQtdPokemon(const Treinador *t);

/* Move o treinador para a coordenada indicada. Nao imprime nada: a mensagem
   do deslocamento pertence a quem narra a missao. */
void treinadorMovimentar(Treinador *t, int cordX, int cordY);

/* Captura o Pokemon: gasta uma Pokebola e guarda uma copia do Pokemon na
   PokeLista do treinador. Devolve 1 em caso de sucesso, 0 se o treinador nao
   tinha Pokebola ou se a insercao na lista falhou. */
int treinadorCapturar(Treinador *t, const Pokemon *p);

/* Retira o primeiro Pokemon da PokeLista do treinador e o copia para
   *retirado. Devolve 1 em caso de sucesso e 0 se ele nao esta carregando
   nenhum Pokemon. */
int treinadorRetirarPokemon(Treinador *t, Pokemon *retirado);

/* Imprime no terminal os dados do treinador: nome, posicao e Pokebolas. */
void treinadorImprimir(const Treinador *t);

/* Libera a PokeLista do treinador. */
void treinadorLiberar(Treinador *t);

#endif
