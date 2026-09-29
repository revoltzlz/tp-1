#include <stdio.h>
#include <string.h>

#include "treinador.h"

int treinadorInicializar(Treinador *t, int identificador, const char *nome,
                         int qntdpokebolas)
{
    treinadorSetId(t, identificador);
    treinadorSetNome(t, nome);
    treinadorSetPokebolas(t, qntdpokebolas);

    /* A especificacao fixa a posicao inicial do treinador em (0,0), que e
       tambem a posicao do Centro de Pesquisa. */
    treinadorSetLocalizacao(t, TREINADOR_X_INICIAL, TREINADOR_Y_INICIAL);

    /* A PokeLista do treinador comeca vazia. Se a celula cabeca dela nao puder
       ser alocada, o treinador nao esta utilizavel e a falha sobe. */
    return pokelistaInicializar(&t->lista);
}

void treinadorSetId(Treinador *t, int identificador)
{
    t->identificador = identificador;
}

void treinadorSetNome(Treinador *t, const char *nome)
{
    strncpy(t->nome, nome, TAM_NOME_COACH - 1);
    t->nome[TAM_NOME_COACH - 1] = '\0';
}

void treinadorSetLocalizacao(Treinador *t, int cordX, int cordY)
{
    t->loccoach.cordX = cordX;
    t->loccoach.cordY = cordY;
}

void treinadorSetPokebolas(Treinador *t, int qntdpokebolas)
{
    t->qntdpokebolas = qntdpokebolas;
}

int treinadorGetId(const Treinador *t)
{
    return t->identificador;
}

const char *treinadorGetNome(const Treinador *t)
{
    return t->nome;
}

cord treinadorGetLocalizacao(const Treinador *t)
{
    return t->loccoach;
}

int treinadorGetPokebolas(const Treinador *t)
{
    return t->qntdpokebolas;
}

void treinadorMovimentar(Treinador *t, int cordX, int cordY)
{
    /* A movimentacao nao imprime nada: quem narra a missao e o modulo da
       missao, que usa esta mesma operacao tanto para ir ate o Pokemon como
       para voltar ao Centro, dois momentos com mensagens diferentes. */
    treinadorSetLocalizacao(t, cordX, cordY);
}

int treinadorCapturar(Treinador *t, const Pokemon *p)
{
    /* Sem Pokebola nao ha captura. A checagem fica aqui, dentro do TAD, para
       que a quantidade de Pokebolas nunca possa ficar negativa, mesmo que o
       programa principal chame a captura fora de hora. */
    if (t->qntdpokebolas <= 0) {
        return 0;
    }

    /* A lista guarda uma copia do Pokemon, entao a captura nao pode falhar
       pela metade: so gasta a Pokebola se a insercao deu certo. */
    if (!pokelistaInserir(&t->lista, p)) {
        return 0;
    }

    t->qntdpokebolas--;

    return 1;
}

int treinadorRetirarPokemon(Treinador *t, Pokemon *retirado)
{
    /* Retira sempre o primeiro da lista, de modo que os Pokemon saiam na mesma
       ordem em que foram capturados. */
    return pokelistaRemoverPrimeiro(&t->lista, retirado);
}

void treinadorImprimir(const Treinador *t)
{
    printf("Treinador(a) %s: posição (%d,%d) | Pokébolas: %d\n",
           t->nome, t->loccoach.cordX, t->loccoach.cordY, t->qntdpokebolas);
}

void treinadorLiberar(Treinador *t)
{
    pokelistaLiberar(&t->lista);
}
