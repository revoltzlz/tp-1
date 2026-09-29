/* Programa principal do TP1 de Algoritmos e Estruturas de Dados I.
   Prepara o sorteio das Pokebolas e passa o controle para o menu da missao. */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "missao.h"

int main(void)
{
    /* srand e chamado uma unica vez, aqui, antes de qualquer sorteio. Se ele
       ficasse dentro da recarga de Pokebolas, duas recargas no mesmo segundo
       receberiam a mesma semente e sorteariam a mesma quantidade. */
    srand(time(NULL));

    missaoMenu();

    return 0;
}
