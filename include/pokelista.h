#include <stdio.h>
#include "conexao.h"
typedef struct {
    conec *inicio;
    int tamanho;
} Pokelista;

void InicializarPokelista(Pokelista *pl, int tamanho);
void IncercaoPokelista(Pokelista *pl, int tamanho);
void RemocaoPokelista(Pokelista *pl, int tamanho);
void BuscarPokelista(Pokelista *pl, int tamanho);
void ImpressaoPokelista(Pokelista *pl, int tamanho);
