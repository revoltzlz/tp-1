#include <stdio.h>
#include <string.h>

#include "treinador.h"

int treinadorInicializar(Treinador *t, int identificador, const char *nome,
                         int qntdpokebolas)
{
    treinadorSetId(t, identificador);
    treinadorSetNome(t, nome);
    treinadorSetPokebolas(t, qntdpokebolas);

    /* A especificacao fixa a posicao inicial em (0,0). */
    treinadorSetLocalizacao(t, TREINADOR_X_INICIAL, TREINADOR_Y_INICIAL);

    /* Se a celula cabeca da lista nao puder ser alocada, o treinador nao esta
       utilizavel e a falha sobe. */
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
    /* Nao imprime nada: o modulo da missao usa esta operacao para ir ate o
       Pokemon e para voltar ao Centro, com mensagens diferentes. */
    treinadorSetLocalizacao(t, cordX, cordY);
}

int treinadorCapturar(Treinador *t, const Pokemon *p)
{
    /* A checagem fica no TAD para a quantidade nunca ficar negativa, mesmo se
       a captura for chamada fora de hora. */
    if (t->qntdpokebolas <= 0) {
        return 0;
    }

    /* So gasta a Pokebola se a insercao deu certo, para o estado nao ficar
       inconsistente. */
    if (!pokelistaInserir(&t->lista, p)) {
        return 0;
    }

    t->qntdpokebolas--;

    return 1;
}

int treinadorRetirarPokemon(Treinador *t, Pokemon *retirado)
{
    /* O primeiro da lista e o mais antigo: a entrega sai na ordem de captura. */
    return pokelistaRemoverPrimeiro(&t->lista, retirado);
}

void treinadorImprimir(const Treinador *t)
{
    cord posicao = treinadorGetLocalizacao(t);

    /* O formato e o do exemplo da especificacao. */
    printf("Treinador(a) %s: posição (%d,%d) | Pokébolas: %d\n",
           treinadorGetNome(t), posicao.cordX, posicao.cordY,
           treinadorGetPokebolas(t));
}

void treinadorLiberar(Treinador *t)
{
    pokelistaLiberar(&t->lista);
}
