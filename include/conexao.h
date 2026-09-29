/* A celula da lista encadeada de Pokemon. */

#ifndef CONEXAO_H
#define CONEXAO_H

#include "pokemon.h"

/* O Pokemon e guardado por valor, e nao por ponteiro: assim cada lista tem a
   sua propria copia e nenhuma memoria e compartilhada entre duas listas.

   O nome struct conec e obrigatorio: o campo prox aponta para o proprio tipo,
   e dentro do typedef o nome conec ainda nao existe. */
typedef struct conec {
    Pokemon pokemon;
    struct conec *prox;
} conec;

#endif
