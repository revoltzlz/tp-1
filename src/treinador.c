#include "pokemon.h"
#include "coordenadas.h"
#include "pokelista.h"
#include "treinador.h"
#include "pokecenter.h"

#include <stdio.h>

void InicializacaoTreinador(Treinador *t, int identificador, char nome[], int qntdpokebolas, int cordX, int cordY)
{
    strcpy(t->Nome, nome);
    t->identificador = identificador;
    t->qntdpokebolas = qntdpokebolas;

    t->loccoach.cordX = cordX;
    t->loccoach.cordY = cordY;
}

void ImpressaoTreinador(Treinador *t)
{
    printf("Treinador: %s\n", t->Nome);
    printf("Indentificador: %d\n", t->identificador);
    printf("Numero na Pokebolas: %d\n", t->qntdpokebolas);
    printf("Localizacao: (%d , %d)", t->loccoach.cordX, t->loccoach.cordY);
}
