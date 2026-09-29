#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "missao.h"

/* ------------------------------------------------------------------------- *
 * Funcoes auxiliares de impressao
 * ------------------------------------------------------------------------- */

/* Imprime o caractere indicado o numero de vezes pedido. Serve para as linhas
   de "=" e de "-" e para os espacos de indentacao dos titulos. */
static void imprimeRepetido(char caractere, int vezes)
{
    int i;

    for (i = 0; i < vezes; i++) {
        putchar(caractere);
    }
}

/* Imprime a linha de "-" que separa o resgate de um Pokemon do seguinte. */
static void imprimeSeparador(void)
{
    imprimeRepetido('-', LARGURA_MOLDURA);
    putchar('\n');
}

/* Imprime uma moldura de destaque: uma linha de "=", o titulo recuado, uma
   linha em branco e outra linha de "=". */
static void imprimeMoldura(const char *titulo, int indentacao)
{
    imprimeRepetido('=', LARGURA_MOLDURA);
    putchar('\n');
    imprimeRepetido(' ', indentacao);
    printf("%s\n\n", titulo);
    imprimeRepetido('=', LARGURA_MOLDURA);
    putchar('\n');
}

/* ------------------------------------------------------------------------- *
 * Funcoes auxiliares de calculo
 * ------------------------------------------------------------------------- */

/* Devolve o QUADRADO da distancia euclidiana entre duas coordenadas.

   A comparacao entre as distancias dos dois treinadores e feita com este valor,
   e nao com a raiz quadrada, por dois motivos: o quadrado e um inteiro exato,
   entao o empate exigido pela especificacao e detectado com seguranca, sem
   comparar numeros de ponto flutuante com ==; e a raiz e crescente, entao a
   ordem entre as duas distancias e a mesma dos seus quadrados.

   A conta e feita em long long porque o resultado nao cabe em int: com as
   coordenadas no limite do mapa, a soma chega a 8 x 10^12, e o maior int vale
   cerca de 2,1 x 10^9. O long long guarda ate cerca de 9,2 x 10^18, entao a
   folga e grande - mas ela so existe porque a leitura recusa coordenadas fora
   de [COORD_MIN, COORD_MAX]. A justificativa completa esta em
   include/coordenadas.h. */
static long long distanciaQuadrado(cord a, cord b)
{
    long long dx = (long long) a.cordX - b.cordX;
    long long dy = (long long) a.cordY - b.cordY;

    return dx * dx + dy * dy;
}

/* Escolhe o treinador que fara a captura: o mais proximo do alvo e, em caso de
   empate, o de menor identificador. */
static Treinador *escolheTreinador(Treinador *t1, Treinador *t2,
                                   long long distancia1, long long distancia2)
{
    if (distancia1 < distancia2) {
        return t1;
    }
    if (distancia2 < distancia1) {
        return t2;
    }

    /* Empate: a missao vai para o treinador de menor identificador. */
    return (treinadorGetId(t1) <= treinadorGetId(t2)) ? t1 : t2;
}

/* ------------------------------------------------------------------------- *
 * Leitura dos dados
 * ------------------------------------------------------------------------- */

/* Remove do fim do texto a quebra de linha e o retorno de carro deixados pelo
   fgets, inclusive no caso de um arquivo com fim de linha do Windows. */
static void removeFimDeLinha(char *texto)
{
    size_t tamanho = strlen(texto);

    while (tamanho > 0 && (texto[tamanho - 1] == '\n' || texto[tamanho - 1] == '\r')) {
        texto[tamanho - 1] = '\0';
        tamanho--;
    }
}

/* Pula a marca de ordem de bytes (BOM) no comeco de um arquivo UTF-8. Sao
   tres bytes invisiveis, EF BB BF, que o Bloco de Notas e outros editores do
   Windows escrevem por padrao. Sem pular, eles entrariam colados no nome do
   primeiro treinador, que apareceria com lixo na frente.

   Se os tres primeiros bytes nao forem a marca, o rewind devolve a leitura ao
   comeco do arquivo. Por isso esta funcao so e usada no modo por arquivo: em
   uma entrada vinda do teclado nao ha como voltar atras. */
static void pulaMarcaUtf8(FILE *entrada)
{
    unsigned char marca[3];

    if (fread(marca, 1, sizeof marca, entrada) == sizeof marca &&
        marca[0] == 0xEF && marca[1] == 0xBB && marca[2] == 0xBF) {
        return;
    }

    rewind(entrada);
}

/* Descarta o que sobrou da linha atual da entrada, inclusive a quebra de
   linha. Usada depois do modo interativo: o fscanf para antes da quebra de
   linha do ultimo dado, e sem isso o fgets do menu leria essa sobra como se
   fosse a opcao digitada, reclamando de uma opcao invalida que ninguem
   digitou. */
static void descartaRestoDaLinha(FILE *entrada)
{
    int caractere;

    do {
        caractere = fgetc(entrada);
    } while (caractere != '\n' && caractere != EOF);
}

/* Le o nome e a quantidade inicial de Pokebolas de um treinador e o
   inicializa com o identificador recebido. No modo interativo, pede cada dado
   antes de ler. Devolve 1 em caso de sucesso e 0 se os dados forem invalidos
   ou a PokeLista do treinador nao puder ser criada. */
static int leTreinador(FILE *entrada, int interativo, Treinador *t, int id)
{
    char nome[TAM_NOME_COACH];
    int pokebolas;

    if (interativo) {
        printf("Nome do treinador %d: ", id);
    }
    /* A largura do FMT_NOME_COACH impede que um nome maior que o vetor escreva
       fora dele. */
    if (fscanf(entrada, FMT_NOME_COACH, nome) != 1) {
        fprintf(stderr, "Erro: nao foi possivel ler o nome do treinador %d.\n", id);
        return 0;
    }

    if (interativo) {
        printf("Pokebolas iniciais de %s: ", nome);
    }
    if (fscanf(entrada, "%d", &pokebolas) != 1) {
        fprintf(stderr,
                "Erro: nao foi possivel ler a quantidade de Pokebolas de %s.\n", nome);
        return 0;
    }
    /* O teto existe para recusar lixo: um numero grande demais para caber em
       um int e lido pelo %d com o valor truncado, e sem esta checagem entraria
       no programa como se fosse valido. */
    if (pokebolas < 0 || pokebolas > MAX_QUANTIDADE) {
        fprintf(stderr,
                "Erro: quantidade de Pokebolas invalida para %s (%d). "
                "O valor precisa estar entre 0 e %d.\n",
                nome, pokebolas, MAX_QUANTIDADE);
        return 0;
    }

    if (!treinadorInicializar(t, id, nome, pokebolas)) {
        fprintf(stderr,
                "Erro: memoria insuficiente para criar o treinador %s.\n", nome);
        return 0;
    }

    return 1;
}

/* Le a quantidade de Pokemon fugitivos e os dados de cada um, registrando-os
   no Centro de Pesquisa. Devolve 1 em caso de sucesso e 0 se os dados forem
   invalidos ou faltarem linhas. A quantidade lida sai em *quantidade. */
static int leFugitivos(FILE *entrada, int interativo, PokeCenter *cp, int *quantidade)
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
        fprintf(stderr,
                "Erro: nao foi possivel ler a quantidade de Pokemon fugitivos.\n");
        return 0;
    }
    if (total < 0 || total > MAX_QUANTIDADE) {
        fprintf(stderr,
                "Erro: quantidade de Pokemon fugitivos invalida (%d). "
                "O valor precisa estar entre 0 e %d.\n",
                total, MAX_QUANTIDADE);
        return 0;
    }

    for (i = 0; i < total; i++) {
        if (interativo) {
            printf("Pokemon %d de %d (numero na Pokedex, nome, tipo, X e Y): ",
                   i + 1, total);
        }

        /* O %s do fscanf para em qualquer espaco em branco, e o '\r' do fim de
           linha do Windows conta como espaco em branco. Por isso o nome e o
           tipo nunca recebem o '\r' dos arquivos gerados no Windows. */
        if (fscanf(entrada, "%d " FMT_NOME " " FMT_TIPO " %d %d",
                   &numPokedex, nome, tipo, &cordX, &cordY) != 5) {
            fprintf(stderr, "Erro: dados incompletos do Pokemon %d de %d. "
                            "Cada linha precisa ter numero na Pokedex, nome, tipo, X e Y.\n",
                    i + 1, total);
            return 0;
        }

        if (numPokedex < 0 || numPokedex > MAX_QUANTIDADE) {
            fprintf(stderr,
                    "Erro: numero na Pokedex invalido para o Pokemon %s (%d).\n",
                    nome, numPokedex);
            return 0;
        }

        /* As coordenadas precisam caber no mapa. Alem de ser uma validacao de
           dados, e o que garante que o calculo da distancia nao estoure: ver
           a explicacao em include/coordenadas.h. */
        if (cordX < COORD_MIN || cordX > COORD_MAX ||
            cordY < COORD_MIN || cordY > COORD_MAX) {
            fprintf(stderr,
                    "Erro: o Pokemon %s esta em (%d,%d), fora do mapa, "
                    "que vai de %d a %d nos dois eixos.\n",
                    nome, cordX, cordY, COORD_MIN, COORD_MAX);
            return 0;
        }

        /* O Id e a ordem de leitura, comecando em 1. A especificacao exige que
           ele seja unico, e o numero da Pokedex nao serve: o proprio arquivo de
           teste oficial traz quatro Pikachus com o numero 25. */
        pokemonInicializar(&p, i + 1, numPokedex, nome, tipo, cordX, cordY);

        if (!pokecenterRegistrarFugitivo(cp, &p)) {
            fprintf(stderr,
                    "Erro: memoria insuficiente para registrar o Pokemon %s.\n", nome);
            return 0;
        }
    }

    *quantidade = total;

    return 1;
}

/* ------------------------------------------------------------------------- *
 * Execucao da missao
 * ------------------------------------------------------------------------- */

/* Imprime a moldura de destaque que anuncia um treinador sem Pokebolas. O
   titulo e montado em um vetor porque inclui o nome do treinador. */
static void imprimeMolduraSemPokebolas(const Treinador *t)
{
    char titulo[TAM_TITULO];

    snprintf(titulo, sizeof titulo, "Treinador(a) %s SEM POKÉBOLAS",
             treinadorGetNome(t));
    imprimeMoldura(titulo, INDENT_SEM_POKEBOLAS);
    putchar('\n');
}

/* Pede ao Centro um novo carregamento de Pokebolas para o treinador e anuncia
   quantas ele recebeu. */
static void recarregaNoCentro(PokeCenter *cp, Treinador *t)
{
    int recebidas = pokecenterRecarregarPokebolas(cp, t);

    printf("Treinador(a) %s recebeu %d Pokébolas.\n\n", treinadorGetNome(t), recebidas);
}

/* Leva o treinador de volta ao Centro, entrega tudo o que ele capturou e pede
   um novo carregamento de Pokebolas. */
static void retornaAoCentro(PokeCenter *cp, Treinador *t)
{
    cord posicaoCentro = pokecenterGetLocalizacao(cp);

    imprimeMolduraSemPokebolas(t);

    printf("Treinador(a) %s retorna ao Centro de Pesquisa.\n\n", treinadorGetNome(t));
    treinadorMovimentar(t, posicaoCentro.cordX, posicaoCentro.cordY);

    printf("Entregando Pokémon ao Centro de Pesquisa.\n\n");
    pokecenterReceberPokemon(cp, t);

    recarregaNoCentro(cp, t);
}

/* Trata o treinador que comecou a missao sem nenhuma Pokebola. Como ninguem
   parte para uma captura sem Pokebola e os dois treinadores comecam no Centro,
   ele e recarregado antes do primeiro resgate. Nao precisa entregar nada nem
   se movimentar: ele ainda nao capturou nada e ja esta no Centro. */
static void recarregaSeComecouSemPokebola(PokeCenter *cp, Treinador *t)
{
    if (treinadorGetPokebolas(t) > 0) {
        return;
    }

    imprimeMolduraSemPokebolas(t);
    recarregaNoCentro(cp, t);
}

/* Faz o resgate de um Pokemon: calcula a distancia de cada treinador ate ele,
   envia o mais proximo, movimenta, captura e avisa o Centro de Pesquisa.
   Devolve o treinador que fez a captura, ou NULL se a captura falhou. */
static Treinador *resgataPokemon(PokeCenter *cp, Treinador *t1, Treinador *t2,
                                 const Pokemon *alvo)
{
    cord posicaoAlvo = pokemonGetLocalizacao(alvo);
    long long distancia1;
    long long distancia2;
    Treinador *escolhido;

    imprimeSeparador();
    printf("Pokémon alvo: %s\n", pokemonGetNome(alvo));
    printf("Localização: (%d,%d)\n\n", posicaoAlvo.cordX, posicaoAlvo.cordY);

    distancia1 = distanciaQuadrado(treinadorGetLocalizacao(t1), posicaoAlvo);
    distancia2 = distanciaQuadrado(treinadorGetLocalizacao(t2), posicaoAlvo);

    /* A raiz quadrada aparece somente na impressao. */
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
        /* So acontece se o treinador escolhido estiver sem Pokebolas, o que o
           fluxo da missao evita. O Pokemon continua na lista de fugitivos. */
        printf("%s escapou: Treinador(a) %s está sem Pokébolas!\n\n",
               pokemonGetNome(alvo), treinadorGetNome(escolhido));
        return NULL;
    }

    printf("%s capturado com sucesso!\n\n", pokemonGetNome(alvo));

    /* A cada captura o treinador informa o Centro, que atualiza a listagem de
       fugas. */
    pokecenterRemoverFugitivo(cp, pokemonGetId(alvo), NULL);

    printf("Pokébolas restantes para o Treinador(a) %s: %d\n\n",
           treinadorGetNome(escolhido), treinadorGetPokebolas(escolhido));

    return escolhido;
}

/* Fecha a missao: os dois treinadores voltam ao Centro e devolvem tudo o que
   ainda estao carregando, na ordem do identificador. */
static void encerraMissao(PokeCenter *cp, Treinador *t1, Treinador *t2)
{
    cord posicaoCentro = pokecenterGetLocalizacao(cp);
    const char *titulo;

    /* Caso anomalo: se alguma captura falhou, mostra quem ficou para tras em
       vez de anunciar que todos foram resgatados. */
    if (pokecenterTemFugitivos(cp)) {
        printf("Atenção: ainda há %d Pokémon na lista de fugitivos:\n",
               pokecenterGetQtdFugitivos(cp));
        pokecenterImprimirFugitivos(cp);
        putchar('\n');
        titulo = "Fim da missão de resgate";
    } else {
        titulo = "Todos Pokemons foram resgatados";
    }

    imprimeMoldura(titulo, INDENT_RESGATADOS);
    putchar('\n');

    printf("Ambos treinadores retornam ao Centro de Pesquisa.\n\n");
    treinadorMovimentar(t1, posicaoCentro.cordX, posicaoCentro.cordY);
    treinadorMovimentar(t2, posicaoCentro.cordX, posicaoCentro.cordY);

    /* t1 foi inicializado com o menor identificador, entao entrega primeiro.
       E essa ordem que faz o relatorio sair na ordem do exemplo. */
    printf("Treinador(a) %s devolve os Pokémon.\n\n", treinadorGetNome(t1));
    pokecenterReceberPokemon(cp, t1);

    printf("Treinador(a) %s devolve os Pokémon.\n\n", treinadorGetNome(t2));
    pokecenterReceberPokemon(cp, t2);

    imprimeMoldura("MISSÃO CONCLUÍDA", INDENT_CONCLUIDA);
}

/* Roda a missao completa sobre dados ja registrados: estado inicial, resgate
   de cada fugitivo na ordem do arquivo, retornos ao Centro e encerramento. */
static void executaMissao(PokeCenter *cp, Treinador *t1, Treinador *t2, int qtdFugitivos)
{
    Pokemon alvo;
    Treinador *escolhido;
    int id;

    imprimeMoldura("INÍCIO DA MISSÃO", INDENT_INICIO);
    putchar('\n');

    treinadorImprimir(t1);
    treinadorImprimir(t2);
    printf("\nPokémons fugitivos a serem resgatados: %d\n\n",
           pokecenterGetQtdFugitivos(cp));

    /* Se nao ha nenhum fugitivo, ninguem precisa de Pokebola. */
    if (qtdFugitivos > 0) {
        recarregaSeComecouSemPokebola(cp, t1);
        recarregaSeComecouSemPokebola(cp, t2);
    }

    /* Percorre os identificadores na ordem em que os Pokemon foram lidos. A
       busca no Centro confirma que aquele Pokemon ainda esta fugido antes de
       montar o resgate. */
    for (id = 1; id <= qtdFugitivos; id++) {
        if (!pokecenterBuscarFugitivo(cp, id, &alvo)) {
            continue;
        }

        escolhido = resgataPokemon(cp, t1, t2, &alvo);
        if (escolhido == NULL) {
            continue;
        }

        /* A ordem destes dois testes importa. Se o Pokemon capturado era o
           ultimo, a missao termina e nao ha recarga, mesmo que o treinador
           tenha ficado sem Pokebolas: e o que acontece com o ultimo resgate do
           exemplo da especificacao. */
        if (!pokecenterTemFugitivos(cp)) {
            break;
        }

        if (treinadorGetPokebolas(escolhido) == 0) {
            retornaAoCentro(cp, escolhido);
        }
    }

    encerraMissao(cp, t1, t2);
}

/* Le os dados da entrada, roda a missao, emite o relatorio e libera toda a
   memoria. E o caminho comum dos dois modos de uso: o parametro interativo
   apenas liga as mensagens que pedem cada dado. */
static int executa(FILE *entrada, int interativo)
{
    PokeCenter centro;
    Treinador treinador1;
    Treinador treinador2;
    int qtdFugitivos = 0;
    int treinadoresProntos = 0;
    int ok;

    if (!pokecenterInicializar(&centro)) {
        fprintf(stderr,
                "Erro: memoria insuficiente para criar o Centro de Pesquisa.\n");
        return 0;
    }

    /* Os treinadores sao duas variaveis, e nao um vetor, porque a especificacao
       fixa a dupla em dois e obriga o uso de lista encadeada nas colecoes.

       O contador treinadoresProntos registra quantos chegaram a ser
       inicializados. Se a leitura parar no meio, ele diz exatamente quais
       PokeLista precisam ser liberadas: liberar uma que nunca foi inicializada
       leria apontadores com lixo. */
    if (leTreinador(entrada, interativo, &treinador1, ID_TREINADOR_1)) {
        treinadoresProntos++;
        if (leTreinador(entrada, interativo, &treinador2, ID_TREINADOR_2)) {
            treinadoresProntos++;
        }
    }

    ok = (treinadoresProntos == NUM_TREINADORES) &&
         leFugitivos(entrada, interativo, &centro, &qtdFugitivos);

    if (!ok) {
        if (treinadoresProntos >= 1) {
            treinadorLiberar(&treinador1);
        }
        if (treinadoresProntos >= NUM_TREINADORES) {
            treinadorLiberar(&treinador2);
        }
        pokecenterLiberar(&centro);
        return 0;
    }

    if (interativo) {
        /* No modo interativo vale mostrar o que foi registrado, para o usuario
           conferir o que digitou antes de a missao comecar. */
        printf("\nPokémon fugitivos registrados no Centro de Pesquisa:\n");
        pokecenterImprimirFugitivos(&centro);
        putchar('\n');
    }

    executaMissao(&centro, &treinador1, &treinador2, qtdFugitivos);

    ok = pokecenterGerarRelatorio(&centro, ARQ_RELATORIO);
    if (ok) {
        /* A frase e montada assim para ficar correta com qualquer quantidade,
           inclusive 1 e 0. */
        printf("\nRelatório gravado em %s: %d Pokémon.\n",
               ARQ_RELATORIO, pokecenterGetQtdRecuperados(&centro));
    } else {
        /* Emitir o relatorio e uma das operacoes que a especificacao exige,
           entao nao conseguir grava-lo e uma falha da execucao, e nao um
           aviso: o valor de ok desce ate o codigo de saida do programa. */
        fprintf(stderr,
                "\nErro: nao foi possivel gravar o relatorio em %s.\n", ARQ_RELATORIO);
    }

    /* Toda a memoria alocada volta para o sistema: as celulas das duas listas
       do Centro e as celulas das listas dos dois treinadores, incluindo as
       quatro celulas cabeca. */
    treinadorLiberar(&treinador1);
    treinadorLiberar(&treinador2);
    pokecenterLiberar(&centro);

    return ok;
}

/* ------------------------------------------------------------------------- *
 * Interface publica
 * ------------------------------------------------------------------------- */

int missaoExecutarPorArquivo(const char *nomeArquivo)
{
    FILE *entrada;
    int resultado;

    entrada = fopen(nomeArquivo, "r");
    if (entrada == NULL) {
        fprintf(stderr,
                "Erro: nao foi possivel abrir o arquivo \"%s\".\n", nomeArquivo);
        return 0;
    }

    pulaMarcaUtf8(entrada);

    resultado = executa(entrada, 0);

    fclose(entrada);

    return resultado;
}

int missaoExecutarInterativo(void)
{
    /* Mesma funcao de leitura do modo por arquivo, com stdin no lugar do
       arquivo e as mensagens que pedem cada dado ligadas. */
    return executa(stdin, 1);
}

void missaoMenu(void)
{
    char linha[TAM_CAMINHO];
    int opcao;

    for (;;) {
        imprimeMoldura("TP1 - RESGATE DE POKÉMON", INDENT_MENU);
        printf("\n%d - Executar a missão a partir de um arquivo\n", OPCAO_ARQUIVO);
        printf("%d - Executar a missão no modo interativo\n", OPCAO_INTERATIVO);
        printf("%d - Sair\n\n", OPCAO_SAIR);
        printf("Opção: ");

        /* A opcao e lida como linha e depois convertida, para que uma letra
           digitada por engano nao deixe caractere preso na entrada e faca o
           menu girar sozinho. */
        if (fgets(linha, sizeof linha, stdin) == NULL) {
            putchar('\n');
            return;
        }
        if (sscanf(linha, "%d", &opcao) != 1) {
            printf("\nOpção inválida.\n\n");
            continue;
        }

        if (opcao == OPCAO_SAIR) {
            printf("\nAté a próxima!\n");
            return;
        }

        if (opcao == OPCAO_ARQUIVO) {
            printf("Caminho do arquivo de entrada: ");
            if (fgets(linha, sizeof linha, stdin) == NULL) {
                putchar('\n');
                return;
            }
            /* O fgets guarda a quebra de linha, que nao faz parte do caminho.
               Ler com fgets, e nao com scanf, permite caminhos com espaco. */
            removeFimDeLinha(linha);
            putchar('\n');
            missaoExecutarPorArquivo(linha);
            putchar('\n');
        } else if (opcao == OPCAO_INTERATIVO) {
            putchar('\n');
            missaoExecutarInterativo();
            /* A leitura do modo interativo usa fscanf, que deixa a quebra de
               linha do ultimo dado na entrada. */
            descartaRestoDaLinha(stdin);
            putchar('\n');
        } else {
            printf("\nOpção inválida.\n\n");
        }
    }
}
