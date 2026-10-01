// sistema de controle da missao: leitura da entrada, laco do resgate,
// retornos ao centro e relatorio

#include <math.h>
#include <stdio.h>

#include "missao.h"

// devolve o quadrado da distancia, sem tirar a raiz: a raiz e crescente,
// entao a ordem nao muda. o float guarda inteiros exatos ate 2^24, entao em
// mapas de ate uns 2000 x 2000 o quadrado sai exato e o empate que a
// especificacao pede e detectado com seguranca
float distanciaQuadrado(Coordenada a, Coordenada b)
{
    float dx = (float) a.cordX - b.cordX;
    float dy = (float) a.cordY - b.cordY;

    return dx * dx + dy * dy;
}

// o mais proximo do alvo e, em caso de empate, o de menor identificador
Treinador *escolheTreinador(Treinador *t1, Treinador *t2, float distancia1, float distancia2)
{
    if (distancia1 < distancia2) {
        return t1;
    }
    if (distancia2 < distancia1) {
        return t2;
    }

    // empate: a missao vai para o treinador de menor identificador
    if (treinadorGetId(t1) <= treinadorGetId(t2)) {
        return t1;
    }
    return t2;
}

// le nome e pokebolas de um treinador e o inicializa
int leTreinador(FILE *entrada, Treinador *t, int id)
{
    // valores iniciais: se o fscanf falhar, nada fica com lixo de memoria
    char nome[TAM_NOME_COACH] = "";
    int pokebolas = 0;

    // o 29 (tam_nome_coach - 1) impede que um nome longo passe do vetor
    fscanf(entrada, "%29s", nome);
    fscanf(entrada, "%d", &pokebolas);

    treinadorInicializar(t, id, nome, pokebolas);

    return 1;
}

// le os n fugitivos e registra cada um no centro. a quantidade sai em
// *quantidade. devolve 0 se faltar memoria para registrar um pokemon
int leFugitivos(FILE *entrada, PokeCenter *cp, int *quantidade)
{
    // valores iniciais: se o fscanf falhar, nada fica com lixo de memoria
    char nome[TAM_NOME] = "";
    char tipo[TAM_TIPO] = "";
    int total = 0, i, numPokedex = 0, cordX = 0, cordY = 0;
    Pokemon p;

    fscanf(entrada, "%d", &total);

    for (i = 0; i < total; i++) {
        // o %s para em espaco em branco, e o '\r' do fim de linha do windows
        // conta como tal: nome e tipo nunca o recebem. o 29 e o 19
        // (tam_nome - 1 e tam_tipo - 1) impedem que passem do vetor
        fscanf(entrada, "%d %29s %19s %d %d", &numPokedex, nome, tipo, &cordX, &cordY);

        // o id e a ordem de leitura. o numero da pokedex nao serviria: o
        // arquivo de teste oficial traz quatro pikachus com o numero 25
        pokemonInicializar(&p, i + 1, numPokedex, nome, tipo, cordX, cordY);

        if (!pokecenterRegistrarFugitivo(cp, &p)) {
            printf("Erro: memoria insuficiente para registrar o Pokemon %s.\n", nome);
            return 0;
        }
    }

    *quantidade = total;

    return 1;
}

// pede o carregamento ao centro e anuncia quantas pokebolas ele recebeu
void recarregaNoCentro(PokeCenter *cp, Treinador *t)
{
    int recebidas;

    recebidas = pokecenterRecarregarPokebolas(cp, t);
    printf("Treinador(a) %s recebeu %d Pokebolas.\n\n", treinadorGetNome(t), recebidas);
}

// volta ao centro, entrega tudo e pede um novo carregamento
void retornaAoCentro(PokeCenter *cp, Treinador *t)
{
    Coordenada posicaoCentro;

    posicaoCentro = pokecenterGetLocalizacao(cp);

    printf("========================================\n");
    printf("        Treinador(a) %s SEM POKEBOLAS   \n", treinadorGetNome(t));
    printf("========================================\n");

    printf("Treinador(a) %s retorna ao Centro de Pesquisa.\n\n", treinadorGetNome(t));
    treinadorMovimentar(t, posicaoCentro.cordX, posicaoCentro.cordY);

    printf("Entregando Pokemon ao Centro de Pesquisa.\n\n");
    pokecenterReceberPokemon(cp, t);

    recarregaNoCentro(cp, t);
}

// ninguem parte para uma captura sem pokebola, e os dois comecam no centro:
// quem tem zero recarrega antes do primeiro resgate
void recarregaSeComecouSemPokebola(PokeCenter *cp, Treinador *t)
{
    if (treinadorGetPokebolas(t) > 0) {
        return;
    }

    printf("========================================\n");
    printf("        Treinador(a) %s SEM POKEBOLAS   \n", treinadorGetNome(t));
    printf("========================================\n");
    recarregaNoCentro(cp, t);
}

// um resgate completo: distancias, escolha, movimento, captura e aviso ao
// centro. devolve o treinador que capturou, ou null se a captura falhou
Treinador *resgataPokemon(PokeCenter *cp, Treinador *t1, Treinador *t2, const Pokemon *alvo)
{
    Coordenada posicaoAlvo;
    float distancia1, distancia2;
    Treinador *escolhido;

    posicaoAlvo = pokemonGetLocalizacao(alvo);

    printf("----------------------------------------\n\n");

    printf("Pokemon alvo: %s\n", pokemonGetNome(alvo));
    printf("Localizacao: (%d,%d)\n\n", posicaoAlvo.cordX, posicaoAlvo.cordY);

    distancia1 = distanciaQuadrado(treinadorGetLocalizacao(t1), posicaoAlvo);
    distancia2 = distanciaQuadrado(treinadorGetLocalizacao(t2), posicaoAlvo);

    // a raiz aparece so aqui, na impressao
    printf("Distancia Treinador(a) %s: %.2f\n", treinadorGetNome(t1), sqrt((double) distancia1));
    printf("Distancia Treinador(a) %s: %.2f\n\n", treinadorGetNome(t2), sqrt((double) distancia2));

    escolhido = escolheTreinador(t1, t2, distancia1, distancia2);
    printf("Missao atribuida ao Treinador(a) %s.\n\n", treinadorGetNome(escolhido));

    treinadorMovimentar(escolhido, posicaoAlvo.cordX, posicaoAlvo.cordY);
    printf("Treinador(a) %s se movimentou para (%d,%d).\n", treinadorGetNome(escolhido), posicaoAlvo.cordX, posicaoAlvo.cordY);

    if (!treinadorCapturar(escolhido, alvo)) {
        // o fluxo da missao evita isto. o pokemon continua fugitivo
        printf("%s escapou: Treinador(a) %s esta sem Pokebolas!\n\n", pokemonGetNome(alvo), treinadorGetNome(escolhido));
        return NULL;
    }

    printf("%s capturado com sucesso!\n\n", pokemonGetNome(alvo));

    // a cada captura o centro atualiza a listagem de fugas
    pokecenterRemoverFugitivo(cp, pokemonGetId(alvo), NULL);

    printf("Pokebolas restantes para o Treinador(a) %s: %d\n\n", treinadorGetNome(escolhido), treinadorGetPokebolas(escolhido));

    return escolhido;
}

// os dois voltam ao centro e devolvem o que carregam, na ordem do
// identificador
void encerraMissao(PokeCenter *cp, Treinador *t1, Treinador *t2)
{
    Coordenada posicaoCentro;

    posicaoCentro = pokecenterGetLocalizacao(cp);

    // se alguma captura falhou, mostra quem ficou para tras
    if (pokecenterTemFugitivos(cp)) {
        printf("Atencao: ainda ha %d Pokemon na lista de fugitivos:\n", pokecenterGetQtdFugitivos(cp));
        pokecenterImprimirFugitivos(cp);
        printf("========================================\n");
        printf("        Fim da missao de resgate        \n");
        printf("========================================\n");
    } else {
        printf("========================================\n");
        printf("        Todos Pokemons foram resgatados \n");
        printf("========================================\n");
    }

    printf("\nAmbos treinadores retornam ao Centro de Pesquisa.\n\n");
    treinadorMovimentar(t1, posicaoCentro.cordX, posicaoCentro.cordY);
    treinadorMovimentar(t2, posicaoCentro.cordX, posicaoCentro.cordY);

    // t1 tem o menor identificador, entao entrega primeiro: e essa ordem que
    // faz o relatorio sair na ordem do exemplo
    printf("Treinador(a) %s devolve os Pokemon.\n\n", treinadorGetNome(t1));
    pokecenterReceberPokemon(cp, t1);

    printf("Treinador(a) %s devolve os Pokemon.\n\n", treinadorGetNome(t2));
    pokecenterReceberPokemon(cp, t2);

    printf("========================================\n");
    printf("        MISSAO CONCLUIDA                \n");
    printf("========================================\n");
}

// o laco da missao, sobre dados ja registrados
void executaMissao(PokeCenter *cp, Treinador *t1, Treinador *t2, int qtdFugitivos)
{
    Pokemon alvo;
    Treinador *escolhido;
    int id;

    printf("========================================\n");
    printf("        INICIO DA MISSAO                \n");
    printf("========================================\n\n");

    treinadorImprimir(t1);
    treinadorImprimir(t2);
    printf("\nPokemons fugitivos a serem resgatados: %d\n\n", pokecenterGetQtdFugitivos(cp));

    // se nao ha nenhum fugitivo, ninguem precisa de pokebola
    if (qtdFugitivos > 0) {
        recarregaSeComecouSemPokebola(cp, t1);
        recarregaSeComecouSemPokebola(cp, t2);
    }

    // os ids sao a ordem de leitura. a busca confirma que o pokemon ainda
    // esta fugido antes de montar o resgate
    for (id = 1; id <= qtdFugitivos; id++) {
        if (!pokecenterBuscarFugitivo(cp, id, &alvo)) {
            continue;
        }

        escolhido = resgataPokemon(cp, t1, t2, &alvo);
        if (escolhido == NULL) {
            continue;
        }

        // a ordem destes dois testes importa: se o pokemon capturado era o
        // ultimo, a missao termina sem recarga, mesmo que o treinador tenha
        // ficado sem pokebolas. e o que o exemplo da especificacao mostra
        if (!pokecenterTemFugitivos(cp)) {
            break;
        }

        if (treinadorGetPokebolas(escolhido) == 0) {
            retornaAoCentro(cp, escolhido);
        }
    }

    encerraMissao(cp, t1, t2);
}

// le a entrada, roda a missao, emite o relatorio e libera tudo
int executa(FILE *entrada)
{
    PokeCenter centro;
    Treinador treinador1;
    Treinador treinador2;
    int qtdFugitivos = 0;
    int relatorioGravado;

    pokecenterInicializar(&centro);

    leTreinador(entrada, &treinador1, ID_TREINADOR_1);

    leTreinador(entrada, &treinador2, ID_TREINADOR_2);

    leFugitivos(entrada, &centro, &qtdFugitivos);

    executaMissao(&centro, &treinador1, &treinador2, qtdFugitivos);

    // nao conseguir gravar o relatorio conta como falha
    relatorioGravado = pokecenterGerarRelatorio(&centro, ARQ_RELATORIO);
    if (!relatorioGravado) {
        printf("\nErro: nao foi possivel gravar o relatorio em %s.\n", ARQ_RELATORIO);
    }

    treinadorLiberar(&treinador1);
    treinadorLiberar(&treinador2);
    pokecenterLiberar(&centro);

    return relatorioGravado;
}

int missaoCaptura()
{
    FILE *entrada;
    int resultado;
    char caminho[TAM_CAMINHO] = "";

    printf("Caminho do arquivo de entrada: ");
    scanf("%255s", caminho);

    entrada = fopen(caminho, "r");
    if (entrada == NULL) {
        printf("Erro: nao foi possivel abrir o arquivo \"%s\".\n", caminho);
        return 0;
    }

    resultado = executa(entrada);

    fclose(entrada);

    return resultado;
}