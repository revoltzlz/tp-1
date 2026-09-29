/* Programa principal do TP1 de Algoritmos e Estruturas de Dados I.

   Este modulo nao conhece os detalhes de nenhum TAD: ele apenas prepara o
   sorteio das Pokebolas, ajusta o terminal e passa o controle para o modulo da
   missao. Toda a logica do resgate esta em missao.c, e as estruturas estao nos
   TADs Pokemon, PokeLista, Treinador e Centro de Pesquisa. */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* No Windows o terminal nao usa UTF-8 por padrao, e os acentos das mensagens
   sairiam trocados. A chamada mais abaixo avisa o terminal que a saida e
   UTF-8. O #ifdef garante que nada disso e compilado no Linux. */
#ifdef _WIN32
#include <windows.h>
#endif

#include "missao.h"

int main(int argc, char *argv[])
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    /* srand e chamado UMA unica vez, aqui, antes de qualquer sorteio. Se ele
       ficasse dentro da recarga de Pokebolas, duas recargas no mesmo segundo
       receberiam a mesma semente e sorteariam a mesma quantidade. */
    srand((unsigned int) time(NULL));

    /* Atalho para os testes: um caminho passado na linha de comando executa a
       missao direto naquele arquivo, sem passar pelo menu. Sem argumento, o
       programa abre o menu com os dois modos de uso. */
    if (argc > 1) {
        return missaoExecutarPorArquivo(argv[1]) ? EXIT_SUCCESS : EXIT_FAILURE;
    }

    missaoMenu();

    return EXIT_SUCCESS;
}
