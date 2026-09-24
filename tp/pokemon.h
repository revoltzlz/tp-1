#include <stdio.h>
#include "coordenadas.h"
#ifndef pokemon_h
#define TAM_NOME 12
#define TAM_TIPO 10

typedef struct {
    int indentificacao;
    int Numpokedex;
    char Nome[TAM_NOME];
    cord localizacao;
} Pokemon;

void InicializarPokemon(Pokemon *p, int indentificacao, int Numpokedex, char nome[], int cordX, int cordY)
void ImprimirPokemon(Pokemon *p)
#endif