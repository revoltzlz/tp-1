#include <stdio.h>
#include <string.h>

#include "treinador.h"

int treinadorInicializar(Treinador *t, int identificador, const char *nome, int qntdpokebolas)
{
    treinadorSetId(t, identificador);
    treinadorSetNome(t, nome);
    treinadorSetPokebolas(t, qntdpokebolas);

    treinadorSetLocalizacao(t, TREINADOR_X_INICIAL, TREINADOR_Y_INICIAL);

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

Coordenada treinadorGetLocalizacao(const Treinador *t)
{
    return t->loccoach;
}

int treinadorGetPokebolas(const Treinador *t)
{
    return t->qntdpokebolas;
}

void treinadorMovimentar(Treinador *t, int cordX, int cordY)
{
    // as mensagens ficam na missao, que usa isto para ir e para voltar.
    treinadorSetLocalizacao(t, cordX, cordY);
}

int treinadorCapturar(Treinador *t, const Pokemon *p)
{
    // sem pokebola nao ha captura.
    if (t->qntdpokebolas <= 0) {
        return 0;
    }

    // so gasta a pokebola se a insercao deu certo.
    if (!pokelistaInserir(&t->lista, p)) {
        return 0;
    }

    t->qntdpokebolas--;

    return 1;
}

int treinadorRetirarPokemon(Treinador *t, Pokemon *retirado)
{
    // o primeiro da lista e o mais antigo: a entrega sai na ordem de captura.
    return pokelistaRemoverPrimeiro(&t->lista, retirado);
}

void treinadorImprimir(const Treinador *t)
{
    Coordenada posicao = treinadorGetLocalizacao(t);

    printf("Treinador(a) %s: posicao (%d,%d) | Pokebolas: %d\n",
        treinadorGetNome(t), posicao.cordX, posicao.cordY,
        treinadorGetPokebolas(t));
}

void treinadorLiberar(Treinador *t)
{
    pokelistaLiberar(&t->lista);
}
