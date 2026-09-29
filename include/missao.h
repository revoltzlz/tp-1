#ifndef MISSAO_H
#define MISSAO_H

#include "pokecenter.h"

/* Quantidade de treinadores do esquadrao, fixada pela especificacao. */
#define NUM_TREINADORES 2

/* Identificadores dos dois treinadores, atribuidos na ordem de leitura. Sao
   eles que desempatam quando os dois estao a mesma distancia do alvo, e por
   isso o primeiro treinador do arquivo precisa receber o menor. */
#define ID_TREINADOR_1 1
#define ID_TREINADOR_2 2

/* Nome do arquivo .txt do relatorio final dos Pokemon recuperados. */
#define ARQ_RELATORIO "relatorio.txt"

/* Tamanho do vetor que guarda o caminho do arquivo de entrada digitado pelo
   usuario, contando o '\0'. */
#define TAM_CAMINHO 256
#define FMT_CAMINHO "%255s"

/* Teto de sanidade para as quantidades lidas da entrada: Pokebolas, Pokemon
   fugitivos e numero na Pokedex. Nao vem da especificacao; existe porque o %d
   do scanf, diante de um numero grande demais para caber em um int, guarda um
   valor truncado sem avisar. Sem um teto, esse lixo entraria no programa como
   se fosse um dado valido. */
#define MAX_QUANTIDADE 1000000

/* Largura das molduras de "=" e das linhas de "-" da saida no terminal. */
#define LARGURA_MOLDURA 40

/* Tamanho do vetor que monta o titulo de uma moldura, contando o '\0'. Cabe o
   maior titulo do programa, que inclui o nome de um treinador. */
#define TAM_TITULO 80

/* Espacos antes do titulo de cada moldura.

   Os tres primeiros foram medidos linha por linha no exemplo de saida da
   especificacao. Os dois ultimos nao vieram de la: INDENT_MENU e do menu, que
   o exemplo nao mostra, e INDENT_CONCLUIDA centraliza o titulo em
   LARGURA_MOLDURA, porque no exemplo essa linha vem com um caractere
   invisivel no lugar da indentacao (ver a errata E05 no REQUISITOS.md). */
#define INDENT_MENU 8
#define INDENT_INICIO 18
#define INDENT_SEM_POKEBOLAS 12
#define INDENT_RESGATADOS 7
#define INDENT_CONCLUIDA 12

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
