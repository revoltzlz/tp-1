#include <stdio.h>
#define TAM_NOME_COACH 30
#include "coordenadas.h"
#include "pokelista.h"
typedef struct {
    int identificador; 
    char Nome[TAM_NOME_COACH];
    cord loccoach;
    Pokelista lista;
    int qntdpokebolas;
} Treinador;

void InicializacaoTreinador(Treinador *t, int identificador, char nome[], int qntdpokebolas, int cordX, int cordY);
void MovimentacaoCoach(Treinador *t);
void CapturadePokemon(Treinador *t);
void RemoveListacoach(Treinador *t);
void RecargaPokebola(Treinador *t);
void ImpressaoTreinador(Treinador *t);