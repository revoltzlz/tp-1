/* Sistema de controle da missao: le a entrada, conduz o resgate e gera o
   relatorio usando os quatro TADs. */

#ifndef MISSAO_H
#define MISSAO_H

#include "pokecenter.h"

/* Identificadores dos treinadores, na ordem do arquivo. No empate de
   distancia, a missao vai para o de menor identificador. */
#define ID_TREINADOR_1 1
#define ID_TREINADOR_2 2

/* Nome do arquivo do relatorio final. */
#define ARQ_RELATORIO "relatorio.txt"

/* Vetor do caminho digitado no menu e a largura de leitura correspondente. */
#define TAM_CAMINHO 256
#define FMT_CAMINHO "%255s"

/* Maior quantidade aceita na entrada. Um numero que nao cabe em int deixa o
   valor lido pelo %d indefinido, entao valores grandes demais sao recusados. */
#define MAX_QUANTIDADE 1000000

/* Largura das linhas de "=" e de "-" da saida. */
#define LARGURA_MOLDURA 40

/* Espacos antes do titulo de cada moldura, para alinhar com o exemplo do
   enunciado. */
#define INDENT_MENU 8
#define INDENT_INICIO 18
#define INDENT_SEM_POKEBOLAS 12
#define INDENT_RESGATADOS 7
#define INDENT_CONCLUIDA 12

/* Opcoes do menu. */
#define OPCAO_SAIR 0
#define OPCAO_ARQUIVO 1

/* Mostra o menu e atende as opcoes ate o usuario escolher sair. E o unico
   ponto de entrada usado pelo programa principal. */
void missaoMenu(void);

/* Roda a missao completa com os dados do arquivo indicado e libera toda a
   memoria. Devolve 0 se o arquivo nao abrir ou tiver dados invalidos. */
int missaoExecutarPorArquivo(const char *nomeArquivo);

#endif
