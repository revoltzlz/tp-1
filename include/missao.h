/* Sistema de controle da missao: conduz o resgate usando os quatro TADs.
   Nao e um TAD: e o modulo separado que a especificacao pede. */

#ifndef MISSAO_H
#define MISSAO_H

#include "pokecenter.h"

/* Os dois treinadores do esquadrao, que a especificacao fixa em dois. Os
   identificadores sao atribuidos na ordem de leitura, e sao eles que
   desempatam quando os dois estao a mesma distancia do alvo: por isso o
   primeiro treinador do arquivo precisa receber o menor. */
#define ID_TREINADOR_1 1
#define ID_TREINADOR_2 2

/* Nome do arquivo do relatorio final. */
#define ARQ_RELATORIO "relatorio.txt"

/* Vetor do caminho digitado no menu e a largura de leitura correspondente. */
#define TAM_CAMINHO 256
#define FMT_CAMINHO "%255s"

/* Teto das quantidades lidas. Existe porque o %d do scanf, diante de um numero
   que nao cabe em um int, nao garante o que grava: o padrao da linguagem diz
   que o comportamento e indefinido. Sem o teto, esse valor entraria no
   programa como se fosse um dado valido. */
#define MAX_QUANTIDADE 1000000

/* Largura das linhas de "=" e de "-" da saida. */
#define LARGURA_MOLDURA 40

/* Espacos antes do titulo de cada moldura. Os tres do meio foram medidos no
   exemplo da especificacao; os outros dois estao explicados em
   descricoes/missao.md. */
#define INDENT_MENU 8
#define INDENT_INICIO 18
#define INDENT_SEM_POKEBOLAS 12
#define INDENT_RESGATADOS 7
#define INDENT_CONCLUIDA 12

/* Opcoes do menu. */
#define OPCAO_SAIR 0
#define OPCAO_ARQUIVO 1
#define OPCAO_INTERATIVO 2

/* Mostra o menu e atende as opcoes ate o usuario escolher sair. E o unico
   ponto de entrada usado pelo programa principal. */
void missaoMenu(void);

/* Roda a missao completa com os dados do arquivo indicado e libera toda a
   memoria. Devolve 0 se o arquivo nao abrir ou tiver dados invalidos. */
int missaoExecutarPorArquivo(const char *nomeArquivo);

/* Roda a missao completa pedindo os dados pelo teclado e libera toda a
   memoria. Devolve 0 se os dados digitados forem invalidos. */
int missaoExecutarInterativo(void);

#endif
