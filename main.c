/* Programa principal do TP1 de AEDS I. Prepara o sorteio e chama a missao. */

#include <stdlib.h>
#include <time.h>

#include "missao.h"

int main(void)
{
    /* Semente do sorteio da recarga. */
    srand(time(NULL));

    missaoCaptura();

    return 0;
}
