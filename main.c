// programa principal do tp1 de aeds i. prepara o sorteio e chama a missao

#include <stdlib.h>
#include <time.h>

#include "missao.h"

int main(void)
{
    // semente do sorteio da recarga
    srand(time(NULL));

    missaoCaptura();

    return 0;
}
