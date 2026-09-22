#include <stdio.h>

typedef struct {
    int indentificacao;
    int Numpokedex;
    char Nome[tamanho_nome_poke];
    cord localizacao;
} Pokemon;

void InicializarPokemon(Pokemon *p, int indentificacao, int Numpokedex, char nome[], int cordX, int cordY)
void ImprimirPokemon(Pokemon *p)
