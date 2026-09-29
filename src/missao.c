/* Sistema de controle da missao: leitura da entrada, laco do resgate,
   retornos ao Centro e menu. */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "missao.h"

/* Usada para as linhas de "=" e de "-" e para a indentacao dos titulos. */
void imprimeRepetido(char caractere, int vezes)
{
    int i;

    for (i = 0; i < vezes; i++) {
        putchar(caractere);
    }
}

/* A linha que separa um resgate do seguinte. */
void imprimeSeparador(void)
{
    imprimeRepetido('-', LARGURA_MOLDURA);
    printf("\n");
}

/* Linha de "=", titulo recuado, linha em branco, linha de "=". */
void imprimeMoldura(const char *titulo, int indentacao)
{
    imprimeRepetido('=', LARGURA_MOLDURA);
    printf("\n");
    imprimeRepetido(' ', indentacao);
    printf("%s\n\n", titulo);
    imprimeRepetido('=', LARGURA_MOLDURA);
    printf("\n");
}

/* O titulo inclui o nome do treinador, entao nao passa por imprimeMoldura. */
void imprimeMolduraSemPokebolas(const Treinador *t)
{
    imprimeRepetido('=', LARGURA_MOLDURA);
    printf("\n");
    imprimeRepetido(' ', INDENT_SEM_POKEBOLAS);
    printf("Treinador(a) %s SEM POKÉBOLAS\n\n", treinadorGetNome(t));
    imprimeRepetido('=', LARGURA_MOLDURA);
    printf("\n\n");
}

/* Devolve o QUADRADO da distancia, sem tirar a raiz: o quadrado e inteiro e
   exato, entao o empate que a especificacao pede e detectado com seguranca, e
   a raiz e crescente, entao a ordem nao muda. O long long e porque a soma nao
   cabe em int; ver include/coordenadas.h. */
long long distanciaQuadrado(cord a, cord b)
{
    long long dx = (long long) a.cordX - b.cordX;
    long long dy = (long long) a.cordY - b.cordY;

    return dx * dx + dy * dy;
}

/* O mais proximo do alvo e, em caso de empate, o de menor identificador. */
Treinador *escolheTreinador(Treinador *t1, Treinador *t2,
                            long long distancia1, long long distancia2)
{
    if (distancia1 < distancia2) {
        return t1;
    }
    if (distancia2 < distancia1) {
        return t2;
    }

    /* Empate: a missao vai para o treinador de menor identificador. */
    if (treinadorGetId(t1) <= treinadorGetId(t2)) {
        return t1;
    }
    return t2;
}

/* Pula os tres bytes invisiveis (EF BB BF) que o Bloco de Notas escreve no
   comeco dos arquivos UTF-8, e que entrariam colados no nome do primeiro
   treinador. So serve para arquivo: numa entrada do teclado o rewind nao
   funciona. */
void pulaMarcaUtf8(FILE *entrada)
{
    int a, b, c;

    a = fgetc(entrada);
    b = fgetc(entrada);
    c = fgetc(entrada);

    if (a != 0xEF || b != 0xBB || c != 0xBF) {
        rewind(entrada);
    }
}

/* Le nome e Pokebolas de um treinador e o inicializa. Devolve 0 se os dados
   forem invalidos ou a PokeLista nao puder ser criada. */
int leTreinador(FILE *entrada, int interativo, Treinador *t, int id)
{
    char nome[TAM_NOME_COACH];
    int pokebolas;

    if (interativo) {
        printf("Nome do treinador %d: ", id);
    }
    /* A largura do formato impede escrita fora do vetor. */
    if (fscanf(entrada, FMT_NOME_COACH, nome) != 1) {
        printf("Erro: nao foi possivel ler o nome do treinador %d.\n", id);
        return 0;
    }

    if (interativo) {
        printf("Pokebolas iniciais de %s: ", nome);
    }
    if (fscanf(entrada, "%d", &pokebolas) != 1) {
        printf("Erro: nao foi possivel ler a quantidade de Pokebolas de %s.\n", nome);
        return 0;
    }
    /* Numero que nao cabe em int: o %d nao garante o que grava. */
    if (pokebolas < 0 || pokebolas > MAX_QUANTIDADE) {
        printf("Erro: quantidade de Pokebolas invalida para %s (%d). "
               "O valor precisa estar entre 0 e %d.\n",
               nome, pokebolas, MAX_QUANTIDADE);
        return 0;
    }

    if (!treinadorInicializar(t, id, nome, pokebolas)) {
        printf("Erro: memoria insuficiente para criar o treinador %s.\n", nome);
        return 0;
    }

    return 1;
}

/* Le os n fugitivos e registra cada um no Centro. A quantidade sai em
   *quantidade. Devolve 0 se os dados forem invalidos ou faltarem linhas. */
int leFugitivos(FILE *entrada, int interativo, PokeCenter *cp, int *quantidade)
{
    char nome[TAM_NOME];
    char tipo[TAM_TIPO];
    Pokemon p;
    int total;
    int i;
    int numPokedex;
    int cordX;
    int cordY;

    if (interativo) {
        printf("Quantidade de Pokemon fugitivos: ");
    }
    if (fscanf(entrada, "%d", &total) != 1) {
        printf("Erro: nao foi possivel ler a quantidade de Pokemon fugitivos.\n");
        return 0;
    }
    if (total < 0 || total > MAX_QUANTIDADE) {
        printf("Erro: quantidade de Pokemon fugitivos invalida (%d). "
               "O valor precisa estar entre 0 e %d.\n",
               total, MAX_QUANTIDADE);
        return 0;
    }

    for (i = 0; i < total; i++) {
        if (interativo) {
            printf("Pokemon %d de %d (numero na Pokedex, nome, tipo, X e Y): ",
                   i + 1, total);
        }

        /* O %s para em espaco em branco, e o '\r' do fim de linha do Windows
           conta como tal: nome e tipo nunca o recebem. */
        if (fscanf(entrada, "%d " FMT_NOME " " FMT_TIPO " %d %d",
                   &numPokedex, nome, tipo, &cordX, &cordY) != 5) {
            printf("Erro: dados incompletos do Pokemon %d de %d. "
                   "Cada linha precisa ter numero na Pokedex, nome, tipo, X e Y.\n",
                   i + 1, total);
            return 0;
        }

        if (numPokedex < 0 || numPokedex > MAX_QUANTIDADE) {
            printf("Erro: numero na Pokedex invalido para o Pokemon %s (%d).\n",
                   nome, numPokedex);
            return 0;
        }

        /* Alem de validar o dado, e o que impede o calculo da distancia de
           estourar: ver include/coordenadas.h. */
        if (cordX < COORD_MIN || cordX > COORD_MAX ||
            cordY < COORD_MIN || cordY > COORD_MAX) {
            printf("Erro: o Pokemon %s esta em (%d,%d), fora do mapa, "
                   "que vai de %d a %d nos dois eixos.\n",
                   nome, cordX, cordY, COORD_MIN, COORD_MAX);
            return 0;
        }

        /* O Id e a ordem de leitura. O numero da Pokedex nao serviria: o
           arquivo de teste oficial traz quatro Pikachus com o numero 25. */
        pokemonInicializar(&p, i + 1, numPokedex, nome, tipo, cordX, cordY);

        if (!pokecenterRegistrarFugitivo(cp, &p)) {
            printf("Erro: memoria insuficiente para registrar o Pokemon %s.\n", nome);
            return 0;
        }
    }

    *quantidade = total;

    return 1;
}

/* Pede o carregamento ao Centro e anuncia quantas Pokebolas ele recebeu. */
void recarregaNoCentro(PokeCenter *cp, Treinador *t)
{
    int recebidas;

    recebidas = pokecenterRecarregarPokebolas(cp, t);
    printf("Treinador(a) %s recebeu %d Pokébolas.\n\n", treinadorGetNome(t), recebidas);
}

/* Volta ao Centro, entrega tudo e pede um novo carregamento. */
void retornaAoCentro(PokeCenter *cp, Treinador *t)
{
    cord posicaoCentro;

    posicaoCentro = pokecenterGetLocalizacao(cp);

    imprimeMolduraSemPokebolas(t);

    printf("Treinador(a) %s retorna ao Centro de Pesquisa.\n\n", treinadorGetNome(t));
    treinadorMovimentar(t, posicaoCentro.cordX, posicaoCentro.cordY);

    printf("Entregando Pokémon ao Centro de Pesquisa.\n\n");
    pokecenterReceberPokemon(cp, t);

    recarregaNoCentro(cp, t);
}

/* Ninguem parte para uma captura sem Pokebola, e os dois comecam no Centro:
   quem tem zero recarrega antes do primeiro resgate. */
void recarregaSeComecouSemPokebola(PokeCenter *cp, Treinador *t)
{
    if (treinadorGetPokebolas(t) > 0) {
        return;
    }

    imprimeMolduraSemPokebolas(t);
    recarregaNoCentro(cp, t);
}

/* Um resgate completo: distancias, escolha, movimento, captura e aviso ao
   Centro. Devolve o treinador que capturou, ou NULL se a captura falhou. */
Treinador *resgataPokemon(PokeCenter *cp, Treinador *t1, Treinador *t2,
                          const Pokemon *alvo)
{
    cord posicaoAlvo;
    long long distancia1;
    long long distancia2;
    Treinador *escolhido;

    posicaoAlvo = pokemonGetLocalizacao(alvo);

    imprimeSeparador();
    printf("Pokémon alvo: %s\n", pokemonGetNome(alvo));
    printf("Localização: (%d,%d)\n\n", posicaoAlvo.cordX, posicaoAlvo.cordY);

    distancia1 = distanciaQuadrado(treinadorGetLocalizacao(t1), posicaoAlvo);
    distancia2 = distanciaQuadrado(treinadorGetLocalizacao(t2), posicaoAlvo);

    /* A raiz aparece so aqui, na impressao. */
    printf("Distância Treinador(a) %s: %.2f\n", treinadorGetNome(t1),
           sqrt((double) distancia1));
    printf("Distância Treinador(a) %s: %.2f\n\n", treinadorGetNome(t2),
           sqrt((double) distancia2));

    escolhido = escolheTreinador(t1, t2, distancia1, distancia2);
    printf("Missão atribuída ao Treinador(a) %s.\n\n", treinadorGetNome(escolhido));

    treinadorMovimentar(escolhido, posicaoAlvo.cordX, posicaoAlvo.cordY);
    printf("Treinador(a) %s se movimentou para (%d,%d).\n",
           treinadorGetNome(escolhido), posicaoAlvo.cordX, posicaoAlvo.cordY);

    if (!treinadorCapturar(escolhido, alvo)) {
        /* O fluxo da missao evita isto. O Pokemon continua fugitivo. */
        printf("%s escapou: Treinador(a) %s está sem Pokébolas!\n\n",
               pokemonGetNome(alvo), treinadorGetNome(escolhido));
        return NULL;
    }

    printf("%s capturado com sucesso!\n\n", pokemonGetNome(alvo));

    /* A cada captura o Centro atualiza a listagem de fugas. */
    pokecenterRemoverFugitivo(cp, pokemonGetId(alvo), NULL);

    printf("Pokébolas restantes para o Treinador(a) %s: %d\n\n",
           treinadorGetNome(escolhido), treinadorGetPokebolas(escolhido));

    return escolhido;
}

/* Os dois voltam ao Centro e devolvem o que carregam, na ordem do
   identificador. */
void encerraMissao(PokeCenter *cp, Treinador *t1, Treinador *t2)
{
    cord posicaoCentro;

    posicaoCentro = pokecenterGetLocalizacao(cp);

    /* Se alguma captura falhou, mostra quem ficou para tras. */
    if (pokecenterTemFugitivos(cp)) {
        printf("Atenção: ainda há %d Pokémon na lista de fugitivos:\n",
               pokecenterGetQtdFugitivos(cp));
        pokecenterImprimirFugitivos(cp);
        printf("\n");
        imprimeMoldura("Fim da missão de resgate", INDENT_RESGATADOS);
    } else {
        imprimeMoldura("Todos Pokemons foram resgatados", INDENT_RESGATADOS);
    }
    printf("\n");

    printf("Ambos treinadores retornam ao Centro de Pesquisa.\n\n");
    treinadorMovimentar(t1, posicaoCentro.cordX, posicaoCentro.cordY);
    treinadorMovimentar(t2, posicaoCentro.cordX, posicaoCentro.cordY);

    /* t1 tem o menor identificador, entao entrega primeiro: e essa ordem que
       faz o relatorio sair na ordem do exemplo. */
    printf("Treinador(a) %s devolve os Pokémon.\n\n", treinadorGetNome(t1));
    pokecenterReceberPokemon(cp, t1);

    printf("Treinador(a) %s devolve os Pokémon.\n\n", treinadorGetNome(t2));
    pokecenterReceberPokemon(cp, t2);

    imprimeMoldura("MISSÃO CONCLUÍDA", INDENT_CONCLUIDA);
}

/* O laco da missao, sobre dados ja registrados. */
void executaMissao(PokeCenter *cp, Treinador *t1, Treinador *t2, int qtdFugitivos)
{
    Pokemon alvo;
    Treinador *escolhido;
    int id;

    imprimeMoldura("INÍCIO DA MISSÃO", INDENT_INICIO);
    printf("\n");

    treinadorImprimir(t1);
    treinadorImprimir(t2);
    printf("\nPokémons fugitivos a serem resgatados: %d\n\n",
           pokecenterGetQtdFugitivos(cp));

    /* Se nao ha nenhum fugitivo, ninguem precisa de Pokebola. */
    if (qtdFugitivos > 0) {
        recarregaSeComecouSemPokebola(cp, t1);
        recarregaSeComecouSemPokebola(cp, t2);
    }

    /* Os Ids sao a ordem de leitura. A busca confirma que o Pokemon ainda
       esta fugido antes de montar o resgate. */
    for (id = 1; id <= qtdFugitivos; id++) {
        if (!pokecenterBuscarFugitivo(cp, id, &alvo)) {
            continue;
        }

        escolhido = resgataPokemon(cp, t1, t2, &alvo);
        if (escolhido == NULL) {
            continue;
        }

        /* A ORDEM DESTES DOIS TESTES IMPORTA: se o Pokemon capturado era o
           ultimo, a missao termina sem recarga, mesmo que o treinador tenha
           ficado sem Pokebolas. E o que o exemplo da especificacao mostra. */
        if (!pokecenterTemFugitivos(cp)) {
            break;
        }

        if (treinadorGetPokebolas(escolhido) == 0) {
            retornaAoCentro(cp, escolhido);
        }
    }

    encerraMissao(cp, t1, t2);
}

/* Caminho comum dos dois modos de uso: le, roda a missao, emite o relatorio e
   libera tudo. O parametro interativo so liga as mensagens que pedem cada
   dado. */
int executa(FILE *entrada, int interativo)
{
    PokeCenter centro;
    Treinador treinador1;
    Treinador treinador2;
    int qtdFugitivos = 0;
    int relatorioGravado;

    if (!pokecenterInicializar(&centro)) {
        printf("Erro: memoria insuficiente para criar o Centro de Pesquisa.\n");
        return 0;
    }

    /* Cada saida de erro libera exatamente o que chegou a ser criado ate ali:
       liberar uma PokeLista que nunca foi inicializada leria lixo. */
    if (!leTreinador(entrada, interativo, &treinador1, ID_TREINADOR_1)) {
        pokecenterLiberar(&centro);
        return 0;
    }

    if (!leTreinador(entrada, interativo, &treinador2, ID_TREINADOR_2)) {
        treinadorLiberar(&treinador1);
        pokecenterLiberar(&centro);
        return 0;
    }

    if (!leFugitivos(entrada, interativo, &centro, &qtdFugitivos)) {
        treinadorLiberar(&treinador1);
        treinadorLiberar(&treinador2);
        pokecenterLiberar(&centro);
        return 0;
    }

    if (interativo) {
        /* Para o usuario conferir o que digitou. */
        printf("\nPokémon fugitivos registrados no Centro de Pesquisa:\n");
        pokecenterImprimirFugitivos(&centro);
        printf("\n");
    }

    executaMissao(&centro, &treinador1, &treinador2, qtdFugitivos);

    /* Emitir o relatorio e uma operacao que a especificacao exige: nao
       conseguir grava-lo e falha da execucao. */
    relatorioGravado = pokecenterGerarRelatorio(&centro, ARQ_RELATORIO);
    if (!relatorioGravado) {
        printf("\nErro: nao foi possivel gravar o relatorio em %s.\n", ARQ_RELATORIO);
    }

    /* Tudo volta: as duas listas do Centro, a lista de cada treinador, e as
       quatro celulas cabeca. */
    treinadorLiberar(&treinador1);
    treinadorLiberar(&treinador2);
    pokecenterLiberar(&centro);

    return relatorioGravado;
}

int missaoExecutarPorArquivo(const char *nomeArquivo)
{
    FILE *entrada;
    int resultado;

    entrada = fopen(nomeArquivo, "r");
    if (entrada == NULL) {
        printf("Erro: nao foi possivel abrir o arquivo \"%s\".\n", nomeArquivo);
        return 0;
    }

    pulaMarcaUtf8(entrada);

    resultado = executa(entrada, 0);

    fclose(entrada);

    return resultado;
}

int missaoExecutarInterativo(void)
{
    /* Mesma funcao do modo por arquivo, com stdin no lugar do arquivo. */
    return executa(stdin, 1);
}

void missaoMenu(void)
{
    char caminho[TAM_CAMINHO];
    int opcao;

    while (1) {
        imprimeMoldura("TP1 - RESGATE DE POKÉMON", INDENT_MENU);
        printf("\n%d - Executar a missão a partir de um arquivo\n", OPCAO_ARQUIVO);
        printf("%d - Executar a missão no modo interativo\n", OPCAO_INTERATIVO);
        printf("%d - Sair\n\n", OPCAO_SAIR);
        printf("Opção: ");

        /* Falha aqui significa fim da entrada ou algo que nao e numero. */
        if (scanf("%d", &opcao) != 1) {
            printf("\nEntrada encerrada.\n");
            return;
        }

        switch (opcao) {
        case OPCAO_SAIR:
            printf("\nAté a próxima!\n");
            return;

        case OPCAO_ARQUIVO:
            printf("Caminho do arquivo de entrada: ");
            if (scanf(FMT_CAMINHO, caminho) != 1) {
                printf("\nErro: nao foi possivel ler o caminho do arquivo.\n");
                return;
            }
            printf("\n");
            missaoExecutarPorArquivo(caminho);
            printf("\n");
            break;

        case OPCAO_INTERATIVO:
            printf("\n");
            missaoExecutarInterativo();
            printf("\n");
            break;

        default:
            printf("\nOpção inválida.\n\n");
            break;
        }
    }
}
