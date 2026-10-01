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

/* Roda a missao completa com os dados do arquivo indicado e libera toda a
   memoria. Devolve 0 se o arquivo nao abrir ou tiver dados invalidos. */
int missaoCaptura();

#endif
