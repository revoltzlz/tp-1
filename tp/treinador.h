#include <stdio.h>

typedef struct {
    int identificador; 
    char Nome[tamanho_nome_coach];
    cord loccoach;
    Pokelista lista;
    int qntdpokebolas;
} Treinador;

void InicializacaoTreinador(Treinador *t, int indentificador, char nome[], int qntdpokebolas, int cordX, int cordY)
void MovimentacaoCoach(Treinador *t)
void CapturadePokemon(Treinador *t)
void RemoveListacoach(Treinador *t)
void RecargaPokebola(Treinador *t)
void ImpressaoTreinador(Treinador *t)