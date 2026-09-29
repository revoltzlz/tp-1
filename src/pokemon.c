#include "pokemon.h"
#include "coordenadas.h"
#include "pokelista.h"
#include "treinador.h"
#include "pokecenter.h"

#include <stdio.h>

void InicializarPokemon(Pokemon *p, int indentificacao, int Numpokedex, char nome[], int cordX, int cordY)
{
    strcpy(p->Nome, nome);
    p->indentificacao = indentificacao;
    p->Numpokedex = Numpokedex;

    p->localizacao.cordX = cordX;
    p->localizacao.cordY = cordY;
}

void ImprimirPokemon(Pokemon *p)
{
    printf("Pokemon alvo: %s\n", p->Nome);
    printf("Indentificacao: %d\n", p->indentificacao);
    printf("Numero na Pokedex: %s\n", p->Numpokedex);
    printf("Localizacao: (%d , %d)", p->localizacao.cordX, p->localizacao.cordY);
}