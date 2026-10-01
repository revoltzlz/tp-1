// sistema de controle da missao: le a entrada, conduz o resgate e gera o
// relatorio usando os quatro tads

#ifndef MISSAO_H
#define MISSAO_H

#include "pokecenter.h"

// identificadores dos treinadores, na ordem do arquivo. no empate de
// distancia, a missao vai para o de menor identificador
#define ID_TREINADOR_1 1
#define ID_TREINADOR_2 2

// nome do arquivo do relatorio final
#define ARQ_RELATORIO "relatorio.txt"

// tamanho do vetor do caminho digitado, contando o '\0'
#define TAM_CAMINHO 256

// pede o caminho do arquivo de entrada, roda a missao completa e libera toda
// a memoria. devolve 0 se o arquivo nao abrir ou o relatorio nao for gravado
int missaoCaptura();

#endif
