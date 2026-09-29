#ifndef MISSAO_H
#define MISSAO_H

#include "pokecenter.h"

/* Quantidade de treinadores do esquadrao, fixada pela especificacao. */
#define NUM_TREINADORES 2

/* Nome do arquivo .txt do relatorio final dos Pokemon recuperados. */
#define ARQ_RELATORIO "relatorio.txt"

/* Tamanho do vetor que guarda o caminho do arquivo de entrada digitado pelo
   usuario, contando o '\0'. */
#define TAM_CAMINHO 256

/* Largura das molduras de "=" e das linhas de "-" da saida no terminal. */
#define LARGURA_MOLDURA 40

/* Opcoes do menu principal. */
#define OPCAO_SAIR 0
#define OPCAO_ARQUIVO 1
#define OPCAO_INTERATIVO 2

/* Mostra o menu principal e atende as opcoes do usuario ate ele escolher
   sair. E o unico ponto de entrada usado pelo programa principal. */
void missaoMenu(void);

/* Executa a missao completa lendo os dados do arquivo indicado: registro,
   capturas, retornos ao Centro e relatorio final. Devolve 1 se a missao foi
   executada e 0 se o arquivo nao pudesse ser aberto ou tivesse dados
   invalidos. Toda a memoria alocada e liberada antes de retornar. */
int missaoExecutarPorArquivo(const char *nomeArquivo);

/* Executa a missao completa pedindo os dados ao usuario pelo teclado, com uma
   mensagem antes de cada dado. Devolve 1 se a missao foi executada e 0 se os
   dados digitados forem invalidos. Toda a memoria alocada e liberada antes de
   retornar. */
int missaoExecutarInterativo(void);

#endif
