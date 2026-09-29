#ifndef CONEXAO_H
#define CONEXAO_H

#include "pokemon.h"

/* Celula da lista encadeada: guarda um Pokemon por valor e o endereco da
   celula seguinte. Guardar por valor, e nao por ponteiro, faz com que passar
   um Pokemon de uma lista para outra seja copiar para fora e copiar para
   dentro, sem nenhuma memoria compartilhada entre as duas listas.

   O nome da struct (struct conec) e obrigatorio aqui: o campo prox aponta para
   o proprio tipo, e dentro do typedef o nome conec ainda nao existe. */
typedef struct conec {
    Pokemon pokemon;
    struct conec *prox;
} conec;

#endif
