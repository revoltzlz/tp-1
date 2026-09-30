/* Programa principal do TP1 de AEDS I. Prepara o sorteio e chama o menu. */

#include <stdlib.h>
#include <time.h>

#include "missao.h"

int main(void)
{
    /* Uma unica vez, antes de qualquer sorteio: duas chamadas no mesmo segundo
       receberiam a mesma semente e sorteariam o mesmo numero. */
    srand(time(NULL));

    missaoMenu();

    return 0;
}
